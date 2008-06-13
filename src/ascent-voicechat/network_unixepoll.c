/*
 * OpenAscent MMORPG Server
 * Copyright (C) 2008 <http://www.openascent.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common.h"
#include "network.h"
#include "log.h"
#include <errno.h>

#if defined(USE_EPOLL) && !defined(USE_KQUEUE) && !defined(WIN32)
#include <sys/epoll.h>

#define MAXCLIENTS 100
int g_maxFd = 0;
network_socket * g_fds[MAXCLIENTS];
volatile int g_bytesSent = 0;
volatile int g_bytesRecv = 0;
volatile int g_bytesSentTotal = 0;
volatile int g_bytesRecvTotal = 0;
volatile int g_byteCounter = 0;
volatile int g_bytesSStored[10] = {0,0,0,0,0,0,0,0,0,0};
volatile int g_bytesRStored[10] = {0,0,0,0,0,0,0,0,0,0};

int g_epoll = 0;
// now we can have the actual code, rofl.
void network_mod(int fd, int flags)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));
	ev.data.fd = fd;
	ev.events = flags;
	epoll_ctl(g_epoll, EPOLL_CTL_MOD, fd, &ev);
}

int network_init()
{
	memset(g_fds, 0, sizeof(network_socket*) * MAXCLIENTS);
	g_epoll = epoll_create(MAXCLIENTS);
	if(g_epoll == -1)
	{
		log_write(ERROR, "epoll control fd could not be created. check your platform?");
		return -1;
	}

	return 0;
}

// default tcp nonblocking write handler
int default_tcp_write_handler(network_socket* s, int act)
{
	int rv;

	// no more data
	if( !s->outlen )
		return 0;

	// try to push out nonblocking data
	rv = send( s->fd, s->outbuffer, s->outlen, 0 );

	// error happened
	if( rv <= 0 )
		return -1;

	g_bytesRecv += rv;
	g_bytesRecvTotal += rv;

	// move the bytes around
	if( rv == s->outlen )
		s->outlen = 0;
	else
	{
		s->outlen -= rv;
		memmove(&s->outbuffer[rv], s->outbuffer, s->outlen);
	}

	// ok!
	return 0;
}

#define SLEEPSEC 1
#define MAXEVENTS MAXCLIENTS*2

int network_io_poll()
{
	int nevents;
	struct epoll_event events[MAXEVENTS];
	int fd;
	int evs;
	int n;
	network_socket * s;

	// check size
	if( g_maxFd == 0 )
	{
		// just sleep, to simulate the delay
		usleep(SLEEPSEC * 1000000);
		return 0;
	}

	// use our blocking epoll()
	nevents = epoll_wait(g_epoll, events, MAXEVENTS, 1000);

	if( nevents < 0 )
	{
		// some error occured.
		log_write(ERROR, "FATAL: epoll_wait() returned %d", nevents);
		return -1;
	}

	// no events
	if( nevents == 0 )
		return 0;

	// loop each each socket and handle events on each of them
	for( n = 0; n < nevents; ++n )
	{
		fd = events[n].data.fd;
		evs = events[n].events;
		s = g_fds[fd];
		if( s != NULL )
		{
			if( evs & EPOLLHUP || evs & EPOLLERR )
			{
				if( s->event_handler(s, IOEVENT_ERROR) != 0 )
				{
					network_close(s);
					continue;
				}
			}
			else
			{
				if( evs & EPOLLIN )
				{
					if( s->event_handler(s, IOEVENT_READ) != 0 )
					{
						network_close(s);
						continue;
					}
				}
				
				if( evs & EPOLLOUT )
				{
					if( s->write_handler(s, IOEVENT_WRITE) != 0 )
					{
						network_close(s);
						continue;
					}
					else
					{
						// any data left?
						if( s->outlen == 0 )
						{
							// all data written, switch back to read mode
							network_mod(s->fd, EPOLLIN);
						}

						// don't have to do anything
					}
				}
			}
		}
	}
	
	// no errors
	return 0;
}

void network_init_socket(network_socket *s, int fd, int buffersize)
{
	int flags;

	s->fd = fd;
	s->miscdata = NULL;
	s->outbuffer = NULL;
	s->outbuffer_size = 0;
	s->outlen = 0;

	if( buffersize == 0 )
	{
		// set the socket to blocking
		if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
			flags = 0;
		
		flags &= ~O_NONBLOCK;
		fcntl(fd, F_SETFL, flags);
	}
	else
	{
		// set socket to nonblocking, and allocate it's buffer
		if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
			flags = 0;

		flags |= O_NONBLOCK;
		fcntl(fd, F_SETFL, flags);

		s->outbuffer_size = buffersize;
		s->outbuffer = (char*)vc_malloc(buffersize);

		// make sure the memory allocated
		assert(s->outbuffer != NULL);
	}
}

int network_add_socket(network_socket * s)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));

	// not hard, just add it to the fd map and it'll be polled next loop
	log_write(DEBUG, "adding socket %u to fd map.", s->fd);
	if( s->fd >= g_maxFd )
		g_maxFd = s->fd + 1;

	g_fds[s->fd] = s;

	// add it depending if we have data to write
	ev.data.fd = s->fd;
	ev.events = (s->outlen>0) ? EPOLLOUT : EPOLLIN;
	epoll_ctl(g_epoll, EPOLL_CTL_ADD, s->fd, &ev);

	return 0;
}

int network_remove_socket(network_socket * s)
{
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));

	// same thing, just remove it from the fd map
	log_write(DEBUG, "removing socket %u from fd map.", s->fd);
	g_fds[s->fd] = NULL;

	ev.data.fd = s->fd;
        ev.events = EPOLLOUT | EPOLLIN;
        epoll_ctl(g_epoll, EPOLL_CTL_DEL, s->fd, &ev);

	return 0;
}

/************************************************************************/
/* R/W Handlers                                                         */
/************************************************************************/

int network_read_data(network_socket * s, char* buffer, int buffer_len, struct sockaddr * read_addr)
{
	int rv;

	if( read_addr != NULL )
	{
		// we're reading a udp socket.
		int fromlen = sizeof(struct sockaddr);
		rv = recvfrom( s->fd, buffer, buffer_len, 0, read_addr, &fromlen );
	}
	else
	{
		// reading a tcp socket
		rv = recv( s->fd, buffer, buffer_len, 0 );
	}	
	
	// 0 = socket disconnection under windows, it doesn't use the except set :(
	if( rv <= 0 )
	{
		log_write(DEBUG, "recv() returned %d on socket %u (%s).", rv, s->fd, read_addr ? "UDP" : "TCP");
		return -1;
	}

	g_bytesRecv += rv;
	g_bytesRecvTotal += rv;	

	// read went ok.
	return rv;
}

int network_write_data(network_socket * s, void* data, int len, struct sockaddr * write_addr)
{
	char * odata = (char*)data;
	int rv;

	// are we a tcp or a udp socket?
	// tcp we can use a nonblocking write.
	// udp we have to send() and block. unfortunately. :(
	//     however, it shouldn't be a long block as the data is just copied into the os's internal buffer for the 
	//     fd.

	if( write_addr == NULL )			// tcp.
	{
		// this will push out as much data as possible
		rv = send( s->fd, odata, len, 0 );

		if( rv >= 0 || errno == EAGAIN )
		{
			// save any additional data in our buffer.
			int copylen = len - rv;

			if( copylen > 0 )
			{
				if( s->outlen + copylen > s->outbuffer_size )
				{
					// buffer overflow, lets treat the socket liek it was dead
					return -1;
				}

				// copy into the obuf
				memcpy(&s->outbuffer[s->outlen], &odata[rv], copylen);
				s->outlen += copylen;
			}

			// avoid the if trigger if 0 bytes were written (this is ok with nonblocking)
			rv = 1;
			
			// switch the socket to writing mode
			network_mod(s->fd, EPOLLOUT);
		}
	}
	else								// udp
	{
		rv = sendto(s->fd, odata, len, 0, write_addr, sizeof(struct sockaddr));
	}

	// error. :(
	if( rv <= 0 )
	{
		log_write(DEBUG, "send() returned %d on socket %u (%s).", rv, s->fd, write_addr ? "UDP" : "TCP");
		return -1;
	}

	g_bytesSent += rv;
	g_bytesSentTotal += rv;	

	// everything went ok
	return rv;
}

int network_close(network_socket * s)
{
	log_write(DEBUG, "closing socket %u", s->fd);

	// close the socket
	close(s->fd);

	// remove it from the set
	g_fds[s->fd] = NULL;

	// free the socket structure
	free(s);

	// no error
	return 0;
}

void network_shutdown()
{
	int n;

	for( n = 0; n < g_maxFd; ++n )
	{
		if( g_fds[n] != NULL )
		{
			network_close(g_fds[n]);
		}
	}
}

void network_get_bandwidth_statistics(float* bwin, float* bwout)
{
	int backup_in = g_bytesRecv;
	int backup_out = g_bytesSent;
	int av[2] = {0,0};
	int i;

	// zero
	g_bytesRecv = 0;
	g_bytesSent = 0;

	// increment our moving counter
	++g_byteCounter;
	g_byteCounter %= 10;

	// update our large moving average array
	g_bytesRStored[g_byteCounter] = backup_in;
	g_bytesSStored[g_byteCounter] = backup_out;

	// calculate our two averages
	for( i = 0; i < 10; ++i )
	{
		av[0] += g_bytesRStored[i];
		av[1] += g_bytesSStored[i];
	}

	// divide and return
	*bwin = ((float)av[0]) / 10.0f;
	*bwout = ((float)av[1]) / 10.0f;
}

// PHEW!

#endif

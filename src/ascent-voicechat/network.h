/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef __NETWORK_H
#define __NETWORK_H

typedef int(*network_io_callback)(void*, int);

typedef struct
{
	network_io_callback event_handler;
	network_io_callback write_handler;
	int fd;
	void* miscdata;

	// not used with udp
	char * outbuffer;
	int outlen;
	int outbuffer_size;
} network_socket;

enum io_event
{
	IOEVENT_READ			= 0,
	IOEVENT_WRITE			= 1,		// not used with udp
	IOEVENT_ERROR			= 2,
};

int network_init();
int network_io_poll();
int network_write_data(network_socket * s, void* data, int len, struct sockaddr * write_addr);
int network_read_data(network_socket * s, char* buffer, int buffer_len, struct sockaddr * read_addr);			// read_addr not used in tcp
int network_close(network_socket * s);
int network_add_socket(network_socket * s);
int network_remove_socket(network_socket * s);
int default_tcp_write_handler(network_socket* s, int act);
void network_shutdown();
void network_init_socket(network_socket *s, int fd, int buffersize);		// bufsize = 0 with udp sockets
void network_get_bandwidth_statistics(float* bwin, float* bwout);

#endif

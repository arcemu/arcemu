/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

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

#ifndef __COMMON_H
#define __COMMON_H

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_NONSTDC_NO_WARNINGS 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <pthread.h>
#define stricmp strcasecmp
#endif

// typedefs for our packets
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8;

static void* vc_malloc(size_t sz)
{
	void* res;
	if( (res = malloc(sz)) == NULL )
		printf("!!! vc_malloc OF %u BYTES FAILED!\n", (int)sz);

	return res;
}

static int vc_inet_pton(int family, const char * ipstr, struct in_addr* daddress)
{
	int b1,b2,b3,b4;
	char * p = (char*)daddress;
	if( sscanf(ipstr, "%u.%u.%u.%u", &b1, &b2, &b3, &b4) != 4 )
		return 0;

	p[0] = b4;
	p[1] = b3;
	p[2] = b2;
	p[3] = b1;
	return 1;
}

static void vc_sleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms*1000);
#endif	
}

// Thread starter
#ifdef WIN32
static void start_thread(void(*faddress)(void*), void* parameter)
{
	HANDLE hThread;
	DWORD threadId;
	hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)faddress, parameter, 0, &threadId);
	if( hThread == INVALID_HANDLE_VALUE )
		printf("CreateThread for 0x%.8X failed.\n", faddress);
}

#else
static void start_thread(void(*faddress)(void*), void* parameter)
{
	pthread_t p;
	if( pthread_create(&p, NULL, ((void*(*)(void*))faddress), parameter) < 0 )
		printf("thread_create() for 0x%.8X failed. errno: %u\n", faddress, errno);
}

#endif

#endif

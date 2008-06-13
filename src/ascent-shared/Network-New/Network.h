/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Include File For Ascent
 * Copyright (c) 2007 Burlex
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "../Common.h"
using namespace std;

/* windows sucks dick! */
#ifdef WIN32
#define USE_IOCP
#endif

/* Define these on non-windows systems */
#ifndef WIN32
#define ioctlsocket ioctl
#define closesocket close
#define TCP_NODELAY 0x6
#define SD_BOTH SHUT_RDWR
#define SOCKET int
#endif

#include "BaseSocket.h"
#include "BaseBuffer.h"
#include "StraightBuffer.h"
#include "CircularBuffer.h"
#include "SocketEngine.h"
#include "TcpSocket.h"

#ifdef USE_POLL
#include <sys/poll.h>
#define NETLIB_POLL
#include "SocketEngine_poll.h"
#endif

#ifdef USE_EPOLL
#include <sys/epoll.h>
#define NETLIB_EPOLL
#include "SocketEngine_epoll.h"
#endif

#ifdef USE_IOCP
#define NETLIB_IOCP
#include "SocketEngine_iocp.h"
#endif

#ifdef USE_SELECT
#define NETLIB_SELECT
#include "SocketEngine_Select.h"
#endif

#ifdef USE_KQUEUE
#include <sys/event.h>
#define NETLIB_KQUEUE
#include "SocketEngine_kqueue.h"
#endif

#include "ListenSocket.h"

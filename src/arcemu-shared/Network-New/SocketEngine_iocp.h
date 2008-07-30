/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implementation of IOCP Socket Engine
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

#ifndef _NETLIB_SOCKETENGINE_IOCP_H
#define _NETLIB_SOCKETENGINE_IOCP_H

#ifdef NETLIB_IOCP

struct Overlapped
{
	int m_op;
	void * m_acceptBuffer;
	OVERLAPPED m_ov;
};

class iocpEngine : public SocketEngine
{
	/** Our completion port
	 */
	HANDLE m_completionPort;

	/** Socket set lock
	 */
	Mutex m_socketLock;

	/** Socket set
	 */
	set<BaseSocket*> m_sockets;

	/** Spawned thread count
	 */
	int thread_count;

public:
	iocpEngine();
	~iocpEngine();

	/** Adds a socket to the engine.
	 */
	void AddSocket(BaseSocket * s);

	/** Removes a socket from the engine. It should not receive any more events.
	 */
	void RemoveSocket(BaseSocket * s);

	/** This is called when a socket has data to write for the first time.
	 */
	void WantWrite(BaseSocket * s);

	/** Spawn however many worker threads this engine requires
	 */
	void SpawnThreads();

	/** Called by SocketWorkerThread, this is the network loop.
	 */
	void MessageLoop();

	/** Shutdown the socket engine, disconnect any associated sockets and 
	 * deletes itself and the socket deleter.
	 */
	void Shutdown();
};

enum SocketEvents
{
	IO_EVENT_ACCEPT,
	IO_EVENT_READ,
	IO_EVENT_WRITE,
	IO_SHUTDOWN,
};

inline void CreateSocketEngine() { new iocpEngine; }

#endif		// NETLIB_IOCP
#endif		// _NETLIB_SOCKETENGINE_IOCP_H


/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implementation of poll() Socket Engine
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

#ifndef _NETLIB_SOCKETENGINE_POLL_H
#define _NETLIB_SOCKETENGINE_POLL_H

#ifdef NETLIB_POLL

/** This is the maximum number of connections you will be able to hold at one time.
 * adjust it accordingly.
 */
#define MAX_DESCRIPTORS 1024

class pollEngine : public SocketEngine
{
	/** Thread running or not?
	 */
	bool m_running;

	/** Binding for fd -> pointer
	 */
	BaseSocket * fds[MAX_DESCRIPTORS];

	/** poll events structures
	 */
	struct pollfd poll_events[MAX_DESCRIPTORS];

	/** highest file descriptor
	 */
	int highest_fd;

public:
	pollEngine();
	~pollEngine();

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

/** Returns the socket engine
 */
inline void CreateSocketEngine() { new pollEngine; }

#endif		// NETLIB_POLL

#endif		// _NETLIB_SOCKETENGINE_POLL_H


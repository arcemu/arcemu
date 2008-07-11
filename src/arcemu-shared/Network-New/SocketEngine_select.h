/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implemented Select Socket Engine
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

#ifndef _NETLIB_SELECTENGINE_H
#define _NETLIB_SELECTENGINE_H

#ifdef NETLIB_SELECT

class BaseSocket;
class SelectEngine : public SocketEngine
{
public:
	SelectEngine();
	~SelectEngine();

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

	/** Shutdown the socket engine, disconnect any associated sockets and 
	 * deletes itself and the socket deleter.
	 */
	void Shutdown();

	/** Called by SocketWorkerThread, this is the network loop.
	 */
	void MessageLoop();

protected:

	/** Thread running or not
	 */
	bool m_running;

	/** Protection for map
	 */
	Mutex m_socketLock;

	/** Map of fd->socket
	 */
	map<int, BaseSocket*> m_sockets;
};

inline void CreateSocketEngine() { new SelectEngine; }

#endif		// NETLIB_SELECT
#endif		// _NETLIB_SELECTENGINE_H

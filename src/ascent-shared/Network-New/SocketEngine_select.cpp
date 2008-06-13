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

#include "Network.h"

#ifdef NETLIB_SELECT

SelectEngine::SelectEngine()
{
	m_running = true;
	new SocketDeleter;
}

SelectEngine::~SelectEngine()
{

}

void SelectEngine::AddSocket(BaseSocket * s)
{
	m_socketLock.Acquire();
	m_sockets.insert( make_pair( s->GetFd(), s ) );
	m_socketLock.Release();
}

void SelectEngine::RemoveSocket(BaseSocket * s)
{
	m_socketLock.Acquire();
	m_sockets.erase(s->GetFd());
	m_socketLock.Release();
}

void SelectEngine::MessageLoop()
{
	FD_SET readable;
	FD_SET writable;
	FD_SET exception;
	map<int, BaseSocket*>::iterator itr, it2;
	int nfds;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 50000;
	while(m_running)
	{
		if(!m_sockets.size())
		{
			Sleep(20);
			continue;
		}

		FD_ZERO(&readable);
		FD_ZERO(&writable);
		FD_ZERO(&exception);

        m_socketLock.Acquire();
		for(itr = m_sockets.begin(); itr != m_sockets.end(); ++itr)
		{
			FD_SET( itr->second->GetFd(), itr->second->Writable() ? &writable : &readable );
		}
		m_socketLock.Release();

		nfds = select(FD_SETSIZE, &readable, &writable, &exception, &timeout);
		if(nfds > 0)
		{
			m_socketLock.Acquire();
			for(it2 = m_sockets.begin(); it2 != m_sockets.end();)
			{
				itr = it2++;
				if( FD_ISSET( itr->second->GetFd(), &exception ) )
					itr->second->OnError(errno);
				else if( FD_ISSET( itr->second->GetFd(), &readable ) )
					itr->second->OnRead(0);
				else if( FD_ISSET( itr->second->GetFd(), &writable ) )
					itr->second->OnWrite(0);
			}
			m_socketLock.Release();
		}
	}
}

void SelectEngine::SpawnThreads()
{
	launch_thread(new SocketEngineThread(this));
}

void SelectEngine::Shutdown()
{
	m_running = false;
	m_socketLock.Acquire();
	BaseSocket * s;
	for(map<int, BaseSocket*>::iterator itr = m_sockets.begin(); itr != m_sockets.end(); )
	{
		s = itr->second;
		++itr;
		s->Delete();
	}
	m_socketLock.Release();

	/* shutdown the socket deleter */
	sSocketDeleter.Kill();

	/* delete the socket deleter */
	delete SocketDeleter::getSingletonPtr();

	/* delete us */
	delete this;
}

void SelectEngine::WantWrite(BaseSocket * s)
{

}

#endif

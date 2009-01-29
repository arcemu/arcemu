/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - iocp-based SocketMgr for windows.
 *
 */


#ifndef SOCKETMGR_H_WIN32
#define SOCKETMGR_H_WIN32

#ifdef CONFIG_USE_IOCP

#define GLOBAL_DEAD_SOCKET_CHECK 80000

class Socket;
class SERVER_DECL SocketMgr : public Singleton<SocketMgr>
{
public:
	SocketMgr();
	~SocketMgr();

	ARCEMU_INLINE HANDLE GetCompletionPort() { return m_completionPort; }
	ARCEMU_INLINE void SetCompletionPort(HANDLE port) { m_completionPort = port; }

	void SpawnWorkerThreads();
	void CloseAll();
	void ShowStatus();
	void AddSocket(Socket * s)
	{
		if ( s==NULL ) return;
		if ( s->IsDeleted() ) return;
		socketLock.Acquire();
		_sockets.insert(s);
		socketLock.Release();
	}

	void RemoveSocket(Socket * s)
	{
		if ( s==NULL ) return;
		if ( s->IsDeleted() ) return;
		socketLock.Acquire();
		_sockets.erase(s);
		socketLock.Release();
	}

	void MakeSnapShot() // execute the global socket snapshot
	{
		set<Socket*>::iterator itr;
		socketLock.Acquire();
		if ( _sockets.size() ){
			for(itr = _sockets.begin(); itr != _sockets.end();)
			{
				Socket *s = (*itr);
				++itr;
				if ( s!=NULL ){
					if ( s->snapshot_read_bytes != s->total_read_bytes || s->snapshot_send_bytes != s->total_send_bytes ) // heartbeat with traffic
					{
						s->snapshot_read_bytes = s->total_read_bytes;
						s->snapshot_send_bytes = s->total_send_bytes;
						s->setLastheartbeat();
					}
				}
			}
		}
		socketLock.Release();
	}

	void CheckDeadSocket_Internal() // kicked for no traffic connection
	{
		set<Socket*>::iterator itr;
		uint32 t = getMSTime();
		uint32 counter=0;
		socketLock.Acquire();
		if ( _sockets.size() ){
			for(itr = _sockets.begin(); itr != _sockets.end();)
			{
				Socket *s = (*itr);
				++itr;
				if ( s!=NULL ){
					if ( s->getLastheartbeat()+GLOBAL_DEAD_SOCKET_CHECK <= t )
					{
						s->Disconnect(); // remove socket to garbage queue.
						counter++;
					}
				}
			}
			if ( counter )
				Log.Notice("SocketMgr","Recover (%u) sockets into garbage due to no traffic.",counter);
		}
		socketLock.Release();
	}

	void ShutdownThreads();
	long threadcount;

private:
	HANDLE m_completionPort;
	set<Socket*> _sockets;
	FastMutex socketLock;
};

#define sSocketMgr SocketMgr::getSingleton()

typedef void(*OperationHandler)(Socket * s, uint32 len);

class SocketWorkerThread : public ThreadBase
{
	bool shutdown;
public:
	/*void OnShutdown()
	{
		shutdown = true;
		Log.Notice("SocketWorkerThread","Detected the shutdown event from threadpool.");
	}*/
	bool run();
};

void SERVER_DECL HandleReadComplete(Socket * s, uint32 len);
void SERVER_DECL HandleWriteComplete(Socket * s, uint32 len);
void SERVER_DECL HandleShutdown(Socket * s, uint32 len);

static OperationHandler ophandlers[NUM_SOCKET_IO_EVENTS] = {
	&HandleReadComplete,
	&HandleWriteComplete,
	&HandleShutdown };

#endif
#endif
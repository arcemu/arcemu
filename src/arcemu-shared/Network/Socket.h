/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 * Socket implementable class.
 *
 * Modded by Cebernic
 */

#ifndef SOCKET_H
#define SOCKET_H

#include "SocketDefines.h"
#include "Timer.h"
#include "../Threading/RWLock.h"

class SERVER_DECL Socket
{
public:
	// Constructor. If fd = 0, it will be assigned 
	Socket(SOCKET fd, uint32 sendbuffersize, uint32 recvbuffersize);
	
	// Destructor.
	virtual ~Socket();

	// Open a connection to another machine.
	bool Connect(const char * Address, uint32 Port);

	// Disconnect the socket.
	void Disconnect(bool no_garbageprocess=false);

	// Accept from the already-set fd.
	void Accept(sockaddr_in * address);

/* Implementable methods */

	// Called when data is received.
	virtual void OnRead() {}

	// Called when a connection is first successfully established.
	virtual void OnConnect() {}

	// Called when the socket is disconnected from the client (either forcibly or by the connection dropping)
	virtual void OnDisconnect() {}

/* Sending Operations */

	// Locks sending mutex, adds bytes, unlocks mutex.
	bool Send(const uint8 * Bytes, uint32 Size);

	// Burst system - Locks the sending mutex.
	ARCEMU_INLINE void BurstBegin() { m_writeMutex.Acquire(); }
	ARCEMU_INLINE void LockReader() { m_readMutex.Acquire(); }

	// Burst system - Adds bytes to output buffer.
	bool BurstSend(const uint8 * Bytes, uint32 Size);

	// Burst system - Pushes event to queue - do at the end of write events.
	void BurstPush();

	// Burst system - Unlocks the sending mutex.
	ARCEMU_INLINE void BurstEnd() { m_writeMutex.Release(); }
	ARCEMU_INLINE void ReleaseReader() { m_readMutex.Release(); }

/* Client Operations */

	// Get the client's ip in numerical form.
	string GetRemoteIP();
	ARCEMU_INLINE uint32 GetRemotePort() { return ntohs(m_client.sin_port); }
	ARCEMU_INLINE SOCKET GetFd() { return m_fd; }
	

	/* Platform-specific methods */

	void SetupReadEvent(uint32 len);
	void ReadCallback(uint32 len);
	void WriteCallback();
	void markConnected();

	ARCEMU_INLINE bool IsDeleted() { return m_deleted; }
	ARCEMU_INLINE bool IsConnected() { return m_connected; }
	
	ARCEMU_INLINE sockaddr_in & GetRemoteStruct() { return m_client; }
	ARCEMU_INLINE CircularBuffer& GetReadBuffer() { return readBuffer; }
	ARCEMU_INLINE CircularBuffer& GetWriteBuffer() { return writeBuffer; }

/* Deletion */
	void Delete(bool force=false);

	ARCEMU_INLINE uint32 GenerateGUID() { return getMSTime(); }
	ARCEMU_INLINE void setGUID(uint32 x) { guid = x; }
	ARCEMU_INLINE uint32 getGUID() { return guid; }
	ARCEMU_INLINE void setLastheartbeat() { lastheartbeat = GenerateGUID(); }
	ARCEMU_INLINE uint32 getLastheartbeat() { return lastheartbeat; }
	ARCEMU_INLINE in_addr GetRemoteAddress() { return m_client.sin_addr; }

	// traffic counter
	uint64 total_send_bytes;
	uint64 total_read_bytes;

	// for internal deadsocket collector.
	uint64 snapshot_send_bytes;
	uint64 snapshot_read_bytes;

	uint32 _recvbuffsize;
	uint32 _sendbuffsize;

protected:

	uint32 guid;
	uint32 lastheartbeat;

	// Called when connection is opened.
	void _OnConnect();
  
	SOCKET m_fd;

	CircularBuffer readBuffer;
	CircularBuffer writeBuffer;

	Mutex m_writeMutex;
	Mutex m_readMutex;

	// we are connected? stop from posting events.
        bool m_connected;

        // We are deleted? Stop us from posting events.
        bool m_deleted;

	sockaddr_in m_client;

/* Win32 - IOCP Specific Calls */
#ifdef CONFIG_USE_IOCP

public:

	// Set completion port that this socket will be assigned to.
	ARCEMU_INLINE void SetCompletionPort(HANDLE cp) { m_completionPort = cp; }
	
	// Atomic wrapper functions for increasing read/write locks
	ARCEMU_INLINE void IncSendLock() { InterlockedIncrement(&m_writeLock); }
	ARCEMU_INLINE void DecSendLock() { InterlockedDecrement(&m_writeLock); }
	ARCEMU_INLINE bool AcquireSendLock()
	{
		if(m_writeLock)
			return false;
		else
		{
			IncSendLock();
			return true;
		}
	}
	OverlappedStruct m_readEvent;
	OverlappedStruct m_writeEvent;

private:
	// Completion port socket is assigned to
	HANDLE m_completionPort;
	
	// Write lock, stops multiple write events from being posted.
	volatile long m_writeLock;
	
	// Assigns the socket to his completion port.
	void AssignToCompletionPort();

	// Send Complete
	void CompletionPortIOComplete();
#endif

/* Linux - EPOLL Specific Calls */
#ifdef CONFIG_USE_EPOLL
public:
	// Posts a epoll event with the specifed arguments.
	void PostEvent(uint32 events);

	// Atomic wrapper functions for increasing read/write locks
	ARCEMU_INLINE void IncSendLock() { m_writeLockMutex.Acquire(); m_writeLock++; m_writeLockMutex.Release(); }
	ARCEMU_INLINE void DecSendLock() { m_writeLockMutex.Acquire(); m_writeLock--; m_writeLockMutex.Release(); }
	ARCEMU_INLINE bool HasSendLock() { bool res; m_writeLockMutex.Acquire(); res = (m_writeLock != 0); m_writeLockMutex.Release(); return res; }
	bool AcquireSendLock()
	{
	  bool rv;
	  m_writeLockMutex.Acquire();
	  if(m_writeLock != 0)
		rv = false;
	  else
	  {
		rv = true;
	m_writeLock++;
	  }
	  m_writeLockMutex.Release();
	  return rv;
	}

private:
	unsigned int m_writeLock;
	Mutex m_writeLockMutex;
#endif

/* FreeBSD - kqueue specific calls */
#ifdef CONFIG_USE_KQUEUE
public:
	// Posts a epoll event with the specifed arguments.
	void PostEvent(int events, bool oneshot);
	// Atomic wrapper functions for increasing read/write locks
	ARCEMU_INLINE void IncSendLock() { m_writeLockMutex.Acquire(); m_writeLock++; m_writeLockMutex.Release(); }
	ARCEMU_INLINE void DecSendLock() { m_writeLockMutex.Acquire(); m_writeLock--; m_writeLockMutex.Release(); }
	ARCEMU_INLINE bool HasSendLock() { bool res; m_writeLockMutex.Acquire(); res = (m_writeLock != 0); m_writeLockMutex.Release(); return res; }
	bool AcquireSendLock()
	{
		bool rv;
		m_writeLockMutex.Acquire();
		if(m_writeLock != 0)
			rv = false;
		else
		{
			rv = true;
			m_writeLock++;
		}
		m_writeLockMutex.Release();
		return rv;
	}

private:
	unsigned int m_writeLock;
	Mutex m_writeLockMutex;
#endif
};

/** Connect to a server.
* @param hostname Hostname or IP address to connect to
* @param port Port to connect to
* @return templated type if successful, otherwise null
*/
template<class T>
T* ConnectTCPSocket(const char * hostname, u_short port)
{
	T * s = new T(0);
	if(!s->Connect(hostname, port))
	{
		delete s; // cebernic:connect failed,so we just delete it directly
		return 0;
	}
	return s;	
}

/* Sockets Garbage Collector :D */
#define SOCKET_GC_TIMEOUT 4000 // cebernic: don't modify this value better !
/* 
  cebernic: The garbageCollector ,it also collecting the shit from WIN32 CompletionPortSocket
	This will gets something helpful for anti-dDOS. the attacker should be made many of connections as vaild.
	with that,they didn't send any size of bytes to us ,they just kept the connecting
	so right now,system will be kicked any connection without bytes sent after 2 sec.
	btw: win32platform only!
*/

class SocketGarbageCollectorThread : public ThreadBase
{
public:
	bool open;
	void OnShutdown()
	{
		open = false;
	}
	bool run();
};

class SocketGarbageCollector : public Singleton<SocketGarbageCollector>
{
	HM_NAMESPACE::hash_map<uint32,Socket*> deletionQueue;
public:
	RWLock lock;
	SocketGarbageCollectorThread *sgthread;
	bool running;
	~SocketGarbageCollector()
	{
		if ( running ) Shutdown();
		HM_NAMESPACE::hash_map<uint32,Socket*>::iterator i,i2;
		lock.AcquireWriteLock();
		if ( deletionQueue.size() ){
			for(i = deletionQueue.begin(); i != deletionQueue.end();)
			{
				i2 = i;
				++i;
				Socket *s = i2->second;
				if ( s==NULL ) continue;
				s->Disconnect(true); // disconnect without queue the socket again.
				deletionQueue.erase(i2);
			}
		}
		lock.ReleaseWriteLock();
		printf("SGC shut down.\n");
	}
	bool FindSocket(Socket *s)
	{
		if ( s==NULL ) return false;
		bool found=false;

		lock.AcquireReadLock();
		if ( deletionQueue.size() ) {
			HM_NAMESPACE::hash_map<uint32,Socket*>::iterator i = deletionQueue.find(s->getGUID());
			if ( i!=deletionQueue.end() ) // found
			{
				found = true;
			}
		}
		lock.ReleaseReadLock();

		return found;
	}
	bool RemoveSocket(Socket *s)
	{
		bool found=false;
		if ( s==NULL ) return found;
		lock.AcquireWriteLock();
		if ( deletionQueue.size() ) {
			HM_NAMESPACE::hash_map<uint32,Socket*>::iterator i = deletionQueue.find(s->getGUID());
			if ( i!=deletionQueue.end() ) // found
			{
				deletionQueue.erase(i);
				found=true;
			}
		}
		lock.ReleaseWriteLock();
		return found;
	}

	uint32 GetSocketSize()
	{
		uint32 t =0;
		if ( !running ) return t;
		lock.AcquireReadLock();
		t = deletionQueue.size();
		lock.ReleaseReadLock();
		return t;
	}

	void Update()
	{
		HM_NAMESPACE::hash_map<uint32,Socket*>::iterator i,i2;
		uint32 t = getMSTime();
		if ( !running ) return;
		lock.AcquireWriteLock();
		if ( deletionQueue.size() ){
			for(i = deletionQueue.begin(); i != deletionQueue.end();)
			{
				i2 = i;
				++i;
				Socket *s = i2->second;
				if ( s==NULL || s->IsConnected() ) continue;
				if( s->getLastheartbeat()+SOCKET_GC_TIMEOUT <=t)
				{
					//printf("Socket timeout! Read:%u Send:%u\n",s->total_read_bytes,s->total_send_bytes);
					s->Disconnect(true);
					deletionQueue.erase(i2);
				}

			}
		}
		lock.ReleaseWriteLock();
	}

	void QueueSocket(Socket * s)
	{
		if ( s == NULL ) return;
		if ( s->getGUID()==0 ) return; // we don't queue the un-listen socket.

		lock.AcquireWriteLock();
		if ( FindSocket(s) ) {
			lock.ReleaseWriteLock();
			return; // already inserted.
		}
		deletionQueue.insert( HM_NAMESPACE::hash_map<uint32,Socket*>::value_type(s->getGUID(), s) );
		lock.ReleaseWriteLock();
	}
	void Startup()
	{
		sgthread = new SocketGarbageCollectorThread();
		sgthread->open = true;
		running = true;
		ThreadPool.ExecuteTask(sgthread);
		Log.Color(TGREEN);
#ifdef WIN32
		Log.Notice("SGC","Socket garbage collector startup. - (AntiAttacker)");
#else
		Log.Notice("SGC","Socket garbage collector startup.");
#endif
		Log.Color(TNORMAL);
	}
	void Shutdown()
	{
		if ( sgthread ) sgthread->open = false;
		running = false;
		while ( sgthread!=NULL )
		{
			printf("Waiting for collector of socket garbage thread to end...\n");
			Sleep(500);
		}
	}
};

#define sSocketGarbageCollector SocketGarbageCollector::getSingleton()

#endif

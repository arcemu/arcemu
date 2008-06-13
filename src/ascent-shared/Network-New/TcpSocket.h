/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Implementation of TCP Socket
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

#ifndef _NETLIB_TCPSOCKET_H
#define _NETLIB_TCPSOCKET_H

class TcpSocket : public BaseSocket
{
public:
	/** Constructor
	 * @param fd File descriptor to use with this socket
	 * @param readbuffersize Incoming data buffer size
	 * @param writebuffersize Outgoing data buffer size
	 * @param use_circular_buffer Use a circular buffer or normal buffer?
	 * @param peer Connection
	 */
	TcpSocket(int fd, size_t readbuffersize, size_t writebuffersize, bool use_circular_buffer, const sockaddr_in * peer);

	/** Destructor
	 */
	~TcpSocket();

	/** Locks the socket's write buffer so you can begin a write operation
	 */
	inline void LockWriteBuffer() { m_writeMutex.Acquire(); }

	/** Unlocks the socket's write buffer so others can write to it
	 */
	inline void UnlockWriteBuffer() { m_writeMutex.Release(); }

	/** Writes the specified data to the end of the socket's write buffer
	 */
	bool Write(const void * data, size_t bytes);

	/** Reads the count of bytes from the buffer and put it in the specified pointer
	 */
	bool Read(void * destination, size_t bytes)
	{
		return m_readBuffer->Read(destination, bytes);
	}

	/** Disconnects the socket, removing it from the socket engine, and queues
	 * deletion.
	 */
	void Disconnect();

	/** Queues the socket for deletion, and disconnects it, if it is connected
	 */
	void Delete();

	/** Implemented OnRead()
	 */
	void OnRead(size_t len);

	/** Implemented OnWrite()
	 */
	void OnWrite(size_t len);

	/** When we read data this is called
	 */
	virtual void OnRecvData() {}
	virtual void OnConnect() {}
	virtual void OnDisconnect() {}
	
	void Finalize();

	/** Get IP in numerical form
	 */
	const char * GetIP() { return inet_ntoa(m_peer.sin_addr); }

	/** Are we writable?
	 */
	bool Writable();

	/** Occurs on error
	 */
	void OnError(int errcode);

	/** Windows gayness :P
	 */
	void OnAccept(void * pointer) {}

protected:

	/** Connected peer
	 */
	sockaddr_in m_peer;

	/** Socket's write buffer protection
	 */
	Mutex m_writeMutex;
};

/** Connect to a server.
 * @param hostname Hostname or IP address to connect to
 * @param port Port to connect to
 * @return templated type if successful, otherwise null
 */
template<class T>
T* ConnectTCPSocket(const char * hostname, u_short port)
{
	sockaddr_in conn;
	hostent * host;

	/* resolve the peer */
	host = gethostbyname(hostname);

	if(!host)
		return NULL;

	/* copy into our address struct */
	memcpy(&conn.sin_addr, host->h_addr_list[0], sizeof(in_addr));
	conn.sin_family = AF_INET;
	conn.sin_port = ntohs(port);

	/* open socket */
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	/* set him to blocking mode */
	u_long arg = 0;
	ioctlsocket(fd, FIONBIO, &arg);

	/* try to connect */
	int result = connect(fd, (const sockaddr*)&conn, sizeof(sockaddr_in));

	if(result < 0)
	{
		/*printf("connect() failed - %u\n", errno);*/
		closesocket(fd);
		return 0;
	}

	/* create the struct */
	T * s = new T(fd, &conn);
	s->Finalize();
	return s;	
}

#endif		// _NETLIB_TCPSOCKET_H

/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketOps - wrapper for any specific socket operations that may be platform-dependant.
 *
 */


#ifndef SOCKET_OPS_H
#define SOCKET_OPS_H

namespace SocketOps
{
	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor();

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd);

	// Enable blocking send/recv calls.
	bool Blocking(SOCKET fd);

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd);

	// Enables nagle buffering algorithm
	bool EnableBuffering(SOCKET fd);

	// Set internal buffer size to socket.
	bool SetRecvBufferSize(SOCKET fd, uint32 size);

	// Set internal buffer size to socket.
	bool SetSendBufferSize(SOCKET fd, uint32 size);

	// Closes socket completely.
	void CloseSocket(SOCKET fd);

	// Sets SO_REUSEADDR
	void ReuseAddr(SOCKET fd);
};

#endif


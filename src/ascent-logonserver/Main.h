/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#include "../ascent-shared/Singleton.h"

extern bool mrunning;
class AuthSocket;
extern set<AuthSocket*> _authSockets;
extern Mutex _authSocketLock;

struct AllowedIP
{
	unsigned int IP;
	unsigned char Bytes;
};

bool IsServerAllowed(unsigned int IP);
bool IsServerAllowedMod(unsigned int IP);

class LogonServer;
class LogonServer : public Singleton< LogonServer >
{
public:
	void CheckForDeadSockets();
	void Run(int argc, char ** argv);
	void Stop();
	uint32 max_build;
	uint32 min_build;
	uint8 sql_hash[20];

private:
	bool m_stopEvent;
};

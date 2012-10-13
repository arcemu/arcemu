/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../arcemu-shared/Singleton.h"

enum RealmFlags
{
    REALM_FLAG_NONE         = 0x00,
    REALM_FLAG_INVALID      = 0x01,
    REALM_FLAG_OFFLINE      = 0x02,
    REALM_FLAG_SPECIFYBUILD = 0x04,                         // client will show realm version in RealmList screen in form "RealmName (major.minor.revision.build)"
    REALM_FLAG_UNK1         = 0x08,
    REALM_FLAG_UNK2         = 0x10,
    REALM_FLAG_NEW_PLAYERS  = 0x20,
    REALM_FLAG_RECOMMENDED  = 0x40,
    REALM_FLAG_FULL         = 0x80
};

extern Arcemu::Threading::AtomicBoolean mrunning;
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
		void Run(int argc, char** argv);
		void Stop();
		uint32 max_build;
		uint32 min_build;
		uint8 sql_hash[20];

		Arcemu::PerformanceCounter perfcounter;
	private:
		bool m_stopEvent;
};

/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#ifndef REALMREGISTRY_H_
#define REALMREGISTRY_H_

typedef struct
{
	string Name;
	string Address;
	uint32 flags;
	uint32 Icon;
	uint32 TimeZone;
	float Population;
	uint8 Lock;
	HM_NAMESPACE::HM_HASH_MAP<uint32, uint8> CharacterMap;
} Realm;

class RealmRegistry : public Singleton<RealmRegistry>
{
		map<uint32, Realm*> m_realms;
		set<RealmSocket*> m_serverSockets;
		Mutex serverSocketLock;
		Mutex realmLock;

		uint32 realmhigh;
		bool usepings;

	public:
		~RealmRegistry();

		ARCEMU_INLINE Mutex & getServerSocketLock() { return serverSocketLock; }
		ARCEMU_INLINE Mutex & getRealmLock() { return realmLock; }

		RealmRegistry()
		{
			realmhigh = 0;
			usepings  = !Config.MainConfig.GetBoolDefault("LogonServer", "DisablePings", false);
			m_realms.clear();
		}

		void SendRealms(AuthSocket* Socket);

		uint32 GenerateRealmID()
		{
			return ++realmhigh;
		}

		Realm* AddRealm(uint32 realm_id, Realm* rlm);
		Realm* GetRealm(uint32 realm_id);
		int32 GetRealmIdByName(string Name);
		void RemoveRealm(uint32 realm_id);
		void SetRealmOffline(uint32 realm_id);
		void UpdateRealmStatus(uint32 realm_id, uint8 flags);
		void UpdateRealmPop(uint32 realm_id, float pop);

		ARCEMU_INLINE void AddServerSocket(RealmSocket* sock) { serverSocketLock.Acquire(); m_serverSockets.insert(sock); serverSocketLock.Release(); }
		ARCEMU_INLINE void RemoveServerSocket(RealmSocket* sock) { serverSocketLock.Acquire(); m_serverSockets.erase(sock); serverSocketLock.Release(); }

		void TimeoutSockets();
		void CheckServers();
};

#define sRealmRegistry RealmRegistry::getSingleton()

#endif

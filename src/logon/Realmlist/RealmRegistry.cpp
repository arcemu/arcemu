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

#include "LogonStdAfx.h"

initialiseSingleton(RealmRegistry);

RealmRegistry::~RealmRegistry()
{
	for(map<uint32, Realm*>::iterator itr = m_realms.begin(); itr != m_realms.end(); ++itr)
		delete itr->second;
}

Realm* RealmRegistry::AddRealm(uint32 realm_id, Realm* rlm)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);

	if(itr == m_realms.end())
		m_realms.insert(make_pair(realm_id, rlm));
	else
	{
		delete itr->second;
		itr->second = rlm;
	}
	realmLock.Release();
	return rlm;
}

Realm* RealmRegistry::GetRealm(uint32 realm_id)
{
	Realm* ret = NULL;

	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		ret = itr->second;
	}
	realmLock.Release();
	return ret;
}

int32 RealmRegistry::GetRealmIdByName(string Name)
{
	map<uint32, Realm*>::iterator itr = m_realms.begin();
	for(; itr != m_realms.end(); ++itr)
		if(itr->second->Name == Name)
		{
			return itr->first;
		}
	return -1;
}

void RealmRegistry::RemoveRealm(uint32 realm_id)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		delete itr->second;
		m_realms.erase(itr);
	}
	realmLock.Release();
}

void RealmRegistry::UpdateRealmStatus(uint32 realm_id, uint8 flags)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		itr->second->flags = flags;
	}
	realmLock.Release();
}

void RealmRegistry::UpdateRealmPop(uint32 realm_id, float pop)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		uint8 flags;
		if(pop >= 3)
			flags =  REALM_FLAG_FULL | REALM_FLAG_INVALID; // Full
		else if(pop >= 2)
			flags = REALM_FLAG_INVALID; // Red
		else if(pop >= 0.5)
			flags = 0; // Green
		else
			flags = REALM_FLAG_NEW_PLAYERS; // recommended

		itr->second->Population = (pop > 0) ? (pop >= 1) ? (pop >= 2) ? 2.0f : 1.0f : 0.0f : 0.0f;
		itr->second->flags = flags;
	}
	realmLock.Release();
}
void RealmRegistry::SendRealms(AuthSocket* Socket)
{
	realmLock.Acquire();

	// packet header
	ByteBuffer data(m_realms.size() * 150 + 20);
	data << uint8(0x10);
	data << uint16(0);	  // Size Placeholder

	// dunno what this is..
	data << uint32(0);

	//sAuthLogonChallenge_C * client = Socket->GetChallenge();
	data << uint16(m_realms.size());

	// loop realms :/
	map<uint32, Realm*>::iterator itr = m_realms.begin();
	HM_NAMESPACE::HM_HASH_MAP<uint32, uint8>::iterator it;
	for(; itr != m_realms.end(); ++itr)
	{
//		data << uint8(itr->second->Icon);
//		data << uint8(0);				   // Locked Flag
//		data << uint8(itr->second->Colour);
		data << uint8(itr->second->Icon);
		data << uint8(itr->second->Lock);		// delete when using data << itr->second->Lock;
		data << uint8(itr->second->flags);

		// This part is the same for all.
		data << itr->second->Name;
		data << itr->second->Address;
//		data << uint32(0x3fa1cac1);
		data << float(itr->second->Population);

		/* Get our character count */
		it = itr->second->CharacterMap.find(Socket->GetAccountID());
		data << uint8((it == itr->second->CharacterMap.end()) ? 0 : it->second);
//		data << uint8(1);   // time zone
//		data << uint8(6);
		data << uint8(itr->second->TimeZone);
		data << uint8(GetRealmIdByName(itr->second->Name));        //Realm ID
	}
	data << uint8(0x17);
	data << uint8(0);

	realmLock.Release();

	// Re-calculate size.

	*(uint16*)&data.contents()[1] = uint16(data.size() - 3);

	// Send to the socket.
	Socket->Send((const uint8*)data.contents(), uint32(data.size()));

	std::list< RealmSocket* > ss;
	std::list< RealmSocket* >::iterator SSitr;

	ss.clear();

	serverSocketLock.Acquire();

	if(m_serverSockets.empty())
	{
		serverSocketLock.Release();
		return;
	}

	set<RealmSocket*>::iterator itr1;

	// We copy the sockets to a list and call RefreshRealmsPop() from there because if the socket is dead,
	//then calling the method deletes the socket and removes it from the set corrupting the iterator and causing a crash!
	for(itr1 = m_serverSockets.begin(); itr1 != m_serverSockets.end(); ++itr1)
	{
		ss.push_back(*itr1);
	}

	serverSocketLock.Release();

	for(SSitr = ss.begin(); SSitr != ss.end(); ++SSitr)
		(*SSitr)->RefreshRealmsPop();

	ss.clear();
}

void RealmRegistry::TimeoutSockets()
{
	if(!usepings)
		return;

	uint32 now = uint32(time(NULL));

	/* burlex: this is vulnerable to race conditions, adding a mutex to it. */
	serverSocketLock.Acquire();

	for(set< RealmSocket* >::iterator itr = m_serverSockets.begin(); itr != m_serverSockets.end();)
	{
		RealmSocket* s = *itr;
		++itr;

		uint32 last_ping = s->last_ping.GetVal();
		if(last_ping < now && ((now - last_ping) > 300))
		{
			for(set< uint32 >::iterator RealmITR = s->server_ids.begin(); RealmITR != s->server_ids.end(); ++RealmITR)
			{
				uint32 RealmID = *RealmITR;

				SetRealmOffline(RealmID);
			}

			s->removed = true;
			m_serverSockets.erase(s);
			s->Disconnect();
		}
	}

	serverSocketLock.Release();
}

void RealmRegistry::CheckServers()
{
	serverSocketLock.Acquire();

	set<RealmSocket*>::iterator itr, it2;
	RealmSocket* s;
	for(itr = m_serverSockets.begin(); itr != m_serverSockets.end();)
	{
		s = *itr;
		it2 = itr;
		++itr;

		if(!IsServerAllowed(s->GetRemoteAddress().s_addr))
		{
			LOG_DETAIL("Disconnecting socket: %s due to it no longer being on an allowed IP.", s->GetRemoteIP().c_str());
			s->Disconnect();
		}
	}

	serverSocketLock.Release();
}

void RealmRegistry::SetRealmOffline(uint32 realm_id)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		itr->second->flags = REALM_FLAG_OFFLINE | REALM_FLAG_INVALID;
		itr->second->CharacterMap.clear();
		Log.Notice("RealmRegistry", "Realm %u is now offline (socket close).", realm_id);
	}
	realmLock.Release();
}


/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#define MAX_SESSIONS 3000

class ClientMgr : public Singleton<ClientMgr>
{
public:
	typedef HM_NAMESPACE::hash_map<uint32, RPlayerInfo*> ClientMap;

protected:
	ClientMap m_clients;
	uint32 m_maxSessionId;
	Session * m_sessions[MAX_SESSIONS];

public:
	ClientMgr();
	~ClientMgr();
	
	/* create rplayerinfo struct */
	RPlayerInfo * CreateRPlayer(uint32 guid);

	/* destroy rplayerinfo struct */
	void DestroyRPlayerInfo(uint32 guid);

	/* get rplayer */
	ASCENT_INLINE RPlayerInfo * GetRPlayer(uint32 guid)
	{
		ClientMap::iterator itr = m_clients.find(guid);
		return (itr != m_clients.end()) ? itr->second : 0;
	}
	/* send "mini" client data to all servers */
	void SendPackedClientInfo(WServer * server);

	/* get session by id */
	ASCENT_INLINE Session * GetSession(uint32 Id) { return (Id < MAX_SESSIONS) ? m_sessions[Id] : 0; }

	/* create a new session, returns null if the player is already logged in */
	Session * CreateSession(uint32 AccountId);

	/* updates sessions */
	void Update();
};

#define sClientMgr ClientMgr::getSingleton()



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



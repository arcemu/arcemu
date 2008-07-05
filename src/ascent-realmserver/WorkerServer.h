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

#define MAX_SESSIONS_PER_SERVER 1000

struct Instance;
typedef void(WServer::*WServerHandler)(WorldPacket &);

class WServer
{
	static WServerHandler PHandlers[IMSG_NUM_TYPES];
	uint32 m_id;
	WSSocket * m_socket;
	FastQueue<WorldPacket*, Mutex> m_recvQueue;
	list<Instance*> m_instances;

public:
	static void InitHandlers();
	WServer(uint32 id, WSSocket * s);
	~WServer();

	ASCENT_INLINE size_t GetInstanceCount() { return m_instances.size(); }
	ASCENT_INLINE void SendPacket(WorldPacket * data) { if(m_socket) m_socket->SendPacket(data); }
	ASCENT_INLINE void SendWoWPacket(Session * from, WorldPacket * data) { if(m_socket) m_socket->SendWoWPacket(from, data); }
	ASCENT_INLINE void AddInstance(Instance * pInstance) { m_instances.push_back(pInstance); }
	ASCENT_INLINE void QueuePacket(WorldPacket * data) { m_recvQueue.Push(data); }
	ASCENT_INLINE uint32 GetID() { return m_id; }

	void Update();

protected:
	/* packet handlers */
	void HandleRegisterWorker(WorldPacket & pck);
	void HandleWoWPacket(WorldPacket & pck);
	void HandlePlayerLoginResult(WorldPacket & pck);
	void HandlePlayerLogout(WorldPacket & pck);
	void HandleTeleportRequest(WorldPacket & pck);
};

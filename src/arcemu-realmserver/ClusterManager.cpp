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

#include "RStdAfx.h"

initialiseSingleton(ClusterMgr);
ClusterMgr::ClusterMgr()
{
	memset(SingleInstanceMaps, 0, sizeof(WServer*) * MAX_SINGLE_MAPID);
	memset(WorkerServers, 0, sizeof(WServer*) * MAX_WORKER_SERVERS);
	m_maxInstanceId = 0;
	m_maxWorkerServer = 0;
	Log.Success("ClusterMgr", "Interface Created");

	WServer::InitHandlers();
}

WServer * ClusterMgr::GetServerByInstanceId(uint32 InstanceId)
{
	InstanceMap::iterator itr = Instances.find(InstanceId);
	return (itr == Instances.end()) ? 0 : itr->second->Server;
}

WServer * ClusterMgr::GetServerByMapId(uint32 MapId)
{
	ASSERT(IS_MAIN_MAP(MapId));
	return SingleInstanceMaps[MapId]->Server;
}

Instance * ClusterMgr::GetInstanceByInstanceId(uint32 InstanceId)
{
	InstanceMap::iterator itr = Instances.find(InstanceId);
	return (itr == Instances.end()) ? 0 : itr->second;
}

Instance * ClusterMgr::GetInstanceByMapId(uint32 MapId)
{
	ASSERT(IS_MAIN_MAP(MapId));
	return SingleInstanceMaps[MapId];
}


WServer * ClusterMgr::CreateWorkerServer(WSSocket * s)
{
	/* find an id */
	uint32 i;
	for(i = 1; i < MAX_WORKER_SERVERS; ++i)
	{
		if(WorkerServers[i] == 0)
			break;
	}

	if(i == MAX_WORKER_SERVERS)
		return 0;		// No spaces

	Log.Debug("ClusterMgr", "Allocating worker server %u to %s:%u", i, s->GetRemoteIP().c_str(), s->GetRemotePort());
	WorkerServers[i] = new WServer(i, s);
	if(m_maxWorkerServer < i)
		m_maxWorkerServer = i;
	return WorkerServers[i];
}

void ClusterMgr::AllocateInitialInstances(WServer * server, vector<uint32>& preferred)
{
	vector<uint32> result;
	result.reserve(10);

	for(vector<uint32>::iterator itr = preferred.begin(); itr != preferred.end(); ++itr)
	{
		if(SingleInstanceMaps[*itr] == 0)
		{
			result.push_back(*itr);
		}
	}

	for(vector<uint32>::iterator itr = result.begin(); itr != result.end(); ++itr)
	{
		CreateInstance(*itr, server);
	}
}

Instance * ClusterMgr::CreateInstance(uint32 MapId, WServer * server)
{
	Instance * pInstance = new Instance;
	pInstance->InstanceId = ++m_maxInstanceId;
	pInstance->MapId = MapId;
	pInstance->Server = server;

	Instances.insert( make_pair( pInstance->InstanceId, pInstance ) );

	if(IS_MAIN_MAP(MapId))
		SingleInstanceMaps[MapId] = pInstance;

	/* tell the actual server to create the instance */
	WorldPacket data(ISMSG_CREATE_INSTANCE, 8);
	data << MapId << pInstance->InstanceId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	Log.Debug("ClusterMgr", "Allocating instance %u on map %u to server %u", pInstance->InstanceId, pInstance->MapId, server->GetID());
	return pInstance;
}

WServer * ClusterMgr::GetWorkerServerForNewInstance()
{
    WServer * lowest = 0;
	int32 lowest_load = -1;

	/* for now we'll just work with the instance count. in the future we might want to change this to
	   use cpu load instead. */

	for(uint32 i = 0; i < MAX_WORKER_SERVERS; ++i) {
		if(WorkerServers[i] != 0) {
			if((int32)WorkerServers[i]->GetInstanceCount() < lowest_load)
			{
				lowest = WorkerServers[i];
				lowest_load = int32(WorkerServers[i]->GetInstanceCount());
			}
		}
	}

	return lowest;
}

/* create new instance based on template, or a saved instance */
Instance * ClusterMgr::CreateInstance(uint32 InstanceId, uint32 MapId)
{
	/* pick a server for us :) */
	WServer * server = GetWorkerServerForNewInstance();
	if(!server) return 0;

	ASSERT(GetInstance(InstanceId) == NULL);

	/* bump up the max id if necessary */
	if(m_maxInstanceId <= InstanceId)
		m_maxInstanceId = InstanceId + 1;

    Instance * pInstance = new Instance;
	pInstance->InstanceId = InstanceId;
	pInstance->MapId = MapId;
	pInstance->Server = server;

	Instances.insert( make_pair( InstanceId, pInstance ) );

	/* tell the actual server to create the instance */
	WorldPacket data(ISMSG_CREATE_INSTANCE, 8);
	data << MapId << InstanceId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	Log.Debug("ClusterMgr", "Allocating instance %u on map %u to server %u", pInstance->InstanceId, pInstance->MapId, server->GetID());
	return pInstance;
}

void ClusterMgr::Update()
{
	for(uint32 i = 1; i <= m_maxWorkerServer; ++i)
		if(WorkerServers[i])
			WorkerServers[i]->Update();
}

void ClusterMgr::DistributePacketToAll(WorldPacket * data, WServer * exclude)
{
	for(uint32 i = 0; i <= m_maxWorkerServer; ++i)
		if(WorkerServers[i] && WorkerServers[i] != exclude)
			WorkerServers[i]->SendPacket(data);
}

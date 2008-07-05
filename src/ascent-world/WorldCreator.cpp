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

//
// WorldCreator.cpp
//

#include "StdAfx.h"

SERVER_DECL InstanceMgr sInstanceMgr;
initialiseSingleton( FormationMgr );

InstanceMgr::InstanceMgr()
{
	memset(m_maps, 0, sizeof(Map*)* NUM_MAPS);
	memset(m_instances, 0, sizeof(InstanceMap*) * NUM_MAPS);
	memset(m_singleMaps,0, sizeof(MapMgr*) * NUM_MAPS);
}

void InstanceMgr::Load(TaskList * l)
{
	new FormationMgr;

	// Create all non-instance type maps.
	QueryResult *result = CharacterDatabase.Query( "SELECT MAX(id) FROM instances" );
	if( result )
	{
		m_InstanceHigh = result->Fetch()[0].GetUInt32()+1;
		delete result;
	}
	else
		m_InstanceHigh = 1;

	// load each map we have in the database.
	result = WorldDatabase.Query("SELECT DISTINCT Map FROM creature_spawns");
	if(result)
	{
		do 
		{
			if(WorldMapInfoStorage.LookupEntry(result->Fetch()[0].GetUInt32()) == NULL)
				continue;

			if( result->Fetch()[0].GetUInt32() >= NUM_MAPS )
			{
				Log.Warning("InstanceMgr", "One or more of your creature_spawns rows specifies an invalid map: %u", result->Fetch()[0].GetUInt32() );
				continue;
			}

			//_CreateMap(result->Fetch()[0].GetUInt32());
			l->AddTask(new Task(new CallbackP1<InstanceMgr,uint32>(this, &InstanceMgr::_CreateMap, result->Fetch()[0].GetUInt32())));
		} while(result->NextRow());
		delete result;
	}

	l->wait();

	// create maps for any we don't have yet.
	StorageContainerIterator<MapInfo> * itr = WorldMapInfoStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		if( itr->Get()->mapid >= NUM_MAPS )
		{
			Log.Warning("InstanceMgr", "One or more of your worldmap_info rows specifies an invalid map: %u", itr->Get()->mapid );
			continue;
		}

		if(m_maps[itr->Get()->mapid] == NULL)
		{
			l->AddTask(new Task(new CallbackP1<InstanceMgr,uint32>(this, &InstanceMgr::_CreateMap, itr->Get()->mapid)));
		}
		//_CreateMap(itr->Get()->mapid);

		itr->Inc();
	}
	itr->Destruct();
	l->wait();

	// load saved instances
	_LoadInstances();
}

InstanceMgr::~InstanceMgr()
{

}

void InstanceMgr::Shutdown()
{
	uint32 i;
	InstanceMap::iterator itr;
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_instances[i] != NULL)
		{
			for(itr = m_instances[i]->begin(); itr != m_instances[i]->end(); ++itr)
			{
				if(itr->second->m_mapMgr)
					itr->second->m_mapMgr->KillThread();

				delete itr->second;
			}

			delete m_instances[i];
			m_instances[i]=NULL;
		}

		if(m_singleMaps[i] != NULL)
		{
			MapMgr * ptr = m_singleMaps[i];
			ptr->KillThread();
			delete ptr;
			m_singleMaps[i]=NULL;
		}

		if(m_maps[i] != NULL)
		{
			delete m_maps[i];
			m_maps[i]=NULL;
		}
	}

	delete FormationMgr::getSingletonPtr();
}

uint32 InstanceMgr::PreTeleport(uint32 mapid, Player * plr, uint32 instanceid)
{
	// preteleport is where all the magic happens :P instance creation, etc.
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(mapid);
	Group * pGroup;
	InstanceMap * instancemap;
	Instance * in;

	if(inf == NULL || mapid>=NUM_MAPS)
		return INSTANCE_ABORT_NOT_FOUND;

	// main continent check.
	if(inf->type == INSTANCE_NULL)
	{
		// this will be useful when clustering comes into play.
		// we can check if the destination world server is online or not and then cancel them before they load.
		return (m_singleMaps[mapid] != NULL) ? INSTANCE_OK : INSTANCE_ABORT_NOT_FOUND;
	}

	// shouldn't happen
	if(inf->type==INSTANCE_PVP)
		return INSTANCE_ABORT_NOT_FOUND;

	// players without groups cannot enter raid instances (no soloing them:P)
	if(plr->GetGroup() == NULL && (inf->type == INSTANCE_RAID || inf->type == INSTANCE_MULTIMODE) && !plr->triggerpass_cheat)
		return INSTANCE_ABORT_NOT_IN_RAID_GROUP;

	// check that heroic mode is available if the player has requested it.
	if(plr->iInstanceType && inf->type != INSTANCE_MULTIMODE)
		return INSTANCE_ABORT_HEROIC_MODE_NOT_AVAILABLE;

	// if we are here, it means:
	// 1) we're a non-raid instance
	// 2) we're a raid instance, and the person is in a group.
	// so, first we have to check if they have an instance on this map already, if so, allow them to teleport to that.
	// otherwise, we can create them a new one.
	m_mapLock.Acquire();
	instancemap = m_instances[mapid];

	// set up our pointers (cleaner code is always good)
	pGroup = plr->GetGroup();

	if(instancemap == NULL)
	{
		if(instanceid != 0)
		{
			m_mapLock.Release();
			return INSTANCE_ABORT_NOT_FOUND;
		}

		// gotta create the hashmap.
		m_instances[mapid] = new InstanceMap;
		instancemap = m_instances[mapid];
	}
	else
	{
		InstanceMap::iterator itr;
		if(instanceid != 0)
		{
			itr = instancemap->find(instanceid);
			if(itr != instancemap->end()) {
				m_mapLock.Release();
				return INSTANCE_OK;
			} else {
				m_mapLock.Release();
				return INSTANCE_ABORT_NOT_FOUND;
			}
		}
		else
		{
			// search the instance and see if we have one here.
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;
				if(PlayerOwnsInstance(in, plr))
				{
					m_mapLock.Release();

					// check the player count and in combat status.
					if(in->m_mapMgr)
					{
						if(in->m_mapMgr->IsCombatInProgress())
							return INSTANCE_ABORT_ENCOUNTER;

						if(in->m_mapMgr->GetPlayerCount() >= inf->playerlimit)
							return INSTANCE_ABORT_FULL;
					}

					// found our instance, allow him in.
					return INSTANCE_OK;
				}
			}
		}
	}

	// if we're here, it means we need to create a new instance.
	in = new Instance;
	in->m_creation = UNIXTIME;
	in->m_expiration = (inf->type == INSTANCE_NONRAID) ? 0 : UNIXTIME + inf->cooldown;		// expire time 0 is 10 minutes after last player leaves
	in->m_creatorGuid = pGroup ? 0 : plr->GetLowGUID();		// creator guid is 0 if its owned by a group.
	in->m_creatorGroup = pGroup ? pGroup->GetID() : 0;
	in->m_difficulty = plr->iInstanceType;
	in->m_instanceId = GenerateInstanceID();
	in->m_mapId = mapid;
	in->m_mapMgr = NULL;		// always start off without a map manager, it is created in GetInstance()
	in->m_mapInfo = inf;
	in->m_isBattleground=false;
	plr->SetInstanceID(in->m_instanceId);
	Log.Debug("InstanceMgr", "Creating instance for player %u and group %u on map %u. (%u)", in->m_creatorGuid, in->m_creatorGroup, in->m_mapId, in->m_instanceId);
	
	// save our new instance to the database.
	in->SaveToDB();
    
	// apply it in the instance map
	instancemap->insert( InstanceMap::value_type( in->m_instanceId, in ) );

	// instance created ok, i guess? return the ok for him to transport.
	m_mapLock.Release();
	return INSTANCE_OK;
}

MapMgr* InstanceMgr::GetMapMgr(uint32 mapId)
{
	return m_singleMaps[mapId];
}

MapMgr * InstanceMgr::GetInstance(Object* obj)
{
	Player * plr;
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(obj->GetMapId());

	// we can *never* teleport to maps without a mapinfo.
	if( inf == NULL || obj->GetMapId() >= NUM_MAPS )
		return NULL;

	if( obj->IsPlayer() )
	{
		// players can join instances based on their groups/solo status.
		plr = static_cast< Player* >( obj );

		// single-instance maps never go into the instance set.
		if( inf->type == INSTANCE_NULL )
			return m_singleMaps[obj->GetMapId()];

		m_mapLock.Acquire();
		instancemap = m_instances[obj->GetMapId()];
		if(instancemap != NULL)
		{
			// check our saved instance id. see if its valid, and if we can join before trying to find one.
			itr = instancemap->find(obj->GetInstanceID());
			if(itr != instancemap->end())
			{
				if(itr->second->m_mapMgr)
				{
					m_mapLock.Release();
					return itr->second->m_mapMgr;
				}
			}

			// iterate over our instances, and see if any of them are owned/joinable by him.
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;
				if(PlayerOwnsInstance(in, plr))
				{
					// this is our instance.
					if(in->m_mapMgr == NULL)
					{
						/*if(plr->m_TeleportState == 1)
						{
							// the player is loading. boot him out to the entry point, we don't want to spam useless instances on startup.
							m_mapLock.Release();
							return NULL;
						}*/

						// create the actual instance.
						in->m_mapMgr = _CreateInstance(in);
						m_mapLock.Release();
						return in->m_mapMgr;
					}
					else
					{
						// instance is already created.
						m_mapLock.Release();
						return in->m_mapMgr;
					}
				}
			}
		}

		// if we're here, it means there are no instances on that map, or none of the instances on that map are joinable
		// by this player.
		m_mapLock.Release();
		return NULL;
	}
	else
	{
		// units are *always* limited to their set instance ids.
		if(inf->type == INSTANCE_NULL)
			return m_singleMaps[obj->GetMapId()];

		m_mapLock.Acquire();
		instancemap = m_instances[obj->GetMapId()];
		if(instancemap)
		{
			itr = instancemap->find(obj->GetInstanceID());
			if(itr != instancemap->end())
			{
				// we never create instances just for units.
				m_mapLock.Release();
				return itr->second->m_mapMgr;
			}
		}

		// instance is non-existant (shouldn't really happen for units...)
		m_mapLock.Release();
		return NULL;
	}
}

MapMgr * InstanceMgr::_CreateInstance(uint32 mapid, uint32 instanceid)
{
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(mapid);
	MapMgr * ret;

	ASSERT(inf && inf->type == INSTANCE_NULL);
	ASSERT(mapid < NUM_MAPS && m_maps[mapid] != NULL);

	Log.Notice("InstanceMgr", "Creating continent %s.", m_maps[mapid]->GetName());

	ret = new MapMgr(m_maps[mapid], mapid, instanceid);
	ASSERT(ret);

	// start its thread
	ThreadPool.ExecuteTask(ret);
    
	// assign pointer
	m_singleMaps[mapid] = ret;
	return ret;
}

MapMgr * InstanceMgr::_CreateInstance(Instance * in)
{
	Log.Notice("InstanceMgr", "Creating saved instance %u (%s)", in->m_instanceId, m_maps[in->m_mapId]->GetName());
	ASSERT(in->m_mapMgr==NULL);

	// we don't have to check for world map info here, since the instance wouldn't have been saved if it didn't have any.
	in->m_mapMgr = new MapMgr(m_maps[in->m_mapId], in->m_mapId, in->m_instanceId);
	in->m_mapMgr->pInstance = in;
	in->m_mapMgr->iInstanceMode = in->m_difficulty;
	in->m_mapMgr->InactiveMoveTime = 60+UNIXTIME;
	ThreadPool.ExecuteTask(in->m_mapMgr);
	return in->m_mapMgr;
}

void InstanceMgr::_CreateMap(uint32 mapid)
{
	if( mapid >= NUM_MAPS )
		return;

	MapInfo * inf;

	inf = WorldMapInfoStorage.LookupEntry(mapid);
	if(inf==NULL)
		return;
	if(m_maps[mapid]!=NULL)
		return;

	m_maps[mapid] = new Map(mapid, inf);
	if(inf->type == INSTANCE_NULL)
	{
		// we're a continent, create the instance.
		_CreateInstance(mapid, GenerateInstanceID());
	}
}

uint32 InstanceMgr::GenerateInstanceID()
{
	uint32 iid;
	m_mapLock.Acquire();
	iid = m_InstanceHigh++;
	m_mapLock.Release();
	return iid;
}

void BuildStats(MapMgr * mgr, char * m_file, Instance * inst, MapInfo * inf)
{
	char tmp[200];
	strcpy(tmp, "");
#define pushline strcat(m_file, tmp)

	snprintf(tmp, 200, "	<instance>\n");																												pushline;
	snprintf(tmp, 200, "		<map>%u</map>\n", mgr->GetMapId());																						pushline;
	snprintf(tmp, 200, "		<maptype>%u</maptype>\n", inf->type);																						pushline;
	snprintf(tmp, 200, "		<players>%u</players>\n", mgr->GetPlayerCount());																			pushline;
	snprintf(tmp, 200, "		<maxplayers>%u</maxplayers>\n", inf->playerlimit);																		pushline;

	//<creationtime>
	if (inst)
	{
		tm *ttime = localtime( &inst->m_creation );
		snprintf(tmp, 200, "		<creationtime>%02u:%02u:%02u %02u/%02u/%u</creationtime>\n",ttime->tm_hour, ttime->tm_min, ttime->tm_sec, ttime->tm_mday, ttime->tm_mon, uint32( ttime->tm_year + 1900 ));
		pushline;
	}
	else
	{
		snprintf(tmp, 200, "		<creationtime>N/A</creationtime>\n");
		pushline;
	}

	//<expirytime>
	if (inst && inst->m_expiration)
	{
		tm *ttime = localtime( &inst->m_expiration );
		snprintf(tmp, 200, "		<expirytime>%02u:%02u:%02u %02u/%02u/%u</expirytime>\n",ttime->tm_hour, ttime->tm_min, ttime->tm_sec, ttime->tm_mday, ttime->tm_mon, uint32( ttime->tm_year + 1900 ));
		pushline;
	}
	else
	{
		snprintf(tmp, 200, "		<expirytime>N/A</expirytime>\n");
		pushline;

	}
	//<idletime>
	if (mgr->InactiveMoveTime)
	{
		tm *ttime = localtime( &mgr->InactiveMoveTime );
		snprintf(tmp, 200, "		<idletime>%02u:%02u:%02u %02u/%02u/%u</idletime>\n",ttime->tm_hour, ttime->tm_min, ttime->tm_sec, ttime->tm_mday, ttime->tm_mon, uint32( ttime->tm_year + 1900 ));
		pushline;
	}
	else
	{
		snprintf(tmp, 200, "		<idletime>N/A</idletime>\n");
		pushline;
	}

	snprintf(tmp, 200, "	</instance>\n");																											pushline;
#undef pushline
}

void InstanceMgr::BuildXMLStats(char * m_file)
{
	uint32 i;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	Instance * in;
	
	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_singleMaps[i] != NULL)
			BuildStats(m_singleMaps[i], m_file, NULL, m_singleMaps[i]->GetMapInfo());
		else
		{
			instancemap = m_instances[i];
			if(instancemap != NULL)
			{
				for(itr = instancemap->begin(); itr != instancemap->end();)
				{
					in = itr->second;
					++itr;

					if(in->m_mapMgr==NULL)
						continue;

					BuildStats(in->m_mapMgr, m_file, in, in->m_mapInfo);
				}
			}
		}
	}
	m_mapLock.Release();
}

void InstanceMgr::_LoadInstances()
{
	MapInfo * inf;
	Instance * in;
	QueryResult * result;

	// clear any instances that have expired.
	Log.Notice("InstanceMgr", "Deleting Expired Instances...");
	CharacterDatabase.WaitExecute("DELETE FROM instances WHERE expiration <= %u", UNIXTIME);
	
	// load saved instances
	result = CharacterDatabase.Query("SELECT * FROM instances");
	Log.Notice("InstanceMgr", "Loading %u saved instances." , result ? result->GetRowCount() : 0);

	if(result)
	{
		do 
		{
			inf = WorldMapInfoStorage.LookupEntry(result->Fetch()[1].GetUInt32());
			if(inf == NULL || result->Fetch()[1].GetUInt32() >= NUM_MAPS)
			{
				CharacterDatabase.Execute("DELETE FROM instances WHERE mapid = %u", result->Fetch()[1].GetUInt32());
				continue;
			}

			in = new Instance();
			in->m_mapInfo = inf;
			in->LoadFromDB(result->Fetch());

			// this assumes that groups are already loaded at this point.
			if(in->m_creatorGroup && objmgr.GetGroupById(in->m_creatorGroup) == NULL)
			{
				delete in;
				continue;
			}

			if(m_instances[in->m_mapId] == NULL)
				m_instances[in->m_mapId] = new InstanceMap;

			m_instances[in->m_mapId]->insert( InstanceMap::value_type( in->m_instanceId, in ) );

		} while(result->NextRow());
		delete result;
	}
}

void Instance::LoadFromDB(Field * fields)
{
	char * p, *q;
	char * m_npcstring = strdup(fields[4].GetString());

	m_instanceId = fields[0].GetUInt32();
	m_mapId = fields[1].GetUInt32();
	m_creation = fields[2].GetUInt32();
	m_expiration = fields[3].GetUInt32();
	m_difficulty = fields[5].GetUInt32();
	m_creatorGroup = fields[6].GetUInt32();
	m_creatorGuid = fields[7].GetUInt32();
	m_mapMgr=NULL;

	// process saved npc's
	q = m_npcstring;
	p = strchr(m_npcstring, ' ');
	while(p)
	{
		*p = 0;
		uint32 val = atol(q);
		if (val)
			m_killedNpcs.insert( val );
		q = p+1;
		p = strchr(q, ' ');
	}

	free(m_npcstring);
}

void InstanceMgr::ResetSavedInstances(Player * plr)
{
	WorldPacket data(SMSG_RESET_INSTANCE, 4);
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	if(!plr->IsInWorld() || plr->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
		return;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_instances[i] != NULL)
		{
			instancemap = m_instances[i];
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				if( ( in->m_mapInfo->type == INSTANCE_NONRAID && (plr->GetGroup() && plr->GetGroup()->GetID() == in->m_creatorGroup) ) || ( in->m_mapInfo->type == INSTANCE_NONRAID && plr->GetLowGUID() == in->m_creatorGuid ) )
				{
					if(in->m_mapMgr && in->m_mapMgr->HasPlayers())
					{
						plr->GetSession()->SystemMessage("Failed to reset instance %u (%s), due to players still inside.", in->m_instanceId, in->m_mapMgr->GetMapInfo()->name);
						continue;
					}

					// <mapid> has been reset.
					data << uint32(in->m_mapId);
					plr->GetSession()->SendPacket(&data);

					// destroy the instance
					_DeleteInstance(in, true);
				}
			}
		}
	}
    m_mapLock.Release();	
}

void InstanceMgr::OnGroupDestruction(Group * pGroup)
{
	// this means a group has been deleted, so lets clear out all instances that they owned.
	// (instances don't transfer to the group leader, or anything)
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		instancemap = m_instances[i];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				if(in->m_creatorGroup && in->m_creatorGroup == pGroup->GetID())
					_DeleteInstance(in, false);
			}
		}
	}
	m_mapLock.Release();
}

bool InstanceMgr::_DeleteInstance(Instance * in, bool ForcePlayersOut)
{
	m_mapLock.Acquire();
	InstanceMap * instancemap;
	InstanceMap::iterator itr;

	if(in->m_mapMgr)
	{
		// "ForcePlayersOut" will teleport the players in this instance to their entry point/hearthstone.
		// otherwise, they will get a 60 second timeout telling them they are not in this instance's group.
		if(in->m_mapMgr->HasPlayers())
		{
			if(ForcePlayersOut)
				in->m_mapMgr->InstanceShutdown();
			else
			{
				in->m_mapMgr->BeginInstanceExpireCountdown();
				in->m_mapMgr->pInstance = NULL;
			}
		}
		else
			in->m_mapMgr->InstanceShutdown();
	}

	// remove the instance from the large map.
	instancemap = m_instances[in->m_mapId];
	if(instancemap)
	{
		itr = instancemap->find(in->m_instanceId);
		if(itr != instancemap->end())
			instancemap->erase(itr);
	}
    
	// cleanup corpses, database references
	in->DeleteFromDB();

	// delete the instance pointer.
	delete in;
	m_mapLock.Release();
	
	return true;
}

void Instance::DeleteFromDB()
{
	// cleanup all the corpses on this map
	CharacterDatabase.Execute("DELETE FROM corpses WHERE instanceid = %u", m_instanceId);

	// delete from the database
	CharacterDatabase.Execute("DELETE FROM instances WHERE id = %u", m_instanceId);
}

void InstanceMgr::CheckForExpiredInstances()
{
	// checking for any expired instances.
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		instancemap = m_instances[i];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				// use a "soft" delete here.
				if(in->m_mapInfo->type != INSTANCE_NONRAID && HasInstanceExpired(in))
					_DeleteInstance(in, false);
			}

		}
	}
	m_mapLock.Release();
}

void InstanceMgr::BuildSavedInstancesForPlayer(Player * plr)
{
	WorldPacket data(4);
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;

	if(!plr->IsInWorld() || plr->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
	{
		m_mapLock.Acquire();
		for(i = 0; i < NUM_MAPS; ++i)
		{
			if(m_instances[i] != NULL)
			{
				instancemap = m_instances[i];
				for(itr = instancemap->begin(); itr != instancemap->end();)
				{
					in = itr->second;
					++itr;

					if( PlayerOwnsInstance(in, plr) && in->m_mapInfo->type == INSTANCE_NONRAID )
					{
						m_mapLock.Release();

						data.SetOpcode(SMSG_INSTANCE_SAVE);
						data << uint32(in->m_mapId);
						plr->GetSession()->SendPacket(&data);

						data.Initialize(SMSG_INSTANCE_RESET_ACTIVATE);
						data << uint32(0x01);
						plr->GetSession()->SendPacket(&data);
					
						return;
					}
				}
			}
		}
		m_mapLock.Release();
	}

	data.SetOpcode(SMSG_INSTANCE_RESET_ACTIVATE);
	data << uint32(0x00);
	plr->GetSession()->SendPacket(&data);
}

void InstanceMgr::BuildRaidSavedInstancesForPlayer(Player * plr)
{
	WorldPacket data(SMSG_RAID_INSTANCE_INFO, 200);
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	uint32 i;
	uint32 counter = 0;

	data << counter;
	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		if(m_instances[i] != NULL)
		{
			instancemap = m_instances[i];
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				if( in->m_mapInfo->type != INSTANCE_NONRAID && PlayerOwnsInstance(in, plr) )
				{
					data << in->m_mapId;
					data << uint32(in->m_expiration - UNIXTIME);
					data << in->m_instanceId;
					++counter;
				}
			}
		}
	}
	m_mapLock.Release();

#ifdef USING_BIG_ENDIAN
	*(uint32*)&data.contents()[0] = swap32(counter);
#else
	*(uint32*)&data.contents()[0] = counter;
#endif
	plr->GetSession()->SendPacket(&data);
}

void Instance::SaveToDB()
{
	// don't save non-raid instances.
	if(m_mapInfo->type == INSTANCE_NONRAID || m_isBattleground)
		return;

	std::stringstream ss;
	set<uint32>::iterator itr;

	ss << "REPLACE INTO instances VALUES("
		<< m_instanceId << ","
		<< m_mapId << ","
		<< (uint32)m_creation << ","
		<< (uint32)m_expiration << ",'";

	for(itr = m_killedNpcs.begin(); itr != m_killedNpcs.end(); ++itr)
		ss << (*itr) << " ";

	ss << "',"
		<< m_difficulty << ","
		<< m_creatorGroup << ","
		<< m_creatorGuid << ")";

	CharacterDatabase.Execute(ss.str().c_str());
}

void InstanceMgr::PlayerLeftGroup(Group * pGroup, Player * pPlayer)
{
	// does this group own any instances? we have to kick the player out of those instances.
	Instance * in;
	InstanceMap::iterator itr;
	InstanceMap * instancemap;
	WorldPacket data(SMSG_RAID_GROUP_ONLY, 8);
	uint32 i;

	m_mapLock.Acquire();
	for(i = 0; i < NUM_MAPS; ++i)
	{
		instancemap = m_instances[i];
		if(instancemap)
		{
			for(itr = instancemap->begin(); itr != instancemap->end();)
			{
				in = itr->second;
				++itr;

				if(in->m_creatorGroup && in->m_creatorGroup == pGroup->GetID())
				{
					// better make sure we're actually in that instance.. :P
					if(!pPlayer->raidgrouponlysent && pPlayer->GetInstanceID() == (int32)in->m_instanceId)
					{
						data << uint32(60000) << uint32(1);
						pPlayer->GetSession()->SendPacket(&data);
						pPlayer->raidgrouponlysent=true;
	
						sEventMgr.AddEvent(pPlayer, &Player::EjectFromInstance, EVENT_PLAYER_EJECT_FROM_INSTANCE, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

						m_mapLock.Release();
						return;
					}
				}
			}
		}
	}
	m_mapLock.Release();
}

MapMgr * InstanceMgr::CreateBattlegroundInstance(uint32 mapid)
{
	// shouldn't happen
	if( mapid >= NUM_MAPS )
		return NULL;

	if(!m_maps[mapid])
	{
		_CreateMap(mapid);
		if(!m_maps[mapid])
			return NULL;
	}

	MapMgr * ret = new MapMgr(m_maps[mapid],mapid,GenerateInstanceID());
	Instance * pInstance = new Instance();
	pInstance->m_creation = UNIXTIME;
	pInstance->m_creatorGroup = 0;
	pInstance->m_creatorGuid = 0;
	pInstance->m_difficulty = 0;
	pInstance->m_expiration = 0;
	pInstance->m_instanceId = ret->GetInstanceID();
	pInstance->m_isBattleground = true;
	pInstance->m_mapId = mapid;
	pInstance->m_mapInfo = WorldMapInfoStorage.LookupEntry( mapid );
	pInstance->m_mapMgr = ret;
	m_mapLock.Acquire();
	if( m_instances[mapid] == NULL )
		m_instances[mapid] = new InstanceMap;

	m_instances[mapid]->insert( make_pair( pInstance->m_instanceId, pInstance ) );
	m_mapLock.Release();
	ThreadPool.ExecuteTask(ret);
	return ret;
}

void InstanceMgr::DeleteBattlegroundInstance(uint32 mapid, uint32 instanceid)
{
	m_mapLock.Acquire();
	InstanceMap::iterator itr = m_instances[mapid]->find( instanceid );
	if( itr == m_instances[mapid]->end() )
	{
		printf("Could not delete battleground instance!\n");
		m_mapLock.Release();
		return;
	}

	m_instances[mapid]->erase( itr );
	m_mapLock.Release();
}

FormationMgr::FormationMgr()
{
	QueryResult * res = WorldDatabase.Query("SELECT * FROM creature_formations");
	if(res)
	{
		Formation *f ;
		do 
		{
			f = new Formation;
			f->fol = res->Fetch()[1].GetUInt32();
			f->ang = res->Fetch()[2].GetFloat();
			f->dist = res->Fetch()[3].GetFloat();
			m_formations[res->Fetch()[0].GetUInt32()] = f;
		} while(res->NextRow());
		delete res;
	}
}

FormationMgr::~FormationMgr()
{
	FormationMap::iterator itr;
	for(itr = m_formations.begin(); itr != m_formations.end(); ++itr)
		delete itr->second;
}

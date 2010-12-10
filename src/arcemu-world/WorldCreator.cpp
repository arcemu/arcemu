/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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
	memset(&m_nextInstanceReset, 0, sizeof(time_t) * NUM_MAPS);
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
				Log.Error("InstanceMgr", "One or more of your creature_spawns rows specifies an invalid map: %u", result->Fetch()[0].GetUInt32() );
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
			Log.Error("InstanceMgr", "One or more of your worldmap_info rows specifies an invalid map: %u", itr->Get()->mapid );
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

	// load reset times
	result = CharacterDatabase.Query("SELECT setting_id, setting_value FROM server_settings WHERE setting_id LIKE 'next_instance_reset_%%'");
	if(result)
	{
		do
		{
			const char *id = result->Fetch()[0].GetString();
			uint32 value = result->Fetch()[1].GetUInt32();
			if(strlen(id) <= 20)
				continue;
			uint32 mapId = atoi(id + 20);
			if(mapId >= NUM_MAPS)
				continue;
			m_nextInstanceReset[mapId] = value;
		}while(result->NextRow());
		delete result;
	}

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
			m_instances[i] = NULL;
		}

		if(m_singleMaps[i] != NULL)
		{
			MapMgr * ptr = m_singleMaps[i];
			ptr->KillThread();
			delete ptr;
			m_singleMaps[i] = NULL;
		}

		if(m_maps[i] != NULL)
		{
			delete m_maps[i];
			m_maps[i] = NULL;
		}
	}

	delete FormationMgr::getSingletonPtr();
}

uint32 InstanceMgr::PreTeleport(uint32 mapid, Player * plr, uint32 instanceid)
{
	// preteleport is where all the magic happens :P instance creation, etc.
	MapInfo *inf = WorldMapInfoStorage.LookupEntry(mapid);
	Group *pGroup;
	InstanceMap *instancemap;
	Instance *in;

	if(inf == NULL || mapid >= NUM_MAPS)
		return INSTANCE_ABORT_NOT_FOUND;

	// main continent check.
	if(inf->type == INSTANCE_NULL)
	{
		// this will be useful when clustering comes into play.
		// we can check if the destination world server is online or not and then cancel them before they load.
		return (m_singleMaps[mapid] != NULL) ? INSTANCE_OK : INSTANCE_ABORT_NOT_FOUND;
	}

	// shouldn't happen
	if(inf->type == INSTANCE_BATTLEGROUND)
		return INSTANCE_ABORT_NOT_FOUND;

	pGroup = plr->GetGroup();

	// players without groups cannot enter raids and heroic instances
	
	if( pGroup == NULL &&
		inf->type == INSTANCE_RAID &&
		!plr->TriggerpassCheat )
		return INSTANCE_ABORT_NOT_IN_RAID_GROUP;

	if( pGroup == NULL &&
		( inf->type == INSTANCE_NONRAID && plr->iInstanceType == MODE_HEROIC ) &&
		!plr->TriggerpassCheat )
		return INSTANCE_ABORT_NOT_IN_RAID_GROUP;
		

	// players without raid groups cannot enter raid instances
	if(pGroup != NULL && pGroup->GetGroupType() != GROUP_TYPE_RAID && inf->type == INSTANCE_RAID && !plr->TriggerpassCheat)
		return INSTANCE_ABORT_NOT_IN_RAID_GROUP;

	// We deny transfer if we requested a heroic instance of a map that has no heroic mode
	// We are trying to enter into a non-multi instance with a heroic group, downscaling
	if( inf->type == INSTANCE_NONRAID && plr->GetDungeonDifficulty() == MODE_HEROIC){
		plr->SetDungeonDifficulty( MODE_NORMAL );
		plr->SendDungeonDifficulty();
		
		Group *grp = plr->GetGroup();
		if( grp != NULL )
			grp->SetDungeonDifficulty( MODE_NORMAL );
	}

	// if it's not a normal / 10men normal then check if we even have this mode
	if( inf->type == INSTANCE_RAID && plr->GetRaidDifficulty() != MODE_NORMAL_10MEN ){
		uint32 newtype = 0;

		if( !inf->HasDifficulty( plr->GetRaidDifficulty() ) ){
			// no it doesn't so we will downscale it

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This part is totally speculative, if you know how this is done actually then do change it
//
			switch( plr->GetRaidDifficulty() ){
				case MODE_NORMAL_25MEN:
				case MODE_HEROIC_10MEN:{
					newtype = MODE_NORMAL_10MEN;
									   break;}

				case MODE_HEROIC_25MEN:{
					newtype = MODE_NORMAL_25MEN;
									   break;}
			}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// check if we have this mode
			if( !inf->HasDifficulty( newtype ) ){
				
				//appearantly we don't so we set to 10men normal, which is the default for old raids too
				//regardless of their playerlimit
				newtype = MODE_NORMAL_10MEN;
			}

			// Setting the new mode on us and our group
			if( plr->GetRaidDifficulty() != newtype ){
				
				plr->SetRaidDifficulty( newtype );
				plr->SendRaidDifficulty();

				Group *grp = plr->GetGroup();
				if( grp != NULL )
					grp->SetRaidDifficulty( newtype );
			}
		}
	}	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If we are here, it means:
	// 1.) We're a simple non-raid and non-heroic dungeon
	// 2.) We're a multi-dungeon set to heroic and we are in a group
	// 3.) We're a raid, and we are in a raid group
	// 4.) We're a raid, we are in a raid group, and we have the right mode set
	//
	// So, first we have to check if they have an instance on this map already, if so, allow them to teleport to that.
	// Otherwise, we will try to create them a new one.
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_mapLock.Acquire();
	instancemap = m_instances[mapid];

	// If there are no instances of this map yet, we need to create the map
	if(instancemap == NULL)
	{
		if(instanceid != 0)
		{
			m_mapLock.Release();
			return INSTANCE_ABORT_NOT_FOUND;
		}

		m_instances[mapid] = new InstanceMap;
		instancemap = m_instances[mapid];
	}
	else
	{
		InstanceMap::iterator itr;

		// this is the case when we enter an already existing instance ( with summons for example )
		if(instanceid != 0)
		{
			itr = instancemap->find(instanceid);
			if(itr != instancemap->end()) {
				in = itr->second;

				if(!CHECK_INSTANCE_GROUP(in, pGroup))
				{
					// Another group is already playing in this instance of the dungeon...
					m_mapLock.Release();
					sChatHandler.SystemMessageToPlr(plr, "Another group is already inside this instance of the dungeon.");
					return INSTANCE_ABORT_NOT_IN_RAID_GROUP;
				}

				// Try to add instance ID to player
				plr->SetPersistentInstanceId(in);

				// Set current group
				if(pGroup)
					in->m_creatorGroup = pGroup->GetID();

				m_mapLock.Release();
				return INSTANCE_OK;
			} else {
				m_mapLock.Release();
				return INSTANCE_ABORT_NOT_FOUND;
			}
		}
		else  // this is the case when we enter the normal way (e.g. we enter thru the portal )
		{
			in = NULL;
			if(pGroup != NULL) // we are in a group
			{

				uint32 grpdiff;

				// We want to use the raid difficulty for raids, and dungeon difficulty for dungeons
				if( inf->type == INSTANCE_RAID )
					grpdiff = pGroup->m_raiddifficulty;
				else
					grpdiff = pGroup->m_difficulty;
				
				if( (inf->type == INSTANCE_MULTIMODE && grpdiff == MODE_HEROIC) || inf->type == INSTANCE_RAID )
				{
					// This is the case when we don't have this map on this difficulty saved yet for the player entering
					if( plr->GetPersistentInstanceId(mapid, grpdiff ) == 0)
					{
						// The group has this instance saved already so we will use it
						if(pGroup->m_instanceIds[mapid][ grpdiff ] != 0)
						{
							in = sInstanceMgr.GetInstanceByIds(mapid, pGroup->m_instanceIds[mapid][ grpdiff ]);
						}
						else if(sWorld.instance_TakeGroupLeaderID)
						{
							PlayerInfo *pLeaderInfo = pGroup->GetLeader();
							if(pLeaderInfo)
							{
								pLeaderInfo->savedInstanceIdsLock.Acquire();
								PlayerInstanceMap::iterator itrLeader = pLeaderInfo->savedInstanceIds[ grpdiff ].find(mapid);
								if(itrLeader != pLeaderInfo->savedInstanceIds[ grpdiff ].end())
								{
									in = sInstanceMgr.GetInstanceByIds(mapid, (*itrLeader).second);
								}
								pLeaderInfo->savedInstanceIdsLock.Release();
							}
						}
					}

					// If we have it saved to the player then use that
					if(in == NULL && plr->GetPersistentInstanceId(mapid, grpdiff ) != 0)
					{
						in = sInstanceMgr.GetInstanceByIds(mapid, plr->GetPersistentInstanceId(mapid, grpdiff ));
					}
				}
				else
				{
					if(pGroup->m_instanceIds[mapid][ grpdiff ] != 0)
					{
						in = sInstanceMgr.GetInstanceByIds(mapid, pGroup->m_instanceIds[mapid][ grpdiff ]);
					}
				}
			}

			if(in == NULL)
			{
				// We are not in a group, so we will look for an instance that we own and has the right difficulty

				uint32 diff;

				if( inf->type == INSTANCE_RAID )
					diff = plr->GetRaidDifficulty();
				else
					diff = plr->GetDungeonDifficulty();

				for(itr = instancemap->begin(); itr != instancemap->end();)
				{
					in = itr->second;
					++itr;

					if(in->m_difficulty == diff && PlayerOwnsInstance(in, plr))
						break;
					
					in = NULL;
				}
			}

			// We've found an instance!
			if(in != NULL)
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

				if(!CHECK_INSTANCE_GROUP(in, pGroup))
				{
					// Another group is already playing in this instance of the dungeon...
					sChatHandler.SystemMessageToPlr(plr, "Another group is already inside this instance of the dungeon.");
					return INSTANCE_ABORT_NOT_IN_RAID_GROUP;
				}
	
				// Try to add instance ID to player
				plr->SetPersistentInstanceId(in);
	
				// Set current group
				if(pGroup)
					in->m_creatorGroup = pGroup->GetID();

				plr->SetInstanceID(in->m_instanceId);

				// found our instance, allow him in.
				return INSTANCE_OK;
			}
		}
	}

	// if we're here, it means we need to create a new instance.
	in = new Instance;
	in->m_creation = UNIXTIME;

	switch( inf->type ){
		case INSTANCE_NONRAID:
		case INSTANCE_MULTIMODE:
			in->m_difficulty = plr->GetDungeonDifficulty(); break;

		case INSTANCE_RAID:
			in->m_difficulty = plr->GetRaidDifficulty(); break;
	}

	in->m_instanceId = GenerateInstanceID();
	in->m_mapId = mapid;
	in->m_mapInfo = inf;
	in->m_mapMgr = NULL;		// always start off without a map manager, it is created in GetInstance()
	in->m_isBattleground = false;
	in->m_persistent = IS_PERSISTENT_INSTANCE(in) && objmgr.m_InstanceBossInfoMap[mapid] == NULL;
	in->m_creatorGuid = pGroup ? 0 : plr->GetLowGUID();		// creator guid is 0 if its owned by a group.
	in->m_creatorGroup = pGroup ? pGroup->GetID() : 0;
	
	if(sWorld.instance_SlidingExpiration)
	{
		if(inf->type == INSTANCE_MULTIMODE && in->m_difficulty == MODE_HEROIC)
			in->m_expiration = UNIXTIME + TIME_DAY;
		else
			in->m_expiration = (inf->type == INSTANCE_NONRAID || (inf->type == INSTANCE_MULTIMODE && in->m_difficulty == MODE_NORMAL)) ? 0 : UNIXTIME + inf->cooldown;
	}
	else
	{
		if(inf->type == INSTANCE_MULTIMODE && in->m_difficulty >= MODE_HEROIC)
		{
			in->m_expiration = UNIXTIME - (UNIXTIME % TIME_DAY) + ( (UNIXTIME % TIME_DAY) > (sWorld.instance_DailyHeroicInstanceResetHour * TIME_HOUR) ? 82800 : -3600 ) + ((sWorld.instance_DailyHeroicInstanceResetHour - sWorld.GMTTimeZone) * TIME_HOUR);
		}
		else if(IS_PERSISTENT_INSTANCE(in))
		{
			if(m_nextInstanceReset[in->m_mapId] == 0)
			{
				m_nextInstanceReset[in->m_mapId] = UNIXTIME - (UNIXTIME % TIME_DAY) - ((sWorld.GMTTimeZone + 1) * TIME_HOUR) + (in->m_mapInfo->cooldown == 0 ? TIME_DAY : in->m_mapInfo->cooldown);
                CharacterDatabase.Execute("DELETE FROM server_settings WHERE setting_id LIKE 'next_instance_reset_%u';", in->m_mapId );
				CharacterDatabase.Execute("INSERT INTO server_settings VALUES ('next_instance_reset_%u', '%u')", in->m_mapId, m_nextInstanceReset[in->m_mapId]);
			}
			if(m_nextInstanceReset[in->m_mapId] + (TIME_MINUTE * 15) < UNIXTIME)
			{
				do
				{
					time_t tmp = m_nextInstanceReset[in->m_mapId];
					if(tmp + (TIME_MINUTE * 15) < UNIXTIME)
						m_nextInstanceReset[in->m_mapId] = tmp + (in->m_mapInfo->cooldown == 0 ? TIME_DAY : in->m_mapInfo->cooldown);
				} while(m_nextInstanceReset[in->m_mapId] + (TIME_MINUTE * 15) < UNIXTIME);
                CharacterDatabase.Execute("DELETE FROM server_settings WHERE setting_id LIKE 'next_instance_reset_%u';", in->m_mapId );
				CharacterDatabase.Execute("INSERT INTO server_settings VALUES ('next_instance_reset_%u', '%u')", in->m_mapId, m_nextInstanceReset[in->m_mapId]);
			}
			in->m_expiration = m_nextInstanceReset[in->m_mapId];
		}
		else
		{
			in->m_expiration = (inf->type == INSTANCE_NONRAID || (inf->type == INSTANCE_MULTIMODE && in->m_difficulty == MODE_NORMAL)) ? 0 : UNIXTIME + inf->cooldown;
		}
	}
	plr->SetInstanceID(in->m_instanceId);
	Log.Debug("InstanceMgr", "Creating instance for player %u and group %u on map %u. (%u)", in->m_creatorGuid, in->m_creatorGroup, in->m_mapId, in->m_instanceId);
	
	// save our new instance to the database.
	in->SaveToDB();
    
	// apply it in the instance map
	instancemap->insert( InstanceMap::value_type( in->m_instanceId, in ) );

	// Try to add instance ID to player
	plr->SetPersistentInstanceId(in);

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
		plr = TO< Player* >( obj );

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
				if(itr->second->m_mapMgr == NULL)
				{
					itr->second->m_mapMgr = _CreateInstance(itr->second);
				}
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

				uint32 difficulty;

				if( in->m_mapInfo->type == INSTANCE_RAID )
					difficulty = plr->GetRaidDifficulty();
				else
					difficulty = plr->GetDungeonDifficulty();

				if(in->m_difficulty == difficulty && PlayerOwnsInstance(in, plr))
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

		// instance is non-existent (shouldn't really happen for units...)
		m_mapLock.Release();
		return NULL;
	}
}

MapMgr * InstanceMgr::_CreateInstance(uint32 mapid, uint32 instanceid)
{
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(mapid);
	   
	Arcemu::Util::ARCEMU_ASSERT(    inf != NULL && inf->type == INSTANCE_NULL );
	Arcemu::Util::ARCEMU_ASSERT(    mapid < NUM_MAPS && m_maps[ mapid ] != NULL );

	Log.Notice("InstanceMgr", "Creating continent %s.", m_maps[mapid]->GetName());

	MapMgr *newMap = new MapMgr( m_maps[mapid], mapid, instanceid );

	Arcemu::Util::ARCEMU_ASSERT(    newMap != NULL );

	// Scheduling the new map for running
	ThreadPool.ExecuteTask( newMap );
	m_singleMaps[mapid] = newMap;

	return newMap;
}

MapMgr * InstanceMgr::_CreateInstance(Instance * in)
{
	if ( m_maps[ in->m_mapId ] == 0 )
		return NULL;

	Log.Notice("InstanceMgr", "Creating saved instance %u (%s)", in->m_instanceId, m_maps[in->m_mapId]->GetName());
	Arcemu::Util::ARCEMU_ASSERT(    in->m_mapMgr == NULL );

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
	if(inf== NULL)
		return;
	if(m_maps[mapid]!= NULL)
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

					if(in->m_mapMgr== NULL)
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
	Log.Success("InstanceMgr", "Deleting Expired Instances...");
	CharacterDatabase.WaitExecute("DELETE FROM instances WHERE expiration > 0 AND expiration <= %u", UNIXTIME);
	CharacterDatabase.Execute("DELETE FROM instanceids WHERE instanceid NOT IN ( SELECT id FROM instances )");
	
	// load saved instances
	result = CharacterDatabase.Query("SELECT id, mapid, creation, expiration, killed_npc_guids, difficulty, creator_group, creator_guid, persistent FROM instances");
	Log.Success("InstanceMgr", "Loading %u saved instances." , result ? result->GetRowCount() : 0);

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
			if(!in->m_persistent && in->m_creatorGroup && objmgr.GetGroupById(in->m_creatorGroup) == NULL)
			{
				CharacterDatabase.Execute("DELETE FROM `instances` WHERE `id` = %u", in->m_instanceId);
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
	m_persistent = fields[8].GetBool();
	m_mapMgr= NULL;
	m_isBattleground = false;

	// Reset group binding if it's a persistent instance
	if(m_persistent)
		m_creatorGroup = 0;

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
	WorldPacket data(SMSG_INSTANCE_RESET, 4);
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

				if (IS_RESETABLE_INSTANCE(in) && (CHECK_INSTANCE_GROUP(in, plr->GetGroup()) || plr->GetLowGUID() == in->m_creatorGuid))
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

				if(in->m_mapMgr && in->m_creatorGroup && in->m_creatorGroup == pGroup->GetID())
				{
					if(IS_RESETABLE_INSTANCE(in))
					{
						_DeleteInstance(in, false);
					}
					else if(in->m_mapMgr->HasPlayers())
					{
						WorldPacket data(SMSG_RAID_GROUP_ONLY, 8);
						data << uint32(60000) << uint32(1);

						for(PlayerStorageMap::iterator mitr = in->m_mapMgr->m_PlayerStorage.begin(); mitr != in->m_mapMgr->m_PlayerStorage.end(); ++mitr)
						{
							if((*mitr).second->IsInWorld() && !(*mitr).second->raidgrouponlysent && (*mitr).second->GetInstanceID() == (int32)in->m_instanceId)
							{
								(*mitr).second->GetSession()->SendPacket(&data);
								(*mitr).second->raidgrouponlysent=true;

								sEventMgr.AddEvent((*mitr).second, &Player::EjectFromInstance, EVENT_PLAYER_EJECT_FROM_INSTANCE, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
							}	
						}
					}
				}
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
	// cleanup all the corpses on this instance
	CharacterDatabase.Execute("DELETE FROM corpses WHERE instanceid = %u", m_instanceId);

	// delete from the database
	CharacterDatabase.Execute("DELETE FROM instances WHERE id = %u", m_instanceId);

	// Delete all instance assignments
	CharacterDatabase.Execute("DELETE FROM instanceids WHERE mapId = %u AND instanceId = %u AND mode = %u", m_mapId, m_instanceId, m_difficulty);
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
				if(in->m_mapInfo->type != INSTANCE_NONRAID && !(in->m_mapInfo->type == INSTANCE_MULTIMODE && in->m_difficulty == MODE_NORMAL) && HasInstanceExpired(in))
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

						data.SetOpcode(SMSG_UPDATE_LAST_INSTANCE);
						data << uint32(in->m_mapId);
						plr->GetSession()->SendPacket(&data);

						data.Initialize(SMSG_UPDATE_INSTANCE_OWNERSHIP);
						data << uint32(0x01);
						plr->GetSession()->SendPacket(&data);
					
						return;
					}
				}
			}
		}
		m_mapLock.Release();
	}

	data.SetOpcode(SMSG_UPDATE_INSTANCE_OWNERSHIP);
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

				if(in->m_persistent && PlayerOwnsInstance(in, plr))
				{
					data << uint32( in->m_mapId );						// obviously the mapid
					data << uint32( in->m_difficulty );					// instance difficulty
					data << uint64( in->m_instanceId );					// self-explanatory
					data << uint8( 1 );									// expired = 0
					data << uint8( 0 );									// extended = 1
					
					if( in->m_expiration > UNIXTIME  )
						data << uint32( in->m_expiration - UNIXTIME );
					else
						data << uint32( 0 );

					++counter;
				}
			}
		}
	}
	m_mapLock.Release();


	*(uint32*)&data.contents()[0] = counter;

	plr->GetSession()->SendPacket(&data);
}

void Instance::SaveToDB()
{
	// don't save non-raid instances.
	if(m_mapInfo->type == INSTANCE_NONRAID || m_isBattleground)
		return;

	std::stringstream ss;

    ss << "DELETE FROM instances WHERE id = " << m_instanceId << ";";
    
    CharacterDatabase.Execute( ss.str().c_str() );

    ss.rdbuf()->str("");

	set<uint32>::iterator itr;

	ss << "INSERT INTO instances VALUES("
		<< m_instanceId << ","
		<< m_mapId << ","
		<< (uint32)m_creation << ","
		<< (uint32)m_expiration << ",'";

	for(itr = m_killedNpcs.begin(); itr != m_killedNpcs.end(); ++itr)
		ss << (*itr) << " ";

	ss << "',"
		<< m_difficulty << ","
		<< m_creatorGroup << ","
		<< m_creatorGuid << ","
		<< m_persistent << ")";

	CharacterDatabase.Execute( ss.str().c_str() );
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
	pInstance->m_persistent = false;
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

MapMgr * InstanceMgr::CreateInstance(uint32 instanceType, uint32 mapid)
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
	pInstance->m_persistent = false;
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

// should be safe to do for instances of any non-world maps
void InstanceMgr::SafeDeleteInstance(MapMgr * mgr)
{
	if (!mgr)
		return;

	uint32 mapId = mgr->GetMapId();
	uint32 instanceId = mgr->GetInstanceID();

	// Shutdown Instance
	mgr->TeleportPlayers(); // Get everyone out
	mgr->InstanceShutdown();

	DeleteBattlegroundInstance( mapId, instanceId );
}

void InstanceMgr::DeleteBattlegroundInstance(uint32 mapid, uint32 instanceid)
{
	m_mapLock.Acquire();
	InstanceMap::iterator itr = m_instances[mapid]->find( instanceid );
	if( itr == m_instances[mapid]->end() )
	{
		sLog.outError("Could not delete battleground instance!");
		m_mapLock.Release();
		return;
	}

	_DeleteInstance( itr->second, true );
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

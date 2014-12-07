/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "git_version.h"

#define ENGINE_NAME "LuaBridge" //You should check in your scripts that GetLuaEngine() == "LuaBridge"
#ifdef LUA_SVN_REVISION_H
#define ENGINE_VERSION LUA_REVISION //If you need a specific version for your script to work, check GetLuaEngineVersion() >= number
#else
#define ENGINE_VERSION 0 //linux people
#endif

/*static int PerformIngameSpawn(lua_State * L)
{
	uint32 spawntype = luaL_checkint(L, 1);
	uint32 entry = luaL_checkint(L, 2);
	uint32 map = luaL_checkint(L, 3);
	float x = CHECK_FLOAT(L, 4);
	float y = CHECK_FLOAT(L, 5);
	float z = CHECK_FLOAT(L, 6);
	float o = CHECK_FLOAT(L, 7);
	uint32 faction = luaL_checkint(L, 8); //also scale as percentage
	uint32 duration = luaL_checkint(L, 9);
	uint32 equip1 = luaL_optint(L, 10, 1);
	uint32 equip2 = luaL_optint(L, 11, 1);
	uint32 equip3 = luaL_optint(L, 12, 1);
	//13: instance id
	uint32 save = luaL_optint(L, 14, 0);
	if(x && y && z && entry)
	{
		if (spawntype == 1) //Unit
		{
			CreatureProto *p = CreatureProtoStorage.LookupEntry(entry);
			CreatureInfo *i = CreatureNameStorage.LookupEntry(entry);
			if (p == NULL || i == NULL)
				return 0;
			MapMgr *mapMgr = sInstanceMgr.GetMapMgr(map);
			if (!mapMgr)
				return 0;
			int32 instanceid = luaL_optint(L, 13, mapMgr->GetInstanceID());
			CreatureSpawn * sp = new CreatureSpawn;
			uint32 gender = i->GenerateModelId(&sp->displayid);
 			sp->entry = entry;
			sp->form = 0;
			sp->id = objmgr.GenerateCreatureSpawnID();
			sp->movetype = 0;
			sp->x = x;
			sp->y = y;
			sp->z = z;
			sp->o = o;
			sp->emote_state = 0;
			sp->flags = 0;
			sp->factionid = faction;
			sp->bytes0 = sp->setbyte(0,2,gender);
			sp->bytes1 = 0;
			sp->bytes2 = 0;
			//sp->respawnNpcLink = 0;
			sp->stand_state = 0;
			sp->death_state = 0;
			sp->channel_target_creature = sp->channel_target_go = sp->channel_spell = 0;
			sp->MountedDisplayID = 0;
			sp->Item1SlotDisplay = equip1;
			sp->Item2SlotDisplay = equip2;
			sp->Item3SlotDisplay = equip3;
			sp->CanFly = 0;
			sp->phase = 1;
			Creature * pCreature = mapMgr->CreateCreature(entry);
			pCreature->Load(sp, (uint32)NULL, NULL);
			pCreature->m_loadedFromDB = true;
			pCreature->SetFaction(faction);
			pCreature->SetInstanceID(instanceid);
			pCreature->SetMapId(map);
			pCreature->SetEquippedItem(MELEE,equip1);
			pCreature->SetEquippedItem(OFFHAND,equip2);
			pCreature->SetEquippedItem(RANGED,equip3);
			pCreature->m_noRespawn = true;
			pCreature->PushToWorld(mapMgr);
			if (duration>0)
				pCreature->Despawn(duration,0);
			if (save)
				pCreature->SaveToDB();
			PUSH_UNIT(L,pCreature);
		}
		else if (spawntype == 2) //GO
		{
			GameObjectInfo *n = GameObjectNameStorage.LookupEntry(entry);
			if (n == NULL)
				return 0;
			MapMgr *mapMgr = sInstanceMgr.GetMapMgr(map);
			if (!mapMgr)
				return 0;
			int32 instanceid = luaL_optint(L, 13, mapMgr->GetInstanceID());

			GameObject *go = mapMgr->CreateGameObject(entry);
			go->SetInstanceID(instanceid);
			go->CreateFromProto(entry,map,x,y,z,o);
			go->Phase(PHASE_SET, 1);
			go->SetScale( ((float)faction) / 100.0f );
			// Create spawn instance
			GOSpawn * gs = new GOSpawn;
			gs->entry = go->GetEntry();
			gs->facing = go->GetOrientation();
			gs->faction = go->GetFaction();
			gs->flags = go->GetUInt32Value(GAMEOBJECT_FLAGS);
			gs->id = objmgr.GenerateGameObjectSpawnID();
			gs->o = 0.0f;
			gs->o1 = go->GetParentRotation(0);
			gs->o2 = go->GetParentRotation(2);
			gs->o3 = go->GetParentRotation(3);
			gs->scale = go->GetScale();
			gs->x = go->GetPositionX();
			gs->y = go->GetPositionY();
			gs->z = go->GetPositionZ();
			gs->state = go->GetByte(GAMEOBJECT_BYTES_1, 0);
			gs->phase = go->GetPhase();

			go->m_spawn = gs;
			go->PushToWorld(mapMgr);

			if (duration)
				sEventMgr.AddEvent(go,&GameObject::ExpireAndDelete,EVENT_GAMEOBJECT_UPDATE,duration,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			if (save)
				go->SaveToDB();
			PUSH_GO(L,go);
		}
		else
			lua_pushnil(L);
	}
	else
		lua_pushnil(L);
	return 1;
}*/

static Player* GetPlayer(const char* name)
{
	Player* plr = objmgr.GetPlayer(name);
	if(plr != NULL && !plr->IsInWorld())
		plr = NULL;
	return plr;
}

ARCEMU_FORCEINLINE static const char* enginename()  //also mapped to GetLuaEngine()
{
	return ENGINE_NAME;
}

ARCEMU_FORCEINLINE static ptrdiff_t engineversion()
{
	return ENGINE_VERSION;
}

ARCEMU_FORCEINLINE static void logcolor(ptrdiff_t color)
{
	//Log.Color(color);
}
ARCEMU_FORCEINLINE static QueryResult* WorldDBQuery(const char* query)
{
	if(query != NULL)
		return WorldDatabase.Query(query);
	return NULL;
}

ARCEMU_FORCEINLINE static QueryResult* CharDBQuery(const char* query)
{
	if(query != NULL)
		return CharacterDatabase.Query(query);
	return NULL;
}

static void SendWorldMessage(const char* msg, uint32 msg_type)
{
	if(msg != NULL && msg_type)
	{
		if(msg_type == 1)
			sWorld.SendWorldWideScreenText(msg);
		else if(msg_type == 2)
			sWorld.SendWorldText(msg);
	}
}

static void ReloadTable(const char* TableName)
{
	if(TableName == NULL)
		return;

	if(!stricmp(TableName, "spell_disable"))
		objmgr.ReloadDisabledSpells();
	else if(!stricmp(TableName, "vendors"))
		objmgr.ReloadVendors();
	else
	{
		if(!stricmp(TableName, "items"))					// Items
			ItemPrototypeStorage.Reload();
		else if(!stricmp(TableName, "creature_proto"))		// Creature Proto
			CreatureProtoStorage.Reload();
		else if(!stricmp(TableName, "creature_names"))		// Creature Names
			CreatureNameStorage.Reload();
		else if(!stricmp(TableName, "gameobject_names"))	// GO Names
			GameObjectNameStorage.Reload();
		else if(!stricmp(TableName, "areatriggers"))		// Areatriggers
			AreaTriggerStorage.Reload();
		else if(!stricmp(TableName, "itempages"))			// Item Pages
			ItemPageStorage.Reload();
		else if(!stricmp(TableName, "worldstring_tables"))			// wst
			WorldStringTableStorage.Reload();
		else if(!stricmp(TableName, "worldbroadcast"))			// wb
			WorldBroadCastStorage.Reload();
		else if(!stricmp(TableName, "quests"))				// Quests
			QuestStorage.Reload();
		else if(!stricmp(TableName, "npc_text"))			// NPC Text Storage
			NpcTextStorage.Reload();
		else if(!stricmp(TableName, "fishing"))				// Fishing Zones
			FishingZoneStorage.Reload();
		else if(!stricmp(TableName, "teleport_coords"))		// Teleport coords
			TeleportCoordStorage.Reload();
		else if(!stricmp(TableName, "graveyards"))			// Graveyards
			GraveyardStorage.Reload();
		else if(!stricmp(TableName, "worldmap_info"))		// WorldMapInfo
			WorldMapInfoStorage.Reload();
		else if(!stricmp(TableName, "zoneguards"))
			ZoneGuardStorage.Reload();
		else if(!stricmp(TableName, "unit_display_sizes"))
			UnitModelSizeStorage.Reload();
		else if(!stricmp(TableName, "command_overrides"))	// Command Overrides
		{
			CommandTableStorage::getSingleton().Dealloc();
			CommandTableStorage::getSingleton().Init();
			CommandTableStorage::getSingleton().Load();
		}
	}
}

static void ReloadLuaEngine()
{
	//we can do this since LUA_COMPILER object is global and visible by all threads and this works out nicely since scripts are reloaded properly.
	le::restartEngine();
}

static void GetPlayersInWorld(lua_stack stack)
{
	ptrdiff_t count = 1;
	//push a new table.
	lua_newtable((lua_thread)stack);
	objmgr._playerslock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Player*>::const_iterator itr;
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		lua_pushinteger((lua_thread)stack, count);
		luabridge::tdstack<Player*>::push((lua_thread)stack, (*itr).second);
		lua_rawset((lua_thread)stack, -3);
		++count;
	}
	objmgr._playerslock.ReleaseReadLock();
}

static void Rehash()
{
	sWorld.Rehash(true);
}

static const char* GetArcemuRevision()
{
	return BUILD_HASH_STR;
}

/*static int GetInstanceIdsByMap(lua_State * L)
{
	uint32 mapid = luaL_checkint(L,1);
	uint32 ret = NULL;
	uint32 count = 0;
	lua_newtable(L);

	InstanceMap * instancemap = sInstanceMgr.GetInstanceMap(mapid);
	for(InstanceMap::iterator itr = instancemap->begin(); itr != instancemap->end(); ++itr)
	{
		count++,
		ret = itr->second->m_instanceId;
		lua_pushinteger(L,count);
		lua_pushinteger(L,ret);
		lua_rawset(L,-3);
	}
	return 1;
}*/
//////////////////////////////////////////////////////////////////////////
// WORLD PVP NOT SUPPORTED!
//////////////////////////////////////////////////////////////////////////
/*
static int SendPvPCaptureMessage(lua_State * L)
{
	uint32 zoneid = luaL_checkint(L, 1);
	const char* msg = luaL_checkstring(L, 2);
	AreaTable * at = dbcArea.LookupEntry(zoneid);
	if(!zoneid || !msg || !at)
		return 1;
	MapMgr* mapmgr = sInstanceMgr.GetMapMgr(at->mapId);
	if (mapmgr)
		mapmgr->SendPvPCaptureMessage(ZONE_MASK_ALL, zoneid, msg);
	return 1;
}
*/
static void GetPlayersInMap(uint32 map_id, lua_stack stack)
{
	uint32 count = 1;
	lua_newtable((lua_thread)stack);
	MapMgr* mgr = sInstanceMgr.GetMapMgr((uint32)map_id);
	if(mgr != NULL)
	{
		for(PlayerStorageMap::iterator itr = mgr->m_PlayerStorage.begin(); itr != mgr->m_PlayerStorage.end(); ++itr)
		{
			lua_pushinteger((lua_thread)stack, count);
			luabridge::tdstack<Player*>::push((lua_thread)stack, itr->second);
			lua_rawset((lua_thread)stack , -3);
			count++;
		}
	}
}

static void GetPlayersInZone(uint32 zone_id, lua_stack stack)
{
	ptrdiff_t count = 1;
	lua_newtable((lua_thread)stack);
	objmgr._playerslock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Player*>::const_iterator itr;
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if(itr->second->GetZoneId() == (uint32)zone_id)
		{
			lua_pushinteger((lua_thread)stack, count);
			luabridge::tdstack<Player*>::push((lua_thread)stack, itr->second);
			lua_rawset((lua_thread)stack, -3);
			++count;
		}
	}
	objmgr._playerslock.ReleaseReadLock();
}

/*static int SendMail(lua_State * L)
{
	uint32 type = luaL_checkint(L,1);
	uint64 sender_guid = CHECK_GUID(L,2);
	uint64 recipient_guid = CHECK_GUID(L,3);
	string subject = luaL_checkstring(L,4);
	string body = luaL_checkstring(L,5);
	uint32 money = luaL_checkint(L,6);
	uint32 cod = luaL_checkint(L,7);
	uint64 item_guid = CHECK_GUID(L,8);
	uint32 stationery = luaL_checkint(L,9);
	sMailSystem.SendAutomatedMessage(type, sender_guid, recipient_guid, subject, body, money, cod, item_guid, stationery);
	return 0;
}*/

uint32 luabit_and(uint32 left, lua_stack stack)
{
	uint32 top = lua_gettop((lua_thread)stack);
	if(top > 1)
	{
		for(uint32 i = 2; i <= top; ++i)
		{
			if(lua_isnumber((lua_thread)stack, i))
				left &= (uint32)luaL_checkint((lua_thread)stack, i);
		}
	}
	return left;
}
uint32 luabit_or(uint32 left, lua_stack stack)
{
	uint32 top = lua_gettop((lua_thread)stack);
	if(top > 1)
	{
		for(uint32 i = 2; i <= top; ++i)
		{
			if(lua_isnumber((lua_thread)stack, i))
				left |= (uint32)luaL_checkint((lua_thread)stack, i);
		}
	}
	return left;
}
uint32 luabit_xor(uint32 left, lua_stack stack)
{
	uint32 top = lua_gettop((lua_thread)stack);
	if(top > 1)
	{
		for(uint32 i = 2; i <= top; ++i)
		{
			if(lua_isnumber((lua_thread)stack, i))
				left ^= (uint32)luaL_checkint((lua_thread)stack, i);
		}
	}
	return left;
}
uint32 luabit_not(uint32 val)
{
	return (~val);
}
uint32 bit_shiftleft(uint32 left, uint8 count)
{
	count &= 0x7F;
	return (left << count);
}
uint32 bit_shiftright(uint32 left, uint8 count)
{
	count &= 0x7F;
	return (left >> count);
}
ARCEMU_FORCEINLINE const char* GetPlatform()
{
	return PLATFORM_TEXT;
}
uint64 NumberToGUID(uint32 num)
{
	return (uint64)num;
}
void SendPacketToZone(WorldPacket* dat, uint32 zone_id)
{
	if(dat != NULL && zone_id)
		sWorld.SendZoneMessage(dat, zone_id);
}

void SendPacketToInstance(WorldPacket* pack, uint32 instance)
{
	if(pack != NULL && instance)
		sWorld.SendInstanceMessage(pack, instance);
}

void SendPacketToWorld(WorldPacket* pack)
{
	if(pack)
		sWorld.SendGlobalMessage(pack);
}

void SendPacketToChannel(WorldPacket* pack, const char* cname, int team)
{
	if(pack != NULL && cname != NULL)
	{
		Channel* pChannel = channelmgr.GetChannel(cname, team);
		if(pChannel)
			pChannel->SendToAll(pack);
	}
}

Creature* GetInstanceCreature(uint32 map, uint32 instance, lua_stack stack)
{
	uint64 guid = 0;
	uint32 spawnId = 0;
	if(lua_type((lua_thread)stack, 3) == LUA_TNUMBER)
		spawnId = (uint32)luaL_checkint((lua_thread)stack, 3);
	else
		guid = luabridge::tdstack<uint64 &>::get((lua_thread)stack, 3);

	Instance* pInstance = sInstanceMgr.GetInstanceByIds(map, instance);
	if(pInstance == NULL || (!guid && !spawnId))
		return NULL;
	if(guid)
		return pInstance->m_mapMgr->GetCreature(GET_LOWGUID_PART(guid));
	else
		return pInstance->m_mapMgr->GetSqlIdCreature(spawnId);

	return NULL;
}

uint32 GetInstancePlayerCount(uint32 map, uint32 instance)
{
	Instance* pInstance = sInstanceMgr.GetInstanceByIds(map, instance);
	if(pInstance != NULL)
		return pInstance->m_mapMgr->GetPlayerCount();
	return 0;
}

void GetPlayersInInstance(uint32 map, uint32 instance, lua_stack stack)
{
	Instance* pInstance = sInstanceMgr.GetInstanceByIds(map, instance);
	if(pInstance != NULL && pInstance->m_mapMgr != NULL)
	{
		int count = 1;
		lua_newtable((lua_thread)stack);
		MapMgr* mgr = pInstance->m_mapMgr;
		for(PlayerStorageMap::iterator itr = mgr->m_PlayerStorage.begin(); itr != mgr->m_PlayerStorage.end(); ++itr)
		{
			lua_pushinteger((lua_thread)stack, count);
			luabridge::tdstack<Player*>::push((lua_thread)stack, itr->second);
			lua_rawset((lua_thread)stack, -3);
		}
	}
}

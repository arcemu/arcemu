/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
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
#ifndef GOFUNCTIONS_H
#define GOFUNCTIONS_H

#include "StdAfx.h"
#include "LUAEngine.h"

class LuaGameObject
{
	public:
		static int GossipCreateMenu(lua_State* L, GameObject* ptr)
		{
			int text_id = luaL_checkint(L, 1);
			Player* target = CHECK_PLAYER(L, 2);
			int autosend = luaL_checkint(L, 3);
			if(!target || !ptr)
				return 0;
			
			if( Menu != NULL )
				delete Menu;
			
			Menu = new Arcemu::Gossip::Menu( ptr->GetGUID(), text_id );
			
			if( autosend )
				Menu->Send( target );
			
			return 0;
		}
		
		static int GossipMenuAddItem(lua_State * L, GameObject * ptr){
			int icon = luaL_checkint(L, 1);
			const char * menu_text = luaL_checkstring(L, 2);
			int IntId = luaL_checkint(L, 3);
			bool coded = (luaL_checkint(L, 4)) ? true : false;
			const char * boxmessage = luaL_optstring(L,5,"");
			uint32 boxmoney = luaL_optint(L,6,0);

			if( Menu == NULL ){
				LOG_ERROR( "There is no menu to add items to!" );
				return 0;
			}
			
			Menu->AddItem( icon, menu_text, IntId, boxmoney, boxmessage, coded );
			return 0;
		}

		static int GossipSendMenu(lua_State* L, GameObject* ptr)
		{
			Player* target = CHECK_PLAYER(L, 1);
			if(!target)
				return 0;

			if( Menu == NULL ){
				LOG_ERROR( "There is no menu to send!" );
				return 0;
			}
			
			Menu->Send( target );

			return 0;
		}

		static int GossipComplete(lua_State* L, GameObject* ptr)
		{
			Player* target = CHECK_PLAYER(L, 1);
			if(!target)
				return 0;

			if( Menu == NULL ){
				LOG_ERROR( "There is no menu to complete!" );
				return 0;
			}
			
			Menu->Complete( target );
			
			return 0;
		}

		static int GossipSendPOI(lua_State* L, GameObject* ptr)
		{
			Player* plr = CHECK_PLAYER(L, 1);
			float x = CHECK_FLOAT(L, 2);
			float y = CHECK_FLOAT(L, 3);
			int icon = luaL_checkint(L, 4);
			int flags = luaL_checkint(L, 5);
			int data = luaL_checkint(L, 6);
			const char* name = luaL_checkstring(L, 7);
			if(!plr)
				return 0;

			plr->Gossip_SendPOI(x, y, icon, flags, data, name);
			return 0;
		}

		static int GossipSendQuickMenu( lua_State *L, GameObject *ptr ){
			TEST_GO()

			uint32 text_id = luaL_checkint( L, 1 );
			Player *player = CHECK_PLAYER( L, 2 );
			uint32 itemid = luaL_checkint( L, 3 );
			uint8 itemicon = CHECK_UINT8( L, 4 );
			const char *itemtext = luaL_checkstring( L, 5 );
			uint32 requiredmoney = CHECK_ULONG( L, 6 );
			const char *moneytext = luaL_checkstring( L, 7 );
			uint8 extra = CHECK_UINT8( L, 8 );

			if( player == NULL )
				return 0;

			Arcemu::Gossip::Menu::SendQuickMenu( ptr->GetGUID(), text_id, player, itemid, itemicon, itemtext, requiredmoney, moneytext, extra );
			
			return 0;
		}

		static int RegisterAIUpdate(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 time = CHECK_ULONG(L, 1);
			sEventMgr.AddEvent(ptr, &GameObject::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, time, 0, 0);
			return 0;
		}

		static int ModAIUpdate(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 newtime = CHECK_ULONG(L, 1);
			sEventMgr.ModifyEventTimeAndTimeLeft(ptr, EVENT_SCRIPT_UPDATE_EVENT, newtime);
			return 0;
		}

		static int RemoveAIUpdate(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			sEventMgr.RemoveEvents(ptr, EVENT_SCRIPT_UPDATE_EVENT);
			return 0;
		}

		/*int EventCastSpell(lua_State * L, GameObject * ptr)
		{
			uint32 guid = luaL_checkint(L,1);
			uint32 sp = luaL_checkint(L,2);
			uint32 delay = luaL_checkint(L,3);
			uint32 repeats = luaL_checkint(L,4);
			if(guid && sp && delay)
			{
				sEventMgr.AddEvent(ptr,&GameObject::EventCastSpell,guid,sp,false,EVENT_GAMEOBJECT_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
			return 0;
		}*/

		static int GetMapId(lua_State* L, GameObject* ptr)
		{
			(ptr != NULL) ? lua_pushinteger(L, ptr->GetMapId()) : lua_pushnil(L);
			return 1;
		}

		static int RemoveFromWorld(lua_State* L, GameObject* ptr)
		{
			if(ptr)
				ptr->RemoveFromWorld(true);
			return 0;
		}

		static int GetName(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			if(!ptr->GetInfo())
				return 0;
			lua_pushstring(L, ptr->GetInfo()->Name);
			return 1;
		}

		static int GetCreatureNearestCoords(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			float x = CHECK_FLOAT(L, 1);
			float y = CHECK_FLOAT(L, 2);
			float z = CHECK_FLOAT(L, 3);
			uint32 entryid = CHECK_ULONG(L, 4);
			Creature* crc = ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x, y, z, entryid);
			if(crc != NULL)
				PUSH_UNIT(L, crc);
			else
				lua_pushnil(L);
			return 1;
		}

		static int GetGameObjectNearestCoords(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			float x = CHECK_FLOAT(L, 1);
			float y = CHECK_FLOAT(L, 2);
			float z = CHECK_FLOAT(L, 3);
			uint32 entryid = CHECK_ULONG(L, 4);
			GameObject* go = ptr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(x, y, z, entryid);
			if(go != NULL)
				PUSH_GO(L, go);
			else
				lua_pushnil(L);
			return 1;
		}

		static int GetClosestPlayer(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			float d2 = 0;
			float dist = 0;
			Player* ret = NULL;

			for(set<Object*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); ++itr)
			{
				d2 = (TO< Player* >(*itr))->GetDistanceSq(ptr);
				if(!ret || d2 < dist)
				{
					dist = d2;
					ret = TO< Player* >(*itr);
				}
			}
			if(ret == NULL)
				lua_pushnil(L);
			else
				PUSH_UNIT(L, ret);
			return 1;
		}

		static int GetDistance(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			Object* target = CHECK_OBJECT(L, 1);
			lua_pushnumber(L, ptr->GetDistance2dSq(target));
			return 1;
		}

		static int IsInWorld(lua_State* L, GameObject* ptr)
		{
			if(ptr)
			{
				if(ptr->IsInWorld())
				{
					lua_pushboolean(L, 1);
					return 1;
				}
			}
			lua_pushboolean(L, 0);
			return 1;
		}

		static int GetZoneId(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			lua_pushinteger(L, ptr->GetZoneId());
			return 1;
		}

		static int PlaySoundToSet(lua_State* L, GameObject* ptr)
		{
			if(!ptr) return 0;
			int soundid = luaL_checkint(L, 1);
			ptr->PlaySoundToSet(soundid);
			return 0;
		}

		static int SpawnCreature(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 entry = CHECK_ULONG(L, 1);
			float x = CHECK_FLOAT(L, 2);
			float y = CHECK_FLOAT(L, 3);
			float z = CHECK_FLOAT(L, 4);
			float o = CHECK_FLOAT(L, 5);
			uint32 faction = CHECK_ULONG(L, 6);
			uint32 duration = CHECK_ULONG(L, 7);
			uint32 equip1 = luaL_optint(L, 8, 1);
			uint32 equip2 = luaL_optint(L, 9, 1);
			uint32 equip3 = luaL_optint(L, 10, 1);
			uint32 phase = luaL_optint(L, 11, ptr->m_phase);
			bool save = luaL_optint(L, 12, 0) ? true : false;

			if(!entry)
			{
				lua_pushnil(L);
				return 1;
			}
			CreatureProto* p = CreatureProtoStorage.LookupEntry(entry);
			CreatureInfo* i = CreatureNameStorage.LookupEntry(entry);

			if(p == NULL || i == NULL)
			{
				lua_pushnil(L);
				return 1;
			}
			Creature* pCreature = ptr->GetMapMgr()->CreateCreature(entry);
			if(pCreature == NULL)
			{
				lua_pushnil(L);
				return 1;
			}
			pCreature->Load(p, x, y, z, o);
			pCreature->m_loadedFromDB = true;
			pCreature->SetFaction(faction);
			pCreature->SetEquippedItem(MELEE, equip1);
			pCreature->SetEquippedItem(OFFHAND, equip2);
			pCreature->SetEquippedItem(RANGED, equip3);
			pCreature->Phase(PHASE_SET, phase);
			pCreature->m_noRespawn = true;
			pCreature->PushToWorld(ptr->GetMapMgr());
			if(duration)
				pCreature->Despawn(duration, 0);
			if(save)
				pCreature->SaveToDB();
			PUSH_UNIT(L, pCreature);
			return 1;
		}

		static int SpawnGameObject(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			uint32 entry_id = CHECK_ULONG(L, 1);
			float x = CHECK_FLOAT(L, 2);
			float y = CHECK_FLOAT(L, 3);
			float z = CHECK_FLOAT(L, 4);
			float o = CHECK_FLOAT(L, 5);
			uint32 duration = CHECK_ULONG(L, 6);
			float scale = (float)(luaL_optint(L, 7, 100) / 100.0f);
			uint32 phase = luaL_optint(L, 8, ptr->m_phase);
			bool save = luaL_optint(L, 9, 0) ? true : false;
			if(!entry_id)
				return 0;

			GameObject* go = ptr->GetMapMgr()->CreateGameObject(entry_id);
			uint32 mapid = ptr->GetMapId();
			go->CreateFromProto(entry_id, mapid, x, y, z, o);
			go->Phase(PHASE_SET, phase);
			go->SetScale(scale);
			go->AddToWorld( ptr->GetMapMgr() );

			if(duration)
				sEventMgr.AddEvent(go, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_UPDATE, duration, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			if(save)
				go->SaveToDB();
			PUSH_GO(L, go);
			return 1;
		}

		static int GetSpawnX(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetSpawnX());
			return 1;
		}

		static int GetSpawnY(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetSpawnY());
			return 1;
		}

		static int GetSpawnZ(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetSpawnZ());
			return 1;
		}

		static int GetSpawnO(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetSpawnO());
			return 1;
		}

		static int GetX(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetPositionX());
			return 1;
		}

		static int GetY(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetPositionY());
			return 1;
		}

		static int GetZ(lua_State* L, GameObject* ptr)
		{
			TEST_GO()if(ptr)
				lua_pushnumber(L, ptr->GetPositionZ());
			return 1;
		}

		static int GetO(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetOrientation());
			return 1;
		}

		static int GetInRangePlayersCount(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetInRangePlayersCount());
			return 1;
		}

		static int GetEntry(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->GetEntry());
			return 1;
		}

		static int SetOrientation(lua_State* L, GameObject* ptr)
		{
			float newo = CHECK_FLOAT(L, 1);
			if(!newo | !ptr)
				return 0;
			ptr->SetOrientation(newo);
			return 0;
		}

		static int CalcRadAngle(lua_State* L, GameObject* ptr)
		{
			float x = CHECK_FLOAT(L, 1);
			float y = CHECK_FLOAT(L, 2);
			float x2 = CHECK_FLOAT(L, 3);
			float y2 = CHECK_FLOAT(L, 4);
			if(!x || !y || !x2 || !y2 || !ptr)
				return 0;
			lua_pushnumber(L, ptr->calcRadAngle(x, y, x2, y2));
			return 1;
		}

		static int GetInstanceID(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			if(ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
				lua_pushnil(L);
			else
				lua_pushinteger(L, ptr->GetInstanceID());
			return 1;
		}

		static int GetInRangePlayers(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			uint32 count = 0;
			lua_newtable(L);
			for(std::set<Object*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
			{
				if((*itr)->IsUnit())
				{
					count++,
					      lua_pushinteger(L, count);
					PUSH_UNIT(L, *itr);
					lua_rawset(L, -3);
				}
			}
			return 1;
		}

		static int GetInRangeGameObjects(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			uint32 count = 0;
			lua_newtable(L);
			for(std::set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
			{
				if((*itr)->IsGameObject())
				{
					count++,
					      lua_pushinteger(L, count);
					PUSH_GO(L, *itr);
					lua_rawset(L, -3);
				}
			}
			return 1;
		}

		static int GetInRangeUnits(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			uint32 count = 0;
			lua_newtable(L);
			for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); itr++)
			{
				if((*itr) ->IsUnit())
				{
					count++,
					      lua_pushinteger(L, count);
					PUSH_UNIT(L, *itr);
					lua_rawset(L, -3);
				}
			}
			return 1;
		}

		static int IsInFront(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			Object* target = CHECK_OBJECT(L, 1);
			if(!target)
			{
				lua_pushnil(L);
				return 1;
			}
			if(ptr->isInFront(target))
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}

		static int IsInBack(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			Object* target = CHECK_OBJECT(L, 1);
			if(!target)
			{
				lua_pushnil(L);
				return 1;
			}
			if(ptr->isInBack(target))
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
			return 1;
		}

		static int GetUInt32Value(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			if(ptr && field > 0)
				lua_pushinteger(L, ptr->GetUInt32Value(field));
			else
				lua_pushnil(L);
			return 1;
		}

		static int GetUInt64Value(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			if(ptr && field)
				PUSH_GUID(L, ptr->GetUInt64Value(field));
			else
				lua_pushnil(L);
			return 1;
		}

		static int SetUInt32Value(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			int value = luaL_checkint(L, 2);
			if(ptr && field)
				ptr->SetUInt32Value(field, value);
			return 0;
		}

		static int SetUInt64Value(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			uint64 guid = CHECK_GUID(L, 1);
			if(ptr && field)
				ptr->SetUInt64Value(field, guid);
			return 0;
		}

		static int SetFloatValue(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			float value = CHECK_FLOAT(L, 2);
			if(ptr)
				ptr->SetFloatValue(field, value);
			return 0;
		}

		static int RemoveFlag(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			int value = luaL_checkint(L, 2);
			if(ptr)
				ptr->RemoveFlag(field, value);
			return 0;
		}

		static int SetFlag(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			int value = luaL_checkint(L, 2);
			if(ptr)
				ptr->SetFlag(field, value);
			return 0;
		}


		static int Update(lua_State* L, GameObject* ptr)
		{
			//just despawns/respawns to update GO visuals
			//credits: Sadikum
			TEST_GO()
			MapMgr* mapmgr = ptr->GetMapMgr();
			uint32 NewGuid = mapmgr->GenerateGameobjectGuid();
			ptr->RemoveFromWorld(true);
			ptr->SetNewGuid(NewGuid);
			ptr->PushToWorld(mapmgr);
			ptr->SaveToDB();
			return 0;
		}

		static int GetFloatValue(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			if(ptr && field)
				lua_pushnumber(L, ptr->GetFloatValue(field));
			else
				lua_pushnil(L);
			return 1;
		}

		static int ModUInt32Value(lua_State* L, GameObject* ptr)
		{
			int field = luaL_checkint(L, 1);
			int value = luaL_checkint(L, 2);
			if(ptr && field)
				ptr->ModSignedInt32Value(field, value);
			return 0;
		}

		static int CastSpell(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 sp = CHECK_ULONG(L, 1);
			if(sp)
			{
				Spell* tSpell = sSpellFactoryMgr.NewSpell(ptr, dbcSpell.LookupEntry(sp), true, NULL);
				SpellCastTargets tar(ptr->GetGUID());
				tSpell->prepare(&tar);
			}
			return 0;
		}
		static int CastSpellOnTarget(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 sp = CHECK_ULONG(L, 1);
			Object* target = CHECK_OBJECT(L, 2);
			if(sp && target != NULL)
			{
				Spell* tSpell = sSpellFactoryMgr.NewSpell(ptr, dbcSpell.LookupEntry(sp), true, NULL);
				SpellCastTargets spCastTargets(target->GetGUID());
				tSpell->prepare(&spCastTargets);
			}
			return 0;
		}

		static int GetLandHeight(lua_State* L, GameObject* ptr)
		{
			float x = CHECK_FLOAT(L, 1);
			float y = CHECK_FLOAT(L, 2);
			if(!ptr || !x || !y)
				lua_pushnil(L);
			else
			{
				float lH = ptr->GetMapMgr()->GetADTLandHeight(x, y);
				lua_pushnumber(L, lH);
			}
			return 1;
		}

		static int SetZoneWeather(lua_State* L, GameObject* ptr)
		{
			/*
			WEATHER_TYPE_NORMAL            = 0, // NORMAL (SUNNY)
			WEATHER_TYPE_FOG               = 1, // FOG
			WEATHER_TYPE_RAIN              = 2, // RAIN
			WEATHER_TYPE_HEAVY_RAIN        = 4, // HEAVY_RAIN
			WEATHER_TYPE_SNOW              = 8, // SNOW
			WEATHER_TYPE_SANDSTORM         = 16 // SANDSTORM
			*/
			uint32 zone_id = CHECK_ULONG(L, 1);
			uint32 type = CHECK_ULONG(L, 2);
			float Density = CHECK_FLOAT(L, 3); //min: 0.30 max: 2.00
			if(Density < 0.30f || Density > 2.0f || !zone_id || !type)
				return 0;

			uint32 sound;
			if(Density <= 0.30f)
				sound = 0;

			switch(type)
			{
				case 2:                                             //rain
				case 4:
					if(Density  < 0.40f)
						sound = 8533;
					else if(Density  < 0.70f)
						sound = 8534;
					else
						sound = 8535;
					break;
				case 8:                                             //snow
					if(Density  < 0.40f)
						sound = 8536;
					else if(Density  < 0.70f)
						sound = 8537;
					else
						sound = 8538;
					break;
				case 16:                                             //storm
					if(Density  < 0.40f)
						sound = 8556;
					else if(Density  < 0.70f)
						sound = 8557;
					else
						sound = 8558;
					break;
				default:											//no sound
					sound = 0;
					break;
			}
			WorldPacket data(SMSG_WEATHER, 9);
			data.Initialize(SMSG_WEATHER);
			if(type == 0)  // set all parameter to 0 for sunny.
				data << uint32(0) << float(0) << uint32(0) << uint8(0);
			else if(type == 1)  // No sound/density for fog
				data << type << float(0) << uint32(0) << uint8(0);
			else
				data << type << Density << sound << uint8(0) ;

			sWorld.SendZoneMessage(&data, zone_id, 0);

			return 0;
		}

		static int GetDistanceYards(lua_State* L, GameObject* ptr)
		{
			Object* target = CHECK_OBJECT(L, 1);
			if(!ptr || !target)
				lua_pushnil(L);
			else
			{
				LocationVector vec = ptr->GetPosition();
				lua_pushnumber(L, (float)vec.Distance(target->GetPosition()));
			}
			return 1;
		}

		static int PhaseSet(lua_State* L, GameObject* ptr)
		{
			uint32 newphase = CHECK_ULONG(L, 1);
			bool Save = (luaL_optint(L, 2, false) > 0 ? true : false);
			if(!ptr)
				return 0;
			ptr->Phase(PHASE_SET, newphase);
			if(ptr->m_spawn)
				ptr->m_spawn->phase = newphase;
			if(Save)
			{
				ptr->SaveToDB();
				ptr->m_loadedFromDB = true;
			}
			return 0;
		}

		static int PhaseAdd(lua_State* L, GameObject* ptr)
		{
			uint32 newphase = CHECK_ULONG(L, 1);
			bool Save = (luaL_optint(L, 2, false) > 0 ? true : false);
			if(!ptr)
				return 0;
			ptr->Phase(PHASE_ADD, newphase);
			if(ptr->m_spawn)
				ptr->m_spawn->phase |= newphase;
			if(Save)
			{
				ptr->SaveToDB();
				ptr->m_loadedFromDB = true;
			}
			return 0;
		}

		static int PhaseDelete(lua_State* L, GameObject* ptr)
		{
			uint32 newphase = CHECK_ULONG(L, 1);
			bool Save = (luaL_optint(L, 2, false) > 0 ? true : false);
			if(!ptr)
				return 0;
			ptr->Phase(PHASE_DEL, newphase);
			if(ptr->m_spawn)
				ptr->m_spawn->phase &= ~newphase;
			if(Save)
			{
				ptr->SaveToDB();
				ptr->m_loadedFromDB = true;
			}
			return 0;
		}

		static int GetPhase(lua_State* L, GameObject* ptr)
		{
			if(ptr)
				lua_pushnumber(L, ptr->m_phase);
			else
				lua_pushnil(L);
			return 1;
		}

		static int SendPacket(lua_State* L, GameObject* ptr)
		{
			WorldPacket* data = CHECK_PACKET(L, 1);
			bool self = CHECK_BOOL(L, 2);
			if(ptr != NULL && data != NULL)
				ptr->SendMessageToSet(data, self);
			return 0;
		}

		static int GetGUID(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			PUSH_GUID(L, ptr->GetGUID());
			return 1;
		}

		static int IsActive(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			if(ptr->GetByte(GAMEOBJECT_BYTES_1, 0))
				RET_BOOL(true)
				RET_BOOL(false)
			}

		static int Activate(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			if(ptr->GetByte(GAMEOBJECT_BYTES_1, 0) == 1)
				ptr->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
			else
				ptr->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			ptr->SetUInt32Value(GAMEOBJECT_FLAGS, (ptr->GetUInt32Value(GAMEOBJECT_FLAGS) & ~1));
			RET_BOOL(true)
		}

		static int DespawnObject(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			int delay = luaL_checkint(L, 1);
			int respawntime = luaL_checkint(L, 2);
			if(!delay)
				delay = 1; //Delay 0 might cause bugs
			ptr->Despawn(delay, respawntime);
			return 0;
		}

		static int AddLoot(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			if( ( lua_gettop( L ) != 3 ) || ( lua_gettop( L ) != 5 ) )
				return 0;

			uint32 itemid = luaL_checkint(L, 1);
			uint32 mincount = luaL_checkint(L, 2);
			uint32 maxcount = luaL_checkint(L, 3);
			bool perm = ((luaL_optint(L, 4, 0) == 1) ? true : false);
			if(perm)
			{
				float chance = CHECK_FLOAT(L, 5);
				QueryResult* result = WorldDatabase.Query("SELECT * FROM loot_gameobjects WHERE entryid = %u, itemid = %u", ptr->GetEntry(), itemid);
				if(!result)
					WorldDatabase.Execute("REPLACE INTO loot_gameobjects VALUES (%u, %u, %f, 0, 0, 0, %u, %u )", ptr->GetEntry(), itemid, chance, mincount, maxcount );
				delete result;
			}
			lootmgr.AddLoot(&ptr->loot, itemid, mincount, maxcount );
			return 0;
		}

		static int GetInstanceOwner(lua_State* L, GameObject* ptr)
		{
			MapInfo* pMapinfo = WorldMapInfoStorage.LookupEntry(ptr->GetMapId());
			if(pMapinfo)  //this block = IsInInstace()
			{
				if(pMapinfo->type != INSTANCE_NULL)
				{
					lua_pushboolean(L, 0);
					return 1;
				}
			}
			Instance* pInstance = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(), ptr->GetInstanceID());
			if(pInstance->m_creatorGuid != 0)  // creator guid is 0 if its owned by a group.
			{
				Player* owner = pInstance->m_mapMgr->GetPlayer(pInstance->m_creatorGuid);
				PUSH_UNIT(L, owner);
			}
			else
			{
				uint32 gId = pInstance->m_creatorGroup;
				PUSH_UNIT(L, objmgr.GetGroupById(gId)->GetLeader()->m_loggedInPlayer);
			}
			return 1;
		}

		static int GetDungeonDifficulty(lua_State* L, GameObject* ptr)
		{
			MapInfo* pMapinfo = WorldMapInfoStorage.LookupEntry(ptr->GetMapId());
			if(pMapinfo)  //this block = IsInInstace()
			{
				if(pMapinfo->type != INSTANCE_NULL)
				{
					lua_pushboolean(L, 0);
					return 1;
				}
			}
			Instance* pInstance = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(), ptr->GetInstanceID());
			lua_pushnumber(L, pInstance->m_difficulty);
			return 1;
		}

		static int SetDungeonDifficulty(lua_State* L, GameObject* ptr)
		{
			uint8 difficulty = luaL_checkint(L, 1);
			MapInfo* pMapinfo = WorldMapInfoStorage.LookupEntry(ptr->GetMapId());
			if(pMapinfo)  //this block = IsInInstace()
			{
				if(pMapinfo->type != INSTANCE_NULL)
				{
					lua_pushboolean(L, 0);
					return 1;
				}
			}
			Instance* pInstance = sInstanceMgr.GetInstanceByIds(ptr->GetMapId(), ptr->GetInstanceID());
			pInstance->m_difficulty = difficulty;
			lua_pushboolean(L, 1);
			return 1;
		}
		static int HasFlag(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			uint32 index = luaL_checkint(L, 1);
			uint32 flag = luaL_checkint(L, 2);
			lua_pushboolean(L, ptr->HasFlag(index, flag) ? 1 : 0);
			return 1;
		}

		static int IsInPhase(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			uint32 phase = luaL_checkint(L, 1);
			lua_pushboolean(L, ((ptr->m_phase & phase) != 0) ? 1 : 0);
			return 1;
		}

		static int GetSpawnId(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			lua_pushnumber(L, ptr->m_spawn != NULL ? ptr->m_spawn->id : 0);
			return 1;
		}

		static int GetAreaId(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET()
			lua_pushnumber(L, (ptr->GetMapMgr()->GetAreaID(ptr->GetPositionX(), ptr->GetPositionY())));
			return 1;
		}

		static int SetPosition(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			MapMgr* mapMgr = ptr->GetMapMgr();
			uint32 NewGuid = mapMgr->GenerateGameobjectGuid();
			ptr->RemoveFromWorld(true);
			ptr->SetNewGuid(NewGuid);
			float x = CHECK_FLOAT(L, 1);
			float y = CHECK_FLOAT(L, 2);
			float z = CHECK_FLOAT(L, 3);
			float o = CHECK_FLOAT(L, 4);
			bool save = luaL_optint(L, 5, 0) ? true : false;
			ptr->SetPosition(x, y, z, o);
			ptr->PushToWorld(mapMgr);
			if(save)
				ptr->SaveToDB();
			RET_BOOL(true)
		}

		static int GetObjectType(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			RET_STRING("GameObject");
		}
		static int ChangeScale(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			float nScale = CHECK_FLOAT(L, 1);
			bool updateNow = CHECK_BOOL(L, 2);
			nScale = (nScale <= 0) ? 1 : nScale;
			ptr->SetScale(nScale);
			if(updateNow)
			{
				MapMgr* mapMgr = ptr->GetMapMgr();
				uint32 nguid = mapMgr->GenerateGameobjectGuid();
				ptr->RemoveFromWorld(true);
				ptr->SetNewGuid(nguid);
				ptr->PushToWorld(mapMgr);
			}
			RET_BOOL(true)
		}
		static int GetByte(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 index = luaL_checkint(L, 1);
			uint32 index2 = luaL_checkint(L, 2);
			uint8 value = ptr->GetByte(index, index2);
			RET_INT(value);
		}
		static int SetByte(lua_State* L, GameObject* ptr)
		{
			TEST_GO_RET();
			int index = luaL_checkint(L, 1);
			int index2 = luaL_checkint(L, 2);
			uint8 value = luaL_checkint(L, 3);
			ptr->SetByte(index, index2, value);
			RET_BOOL(true)
		}
		static int FullCastSpellOnTarget(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 sp = CHECK_ULONG(L, 1);
			Object* target = CHECK_OBJECT(L, 2);
			if(sp && target != NULL)
			{
				Spell* tSpell = sSpellFactoryMgr.NewSpell(ptr, dbcSpell.LookupEntry(sp), false, NULL);
				SpellCastTargets sct(target->GetGUID());
				tSpell->prepare(&sct);
			}
			return 0;
		}
		static int FullCastSpell(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 sp = CHECK_ULONG(L, 1);
			if(sp)
			{
				Spell* tSpell = sSpellFactoryMgr.NewSpell(ptr, dbcSpell.LookupEntry(sp), false, NULL);
				SpellCastTargets sct(ptr->GetGUID());
				tSpell->prepare(&sct);
			}
			return 0;
		}
		static int CustomAnimate(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			uint32 aindex = CHECK_ULONG(L, 1);
			if(aindex < 2 && ptr != NULL)
			{
				WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM, 12);
				data << ptr->GetGUID();
				data << aindex;
				ptr->SendMessageToSet(&data, false);
				RET_BOOL(true)
			}
			RET_BOOL(false)
		}
		static int GetLocation(lua_State* L, GameObject* ptr)
		{
			TEST_GO()

			lua_pushnumber(L, ptr->GetPositionX());
			lua_pushnumber(L, ptr->GetPositionY());
			lua_pushnumber(L, ptr->GetPositionZ());
			lua_pushnumber(L, ptr->GetOrientation());
			return 4;
		}
		static int GetSpawnLocation(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			lua_pushnumber(L, ptr->GetPositionX());
			lua_pushnumber(L, ptr->GetPositionY());
			lua_pushnumber(L, ptr->GetPositionZ());
			lua_pushnumber(L, ptr->GetOrientation());
			return 4;
		}
		static int GetObject(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			uint64 guid = CHECK_GUID(L, 1);
			Object* obj = ptr->GetMapMgr()->_GetObject(guid);
			if(obj != NULL && obj->IsUnit())
				PUSH_UNIT(L, obj);
			else if(obj != NULL && obj->IsGameObject())
				PUSH_GO(L, obj);
			else
				lua_pushnil(L);
			return 1;
		}
		static int RegisterEvent(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			const char* typeName = luaL_typename(L, 1);
			int delay = luaL_checkint(L, 2);
			int repeats = luaL_checkint(L, 3);
			if(!delay) return 0;
			lua_settop(L, 1);
			int functionRef = 0;
			if(!strcmp(typeName, "function"))
				functionRef = lua_ref(L, true);
			else if(!strcmp(typeName, "string"))
				functionRef = ExtractfRefFromCString(L, luaL_checkstring(L, 1));
			if(functionRef)
			{
				TimedEvent* ev = TimedEvent::Allocate(ptr, new CallbackP1<LuaEngine, int>(&sLuaMgr, &LuaEngine::CallFunctionByReference, functionRef), EVENT_LUA_GAMEOBJ_EVENTS, delay, repeats);
				ptr->event_AddEvent(ev);
				std::map< uint64, std::set<int> > & objRefs = sLuaMgr.getObjectFunctionRefs();
				std::map< uint64, std::set<int> >::iterator itr = objRefs.find(ptr->GetGUID());
				if(itr == objRefs.end())
				{
					std::set<int> refs;
					refs.insert(functionRef);
					objRefs.insert(make_pair(ptr->GetGUID(), refs));
				}
				else
				{
					std::set<int> & refs = itr->second;
					refs.insert(functionRef);
				}
			}
			return 0;
		}
		static int RemoveEvents(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			sEventMgr.RemoveEvents(ptr, EVENT_LUA_GAMEOBJ_EVENTS);
			std::map< uint64, std::set<int> > & objRefs = sLuaMgr.getObjectFunctionRefs();
			std::map< uint64, std::set<int> >::iterator itr = objRefs.find(ptr->GetGUID());
			if(itr != objRefs.end())
			{
				std::set<int> & refs = itr->second;
				for(std::set<int>::iterator it = refs.begin(); it != refs.end(); ++it)
					lua_unref(L, (*it));
				refs.clear();
			}
			return 0;
		}
		static int SetScale(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			float scale = luaL_checknumber(L, 1);
			if(scale > 0)
				ptr->SetScale(scale);
			return 0;
		}
		static int GetScale(lua_State* L, GameObject* ptr)
		{
			TEST_GO();
			lua_pushnumber(L, ptr->GetScale());
			return 1;
		}
		static int GetClosestUnit(lua_State* L, GameObject* ptr)
		{
			TEST_GO()
			float closest_dist = 99999.99f;
			float current_dist = 0;
			Object* closest_unit = NULL;
			Unit* ret = NULL;
			for(set<Object*>::iterator itr = ptr->GetInRangeSetBegin(); itr != ptr->GetInRangeSetEnd(); ++itr)
			{
				closest_unit = (*itr);
				if(!closest_unit->IsUnit())
					continue;
				current_dist = ptr->GetDistance2dSq(closest_unit);
				if(current_dist < closest_dist)
				{
					closest_dist = current_dist;
					ret = TO_UNIT(closest_unit);
				}
			}
			PUSH_UNIT(L, ret);
			return 1;
		}

		static int Damage( lua_State *L, GameObject *ptr ){
			TEST_GO();

			if( ptr->GetInfo()->Type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING )
				return 0;

			if( lua_gettop( L ) != 3 )
				return 0;

			uint32 damage = luaL_checkint( L, 1 );
			uint64 guid = CHECK_GUID( L, 2 );
			uint32 spellid = luaL_checkint( L, 3 );

			ptr->Damage( damage, guid, guid, spellid );

			return 0;
		}

		static int Rebuild( lua_State *L, GameObject *ptr ){
			TEST_GO();

			if( ptr->GetInfo()->Type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING )
				return 0;

			ptr->Rebuild();


			return 0;
		}

		static int GetHP( lua_State *L, GameObject *ptr ){
			TEST_GO();

			if( ptr->GetInfo()->Type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING )
				return 0;

			lua_pushinteger( L, ptr->GetHP() );

			return 1;
		}

		static int GetMaxHP( lua_State *L, GameObject *ptr ){
			TEST_GO();

			if( ptr->GetInfo()->Type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING )
				return 0;

			lua_pushinteger( L, ptr->GetMaxHP() );

			return 1;
		}

		static int GetWorldStateForZone( lua_State *L, GameObject *ptr ){
			TEST_GO();

			if( lua_gettop( L ) != 1 )
				return 0;

			uint32 field = luaL_checkint( L, 1 );

			AreaTable *a = ptr->GetMapMgr()->GetArea( ptr->GetPositionX(), ptr->GetPositionY(), ptr->GetPositionZ() );
			if( a == NULL )
				return 0;

			uint32 zone = a->ZoneId;

			if( zone == 0 )
				zone = a->AreaId;

			if( zone == 0 )
				return 0;

			uint32 value
				= ptr->GetMapMgr()->GetWorldStatesHandler().GetWorldStateForZone( zone, 0, field );

			lua_pushinteger( L, value );

			return 1;
		}


		static int SetWorldStateForZone( lua_State *L, GameObject *ptr ){
			TEST_GO();

			if( lua_gettop( L ) != 2 )
				return 0;

			uint32 field = luaL_checkint( L, 1 );
			uint32 value = luaL_checkint( L, 2 );

			AreaTable *a = ptr->GetMapMgr()->GetArea( ptr->GetPositionX(), ptr->GetPositionY(), ptr->GetPositionZ() );
			if( a == NULL )
				return 0;

			uint32 zone = a->ZoneId;

			if( zone == 0 )
				zone = a->AreaId;

			if( zone == 0 )
				return 0;

			ptr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone( zone, 0, field, value );

			return 0;
		}
};
#endif

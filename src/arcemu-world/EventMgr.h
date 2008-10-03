/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef __EVENTMGR_H
#define __EVENTMGR_H

enum EventTypes
{
	EVENT_UNK = 0,
	EVENT_MAPMGR_UPDATEOBJECTS,
	EVENT_MAPCELL_UNLOAD,
	EVENT_WORLD_UPDATEAUCTIONS,
	EVENT_WORLD_UPDATESESSIONS,
	EVENT_CREATURE_UPDATE,
	EVENT_PLAYER_UPDATE,
	EVENT_PLAYER_UPDATEEXPLORATION,
	EVENT_GAMEOBJECT_UPDATE,
	EVENT_DYNAMICOBJECT_UPDATE,
	EVENT_GAMEOBJECT_ENV_DAMAGE,
	EVENT_PLAYER_STOPPVP,
	EVENT_CREATURE_JUSTDIED,
	EVENT_CREATURE_REMOVE_CORPSE,
	EVENT_CREATURE_RESPAWN,
	EVENT_MAPMGR_UNLOAD_CELL,
	EVENT_PLAYER_REST,
	EVENT_PLAYER_SEND_PACKET,
	EVENT_UNIT_EMOTE,
	EVENT_WORLD_UPDATEQUEUES,
	EVENT_PET_UPDATE,
	EVENT_PLAYER_REDUCEDRUNK,
	EVENT_WEATHER_UPDATE,
	EVENT_TOTEM_EXPIRE,
	EVENT_TOTEM_SEEK_TARGET,
	EVENT_TOTEM_SEEK_TARGET_AREA_AURA,
	EVENT_ENSLAVE_EXPIRE,
	EVENT_PLAYER_TAXI_DISMOUNT,
	EVENT_GAMEOBJECT_DOOR_CLOSE,
	EVENT_PLAYER_ALLOW_TRIGGERPORT,
	EVENT_SPELL_DAMAGE_HIT,
	EVENT_PET_SPELL_COOLDOWN,
	EVENT_GAMEOBJECT_UNHOOK,
	EVENT_GAMEOBJECT_FISH_HOOKED,
	EVENT_GAMEOBJECT_FISH_NOT_HOOKED,
	EVENT_GAMEOBJECT_END_FISHING,
	EVENT_PLAYER_REPEAT_SHOT,
	EVENT_PLAYER_ACTIVATE_GAMEOBJECT,
	EVENT_UNIT_REPEAT_MSG,
	EVENT_UNIT_CHAT_MSG,
	EVENT_PLAYER_MOTD,
	EVENT_TIMED_QUEST_EXPIRE,
	EVENT_UNIT_MANUAL_REPEAT_MSG,
	EVENT_PLAYER_TAXI_POSITION_UPDATE,
	EVENT_PLAYER_TAXI_INTERPOLATE,
	EVENT_PLAYER_CHECKFORCHEATS,
	EVENT_AREAAURA_UPDATE,
	EVENT_AURA_REMOVE,
	EVENT_AURA_PERIODIC_DAMAGE,
	EVENT_AURA_PERIODIC_DAMAGE_PERCENT,
	EVENT_AURA_PERIODIC_HEAL,
	EVENT_AURA_PERIODIC_TRIGGERSPELL,
	EVENT_AURA_PERIODIC_ENERGIZE,
	EVENT_AURA_PERIODIC_LEECH,
	EVENT_AURA_PERIODIC_BURN,
	EVENT_AURA_PERIOCIC_MANA,
	EVENT_AURA_PERIODIC_HEALTH_FUNNEL,
	EVENT_AURA_MANA_FUNNEL,
	EVENT_CANNIBALIZE,
	EVENT_GROUP_POSITION_UPDATE,
	EVNET_RAID_POSITION_UPDATE,
	EVENT_DELETE_TIMER,
	EVENT_UNIT_SENDMOVE,
	EVENT_GAMEOBJECT_ITEM_SPAWN,
	EVENT_GAMEOBJECT_ITEM_DESPAWN,
	EVENT_GAMEOBJECT_EXPIRE,
	EVENT_CREATURE_SAFE_DELETE,
	EVENT_PET_DELAYED_REMOVE,
	EVENT_REMOVE_ENCHANTMENT,
	EVENT_REMOVE_ENCHANTMENT1,
	EVENT_REMOVE_ENCHANTMENT2,
	EVENT_REMOVE_ENCHANTMENT3,
	EVENT_REMOVE_ENCHANTMENT4,
	EVENT_REMOVE_ENCHANTMENT5,
	EVENT_REMOVE_ENCHANTMENT6,
	EVENT_REMOVE_ENCHANTMENT7,
	EVENT_REMOVE_ENCHANTMENT8,
	EVENT_REMOVE_ENCHANTMENT9,
	EVENT_REMOVE_ENCHANTMENT10,
	EVENT_REMOVE_ENCHANTMENT11,
	EVENT_REMOVE_ENCHANTMENT12,
	EVENT_REMOVE_ENCHANTMENT13,
	EVENT_REMOVE_ENCHANTMENT14,
	EVENT_REMOVE_ENCHANTMENT15,
	EVENT_REMOVE_ENCHANTMENT16,
	EVENT_REMOVE_ENCHANTMENT17,
	EVENT_REMOVE_ENCHANTMENT18,
	EVENT_REMOVE_ENCHANTMENT19,
	EVENT_REMOVE_ENCHANTMENT20,
	EVENT_REMOVE_ENCHANTMENT21,
	EVENT_PLAYER_CHARM_ATTACK,
	EVENT_PLAYER_KICK,
	EVENT_TRANSPORTER_NEXT_WAYPOINT,
	EVENT_LOOT_ROLL_FINALIZE,
	EVENT_FIELD_UPDATE_EXPIRE,
	EVENT_SUMMON_PET_EXPIRE,
	EVENT_CORPSE_DESPAWN,
	EVENT_SCRIPT_UPDATE_EVENT,
	EVENT_INSTANCE_SOFT_RESET,
	EVENT_INSTANCE_HARD_RESET,
	EVENT_INSTANCE_LIMIT_COUNTER,
	EVENT_MAP_SPAWN_EVENTS,
	EVENT_PLAYER_DUEL_COUNTDOWN,
	EVENT_PLAYER_DUEL_BOUNDARY_CHECK,
	EVENT_GAMEOBJECT_TRAP_SEARCH_TARGET,
	EVENT_PLAYER_TELEPORT,
	EVENT_UNIT_DIMINISHING_RETURN,
	EVENT_UNIT_UNROOT,
	EVENT_MAILSYSTEM_RELOAD,
	EVENT_CREATURE_FORMATION_LINKUP,
	EVENT_CREATURE_CHANNEL_LINKUP,
	EVENT_AURA_REGEN_MANA_STAT_PCT,
	EVENT_AURA_PERIODIC_HEALINCOMB,
	EVENT_AURA_REGEN,
	EVENT_AURA_PERIODIC_REGEN,
	EVENT_AURA_PERIODIC_HEALPERC,
	EVENT_ATTACK_TIMEOUT,		//Zack 2007 05 05: if no action is taken then combat should be exited after 5 seconds.
	EVENT_SUMMON_EXPIRE,		//Zack 2007 05 28: similar to pet expire but we can have multiple guardians
	EVENT_MUTE_PLAYER,			//Zack 2007 06 05: player gains his voice back
	EVENT_PLAYER_FORECED_RESURECT,		//Zack 2007 06 08: After player not pushing release spirit for 6 minutes while dead
	EVENT_PLAYER_SOFT_DISCONNECT,		//Zack 2007 06 12: Kick AFK players to not eat resources
	EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG,
	EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG_1,
	EVENT_CORPSE_SPAWN_BONES,
	EVENT_DODGE_BLOCK_FLAG_EXPIRE,	//yeah, there are more then 1 flags
	EVENT_REJUVENATION_FLAG_EXPIRE,
	EVENT_PARRY_FLAG_EXPIRE,		
	EVENT_CRIT_FLAG_EXPIRE,		
	EVENT_LASTKILLWITHHONOR_FLAG_EXPIRE,
	EVENT_GMSCRIPT_EVENT,
	EVENT_RELOCATE,
	EVENT_BATTLEGROUND_QUEUE_UPDATE,
	EVENT_BATTLEGROUND_SPAWN_BUFF,
	EVENT_BATTLEGROUND_COUNTDOWN,
	EVENT_BATTLEGROUND_CLOSE,
	EVENT_PLAYER_EJECT_FROM_INSTANCE,
	EVENT_AB_RESOURCES_UPDATE_TEAM_0,
	EVENT_AB_RESOURCES_UPDATE_TEAM_1,
	EVENT_AB_CAPTURE_CP_1,
	EVENT_AB_CAPTURE_CP_2,
	EVENT_AB_CAPTURE_CP_3,
	EVENT_AB_CAPTURE_CP_4,
	EVENT_AB_CAPTURE_CP_5,
	EVENT_AB_RESPAWN_BUFF,
	EVENT_COMBO_POINT_CLEAR_FOR_TARGET,
	EVENT_ITEM_UPDATE,
	EVENT_PLAYER_JOIN_INSTANCE,
	EVENT_PLAYER_FLUSH_MOVEMENT,
	EVENT_EOTS_GIVE_POINTS,
	EVENT_EOTS_CHECK_CAPTURE_POINT_STATUS,
	EVENT_EOTS_RESET_FLAG,
	EVENT_PLAYER_RESET_HEARTBEAT,
	EVENT_COMBAT_TIMER,
	EVENT_CREATURE_AISPELL,
	EVENT_UNIT_DELAYED_SPELL_CANCEL,
	EVENT_AURA_PERIODIC_ENERGIZE_VARIABLE, // Aspect of the Viper
	EVENT_ARENA_SHADOW_SIGHT,
};

enum EventFlags
{
	EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT = 0x1,
	EVENT_FLAG_DELETES_OBJECT				   = 0x2,
};

struct SERVER_DECL TimedEvent
{
	TimedEvent(void* object, CallbackBase* callback, uint32 type, time_t time, uint32 repeat, uint32 flags) : 
		obj(object), cb(callback), eventType(type), eventFlag(flags), msTime(time), currTime(time), repeats(repeat), deleted(false),ref(0) {}
		
	void *obj;
	CallbackBase *cb;
	uint32 eventType;
	uint16 eventFlag;
	time_t msTime;
	time_t currTime;
	uint16 repeats;
	bool deleted;
	int instanceId;
	volatile long ref;

	static TimedEvent * Allocate(void* object, CallbackBase* callback, uint32 flags, time_t time, uint32 repeat);

#ifdef WIN32
	void DecRef()
	{
		InterlockedDecrement(&ref);
		if(ref <= 0)
		{
			delete cb;
			delete this;
		}
	}

	void IncRef() { InterlockedIncrement(&ref); }
#else

	/* burlex: if anyone knows how to do the equivilent of InterlockedIncrement/Decrement on linux feel free
	   to change this, I couldn't find the atomic functions anymore though :*( */

	void IncRef() { ++ref; }
    
	void DecRef()
	{
		--ref;
		if(ref <= 0)
		{
			 delete cb;
			 delete this;
		}
	}
#endif

};

class EventMgr;
class EventableObjectHolder;
typedef map<int32, EventableObjectHolder*> HolderMap;

class SERVER_DECL EventMgr : public Singleton < EventMgr >
{
	friend class MiniEventMgr;
public:
	template <class Class>
		void AddEvent(Class *obj, void (Class::*method)(), uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP0<Class>(obj, method), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1>
		void AddEvent(Class *obj, void (Class::*method)(P1), P1 p1, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP1<Class, P1>(obj, method, p1), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2>
		void AddEvent(Class *obj, void (Class::*method)(P1,P2), P1 p1, P2 p2, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP2<Class, P1, P2>(obj, method, p1, p2), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3>
		void AddEvent(Class *obj,void (Class::*method)(P1,P2,P3), P1 p1, P2 p2, P3 p3, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP3<Class, P1, P2, P3>(obj, method, p1, p2, p3), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4>
		void AddEvent(Class *obj, void (Class::*method)(P1,P2,P3,P4), P1 p1, P2 p2, P3 p3, P4 p4, uint32 type, uint32 time, uint32 repeats, uint32 flags)
	{
		// create a timed event
		TimedEvent * event = new TimedEvent(obj, new CallbackP4<Class, P1, P2, P3, P4>(obj, method, p1, p2, p3, p4), type, time, repeats, flags);

		// add this to the object's list, updating will all be done later on...
		obj->event_AddEvent(event);
	}

	template <class Class> void RemoveEvents(Class *obj) { obj->event_RemoveEvents(static_cast<uint32>(-1)); }
	template <class Class> void RemoveEvents(Class *obj, int32 type)
	{
		obj->event_RemoveEvents(type);
	}

	template <class Class> void ModifyEventTimeLeft(Class *obj, uint32 type, uint32 time,bool unconditioned=true)
	{
		obj->event_ModifyTimeLeft(type, time,unconditioned);
	}

	template <class Class> void ModifyEventTimeAndTimeLeft(Class *obj, uint32 type, uint32 time)
	{
		obj->event_ModifyTimeAndTimeLeft(type, time);
	}

	template <class Class> void ModifyEventTime(Class *obj, uint32 type, uint32 time)
	{
		obj->event_ModifyTime(type, time);
	}

	template <class Class> bool HasEvent(Class *obj, uint32 type)
	{
		return obj->event_HasEvent(type);
	}

	EventableObjectHolder * GetEventHolder(int32 InstanceId)
	{
		HolderMap::iterator itr = mHolders.find(InstanceId);
		if(itr == mHolders.end()) return 0;
		return itr->second;
	}

	void AddEventHolder(EventableObjectHolder * holder, int32 InstanceId)
	{
		holderLock.Acquire();
		mHolders.insert( HolderMap::value_type( InstanceId, holder) );
		holderLock.Release();
	}

	void RemoveEventHolder(int32 InstanceId)
	{
		holderLock.Acquire();
		mHolders.erase(InstanceId);
		holderLock.Release();
	}

	void RemoveEventHolder(EventableObjectHolder * holder)
	{
		holderLock.Acquire();
		HolderMap::iterator itr = mHolders.begin();
		for(; itr != mHolders.end(); ++itr)
		{
			if(itr->second == holder)
			{
				mHolders.erase(itr);
				holderLock.Release();
				return;
			}
		}
		holderLock.Release();
	}

protected:
	HolderMap mHolders;
	Mutex holderLock;
};

#define sEventMgr EventMgr::getSingleton()

#endif

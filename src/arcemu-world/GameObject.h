/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
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

#ifndef WOWSERVER_GAMEOBJECT_H
#define WOWSERVER_GAMEOBJECT_H

enum GO_STATE{
	GAMEOBJECT_STATE_OPEN              =  0,
	GAMEOBJECT_STATE_CLOSED            =  1,
	GAMEOBJECT_STATE_ALTERNATIVE_OPEN  =  2
};

enum GO_FLAGS{
	GAMEOBJECT_FLAG_NONSELECTABLE  = 0x1,
	GAMEOBJECT_FLAG_LOCKED         = 0x2,
	GAMEOBJECT_FLAG_UNTARGETABLE   = 0x4,
	GAMEOBJECT_FLAG_DAMAGED        = 0x200,
	GAMEOBJECT_FLAG_DESTROYED      = 0x400
};

class Player;
class GameObjectAIScript;
class GameObjectTemplate;

struct GOQuestItem
{
	uint32 itemid;
	uint32 requiredcount;
};

struct GOQuestGameObject
{
	uint32 goid;
	uint32 requiredcount;
};

enum GAMEOBJECT_FLAG_BIT
{
    GAMEOBJECT_UNCLICKABLE = 0x01,
    GAMEOBJECT_CLICKABLE = 0x20,
};

enum GAMEOBJECT_OVERRIDES //by VLack
{
    GAMEOBJECT_INFVIS = 0x01,		//Makes the gameobject forever visible on the map after you saw it at least once - for various transports; actually it just doesn't erase it while you're on the same map.
    GAMEOBJECT_MAPWIDE = 0x02,		//When you enter its map, the gameobject gets pushed to you no matter how far it is (but only for players), especially for Deeprun and Ulduar Trams.
    GAMEOBJECT_AREAWIDE = 0x04,		//UNIMPLEMENTED, but will work like this: the Map will get marked that it contains an object like this, and on player movement these objects will get distance-checked to spawn them from a greater distance than normal if needed - for few objects on smaller maps, like on battlegrounds; maybe they'll get area-triggered, haven't decided yet.
    GAMEOBJECT_ONMOVEWIDE = 0x08,		//When this gameobject moves and sends updates about it's position, do so in the second range - MapMgr::ChangeObjectLocation, +/- 6 units wide instead of +/- 1.
    GAMEOBJECT_OVERRIDE_FLAGS = 0x10,	//UNIMPLEMENTED, Let the core decide about the flags sent in the A9 - example: 252 instead of 352 for Deeprun Tram.
    GAMEOBJECT_OVERRIDE_BYTES1 = 0x20,	//UNIMPLEMENTED, Let the core use the full field instead an uint8 in GAMEOBJECT_BYTES_1, if the database creator knows what to do with it.
    GAMEOBJECT_OVERRIDE_PARENTROT = 0x40,	//Makes it possible for the core to skip calculating these fields and use whatever was specified in the spawn.
    //Later other types might folow, or the upper bytes might get used for the AREAWIDE option in the overrides variable...
};

typedef HM_NAMESPACE::hash_map<Quest*, uint32 > GameObjectGOMap;
typedef HM_NAMESPACE::hash_map<Quest*, std::map<uint32, uint32> > GameObjectItemMap;

#pragma pack(push,1)
struct GameObjectInfo
{
	uint32 ID;
	uint32 Type;
	uint32 DisplayID;
	const char* Name;
	const char* Category;
	const char* Castbartext;
	const char* Unkstr;

	// different GO types have different data field
	union
	{
		//0 GAMEOBJECT_TYPE_DOOR
		struct
		{
			uint32 startOpen;			//0 used client side to determine GO_ACTIVATED means open/closed
			uint32 lockId;				//1 -> Lock.dbc
			uint32 autoCloseTime;		//2 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
			uint32 noDamageImmune;		//3 break opening whenever you recieve damage?
			uint32 openTextID;			//4 can be used to replace castBarCaption?
			uint32 closeTextID;			//5
			uint32 ignoredByPathing;	//6
		}door;

		//1 GAMEOBJECT_TYPE_BUTTON
		struct
		{
			uint32 startOpen;			//0
			uint32 lockId;				//1 -> Lock.dbc
			uint32 autoCloseTime;		//2 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
			uint32 linkedTrapId;		//3
			uint32 noDamageImmune;		//4 isBattlegroundObject
			uint32 large;				//5
			uint32 openTextID;			//6 can be used to replace castBarCaption?
			uint32 closeTextID;			//7
			uint32 losOK;				//8
		}button;

		//2 GAMEOBJECT_TYPE_QUESTGIVER
		struct
		{
			uint32 lockId;			//0 -> Lock.dbc
			uint32 questList;		//1
			uint32 pageMaterial;	//2
			uint32 gossipID;		//3
			uint32 customAnim;		//4
			uint32 noDamageImmune;	//5
			uint32 openTextID;		//6 can be used to replace castBarCaption?
			uint32 losOK;			//7
			uint32 allowMounted;	//8
			uint32 large;			//9
		}questgiver;

		//3 GAMEOBJECT_TYPE_CHEST
		struct
		{
			uint32 lockId;				//0 -> Lock.dbc
			uint32 lootId;				//1
			uint32 chestRestockTime;	//2
			uint32 consumable;			//3
			uint32 minSuccessOpens;		//4
			uint32 maxSuccessOpens;		//5
			uint32 eventId;				//6 lootedEvent
			uint32 linkedTrapId;		//7
			uint32 questId;				//8 not used currently but store quest required for GO activation for player
			uint32 level;				//9
			uint32 losOK;				//10
			uint32 leaveLoot;			//11
			uint32 notInCombat;			//12
			uint32 logLoot;				//13
			uint32 openTextID;			//14 can be used to replace castBarCaption?
			uint32 groupLootRules;		//15
			uint32 floatingTooltip;		//16
		}chest;

		//4 GAMEOBJECT_TYPE_BINDER - empty

		//5 GAMEOBJECT_TYPE_GENERIC
		struct
		{
			uint32 floatingTooltip;		//0
			uint32 highlight;			//1
			uint32 serverOnly;			//2
			uint32 large;				//3
			uint32 floatOnWater;		//4
			uint32 questID;				//5
		}genericGO;

		//6 GAMEOBJECT_TYPE_TRAP
		struct
		{
			uint32 lockId;				//0 -> Lock.dbc
			uint32 level;				//1
			uint32 radius;				//2 radius for trap activation
			uint32 spellId;				//3
			uint32 charges;				//4 need respawn (if > 0)
			uint32 cooldown;			//5 time in secs
			uint32 autoCloseTime;		//6 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
			uint32 startDelay;			//7
			uint32 serverOnly;			//8
			uint32 stealthed;			//9
			uint32 large;				//10
			uint32 stealthAffected;		//11
			uint32 openTextID;			//12 can be used to replace castBarCaption?
			uint32 closeTextID;			//13
			uint32 ignoreTotems;		//14
		}trap;

		//7 GAMEOBJECT_TYPE_CHAIR
		struct
		{
			uint32 slots;				//0
			uint32 height;				//1
			uint32 onlyCreatorUse;		//2
			uint32 triggeredEvent;		//3
		}chair;

		//8 GAMEOBJECT_TYPE_SPELL_FOCUS
		struct
		{
			uint32 focusId;				//0
			uint32 dist;				//1
			uint32 linkedTrapId;		//2
			uint32 serverOnly;			//3
			uint32 questID;				//4
			uint32 large;				//5
			uint32 floatingTooltip;		//6
		}spellFocus;

		//9 GAMEOBJECT_TYPE_TEXT
		struct
		{
			uint32 pageID;				//0
			uint32 language;			//1
			uint32 pageMaterial;		//2
			uint32 allowMounted;		//3
		}text;

		//10 GAMEOBJECT_TYPE_GOOBER
		struct
		{
			uint32 lockId;				//0 -> Lock.dbc
			uint32 questId;				//1
			uint32 eventId;				//2
			uint32 autoCloseTime;		//3 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
			uint32 customAnim;			//4
			uint32 consumable;			//5
			uint32 cooldown;			//6
			uint32 pageId;				//7
			uint32 language;			//8
			uint32 pageMaterial;		//9
			uint32 spellId;				//10
			uint32 noDamageImmune;		//11
			uint32 linkedTrapId;		//12
			uint32 large;				//13
			uint32 openTextID;			//14 can be used to replace castBarCaption?
			uint32 closeTextID;			//15
			uint32 losOK;				//16 isBattlegroundObject
			uint32 allowMounted;		//17
			uint32 floatingTooltip;		//18
			uint32 gossipID;			//19
			uint32 WorldStateSetsState;	//20
		}goober;

		//11 GAMEOBJECT_TYPE_TRANSPORT
		struct
		{
			uint32 pause;			//0
			uint32 startOpen;		//1
			uint32 autoCloseTime;	//2 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
			uint32 pause1EventID;	//3
			uint32 pause2EventID;	//4
		}transport;

		//12 GAMEOBJECT_TYPE_AREADAMAGE
		struct
		{
			uint32 lockId;			//0
			uint32 radius;			//1
			uint32 damageMin;		//2
			uint32 damageMax;		//3
			uint32 damageSchool;	//4
			uint32 autoCloseTime;	//5 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
			uint32 openTextID;		//6
			uint32 closeTextID;		//7
		}areadamage;

		//13 GAMEOBJECT_TYPE_CAMERA
		struct
		{
			uint32 lockId;			//0 -> Lock.dbc
			uint32 cinematicId;		//1
			uint32 eventID;			//2
			uint32 openTextID;		//3 can be used to replace castBarCaption?
		}camera;

		//14 GAMEOBJECT_TYPE_MAPOBJECT - empty

		//15 GAMEOBJECT_TYPE_MO_TRANSPORT
		struct
		{
			uint32 taxiPathId;			//0
			uint32 moveSpeed;			//1
			uint32 accelRate;			//2
			uint32 startEventID;		//3
			uint32 stopEventID;			//4
			uint32 transportPhysics;	//5
			uint32 mapID;				//6
			uint32 worldState1;			//7
		}moTransport;

		//16 GAMEOBJECT_TYPE_DUELFLAG - empty

		//17 GAMEOBJECT_TYPE_FISHINGNODE - empty

		//18 GAMEOBJECT_TYPE_SUMMONING_RITUAL
		struct
		{
			uint32 reqParticipants;				//0
			uint32 spellId;						//1
			uint32 animSpell;					//2
			uint32 ritualPersistent;			//3
			uint32 casterTargetSpell;			//4
			uint32 casterTargetSpellTargets;	//5
			uint32 castersGrouped;				//6
			uint32 ritualNoTargetCheck;			//7
		}summoningRitual;

		//19 GAMEOBJECT_TYPE_MAILBOX - empty

		//20 GAMEOBJECT_TYPE_DONOTUSE - empty

		//21 GAMEOBJECT_TYPE_GUARDPOST
		struct
		{
			uint32 creatureID;			//0
			uint32 charges;				//1
		}guardpost;

		//22 GAMEOBJECT_TYPE_SPELLCASTER
		struct
		{
			uint32 spellId;			//0
			uint32 charges;			//1
			uint32 partyOnly;		//2
			uint32 allowMounted;	//3
			uint32 large;			//4
		}spellcaster;

		//23 GAMEOBJECT_TYPE_MEETINGSTONE
		struct
		{
			uint32 minLevel;		//0
			uint32 maxLevel;		//1
			uint32 areaID;			//2
		}meetingstone;

		//24 GAMEOBJECT_TYPE_FLAGSTAND
		struct
		{
			uint32 lockId;			//0
			uint32 pickupSpell;		//1
			uint32 radius;			//2
			uint32 returnAura;		//3
			uint32 returnSpell;		//4
			uint32 noDamageImmune;	//5
			uint32 openTextID;		//6
			uint32 losOK;			//7
		}flagstand;

		//25 GAMEOBJECT_TYPE_FISHINGHOLE
		struct
		{
			uint32 radius;				//0 how close bobber must land for sending loot
			uint32 lootId;				//1
			uint32 minSuccessOpens;		//2
			uint32 maxSuccessOpens;		//3
			uint32 lockId;				//4 -> Lock.dbc; possibly 1628 for all?
		}fishinghole;

		//26 GAMEOBJECT_TYPE_FLAGDROP
		struct
		{
			uint32 lockId;				//0
			uint32 eventID;				//1
			uint32 pickupSpell;			//2
			uint32 noDamageImmune;		//3
			uint32 openTextID;			//4
			
		}flagdrop;

		//27 GAMEOBJECT_TYPE_MINI_GAME
		struct
		{
			uint32 gameType;	//0
		}miniGame;

		//29 GAMEOBJECT_TYPE_CAPTURE_POINT
		struct
		{
			uint32 radius;				//0
			uint32 spell;				//1
			uint32 worldState1;			//2
			uint32 worldstate2;			//3
			uint32 winEventID1;			//4
			uint32 winEventID2;			//5
			uint32 contestedEventID1;	//6
			uint32 contestedEventID2;	//7
			uint32 progressEventID1;	//8
			uint32 progressEventID2;	//9
			uint32 neutralEventID1;		//10
			uint32 neutralEventID2;		//11
			uint32 neutralPercent;		//12
			uint32 worldstate3;			//13
			uint32 minSuperiority;		//14
			uint32 maxSuperiority;		//15
			uint32 minTime;				//16
			uint32 maxTime;				//17
			uint32 large;				//18
			uint32 highlight;			//19
			uint32 startingValue;		//20
			uint32 unidirectional;		//21
		}capturePoint;

		//30 GAMEOBJECT_TYPE_AURA_GENERATOR
		struct
		{
			uint32 startOpen;			//0
			uint32 radius;				//1
			uint32 auraID1;				//2
			uint32 conditionID1;		//3
			uint32 auraID2;				//4
			uint32 conditionID2;		//5
			uint32 serverOnly;			//6
		}auraGenerator;

		//31 GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY
		struct
		{
			uint32 mapID;			//0
			uint32 difficulty;		//1
		}dungeonDifficulty;

		//32 GAMEOBJECT_TYPE_BARBER_CHAIR
		struct
		{
			uint32 chairheight;		//0
			uint32 heightOffset;	//1
		}barberChair;

		//33 GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING
		struct
		{
			uint32 intactNumHits;			//0
			uint32 creditProxyCreature;		//1
			uint32 empty1;					//2
			uint32 intactEvent;				//3
			uint32 empty2;					//4
			uint32 damagedNumHits;			//5
			uint32 empty3;					//6
			uint32 empty4;					//7
			uint32 empty5;					//8
			uint32 damagedEvent;			//9
			uint32 empty6;					//10
			uint32 empty7;					//11
			uint32 empty8;					//12
			uint32 empty9;					//13
			uint32 destroyedEvent;			//14
			uint32 empty10;					//15
			uint32 debuildingTimeSecs;		//16
			uint32 empty11;					//17
			uint32 destructibleData;		//18
			uint32 rebuildingEvent;			//19
			uint32 empty12;					//20
			uint32 empty13;					//21
			uint32 damageEvent;				//22
			uint32 empty14;					//23
		}destructibleBuilding;

		//34 GAMEOBJECT_TYPE_GUILDBANK - empty

		//35 GAMEOBJECT_TYPE_TRAPDOOR
		struct
		{
			uint32 whenToPause;		// 0
			uint32 startOpen;		// 1
			uint32 autoClose;		// 2
		}trapDoor;

		// Raw data fields
		struct
		{
			uint32 sound0;
			uint32 sound1;
			uint32 sound2;
			uint32 sound3;
			uint32 sound4;
			uint32 sound5;
			uint32 sound6;
			uint32 sound7;
			uint32 sound8;
			uint32 sound9;
			uint32 Unknown1;
			uint32 Unknown2;
			uint32 Unknown3;
			uint32 Unknown4;
			uint32 Unknown5;
			uint32 Unknown6;
			uint32 Unknown7;
			uint32 Unknown8;
			uint32 Unknown9;
			uint32 Unknown10;
			uint32 Unknown11;
			uint32 Unknown12;
			uint32 Unknown13;
			uint32 Unknown14;
		}raw;

	};

	float Size;
	uint32 QuestItems[6];

	// Quests
	GameObjectGOMap goMap;
	GameObjectItemMap itemMap;
};
#pragma pack(pop)

enum GAMEOBJECT_BYTES //need to be removed ->Quest_BladeEdge_Mountains.cpp, IsleOfConquest.cpp, RingOfValdo.cpp
{
	GAMEOBJECT_BYTES_STATE = 0,
	GAMEOBJECT_BYTES_TYPE_ID = 1,
	GAMEOBJECT_BYTES_UNK = 2, // todo: unknown atm
	GAMEOBJECT_BYTES_ANIMPROGRESS = 3,
};
enum GAMEOBJECT_TYPES
{
    GAMEOBJECT_TYPE_DOOR					= 0,
    GAMEOBJECT_TYPE_BUTTON					= 1,
    GAMEOBJECT_TYPE_QUESTGIVER				= 2,
    GAMEOBJECT_TYPE_CHEST					= 3,
    GAMEOBJECT_TYPE_BINDER					= 4,
    GAMEOBJECT_TYPE_GENERIC					= 5,
    GAMEOBJECT_TYPE_TRAP					= 6,
    GAMEOBJECT_TYPE_CHAIR					= 7,
    GAMEOBJECT_TYPE_SPELL_FOCUS				= 8,
    GAMEOBJECT_TYPE_TEXT					= 9,
    GAMEOBJECT_TYPE_GOOBER					= 10,
    GAMEOBJECT_TYPE_TRANSPORT				= 11,
    GAMEOBJECT_TYPE_AREADAMAGE				= 12,
    GAMEOBJECT_TYPE_CAMERA					= 13,
    GAMEOBJECT_TYPE_MAP_OBJECT				= 14,
    GAMEOBJECT_TYPE_MO_TRANSPORT			= 15,
    GAMEOBJECT_TYPE_DUEL_ARBITER			= 16,
    GAMEOBJECT_TYPE_FISHINGNODE				= 17,
    GAMEOBJECT_TYPE_RITUAL					= 18,
    GAMEOBJECT_TYPE_MAILBOX					= 19,
    GAMEOBJECT_TYPE_AUCTIONHOUSE			= 20,
    GAMEOBJECT_TYPE_GUARDPOST				= 21,
    GAMEOBJECT_TYPE_SPELLCASTER				= 22,
    GAMEOBJECT_TYPE_MEETINGSTONE			= 23,
    GAMEOBJECT_TYPE_FLAGSTAND				= 24,
    GAMEOBJECT_TYPE_FISHINGHOLE				= 25,
    GAMEOBJECT_TYPE_FLAGDROP				= 26,
    GAMEOBJECT_TYPE_MINI_GAME				= 27,
    GAMEOBJECT_TYPE_LOTTERY_KIOSK			= 28,
    GAMEOBJECT_TYPE_CAPTURE_POINT			= 29,
    GAMEOBJECT_TYPE_AURA_GENERATOR			= 30,
    GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY		= 31,
    GAMEOBJECT_TYPE_BARBER_CHAIR			= 32,
    GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING	= 33,
    GAMEOBJECT_TYPE_GUILD_BANK				= 34,
    GAMEOBJECT_TYPE_TRAPDOOR				= 35
};

#define CALL_GO_SCRIPT_EVENT(obj, func) if(obj->IsGameObject() && TO< GameObject* >(obj)->GetScript() != NULL) TO< GameObject* >(obj)->GetScript()->func

class SERVER_DECL GameObject : public Object
{
	public:
		/************************************************************************/
		/* LUA Stuff                                                            */
		/************************************************************************/
		GameObject();

		GameObject(uint64 guid);

		~GameObject();

		GameObjectInfo* GetInfo() { return pInfo; }
		void SetInfo(GameObjectInfo* goi) { pInfo = goi; }

		bool CreateFromProto(uint32 entry, uint32 mapid, float x, float y, float z, float ang, float r0 = 0.0f, float r1 = 0.0f, float r2 = 0.0f, float r3 = 0.0f, uint32 overrides = 0);

		bool Load(GOSpawn* spawn);

		void Update(uint32 p_time);

		void Spawn(MapMgr* m);
		void Despawn(uint32 delay, uint32 respawntime);
		Loot loot;

		// Serialization
		void SaveToDB();
		void DeleteFromDB();

		void EventCloseDoor();
		//void EventCastSpell(uint32 guid, uint32 sp, bool triggered);	Need to be checked if it is unused
		void SetRotation(float rad);
		uint64 m_rotation;
		void UpdateRotation();

		//Fishing stuff
		void UseFishingNode(Player* player);
		void EndFishing(Player* player, bool abort);
		void FishHooked(Player* player);

		// Quests
		void _LoadQuests();
		bool HasQuests() { return m_quests != NULL; };
		void AddQuest(QuestRelation* Q);
		void DeleteQuest(QuestRelation* Q);
		Quest* FindQuest(uint32 quest_id, uint8 quest_relation);
		uint16 GetQuestRelation(uint32 quest_id);
		uint32 NumOfQuests();
		std::list<QuestRelation*>::iterator QuestsBegin() { return m_quests->begin(); };
		std::list<QuestRelation*>::iterator QuestsEnd() { return m_quests->end(); };
		void SetQuestList(std::list<QuestRelation*>* qst_lst) { m_quests = qst_lst; };

		void SetSummoned(Unit* mob)
		{
			m_summoner = mob;
			m_summonedGo = true;
		}
		void _Expire();

		void ExpireAndDelete();

		/// Quest data
		std::list<QuestRelation*>* m_quests;

		uint32* m_ritualmembers;
		uint32 m_ritualcaster;
		uint32 m_ritualtarget;
		uint16 m_ritualspell;

		virtual void InitAI();
		SpellEntry* spell;

		float range;
		uint16 counter;
		int32 charges;//used for type==22,to limit number of usages.
		bool invisible;//invisible
		uint8 invisibilityFlag;
		Unit* m_summoner;
		int8 bannerslot;
		int8 bannerauraslot;

		void CallScriptUpdate();


		GameObjectAIScript* GetScript() { return myScript; }

		//void TrapSearchTarget();	Need to be checked if it is uused //Traps need to find targets faster :P

		bool HasAI() { return spell != 0; }
		GOSpawn* m_spawn;
		void OnPushToWorld();
		void OnRemoveInRangeObject(Object* pObj);
		void RemoveFromWorld(bool free_guid);

		bool CanFish() { return (usage_remaining > 0); }
		void CatchFish() { if(usage_remaining) usage_remaining--; }
		void CalcFishRemaining(bool force)
		{
			if(force || !usage_remaining)
				usage_remaining = pInfo->raw.sound2 + RandomUInt(pInfo->raw.sound3 - pInfo->raw.sound2) - 1;
		}
		bool HasLoot();
		uint32 GetGOReqSkill();
		MapCell* m_respawnCell;

		void SetState(uint8 state){ SetByte(GAMEOBJECT_BYTES_1, 0, state); }
		uint8 GetState(){ return GetByte(GAMEOBJECT_BYTES_1, 0); }

		void SetType(uint8 type){ SetByte(GAMEOBJECT_BYTES_1, 1, type); }
		uint32 GetType(){ return this->GetInfo()->Type; }

		void SetArtKit(uint8 artkit){ SetByte(GAMEOBJECT_BYTES_1, 2, artkit); }
		uint8 GetArtkKit(){ return GetByte(GAMEOBJECT_BYTES_1, 2); }

		void Deactivate(){ SetUInt32Value(GAMEOBJECT_DYNAMIC, 0); }
		void Activate(){ SetUInt32Value(GAMEOBJECT_DYNAMIC, 1); }
		bool IsActive()
		{
			if (m_uint32Values[GAMEOBJECT_DYNAMIC] == 1)
				return true;
			else
				return false;
		}

		void SetDisplayId(uint32 id){ SetUInt32Value(GAMEOBJECT_DISPLAYID, id); }
		uint32 GetDisplayId(){ return GetUInt32Value(GAMEOBJECT_DISPLAYID); }

		void SetParentRotation(uint8 rot, float value){ SetFloatValue(GAMEOBJECT_PARENTROTATION + rot, value); }
		float GetParentRotation(uint8 rot){ return GetFloatValue(GAMEOBJECT_PARENTROTATION + rot); }

		void SetFaction(uint32 id)
		{
			SetUInt32Value(GAMEOBJECT_FACTION, id);
			_setFaction();
		}
		uint32 GetFaction(){ return GetUInt32Value(GAMEOBJECT_FACTION); }

		void SetLevel(uint32 level){ SetUInt32Value(GAMEOBJECT_LEVEL, level); }
		uint32 GetLevel(){ return GetUInt32Value(GAMEOBJECT_LEVEL); }

		void SetFlags(uint32 flags){ SetUInt32Value(GAMEOBJECT_FLAGS, flags); }
		uint32 GetFlags(){ return GetUInt32Value(GAMEOBJECT_FLAGS); }
		void RemoveFlags( uint32 flags ){ RemoveFlag( GAMEOBJECT_FLAGS, flags ); }

		bool HasFlags( uint32 flags )
		{
			if( HasFlag( GAMEOBJECT_FLAGS, flags ) != 0 )
				return true;
			else
				return false;
		}

		uint32 GetOverrides() { return m_overrides; }

		void SetAnimProgress( uint8 progress ){ SetByte( GAMEOBJECT_BYTES_1, 3, progress ); }
		uint8 GetAnimProgress(){ return GetByte( GAMEOBJECT_BYTES_1, 3 ); }
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		//void Damage( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID,  uint32 SpellID )
		//  Damages the destructible GameObject with a spell
		//
		//Parameters
		//  uint32 damage          -  The hit points that the GO will lose
		//  uint64 AttackerGUID    -  GUID of the caster of the damaging spell
		//  uint64 ControllerGUID  -  GUID of the controller of the caster of the damaging spell
		//  uint32 SpellID         -  ID of the damaging spell
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Damage( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID,  uint32 SpellID );

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//void Rebuild()
		//  Rebuilds the damaged/destroyed GameObject.
		//
		//Parameters
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Rebuild();


		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//uint32 GetHP()
		//  Returns the current hitpoints of the GameObject
		//
		//Parameters
		//  None
		//
		//Return Value
		//  Returns the current hitpoints of the GameObject
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		uint32 GetHP(){ return hitpoints; }


		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//uint32 GetMaxHP()
		//  Returns the maximum hitpoints of the GameObject
		//
		//Parameters
		//  None
		//
		//Return Value
		//  Returns the maximum hitpoints of the GameObject
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		uint32 GetMaxHP(){ return maxhitpoints; }

		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//void Restock()
		//  Restocks the gameobject's loot
		//
		//Parameters
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ReStock();

	protected:

		bool m_summonedGo;
		bool m_deleted;
		GameObjectInfo* pInfo;
		GameObjectAIScript* myScript;
		uint32 _fields[GAMEOBJECT_END];
		uint32 usage_remaining; //used for mining to mark times it can be mined

		uint32 m_overrides; //See enum GAMEOBJECT_OVERRIDES!


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//void SendDamagePacket( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID,  uint32 SpellID )
		//  Notifies the surrounding clients about the GameObject taking damage
		//
		//Parameters
		//  uint32 damage          -  The hit points that the GO will lose
		//  uint64 AttackerGUID    -  GUID of the caster of the damaging spell
		//  uint64 ControllerGUID  -  GUID of the controller of the caster of the damaging spell
		//  uint32 SpellID         -  ID of the damaging spell
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		void SendDamagePacket( uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID );
		
		
		uint32 hitpoints;
		uint32 maxhitpoints;

};

#endif




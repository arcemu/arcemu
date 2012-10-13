/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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
	char* Category;
	char* Castbartext;
	char* Unkstr;
	uint32 SpellFocus;
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
	float Size;
	uint32 QuestItems[6];

	// Quests
	GameObjectGOMap goMap;
	GameObjectItemMap itemMap;
};
#pragma pack(pop)

enum GAMEOBJECT_BYTES
{
    GAMEOBJECT_BYTES_STATE			= 0,
    GAMEOBJECT_BYTES_TYPE_ID		= 1,
    GAMEOBJECT_BYTES_UNK			= 2, // todo: unknown atm
    GAMEOBJECT_BYTES_ANIMPROGRESS	= 3,
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
		GameObject(uint64 guid);
		~GameObject();

		GameObjectInfo* GetInfo() { return pInfo; }
		void SetInfo(GameObjectInfo* goi) { pInfo = goi; }

		bool CreateFromProto(uint32 entry, uint32 mapid, float x, float y, float z, float ang, float r0 = 0.0f, float r1 = 0.0f, float r2 = 0.0f, float r3 = 0.0f, uint32 overrides = 0);

		bool Load(GOSpawn* spawn);

		virtual void Update(uint32 p_time);

		void Spawn(MapMgr* m);
		void Despawn(uint32 delay, uint32 respawntime);
		Loot loot;
		//void _EnvironmentalDamageUpdate();
		// Serialization
		void SaveToDB();
		void SaveToFile(std::stringstream & name);
		//bool LoadFromDB(uint32 guid);
		//void LoadFromDB(GameObjectTemplate *t);
		void DeleteFromDB();
		void EventCloseDoor();
		void EventCastSpell(uint32 guid, uint32 sp, bool triggered);
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

		void Deactivate();
		void Activate();

		bool isQuestGiver()
		{
			//from GameObject::CreateFromProto - SetType( pInfo->Type );
			if(GetType() == GAMEOBJECT_TYPE_QUESTGIVER)
				return true;
			else
				return false;
		};

		/// Quest data
		std::list<QuestRelation*>* m_quests;

		uint32* m_ritualmembers;
		uint32 m_ritualcaster, m_ritualtarget;
		uint16 m_ritualspell;

		void InitAI();
		SpellEntry* spell;

		float range;
		uint8 checkrate;
		uint16 counter;
		int32 charges;//used for type==22,to limit number of usages.
		bool invisible;//invisible
		uint8 invisibilityFlag;
		Unit* m_summoner;
		int8 bannerslot;
		int8 bannerauraslot;

		void CallScriptUpdate();


		GameObjectAIScript* GetScript() { return myScript; }

		void TrapSearchTarget();	// Traps need to find targets faster :P

		bool HasAI() { return spell != 0; }
		GOSpawn* m_spawn;
		void OnPushToWorld();
		void OnRemoveInRangeObject(Object* pObj);
		void RemoveFromWorld(bool free_guid);

		bool CanMine() {return (usage_remaining > 0);}
		void UseMine() { if(usage_remaining) usage_remaining--;}
		void CalcMineRemaining(bool force)
		{
			if(force || !usage_remaining)
				usage_remaining = GetInfo()->sound4 + RandomUInt(GetInfo()->sound5 - GetInfo()->sound4) - 1;
		}
		bool CanFish() { return (usage_remaining > 0); }
		void CatchFish() { if(usage_remaining) usage_remaining--; }
		void CalcFishRemaining(bool force)
		{
			if(force || !usage_remaining)
				usage_remaining = GetInfo()->sound2 + RandomUInt(GetInfo()->sound3 - GetInfo()->sound2) - 1;
		}
		bool HasLoot();
		uint32 GetGOReqSkill();
		MapCell* m_respawnCell;

		void SetState(uint8 state);
		uint8 GetState();

		uint32 GetOverrides() { return m_overrides; }

		//Easy Functions
		void SetDisplayId(uint32 id) { SetUInt32Value(GAMEOBJECT_DISPLAYID, id); }
		uint32 GetDisplayId() { return GetUInt32Value(GAMEOBJECT_DISPLAYID); }

		void SetParentRotation(uint8 rot, float value) { SetFloatValue(GAMEOBJECT_PARENTROTATION + rot, value); }
		float GetParentRotation(uint8 rot) { return GetFloatValue(GAMEOBJECT_PARENTROTATION + rot); }

		void SetFaction(uint32 id)
		{
			SetUInt32Value(GAMEOBJECT_FACTION, id);
			_setFaction();
		}
		uint32 GetFaction() { return GetUInt32Value(GAMEOBJECT_FACTION); }

		void SetLevel(uint32 level) { SetUInt32Value(GAMEOBJECT_LEVEL, level); }
		uint32 GetLevel() { return GetUInt32Value(GAMEOBJECT_LEVEL); }

		void SetType(uint8 type) { SetByte(GAMEOBJECT_BYTES_1, 1, type); }
		uint8 GetType() { return GetByte(GAMEOBJECT_BYTES_1, 1); }
		
		void SetFlags( uint32 flags ){ SetUInt32Value( GAMEOBJECT_FLAGS, flags ); }		
		uint32 GetFlags(){ return GetUInt32Value( GAMEOBJECT_FLAGS ); }
		void RemoveFlags( uint32 flags ){ RemoveFlag( GAMEOBJECT_FLAGS, flags ); }
		
		bool HasFlags( uint32 flags ){
			if( HasFlag( GAMEOBJECT_FLAGS, flags ) != 0 )
				return true;
			else
				return false;
		}
		
		void SetArtKit( uint8 artkit ){ SetByte( GAMEOBJECT_BYTES_1, 2, artkit ); }
		uint8 GetArtkKit(){ return GetByte( GAMEOBJECT_BYTES_1, 2 ); }
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




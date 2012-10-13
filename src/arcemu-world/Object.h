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

#ifndef _OBJECT_H
#define _OBJECT_H

class Unit;
class Group;

enum HIGHGUID_TYPE
{
    HIGHGUID_TYPE_PLAYER			= 0x00000000,
    HIGHGUID_TYPE_CORPSE			= 0x30000000,
    HIGHGUID_TYPE_ITEM				= 0x40000000,
    HIGHGUID_TYPE_CONTAINER			= 0x50000000,			// confirm this pl0x
    HIGHGUID_TYPE_DYNAMICOBJECT		= 0x60000000,
    HIGHGUID_TYPE_WAYPOINT			= 0x10000000,
    HIGHGUID_TYPE_TRANSPORTER		= 0x1FC00000,
    HIGHGUID_TYPE_GAMEOBJECT		= 0xF1100000,
    HIGHGUID_TYPE_UNIT				= 0xF1300000,
    HIGHGUID_TYPE_PET				= 0xF1400000,
    HIGHGUID_TYPE_VEHICLE			= 0xF1500000,
//===============================================
    HIGHGUID_TYPE_MASK				= 0xFFF00000,
    LOWGUID_ENTRY_MASK				= 0x00FFFFFF,
};

#define GET_TYPE_FROM_GUID(x) ( Arcemu::Util::GUID_HIPART( (x) ) & HIGHGUID_TYPE_MASK )
#define GET_LOWGUID_PART(x) ( Arcemu::Util::GUID_LOPART( (x) ) & LOWGUID_ENTRY_MASK )

#define MAX_INTERACTION_RANGE 5.0f

// TODO: fix that type mess

enum TYPE
{
    TYPE_OBJECT		 = 1,
    TYPE_ITEM		   = 2,
    TYPE_CONTAINER	  = 4,
    TYPE_UNIT		   = 8,
    TYPE_PLAYER		 = 16,
    TYPE_GAMEOBJECT	 = 32,
    TYPE_DYNAMICOBJECT  = 64,
    TYPE_CORPSE		 = 128,
    TYPE_AIGROUP		= 256,
    TYPE_AREATRIGGER	= 512,
};

enum TYPEID
{
    TYPEID_OBJECT		= 0,
    TYPEID_ITEM		  = 1,
    TYPEID_CONTAINER	 = 2,
    TYPEID_UNIT		  = 3,
    TYPEID_PLAYER		= 4,
    TYPEID_GAMEOBJECT	= 5,
    TYPEID_DYNAMICOBJECT = 6,
    TYPEID_CORPSE		= 7,
    TYPEID_AIGROUP	   = 8,
    TYPEID_AREATRIGGER   = 9,
};

enum OBJECT_UPDATE_TYPE
{
    UPDATETYPE_VALUES = 0,
    //  8 bytes - GUID
    //  Goto Update Block
    UPDATETYPE_MOVEMENT = 1,
    //  8 bytes - GUID
    //  Goto Position Update
    UPDATETYPE_CREATE_OBJECT = 2,
    //  8 bytes - GUID
    //  1 byte - Object Type (*)
    //  Goto Position Update
    //  Goto Update Block
    UPDATETYPE_CREATE_YOURSELF = 3, // Looks like 3 & 4 do the same thing
    //  4 bytes - Count
    //  Loop Count Times:
    //  8 bytes - GUID
    UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4 // <- this is correct, not sure about 3
                                      //  4 bytes - Count
                                      //  Loop Count Times:
                                      //  8 bytes - GUID

};

enum PHASECOMMANDS
{
    PHASE_SET = 0, //overwrites the phase value with the supplied one
    PHASE_ADD = 1, //adds the new bits to the current phase value
    PHASE_DEL = 2, //removes the given bits from the current phase value
    PHASE_RESET = 3 //sets the default phase of 1, same as PHASE_SET with 1 as the new value
};

typedef struct
{
	uint32 school_type;
	int32 full_damage;
	uint32 resisted_damage;
} dealdamage;

struct TransporterInfo{
	uint64 guid;
	float x;
	float y;
	float z;
	float o;
	uint32 flags;
	uint8 seat;

	TransporterInfo(){
		guid = 0;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		o = 0.0f;
		flags = 0;
		seat = 0;
	}
};


class WorldPacket;
class ByteBuffer;
class WorldSession;
class Player;
class MapCell;
class MapMgr;
class ObjectContainer;
class DynamicObject;
class Creature;
class GameObject;
class Unit;
class Group;
class Pet;
class Spell;

//====================================================================
//  Object
//  Base object for every item, unit, player, corpse, container, etc
//====================================================================
class SERVER_DECL Object : public EventableObject
{
	public:
		typedef std::set<Object*> InRangeSet;
		typedef std::map<string, void*> ExtensionSet;

		virtual ~Object();

		virtual void Update(uint32 time) { }

		//! True if object exists in world, else false
		bool IsInWorld() { return m_mapMgr != NULL; }
		virtual void AddToWorld();
		virtual void AddToWorld(MapMgr* pMapMgr);
		void PushToWorld(MapMgr*);
		virtual void RemoveFromWorld(bool free_guid);


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnPrePushToWorld()
		//  Virtual method that is called, BEFORE pushing the Object in the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnPrePushToWorld() {}


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnPushToWorld()
		//  Virtual method that is called, AFTER pushing the Object in the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnPushToWorld() {}


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnPreRemoveFromWorld()
		//  Virtual method that is called, BEFORE removing the Object from the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnPreRemoveFromWorld() {}


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnRemoveFromWorld()
		//  Virtual method that is called, AFTER removing the Object from the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnRemoveFromWorld() {}



		//! Guid always comes first
		const uint64 & GetGUID() const { return GetUInt64Value(OBJECT_FIELD_GUID); }
		void SetGUID(uint64 GUID) { SetUInt64Value(OBJECT_FIELD_GUID, GUID);  }
		const uint32 GetLowGUID() const { return m_uint32Values[ LOWGUID ]; }
		uint32 GetHighGUID() { return m_uint32Values[ HIGHGUID ]; }
		void SetLowGUID(uint32 val) { m_uint32Values[ LOWGUID ] = val; }
		void SetHighGUID(uint32 val) { m_uint32Values[ HIGHGUID ] = val; }

		const WoWGuid & GetNewGUID() const { return m_wowGuid; }
		uint32 GetEntry() { return m_uint32Values[ OBJECT_FIELD_ENTRY ]; }
		void SetEntry(uint32 value) { SetUInt32Value(OBJECT_FIELD_ENTRY, value); }

		float GetScale() { return m_floatValues[ OBJECT_FIELD_SCALE_X ]; }
		void SetScale(float scale) { SetFloatValue(OBJECT_FIELD_SCALE_X, scale); };

		const uint32 GetTypeFromGUID() const { return (m_uint32Values[ HIGHGUID ] & HIGHGUID_TYPE_MASK); }
		const uint32 GetUIdFromGUID() const { return (m_uint32Values[ LOWGUID ] & LOWGUID_ENTRY_MASK); }
		
		// type
		const uint8 & GetTypeId() const { return m_objectTypeId; }
		bool IsUnit()	{ return (m_objectTypeId == TYPEID_UNIT || m_objectTypeId == TYPEID_PLAYER); }
		bool IsPlayer() { return m_objectTypeId == TYPEID_PLAYER; }
		bool IsCreature() { return m_objectTypeId == TYPEID_UNIT; }
		bool IsItem() { return m_objectTypeId == TYPEID_ITEM; }
		virtual bool IsPet() { return false; }
		virtual bool IsTotem() { return false; }
		virtual bool IsSummon() { return false; }
		virtual bool IsVehicle(){ return false; }
		bool IsGameObject() { return m_objectTypeId == TYPEID_GAMEOBJECT; }
		bool IsCorpse() { return m_objectTypeId == TYPEID_CORPSE; }
		bool IsContainer() { return m_objectTypeId == TYPEID_CONTAINER; }

		//! This includes any nested objects we have, inventory for example.
		virtual uint32  BuildCreateUpdateBlockForPlayer(ByteBuffer* data, Player* target);
		uint32  BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, Player* target);
		uint32  BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, UpdateMask* mask);
		uint32  BuildOutOfRangeUpdateBlock(ByteBuffer* buf);

		WorldPacket* BuildFieldUpdatePacket(uint32 index, uint32 value);
		void BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value);
		void BuildFieldUpdatePacket(ByteBuffer* buf, uint32 Index, uint32 Value);

		virtual void DealDamage(Unit* pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);

		void BuildHeartBeatMsg(WorldPacket* data) const;

		bool SetPosition(float newX, float newY, float newZ, float newOrientation, bool allowPorting = false);
		bool SetPosition(const LocationVector & v, bool allowPorting = false);

		const float & GetPositionX() const { return m_position.x; }
		const float & GetPositionY() const { return m_position.y; }
		const float & GetPositionZ() const { return m_position.z; }
		const float & GetOrientation() const { return m_position.o; }
		void SetOrientation(float o) { m_position.o = o; }

		const float & GetSpawnX() const { return m_spawnLocation.x; }
		const float & GetSpawnY() const { return m_spawnLocation.y; }
		const float & GetSpawnZ() const { return m_spawnLocation.z; }
		const float & GetSpawnO() const { return m_spawnLocation.o; }

		const LocationVector & GetPosition() { return m_position; }
		LocationVector & GetPositionNC() { return m_position; }
		LocationVector* GetPositionV() { return &m_position; }

		//! Distance Calculation
		float CalcDistance(Object* Ob);
		float CalcDistance(float ObX, float ObY, float ObZ);
		float CalcDistance(Object* Oa, Object* Ob);
		float CalcDistance(Object* Oa, float ObX, float ObY, float ObZ);
		float CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ);
		//! NYS: scriptdev2
		bool IsInMap(Object* obj) { return GetMapId() == obj->GetMapId() && GetInstanceID() == obj->GetInstanceID(); }
		bool IsWithinDistInMap(Object* obj, const float dist2compare) const;
		bool IsWithinLOSInMap(Object* obj);
		bool IsWithinLOS(LocationVector location);

		//! Only for MapMgr use
		MapCell* GetMapCell() const;
		const uint32 GetMapCellX() { return m_mapCell_x; }
		const uint32 GetMapCellY() { return m_mapCell_y; }
		//! Only for MapMgr use
		void SetMapCell(MapCell* cell);
		//! Only for MapMgr use
		MapMgr* GetMapMgr() const { return m_mapMgr; }

		Object* GetMapMgrObject(const uint64 & guid);
		Pet* GetMapMgrPet(const uint64 & guid);
		Unit* GetMapMgrUnit(const uint64 & guid);
		Player* GetMapMgrPlayer(const uint64 & guid);
		Creature* GetMapMgrCreature(const uint64 & guid);
		GameObject* GetMapMgrGameObject(const uint64 & guid);
		DynamicObject* GetMapMgrDynamicObject(const uint64 & guid);

		void SetMapId(uint32 newMap) { m_mapId = newMap; }
		void SetZoneId(uint32 newZone);

		const uint32 GetMapId() const { return m_mapId; }
		const uint32 & GetZoneId() const { return m_zoneId; }

		//! Get uint32 property
		const uint32 & GetUInt32Value(uint32 index) const
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_uint32Values[ index ];
		}

		const uint64 & GetUInt64Value(uint32 index) const
		{
			ARCEMU_ASSERT(index + uint32(1) < m_valuesCount);

			uint64* p = reinterpret_cast< uint64* >(&m_uint32Values[ index ]);

			return *p;
		}

		//! Get float property
		const float & GetFloatValue(uint32 index) const
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_floatValues[ index ];
		}

		void  ModFloatValue(const uint32 index, const float value);
		void ModFloatValueByPCT(const uint32 index, int32 byPct);
		void ModSignedInt32Value(uint32 index, int32 value);
		void ModUnsigned32Value(uint32 index, int32 mod);
		uint32 GetModPUInt32Value(const uint32 index, const int32 value);

		//! Set uint32 property
		void SetByte(uint32 index, uint32 index1, uint8 value);

		uint8 GetByte(uint32 i, uint32 i1)
		{
			ARCEMU_ASSERT(i < m_valuesCount);
			ARCEMU_ASSERT(i1 < 4);
			return ((uint8*)m_uint32Values)[i * 4 + i1];
		}

		void  SetByteFlag(uint16 index, uint8 offset, uint8 newFlag);
		void  RemoveByteFlag(uint16 index, uint8 offset, uint8 newFlag);

		bool HasByteFlag(uint32 index, uint32 index1, uint8 flag)
		{
			return ((GetByte(index, index1) & flag) != 0);
		}

		void SetNewGuid(uint32 Guid)
		{
			SetLowGUID(Guid);
			m_wowGuid.Init(GetGUID());
		}

		void EventSetUInt32Value(uint32 index, uint32 value);
		void  SetUInt32Value(const uint32 index, const uint32 value);

		//! Set uint64 property
		void  SetUInt64Value(const uint32 index, const uint64 value);

		//! Set float property
		void  SetFloatValue(const uint32 index, const float value);

		void  SetFlag(const uint32 index, uint32 newFlag);

		void  RemoveFlag(const uint32 index, uint32 oldFlag);

		uint32 HasFlag(const uint32 index, uint32 flag) const
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_uint32Values[ index ] & flag;
		}

		////////////////////////////////////////
		void ClearUpdateMask()
		{
			m_updateMask.Clear();
			m_objectUpdated = false;
		}

		bool HasUpdateField(uint32 index)
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_updateMask.GetBit(index);
		}

		//! Use this to check if a object is in range of another
		bool isInRange(Object* target, float range);

		//! Use this to Check if a object is in front of another object.
		bool isInFront(Object* target);
		//! Use this to Check if a object is in back of another object.
		bool isInBack(Object* target);
		//! Check to see if an object is in front of a target in a specified arc (in degrees)
		bool isInArc(Object* target , float degrees);
		//! NYS: Scriptdev2
		bool HasInArc(float degrees, Object* target);
		//! Calculates the angle between two positions
		float calcAngle(float Position1X, float Position1Y, float Position2X, float Position2Y);
		float calcRadAngle(float Position1X, float Position1Y, float Position2X, float Position2Y);

		//! Converts to 360 > x > 0
		float getEasyAngle(float angle);

		const float GetDistanceSq(Object* obj)
		{
			if(obj->GetMapId() != m_mapId) return 40000.0f; //enough for out of range
			return m_position.DistanceSq(obj->GetPosition());
		}

		float GetDistanceSq(LocationVector & comp)
		{
			return comp.DistanceSq(m_position);
		}

		float CalcDistance(LocationVector & comp)
		{
			return comp.Distance(m_position);
		}

		const float GetDistanceSq(float x, float y, float z)
		{
			return m_position.DistanceSq(x, y, z);
		}

		const float GetDistance2dSq(Object* obj)
		{
			if(obj->GetMapId() != m_mapId)
				return 40000.0f; //enough for out of range
			return m_position.Distance2DSq(obj->m_position);
		}

		// In-range object management, not sure if we need it
		bool IsInRangeSet(Object* pObj)
		{
			return !(m_objectsInRange.find(pObj) == m_objectsInRange.end());
		}

		virtual void AddInRangeObject(Object* pObj);

		Mutex m_inrangechangelock;

		void RemoveInRangeObject(Object* pObj);

		//////////////////////////////////////////////////////////////////////
		//void RemoveSelfFromInrangeSets()
		// Removes the Object from the inrangesets of the Objects in range
		//
		// Parameters:
		//  None
		//
		// Return Value:
		//  None
		//
		//
		///////////////////////////////////////////////////////////////////////
		void RemoveSelfFromInrangeSets();

		bool HasInRangeObjects()
		{
			return (m_objectsInRange.size() > 0);
		}

		virtual void OnRemoveInRangeObject(Object* pObj);

		virtual void ClearInRangeSet()
		{
			m_objectsInRange.clear();
			m_inRangePlayers.clear();
			m_oppFactsInRange.clear();
			m_sameFactsInRange.clear();
		}

		size_t GetInRangeCount() { return m_objectsInRange.size(); }
		size_t GetInRangePlayersCount() { return m_inRangePlayers.size();}
		InRangeSet::iterator GetInRangeSetBegin() { return m_objectsInRange.begin(); }
		InRangeSet::iterator GetInRangeSetEnd() { return m_objectsInRange.end(); }
		InRangeSet::iterator FindInRangeSet(Object* obj) { return m_objectsInRange.find(obj); }

		void RemoveInRangeObject(InRangeSet::iterator itr)
		{
			OnRemoveInRangeObject(*itr);
			m_objectsInRange.erase(itr);
		}

		bool RemoveIfInRange(Object* obj)
		{
			InRangeSet::iterator itr = m_objectsInRange.find(obj);
			if(obj->IsPlayer())
				m_inRangePlayers.erase(obj);

			if(itr == m_objectsInRange.end())
				return false;

			m_objectsInRange.erase(itr);

			return true;
		}

		bool IsInRangeSameFactSet(Object* pObj) { return (m_sameFactsInRange.count(pObj) > 0); }
		void UpdateSameFactionSet();
		std::set<Object*>::iterator GetInRangeSameFactsSetBegin() { return m_sameFactsInRange.begin(); }
		std::set<Object*>::iterator GetInRangeSameFactsSetEnd() { return m_sameFactsInRange.end(); }

		bool IsInRangeOppFactSet(Object* pObj) { return (m_oppFactsInRange.count(pObj) > 0); }
		void UpdateOppFactionSet();
		size_t GetInRangeOppFactsSize() { return m_oppFactsInRange.size(); }
		std::set<Object*>::iterator GetInRangeOppFactsSetBegin() { return m_oppFactsInRange.begin(); }
		std::set<Object*>::iterator GetInRangeOppFactsSetEnd() { return m_oppFactsInRange.end(); }
		std::set<Object*>::iterator GetInRangePlayerSetBegin() { return m_inRangePlayers.begin(); }
		std::set<Object*>::iterator GetInRangePlayerSetEnd() { return m_inRangePlayers.end(); }
		std::set<Object*> * GetInRangePlayerSet() { return &m_inRangePlayers; }

		std::set<Object*> & GetInRangePlayers() { return m_inRangePlayers; }

		std::set<Object*> & GetInRangeOpposingFactions() { return m_oppFactsInRange; }

		std::set<Object*> & GetInRangeSameFactions() { return m_sameFactsInRange; }

		std::set<Object*> & GetInRangeObjects() { return m_objectsInRange; }



		///////////////////////////////////////////////////////////////////////////
		//void OutPacket( uint16 opcode, uint16 len, const void *data )
		// Sends a packet to the Player
		//
		//
		// Parameters
		//  uint16 opcode      -   opcode of the packet
		//  uint16 len         -   length/size of the packet
		//  const void *data   -   the data that needs to be sent
		//
		//
		// Return value
		//  none
		//
		//////////////////////////////////////////////////////////////////////////
		virtual void OutPacket(uint16 opcode, uint16 len, const void* data) {};



		/////////////////////////////////////////////////////////////////////////
		//void SendPacket( WorldPacket *packet )
		// Sends a packet to the Player
		//
		// Parameters
		//  WorldPAcket *packet      -     the packet that needs to be sent
		//
		//
		// Return value
		//  none
		//
		////////////////////////////////////////////////////////////////////////
		virtual void SendPacket(WorldPacket* packet) {};

		virtual void SendMessageToSet(WorldPacket* data, bool self, bool myteam_only = false);
		void SendMessageToSet(StackBufferBase* data, bool self) { OutPacketToSet(data->GetOpcode(), static_cast<uint16>(data->GetSize()), data->GetBufferPointer(), self); }
		virtual void OutPacketToSet(uint16 Opcode, uint16 Len, const void* Data, bool self);

		/////////////////////////////////////////////////////////////////////////
		//void SendAIReaction( uint32 reaction = 2 )
		//  Notifies the player's clients about the AI reaction of this object
		//  ( NPC growl for example "aggro sound" )
		//
		//Parameter(s)
		//  uint32 reaction  -  Reaction type
		//
		//Return Value
		//  None
		//
		//
		/////////////////////////////////////////////////////////////////////////
		void SendAIReaction(uint32 reaction = 2);

		/////////////////////////////////////////////////////////////////////////
		//void SendDestroyObject()
		//  Destroys this Object for the players' clients that are nearby
		//  ( removes object from the scene )
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		/////////////////////////////////////////////////////////////////////////
		void SendDestroyObject();

		//! Fill values with data from a space separated string of uint32s.
		void LoadValues(const char* data);

		uint16 GetValuesCount() const { return m_valuesCount; }

		//! Blizzard seem to send those for all object types. weird.
		float m_walkSpeed;
		float m_runSpeed;
		float m_backWalkSpeed;
		float m_swimSpeed;
		float m_backSwimSpeed;
		float m_turnRate;
		float m_flySpeed;
		float m_backFlySpeed;

		float m_base_runSpeed;
		float m_base_walkSpeed;

		TransporterInfo transporter_info;

		uint32 m_phase; //This stores the phase, if two objects have the same bit set, then they can see each other. The default phase is 0x1.

		const uint32 GetPhase() { return m_phase; }
		virtual void Phase(uint8 command = PHASE_SET, uint32 newphase = 1);

		void EventSpellDamage(uint64 Victim, uint32 SpellID, uint32 Damage);
		void SpellNonMeleeDamageLog(Unit* pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage = false, bool no_remove_auras = false);
		virtual bool IsCriticalDamageForSpell(Object* victim, SpellEntry* spell) { return false; }
		virtual float GetCriticalDamageBonusForSpell(Object* victim, SpellEntry* spell, float amount) { return 0; }
		virtual bool IsCriticalHealForSpell(Object* victim, SpellEntry* spell) { return false; }
		virtual float GetCriticalHealBonusForSpell(Object* victim, SpellEntry* spell, float amount) { return 0; }

		//*****************************************************************************************
		//* SpellLog packets just to keep the code cleaner and better to read
		//*****************************************************************************************
		void SendSpellLog(Object* Caster, Object* Target, uint32 Ability, uint8 SpellLogType);
		void SendSpellNonMeleeDamageLog(Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToSet);
		void SendAttackerStateUpdate(Object* Caster, Object* Target, dealdamage* Dmg, uint32 Damage, uint32 Abs, uint32 BlockedDamage, uint32 HitStatus, uint32 VState);

		//object faction
		void _setFaction();
		uint32 _getFaction() {return m_faction->Faction;}

		FactionTemplateDBC* m_faction;
		FactionDBC* m_factionDBC;

		void SetInstanceID(int32 instance) { m_instanceId = instance; }
		int32 GetInstanceID() { return m_instanceId; }

		int32 event_GetInstanceID();

		// Object activation
	private:
		bool Active;
	public:
		bool IsActive() { return Active; }
		virtual bool CanActivate();
		virtual void Activate(MapMgr* mgr);
		virtual void Deactivate(MapMgr* mgr);
		//! Player is in pvp queue.
		bool m_inQueue;
		void SetMapMgr(MapMgr* mgr) { m_mapMgr = mgr; }

		void Delete()
		{
			if(IsInWorld())
				RemoveFromWorld(true);
			delete this;
		}
		//! Play's a sound to players in range.
		void PlaySoundToSet(uint32 sound_entry);
		//! Is the player in a battleground?
		bool IsInBg();
		//! What's their faction? Horde/Ally.
		uint32 GetTeam();
		//! Objects directly cannot be in a group.
		virtual Group* GetGroup() { return NULL; }

	protected:
		Object();

		//void _Create (uint32 guidlow, uint32 guidhigh);
		void _Create(uint32 mapid, float x, float y, float z, float ang);

		//! Mark values that need updating for specified player.
		virtual void _SetUpdateBits(UpdateMask* updateMask, Player* target) const;
		//! Mark values that player should get when he/she/it sees object for first time.
		virtual void _SetCreateBits(UpdateMask* updateMask, Player* target) const;
		//! Create updates that player will see
		void _BuildMovementUpdate(ByteBuffer* data, uint16 flags, uint32 flags2, Player* target);
		void _BuildValuesUpdate(ByteBuffer* data, UpdateMask* updateMask, Player* target);

		//! WoWGuid class
		WoWGuid m_wowGuid;
		//! Type id.
		uint8 m_objectTypeId;
		//! Zone id.
		uint32 m_zoneId;
		//! Continent/map id.
		uint32 m_mapId;
		//! Map manager
		MapMgr* m_mapMgr;
		//! Current map cell row and column
		uint32 m_mapCell_x, m_mapCell_y;

		/* Main Function called by isInFront(); */
		bool inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y);

		LocationVector m_position;
		LocationVector m_lastMapUpdatePosition;
		LocationVector m_spawnLocation;

		//! Object properties.
		union
		{
			uint32* m_uint32Values;
			float* m_floatValues;
		};

		//! Number of properties
		uint16 m_valuesCount;

		//! List of object properties that need updating.
		UpdateMask m_updateMask;

		//! True if object was updated
		bool m_objectUpdated;

		//! Set of Objects in range.
		//! TODO: that functionality should be moved into WorldServer.
		std::set<Object*> m_objectsInRange;
		std::set<Object*> m_inRangePlayers;
		std::set<Object*> m_oppFactsInRange;
		std::set<Object*> m_sameFactsInRange;


		int32 m_instanceId;

		ExtensionSet* m_extensions;

		// so we can set from scripts. :)
		void _SetExtension(const string & name, void* ptr);

	public:

		template<typename T>
		void SetExtension(const string & name, T ptr)
		{
			_SetExtension(name, ((void*)ptr));
		}

		template<typename T>
		T GetExtension(const string & name)
		{
			if(m_extensions == NULL)
				return ((T)NULL);
			else
			{
				ExtensionSet::iterator itr = m_extensions->find(name);
				if(itr == m_extensions->end())
					return ((T)NULL);
				else
					return ((T)itr->second);
			}
		}

		bool m_loadedFromDB;

		// Spell currently casting
		Spell* m_currentSpell;
		Spell* GetCurrentSpell() {return m_currentSpell;}
		void SetCurrentSpell(Spell* cSpell) { m_currentSpell = cSpell; }

		//Andy's crap
		virtual Object* GetPlayerOwner();
		std::set<Spell*> m_pendingSpells;

		bool GetPoint(float angle, float rad, float & outx, float & outy, float & outz, bool sloppypath = false);
		bool GetRandomPoint(float rad, float & outx, float & outy, float & outz) { return GetPoint(RandomFloat(float(M_PI * 2)), rad, outx, outy, outz); }
		bool GetRandomPoint(float rad, LocationVector & out) { return GetRandomPoint(rad, out.x, out.y, out.z); }
};


#endif




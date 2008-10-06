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

#ifndef _OBJECT_H
#define _OBJECT_H

class Unit;
class Group;

enum HIGHGUID_TYPE
{
	HIGHGUID_TYPE_UNIT				= 0xF1300000,
	HIGHGUID_TYPE_PET				= 0xF1400000,
	HIGHGUID_TYPE_GAMEOBJECT		= 0xF1100000,
	HIGHGUID_TYPE_ITEM				= 0x40000000,
	HIGHGUID_TYPE_CONTAINER			= 0x50000000,			// confirm this pl0x
	HIGHGUID_TYPE_PLAYER			= 0x00000000,
	HIGHGUID_TYPE_DYNAMICOBJECT		= 0x60000000,
	HIGHGUID_TYPE_TRANSPORTER		= 0x1FC00000,
	HIGHGUID_TYPE_WAYPOINT			= 0x10000000,
	HIGHGUID_TYPE_CORPSE			= 0x30000000,
//===============================================
	HIGHGUID_TYPE_MASK				= 0xFFF00000,
	LOWGUID_ENTRY_MASK				= 0x00FFFFFF,
};

#define GET_TYPE_FROM_GUID(x) (GUID_HIPART((x)) & HIGHGUID_TYPE_MASK)
#define GET_LOWGUID_PART(x) (GUID_LOPART((x)) & LOWGUID_ENTRY_MASK)

// TODO: fix that type mess

enum TYPE {
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

enum TYPEID {
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
	TYPEID_UNUSED			= 10,//used to signal invalid reference (object dealocated but someone is still using it)
};

enum OBJECT_UPDATE_TYPE {
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
	UPDATETYPE_CREATE_YOURSELF = 3, // looks like 3 & 4 do the same thing
	//  4 bytes - Count
	//  Loop Count Times:
	//  8 bytes - GUID
	UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4 // <- this is correct, not sure about 3
	//  4 bytes - Count
	//  Loop Count Times:
	//  8 bytes - GUID

};

typedef struct
{ 
	uint32 school_type;
	int32 full_damage;
	uint32 resisted_damage;
} dealdamage;

class WorldPacket;
class ByteBuffer;
class WorldSession;
class Player;
class MapCell;
class MapMgr;

//====================================================================
//  Object
//  Base object for every item, unit, player, corpse, container, etc
//====================================================================
class SERVER_DECL Object : public EventableObject
{
public:
	typedef std::set<Object*> InRangeSet;
	typedef std::map<string, void*> ExtensionSet;

	virtual ~Object ( );

	virtual void Update ( uint32 time ) { }
  //! True if object exists in world
 
	
	ARCEMU_INLINE bool IsInWorld() { return m_mapMgr != NULL; }
	virtual void AddToWorld();
	virtual void AddToWorld(MapMgr * pMapMgr);
	void PushToWorld(MapMgr*);
	virtual void OnPushToWorld() { }
	virtual void OnPrePushToWorld() { }
	virtual void RemoveFromWorld(bool free_guid);

	// guid always comes first
#ifndef USING_BIG_ENDIAN
	ARCEMU_INLINE const uint64& GetGUID() const { return *((uint64*)m_uint32Values); }
#else
	ARCEMU_INLINE const uint64 GetGUID() const { return GetUInt64Value(0); }
#endif

	ARCEMU_INLINE const WoWGuid& GetNewGUID() const { return m_wowGuid; }
	ARCEMU_INLINE uint32 GetEntry(){return m_uint32Values[3];}
	
	ARCEMU_INLINE const uint32 GetEntryFromGUID() const
	{
/*		uint64 entry = *(uint64*)m_uint32Values;
		entry >>= 24;
		return (uint32)(entry & 0xFFFFFFFF);*/

		return uint32( (*(uint64*)m_uint32Values >> 24) & 0xFFFFFFFF );
	}

	ARCEMU_INLINE const uint32 GetTypeFromGUID() const { return (m_uint32Values[1] & HIGHGUID_TYPE_MASK); }
	ARCEMU_INLINE const uint32 GetUIdFromGUID() const { return (m_uint32Values[0] & LOWGUID_ENTRY_MASK); }
	ARCEMU_INLINE const uint32 GetLowGUID() const { return (m_uint32Values[0]); }

	// type
	ARCEMU_INLINE const uint8& GetTypeId() const { return m_objectTypeId; }
	ARCEMU_INLINE bool IsUnit()	{ return ( m_objectTypeId == TYPEID_UNIT || m_objectTypeId == TYPEID_PLAYER ); }
	ARCEMU_INLINE bool IsPlayer() { return m_objectTypeId == TYPEID_PLAYER; }
	ARCEMU_INLINE bool IsCreature() { return m_objectTypeId == TYPEID_UNIT; }
	bool IsPet();

	//! This includes any nested objects we have, inventory for example.
	virtual uint32 __fastcall BuildCreateUpdateBlockForPlayer( ByteBuffer *data, Player *target );
	uint32 __fastcall BuildValuesUpdateBlockForPlayer( ByteBuffer *buf, Player *target );
	uint32 __fastcall BuildValuesUpdateBlockForPlayer( ByteBuffer * buf, UpdateMask * mask );
	uint32 __fastcall BuildOutOfRangeUpdateBlock( ByteBuffer *buf );

	WorldPacket* BuildFieldUpdatePacket(uint32 index,uint32 value);
	void BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value);
	void BuildFieldUpdatePacket(ByteBuffer * buf, uint32 Index, uint32 Value);

	void DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);
	

	virtual void DestroyForPlayer( Player *target ) const;

	void BuildHeartBeatMsg( WorldPacket *data ) const;
	WorldPacket * BuildTeleportAckMsg( const LocationVector & v);
	bool IsBeingTeleported() { return mSemaphoreTeleport; }
	void SetSemaphoreTeleport(bool semphsetting) { mSemaphoreTeleport = semphsetting; }

	bool SetPosition( float newX, float newY, float newZ, float newOrientation, bool allowPorting = false );
	bool SetPosition( const LocationVector & v, bool allowPorting = false);
	void SetRotation( uint64 guid );

	ARCEMU_INLINE const float& GetPositionX( ) const { return m_position.x; }
	ARCEMU_INLINE const float& GetPositionY( ) const { return m_position.y; }
	ARCEMU_INLINE const float& GetPositionZ( ) const { return m_position.z; }
	ARCEMU_INLINE const float& GetOrientation( ) const { return m_position.o; }
	ARCEMU_INLINE void SetOrientation( float &o ) { m_position.o = o; }

	ARCEMU_INLINE const float& GetSpawnX( ) const { return m_spawnLocation.x; }
	ARCEMU_INLINE const float& GetSpawnY( ) const { return m_spawnLocation.y; }
	ARCEMU_INLINE const float& GetSpawnZ( ) const { return m_spawnLocation.z; }
	ARCEMU_INLINE const float& GetSpawnO( ) const { return m_spawnLocation.o; }

	ARCEMU_INLINE const LocationVector & GetPosition() { return m_position; }
	ARCEMU_INLINE LocationVector & GetPositionNC() { return m_position; }
	ARCEMU_INLINE LocationVector * GetPositionV() { return &m_position; }

	//Distance Calculation
	float CalcDistance(Object* Ob);
	float CalcDistance(float ObX, float ObY, float ObZ);
	float CalcDistance(Object *Oa, Object *Ob);
	float CalcDistance(Object *Oa, float ObX, float ObY, float ObZ);
	float CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ);
	// scriptdev2
	bool IsInMap( Object* obj) { return GetMapId()==obj->GetMapId() && GetInstanceID()==obj->GetInstanceID(); }
 	bool IsWithinDistInMap( Object* obj, const float dist2compare) const;
	bool IsWithinLOSInMap( Object* obj);
	bool IsWithinLOS( LocationVector location  );

	//! Only for MapMgr use
	ARCEMU_INLINE MapCell* GetMapCell() const { return m_mapCell; }
	//! Only for MapMgr use
	ARCEMU_INLINE void SetMapCell(MapCell* cell) { m_mapCell = cell; }
	//! Only for MapMgr use
	ARCEMU_INLINE MapMgr* GetMapMgr() const { return m_mapMgr; }

	ARCEMU_INLINE void SetMapId(uint32 newMap) { m_mapId = newMap; }
	void SetZoneId(uint32 newZone);

	ARCEMU_INLINE const uint32 GetMapId( ) const { return m_mapId; }
	ARCEMU_INLINE const uint32& GetZoneId( ) const { return m_zoneId; }

	//! Get uint32 property
	ARCEMU_INLINE const uint32& GetUInt32Value( uint32 index ) const
	{
		ASSERT( index < m_valuesCount );
		return m_uint32Values[ index ];
	}

	//! Get uint64 property
#ifdef USING_BIG_ENDIAN
        __inline const uint64 GetUInt64Value( uint32 index ) const
#else
	ARCEMU_INLINE const uint64& GetUInt64Value( uint32 index ) const
#endif
	{
		ASSERT( index + uint32(1) < m_valuesCount );
#ifdef USING_BIG_ENDIAN
		/* these have to be swapped here :< */
		return uint64((uint64(m_uint32Values[index+1]) << 32) | m_uint32Values[index]);
#else
		return *((uint64*)&(m_uint32Values[ index ]));
#endif
	}

	//! Get float property
	ARCEMU_INLINE const float& GetFloatValue( uint32 index ) const
	{
		ASSERT( index < m_valuesCount );
		return m_floatValues[ index ];
	}

	void __fastcall ModFloatValue(const uint32 index, const float value );
	void ModFloatValueByPCT(const uint32 index, int32 byPct );
	void ModSignedInt32Value(uint32 index, int32 value);
	void ModUnsigned32Value(uint32 index, int32 mod);
	uint32 GetModPUInt32Value(const uint32 index, const int32 value);

	//! Set uint32 property
	void SetByte(uint32 index, uint32 index1,uint8 value);

	ARCEMU_INLINE uint8 GetByte(uint32 i,uint32 i1)
	{
		ASSERT( i < m_valuesCount);
		ASSERT(i1 < 4);
#ifdef USING_BIG_ENDIAN
		return ((uint8*)m_uint32Values)[i*4+(3-i1)];
#else
		return ((uint8*)m_uint32Values)[i*4+i1];
#endif
	}
	
	ARCEMU_INLINE void SetNewGuid(uint32 Guid)
	{
		SetUInt32Value(OBJECT_FIELD_GUID, Guid);
		m_wowGuid.Init(GetGUID());
	}

	void EventSetUInt32Value(uint32 index, uint32 value);
	void __fastcall SetUInt32Value( const uint32 index, const uint32 value );

	//! Set uint64 property
	void __fastcall SetUInt64Value( const uint32 index, const uint64 value );

	//! Set float property
	void __fastcall SetFloatValue( const uint32 index, const float value );

	void __fastcall SetFlag( const uint32 index, uint32 newFlag );

	void __fastcall RemoveFlag( const uint32 index, uint32 oldFlag );

	ARCEMU_INLINE bool HasFlag( const uint32 index, uint32 flag ) const
	{
		ASSERT( index < m_valuesCount );
		return (m_uint32Values[ index ] & flag) != 0;
	}
	
	////////////////////////////////////////
	void ClearUpdateMask( )
	{
		m_updateMask.Clear();
		m_objectUpdated = false;
	}

	bool HasUpdateField(uint32 index)
	{
		ASSERT( index < m_valuesCount);
		return m_updateMask.GetBit(index);
	}

	//use it to check if a object is in range of another
	bool isInRange(Object* target, float range);


	// Use it to Check if a object is in front of another one
	bool isInFront(Object* target);
	bool isInBack(Object* target);
	// Check to see if an object is in front of a target in a specified arc (in degrees)
	bool isInArc(Object* target , float degrees); 
	bool HasInArc( float degrees, Object* target );  // scriptdev2
	/* Calculates the angle between two Positions */
	float calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );
	float calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );

	/* converts to 360 > x > 0 */
	float getEasyAngle( float angle );

	ARCEMU_INLINE const float GetDistanceSq(Object* obj)
	{
		if(obj->GetMapId() != m_mapId) return 40000.0f; //enough for out of range
		return m_position.DistanceSq(obj->GetPosition());
	}

	ARCEMU_INLINE float GetDistanceSq(LocationVector & comp)
	{
		return comp.DistanceSq(m_position);
	}

	ARCEMU_INLINE float CalcDistance(LocationVector & comp)
	{
		return comp.Distance(m_position);
	}

	ARCEMU_INLINE const float GetDistanceSq(float x, float y, float z)
	{
		return m_position.DistanceSq(x, y, z);
	}

	ARCEMU_INLINE const float GetDistance2dSq( Object* obj )
	{
		if( obj->GetMapId() != m_mapId )
			return 40000.0f; //enough for out of range
		return m_position.Distance2DSq( obj->m_position );
	}

	// In-range object management, not sure if we need it
	ARCEMU_INLINE bool IsInRangeSet( Object* pObj )
	{
		return !( m_objectsInRange.find( pObj ) == m_objectsInRange.end() );
	}
	
	virtual void AddInRangeObject(Object* pObj)
	{
		if( pObj == NULL )
			return;

		if( pObj == this )
			printf("!!!!we are in range of self !\n");

		//Zack: as far as i know list inserts do not corrupt iterators
		m_objectsInRange.insert( pObj );

		// NOTES: Since someone will come along and try and change it.
		// Don't reinrepret_cast has to be used static_cast will not work when we are
		// inside the class we are casting from if we want to cast up the inheritance
		// chain, as Object has no concept of Player.

		if( pObj->GetTypeId() == TYPEID_PLAYER )
			m_inRangePlayers.insert( reinterpret_cast< Player* >( pObj ) );
	}

	Mutex m_inrangechangelock;
	ARCEMU_INLINE void AquireInrangeLock(){ m_inrangechangelock.Acquire(); }
	ARCEMU_INLINE void ReleaseInrangeLock(){ m_inrangechangelock.Release(); }

	ARCEMU_INLINE void RemoveInRangeObject( Object* pObj )
	{
		if( pObj == NULL )
			return;

		if( pObj == this )
			printf("!!!!we are in range of self !\n");
		AquireInrangeLock();
		OnRemoveInRangeObject( pObj );
		m_objectsInRange.erase( pObj );
		ReleaseInrangeLock();
	}

	ARCEMU_INLINE bool HasInRangeObjects()
	{
		return ( m_objectsInRange.size() > 0 );
	}

	virtual void OnRemoveInRangeObject( Object * pObj )
	{
		if( pObj->GetTypeId() == TYPEID_PLAYER )
			ASSERT( m_inRangePlayers.erase( reinterpret_cast< Player* >( pObj ) ) == 1 );
	}

	virtual void ClearInRangeSet()
	{
		m_objectsInRange.clear();
		m_inRangePlayers.clear();
		m_oppFactsInRange.clear();
		m_sameFactsInRange.clear();
	}

	ARCEMU_INLINE size_t GetInRangeCount() { return m_objectsInRange.size(); }
	ARCEMU_INLINE size_t GetInRangePlayersCount() { return m_inRangePlayers.size();}
	ARCEMU_INLINE InRangeSet::iterator GetInRangeSetBegin() { return m_objectsInRange.begin(); }
	ARCEMU_INLINE InRangeSet::iterator GetInRangeSetEnd() { return m_objectsInRange.end(); }
	ARCEMU_INLINE InRangeSet::iterator FindInRangeSet(Object * obj) { return m_objectsInRange.find(obj); }

	void RemoveInRangeObject(InRangeSet::iterator itr)
	{ 
		AquireInrangeLock();
		OnRemoveInRangeObject(*itr);
		m_objectsInRange.erase(itr);
		ReleaseInrangeLock();
	}

	ARCEMU_INLINE bool RemoveIfInRange( Object * obj )
	{
		InRangeSet::iterator itr = m_objectsInRange.find(obj);
		if( obj->GetTypeId() == TYPEID_PLAYER )
			m_inRangePlayers.erase( reinterpret_cast< Player* >( obj ) );

		if( itr == m_objectsInRange.end() )
			return false;
		
		AquireInrangeLock();
		m_objectsInRange.erase( itr );
		ReleaseInrangeLock();
		return true;
	}

	ARCEMU_INLINE void AddInRangePlayer( Object * obj )
	{
		m_inRangePlayers.insert( reinterpret_cast< Player* >( obj ) );
	}

	ARCEMU_INLINE void RemoveInRangePlayer( Object * obj )
	{
		m_inRangePlayers.erase( reinterpret_cast< Player* >( obj ) );
	}

	bool IsInRangeSameFactSet(Object* pObj) { return (m_sameFactsInRange.count(pObj) > 0); }
	void UpdateSameFactionSet();
	ARCEMU_INLINE std::set<Object*>::iterator GetInRangeSameFactsSetBegin() { return m_sameFactsInRange.begin(); }
	ARCEMU_INLINE std::set<Object*>::iterator GetInRangeSameFactsSetEnd() { return m_sameFactsInRange.end(); }

	bool IsInRangeOppFactSet(Object* pObj) { return (m_oppFactsInRange.count(pObj) > 0); }
	void UpdateOppFactionSet();
	ARCEMU_INLINE size_t GetInRangeOppFactsSize(){ return m_oppFactsInRange.size(); }
	ARCEMU_INLINE std::set<Object*>::iterator GetInRangeOppFactsSetBegin() { return m_oppFactsInRange.begin(); }
	ARCEMU_INLINE std::set<Object*>::iterator GetInRangeOppFactsSetEnd() { return m_oppFactsInRange.end(); }
	ARCEMU_INLINE std::set<Player*>::iterator GetInRangePlayerSetBegin() { return m_inRangePlayers.begin(); }
	ARCEMU_INLINE std::set<Player*>::iterator GetInRangePlayerSetEnd() { return m_inRangePlayers.end(); }
	ARCEMU_INLINE std::set<Player*> * GetInRangePlayerSet() { return &m_inRangePlayers; };

	void __fastcall SendMessageToSet(WorldPacket *data, bool self,bool myteam_only=false);
	ARCEMU_INLINE void SendMessageToSet(StackBufferBase * data, bool self) { OutPacketToSet(data->GetOpcode(), data->GetSize(), data->GetBufferPointer(), self); }
	void OutPacketToSet(uint16 Opcode, uint16 Len, const void * Data, bool self);

	//! Fill values with data from a space seperated string of uint32s.
	void LoadValues(const char* data);

	ARCEMU_INLINE uint16 GetValuesCount() const { return m_valuesCount; }

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

	void EventSpellDamage(uint64 Victim, uint32 SpellID, uint32 Damage);
	void SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage = false, bool no_remove_auras = false);
	
	//*****************************************************************************************
	//* SpellLog packets just to keep the code cleaner and better to read
	//*****************************************************************************************
	void SendSpellLog(Object *Caster, Object *Target,uint32 Ability, uint8 SpellLogType);
	void SendSpellNonMeleeDamageLog( Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToSet );
	void SendAttackerStateUpdate( Object* Caster, Object* Target, dealdamage *Dmg, uint32 Damage, uint32 Abs, uint32 BlockedDamage, uint32 HitStatus, uint32 VState );

	//object faction
	void _setFaction();
	uint32 _getFaction(){return m_faction->Faction;}
	
	FactionTemplateDBC *m_faction;
	FactionDBC *m_factionDBC;

	ARCEMU_INLINE void SetInstanceID(int32 instance) { m_instanceId = instance; }
	ARCEMU_INLINE int32 GetInstanceID() { return m_instanceId; }

	int32 event_GetInstanceID();

	bool Active;
	bool CanActivate();
	void Activate(MapMgr * mgr);
	void Deactivate(MapMgr * mgr);
	bool m_inQueue;
	ARCEMU_INLINE void SetMapMgr(MapMgr * mgr) { m_mapMgr = mgr; }

	void Delete()
	{
		if(IsInWorld())
			RemoveFromWorld(true);
		delete this;
	}

	void GMScriptEvent(void * function, uint32 argc, uint32 * argv, uint32 * argt);
	ARCEMU_INLINE size_t GetInRangeOppFactCount() { return m_oppFactsInRange.size(); }
	void PlaySoundToSet(uint32 sound_entry);
	bool IsInBg();
	uint32 GetTeam();
	ARCEMU_INLINE virtual Group *GetGroup() { return NULL; } //objects directly cannot be in a group

protected:
	Object (  );

	//void _Create (uint32 guidlow, uint32 guidhigh);
	void _Create( uint32 mapid, float x, float y, float z, float ang);

	//! Mark values that need updating for specified player.
	virtual void _SetUpdateBits(UpdateMask *updateMask, Player *target) const;
	//! Mark values that player should get when he/she/it sees object for first time.
	virtual void _SetCreateBits(UpdateMask *updateMask, Player *target) const;

	void _BuildMovementUpdate( ByteBuffer *data, uint8 flags, uint32 flags2, Player* target );
	void _BuildValuesUpdate( ByteBuffer *data, UpdateMask *updateMask, Player* target );


	//! WoWGuid class
	WoWGuid m_wowGuid;

	 //! Type id.
	uint8 m_objectTypeId;

	//! Zone id.
	uint32 m_zoneId;
	//! Continent/map id.
	uint32 m_mapId;
	//! Map manager
	MapMgr *m_mapMgr;
	//! Current map cell
	MapCell *m_mapCell;

	/* Main Function called by isInFront(); */
	bool inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y );
	
	LocationVector m_position;
	LocationVector m_lastMapUpdatePosition;
	LocationVector m_spawnLocation;

	// Semaphores - needed to forbid two operations on the same object at the same very time (may cause crashing\lack of data)
	bool mSemaphoreTeleport;

	//! Object properties.
	union {
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
	std::set<Player*> m_inRangePlayers;
	std::set<Object*> m_oppFactsInRange;
	std::set<Object*> m_sameFactsInRange;
   
  
	//! Remove object from map
	void _RemoveFromMap();

	int32 m_instanceId;

	ExtensionSet * m_extensions;
	void _SetExtension(const string& name, void* ptr);		// so we can set from scripts. :)

public:

	template<typename T>
		void SetExtension(const string& name, T ptr)
	{
		_SetExtension(name, ((void*)ptr));
	}

	template<typename T>
		T GetExtension(const string& name)
	{
		if( m_extensions == NULL )
			return ((T)NULL);
		else
		{
			ExtensionSet::iterator itr = m_extensions->find( name );
			if( itr == m_extensions->end() )
				return ((T)NULL);
			else
				return ((T)itr->second);
		}
	}

	bool m_loadedFromDB;
};

#endif




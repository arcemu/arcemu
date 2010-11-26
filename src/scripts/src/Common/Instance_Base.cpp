/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#include "Setup.h"

MoonInstanceScript::MoonInstanceScript( MapMgr* pMapMgr ) : InstanceScript( pMapMgr )
{
	mUpdateFrequency = DEFAULT_UPDATE_FREQUENCY;
};

MoonInstanceScript::~MoonInstanceScript()
{
};

Creature* MoonInstanceScript::GetCreatureBySqlId( uint32 pSqlId )
{
	if ( pSqlId == 0 )
		return NULL;

	return mInstance->GetSqlIdCreature( pSqlId );
};

Creature* MoonInstanceScript::GetCreatureByGuid( uint32 pGuid )
{
	if ( pGuid == 0 )
		return NULL;

	return mInstance->GetCreature( pGuid );
};

Creature*	MoonInstanceScript::FindClosestCreatureOnMap( uint32 pEntry, float pX, float pY, float pZ )
{
	CreatureSet Creatures = FindCreaturesOnMap( pEntry );

	if ( Creatures.size() == 0 )
		return NULL;
	if ( Creatures.size() == 1 )
		return *( Creatures.begin() );

	Creature* NearestCreature = NULL;
	float Distance, NearestDistance = 99999;
	for ( CreatureSet::iterator Iter = Creatures.begin(); Iter != Creatures.end(); ++Iter )
	{
		Distance = GetRangeToObject( *Iter, pX, pY, pZ );
		if ( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestCreature = ( *Iter );
		};
	};

	return NearestCreature;
};

Creature* MoonInstanceScript::SpawnCreature( uint32 pEntry, float pX, float pY, float pZ, float pO )
{
	Creature* NewCreature = mInstance->GetInterface()->SpawnCreature( pEntry, pX, pY, pZ, pO, true, true, NULL, NULL);
	return NewCreature;
};

Creature* MoonInstanceScript::SpawnCreature( uint32 pEntry, float pX, float pY, float pZ, float pO, uint32 pFactionId )
{
	Creature* NewCreature = mInstance->GetInterface()->SpawnCreature( pEntry, pX, pY, pZ, pO, true, true, NULL, NULL);
	if ( NewCreature != NULL )
		NewCreature->SetFaction(pFactionId);

	return NewCreature;
};

CreatureSet MoonInstanceScript::FindCreaturesOnMap( uint32 pEntry )
{
	Creature* CurrentCreature = NULL;
	CreatureSet ReturnSet;
	for ( std::vector< Creature* >::iterator CreatureIter = mInstance->CreatureStorage.begin(); CreatureIter != mInstance->CreatureStorage.end(); ++CreatureIter )
	{
		CurrentCreature = (*CreatureIter);
		if ( CurrentCreature != NULL )
		{ 
			if ( CurrentCreature->GetEntry() == pEntry )
				ReturnSet.insert( CurrentCreature );
		};
	};

	return ReturnSet;
};

GameObject* MoonInstanceScript::FindClosestGameObjectOnMap( uint32 pEntry, float pX, float pY, float pZ )
{
	GameObjectSet GameObjects = FindGameObjectsOnMap( pEntry );

	if ( GameObjects.size() == 0 )
		return NULL;
	if ( GameObjects.size() == 1 )
		return *( GameObjects.begin() );

	GameObject* NearestObject = NULL;
	float Distance, NearestDistance = 99999;
	for ( GameObjectSet::iterator Iter = GameObjects.begin(); Iter != GameObjects.end(); ++Iter )
	{
		Distance = GetRangeToObject( *Iter, pX, pY, pZ );
		if ( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestObject = ( *Iter );
		};
	};

	return NearestObject;
};

GameObject* MoonInstanceScript::SpawnGameObject( uint32 pEntry, float pX, float pY, float pZ, float pO )
{
	GameObject* pNewGO = mInstance->GetInterface()->SpawnGameObject( pEntry, pX, pY, pZ, pO, true, NULL, NULL );
	return pNewGO;
};

GameObjectSet MoonInstanceScript::FindGameObjectsOnMap( uint32 pEntry )
{
	GameObject* CurrentObject = NULL;
	GameObjectSet ReturnSet;
	for ( std::vector< GameObject* >::iterator GOIter = mInstance->GOStorage.begin(); GOIter != mInstance->GOStorage.end(); ++GOIter )
	{
		CurrentObject = (*GOIter);
		if ( CurrentObject != NULL )
		{ 
			if ( CurrentObject->GetEntry() == pEntry )
				ReturnSet.insert( CurrentObject );
		};
	};

	return ReturnSet;
};

GameObject* MoonInstanceScript::GetGameObjectBySqlId( uint32 pSqlId )
{
	if ( pSqlId == 0 )
		return NULL;

	return mInstance->GetSqlIdGameObject( pSqlId );
};

GameObject* MoonInstanceScript::GetGameObjectByGuid( uint32 pGuid )
{
	if ( pGuid == 0 )
		return NULL;

	return mInstance->GetGameObject( pGuid );
};

void MoonInstanceScript::AddGameObjectStateByEntry( uint32 pEntry, GameObjectState pState, bool pUseQuery )
{
	if ( pEntry == 0 )
		return;

	GameObjectEntryMap::iterator Iter = mGameObjects.find( pEntry );
	if ( Iter != mGameObjects.end() )
		( *Iter ).second = pState;
	else
		mGameObjects.insert( GameObjectEntryMap::value_type( pEntry, pState ) );

	GameObject* CurrentObject = NULL;
	if ( !pUseQuery )
	{
		for ( std::vector< GameObject* >::iterator GOIter = mInstance->GOStorage.begin(); GOIter != mInstance->GOStorage.end(); ++GOIter )
		{
			CurrentObject = (*GOIter);
			if ( CurrentObject != NULL )
			{ 
				if ( CurrentObject->GetEntry() == pEntry )
					CurrentObject->SetState( pState );
			};
		};
	}
	else
	{
		QueryResult* Result = WorldDatabase.Query( "SELECT id FROM gameobject_spawns WHERE entry = %u", pEntry );
		if ( Result != NULL )
		{
			do
			{
				CurrentObject = GetGameObjectBySqlId( Result->Fetch()[ 0 ].GetUInt32() );
				if ( CurrentObject != NULL )
					CurrentObject->SetState( pState );
			}
			while ( Result->NextRow() );

			delete Result;
		};
	};
};

void MoonInstanceScript::AddGameObjectStateById( uint32 pId, GameObjectState pState )
{
	if ( pId == 0 )
		return;

	GameObject* StateObject = GetGameObjectBySqlId( pId );
	GameObjectEntryMap::iterator Iter;
	if ( StateObject != NULL )
	{
		StateObject->SetState( pState );
		Iter = mGameObjects.find( StateObject->GetEntry() );
		if ( Iter != mGameObjects.end() )
			( *Iter ).second = pState;
		else
			mGameObjects.insert( GameObjectEntryMap::value_type( StateObject->GetEntry(), pState ) );
	}
	else
	{
		QueryResult* Result = WorldDatabase.Query( "SELECT entry FROM gameobject_spawns WHERE id = %u", pId );
		if ( Result != NULL )
		{
			uint32 Entry = Result->Fetch()[ 0 ].GetUInt32();
			Iter = mGameObjects.find( Entry );
			if ( Iter != mGameObjects.end() )
				( *Iter ).second = pState;
			else
				mGameObjects.insert( GameObjectEntryMap::value_type( Entry, pState ) );

			delete Result;
		};
	};
};

float MoonInstanceScript::GetRangeToObject( Object* pObjectA, Object* pObjectB )
{
	if ( pObjectA == NULL || pObjectB == NULL )
		return 0.0f;

	return GetRangeToObject( pObjectA->GetPositionX(), pObjectA->GetPositionY(), pObjectA->GetPositionZ(), pObjectB->GetPositionX(), pObjectB->GetPositionY(), pObjectB->GetPositionZ() );
};

float MoonInstanceScript::GetRangeToObject( Object* pObject, float pX, float pY, float pZ )
{
	if ( pObject == NULL )
		return 0.0f;

	return GetRangeToObject( pObject->GetPositionX(), pObject->GetPositionY(), pObject->GetPositionZ(), pX, pY, pZ );
};

float MoonInstanceScript::GetRangeToObject( float pX1, float pY1, float pZ1, float pX2, float pY2, float pZ2 )
{
	float dX = pX1 - pX2;
	float dY = pY1 - pY2;
	float dZ = pZ1 - pZ2;

	return sqrtf( dX * dX + dY * dY + dZ * dZ );
};

bool MoonInstanceScript::HasPlayers()
{
	return mInstance->GetPlayerCount() > 0;
};

size_t MoonInstanceScript::GetPlayerCount()
{
	return mInstance->GetPlayerCount();
};

Player* MoonInstanceScript::GetPlayerByGuid( uint32 pGuid )
{
	if ( pGuid == 0 )
		return NULL;

	return mInstance->GetPlayer( pGuid );
};

bool MoonInstanceScript::IsCombatInProgress()
{
	return mInstance->_combatProgress.size() > 0;
};

int32 MoonInstanceScript::AddTimer( int32 pDurationMillisec )
{
	int32 Index = mTimerIdCounter++;
	mTimers.push_back( std::make_pair( Index, pDurationMillisec ) );
	return Index;
}

int32 MoonInstanceScript::GetTimer( int32 pTimerId )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
			return TimerIter->second;
	};

	return 0;
};

void MoonInstanceScript::RemoveTimer( int32& pTimerId )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
		{
			mTimers.erase( TimerIter );
			pTimerId = INVALIDATE_TIMER;
			break;
		};
	};
};

void MoonInstanceScript::ResetTimer( int32 pTimerId, int32 pDurationMillisec )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
		{
			TimerIter->second = pDurationMillisec;
			break;
		};
	};
};

bool MoonInstanceScript::IsTimerFinished( int32 pTimerId )
{
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		if ( TimerIter->first == pTimerId )
			return ( TimerIter->second <= 0 ) ? true : false;
	};

	return false;
};

void MoonInstanceScript::CancelAllTimers()
{
	mTimers.clear();
	mTimerIdCounter = 0;
};

void MoonInstanceScript::AddWorldState(uint32 pIndex, uint32 pValue)
{
	mWorldStates.insert( make_pair( pIndex, pValue ) );
};

void MoonInstanceScript::SetWorldState(uint32 pIndex, uint32 pValue)
{
	WorldStateMap::iterator itr = mWorldStates.find( pIndex );
	if( itr == mWorldStates.end() )
		mWorldStates.insert( make_pair( pIndex, pValue ) );
	else
		itr->second = pValue;

	WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);
	data << pIndex << pValue;
	Player* pPlayer;
    for( PlayerStorageMap::iterator itr = mInstance->m_PlayerStorage.begin(); itr !=  mInstance->m_PlayerStorage.end(); ++itr )
	{
		pPlayer = itr->second;

		if( pPlayer->GetSession() )
			pPlayer->GetSession()->SendPacket(&data);
	};
};

void MoonInstanceScript::SendWorldStates(Player* pPlayer)
{
	if( !mWorldStates.size() )
		return;

	WorldPacket data(SMSG_INIT_WORLD_STATES, 10 + ( mWorldStates.size() * 8 ) );
	data << mInstance->GetMapId();
	data << uint32(0);
	data << uint32(0);
	data << uint16( mWorldStates.size() );

	for(WorldStateMap::iterator itr = mWorldStates.begin(); itr != mWorldStates.end(); ++itr)
		data << itr->first << itr->second;

	pPlayer->GetSession()->SendPacket(&data);
};

void MoonInstanceScript::RemoveWorldState(uint32 pIndex)
{
	if( !mWorldStates.size() )
		RemoveWorldStates();
	
	for(WorldStateMap::iterator itr = mWorldStates.begin(); itr != mWorldStates.end(); ++itr)
	{
		if( itr->first == pIndex )
			mWorldStates.erase( itr );
	}

	WorldPacket data(SMSG_INIT_WORLD_STATES, 10 + ( mWorldStates.size() * 8 ) );
	data << mInstance->GetMapId();
	data << uint32(0);
	data << uint32(0);
	data << uint16( mWorldStates.size() );

	for(WorldStateMap::iterator itr = mWorldStates.begin(); itr != mWorldStates.end(); ++itr)
		data << itr->first << itr->second;

	Player* pPlayer = NULL;
	for( PlayerStorageMap::iterator itr = mInstance->m_PlayerStorage.begin(); itr !=  mInstance->m_PlayerStorage.end(); ++itr )
	{
		pPlayer = itr->second;

		if(pPlayer->GetSession() )
			pPlayer->GetSession()->SendPacket(&data);
	};
};

void MoonInstanceScript::RemoveWorldStates()
{
	WorldPacket data(SMSG_INIT_WORLD_STATES, 10 );
	data << mInstance->GetMapId();
	data << uint32(0);
	data << uint32(0);
	data << uint16(0);

	Player* pPlayer = NULL;
	for( PlayerStorageMap::iterator itr = mInstance->m_PlayerStorage.begin(); itr !=  mInstance->m_PlayerStorage.end(); ++itr )
	{
		pPlayer = itr->second;

		if( pPlayer->GetSession() )
			pPlayer->GetSession()->SendPacket(&data);
	};
	
	mWorldStates.clear();
};

void MoonInstanceScript::RegisterScriptUpdateEvent()
{
	RegisterUpdateEvent( mUpdateFrequency );
};

void MoonInstanceScript::SetUpdateEventFreq( uint32 pUpdateFreq )
{
	if ( mUpdateFrequency != pUpdateFreq )
	{
		mUpdateFrequency = pUpdateFreq;
		ModifyUpdateEvent( mUpdateFrequency );
	};
};

uint32 MoonInstanceScript::GetUpdateEventFreq()
{
	return mUpdateFrequency;
};

void MoonInstanceScript::SetCellForcedStates( float pMinX, float pMaxX, float pMinY, float pMaxY, bool pActivate )
{
	if ( pMinX == pMaxX || pMinY == pMaxY )
		return;

	float Y = pMinY;
	while ( pMinX < pMaxX )
	{
		while ( pMinY < pMaxY )
		{
			MapCell* CurrentCell = mInstance->GetCellByCoords( pMinX, pMinY );
			if ( pActivate && CurrentCell == NULL )
			{
				CurrentCell = mInstance->CreateByCoords( pMinX, pMinY );
				if ( CurrentCell != NULL )
					CurrentCell->Init( mInstance->GetPosX( pMinX ), mInstance->GetPosY( pMinY ), mInstance );
			};

			if ( CurrentCell != NULL )
			{
				if ( pActivate )
					mInstance->AddForcedCell( CurrentCell );
				else
					mInstance->RemoveForcedCell( CurrentCell );
			};

			pMinY += 40.0f;
		};

		pMinY = Y;
		pMinX += 40.0f;
	};
};

void MoonInstanceScript::OnPlayerDeath( Player* pVictim, Unit* pKiller )
{
};

void MoonInstanceScript::OnPlayerEnter( Player* pPlayer )
{
};

void MoonInstanceScript::OnAreaTrigger( Player* pPlayer, uint32 pAreaId )
{
};

void MoonInstanceScript::OnZoneChange( Player* pPlayer, uint32 pNewZone, uint32 pOldZone )
{
};

void MoonInstanceScript::SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData )
{
};

uint32 MoonInstanceScript::GetInstanceData( uint32 pType, uint32 pIndex )
{
	return 0;
};

void MoonInstanceScript::OnCreatureDeath( Creature* pVictim, Unit* pKiller )
{
};

void MoonInstanceScript::OnCreaturePushToWorld( Creature* pCreature )
{
};

void MoonInstanceScript::OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer )
{
};

void MoonInstanceScript::OnGameObjectPushToWorld( GameObject* pGameObject )
{
	// Dunno how part of those would happen
	if ( mGameObjects.size() == 0 || pGameObject == NULL )
		return;

	GameObjectEntryMap::iterator Iter = mGameObjects.find( pGameObject->GetEntry() );
	if ( Iter != mGameObjects.end() )
		pGameObject->SetState( ( *Iter ).second );
};

GameObject* MoonInstanceScript::GetObjectForOpenLock( Player* pCaster, Spell* pSpell, SpellEntry* pSpellEntry )
{
	return NULL;
};

void MoonInstanceScript::SetLockOptions( uint32 pEntryId, GameObject* pGameObject )
{
};

uint32 MoonInstanceScript::GetRespawnTimeForCreature( uint32 pEntryId, Creature* pCreature)
{
	return 240000;
};

void MoonInstanceScript::OnLoad()
{
};

void MoonInstanceScript::UpdateEvent()
{
	//uint32 CurrentTime = static_cast< uint32 >( time( NULL ) );
	for ( TimerArray::iterator TimerIter = mTimers.begin(); TimerIter != mTimers.end(); ++TimerIter )
	{
		TimerIter->second -= mUpdateFrequency;
	};
};

void MoonInstanceScript::Destroy()
{
	delete this;
};

void MoonInstanceScript::BuildEncounterMap()
{
	if ( mInstance->pInstance == NULL )
		return;

	QueryResult* KillResult = WorldDatabase.Query( "SELECT id, entry FROM creature_spawns WHERE map = %u AND entry IN ( SELECT entry FROM creature_names WHERE rank = 3 )", mInstance->GetMapId() );
	if ( KillResult != NULL )
	{
		uint32 Id = 0, Entry = 0;
		Field* CurrentField = NULL;
		EncounterMap::iterator EncounterIter;
		EncounterState State = State_NotStarted;
		bool StartedInstance = mInstance->pInstance->m_killedNpcs.size() > 0;
		do
		{
			CurrentField = KillResult->Fetch();
			Id = CurrentField[ 0 ].GetUInt32();
			Entry = CurrentField[ 1 ].GetUInt32();

			EncounterIter = mEncounters.find( Entry );
			if ( EncounterIter != mEncounters.end() )
				continue;

			if ( StartedInstance )
			{
				if ( mInstance->pInstance->m_killedNpcs.find( Id ) != mInstance->pInstance->m_killedNpcs.end() )
					State = State_Finished;
				else
					State = State_NotStarted;
			};

			mEncounters.insert( EncounterMap::value_type( Entry, BossData( Id, 0, State ) ) );
		}
		while ( KillResult->NextRow() );

		delete KillResult;
	};
};

// Dynamic data creation that still involves MySQL
void MoonInstanceScript::BuildEncounterMapWithEntries( IdVector pEntries )
{
	if ( mInstance->pInstance == NULL || pEntries.size() == 0 )
		return;

	std::stringstream Query;
	Query << "SELECT id, entry FROM creature_spawns WHERE entry IN ( ";
	Query << pEntries[ 0 ];
	for ( size_t i = 1; i < pEntries.size(); ++i )
	{
		Query << ", ";
		Query << pEntries[ i ];
	};

	Query << " )";
	QueryResult* KillResult = WorldDatabase.Query( Query.str().c_str() );
	if ( KillResult != NULL )
	{
		uint32 Id = 0, Entry = 0;
		Field* CurrentField = NULL;
		EncounterMap::iterator EncounterIter;
		EncounterState State = State_NotStarted;
		bool StartedInstance = mInstance->pInstance->m_killedNpcs.size() > 0;
		do
		{
			CurrentField = KillResult->Fetch();
			Id = CurrentField[ 0 ].GetUInt32();
			Entry = CurrentField[ 1 ].GetUInt32();

			EncounterIter = mEncounters.find( Entry );
			if ( EncounterIter != mEncounters.end() )
				continue;

			if ( StartedInstance )
			{
				if ( mInstance->pInstance->m_killedNpcs.find( Id ) != mInstance->pInstance->m_killedNpcs.end() )
					State = State_Finished;
				else
					State = State_NotStarted;
			};

			mEncounters.insert( EncounterMap::value_type( Entry, BossData( Id, 0, State ) ) );
		}
		while ( KillResult->NextRow() );

		delete KillResult;
	};
};

// Static data creation without MySQL use
void MoonInstanceScript::BuildEncounterMapWithIds( IdVector pIds )
{
	// Won't work with spawns that are not in world - would work well with instance fully loaded
	if ( mInstance->pInstance == NULL || pIds.size() == 0 )
		return;

	uint32 CurrentId = 0;
	EncounterState State = State_NotStarted;
	Creature* Boss = NULL;
	set< uint32 >::iterator Iter;
	EncounterMap::iterator EncounterIter;
	for ( size_t i = 0; i < pIds.size(); ++i )
	{
		CurrentId = pIds[ i ];
		if ( CurrentId == 0 )
			continue;

		Iter = mInstance->pInstance->m_killedNpcs.find( CurrentId );
		if ( Iter != mInstance->pInstance->m_killedNpcs.end() )
			State = State_Finished;

		Boss = mInstance->GetCreature( CurrentId );
		if ( Boss != NULL )
		{
			EncounterIter = mEncounters.find( Boss->GetEntry() );
			if ( EncounterIter != mEncounters.end() )
				continue;

			mEncounters.insert( EncounterMap::value_type( Boss->GetEntry(), BossData( CurrentId, Boss->GetGUID(), State ) ) );
		};

		State = State_NotStarted;
	};
};

IdVector MoonInstanceScript::BuildIdVector( uint32 pCount, ... )
{
	IdVector NewVector;
	va_list List;
	va_start( List, pCount );
	for ( uint32 i = 0; i < pCount; ++i )
	{
		NewVector.push_back( va_arg( List, uint32 ) );
	};
		
	va_end( List );
	return NewVector;
};

IdSet MoonInstanceScript::BuildIdSet( uint32 pCount, ... )
{
	IdSet NewSet;
	va_list List;
	va_start( List, pCount );
	for ( uint32 i = 0; i < pCount; ++i )
	{
		NewSet.insert( va_arg( List, uint32 ) );
	};
		
	va_end( List );
	return NewSet;
};

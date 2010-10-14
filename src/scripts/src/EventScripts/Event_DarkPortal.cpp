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
 * along with this program.  If not, see <http://www.gnu.mOrg/licenses/>.
 */

#include "StdAfx.h"
#include "Event_DarkPortal.h"

/*
 * TO DO:
 *  2) Fix Timer for Alliance Summon (and remove double add - temporary way)
 *  I) Add spawning of special units On Creature AreaTrigger reach instead of specified unit states (eventually)
 *  D) Research aggro ranges for units and add modifiers for those
 *	D) Fix facing when going out of combat
 *  6) Check why Emote State is not applied in OnCombatStop Event
 *  7) Fix faction mess up and bow use of Darnassian Archers - DB related
 */

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Outlands Instance Script

OutlandScript::OutlandScript( MapMgrPointer pMapMgr ) : MoonInstanceScript( pMapMgr )
{
	// I'm lazy... slower, but simpler
	SetCellForcedStates( -453.933472f, -28.010971f, 882.065369f, 1280.692871f );
};

void OutlandScript::Destroy()
{
	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Dark Portal Event Trigger AI

DarkPortalEventTriggerAI::DarkPortalEventTriggerAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	// Trigger settings (added MoveTo to prevent situations when unit falls on first cell loading.
	MoveTo( _unit->GetSpawnX() + 5.0f, _unit->GetSpawnY(), _unit->GetSpawnZ() );
	SetCanEnterCombat( false );
	SetFlyMode( true );
	_unit->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE );

	mAllianceSummonTimer = AddTimer( MIN_TIME_TO_SUMMON );
	mAllianceSummonTimer = AddTimer( MIN_TIME_TO_SUMMON );
	mHordeSummonTimer = AddTimer( MIN_TIME_TO_SUMMON );
	mInfernalSummonTimer = AddTimer( SUMMON_INFERNAL_TIME );
	mDemonRespawnTimer = AddTimer( DEMON_RESPAWN_TIME );
	mSpecialUnitCheckTimer = AddTimer( SPECIAL_UNIT_CHECK_TIME );
	mPitCommanderRespawnTimer = mAllianceMoveTimer = mHordeMoveTimer = mJustiMoveTimer = mJustiMoveTimer = INVALIDATE_TIMER;
	mAlliance = mHorde = NULL;
	mPitCommander = NULL;
	mJusti = NULL;
	mMelgr = NULL;
	mFormationChain = NULL;
	mInfernalSpawnPointId = 2;
	mHasSummonedInfernals = false;
	mResurrected = false;
	mResurrect = false;

	// AIUpdate registering
	RegisterAIUpdateEvent( 1000 );
};

void DarkPortalEventTriggerAI::AIUpdate()
{
	// Hack to prevent huge army spawning - not really sure if it should be here and not sure if it even works
	if ( _unit->GetMapMgr() == NULLMAPMGR )
		return;

	ParentClass::AIUpdate();
	// Hack to prevent trigger falling from his default/spawn position
	if ( _unit->GetPositionX() != _unit->GetSpawnX() || _unit->GetPositionY() != _unit->GetSpawnY() || _unit->GetPositionZ() != _unit->GetSpawnZ() )
		MoveTo( _unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ() );

	// Function to ressurect Melgromm when he dies for the first time
	if ( mResurrect && !mResurrected )
	{
		// We check if he's in memory and is alive - if so we renew his HP and make him alive
		if ( mMelgr != NULL && mMelgr->GetUnit()->IsInWorld() && !mMelgr->IsAlive() )
		{
			sEventMgr.RemoveEvents( mMelgr->GetUnit(), EVENT_CREATURE_REMOVE_CORPSE );
			mMelgr->GetUnit()->SetUInt32Value( UNIT_FIELD_HEALTH, mMelgr->GetUnit()->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
			mMelgr->GetUnit()->SetUInt32Value( UNIT_FIELD_POWER1, mMelgr->GetUnit()->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
			mMelgr->GetUnit()->SetUInt32Value( UNIT_DYNAMIC_FLAGS, 0 );
			mMelgr->GetUnit()->setDeathState( ALIVE );
			mMelgr->GetUnit()->ClearTag();
			mMelgr->GetUnit()->GetLoot()->gold = 0;
			mMelgr->GetUnit()->GetLoot()->looters.clear();
			mMelgr->GetUnit()->GetLoot()->items.clear();

			if ( mMelgr->GetWaypointCount() > 0 )
			{
				mMelgr->SetMoveType( Move_ForwardThenStop );
				if ( !mMelgr->GetCanEnterCombat() )
				{
					mMelgr->SetCanEnterCombat( true );
					mMelgr->SetWaypointToMove( 1 );
				}
				else
				{
					uint32 LastWP = static_cast< MelGrommHighmountainAI* >( mMelgr )->mLastWP;
					if ( LastWP == 0 )
						LastWP = 1;
					if ( LastWP >= mMelgr->GetWaypointCount() )
						LastWP = mMelgr->GetWaypointCount() - 1;

					mMelgr->SetWaypointToMove( LastWP );
				};
			};
		};

		// We set Ressed = true to prevent double ressurection
		mResurrected = true;
	};

	// Conditions written to set special unit pointers to null
	if ( mJusti != NULL )
	{
		// We set mJusti = NULL when he's in memory, but is not alive or is not in world
		if ( !mJusti->GetUnit()->IsInWorld() || !mJusti->IsAlive() )
			mJusti = NULL;
	};

	if ( mMelgr != NULL )
	{
		// If Melgromm is in memory, but is not alive and wasn't ressurected yet we cast 'ress' spell on him
		if ( mMelgr->GetUnit()->IsInWorld() && !mMelgr->IsAlive() && !mResurrect && !mResurrected )
		{
			// We cast Self Ressurection spell on Melgromm
			mMelgr->GetUnit()->CastSpell( mMelgr->GetUnit(), dbcSpell.LookupEntry( DARK_PORTAL_EVENT_TRIGGER_REVIVE_SELF ), true );

			// We set Ress = true to prevent double spell casting
			mResurrect = true;
		};

		// If Melgromm's pointer is valid and unit is not alive or is not in world, but was resurrected we NULL it's pointer
		if ( ( !mMelgr->IsAlive() || !mMelgr->GetUnit()->IsInWorld() ) && mResurrect && mResurrected )
		{
			// We reset Melgromm ressurection settings
			mResurrected = false;
			mResurrect = false;

			// We null pointer
			mMelgr = NULL;
		};
	};

	// We check each 5 seconds (default setting) if special units should be spawned
	if ( IsTimerFinished( mSpecialUnitCheckTimer ) )
		SpecialUnitChecks();

	// We rise special units' move timer to let them move naturally (not right after being spawned)
	if ( IsTimerFinished( mJustiMoveTimer ) )
	{
		if ( mJusti != NULL && mJusti->IsAlive() )
		{
			mJusti->SetCanEnterCombat( true );
			mJusti->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			mJusti->SetMoveType( Move_ForwardThenStop );
			mJusti->SetWaypointToMove( 1 );
		};

		RemoveTimer( mJustiMoveTimer );
	};

	if ( IsTimerFinished( mMelgrMoveTimer ) )
	{
		if ( mMelgr != NULL && mMelgr->IsAlive() )
		{
			mMelgr->SetCanEnterCombat( true );
			mMelgr->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			mMelgr->SetMoveType( Move_ForwardThenStop );
			mMelgr->SetWaypointToMove( 1 );
		};

		RemoveTimer( mMelgrMoveTimer );
	};

	// If alliance portal unit was spawned we rise it's portal move timer to let it move naturally
	if ( IsTimerFinished( mAllianceMoveTimer ) )
	{
		if ( mAlliance != NULL )
		{
			if ( !mAlliance->GetUnit()->IsInWorld() || !mAlliance->IsAlive() )
				mAlliance = NULL;
			else if ( mAlliance->IsAlive() )
			{
				mAlliance->SetCanEnterCombat( true );
				mAlliance->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
				mAlliance->SetMoveType( Move_ForwardThenStop );
				mAlliance->SetWaypointToMove( 1 );
			};
		};

		RemoveTimer( mAllianceMoveTimer );
	};

	// If horde portal unit was spawned we rise it's portal move timer to let it move naturally
	if ( IsTimerFinished( mHordeMoveTimer ) )
	{
		if ( mHorde != NULL )
		{
			if ( !mHorde->GetUnit()->IsInWorld() || !mHorde->IsAlive() )
				mHorde = NULL;
			else if ( mHorde->IsAlive() )
			{
				mHorde->SetCanEnterCombat( true );
				mHorde->GetUnit()->SetUInt64Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
				mHorde->SetMoveType( Move_ForwardThenStop );
				mHorde->SetWaypointToMove( 1 );
			};
		};

		RemoveTimer( mHordeMoveTimer );
	};

	// We try to spawn units whenever timer is finihed or we hadn't spawn new unit before
	if ( IsTimerFinished( mAllianceSummonTimer ) )
		SpawnAllianceUnit();

	if ( IsTimerFinished( mHordeSummonTimer ) )
		SpawnHordeUnit();

	// If Infernal Summon timer is equal to 0 we let Pit Commander ( if he exists ) cast spell to summon infernals
	if ( IsTimerFinished( mInfernalSummonTimer ) )
	{
		// We cast spells and after 3 seconds we spawn infernals manually
		if ( !mHasSummonedInfernals )
			PitCommanderInfernalSummon();
		else
		{
			for ( uint32 i = 0; i < 2; ++i )
			{
				InfernalSiegebreakerAI* InfernalAI = static_cast< InfernalSiegebreakerAI* >( SpawnCreature( CN_INFERNAL_SIEGEBREAKER, Infernals[ mInfernalSpawnPointId + i * 2 ].mX, Infernals[ mInfernalSpawnPointId + i * 2 ].mY, Infernals[ mInfernalSpawnPointId + i * 2 ].mZ, Infernals[ mInfernalSpawnPointId + i * 2 ].mO ) );
				if ( InfernalAI != NULL )
				{
					InfernalAI->GetUnit()->GetAIInterface()->skip_reset_hp = true;
					InfernalAI->GetUnit()->m_canRegenerateHP = false;
					InfernalAI->GetUnit()->m_noRespawn = true;
					mInfernalTable.insert( InfernalAI );
				};
			};

			ResetTimer( mInfernalSummonTimer, 47000 );
			mHasSummonedInfernals = false;
		};
	};

	if ( IsTimerFinished( mDemonRespawnTimer ) )
	{
		// After 20 seconds of spawning demons we command 5 of them (if it's 10-demon-formation) to attack.
		// After 50 seconds of first 5-demon-formation attack we let another 5 demons to attack Dark Portal (only if ten-demon formation was spawned)
		// If PitCommanderRespawn queue is empty, we spawn (if we can) another demon formation to take over Dark Portal.
		if ( mQueuedDemonSet.size() > 0 )
			PitCommanderAttackCmd();
		else
			PitCommanderSpawn();
	};
};

// Function to check Commander/Lieutenant units' states and spawn special units if needed
void DarkPortalEventTriggerAI::SpecialUnitChecks()
{
	// If Justi hasn't joined fight yet we check if he should
	if ( mJusti == NULL )
	{
		// We get Justinius unit for further use
		mJusti = static_cast< JustiniusTheHarbingerAI* >( SpawnCreature( CN_JUSTINIUS_THE_HARBINGER, AHSPoints[ 0 ].mX, AHSPoints[ 0 ].mY, AHSPoints[ 0] .mZ, AHSPoints[ 0 ].mO ) );
		if ( mJusti != NULL )
		{
			mJusti->SetCanEnterCombat( false );
			mJusti->SetMoveType( Move_DontMoveWP );
			mJusti->GetUnit()->GetAIInterface()->m_moveBackward = true;
			mJusti->GetUnit()->m_noRespawn = true;
			AddUnitToChainAggro( mJusti->GetUnit() );

			WayPoint* NewWaypoint = NULL;
			for ( uint32 x = 1; x < 11; ++x )
			{
				if ( x == 10 )
					NewWaypoint = CreateWaypoint( x, 0, Flag_Run, SWP[ 0 ] );
				else
					NewWaypoint = CreateWaypoint( x, 0, Flag_Run, AWP[x] );

				if ( x == 9 )
					NewWaypoint->waittime = 3000;

				mJusti->AddWaypoint( NewWaypoint );
			};

			// We reset movement variable
			mJustiMoveTimer = AddTimer( 5000 );
		};
	};

	// If Melgr hasn't joined fight yet we check if he should
	if ( mMelgr == NULL )
	{
		// We get Melgromm unit for further use
		mMelgr = static_cast< MelGrommHighmountainAI* >( SpawnCreature( CN_MELGROMM_HIGHMOUNTAIN, AHSPoints[ 1 ].mX, AHSPoints[ 1 ].mY, AHSPoints[ 1 ].mZ, AHSPoints[ 1 ].mO ) );
		if ( mMelgr != NULL )
		{
			mMelgr->SetCanEnterCombat( false );
			mMelgr->SetMoveType( Move_DontMoveWP );
			mMelgr->GetUnit()->GetAIInterface()->m_moveBackward = true;
			mMelgr->GetUnit()->m_noRespawn = true;
			AddUnitToChainAggro( mMelgr->GetUnit() );

			WayPoint* NewWaypoint = NULL;
			for ( uint32 x = 1; x < 12; ++x )
			{
				if ( x == 11 )
					NewWaypoint = CreateWaypoint( x, 0, Flag_Run, SWP[ 1 ] );
				else
					NewWaypoint = CreateWaypoint( x, 0, Flag_Run, HWP[ x ] );

				if ( x == 10 )
					NewWaypoint->waittime = 3000;

				mMelgr->AddWaypoint( NewWaypoint );
			};

			// We reset movement variable
			mMelgrMoveTimer = AddTimer( 5000 );
		};
	};

	// We reset special unit check timer to 0 to prevent check triggering every second
	ResetTimer( mSpecialUnitCheckTimer, SPECIAL_UNIT_CHECK_TIME );
}

// Bool used to check if we can spawn any unit for this formation to prevent its messing
bool DarkPortalEventTriggerAI::IsRegimentFull( AIContainer pVectorSet, size_t pSetSize )
{
	// We check size of unit list - if it's smaller than meant size we return true - may be spawned
	if ( pVectorSet.size() < pSetSize )
		return false;

	MoonScriptCreatureAI* SetUnit = NULL;
	// If not we check all units to find out if we can replace any
	for ( size_t i = 0; i < pSetSize; ++i )
	{
		// We check every unit from our set
		SetUnit = pVectorSet[ i ].second;

		// If unit doesn't exist or exists, but is not alive or is not in world we return true and break loop
		if ( SetUnit == NULL || !SetUnit->IsAlive() || !SetUnit->GetUnit()->IsInWorld() )
			return false;
	};

	// If it comes to this point it means regiment is full and does not need replacements
	return true;
};

// Function used to check if any alliance unit should be spawned
void DarkPortalEventTriggerAI::SpawnAllianceUnit()
{
	size_t Pos = 0;
	uint32 Entry = 0;
	uint32 Change = RandomUInt( 6 );
	AIContainer* AIList = NULL;
	Coords* UnitCoords = NULL;

	// Default unit to spawn is basic close combat soldier
	if ( !IsRegimentFull( mAllianceMelee, 10 ) )
	{
		// We change Entry and set position count in which units may be spawned
		if ( Change == 1 )
			Entry = CN_IRONFORGE_PALADIN;
		else
			Entry = CN_STORMWIND_SOLDIER;

		AIList = &mAllianceMelee;
		UnitCoords = AMWP;
		Pos = 10;
	};

	// Default unit to spawn is ranged combat soldier if melee regiment is full or if it's rolled
	if ( ( Entry == 0 || Change == 2 ) && !IsRegimentFull( mAllianceRanged, 6 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_DARNASSIAN_ARCHER;
		AIList = &mAllianceRanged;
		UnitCoords = ARWP;
		Pos = 6;
	};

	// Default unit to spawn is caster if melee and ranged regiments are full or if it's rolled
	if ( ( Entry == 0 || Change == 3 ) && !IsRegimentFull( mAllianceCaster, 2 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_STORMWIND_MAGE;
		AIList = &mAllianceCaster;
		UnitCoords = ACWP;
		Pos = 2;
	};

	// No need to go through rest of code if all regiments are full
	if ( Entry == 0 )
		return;

	// Adding false unit to expand our container
	MoonScriptCreatureAI* SetUnit = NULL;
	if ( AIList->size() < Pos )
	{
		if ( AIList->size() == 0 || ( AIList->size() > 0 && AIList->back().second != NULL ) )
			AIList->push_back( make_pair( 0, SetUnit ) );
	};

	// We go through all positions and check units
	for ( uint32 i = 0; i < AIList->size(); ++i )
	{
		// We set unit to check
		SetUnit = ( *AIList )[ i ].second;

		// If unit does not exist or exists, but is not alive or in world and unit wasn't already spawned we spawn one
		if ( SetUnit == NULL || !SetUnit->IsAlive() || !SetUnit->GetUnit()->IsInWorld() )
		{
			SetUnit = mAlliance = SpawnCreature( Entry, ASPoints[ 0 ].mX, ASPoints[ 0 ].mY, ASPoints[ 0 ].mZ, ASPoints[ 0 ].mO );
			
			// We set move out timer to let those units move after exact time and stop looping though our units
			if ( SetUnit != NULL )
			{
				// Meh, I don't like it
				switch ( Entry )
				{
				case CN_STORMWIND_SOLDIER:
					static_cast< StormwindSoldierAI* >( SetUnit )->mTriggerAI = this;
					break;
				case CN_IRONFORGE_PALADIN:
					static_cast< IronforgePaladinAI* >( SetUnit )->mTriggerAI = this;
					break;
				case CN_DARNASSIAN_ARCHER:
					static_cast< DarnassianArcherAI* >( SetUnit )->mTriggerAI = this;
					break;
				case CN_STORMWIND_MAGE:
					static_cast< StormwindMageAI* >( SetUnit )->mTriggerAI = this;
					break;
				default:
					{
					};
				};

				SetUnit->SetCanEnterCombat( false );
				SetUnit->SetMoveType( Move_DontMoveWP );
				SetUnit->GetUnit()->GetAIInterface()->m_moveBackward = true;
				SetUnit->GetUnit()->m_noRespawn = true;
				AddUnitToChainAggro( SetUnit->GetUnit() );

				WayPoint* NewWaypoint = NULL;
				for ( uint32 x = 1; x < 11; ++x )
				{
					if ( x != 10 )
						NewWaypoint = CreateWaypoint( x, 0, Flag_Run, AWP[ x ] );
					else
						NewWaypoint = CreateWaypoint( x, 0, Flag_Run, UnitCoords[ i ] );

					if ( x == 9 )
						NewWaypoint->waittime = 3000;

					SetUnit->AddWaypoint( NewWaypoint );
				};
			};

			mAllianceMoveTimer = AddTimer( 3000 );
			( *AIList )[ i ].first = Entry;
			( *AIList )[ i ].second = SetUnit;
			break;
		};
	};

	ResetTimer( mAllianceSummonTimer, SPAWN_NEW_UNIT_PORTAL );
};

// Function used to check if any horde unit should be spawned
void DarkPortalEventTriggerAI::SpawnHordeUnit()
{
	// Variable initialization
	uint32 Pos = 0;
	uint32 Entry = 0;
	uint32 Change = RandomUInt( 6 );
	AIContainer* AIList = NULL;
	Coords* UnitCoords = NULL;

	// Default unit to spawn is basic close combat soldier
	if ( !IsRegimentFull( mHordeMelee, 10 ) )
	{
		// We change Entry and set position count in which units may be spawned
		if ( Change == 1 )
			Entry = CN_ORGRIMMAR_SHAMAN;
		else
			Entry = CN_ORGRIMMAR_GRUNT;

		AIList = &mHordeMelee;
		UnitCoords = HMWP;
		Pos = 10;
	};

	// Default unit to spawn is ranged combat soldier if melee regiment is full or if it's rolled
	if ( ( Entry == 0 || Change == 2 ) && !IsRegimentFull( mHordeRanged, 6 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_DARKSPEAR_AXE_THROWER;
		AIList = &mHordeRanged;
		UnitCoords = HRWP;
		Pos = 6;
	};

	// Default unit to spawn is caster if melee and ranged regiments are full or if it's rolled
	if ( ( Entry == 0 || Change == 3 ) && !IsRegimentFull( mHordeCaster, 2 ) )
	{
		// We change Entry and set position count in which units may be spawned
		Entry = CN_UNDERCITY_MAGE;
		AIList = &mHordeCaster;
		UnitCoords = HCWP;
		Pos = 2;
	};

	// No need to go through rest of code if all regiments are full
	if ( Entry == 0 )
		return;

	// Adding false unit to expand our container
	MoonScriptCreatureAI* SetUnit = NULL;
	if ( AIList->size() < Pos )
	{
		if ( AIList->size() == 0 || ( AIList->size() > 0 && AIList->back().second != NULL ) )
			AIList->push_back( make_pair( 0, SetUnit ) );
	};

	// We go through all positions and check units
	for ( uint32 i = 0; i < AIList->size(); ++i )
	{
		// We set unit to check
		SetUnit = ( *AIList )[ i ].second;

		// If unit does not exist or exists, but is not alive or in world and unit wasn't already spawned we spawn one
		if ( SetUnit == NULL || !SetUnit->IsAlive() || !SetUnit->GetUnit()->IsInWorld() )
		{
			SetUnit = mHorde = SpawnCreature( Entry, HSPoints[ 0 ].mX, HSPoints[ 0 ].mY, HSPoints[ 0 ].mZ, HSPoints[ 0 ].mO );
			
			// We set move out timer to let those units move after exact time and stop looping though our units
			if ( SetUnit != NULL )
			{
				// Meh, I don't like it
				switch ( Entry )
				{
				case CN_ORGRIMMAR_GRUNT:
					static_cast< OrgrimmarGruntAI* >( SetUnit )->mTriggerAI = this;
					break;
				case CN_ORGRIMMAR_SHAMAN:
					static_cast< OrgrimmarShamanAI* >( SetUnit )->mTriggerAI = this;
					break;
				case CN_DARKSPEAR_AXE_THROWER:
					static_cast< DarkspearAxeThrowerAI* >( SetUnit )->mTriggerAI = this;
					break;
				case CN_UNDERCITY_MAGE:
					static_cast< UndercityMageAI* >( SetUnit )->mTriggerAI = this;
					break;
				default:
					{
					};
				};

				SetUnit->SetCanEnterCombat( false );
				SetUnit->SetMoveType( Move_DontMoveWP );
				SetUnit->GetUnit()->GetAIInterface()->m_moveBackward = true;
				SetUnit->GetUnit()->m_noRespawn = true;
				AddUnitToChainAggro( SetUnit->GetUnit() );

				WayPoint* NewWaypoint = NULL;
				for ( uint32 x = 1; x < 12; ++x )
				{
					if ( x != 11 )
						NewWaypoint = CreateWaypoint( x, 0, Flag_Run, HWP[ x ] );
					else
						NewWaypoint = CreateWaypoint( x, 0, Flag_Run, UnitCoords[ i ] );

					if ( x == 10 )
						NewWaypoint->waittime = 3000;

					SetUnit->AddWaypoint( NewWaypoint );
				};
			};

			mHordeMoveTimer = AddTimer( 3000 );
			( *AIList )[ i ].first = Entry;
			( *AIList )[ i ].second = SetUnit;
			break;
		};
	};

	ResetTimer( mHordeSummonTimer, SPAWN_NEW_UNIT_PORTAL );
}

void DarkPortalEventTriggerAI::AddUnitToChainAggro( UnitPointer pUnit )
{
#ifndef DISABLE_CHAIN_AGGRO
	if ( pUnit == NULL || !pUnit->IsCreature() || !pUnit->isAlive() || !pUnit->IsInWorld() )
		return;

	if ( mFormationChain == NULL )
	{
		mFormationChain = ChainAggroEntity::Create( TO_CREATURE( pUnit ) );
		if ( mFormationChain == NULL )
			return;
	};
	
	mFormationChain->AddAggroEntity( TO_CREATURE( pUnit ) );
#endif
};

void DarkPortalEventTriggerAI::PitCommanderChecks()
{
	// We check if Pit Commander is on the battlefield, alive and ready to fight if not 
	if ( mPitCommander == NULL || !mPitCommander->IsAlive() || !mPitCommander->GetUnit()->IsInWorld() )
	{
		if ( mPitCommanderRespawnTimer == INVALIDATE_TIMER )
		{
			if ( mPitCommander != NULL )
				mPitCommander = NULL;
			else
			{
				UnitPointer PitCommander = ForceCreatureFind( CN_PIT_COMMANDER, -249.119995f, 1169.209961f, 41.552101f );
				if ( PitCommander != NULL && PitCommander->isAlive() && TO_CREATURE( PitCommander )->GetScript() != NULL )
				{
					mPitCommander = static_cast< PitCommanderAI* >( TO_CREATURE( PitCommander )->GetScript() );
					TO_CREATURE( PitCommander )->m_noRespawn = true;
					return;
				};
			};

			mPitCommanderRespawnTimer = AddTimer( 1200000 );
		}
		else if ( IsTimerFinished( mPitCommanderRespawnTimer ) )
		{
			mPitCommander = static_cast< PitCommanderAI* >( SpawnCreature( CN_PIT_COMMANDER, -249.119995f, 1169.209961f, 41.552101f, 4.698460f ) );
			if ( mPitCommander != NULL )
				mPitCommander->GetUnit()->m_noRespawn = true;

			RemoveTimer( mPitCommanderRespawnTimer );
		};
	};
};

// Function used to summon infernals
void DarkPortalEventTriggerAI::PitCommanderInfernalSummon()
{
	// We run through whole Infernal list
	if ( mInfernalTable.size() > 0 )
	{
		// We iterate through list and remove from it invalid, not in world or not dead creatures
		for ( AIInfernalSet::iterator Iter = mInfernalTable.begin(); Iter != mInfernalTable.end(); )
		{
			if ( ( *Iter ) == NULL || !( *Iter )->GetUnit()->IsInWorld() || !( *Iter )->IsAlive() )
			{
				if ( ( *Iter ) != NULL && !( *Iter )->IsAlive() && ( *Iter )->GetUnit()->IsInWorld() )
					( *Iter )->Despawn( 1000 );

				mInfernalTable.erase( Iter++ );
				continue;
			};

			++Iter;
		};
	};

	// We validate Pit Commander for further use
	PitCommanderChecks();

	// We check if Pit Commander pointer is valid and if he's alive and we have on battlefield less than 3 infernals
	if ( mPitCommander != NULL && mInfernalTable.size() <= 2 )
	{
		// We force Pit Commander to cast spell that looks like summoning infernals
		mPitCommander->GetUnit()->CastSpell( mPitCommander->GetUnit(), DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS, true );

		// We roll Infernals' spawn positions
		uint32 i = mInfernalSpawnPointId;
		while ( i == mInfernalSpawnPointId )
		{
			i = RandomUInt( 1 );
		};

		// We force trigger to "throw huge fel stones" from it's actual position
		_unit->CastSpellAoF( Infernals[ i ].mX, Infernals[ i ].mY, Infernals[ i ].mZ, dbcSpell.LookupEntry( DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS2 ), true );
		_unit->CastSpellAoF( Infernals[ i + 2 ].mX, Infernals[ i + 2 ].mY, Infernals[ i + 2 ].mZ, dbcSpell.LookupEntry( DARK_PORTAL_EVENT_TRIGGER_SUMMON_INFERNALS2 ), true );

		// We mark infernals as spawned
		ResetTimer( mInfernalSummonTimer, 3000 );
		mHasSummonedInfernals = true;
		mInfernalSpawnPointId = i;
		return;							// will prevent adding infernals and letting demon army to move in the same time
	}

	// We reset InfernalSpawn timer to default value (50 sec)
	ResetTimer( mInfernalSummonTimer, SUMMON_INFERNAL_TIME );
}

// Function we use to send 5 demons to attack Dark Portal defenders
void DarkPortalEventTriggerAI::PitCommanderAttackCmd()
{
	// We validate Pit Commander
	PitCommanderChecks();

	// If pointer is valid, Pit Commander is not in combat we randomize emoting
	if ( mPitCommander != NULL && mPitCommander->GetUnit()->GetAIInterface()->GetNextTarget() == NULL && RandomUInt(3) == 1 )
		mPitCommander->GetUnit()->Emote( EMOTE_ONESHOT_POINT );

	// If DemonTable size is equal to 5 we send all demons to fight
	bool MultiWaves = false;

	// We enable Multiwaves if there're too many demons
	if ( mQueuedDemonSet.size() > 5 && mQueuedDemonSet.size() % 2 == 0 )
		MultiWaves = true;

	MoonScriptCreatureAI* ScriptAI = NULL;
	for ( AIMap::iterator Iter = mQueuedDemonSet.begin(); Iter != mQueuedDemonSet.end(); )
	{
		ScriptAI = ( *Iter ).first;
		// If Demon's data was not saved saved to memory, creature is not in world or is not alive we remove it from our set
		if ( ScriptAI == NULL || !ScriptAI->IsAlive() || !ScriptAI->GetUnit()->IsInWorld() )
		{
			if ( ScriptAI != NULL && !ScriptAI->IsAlive() && ScriptAI->GetUnit()->IsInWorld() )
				ScriptAI->Despawn( 1000 );

			AIMap::iterator Iter2 = mFightingDemonSet.find( ScriptAI );
			if ( Iter2 != mFightingDemonSet.end() )
				mFightingDemonSet.erase( Iter2 );
		}
		else
		{
			ScriptAI->SetMoveType( Move_ForwardThenStop );
			ScriptAI->SetWaypointToMove( 3 );
		};

		mQueuedDemonSet.erase( Iter++ );
		if ( Iter != mQueuedDemonSet.end() && MultiWaves )
			++Iter;
	};

	if ( mQueuedDemonSet.size() > 0 )
		ResetTimer( mDemonRespawnTimer, 50000 );
	else
		ResetTimer( mDemonRespawnTimer, 160000 );
};

// Void used to spawn demons
void DarkPortalEventTriggerAI::PitCommanderSpawn()
{
	// We reset Respawn timer and initialize other variables
	ResetTimer( mDemonRespawnTimer, 20000 );

	// We run through set of objects and check how many spawned demons are still fighting
	MoonScriptCreatureAI* ScriptAI = NULL;
	for ( AIMap::iterator Iter = mFightingDemonSet.begin(); Iter != mFightingDemonSet.end(); )
	{
		ScriptAI = ( *Iter ).first;
		if ( ScriptAI == NULL || !ScriptAI->IsAlive() || !ScriptAI->GetUnit()->IsInWorld() )
		{
			if ( ScriptAI != NULL && !ScriptAI->IsAlive() && ScriptAI->GetUnit()->IsInWorld() )
				ScriptAI->Despawn( 1000 );

			mFightingDemonSet.erase( Iter++ );
		}
		else
			++Iter;
	};

	// We validate Pit Commander
	PitCommanderChecks();
	if ( mPitCommander == NULL )
	{
		ResetTimer( mDemonRespawnTimer, 60000 );
		return;
	};

	// We count how many demons will be spawned
	uint32 WaveSize = 1;
	size_t DemonCount = mFightingDemonSet.size();
	if ( DemonCount > 0 )
	{
		float Rate = static_cast< float >( DemonCount ) / 5.0f;
		if ( Rate > 2 )
		{
			ResetTimer( mDemonRespawnTimer, 60000 );
			return;
		};

		WaveSize = static_cast< uint32 >( Rate );
		if ( WaveSize < 1 )
			WaveSize = 1;
	};

	// 1 / 4 waves will include 10 demons
	if ( WaveSize == 1 )
	{
		if ( RandomUInt( 3 ) > 0 )
			WaveSize = 2;
	};

	uint32 Entry = CN_FEL_SOLDIER;
	for ( uint32 i = 0; i < 10; i += WaveSize )
	{
		Entry = CN_FEL_SOLDIER;
		if ( RandomUInt( 3 ) == 1 )
			Entry = CN_WRATH_MASTER;

		MoonScriptCreatureAI* ScriptAI = SpawnCreature( Entry, DSPoints[ i ].mX, DSPoints[ i ].mY, DSPoints[ i ].mZ, DSPoints[ i ].mO );
		if ( ScriptAI != NULL )
		{
			// Crappy method
			switch ( Entry )
			{
			case CN_FEL_SOLDIER:
				static_cast< FelSoldierAI* >( ScriptAI )->mTriggerAI = this;
				break;
			case CN_WRATH_MASTER:
				static_cast< WrathMasterAI* >( ScriptAI )->mTriggerAI = this;
				break;
			default:
				{
				};
			};

			ScriptAI->SetMoveType( Move_ForwardThenStop );
			ScriptAI->GetUnit()->GetAIInterface()->m_moveBackward = true;
			ScriptAI->GetUnit()->GetAIInterface()->skip_reset_hp = true;
			ScriptAI->GetUnit()->m_canRegenerateHP = false;
			ScriptAI->GetUnit()->m_noRespawn = true;

			uint32 j = 8 * i;
			for ( uint32 x = 1; x < 9; ++x )
			{
				++j;
				ScriptAI->AddWaypoint( CreateWaypoint( x, 0, Flag_Run, PitDemonPaths[ j ] ) );
			};

			mFightingDemonSet.insert( make_pair( ScriptAI, Entry ) );
			mQueuedDemonSet.insert( make_pair( ScriptAI, Entry ) );
		};
	};
};

// Procedure that removes all event spawns
void DarkPortalEventTriggerAI::Destroy()
{
	MoonScriptCreatureAI* ScriptAI = NULL;
	for ( AIMap::iterator Iter = mFightingDemonSet.begin(); Iter != mFightingDemonSet.end(); )
	{
		ScriptAI = ( *Iter ).first;

		// Null references to Dark Portal Event Trigger AI
		if ( ScriptAI != NULL )
		{
			switch ( ( *Iter ).second )
			{
			case CN_FEL_SOLDIER:
				static_cast< FelSoldierAI* >( ScriptAI )->mTriggerAI = NULL;
				break;
			case CN_WRATH_MASTER:
				static_cast< WrathMasterAI* >( ScriptAI )->mTriggerAI = NULL;
				break;
			default:
				{
				};
			};

			ScriptAI->Despawn( 1000 );
		};
	};

	for ( AIInfernalSet::iterator Iter = mInfernalTable.begin(); Iter != mInfernalTable.end(); ++Iter )
	{
		if ( ( *Iter ) != NULL )
		{
			( *Iter )->mTriggerAI = NULL;
			( *Iter )->Despawn( 1000 );
		};
	};

	for ( AIContainer::iterator Iter = mAllianceMelee.begin(); Iter != mAllianceMelee.end(); ++Iter )
	{
		if ( ( *Iter ).second != NULL )
		{
			switch ( ( *Iter ).first )
			{
			case CN_STORMWIND_SOLDIER:
				static_cast< StormwindSoldierAI* >( ( *Iter ).second )->mTriggerAI = NULL;
				break;
			case CN_IRONFORGE_PALADIN:
				static_cast< IronforgePaladinAI* >( ( *Iter ).second )->mTriggerAI = NULL;
				break;
			default:
				{
				};
			};

			( *Iter ).second->Despawn( 1000 );
		};
	};

	for ( AIContainer::iterator Iter = mAllianceRanged.begin(); Iter != mAllianceRanged.end(); ++Iter )
	{
		if ( ( *Iter ).second != NULL )
		{
			static_cast< DarnassianArcherAI* >( ( *Iter ).second )->mTriggerAI = NULL;
			( *Iter ).second->Despawn( 1000 );
		};
	};

	for ( AIContainer::iterator Iter = mAllianceCaster.begin(); Iter != mAllianceCaster.end(); ++Iter )
	{
		if ( ( *Iter ).second != NULL )
		{
			static_cast< StormwindMageAI* >( ( *Iter ).second )->mTriggerAI = NULL;
			( *Iter ).second->Despawn( 1000 );
		};
	};

	for ( AIContainer::iterator Iter = mHordeMelee.begin(); Iter != mHordeMelee.end(); ++Iter )
	{
		if ( ( *Iter ).second != NULL )
		{
			switch ( ( *Iter ).first )
			{
			case CN_ORGRIMMAR_GRUNT:
				static_cast< OrgrimmarGruntAI* >( ( *Iter ).second )->mTriggerAI = NULL;
				break;
			case CN_ORGRIMMAR_SHAMAN:
				static_cast< OrgrimmarShamanAI* >( ( *Iter ).second )->mTriggerAI = NULL;
				break;
			default:
				{
				};
			};

			( *Iter ).second->Despawn( 1000 );
		};
	};

	for ( AIContainer::iterator Iter = mHordeRanged.begin(); Iter != mHordeRanged.end(); ++Iter )
	{
		if ( ( *Iter ).second != NULL )
		{
			static_cast< DarkspearAxeThrowerAI* >( ( *Iter ).second )->mTriggerAI = NULL;
			( *Iter ).second->Despawn( 1000 );
		};
	};

	for ( AIContainer::iterator Iter = mHordeCaster.begin(); Iter != mHordeCaster.end(); ++Iter )
	{
		if ( ( ( *Iter ).second ) != NULL )
		{
			static_cast< UndercityMageAI* >( ( *Iter ).second )->mTriggerAI = NULL;
			( ( *Iter ).second )->Despawn( 1000 );
		};
	};

	mFightingDemonSet.clear();
	mQueuedDemonSet.clear();
	mInfernalTable.clear();
	mAllianceMelee.clear();
	mAllianceRanged.clear();
	mAllianceCaster.clear();
	mHordeMelee.clear();
	mHordeRanged.clear();
	mHordeCaster.clear();

	if ( mPitCommander != NULL )
	{
		mPitCommander->mTriggerAI = NULL;
		mPitCommander->Despawn( 1000 );
		mPitCommander = NULL;
	};

	if ( mJusti != NULL )
	{
		mJusti->mTriggerAI = NULL;
		mJusti->Despawn( 1000 );
		mJusti = NULL;
	};

	if ( mMelgr != NULL )
	{
		mMelgr->mTriggerAI = NULL;
		mMelgr->Despawn( 1000 );
		mMelgr = NULL;
	};

	mAlliance = mHorde = NULL;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Mel Gromm Highmountain AI

MelGrommHighmountainAI::MelGrommHighmountainAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( MELGROMM_HIGHMOUNTAIN_EARTH_SHOCK, Target_Current, 8, 0, 10, 0, 20 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_CHAIN_LIGHTNING, Target_Current, 7, 3, 20, 0, 20 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_CHAIN_HEAL, Target_WoundedFriendly, 7, 2.5, 20, 0, 40 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_STRENGHT_OF_THE_STORM_TOTEM, Target_Self, 5, 0, 25 );
	AddSpell( MELGROMM_HIGHMOUNTAIN_MAGMA_FLOW_TOTEM, Target_Self, 5, 0, 25 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void MelGrommHighmountainAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( RandomUInt( 3 ) == 1 )
		Emote( "The attackers shall not succeed!  Strength and honor to the Horde!", Text_Yell, 0 );

	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void MelGrommHighmountainAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		_unit->SetFacing( SWP[ 1 ].mO );
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void MelGrommHighmountainAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		if ( mTriggerAI->mMelgr == this )
			mTriggerAI->mMelgr = NULL;

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Orgrimmar Grunt AI

OrgrimmarGruntAI::OrgrimmarGruntAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( ORGRIMMAR_GRUNT_IRON_BITE, Target_Current, 15, 0, 5, 0, 5 );	// is 5 enough?
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void OrgrimmarGruntAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void OrgrimmarGruntAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 375 );
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void OrgrimmarGruntAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mHordeMelee.size(); ++i )
		{
			if ( mTriggerAI->mHordeMelee[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mHordeMelee[ i ].second == this )
			{
				mTriggerAI->mHordeMelee[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Orgrimmar Shaman AI

OrgrimmarShamanAI::OrgrimmarShamanAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( ORGRIMMAR_SHAMAN_FLAME_SHOCK, Target_Current, 8, 0, 15, 0, 20 );
	AddSpell( ORGRIMMAR_SHAMAN_BLOODLUST, Target_RandomFriendly, 6, 0, 10 );
	AddSpell( ORGRIMMAR_SHAMAN_LIGHTNING_SHIELD, Target_Self, 5, 0, 25 );
	AddSpell( ORGRIMMAR_SHAMAN_SLOW, Target_RandomUnit, 6, 0, 10, 0, 30, true );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void OrgrimmarShamanAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void OrgrimmarShamanAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 45 );
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void OrgrimmarShamanAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mHordeMelee.size(); ++i )
		{
			if ( mTriggerAI->mHordeMelee[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mHordeMelee[ i ].second == this )
			{
				mTriggerAI->mHordeMelee[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Darkspear Axe Thrower AI

DarkspearAxeThrowerAI::DarkspearAxeThrowerAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( DARKSPEAR_AXE_THROWER_THROW, Target_Current, 100, 0.5f, 0, 8, 35, true );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void DarkspearAxeThrowerAI::OnCombatStart( UnitPointer pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	if ( GetRangeToUnit( pTarget ) >= 8.0f && GetRangeToUnit( pTarget ) <= 35.0f )
	{
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
	};
};

void DarkspearAxeThrowerAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void DarkspearAxeThrowerAI::AIUpdate()
{
	ParentClass::AIUpdate();
	UnitPointer Target = _unit->GetAIInterface()->GetNextTarget();
	if ( Target != NULL )
	{
		if ( GetRangeToUnit( Target ) >= 8.0f && GetRangeToUnit( Target ) <= 35.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		};
	};
};

void DarkspearAxeThrowerAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void DarkspearAxeThrowerAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mHordeRanged.size(); ++i )
		{
			if ( mTriggerAI->mHordeRanged[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mHordeRanged[ i ].second == this )
			{
				mTriggerAI->mHordeRanged[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Undercity Mage AI

UndercityMageAI::UndercityMageAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( UNDERCITY_MAGE_ICEBOLT, Target_Current, 60, 0, 0, 0, 100 );
	AddSpell( UNDERCITY_MAGE_FIREBALL, Target_Current, 25, 3, 10, 0, 100 );
	AddSpell( UNDERCITY_MAGE_BLIZZARD, Target_RandomDestination, 15, 2, 10, 0, 100 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void UndercityMageAI::OnCombatStart( UnitPointer pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
};

void UndercityMageAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void UndercityMageAI::AIUpdate()
{
	ParentClass::AIUpdate();
	UnitPointer Target = _unit->GetAIInterface()->GetNextTarget();
	if ( Target != NULL )
	{
		if ( GetRangeToUnit( Target ) <= 100.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		}
		else
			_unit->GetAIInterface()->RemoveThreatByPtr( Target );
	};
};

void UndercityMageAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 10:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 11:{
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 11 );
		if ( Waypoint != NULL )
		{
			_unit->SetFacing( Waypoint->o );
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void UndercityMageAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mHordeCaster.size(); ++i )
		{
			if ( mTriggerAI->mHordeCaster[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mHordeCaster[ i ].second == this )
			{
				mTriggerAI->mHordeCaster[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Justinius The Harbinger AI

JustiniusTheHarbingerAI::JustiniusTheHarbingerAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( JUSTINIUS_THE_HARBINGER_FLASH_OF_LIGHT, Target_WoundedFriendly, 8, 1.5, 10, 0, 40 );
	AddSpellFunc( &SpellFunc_Justinius_Consecration, Target_Self, 6, 0, 5 );
	mConsecration = AddSpell( JUSTINIUS_THE_HARBINGER_CONSECRATION, Target_Self, 0, 0, 15 );
	AddSpell( JUSTINIUS_THE_HARBINGER_JUDGEMENT_OF_COMMAND, Target_Current, 7, 0, 20, 0, 35 );
	AddSpell( JUSTINIUS_THE_HARBINGER_DIVINE_SHIELD, Target_Self, 7, 0, 20 );
	AddSpell( JUSTINIUS_THE_HARBINGER_GREATER_BLESSING_OF_MIGHT, Target_Self, 5, 0, 30 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void JustiniusTheHarbingerAI::OnCombatStart( UnitPointer pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	Emote( "", Text_Yell, 9705 );
	_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 375 );
};

void JustiniusTheHarbingerAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( RandomUInt( 3 ) == 1 )
		Emote( "Behold the power of the Light! Grace and glory to the Alliance!", Text_Yell, 0 );

	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

bool JustiniusTheHarbingerAI::CanCastConsecration()
{
	UnitArray TargetArray;
	for ( unordered_set< ObjectPointer >::iterator Iter = _unit->GetInRangeOppFactsSetBegin(); Iter != _unit->GetInRangeOppFactsSetEnd(); ++Iter )
	{
		if ( IsValidUnitTarget( *Iter, TargetFilter_None ) && GetRangeToUnit( TO_UNIT( *Iter ) ) <= 8.0f  )	// Set from 5 to 8 for better effect
			TargetArray.push_back( TO_UNIT( *Iter ) );
	};

	if ( TargetArray.size() >= 5 )
		return true;

	return false;
};

void JustiniusTheHarbingerAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		_unit->SetFacing( SWP[ 0 ].mO );
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void JustiniusTheHarbingerAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		if ( mTriggerAI->mJusti == this )
			mTriggerAI->mJusti = NULL;

		mTriggerAI = NULL;
	};

	delete this;
};

void SpellFunc_Justinius_Consecration(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	JustiniusTheHarbingerAI *pJustiniusAI = ( pCreatureAI ) ? ( JustiniusTheHarbingerAI* )pCreatureAI : NULL;
	if ( pJustiniusAI != NULL && pJustiniusAI->mConsecration != NULL )
	{
		if ( pJustiniusAI->mConsecration->mLastCastTime + pJustiniusAI->mConsecration->mCooldown <= ( uint32 )time( NULL ) )
		{
			if ( pJustiniusAI->CanCastConsecration() && !pJustiniusAI->IsCasting() )	// That's just for 'freaking out' occasions
			{
				pJustiniusAI->CastSpellNowNoScheduling( pJustiniusAI->mConsecration );
			};
		};
	};
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Stormwind Soldier AI

StormwindSoldierAI::StormwindSoldierAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( STORMWIND_SOLDIER_STRIKE, Target_Current, 15, 0, 5, 0, 5 );	// 5 enough?
	AddSpell( STORMWIND_SOLDIER_DEMORALIZING_SHOUT, Target_Self, 7, 0, 20 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void StormwindSoldierAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void StormwindSoldierAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 45 );
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void StormwindSoldierAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mAllianceCaster.size(); ++i )
		{
			if ( mTriggerAI->mAllianceCaster[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mAllianceCaster[ i ].second == this )
			{
				mTriggerAI->mAllianceCaster[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Ironforge Paladin AI

IronforgePaladinAI::IronforgePaladinAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( IRONFORGE_PALADIN_SEAL_OF_SACRIFICE, Target_RandomFriendly, 8, 0, 10 );
	AddSpell( IRONFORGE_PALADIN_EXORCISM, Target_Current, 7, 0, 0 );
	AddSpell( IRONFORGE_PALADIN_HAMMER_OF_JUSTICE, Target_Current, 7, 0, 10, 0, 10 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void IronforgePaladinAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void IronforgePaladinAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		_unit->SetUInt32Value( UNIT_NPC_EMOTESTATE, 45 );
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void IronforgePaladinAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mAllianceCaster.size(); ++i )
		{
			if ( mTriggerAI->mAllianceCaster[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mAllianceCaster[ i ].second == this )
			{
				mTriggerAI->mAllianceCaster[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Darnassian Archer AI

DarnassianArcherAI::DarnassianArcherAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( DARNASSIAN_ARCHER_SHOOT, Target_Current, 100, 0.5f, 0, 8, 35, true );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
}

void DarnassianArcherAI::OnCombatStart( UnitPointer pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	if ( GetRangeToUnit( pTarget ) >= 8.0f && GetRangeToUnit( pTarget ) <= 35.0f )
	{
		SetBehavior( Behavior_Spell );
		SetCanMove( false );
	};
};

void DarnassianArcherAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void DarnassianArcherAI::AIUpdate()
{
	ParentClass::AIUpdate();
	UnitPointer Target = _unit->GetAIInterface()->GetNextTarget();
	if ( Target != NULL )
	{
		if ( GetRangeToUnit( Target ) >= 8.0f && GetRangeToUnit( Target ) <= 35.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		};
	};
};

void DarnassianArcherAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( Waypoint != NULL )
		{
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void DarnassianArcherAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mAllianceRanged.size(); ++i )
		{
			if ( mTriggerAI->mAllianceRanged[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mAllianceRanged[ i ].second == this )
			{
				mTriggerAI->mAllianceRanged[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Stormwind Mage AI

StormwindMageAI::StormwindMageAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( STORMWIND_MAGE_FIREBALL, Target_Current, 70, 3, 0, 0, 100 );
	AddSpell( STORMWIND_MAGE_ARCANE_MISSILES, Target_Current, 15, 5, 10, 0, 100 );
	AddSpell( STORMWIND_MAGE_BLIZZARD, Target_RandomDestination, 15, 2, 10, 0, 100 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void StormwindMageAI::OnCombatStart( UnitPointer pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
};

void StormwindMageAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void StormwindMageAI::AIUpdate()
{
	ParentClass::AIUpdate();
	UnitPointer Target = _unit->GetAIInterface()->GetNextTarget();
	if ( Target != NULL )
	{
		if ( GetRangeToUnit( Target ) <= 100.0f )
		{
			SetBehavior( Behavior_Spell );
			SetCanMove( false );
		}
		else
			_unit->GetAIInterface()->RemoveThreatByPtr( Target );
	};
};

void StormwindMageAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 9:
		_unit->Emote( EMOTE_ONESHOT_SALUTE );
		break;
	case 10:{
		WayPoint* Waypoint = _unit->GetAIInterface()->getWayPoint( 10 );
		if ( Waypoint != NULL )
		{
			_unit->SetFacing( Waypoint->o );
			_unit->SetSpawnX( Waypoint->x );
			_unit->SetSpawnY( Waypoint->y );
			_unit->SetSpawnZ( Waypoint->z );
			_unit->SetSpawnO( Waypoint->o );
		};

		_unit->GetAIInterface()->deleteWaypoints();
		}break;
	};
};

void StormwindMageAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		for ( size_t i = 0; i < mTriggerAI->mAllianceCaster.size(); ++i )
		{
			if ( mTriggerAI->mAllianceCaster[ i ].first != _unit->GetEntry() )
				continue;

			if ( mTriggerAI->mAllianceCaster[ i ].second == this )
			{
				mTriggerAI->mAllianceCaster[ i ].second = NULL;
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Pit Commander AI

PitCommanderAI::PitCommanderAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( PIT_COMMANDER_RAIN_OF_FIRE, Target_RandomDestination, 7, 9, 15 );
	AddSpell( PIT_COMMANDER_CLEAVE, Target_Current, 9, 0, 15, 0, 10 );			// Added 10 'cause of size of the unit
	SetWieldWeapon( true );

	mTriggerAI = NULL;
};

void PitCommanderAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		if ( mTriggerAI->mPitCommander == this )
			mTriggerAI->mPitCommander = NULL;

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Infernal Siegebreaker AI

InfernalSiegebreakerAI::InfernalSiegebreakerAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	mSuicide = AddSpell( INFERNAL_SIEGEBREAKER_SUICIDE, Target_Self, 5, 0, 0 );
	mTriggerAI = NULL;
};

void InfernalSiegebreakerAI::OnCombatStart( UnitPointer pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	mSuicide->mEnabled = false;
};

void InfernalSiegebreakerAI::AIUpdate()
{
	if ( !mSuicide->mEnabled && GetHealthPercent() <= 5 )
		mSuicide->mEnabled = true;

	ParentClass::AIUpdate();
};

void InfernalSiegebreakerAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		typedef unordered_set< InfernalSiegebreakerAI* > AIInfernalSet;
		for ( AIInfernalSet::iterator Iter = mTriggerAI->mInfernalTable.begin(); Iter != mTriggerAI->mInfernalTable.end(); ++Iter )
		{
			if ( ( *Iter ) == this )
			{
				mTriggerAI->mInfernalTable.erase( Iter );
				break;
			};
		};

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Fel Soldier AI

FelSoldierAI::FelSoldierAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	AddSpell( FEL_SOLDIER_CUTDOWN, Target_Current, 8, 0, 15, 0, 8 );
	AddSpell( FEL_SODLIER_CLEAVE, Target_Current, 8, 0, 20, 0, 8 );
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void FelSoldierAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	MoveTo( _unit->GetPositionX(), _unit->GetPositionY() + 3.0f, _unit->GetPositionZ() );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void FelSoldierAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 3:
		SetMoveType( Move_DontMoveWP );
		_unit->SetFacing( 4.708117f );
		break;
	case 8:
		Despawn();
		break;
	};
};

void FelSoldierAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		AIMap::iterator Iter = mTriggerAI->mQueuedDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( Iter != mTriggerAI->mQueuedDemonSet.end() )
			mTriggerAI->mQueuedDemonSet.erase( Iter );

		Iter = mTriggerAI->mFightingDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( Iter != mTriggerAI->mFightingDemonSet.end() )
			mTriggerAI->mFightingDemonSet.erase( Iter );

		mTriggerAI = NULL;
	};

	delete this;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////// Wrath Master AI

WrathMasterAI::WrathMasterAI( CreaturePointer pCreature ) : MoonScriptCreatureAI( pCreature )
{
	SetWieldWeapon( true );

	mTriggerAI = NULL;
	mLastWP = 0;
};

void WrathMasterAI::OnCombatStop( UnitPointer pTarget )
{
	ParentClass::OnCombatStop( pTarget );
	MoveTo( _unit->GetPositionX(), _unit->GetPositionY() + 3.0f, _unit->GetPositionZ() );
	if ( HasWaypoints() )
	{
		size_t WPCount = GetWaypointCount();
		if ( WPCount == 0 )
			return;

		SetMoveType( Move_ForwardThenStop );
		if ( mLastWP <= WPCount - 1 )
			SetWaypointToMove( mLastWP );
	};
};

void WrathMasterAI::OnReachWP( uint32 pWaypointId, bool pForwards )
{
	mLastWP = pWaypointId;
	switch ( pWaypointId )
	{
	case 3:
		SetMoveType( Move_DontMoveWP );
		_unit->SetFacing( 4.708117f );
		break;
	case 8:
		Despawn();
		break;
	};
};

void WrathMasterAI::Destroy()
{
	if ( mTriggerAI != NULL )
	{
		AIMap::iterator Iter = mTriggerAI->mQueuedDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( Iter != mTriggerAI->mQueuedDemonSet.end() )
			mTriggerAI->mQueuedDemonSet.erase( Iter );

		Iter = mTriggerAI->mFightingDemonSet.find( static_cast< MoonScriptCreatureAI* >( this ) );
		if ( Iter != mTriggerAI->mFightingDemonSet.end() )
			mTriggerAI->mFightingDemonSet.erase( Iter );

		mTriggerAI = NULL;
	};

	delete this;
};

void SetupEventDarkPortal( ScriptMgr* pScriptMgr )
{
	/* Instance Script */
	pScriptMgr->register_instance_script( MAP_OUTLAND, &OutlandScript::Create );

	/* Event Trigger */
	pScriptMgr->register_creature_script( CN_DARK_PORTAL_EVENT_TRIGGER, &DarkPortalEventTriggerAI::Create );

	/* Horde Units */
	pScriptMgr->register_creature_script( CN_MELGROMM_HIGHMOUNTAIN, &MelGrommHighmountainAI::Create );
	pScriptMgr->register_creature_script( CN_ORGRIMMAR_GRUNT, &OrgrimmarGruntAI::Create );
	pScriptMgr->register_creature_script( CN_ORGRIMMAR_SHAMAN, &OrgrimmarShamanAI::Create );
	pScriptMgr->register_creature_script( CN_DARKSPEAR_AXE_THROWER, &DarkspearAxeThrowerAI::Create );
	pScriptMgr->register_creature_script( CN_UNDERCITY_MAGE, &UndercityMageAI::Create );

	/* Alliance Units */
	pScriptMgr->register_creature_script( CN_JUSTINIUS_THE_HARBINGER, &JustiniusTheHarbingerAI::Create );
	pScriptMgr->register_creature_script( CN_STORMWIND_SOLDIER, &StormwindSoldierAI::Create );
	pScriptMgr->register_creature_script( CN_IRONFORGE_PALADIN, &IronforgePaladinAI::Create );
	pScriptMgr->register_creature_script( CN_DARNASSIAN_ARCHER, &DarnassianArcherAI::Create );
	pScriptMgr->register_creature_script( CN_STORMWIND_MAGE, &StormwindMageAI::Create );

	/* Demon Units */
	pScriptMgr->register_creature_script( CN_PIT_COMMANDER, &PitCommanderAI::Create );
	pScriptMgr->register_creature_script( CN_INFERNAL_SIEGEBREAKER, &InfernalSiegebreakerAI::Create );
	pScriptMgr->register_creature_script( CN_FEL_SOLDIER, &FelSoldierAI::Create );
	pScriptMgr->register_creature_script( CN_WRATH_MASTER, &WrathMasterAI::Create );
};

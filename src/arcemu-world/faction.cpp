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

#include "StdAfx.h"

#ifdef WIN32
#define HACKY_CRASH_FIXES 1		// SEH stuff
#endif

int isBgEnemy(Object* objA, Object* objB)
{
	// if objA is in battleground check objB hostile based on teams
	if( ( objA->IsInBg() && (objA->IsPlayer() || objA->IsPet() || ( objA->IsUnit() && !objA->IsPlayer() && static_cast< Creature* >( objA )->IsTotem() ) ) ) )
	{
		if ( (objB->IsPlayer() || objB->IsPet() || ( objB->IsUnit() && !objB->IsPlayer() && static_cast< Creature* >( objB )->IsTotem() ) ) )
		{
			if ( objB->IsInBg() == false )
			{
				// objA is in battleground, objB no, so return false
				return 0;
			}
			else
			{
				uint32 teamA = objA->GetTeam();
				uint32 teamB = objB->GetTeam();
				if (teamA == (uint32)-1 || teamB == (uint32)-1)
				{
					return 0;
				}

				return teamA != teamB;
			}
		}
	}

	return -1;
}

bool isHostile(Object* objA, Object* objB)// B is hostile for A?
{
	if(!objA || !objB)
		return false;
	bool hostile = false;

  if ( !objA->IsInWorld() || !objB->IsInWorld() )  // pending or ...?
    return false;

	if(objA == objB)
		return false;   // can't attack self.. this causes problems with buffs if we dont have it :p

	if(objA->GetTypeId() == TYPEID_CORPSE)
		return false;

	if(objB->GetTypeId() == TYPEID_CORPSE)
		return false;

	int ret = isBgEnemy(objA, objB);
	if (ret != -1) return ret == 1;

	uint32 faction = objB->m_faction->Mask;
	uint32 host = objA->m_faction->HostileMask;

	/*if( faction != 0 )
	{*/
		if(faction & host)
		{
			hostile = true;
		}
	/*}
	else
	{
		// default to true
		hostile = true;
	}*/

	// anway,this place won't fight
	AreaTable *atA = NULL;
	AreaTable *atB = NULL;

	if( objA->IsPlayer() )
		atA = dbcArea.LookupEntry( static_cast< Player* >( objA )->GetAreaID() );

	if( objB->IsPlayer() )
		atB = dbcArea.LookupEntry( static_cast< Player* >( objB )->GetAreaID() );

	if( ( atA && atA->AreaFlags & 0x800) || (atB && atB->AreaFlags & 0x800) ) // cebernic: fix older logic error
		return false;

		
	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == objB->m_faction->Faction)
		{
			hostile = true;
			break;
		}
		if(objA->m_faction->FriendlyFactions[i] == objB->m_faction->Faction)
		{
			hostile = false;
			break;
		}
	}

	// PvP Flag System Checks
	// We check this after the normal isHostile test, that way if we're
	// on the opposite team we'll already know :p

	if( hostile && ( objA->IsPlayer() || objA->IsPet() || ( objA->IsUnit() && !objA->IsPlayer() && static_cast< Creature* >( objA )->IsTotem() && static_cast< Creature* >( objA )->GetTotemOwner()->IsPvPFlagged() ) ) )
	{
		if( objB->IsPlayer() )
		{
			// Check PvP Flags.
			if( static_cast< Player* >( objB )->IsPvPFlagged() )
				return true;
			else
				return false;
		}
		if( objB->IsPet() )
		{
#if defined(WIN32) && defined(HACKY_CRASH_FIXES)
			__try {
				// Check PvP Flags.
				if( static_cast< Pet* >( objB )->GetPetOwner() != NULL && static_cast< Pet* >( objB )->GetPetOwner()->GetMapMgr() == objB->GetMapMgr() && static_cast< Pet* >( objB )->GetPetOwner()->IsPvPFlagged() )
					return true;
				else
					return false;
			} __except(EXCEPTION_EXECUTE_HANDLER)
			{
				static_cast<Pet*>(objB)->ClearPetOwner();
				static_cast<Pet*>(objB)->SafeDelete();
			}
#else
			// Check PvP Flags.
			if( static_cast< Pet* >( objB )->GetPetOwner() != NULL && static_cast< Pet* >( objB )->GetPetOwner()->GetMapMgr() == objB->GetMapMgr() && static_cast< Pet* >( objB )->GetPetOwner()->IsPvPFlagged() )
				return true;
			else
				return false;
#endif
		}
	}

	// Reputation System Checks
	if(objA->IsPlayer() && !objB->IsPlayer())	   // PvE
	{
		if(objB->m_factionDBC->RepListId >= 0)
			hostile = static_cast< Player* >( objA )->IsHostileBasedOnReputation( objB->m_factionDBC );
	}
	
	if(objB->IsPlayer() && !objA->IsPlayer())	   // PvE
	{
		if(objA->m_factionDBC->RepListId >= 0)
			hostile = static_cast< Player* >( objB )->IsHostileBasedOnReputation( objA->m_factionDBC );
	}

	if( objA->IsPlayer() && objB->IsPlayer() && static_cast<Player*>(objA)->m_bg != NULL )
	{
		if( static_cast<Player*>(objA)->m_bgTeam != static_cast<Player*>(objB)->m_bgTeam )
			return true;
	}

	return hostile;
}

/// Where we check if we object A can attack object B. This is used in many feature's
/// Including the spell class and the player class.
bool isAttackable(Object* objA, Object* objB, bool CheckStealth)// A can attack B?
{
	if(!objA || !objB )
		return false;

  if ( !objA->IsInWorld() || !objB->IsInWorld() )  // pending or ...?
    return false;

	if(objA == objB)
		return false;   // can't attack self.. this causes problems with buffs if we don't have it :p

	if(objA->GetTypeId() == TYPEID_CORPSE)
		return false;

	if(objB->GetTypeId() == TYPEID_CORPSE)
		return false;

	// Players in feign death flags can't be attacked (where did you get this information from?)
	// Changed by Supa: Creatures cannot attack players with feign death flags.
	/*if(!objA->IsPlayer())
		if(objA->HasFlag(UNIT_FIELD_FLAGS_2, 0x00000001))
			return false;
	if(!objB->IsPlayer())
		if(objB->HasFlag(UNIT_FIELD_FLAGS_2, 0x00000001))
			return false;*/

	// Checks for untouchable, unattackable
	if(objA->IsUnit() && objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DEAD))
		return false;
	if(objB->IsUnit())
	{
		if(objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DEAD))
			return false;

		/// added by Zack : 
        /// we cannot attack shealthed units. Maybe checked in other places too ?
		/// !! warning, this presumes that objA is attacking ObjB
        /// Capt: Added the possibility to disregard this (regarding the spell class)
		if(static_cast<Unit *>(objB)->IsStealth() && CheckStealth)
			return false;
	}

	int ret = isBgEnemy(objA, objB);
	if (ret != -1) return ret == 1;

	if(objA->IsPlayer() && objB->IsPlayer())
	{
		if(
			static_cast< Player* >( objA )->DuelingWith == static_cast< Player* >(objB) && 
			static_cast< Player* >( objA )->GetDuelState() == DUEL_STATE_STARTED
			)
		return true;

		//players in same group should not attack each other. Required for arenas with mixed groups
		if( static_cast< Player* >( objA )->GetGroup() && static_cast< Player* >( objA )->GetGroup() == static_cast< Player* >( objB )->GetGroup() )
			return false;

		if(objA->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP) && objB->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP))
		{
			if(static_cast< Player* >( objA )->m_bg != NULL && static_cast< Player* >( objB )->m_bg != NULL)
				if(static_cast< Player* >( objA )->m_bgTeam == static_cast< Player* >( objB )->m_bgTeam)
					return false;

			if( static_cast< Player* >( objA )->m_bg != NULL )
				if( static_cast< Player* >( objA )->GetGroup() == static_cast< Player* >( objB )->GetGroup() )
					return false;

			return true;		// can hurt each other in FFA pvp
		}
	}
	
	// handle for pets in duel
	if(objA->IsPet())
	{
		if(objB->IsPlayer())
			if(
				static_cast<Pet *>(objA)->GetPetOwner() )
			{
				if(	static_cast<Pet *>(objA)->GetPetOwner()->DuelingWith == static_cast< Player* >(objB) && 
					static_cast<Pet *>(objA)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED )
					return true;
				if(	static_cast<Pet *>(objA)->GetPetOwner()->m_bg &&
					static_cast<Pet *>(objA)->GetPetOwner()->m_bgTeam != static_cast< Player* >(objB)->m_bgTeam	)
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( static_cast<Player *>(objB)->GetGroup() && 
					static_cast<Pet *>(objA)->GetGroup() == static_cast< Player* >( objB )->GetGroup() )
					return false;
			}
		else if(objB->IsPet())
			if(static_cast<Pet *>(objA)->GetPetOwner() && static_cast<Pet *>(objB)->GetPetOwner())
			{
				if(	static_cast<Pet *>(objA)->GetPetOwner()->DuelingWith == static_cast<Pet *>(objB)->GetPetOwner() && 
				static_cast<Pet *>(objA)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED
				)
					return true;
				if(	static_cast<Pet *>(objA)->GetPetOwner()->m_bg &&
					static_cast<Pet *>(objA)->GetPetOwner()->m_bgTeam != static_cast<Pet *>(objB)->GetPetOwner()->m_bgTeam )
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( static_cast<Pet *>(objA)->GetGroup() && 
					static_cast<Pet *>(objA)->GetGroup() == static_cast<Pet *>(objB)->GetGroup() )
					return false;
			}
	}
	if(objB->IsPet())
	{
		if(objA->IsPlayer())
			if( static_cast<Pet*>(objB)->GetPetOwner() )
			{
				if(	static_cast<Pet *>(objB)->GetPetOwner()->DuelingWith == static_cast< Player* >(objA) && 
					static_cast<Pet *>(objB)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED )
				return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( static_cast<Player *>(objA)->GetGroup() && 
					static_cast<Pet *>(objB)->GetGroup() == static_cast< Player* >( objA )->GetGroup() )
					return false;
			}
		//we already made this check a few lines before : if A -> B
/*		else if(objA->IsPet())
			if(static_cast<Pet*>(objA)->GetPetOwner() && static_cast<Pet *>(objB)->GetPetOwner() &&
				static_cast<Pet*>(objB)->GetPetOwner() &&
				static_cast<Pet *>(objB)->GetPetOwner()->DuelingWith == static_cast<Pet *>(objA)->GetPetOwner() && 
				static_cast<Pet *>(objB)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED
				)
				return true;*/
	}

	// handle for totems
	if( objA->IsCreature() && static_cast<Creature *>(objA)->IsTotem() ) 
	{
		if(objB->IsPlayer())
		{
			if( static_cast<Creature *>(objA)->GetTotemOwner() &&
				static_cast<Creature *>(objA)->GetTotemOwner()->DuelingWith == static_cast< Player* >(objB) && 
				static_cast<Creature *>(objA)->GetTotemOwner()->GetDuelState() == DUEL_STATE_STARTED
				)
				return true;
			if( static_cast<Creature *>(objA)->GetTotemOwner()->m_bg &&
				static_cast<Creature *>(objA)->GetTotemOwner()->m_bgTeam != static_cast< Player* >(objB)->m_bgTeam )
				return true;
			//players in same group should not attack each other. Required for arenas with mixed groups
			if( static_cast<Player *>(objB)->GetGroup() && 
				static_cast<Creature *>(objA)->GetGroup() == static_cast< Player* >( objB )->GetGroup() )
				return false;
		}
		if( static_cast<Creature *>(objA)->GetTotemOwner() )
		{
			if( objB->IsPet() )
			{
				if(	static_cast<Creature *>(objA)->GetTotemOwner()->DuelingWith == static_cast<Pet *>(objB)->GetPetOwner() && 
					static_cast<Creature *>(objA)->GetTotemOwner()->GetDuelState() == DUEL_STATE_STARTED
					)
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( static_cast<Pet *>(objB)->GetGroup() && 
					static_cast<Creature *>(objA)->GetGroup() == static_cast<Pet *>(objB)->GetGroup() )
					return false;
			}
			else if( objB->IsCreature() && static_cast<Creature *>(objB)->IsTotem() )
			{
				if(	static_cast<Creature *>(objA)->GetTotemOwner()->DuelingWith == static_cast<Creature *>(objB)->GetTotemOwner() && 
					static_cast<Creature *>(objA)->GetTotemOwner()->GetDuelState() == DUEL_STATE_STARTED
					)
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( static_cast<Creature *>(objA)->GetGroup() && 
					static_cast<Creature *>(objA)->GetGroup() == static_cast<Creature *>(objB)->GetGroup() )
					return false;
			}
		}
		
	}
	if( objB->IsCreature() && static_cast<Creature *>(objB)->IsTotem() ) // must be creature
	{
		if(objA->IsPlayer() && static_cast<Creature *>(objB)->GetTotemOwner())
		{
			if( static_cast<Creature *>(objB)->GetTotemOwner()->DuelingWith == static_cast< Player* >(objA) && 
				static_cast<Creature *>(objB)->GetTotemOwner()->GetDuelState() == DUEL_STATE_STARTED )
				return true;
			//players in same group should not attack each other. Required for arenas with mixed groups
			if( static_cast<Player *>(objA)->GetGroup() && 
				static_cast<Creature *>(objB)->GetGroup() == static_cast< Player* >( objA )->GetGroup() )
				return false;
		}
		if(objA->IsPet() && static_cast<Creature *>(objB)->GetTotemOwner() )
		{
			if( static_cast<Creature *>(objB)->GetTotemOwner()->DuelingWith == static_cast<Pet *>(objA)->GetPetOwner() && 
			static_cast<Creature *>(objB)->GetTotemOwner()->GetDuelState() == DUEL_STATE_STARTED
			)
				return true;
			//players in same group should not attack each other. Required for arenas with mixed groups
			if( static_cast<Creature *>(objB)->GetGroup() && 
				static_cast<Creature *>(objB)->GetGroup() == static_cast< Pet* >( objA )->GetGroup() )
				return false;
		}
	}

	// do not let people attack each other in sanctuary
	// Dueling is already catered for
	AreaTable *atA = NULL;
	AreaTable *atB = NULL;

  // cebernic: don't forget totem

	if ( objA->IsCreature() )
	{
		if( static_cast<Creature *>(objA)->IsTotem() && static_cast< Creature* >( objA )->GetTotemOwner() )
			atA = dbcArea.LookupEntry( static_cast< Creature* >( objA )->GetTotemOwner()->GetAreaID() );
		else
		if( objA->IsPet() && static_cast< Pet* >( objA )->GetPetOwner() )
			atA = dbcArea.LookupEntry( static_cast< Pet* >( objA )->GetPetOwner()->GetAreaID() );
	}
	
	if ( objB->IsCreature() )
	{
		if( static_cast<Creature *>(objB)->IsTotem() && static_cast< Creature* >( objB )->GetTotemOwner() )
			atB = dbcArea.LookupEntry( static_cast< Creature* >( objB )->GetTotemOwner()->GetAreaID() );
		else
		if( objB->IsPet() && static_cast< Pet* >( objB )->GetPetOwner() )
			atB = dbcArea.LookupEntry( static_cast< Pet* >( objB )->GetPetOwner()->GetAreaID() );
/*		if ( atB==NULL ) {
			Unit *_creator = objB->GetMapMgr()->GetUnit( objB->GetUInt64Value( UNIT_FIELD_CREATEDBY ) );
			if( _creator!=NULL && _creator->IsCreature() && _creator->GetMapMgr() ){
//				printf("%s\n",__ansi(static_cast< Creature* >( _creator )->GetCreatureInfo()->Name ));
				atB = dbcArea.LookupEntry( _creator->GetMapMgr()->GetAreaID(_creator->GetPositionX(),_creator->GetPositionY())  );	
			}*/
//		}
	}			
		
	if( objA->IsPlayer() )
		atA = dbcArea.LookupEntry( static_cast< Player* >( objA )->GetAreaID() );

	if( objB->IsPlayer() )
		atB = dbcArea.LookupEntry( static_cast< Player* >( objB )->GetAreaID() );

	// We have the area codes
	// We know they aren't dueling
	if( ( atA && atA->AreaFlags & 0x800) || (atB && atB->AreaFlags & 0x800) ) // cebernic: fix older logic error
		return false;

	if(objA->m_faction == objB->m_faction)  // same faction can't kill each other unless in ffa pvp/duel
		return false;

	bool attackable = isHostile(objA, objB); // B is attackable if its hostile for A
	/*if((objA->m_faction->HostileMask & 8) && (objB->m_factionDBC->RepListId != 0) && 
		(objB->GetTypeId() != TYPEID_PLAYER) && objB->m_faction->Faction != 31) // B is attackable if its a neutral Creature*/

	// Neutral Creature Check
	if(objA->IsPlayer() || objA->IsPet())
	{
		if(objB->m_factionDBC->RepListId == -1 && objB->m_faction->HostileMask == 0 && objB->m_faction->FriendlyMask == 0)
		{
			attackable = true;
		}
	}
	else if(objB->IsPlayer() || objB->IsPet())
	{
		if(objA->m_factionDBC->RepListId == -1 && objA->m_faction->HostileMask == 0 && objA->m_faction->FriendlyMask == 0)
		{
			attackable = true;
		}
	}

	return attackable;
}

bool isCombatSupport(Object* objA, Object* objB)// B combat supports A?
{
	if(!objA || !objB)
		return false;

	if(objA->GetTypeId() == TYPEID_CORPSE )
		return false;

	if(objB->GetTypeId() == TYPEID_CORPSE)
		return false;

  if ( objA->GetTypeId()!=TYPEID_UNIT || objB->GetTypeId()!=TYPEID_UNIT ) return false; // cebernic: lowchance crashfix.
  	
	if( objA->IsPet() || objB->IsPet() ) // fixes an issue where horde pets would chain aggro horde guards and vice versa for alliance.
		return false;

	bool combatSupport = false;

	uint32 fSupport = objB->m_faction->FriendlyMask;
	uint32 myFaction = objA->m_faction->Mask;

	if(myFaction & fSupport)
	{
		combatSupport = true;
	}
	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objB->m_faction->EnemyFactions[i] == objA->m_faction->Faction)
		{
			combatSupport = false;
			break;
		}
		if(objB->m_faction->FriendlyFactions[i] == objA->m_faction->Faction)
		{
			combatSupport = true;
			break;
		}
	}

#ifdef _TEST_EXTENDED_FEATURES_
  if ( !combatSupport && objA->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE) == objB->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE) && ((Creature*)objA)->GetCreatureInfo()->Rank >= 1 )
		combatSupport = true;

	if ( objB->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE)==1080 || objA->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE)==1080 )
		combatSupport = false;
#endif


	return combatSupport;
}


bool isAlliance(Object* objA)// A is alliance?
{
	FactionTemplateDBC * m_sw_faction = dbcFactionTemplate.LookupEntry(11);
	FactionDBC * m_sw_factionDBC = dbcFaction.LookupEntry(72);
	if(!objA 
//		|| objA->m_factionDBC == NULL || objA->m_faction == NULL
		)
		return true;

	if(m_sw_faction == objA->m_faction || m_sw_factionDBC == objA->m_factionDBC)
		return true;

	//bool hostile = false;
	uint32 faction = m_sw_faction->Faction;
	uint32 host = objA->m_faction->HostileMask;

	if(faction & host)
		return false;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == faction)
			return false;
	}

	faction = objA->m_faction->Faction;
	host = m_sw_faction->HostileMask;

	if(faction & host)
		return false;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == faction)
			return false;
	}

	return true;
}








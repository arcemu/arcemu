/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

Player* GetPlayerOwner( Object *A ){

    Player *pAttacker = NULL;

    if( A->IsPlayer() ){
        pAttacker = TO< Player* >( A );
	}
    else
	if( A->IsPet() ){
        pAttacker = TO< Pet* >( A )->GetPetOwner();

        // Pet must have an owner if it's not being deleted
		Arcemu::Util::ARCEMU_ASSERT(    TO< Pet* >( A )->IsBeingDeleted() || pAttacker != NULL );                
	}
    else // Player totem
    if( A->IsCreature() && TO< Creature* >( A )->IsTotem() ){
		pAttacker = TO< Player* >( TO< Creature* >( A )->GetOwner() );

        // Totem must have an owner
        Arcemu::Util::ARCEMU_ASSERT(    pAttacker != NULL );

    }
    else // Player summon
    if( A->IsCreature() && TO< Creature* >( A )->GetOwner() != NULL && TO< Creature* >( A )->GetOwner()->IsPlayer() ){
        pAttacker = TO< Player* >( TO< Creature* >( A )->GetOwner() );
    }

    return pAttacker;
}

int isBgEnemy(Object* objA, Object* objB)
{
	// if objA is in battleground check objB hostile based on teams
	if( ( objA->IsInBg() && (objA->IsPlayer() || objA->IsPet() || ( objA->IsUnit() && !objA->IsPlayer() && TO< Creature* >( objA )->IsTotem() ) ) ) )
	{
		if ( (objB->IsPlayer() || objB->IsPet() || ( objB->IsUnit() && !objB->IsPlayer() && TO< Creature* >( objB )->IsTotem() ) ) )
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

bool isNeutral( Object *a,  Object *b ){
	if( ( a->m_faction->HostileMask & b->m_faction->Mask ) == 0 &&
		( a->m_faction->FriendlyMask & b->m_faction->Mask ) == 0 )
		return true;

	return false;
}

bool isHostile(Object* objA, Object* objB)// B is hostile for A?
{
	if(!objA || !objB)
		return false;
	bool hostile = false;

  if ( !objA->IsInWorld() || !objB->IsInWorld() )  // pending or ...?
    return false;

	if(objA == objB)
		return false;   // can't attack self.. this causes problems with buffs if we don't have it :p

	if(objA->IsCorpse())
		return false;

	if(objB->IsCorpse())
		return false;

	if( !(objA->m_phase & objB->m_phase) ) //What you can't see, can't be hostile!
		return false;

	if( objA->IsPlayer() && objA->HasFlag( PLAYER_FLAGS, 0x100) && objB->IsCreature() && TO< Unit* >(objB)->GetAIInterface()->m_isNeutralGuard )
		return true;
	if( objB->IsPlayer() && objB->HasFlag( PLAYER_FLAGS, 0x100) && objA->IsCreature() && TO< Unit* >(objA)->GetAIInterface()->m_isNeutralGuard )
		return true;

    if( objB->IsUnit() && objB->HasFlag( UNIT_FIELD_FLAGS, 2 | 128 | 256 | 65536 ) )
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

		// anyway, this place won't fight
	AreaTable *atA = NULL;
	AreaTable *atB = NULL;

	if( objA->IsPlayer() )
		atA = dbcArea.LookupEntry( TO< Player* >( objA )->GetAreaID() );

	if( objB->IsPlayer() )
		atB = dbcArea.LookupEntry( TO< Player* >( objB )->GetAreaID() );

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
    

//////////////////////////////////////////// PvP checks /////////////////////////////////
    {
        Player *A = GetPlayerOwner( objA );
        Player *B = GetPlayerOwner( objB );

        if( hostile && A != NULL && B != NULL ){
			if( !B->IsSanctuaryFlagged() && ( B->IsPvPFlagged() || B->IsFFAPvPFlagged() ) )
                return true;
            else
                return false;
        }

    }
/////////////////////////////////////////////////////////////////////////////////////////

	// Reputation System Checks
	if(objA->IsPlayer() && !objB->IsPlayer())	   // PvE
	{
		if(objB->m_factionDBC->RepListId >= 0)
			hostile = TO< Player* >( objA )->IsHostileBasedOnReputation( objB->m_factionDBC );
	}
	
	if(objB->IsPlayer() && !objA->IsPlayer())	   // PvE
	{
		if(objA->m_factionDBC->RepListId >= 0)
			hostile = TO< Player* >( objB )->IsHostileBasedOnReputation( objA->m_factionDBC );
	}

	if( objA->IsPlayer() && objB->IsPlayer() && TO< Player* >(objA)->m_bg != NULL )
	{
		if( TO< Player* >(objA)->m_bgTeam != TO< Player* >(objB)->m_bgTeam )
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

	if( !(objA->m_phase & objB->m_phase) ) //What you can't see, you can't attack either...
		return false;

	if(objA->IsCorpse())
		return false;

	if(objB->IsCorpse())
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
		if(TO< Unit* >(objB)->IsStealth() && CheckStealth)
			return false;
	}

	int ret = isBgEnemy(objA, objB);
	if (ret != -1) return ret == 1;

	if(objA->IsPlayer() && objB->IsPlayer())
	{
		if(
			TO< Player* >( objA )->DuelingWith == TO< Player* >(objB) && 
			TO< Player* >( objA )->GetDuelState() == DUEL_STATE_STARTED
			)
		return true;

		//players in same group should not attack each other. Required for arenas with mixed groups
		if( TO< Player* >( objA )->GetGroup() && TO< Player* >( objA )->GetGroup() == TO< Player* >( objB )->GetGroup() )
			return false;

		if(objA->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP) && objB->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP))
		{
			if(TO< Player* >( objA )->m_bg != NULL && TO< Player* >( objB )->m_bg != NULL)
				if(TO< Player* >( objA )->m_bgTeam == TO< Player* >( objB )->m_bgTeam)
					return false;

			if( TO< Player* >( objA )->m_bg != NULL )
				if( TO< Player* >( objA )->GetGroup() == TO< Player* >( objB )->GetGroup() )
					return false;

			return true;		// can hurt each other in FFA pvp
		}
	}
	
	// handle for pets in duel
	if(objA->IsPet())
	{
		if(objB->IsPlayer())
		{
			if(
				TO< Pet* >(objA)->GetPetOwner() )
			{
				if(	TO< Pet* >(objA)->GetPetOwner()->DuelingWith == TO< Player* >(objB) && 
					TO< Pet* >(objA)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED )
					return true;
				if(	TO< Pet* >(objA)->GetPetOwner()->m_bg &&
					TO< Pet* >(objA)->GetPetOwner()->m_bgTeam != TO< Player* >(objB)->m_bgTeam	)
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( TO< Player* >(objB)->GetGroup() && 
					TO< Pet* >(objA)->GetGroup() == TO< Player* >( objB )->GetGroup() )
					return false;
			}
		}
		else if(objB->IsPet())
		{
			if(TO< Pet* >(objA)->GetPetOwner() && TO< Pet* >(objB)->GetPetOwner())
			{
				if(	TO< Pet* >(objA)->GetPetOwner()->DuelingWith == TO< Pet* >(objB)->GetPetOwner() && 
				TO< Pet* >(objA)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED
				)
					return true;
				if(	TO< Pet* >(objA)->GetPetOwner()->m_bg &&
					TO< Pet* >(objA)->GetPetOwner()->m_bgTeam != TO< Pet* >(objB)->GetPetOwner()->m_bgTeam )
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( TO< Pet* >(objA)->GetGroup() && 
					TO< Pet* >(objA)->GetGroup() == TO< Pet* >(objB)->GetGroup() )
					return false;
			}
		}
	}
	if(objB->IsPet())
	{
		if(objA->IsPlayer())
		{
			if( TO< Pet* >(objB)->GetPetOwner() )
			{
				if(	TO< Pet* >(objB)->GetPetOwner()->DuelingWith == TO< Player* >(objA) && 
					TO< Pet* >(objB)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED )
				return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( TO< Player* >(objA)->GetGroup() && 
					TO< Pet* >(objB)->GetGroup() == TO< Player* >( objA )->GetGroup() )
					return false;
			}
		}
		//we already made this check a few lines before : if A -> B
/*		else if(objA->IsPet())
		{
			if(TO< Pet* >(objA)->GetPetOwner() && TO< Pet* >(objB)->GetPetOwner() &&
				TO< Pet* >(objB)->GetPetOwner() &&
				TO< Pet* >(objB)->GetPetOwner()->DuelingWith == TO< Pet* >(objA)->GetPetOwner() && 
				TO< Pet* >(objB)->GetPetOwner()->GetDuelState() == DUEL_STATE_STARTED
				)
				return true;
		}*/
	}

	// handle for totems
	if( objA->IsCreature() && TO< Creature* >(objA)->IsTotem() ) 
	{
		if(objB->IsPlayer())
		{

			Player *pb = TO< Player* >(objB);
			Player *pa = TO< Player* >( TO< Creature* >(objA)->GetOwner() );

			if( pa &&
				pa->DuelingWith  == pb && 
				pa->GetDuelState() == DUEL_STATE_STARTED
				)
				return true;
			
			if( pa->m_bg  &&
				pa->m_bgTeam != pb->m_bgTeam )
				return true;
			//players in same group should not attack each other. Required for arenas with mixed groups
			if( pb->GetGroup() && 
				TO< Creature* >(objA)->GetGroup() == pb->GetGroup() )
				return false;
		}
		if( TO< Creature* >(objA)->GetOwner() )
		{
			if( objB->IsPet() )
			{

				Player *pb = TO< Pet* >(objB)->GetPetOwner();
				Player *pa = TO< Player* >( TO< Creature* >(objA)->GetOwner());

				if(	pa->DuelingWith == pb && pa->GetDuelState() == DUEL_STATE_STARTED )
					return true;
				
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( TO< Pet* >(objB)->GetGroup() && 
					TO< Creature* >(objA)->GetGroup() == TO< Pet* >(objB)->GetGroup() )
					return false;
			}
			else if( objB->IsCreature() && TO< Creature* >(objB)->IsTotem() )
			{
				if(	TO< Player* >( TO< Creature* >(objA)->GetOwner())->DuelingWith == TO< Creature* >(objB)->GetOwner() && 
					TO< Player* >( TO< Creature* >(objA)->GetOwner())->GetDuelState() == DUEL_STATE_STARTED
					)
					return true;
				//players in same group should not attack each other. Required for arenas with mixed groups
				if( TO< Creature* >(objA)->GetGroup() && 
					TO< Creature* >(objA)->GetGroup() == TO< Creature* >(objB)->GetGroup() )
					return false;
			}
		}
		
	}
	if( objB->IsCreature() && TO< Creature* >(objB)->IsTotem() ) // must be creature
	{
		if(objA->IsPlayer() && TO< Creature* >(objB)->GetOwner())
		{
			if( TO< Player* >( TO< Creature* >(objB)->GetOwner() )->DuelingWith == TO< Player* >(objA) && 
				TO< Player* >( TO< Creature* >(objB)->GetOwner() )->GetDuelState() == DUEL_STATE_STARTED )
				return true;
			//players in same group should not attack each other. Required for arenas with mixed groups
			if( TO< Player* >(objA)->GetGroup() && 
				TO< Creature* >(objB)->GetGroup() == TO< Player* >( objA )->GetGroup() )
				return false;
		}
		if(objA->IsPet() && TO< Creature* >(objB)->GetOwner() )
		{

			Player *pa = TO< Pet* >( objA )->GetPetOwner();
			Player *pb = TO< Player* >( TO< Creature* >( objB )->GetOwner() );

			if( pb->DuelingWith == pa && pb->GetDuelState() == DUEL_STATE_STARTED )
				return true;

			//players in same group should not attack each other. Required for arenas with mixed groups
			if( TO< Creature* >(objB)->GetGroup() && 
				TO< Creature* >(objB)->GetGroup() == TO< Pet* >( objA )->GetGroup() )
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
		if( TO< Creature* >(objA)->IsTotem() && TO< Creature* >( objA )->GetOwner() )
			atA = dbcArea.LookupEntry( TO< Player* >( TO< Creature* >( objA )->GetOwner() )->GetAreaID() );
		else
		if( objA->IsPet() && TO< Pet* >( objA )->GetPetOwner() )
			atA = dbcArea.LookupEntry( TO< Pet* >( objA )->GetPetOwner()->GetAreaID() );
	}
	
	if ( objB->IsCreature() )
	{
		if( TO< Creature* >(objB)->IsTotem() && TO< Creature* >( objB )->GetOwner() )
			atB = dbcArea.LookupEntry( TO< Player* >( TO< Creature* >( objB )->GetOwner())->GetAreaID() );
		else if( objB->IsPet() && TO< Pet* >( objB )->GetPetOwner() )
			atB = dbcArea.LookupEntry( TO< Pet* >( objB )->GetPetOwner()->GetAreaID() );

	}			
		
	if( objA->IsPlayer() )
		atA = dbcArea.LookupEntry( TO< Player* >( objA )->GetAreaID() );

	if( objB->IsPlayer() )
		atB = dbcArea.LookupEntry( TO< Player* >( objB )->GetAreaID() );

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

	if(objA->IsCorpse() )
		return false;

	if(objB->IsCorpse())
		return false;

	if ( !objA->IsCreature() || !objB->IsCreature() ) return false; // cebernic: lowchance crashfix.
	// also if it's not a unit, it shouldn't support combat anyways.

	if( objA->IsPet() || objB->IsPet() ) // fixes an issue where horde pets would chain aggro horde guards and vice versa for alliance.
		return false;

	if( !(objA->m_phase & objB->m_phase) ) //What you can't see, you can't support either...
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


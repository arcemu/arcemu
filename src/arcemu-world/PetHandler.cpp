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

void WorldSession::HandlePetAction(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	//WorldPacket data;
	uint64 petGuid = 0;
	uint16 misc = 0;
	uint16 action = 0;

	uint64 targetguid = 0;
	recv_data >> petGuid >> misc >> action;
	//recv_data.hexlike();

	//Gona treat possed creatures and pets the same way.
	Creature * pet = NULL;
	if( GET_TYPE_FROM_GUID(petGuid) == HIGHGUID_TYPE_UNIT)
		pet = GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(petGuid));
	else
	{
		for(std::list<Pet*>::iterator itr = _player->getSummonStart(); itr != _player->getSummonEnd(); )
		{
			if( (*itr)->GetGUID() == petGuid)
			{
				if( !(*itr)->isAlive() )
					(*itr)->SendActionFeedback(PET_FEEDBACK_PET_DEAD);
				else
					pet = (*itr);
				break;
			}
		}
	}
	//Once we have a valid creature, they both have pet ai
	if(pet != NULL)
	{
		PetAI * ai = TO_AIPET(pet->GetAIInterface());
		recv_data >> targetguid;
		Unit * target = GetPlayer()->GetMapMgr()->GetUnit(targetguid);
		if(action == PET_ACTION_ACTION)
			ai->Pet_setaction(misc, target);
		else if(action == PET_ACTION_SPELL || action == PET_ACTION_SPELL_1 || action == PET_ACTION_SPELL_2 )
		{
			AI_Spell * spell = ai->Spell_findbyId( uint32(misc) );
			if(spell != NULL)
			{
				ai->Spell_setnext(spell);
				ai->setNextTarget(target);
				//if we are casting on an enemy, then we are attacking.
				if( isAttackable(pet, target) )
					ai->Pet_setaction(PET_ACTION_ATTACK, target);
			}
		}
		else if(action == PET_ACTION_STATE)
			ai->Pet_setstate( uint32(misc) );
		else
			printf("WARNING: Unknown pet action received. Action = %.4X, Misc = %.4X\n", action, misc);

		/* Send pet action sound - WHEE THEY TALK */
		WorldPacket actionp(SMSG_PET_ACTION_SOUND, 12);
		actionp << pet->GetGUID() << uint32(1);//should we send only 1 sound for all the pets?
		SendPacket(&actionp);
	}
}

void WorldSession::HandlePetInfo(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	//nothing
	sLog.outDebug("HandlePetInfo is called");
}

void WorldSession::HandlePetNameQuery( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint32 petNumber = 0;
	uint64 petGuid = 0;

	recv_data >> petNumber >> petGuid;
	Pet *pPet = _player->GetMapMgr()->GetPet( GET_LOWGUID_PART( petGuid ) );
	if( !pPet )
		return;

	WorldPacket data( 9 + pPet->GetName().size() );
	data.SetOpcode( SMSG_PET_NAME_QUERY_RESPONSE );
	data << petNumber;
	data << pPet->GetName();
	data << pPet->GetUInt32Value( UNIT_FIELD_PET_NAME_TIMESTAMP );
	data << uint8( 0 );
	SendPacket( &data );
}

void WorldSession::HandleStablePet(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	// remove pet from world and association with player
	Pet *pPet = _player->GetSummon();
	if( pPet != NULL && pPet->IsSummon() ) 
		return;
	
	PlayerPet *pet = _player->GetPlayerPet( _player->GetUnstabledPetNumber() );
	if( !pet )
		return;
	pet->stablestate = STABLE_STATE_PASSIVE;
	
	if( pPet != NULL )//if pPet is NULL here then the pet is dead and we relogged.
		pPet->Remove( true, true );

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x8);  // success
	SendPacket(&data);
}

void WorldSession::HandleUnstablePet(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN
	
	uint64 npcguid = 0;
	uint32 petnumber = 0;

	recv_data >> npcguid >> petnumber;
	PlayerPet *pet = _player->GetPlayerPet( petnumber );
	if(!pet)
	{
		sLog.outError("PET SYSTEM: Player "I64FMT" tried to unstable non-existent pet %d", _player->GetGUID(), petnumber);
		return;
	}
	//unstable selected pet but spawn it only if it's alive
	if( pet->alive )
		_player->SpawnPet( petnumber );
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data(1);
	data.SetOpcode( SMSG_STABLE_RESULT );
	data << uint8(0x9); // success?
	SendPacket(&data);
}
void WorldSession::HandleStableSwapPet(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 npcguid = 0;
	uint32 petnumber = 0;
	recv_data >> npcguid >> petnumber;

	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		sLog.outError("PET SYSTEM: Player "I64FMT" tried to unstable non-existent pet %d", _player->GetGUID(), petnumber);
		return;
	}
	Pet *pPet = _player->GetSummon();
	if( pPet != NULL && pPet->IsSummon() )
		return;

	//stable current pet
	PlayerPet *pet2 = _player->GetPlayerPet( _player->GetUnstabledPetNumber() );
	if( !pet2 )
		return;

	if( pPet != NULL)//if pPet is NULL here then the pet is dead and we relogged.
		pPet->Remove( true, true );
	pet2->stablestate = STABLE_STATE_PASSIVE;

	//unstable selected pet but spawn it only if it's alive
	if( pet->alive )
		_player->SpawnPet( petnumber );
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data;
	data.SetOpcode( SMSG_STABLE_RESULT );
	data << uint8(0x09);
	SendPacket(&data);
}

void WorldSession::HandleStabledPetList(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

    uint64 npcguid = 0;
	recv_data >> npcguid;

    if( _player->getClass() != HUNTER ){
        GossipMenu * pMenu;
		objmgr.CreateGossipMenuForPlayer(&pMenu,npcguid,13584,_player);
		pMenu->SendTo(_player);
		return;
    }

	SendStabledPetList(npcguid);
}

void WorldSession::HandleBuyStableSlot( WorldPacket &recv_data )
{
	CHECK_INWORLD_RETURN

	BankSlotPrice* bsp = dbcStableSlotPrices.LookupEntryForced( _player->GetStableSlotCount() + 1 );
	int32 cost = ( bsp != NULL ) ? bsp->Price : 99999999;
	
	WorldPacket data( SMSG_STABLE_RESULT, 1 );
	
	if( !_player->HasGold(cost) )
	{
		data << uint8(1); // not enough money
		SendPacket( &data );
 		return;
	}
 	_player->ModGold( -cost );
 	
	data << uint8( 0x0A );
	SendPacket( &data );
 	
	_player->m_StableSlotCount++;

#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Misc();
#endif
}


void WorldSession::HandlePetSetActionOpcode( WorldPacket& recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint32 slot;
	uint16 spell;
	uint16 state;
	
	recv_data >> guid >> slot >> spell >> state;
	
	if( !_player->GetSummon() )
		return;
	SpellEntry * spe = dbcSpell.LookupEntryForced( spell );
	if( spe == NULL )
		return;
	Pet * pet = NULL;
	for(std::list<Pet*>::iterator itr = _player->getSummonStart(); itr != _player->getSummonEnd(); ++itr)
	{
		if( (*itr)->GetGUID() == guid )
		{
			pet = *itr;
			break;
		}
	}
	if(pet != NULL)
	{
		PetAI * ai = TO_AIPET( pet->GetAIInterface() );
		if(ai != NULL)
		{
		//find the spell.
		AI_Spell * aispell = ai->Spell_findbyId( uint32(spell) );
		//set the autocast.
		if( aispell)
			ai->Spell_setautocast( (AI_PetSpell*)aispell, (state != 0) ? true : false);
		} 
	}
	//pet->ActionBar[ slot ] = spell;
}

void WorldSession::HandlePetRename(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	string name;
	recv_data >> guid >> name;

	Pet * pet = NULL;
	//std::list<Pet*> & summons = _player->GetSummons();
	for(std::list<Pet*>::iterator itr = _player->getSummonStart(); itr != _player->getSummonEnd(); ++itr)
	{
		if( (*itr)->GetGUID() == guid)
		{
			pet = (*itr);
			break;
		}
	}
	if(pet == NULL)
	{	
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}
	name = CharacterDatabase.EscapeString(name);

	pet->Rename(name);

	// Disable pet rename.
	pet->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | /* (0x28 << 8) | */ (PET_RENAME_NOT_ALLOWED << 16) );

    Arcemu::Util::ARCEMU_ASSERT(    pet->GetPetOwner() != NULL );
    
    if( pet->GetPetOwner()->IsPvPFlagged() )
        pet->SetPvPFlag();
    else
        pet->RemovePvPFlag();

    if( pet->GetPetOwner()->IsFFAPvPFlagged() )
        pet->SetFFAPvPFlag();
    else
        pet->RemoveFFAPvPFlag();

	if( pet->GetPetOwner()->IsSanctuaryFlagged() )
		pet->SetSanctuaryFlag();
	else
		pet->RemoveSanctuaryFlag();
}

void WorldSession::HandlePetAbandon(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	Pet * pet = _player->GetSummon();
	if(!pet) return;

	pet->Dismiss();
}
void WorldSession::HandlePetUnlearn( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Pet* pPet = _player->GetSummon();
	if( pPet == NULL || pPet->GetGUID() != guid )
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	int32 cost = pPet->getUntrainCost();
	if( !_player->HasGold(cost) )
	{
		WorldPacket data( SMSG_BUY_FAILED, 12 );
		data << uint64( _player->GetGUID() );
		data << uint32( 0 );
		data << uint8( 2 );		//not enough money
		SendPacket( &data );
		return;	
	}
	_player->ModGold( -cost );
	
	pPet->Talent_wipe();
	pPet->SetTPs( pPet->GetTPsForLevel( pPet->getLevel() ) );
	pPet->SendTalentsToOwner();
}

void WorldSession::HandlePetSpellAutocast( WorldPacket& recvPacket )
{
	// handles toggle autocast from spellbook
	CHECK_INWORLD_RETURN

	uint64 guid;
    uint16 spellid;
    uint16 unk;
    uint8  state;
    recvPacket >> guid >> spellid >> unk >> state;

	SpellEntry * spe = dbcSpell.LookupEntryForced( spellid );
	if( spe == NULL )
		return;

	//std::list<Pet*> & summons = _player->GetSummons();
	
	Pet * pet = NULL;
	for(std::list<Pet*>::iterator itr = _player->getSummonStart(); itr != _player->getSummonEnd(); ++itr)
	{
		if( (*itr)->GetGUID() == guid)
		{
			pet = *itr;
			break;
		}
	}
	if(pet != NULL)
	{
		PetAI * ai = TO_AIPET(pet->GetAIInterface() );
		AI_Spell * aispell = ai->Spell_findbyId( uint32(spellid) );
		if( aispell)
			ai->Spell_setautocast( (AI_PetSpell*)aispell, (state != 0) ? true : false );
		pet->SendSpellsToOwner();
	}
}
void WorldSession::HandlePetCancelAura( WorldPacket& recvPacket )
{
	CHECK_INWORLD_RETURN

	uint64 guid;
    uint16 spellid;

	recvPacket >> guid >> spellid;

	//std::list<Pet*> & summons = _player->GetSummons();
	for(std::list<Pet*>::iterator itr = _player->getSummonStart(); itr != _player->getSummonEnd(); ++itr)
	{
		if( (*itr)->GetGUID() == guid )//guid should be the pet guid
		{
			if( !(*itr)->RemoveAura( spellid ) )
				sLog.outError("PET SYSTEM: Player "I64FMT" failed to cancel aura %u from pet", _player->GetGUID(), spellid );
			break;
		}
	}
}

void WorldSession::HandlePetLearnTalent( WorldPacket & recvPacket )
{
	CHECK_INWORLD_RETURN

	uint64 guid;
    uint32 talentid;
	uint32 talentcol;

	recvPacket >> guid >> talentid >> talentcol;

	Pet * pPet = _player->GetSummon();
	if( pPet == NULL )
		return;
	
	// check talent points first
	if( pPet->GetTPs() < 1 )
		return;

	// find talent entry
	TalentEntry *te = dbcTalent.LookupEntryForced( talentid );
	if( te == NULL )
		return;

	// check if it requires another talent
	if( te->DependsOn > 0 )
	{
		TalentEntry *dep_te = dbcTalent.LookupEntryForced( te->DependsOn );
		if( dep_te == NULL )
			return;

		bool req_ok = false;
		for( uint8 i = 0; i < 5; ++i )
		{
			if( dep_te->RankID[i] != 0 )
			{
				if( pPet->Talent_has( dep_te->RankID[i] ) )
				{
					req_ok = true;
					break;
				}
			}
		}
		if( !req_ok )
			return;
	}

	// check if we have enough spent points
	if( pPet->GetSpentTPs() < ( te->Row * 3 ) )
		return;
	
	// remove lower talent rank
	if( talentcol > 0 && te->RankID[ talentcol - 1 ] != 0 )
		pPet->Talent_remove( te->RankID[ talentcol - 1 ] );

	// add spell, discount talent point
	SpellEntry* sp = dbcSpell.LookupEntryForced( te->RankID[ talentcol ] );
	if( sp != NULL )
	{
		pPet->Talent_add(sp->Id, true);
		pPet->SetTPs( pPet->GetTPs() - 1 );
		OutPacket( SMSG_PET_LEARNED_SPELL, 4, &sp->Id );
	}
	
	// send talent update
	pPet->SendTalentsToOwner();
}


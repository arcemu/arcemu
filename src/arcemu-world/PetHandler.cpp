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

void WorldSession::HandlePetAction(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;

	//WorldPacket data;
	uint64 petGuid = 0;
	uint16 misc = 0;
	uint16 action = 0;

	uint64 targetguid = 0;
	recv_data >> petGuid >> misc >> action;
	//recv_data.hexlike();

	//printf("Pet_Action: 0x%.4X 0x%.4X\n", misc, action);

	if(GET_TYPE_FROM_GUID(petGuid) == HIGHGUID_TYPE_UNIT)
	{
		Creature *pCharm = GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(petGuid));
		if(!pCharm) 
			return;

		// must be a mind controled creature..
		if(action == PET_ACTION_ACTION)
		{
			recv_data >> targetguid;
			switch(misc)
			{
			case PET_ACTION_ATTACK:
				{
					if(!sEventMgr.HasEvent(_player, EVENT_PLAYER_CHARM_ATTACK))
					{
						uint32 timer = pCharm->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
						if(!timer) timer = 2000;

						sEventMgr.AddEvent(_player, &Player::_EventCharmAttack, EVENT_PLAYER_CHARM_ATTACK, timer, 0,0);
						_player->_EventCharmAttack();
					}
				}break;
			}
		}
		return;
	}
	Pet *pPet = _player->GetMapMgr()->GetPet((uint32)petGuid);
	if(!pPet || !pPet->isAlive())
		return;

	Unit *pTarget = NULL;

	if(action == PET_ACTION_SPELL || action == PET_ACTION_SPELL_1 || action == PET_ACTION_SPELL_2 || (action == PET_ACTION_ACTION && misc == PET_ACTION_ATTACK )) // >> target
	{
		recv_data >> targetguid;
		pTarget = _player->GetMapMgr()->GetUnit(targetguid);
		if(!pTarget) pTarget = pPet;	// target self
	}

	switch(action)
	{
	case PET_ACTION_ACTION:
		{
			pPet->SetPetAction(misc);	   // set current action
			switch(misc)
			{
			case PET_ACTION_ATTACK:
				{
					// make sure the target is attackable
					if( pTarget == pPet || !isAttackable( pPet, pTarget ) )
 					{
						pPet->SendCastFailed( 0, SPELL_FAILED_BAD_TARGETS );
						return;
					}

					// Clear the threat
					pPet->GetAIInterface()->WipeTargetList();
					pPet->GetAIInterface()->WipeHateList();

					// Attack target with melee if the owner if we dont have spells - other wise cast. All done by AIInterface.
					if(pPet->GetAIInterface()->getUnitToFollow() == NULL)
						pPet->GetAIInterface()->SetUnitToFollow(_player);

					// EVENT_PET_ATTACK
					pPet->GetAIInterface()->SetAIState(STATE_ATTACKING);
					pPet->GetAIInterface()->AttackReaction(pTarget, 1, 0);
				}break;
			case PET_ACTION_FOLLOW:
				{
					// Clear the threat
					pPet->GetAIInterface()->WipeTargetList();
					pPet->GetAIInterface()->WipeHateList();

					// Follow the owner... run to him...
					pPet->GetAIInterface()->SetUnitToFollow(_player);
					pPet->GetAIInterface()->HandleEvent(EVENT_FOLLOWOWNER, pPet, 0);
				}break;
			case PET_ACTION_STAY:
				{
					// Clear the threat
					pPet->GetAIInterface()->WipeTargetList();
					pPet->GetAIInterface()->WipeHateList();

					// Stop following the owner, and sit.
					pPet->GetAIInterface()->SetUnitToFollow(NULL);
				}break;
			case PET_ACTION_DISMISS:
				{
					// Bye byte...
					pPet->Dismiss();
				}break;
			}
		}break;

	case PET_ACTION_SPELL_2:
	case PET_ACTION_SPELL_1:
	case PET_ACTION_SPELL:
		{
			// misc == spellid
			SpellEntry * entry = dbcSpell.LookupEntry( misc );
			if( entry == NULL ) 
				return;

			AI_Spell * sp = pPet->GetAISpellForSpellId( entry->Id );
			if( sp != NULL )
			{
				// Check the cooldown
				if(sp->cooldowntime && getMSTime() < sp->cooldowntime)
				{
					pPet->SendCastFailed( misc, SPELL_FAILED_NOT_READY );
					return;
				}
				else
				{
					if( sp->spellType != STYPE_BUFF )
					{
						// make sure the target is attackable
						if( pTarget == pPet || !isAttackable( pPet, pTarget ) )
						{
							pPet->SendCastFailed( misc, SPELL_FAILED_BAD_TARGETS );
							return;
						}
					}

					if(sp->autocast_type != AUTOCAST_EVENT_ATTACK)
					{
						if(sp->autocast_type == AUTOCAST_EVENT_OWNER_ATTACKED)
							pPet->CastSpell(_player, sp->spell, false);
						else
							pPet->CastSpell(pPet, sp->spell, false);
					}
					else
					{
						// Clear the threat
						pPet->GetAIInterface()->WipeTargetList();
						pPet->GetAIInterface()->WipeHateList();

						pPet->GetAIInterface()->AttackReaction(pTarget, 1, 0);
						pPet->GetAIInterface()->SetNextSpell(sp);
					}
				}
			}
		}break;
	case PET_ACTION_STATE:
		{
			pPet->SetPetState(misc);
		}break;
	default:
		{
			printf("WARNING: Unknown pet action received. Action = %.4X, Misc = %.4X\n", action, misc);
		}break;
	}

	/* Send pet action sound - WHEE THEY TALK */
	WorldPacket actionp(SMSG_PET_ACTION_SOUND, 12);
	actionp << pPet->GetGUID() << uint32(1);
	SendPacket(&actionp);
}

void WorldSession::HandlePetInfo(WorldPacket & recv_data)
{
	//nothing
	sLog.outDebug("HandlePetInfo is called");
}

void WorldSession::HandlePetNameQuery(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 petNumber = 0;
	uint64 petGuid = 0;

	recv_data >> petNumber >> petGuid;
	Pet *pPet = _player->GetMapMgr()->GetPet((uint32)petGuid);
	if(!pPet) return;

	WorldPacket data(8 + pPet->GetName().size());
	data.SetOpcode(SMSG_PET_NAME_QUERY_RESPONSE);
	data << (uint32)pPet->GetUIdFromGUID();
	data << pPet->GetName();
	data << pPet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);		// stops packet flood
	SendPacket(&data);
}

void WorldSession::HandleStablePet(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;

	// remove pet from world and association with player
	Pet *pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) 
		return;
	
	PlayerPet *pet = _player->GetPlayerPet(_player->GetUnstabledPetNumber());
	if(!pet) return;
	pet->stablestate = STABLE_STATE_PASSIVE;
	
	if(pPet) pPet->Remove(false, true, true);	// no safedelete needed

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x8);  // success
	SendPacket(&data);
}

void WorldSession::HandleUnstablePet(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	
	uint64 npcguid = 0;
	uint32 petnumber = 0;

	recv_data >> npcguid >> petnumber;
	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		sLog.outError("PET SYSTEM: Player "I64FMT" tried to unstable non-existant pet %d", _player->GetGUID(), petnumber);
		return;
	}
	_player->SpawnPet(petnumber);
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x9); // success?
	SendPacket(&data);
}
void WorldSession::HandleStableSwapPet(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;

	uint64 npcguid = 0;
	uint32 petnumber = 0;
	recv_data >> npcguid >> petnumber;

	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		sLog.outError("PET SYSTEM: Player "I64FMT" tried to unstable non-existant pet %d", _player->GetGUID(), petnumber);
		return;
	}
	Pet *pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) return;

	//stable current pet
	PlayerPet *pet2 = _player->GetPlayerPet(_player->GetUnstabledPetNumber());
	if(!pet2) return;
	if(pPet)
		pPet->Remove(false, true, true);	// no safedelete needed
	pet2->stablestate = STABLE_STATE_PASSIVE;

	//unstable selected pet
	_player->SpawnPet(petnumber);
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data;
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x09);
	SendPacket(&data);
}

void WorldSession::HandleStabledPetList(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(10 + (_player->m_Pets.size() * 25));
	data.SetOpcode(MSG_LIST_STABLED_PETS);

	uint64 npcguid = 0;
	recv_data >> npcguid;
	data << npcguid;

	data << uint8(_player->m_Pets.size());
	data << uint8(_player->m_StableSlotCount);
	char i=0;
	for(std::map<uint32, PlayerPet*>::iterator itr = _player->m_Pets.begin(); itr != _player->m_Pets.end(); ++itr)
	{
		data << uint32(itr->first); // pet no
		data << uint32(itr->second->entry); // entryid
		data << uint32(itr->second->level); // level
		data << itr->second->name;		  // name
		data << uint32(itr->second->loyaltylvl);
		if(itr->second->stablestate == STABLE_STATE_ACTIVE)
			data << uint8(STABLE_STATE_ACTIVE);
		else
		{
			data << uint8(STABLE_STATE_PASSIVE + i);
			i++;
		}
	}

	SendPacket(&data);
}

void WorldSession::HandleBuyStableSlot(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || _player->GetStableSlotCount() == 2) return;
	uint8 scount = _player->GetStableSlotCount();
	BankSlotPrice* bsp = dbcStableSlotPrices.LookupEntry(scount+1);
	int32 cost = (bsp != NULL) ? bsp->Price : 99999999;
	if(cost > (int32)_player->GetUInt32Value(PLAYER_FIELD_COINAGE))
		return;

	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -cost);
	
	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x0A);
	SendPacket(&data);
	if(_player->GetStableSlotCount() > 2)
		_player->m_StableSlotCount = 2;
	else
		_player->m_StableSlotCount++;
#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Misc();
#endif
}


void WorldSession::HandlePetSetActionOpcode(WorldPacket& recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 unk1;
	uint32 unk2;
	uint32 slot;
	uint16 spell;
	uint16 state;
	recv_data >> unk1 >> unk2 >> slot >> spell >> state;
	if(!_player->GetSummon())
		return;

	Pet * pet = _player->GetSummon();
	SpellEntry * spe = dbcSpell.LookupEntryForced( spell );
	if( spe == NULL )
		return;

	// do we have the spell? if not don't set it (exploit fix)
	PetSpellMap::iterator itr = pet->GetSpells()->find( spe );
	if( itr == pet->GetSpells()->end( ) )
		return;

	pet->ActionBar[slot] = spell;
	pet->SetSpellState(spell, state);
}

void WorldSession::HandlePetRename(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	string name;
	recv_data >> guid >> name;

	if(!_player->GetSummon() || _player->GetSummon()->GetGUID() != guid)
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	Pet * pet = _player->GetSummon();
	pet->Rename(name);

	// Disable pet rename.
	pet->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x28 << 8) | (0x2 << 16));
}

void WorldSession::HandlePetAbandon(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	Pet * pet = _player->GetSummon();
	if(!pet) return;

	pet->Dismiss(false);
}
void WorldSession::HandlePetUnlearn(WorldPacket & recv_data)
{
	if( !_player->IsInWorld() )
		return;

	uint64 guid;
	recv_data >> guid;

	Pet* pPet = _player->GetSummon();
	if( pPet == NULL || pPet->GetGUID() != guid )
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	int32 cost = pPet->GetUntrainCost();
	if( cost > ( int32 )_player->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
	{
		WorldPacket data(SMSG_BUY_FAILED, 12);
		data << uint64( _player->GetGUID() );
		data << uint32( 0 );
		data << uint8( 2 );		//not enough money
		return;	
	}
	_player->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
	pPet->WipeSpells();
}
void WorldSession::HandlePetSpellAutocast( WorldPacket& recvPacket )
{
	// handles toggle autocast from spellbook
	if( !_player->IsInWorld() )
		return;

	uint64 guid;
    uint16 spellid;
    uint16 unk;
    uint8  state;
    recvPacket >> guid >> spellid >> unk >> state;

	Pet * pPet = _player->GetSummon();
	if( pPet == NULL )
		return;
	
	SpellEntry * spe = dbcSpell.LookupEntryForced( spellid );
	if( spe == NULL )
		return;
	
	// do we have the spell? if not don't set it (exploit fix)
	PetSpellMap::iterator itr = pPet->GetSpells()->find( spe );
	if( itr == pPet->GetSpells()->end( ) )
		return;

	pPet->SetSpellState( spellid, state > 0 ? AUTOCAST_SPELL_STATE : DEFAULT_SPELL_STATE );
}
void WorldSession::HandlePetCancelAura( WorldPacket& recvPacket )
{
	if( !_player->IsInWorld() )
		return;

	uint64 guid;
    uint16 spellid;

	recvPacket >> guid >> spellid;

	Pet * pPet = _player->GetSummon();
	if( pPet == NULL )
		return;

	if( !pPet->RemoveAura( spellid ) )
		sLog.outError("PET SYSTEM: Player "I64FMT" failed to cancel aura %u from pet", _player->GetGUID(), spellid );
}



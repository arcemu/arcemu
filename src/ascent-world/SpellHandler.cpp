/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#include "StdAfx.h"

void WorldSession::HandleUseItemOpcode(WorldPacket& recvPacket)
{
	if(!_player->IsInWorld()) return;
	typedef std::list<Aura*> AuraList;
	
	Player* p_User = GetPlayer();
	sLog.outDetail("WORLD: got use Item packet, data length = %i",recvPacket.size());
	int8 tmp1,slot,tmp3;
	uint64 item_guid;
	uint8 cn;
	uint32 spellId = 0;

	recvPacket >> tmp1 >> slot >> tmp3 >> cn >> item_guid;
	Item* tmpItem = NULL;
	tmpItem = p_User->GetItemInterface()->GetInventoryItem(tmp1,slot);
	if (!tmpItem)
		tmpItem = p_User->GetItemInterface()->GetInventoryItem(slot);
	if (!tmpItem)
		return;
	ItemPrototype *itemProto = tmpItem->GetProto();
	if(!itemProto)
		return;

	if(_player->getDeathState()==CORPSE)
		return;

	if(itemProto->Bonding == ITEM_BIND_ON_USE)
		tmpItem->SoulBind();

	if(sScriptMgr.CallScriptedItem(tmpItem,_player))
		return;

	if(itemProto->QuestId)
	{
		// Item Starter
		Quest *qst = QuestStorage.LookupEntry(itemProto->QuestId);
		if(!qst) 
			return;

		WorldPacket data;
		sQuestMgr.BuildQuestDetails(&data, qst, tmpItem, 0, language, _player);
		SendPacket(&data);
	}
	
	SpellCastTargets targets(recvPacket, _player->GetGUID());
	uint32 x;
	for(x = 0; x < 5; x++)
	{
		if(itemProto->Spells[x].Trigger == USE)
		{
			if(itemProto->Spells[x].Id)
			{
				spellId = itemProto->Spells[x].Id;
				break;
			}
		}
	}
	if(!spellId)
		return;
	// check for spell id
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced( spellId );

	if ( !spellInfo || !sHookInterface.OnCastSpell( _player, spellInfo ) )
	{
		sLog.outError("WORLD: unknown spell id %i\n", spellId);
		return;
	}

	if (spellInfo->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		if (p_User->CombatStatus.IsInCombat() || p_User->IsMounted())
		{
			_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_CANT_DO_IN_COMBAT);
			return;
		}
		if(p_User->GetStandState()!=1)
		p_User->SetStandState(STANDSTATE_SIT);
		// loop through the auras and removing existing eating spells
	}
	
	// ceberwow remove stealth on using item
	if (!(spellInfo->AuraInterruptFlags & ATTRIBUTESEX_NOT_BREAK_STEALTH))
	{
		if( p_User->IsStealth() )
			p_User->RemoveAllAuraType( SPELL_AURA_MOD_STEALTH );
	}

	if(itemProto->RequiredLevel)
	{
		if(_player->getLevel() < itemProto->RequiredLevel)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			return;
		}
	}

	if(itemProto->RequiredSkill)
	{
		if(!_player->_HasSkillLine(itemProto->RequiredSkill))
		{
			_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			return;
		}

		if(itemProto->RequiredSkillRank)
		{
			if(_player->_GetSkillLineCurrent(itemProto->RequiredSkill, false) < itemProto->RequiredSkillRank)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_ITEM_RANK_NOT_ENOUGH);
				return;
			}
		}
	}
	
	if( itemProto->AllowableClass && !(_player->getClassMask() & itemProto->AllowableClass) || itemProto->AllowableRace && !(_player->getRaceMask() & itemProto->AllowableRace) )
	{
		_player->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM);
		return;
	}		

	if( !_player->Cooldown_CanCast( itemProto, x ) )
	{
		_player->SendCastResult(spellInfo->Id, SPELL_FAILED_NOT_READY, cn, 0);
		return;
	}

	if(_player->m_currentSpell)
	{
		_player->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
		return;
	}

	if( itemProto->ForcedPetId >= 0 )
	{
		if( itemProto->ForcedPetId == 0 )
		{
			if( _player->GetGUID() != targets.m_unitTarget )
			{
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0);
				return;
			}
		}
		else
		{
			if( !_player->GetSummon() || _player->GetSummon()->GetEntry() != (uint32)itemProto->ForcedPetId )
			{
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
				return;
			}
		}
	}

	Spell *spell = new Spell(_player, spellInfo, false, NULL);
	uint8 result;
	spell->extra_cast_number=cn;
	spell->i_caster = tmpItem;
	//GetPlayer()->setCurrentSpell(spell);
	result = spell->prepare(&targets);

	//if( result == SPELL_CANCAST_OK ) // incorrect - should be on Completed Cast
	//	_player->Cooldown_AddItem( itemProto, x );
}

void WorldSession::HandleCastSpellOpcode(WorldPacket& recvPacket)
{
	if(!_player->IsInWorld()) return;
	if(_player->getDeathState()==CORPSE)
		return;

	uint32 spellId;
	uint8 cn;

	recvPacket >> spellId >> cn;
	// check for spell id
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced(spellId );

	if(!spellInfo || !sHookInterface.OnCastSpell(_player, spellInfo))
	{
		sLog.outError("WORLD: unknown spell id %i\n", spellId);
		return;
	}

	sLog.outDetail("WORLD: got cast spell packet, spellId - %i (%s), data length = %i",
		spellId, spellInfo->Name, recvPacket.size());
	
	// Cheat Detection only if player and not from an item
	// this could fuck up things but meh it's needed ALOT of the newbs are using WPE now
	// WPE allows them to mod the outgoing packet and basicly choose what ever spell they want :(

	if( !GetPlayer()->HasSpell(spellId) || spellInfo->Attributes & ATTRIBUTES_PASSIVE )
	{
		sLog.outDetail("WORLD: Spell isn't casted because player \"%s\" is cheating", GetPlayer()->GetName());
		return;
	}

	if (GetPlayer()->GetOnMeleeSpell() != spellId)
	{
		//autoshot 75
		if((spellInfo->AttributesExB & FLAGS3_ACTIVATE_AUTO_SHOT) /*spellInfo->Attributes == 327698*/)	// auto shot..
		{
			//sLog.outString( "HandleSpellCast: Auto Shot-type spell cast (id %u, name %s)" , spellInfo->Id , spellInfo->Name );
			Item *weapon = GetPlayer()->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
			if(!weapon) 
				return;
			uint32 spellid;
			switch(weapon->GetProto()->SubClass)
			{
			case 2:			 // bows
			case 3:			 // guns
            case 18:		 // crossbow
				spellid = SPELL_RANGED_GENERAL;
				break;
			case 16:			// thrown
				spellid = SPELL_RANGED_THROW;
				break;
			case 19:			// wands
				spellid = SPELL_RANGED_WAND;
				break;
			default:
				spellid = 0;
				break;
			}
		   
			if(!spellid) 
				spellid = spellInfo->Id;
			
			if(!_player->m_onAutoShot)
			{
				_player->m_AutoShotTarget = _player->GetSelection();
				uint32 duration = _player->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME);
				SpellCastTargets targets(recvPacket,GetPlayer()->GetGUID());
				if(!targets.m_unitTarget)
				{
					sLog.outString( "Cancelling auto-shot cast because targets.m_unitTarget is null!" );
					return;
				}
				SpellEntry *sp = dbcSpell.LookupEntry(spellid);
			
				_player->m_AutoShotSpell = sp;
				_player->m_AutoShotDuration = duration;
				//This will fix fast clicks
				if(_player->m_AutoShotAttackTimer < 500)
					_player->m_AutoShotAttackTimer = 500;
				_player->m_onAutoShot = true;
			}

			return;
		}
		/*const char * name = sSpellStore.LookupString(spellInfo->Name);
		if(name)
			sChatHandler.SystemMessageToPlr(_player, "%sSpell Cast:%s %s %s[Group %u, family %u]", MSG_COLOR_LIGHTBLUE,
			MSG_COLOR_SUBWHITE, name, MSG_COLOR_YELLOW, spellInfo->SpellGroupType, spellInfo->SpellFamilyName);*/

        if(_player->m_currentSpell)
        {
			if( _player->m_currentSpell->getState() == SPELL_STATE_CASTING )
			{
				// cancel the existing channel spell, cast this one
				_player->m_currentSpell->cancel();
			}
			else
			{
				// send the error message
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
				return;
			}
        }

		SpellCastTargets targets(recvPacket,GetPlayer()->GetGUID());

		// some anticheat stuff
		if( spellInfo->self_cast_only )
		{
			if( targets.m_unitTarget && targets.m_unitTarget != _player->GetGUID() )
			{
				// send the error message
				_player->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0);
				return;
			}
		}

		Spell *spell = new Spell(GetPlayer(), spellInfo, false, NULL);
		spell->extra_cast_number=cn;
		spell->prepare(&targets);
	}
}

void WorldSession::HandleCancelCastOpcode(WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	if(GetPlayer()->m_currentSpell)
		GetPlayer()->m_currentSpell->cancel();
}

void WorldSession::HandleCancelAuraOpcode( WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;
	
	for(uint32 x = 0; x < MAX_AURAS+MAX_POSITIVE_AURAS; ++x)
	{
		if(_player->m_auras[x] && _player->m_auras[x]->IsPositive() && _player->m_auras[x]->GetSpellId() == spellId)
			_player->m_auras[x]->Remove();
	}
	sLog.outDebug("removing aura %u",spellId);
}

void WorldSession::HandleCancelChannellingOpcode( WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	Player *plyr = GetPlayer();
	if(!plyr)
		return;
	if(plyr->m_currentSpell)
	{		
		plyr->m_currentSpell->cancel();
	}
}

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& recv_data)
{
	//sLog.outString("Received CMSG_CANCEL_AUTO_REPEAT_SPELL message.");
	//on original we automatically enter combat when creature got close to us
//	GetPlayer()->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
	GetPlayer()->m_onAutoShot = false;
}

void WorldSession::HandleAddDynamicTargetOpcode(WorldPacket & recvPacket)
{
	uint64 guid;
	uint32 spellid;
	uint32 flags;
	recvPacket >> guid >> spellid >> flags;
	
	SpellEntry * sp = dbcSpell.LookupEntryForced(spellid);
	if ( !sp ) 
		return;
	// Summoned Elemental's Freeze
    if (spellid == 33395)
    {
        if (!_player->m_Summon)
            return;
    }
    else if ( guid != _player->m_CurrentCharm )
    {
        return;
    }
	
	/* burlex: this is.. strange */
	SpellCastTargets targets;
	targets.m_targetMask = flags;

	if(flags == 0)
		targets.m_unitTarget = guid;
	else if(flags & TARGET_FLAG_UNIT)
	{
		WoWGuid guid;
//		recvPacket >> flags;		// skip one byte
		recvPacket >> guid;
		targets.m_unitTarget = guid.GetOldGuid();
	}
	else if(flags & TARGET_FLAG_SOURCE_LOCATION)
	{
//		recvPacket >> flags;		// skip one byte
		recvPacket >> targets.m_srcX >> targets.m_srcY >> targets.m_srcZ;
	}
	else if(flags & TARGET_FLAG_DEST_LOCATION)
	{
//		recvPacket >> flags;		// skip one byte
		recvPacket >> targets.m_destX >> targets.m_destY >> targets.m_destZ;
	}
	else if (flags & TARGET_FLAG_STRING)
	{
//		recvPacket >> flags;		// skip one byte
		recvPacket >> targets.m_strTarget;
	}
	if(spellid == 33395)	// Summoned Water Elemental's freeze
	{
		Spell * pSpell = new Spell(_player->m_Summon, sp, false, 0);
		pSpell->prepare(&targets);
	}
	else			// trinket?
	{
		Unit *nc = _player->GetMapMgr()->GetUnit( _player->m_CurrentCharm );
		if( nc )
		{
			Spell * pSpell = new Spell(nc, sp, false, 0);
			pSpell->prepare(&targets);
		}
	}
}

/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

trainertype trainer_types[TRAINER_TYPE_MAX] = 
{
{	"Warrior",			   0 },
{	"Paladin",			   0 },
{	"Rogue"  ,			   0 },
{	"Warlock",			   0 },
{	"Mage",				  0 },
{	"Shaman",				0 },
{	"Priest",				0 },
{	"Hunter",				0 },
{	"Druid",				 0 },
{	"Leatherwork",		   2 },
{	"Skinning",			  2 },
{	"Fishing",			   2 },
{	"First Aid",			 2 },
{	"Physician",			 2 },
{	"Engineer",			  2 },
{	"Weapon Master",		 0 },
};

bool CanTrainAt(Player * plr, Trainer * trn)
{
	if ( (trn->RequiredClass && plr->getClass() != trn->RequiredClass) ||
		 ( (trn->RequiredRace && plr->getRace() != trn->RequiredRace) && ((trn->RequiredRepFaction && trn->RequiredRepValue) && plr->GetStanding(trn->RequiredRepFaction) != trn->RequiredRepValue)) ||
		 (trn->RequiredSkill && !plr->_HasSkillLine(trn->RequiredSkill)) ||
		 (trn->RequiredSkillLine && plr->_GetSkillLineCurrent(trn->RequiredSkill) < trn->RequiredSkillLine) )
	{
		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_TABARDVENDOR_ACTIVATE:
//////////////////////////////////////////////////////////////
void WorldSession::HandleTabardVendorActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;
	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature) return;

	SendTabardHelp(pCreature);
}

void WorldSession::SendTabardHelp(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(8);
	data.Initialize( MSG_TABARDVENDOR_ACTIVATE );
	data << pCreature->GetGUID();
	SendPacket( &data );
}


//////////////////////////////////////////////////////////////
/// This function handles CMSG_BANKER_ACTIVATE:
//////////////////////////////////////////////////////////////
void WorldSession::HandleBankerActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;

	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature) return;

	SendBankerList(pCreature);
}

void WorldSession::SendBankerList(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(8);
	data.Initialize( SMSG_SHOW_BANK );
	data << pCreature->GetGUID();
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_TRAINER_LIST
//////////////////////////////////////////////////////////////
//NOTE: we select prerequirements for spell that TEACHES you
//not by spell that you learn!
void WorldSession::HandleTrainerListOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	// Inits, grab creature, check.
	uint64 guid;
	recv_data >> guid;
	Creature *train = GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!train) return;

	_player->Reputation_OnTalk(train->m_factionDBC);
	SendTrainerList(train);
}

void WorldSession::SendTrainerList(Creature* pCreature)
{
	Trainer * pTrainer = pCreature->GetTrainer();
	//if(pTrainer == 0 || !CanTrainAt(_player, pTrainer)) return;
	if(pTrainer==0)
		return;

	if(!CanTrainAt(_player,pTrainer))
	{
		GossipMenu * pMenu;
		objmgr.CreateGossipMenuForPlayer(&pMenu,pCreature->GetGUID(),pTrainer->Cannot_Train_GossipTextId,_player);
		pMenu->SendTo(_player);
		return;
	}

	WorldPacket data(SMSG_TRAINER_LIST, 5000);
	TrainerSpell * pSpell;
	uint32 Spacer = 0;
	uint32 Count=0;
	uint8 Status;
	string Text;

	data << pCreature->GetGUID();
	data << pTrainer->TrainerType;

	data << uint32(0);
	for(vector<TrainerSpell>::iterator itr = pTrainer->Spells.begin(); itr != pTrainer->Spells.end(); ++itr)
	{
		pSpell = &(*itr);
		Status = TrainerGetSpellStatus(pSpell);
		if( pSpell->pCastRealSpell != NULL )
			data << pSpell->pCastSpell->Id;
		else if( pSpell->pLearnSpell )
			data << pSpell->pLearnSpell->Id;
		else
			continue;

		data << Status;
		data << pSpell->Cost;
		data << Spacer;
		data << uint32(pSpell->IsProfession);
		data << uint8(pSpell->RequiredLevel);
		data << pSpell->RequiredSkillLine;
		data << pSpell->RequiredSkillLineValue;
		data << pSpell->RequiredSpell;
		data << Spacer;	//this is like a spell override or something, ex : (id=34568 or id=34547) or (id=36270 or id=34546) or (id=36271 or id=34548)
		data << Spacer;
		++Count;
	}

#ifdef USING_BIG_ENDIAN
	*(uint32*)&data.contents()[12] = swap32(Count);
#else
	*(uint32*)&data.contents()[12] = Count;
#endif
	if ( stricmp(pTrainer->UIMessage,"DMSG")==0 )
		data << _player->GetSession()->LocalizedWorldSrv(37);
	else
		data << pTrainer->UIMessage;
	SendPacket(&data);
}

void WorldSession::HandleTrainerBuySpellOpcode(WorldPacket& recvPacket)
{
	if(!_player->IsInWorld()) return;
	uint64 Guid;
	uint32 TeachingSpellID;

	recvPacket >> Guid >> TeachingSpellID;
	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(Guid));
	if(pCreature == 0) return;

	Trainer *pTrainer = pCreature->GetTrainer();
	if(pTrainer == 0 || !CanTrainAt(_player, pTrainer)) return;

	TrainerSpell * pSpell=NULL;
	for(vector<TrainerSpell>::iterator itr = pTrainer->Spells.begin(); itr != pTrainer->Spells.end(); ++itr)
	{
		if( ( itr->pCastSpell && itr->pCastSpell->Id == TeachingSpellID ) ||
			( itr->pLearnSpell && itr->pLearnSpell->Id == TeachingSpellID ) )
		{
			pSpell = &(*itr);
		}
	}
	
	if(pSpell == NULL)
		return;

	if(TrainerGetSpellStatus(pSpell) > 0) return;
	
	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)pSpell->Cost);

	if( pSpell->pCastSpell)
	{
		// Cast teaching spell on player
		pCreature->CastTrainerSpell(_player, pSpell->pCastSpell, true);
	}

	if( pSpell->pLearnSpell )
	{
		packetSMSG_PLAY_SPELL_VISUAL pck;
		pck.guid = pCreature->GetGUID();
		pck.visualid = 0x5b3;
		_player->OutPacketToSet( SMSG_PLAY_SPELL_VISUAL, sizeof(packetSMSG_PLAY_SPELL_VISUAL), &pck, true );

		pck.guid = _player->GetGUID();
		pck.visualid = 0x16a;
		_player->OutPacketToSet( 0x1F7, sizeof(packetSMSG_PLAY_SPELL_VISUAL), &pck, true );

		// add the spell
		_player->addSpell( pSpell->pLearnSpell->Id );

		uint32 i;
		for( i = 0; i < 3; ++i )
		{
			if( pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_PROFICIENCY || pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_WEAPON ||
				( pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_LEARN_SPELL && pSpell->pLearnSpell->EffectImplicitTargetA[i] != EFF_TARGET_PET ) )  // avoid direct pet teaching
			{
				_player->CastSpell( _player, pSpell->pLearnSpell, true );
				break;
			}
		}

		for( i = 0; i < 3; ++i)
		{
			if( pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_SKILL )
			{
				uint32 skill = pSpell->pLearnSpell->EffectMiscValue[i];
				uint32 val = (pSpell->pLearnSpell->EffectBasePoints[i]+1) * 75;
				if( val > 375 )
					val = 375;

				if( _player->_GetSkillLineMax(skill) > val ) // cebernic: eq
					continue;

				if( skill == SKILL_RIDING )
					_player->_AddSkillLine( skill, val, val );
				else
				{
					if( _player->_HasSkillLine(skill) )
						_player->_ModifySkillMaximum(skill, val);
					else
						_player->_AddSkillLine( skill, 1, val);
				}
			}
		}
	}

	if(pSpell->DeleteSpell)
	{
		// Remove old spell.
		if( pSpell->pLearnSpell )
			_player->removeSpell(pSpell->DeleteSpell, true, true, pSpell->pLearnSpell->Id);
		else if(pSpell->pCastSpell)
			_player->removeSpell(pSpell->DeleteSpell, true, true, pSpell->pCastRealSpell->Id);
		else
			_player->removeSpell(pSpell->DeleteSpell,true,false,0);
	}

	_player->_UpdateSkillFields();
}

uint8 WorldSession::TrainerGetSpellStatus(TrainerSpell* pSpell)
{
	if(!pSpell->pCastSpell && !pSpell->pLearnSpell)
		return TRAINER_STATUS_NOT_LEARNABLE;

	if( pSpell->pCastRealSpell && (_player->HasSpell(pSpell->pCastRealSpell->Id) || _player->HasDeletedSpell(pSpell->pCastRealSpell->Id)) )
		return TRAINER_STATUS_ALREADY_HAVE;

	if( pSpell->pLearnSpell && (_player->HasSpell(pSpell->pLearnSpell->Id) || _player->HasDeletedSpell(pSpell->pLearnSpell->Id)) )
		return TRAINER_STATUS_ALREADY_HAVE;

	if(pSpell->DeleteSpell && _player->HasDeletedSpell(pSpell->DeleteSpell))
		return TRAINER_STATUS_ALREADY_HAVE;

	if(	(pSpell->RequiredLevel && _player->getLevel()<pSpell->RequiredLevel)
		|| (pSpell->RequiredSpell && !_player->HasSpell(pSpell->RequiredSpell))
		|| (pSpell->Cost && _player->GetUInt32Value(PLAYER_FIELD_COINAGE) < pSpell->Cost)
		|| (pSpell->RequiredSkillLine && _player->_GetSkillLineCurrent(pSpell->RequiredSkillLine,true) < pSpell->RequiredSkillLineValue)
		|| (pSpell->IsProfession && pSpell->RequiredSkillLine==0 && _player->GetUInt32Value(PLAYER_CHARACTER_POINTS2) == 0)//check level 1 professions if we can learn a new profession
		)
		return TRAINER_STATUS_NOT_LEARNABLE;
	return TRAINER_STATUS_LEARNABLE;
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_PETITION_SHOWLIST:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCharterShowListOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;

	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature) return;

	SendCharterRequest(pCreature);
}

void WorldSession::SendCharterRequest(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	if(pCreature && ( pCreature->GetEntry()==19861 ||
			pCreature->GetEntry()==18897 || pCreature->GetEntry()==19856 || pCreature->GetEntry()==sWorld.m_CustomCharterGiver ) )
	{
		WorldPacket data(SMSG_PETITION_SHOWLIST, 81);

		data << pCreature->GetGUID();
		data << uint8(0x03); //number of charter types in packet
		
		//2v2 arena charters start
		data << uint32(0x01); //petition number (in packet)
		data << uint32(ARENA_TEAM_CHARTER_2v2); //itemid
		data << uint32(0x3F21); //item displayid
		data << uint32(ARENA_TEAM_CHARTER_2v2_COST); //charter cost
		data << uint32(0x01); //unknown, (charter type? seems to be 0x0 for guilds and 0x1 for arena charters)
		data << uint32(0x01); // Signatures required (besides petition owner)
		//2v2 arena charters end
		
		//3v3 arena charters start
		data << uint32(0x02); //petition number (in packet)
		data << uint32(ARENA_TEAM_CHARTER_3v3); //itemid
		data << uint32(0x3F21); //item displayid
		data << uint32(ARENA_TEAM_CHARTER_3v3_COST); //charter cost
		data << uint32(0x01);
		data << uint32(0x02); // Signatures required (besides petition owner)
		//3v3 arena charters end
		
		//5v5 arena charters start
		data << uint32(0x03); //petition number (in packet)
		data << uint32(ARENA_TEAM_CHARTER_5v5); //itemid
		data << uint32(0x3F21); //item displayid
		data << uint32(ARENA_TEAM_CHARTER_5v5_COST); //charter cost
		data << uint32(0x01);
		data << uint32(0x04); // Signatures required (besides petition owner)
		//5v5 arena charters end
		
		SendPacket(&data);
	}
	else
	{
		WorldPacket data(33);
		data.Initialize( SMSG_PETITION_SHOWLIST );
		data << pCreature->GetGUID();
		data << uint8(1);                  // num charters in packet (although appears to only turn off the cost display, maybe due to packet not being parsed /shrug)
		data << uint32(1);                // charter 1 in packet
		data << uint32(0x16E7);  // ItemId of the guild charter
	    data << uint32(0x3F21);  // item displayid

		data << uint32(1000);      // charter price
		data << uint32(0);                // unknown, maybe charter type
		data << uint32(9);                // amount of unique players needed to sign the charter
		SendPacket( &data );
	}
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_AUCTION_HELLO:
//////////////////////////////////////////////////////////////
void WorldSession::HandleAuctionHelloOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;
	Creature* auctioneer = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!auctioneer)
		return;

	SendAuctionList(auctioneer);
}

void WorldSession::SendAuctionList(Creature* auctioneer)
{
	AuctionHouse* AH = sAuctionMgr.GetAuctionHouse(auctioneer->GetEntry());
	if(!AH)
	{
		sChatHandler.BlueSystemMessage(this, "Report to devs: Unbound auction house npc %u.", auctioneer->GetEntry());
		return;
	}

	WorldPacket data(MSG_AUCTION_HELLO, 12);
	data << auctioneer->GetGUID();
	data << uint32(AH->GetID());

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GOSSIP_HELLO:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGossipHelloOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	list<QuestRelation *>::iterator it;
	std::set<uint32> ql;

	recv_data >> guid;
	Creature *qst_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!qst_giver) 
		return;

	//stop when talked to
	if(qst_giver->GetAIInterface())
		qst_giver->GetAIInterface()->StopMovement(0);

	// unstealth meh
	if( _player->IsStealth() )
		_player->RemoveAllAuraType( SPELL_AURA_MOD_STEALTH );

	// reputation
	_player->Reputation_OnTalk(qst_giver->m_factionDBC);

	sLog.outDebug( "WORLD: Received CMSG_GOSSIP_HELLO from %u",GUID_LOPART(guid) );

	GossipScript * Script = qst_giver->GetCreatureInfo() ? qst_giver->GetCreatureInfo()->gossip_script : NULL;
	if(!Script)
		return;

	if (qst_giver->isQuestGiver() && qst_giver->HasQuests())
	{
		WorldPacket data;
		data.SetOpcode(SMSG_GOSSIP_MESSAGE);
		Script->GossipHello(qst_giver, _player, false);
		if(!_player->CurrentGossipMenu)
			return;

		_player->CurrentGossipMenu->BuildPacket(data);
		uint32 count=0;//sQuestMgr.ActiveQuestsCount(qst_giver, GetPlayer());
		size_t pos=data.wpos();
		data << uint32(count);
		for (it = qst_giver->QuestsBegin(); it != qst_giver->QuestsEnd(); ++it)
		{
			uint32 status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), *it);
			if (status >= QMGR_QUEST_CHAT)
			{
				if (!ql.count((*it)->qst->id) )
				{	
					ql.insert((*it)->qst->id);
					count++;
					data << (*it)->qst->id;
					/*data << status;//sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), *it);
					data << uint32(0);*/
					switch(status)
					{
					case QMGR_QUEST_NOT_FINISHED:
						data << uint32(4) << uint32(0);
						break;

					case QMGR_QUEST_FINISHED:
						data << uint32(4) << uint32(1);
						break;

					case QMGR_QUEST_CHAT:
						data << QMGR_QUEST_AVAILABLE << uint32(0);
						break;

					default:
						data << status << uint32(0);
						break;
					}

					LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest((*it)->qst->id,language):NULL;
					if(lq)
						data << lq->Title;
					else
						data << (*it)->qst->title;
				}
			}
		}
		data.wpos(pos);
		data << count;
		SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_GOSSIP_MESSAGE" );
	}
	else
	{
		Script->GossipHello(qst_giver, _player, true);
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GOSSIP_SELECT_OPTION:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGossipSelectOptionOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	//WorldPacket data;
	uint32 option;
	uint32 unk24;
	uint64 guid;
	int8 extra=0;

	recv_data >> guid >> unk24 >> option;

	sLog.outDetail("WORLD: CMSG_GOSSIP_SELECT_OPTION Option %i Guid %.8X", option, guid );
	GossipScript * Script=NULL;
	Object * qst_giver=NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *crt = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!crt)
			return;

		qst_giver=crt;
		Script=crt->GetCreatureInfo()?crt->GetCreatureInfo()->gossip_script:NULL;
	}
	else if(guidtype==HIGHGUID_TYPE_ITEM)
	{
		Item * pitem = _player->GetItemInterface()->GetItemByGUID(guid);
		if(pitem==NULL)
			return;

		qst_giver=pitem;
		Script=pitem->GetProto()->gossip_script;
	}
	else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
        GameObject *gobj = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(!gobj)
			return;
        
		qst_giver=gobj;
        Script=gobj->GetInfo()->gossip_script;
    }

	if(!Script||!qst_giver)
		return;

	uint32 IntId = 1;
	if(_player->CurrentGossipMenu)
	{
		GossipMenuItem item = _player->CurrentGossipMenu->GetItem(option);
		IntId = item.IntId;
		extra = item.Extra;
	}

	if(extra)
	{
		string str;
		if(recv_data.rpos()!=recv_data.wpos())
			recv_data >> str;

		Script->GossipSelectOption(qst_giver, _player, option, IntId, str.c_str());
	}
	else
		Script->GossipSelectOption(qst_giver, _player, option, IntId, NULL);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_SPIRIT_HEALER_ACTIVATE:
//////////////////////////////////////////////////////////////
void WorldSession::HandleSpiritHealerActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld() ||!_player->IsDead()) return;
	GetPlayer( )->DeathDurabilityLoss(0.25);
	GetPlayer( )->ResurrectPlayer();

	if(_player->getLevel() > 10)
	{
		Aura* aur = GetPlayer()->FindAura(15007);
		
		if( aur == NULL ) // If the player already have the aura, just extend it.
		{
			SpellEntry *spellInfo = dbcSpell.LookupEntry( 15007 );//resurrection sickness
			SpellCastTargets targets;
			targets.m_unitTarget = GetPlayer()->GetGUID();
			Spell * sp = SpellPool.PooledNew();
			if (!sp)
				return;
			sp->Init(_player,spellInfo,true,NULL);
			sp->prepare(&targets);
		}

		//calc new duration
		int32 duration=600000; //10mins

		if (_player->getLevel() < 20)
			duration=(_player->getLevel() - 10) * 60000;

			_player->SetAurDuration(15007,duration); //cebernic: change this to setaurduration() to be refreshed.
	}

	GetPlayer( )->SetUInt32Value(UNIT_FIELD_HEALTH, GetPlayer()->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NPC_TEXT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNpcTextQueryOpcode( WorldPacket & recv_data )
{
	WorldPacket data;
	uint32 textID;
	uint64 targetGuid;
	GossipText *pGossip;

	recv_data >> textID;
	sLog.outDetail("WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID );

	recv_data >> targetGuid;
	GetPlayer()->SetUInt64Value(UNIT_FIELD_TARGET, targetGuid);

	pGossip = NpcTextStorage.LookupEntry(textID);
	LocalizedNpcText * lnc = (language>0) ? sLocalizationMgr.GetLocalizedNpcText(textID,language) : NULL;

	data.Initialize( SMSG_NPC_TEXT_UPDATE );
	data << textID;
	
	if(pGossip)
	{
		data << float(1.0f);		// Unknown
		for(uint32 i=0;i<8;i++)
		{
			if(lnc)
			{
				data << lnc->Texts[i][0];
				data << lnc->Texts[i][1];
			}
			else
			{
				data << pGossip->Texts[i].Text[0];
				data << pGossip->Texts[i].Text[1];
			}

			data << pGossip->Texts[i].Lang;
			data << uint32(0x00);		// Was prob.. but if you set it to 0 emotes work ;)
			for(uint32 e=0;e<6;e++)
				data << uint32(pGossip->Texts[i].Emote[e]);

			if(i!=7) data << uint32(0x00);	// don't append to last
		}
	} 
	else 
	{
		data << float(1.0f);		// unknown
		data << (char*)_player->GetSession()->LocalizedWorldSrv(70);
		data << (char*)_player->GetSession()->LocalizedWorldSrv(70);
		data << uint32(0x00);	// ?
		data << uint32(0x00);	// ?
		for(uint32 e=0;e<6;e++)
			data << uint32(0x00);

		for(int i=0;i<7;i++)
		{
			data << uint32(0x00);
			data << uint8(0x00) << uint8(0x00);
			data << uint32(0x00);	// ?
			data << uint32(0x00);	// ?
			for(uint32 e=0;e<6;e++)
				data << uint32(0x00);	// emote 1
		}
	}

	SendPacket(&data);
	return;
}

void WorldSession::HandleBinderActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;

	Creature *pC = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pC) return;

	SendInnkeeperBind(pC);
}

#define ITEM_ID_HEARTH_STONE 6948
#define BIND_SPELL_ID 3286

void WorldSession::SendInnkeeperBind(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(45);

	if(!_player->bHasBindDialogOpen)
	{
        OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);

		data.Initialize(SMSG_BINDER_CONFIRM);
		data << pCreature->GetGUID() << _player->GetZoneId();
		SendPacket(&data);

		_player->bHasBindDialogOpen = true;
		return;
	}

	// Add a hearthstone if they don't have one
	if(!_player->GetItemInterface()->GetItemCount(ITEM_ID_HEARTH_STONE, true))
	{
		// We don't have a hearthstone. Add one.
		if(_player->GetItemInterface()->CalculateFreeSlots(NULL) > 0)
		{
			Item *item = objmgr.CreateItem( ITEM_ID_HEARTH_STONE, _player);
			if(item==NULL)
			return;

			if( _player->GetItemInterface()->AddItemToFreeSlot(item) )
			{
				SlotResult * lr = _player->GetItemInterface()->LastSearchResult();
				SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,1);
			}
			else
				item->DeleteMe();
		}
	}

	_player->bHasBindDialogOpen = false;

	_player->SetBindPoint(_player->GetPositionX(),_player->GetPositionY(),_player->GetPositionZ(),_player->GetMapId(),_player->GetZoneId());

	data.Initialize(SMSG_BINDPOINTUPDATE);
	data << _player->GetBindPositionX() << _player->GetBindPositionY() << _player->GetBindPositionZ() << _player->GetBindMapId() << _player->GetBindZoneId();
	SendPacket( &data );

	data.Initialize(SMSG_PLAYERBOUND);
	data << pCreature->GetGUID() << _player->GetBindZoneId();
	SendPacket(&data);

    OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);

	data.Initialize( SMSG_SPELL_START );
	data << pCreature->GetNewGUID();
	data << pCreature->GetNewGUID();
	data << uint32(BIND_SPELL_ID);
	data << uint8(0);
	data << uint16(0);
	data << uint32(0);
	data << uint16(2);
	data << _player->GetGUID();
	_player->SendMessageToSet(&data, true);

	data.Initialize( SMSG_SPELL_GO );
	data << pCreature->GetNewGUID();
	data << pCreature->GetNewGUID();
	data << uint32(BIND_SPELL_ID);  // spellID
	data << uint8(0) << uint8(1);   // flags
	data << uint8(1);			   // amount of targets
	data << _player->GetGUID();
	data << uint8(0);
	data << uint16(2);
	data << _player->GetGUID();
	_player->SendMessageToSet( &data, true );
}

#undef ITEM_ID_HEARTH_STONE
#undef BIND_SPELL_ID

void WorldSession::SendSpiritHealerRequest(Creature* pCreature)
{
	WorldPacket data(SMSG_SPIRIT_HEALER_CONFIRM, 8);
	data << pCreature->GetGUID();
	SendPacket(&data);
}

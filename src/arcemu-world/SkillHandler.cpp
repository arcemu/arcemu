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

/************************************************************************/
/* Talent Anti-Cheat                                                    */
/************************************************************************/

/*
	mage - arcane - 81			
	mage - fire - 41
	mage - frost - 61

	rogue - assassination - 182
	rogue - combat - 181
	rogue - subelty - 183

	warlock - afflication - 302
	warlock - demonology - 303
	warlock - destruction - 301

	warrior - arms - 161
	warrior - fury - 163
	warrior - protection - 164

	shaman - elemental - 261
	shaman - enchantment - 263
	shaman - restoration - 262

	paladin - holy - 382
	paladin - protection - 383
	paladin - retribution - 381

	priest - dicipline - 201
	priest - holy - 202
	priest - shadow - 203

	hunter - beast - 361
	hunter - marksmanship - 363
	hunter - survival - 362

	druid - balance - 283
	druid - feral combat - 281
	druid - restoration - 282
*/

static const uint32 TalentTreesPerClass[DRUID+1][3] =  {
	{ 0, 0, 0 },		// NONE
	{ 161, 163, 164 },	// WARRIOR
	{ 382, 383, 381 },	// PALADIN
	{ 361, 363, 362 },	// HUNTER
	{ 182, 181, 183 },	// ROGUE
	{ 201, 202, 203 },	// PRIEST
	{ 0, 0, 0 },		// NONE
	{ 261, 263, 262 },	// SHAMAN
	{ 81, 41, 61 },		// MAGE
	{ 302, 303, 301 },	// WARLOCK
	{ 0, 0, 0 },		// NONE
	{ 283, 281, 282 },	// DRUID
};

void WorldSession::HandleLearnTalentOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
 	 
	uint32 talent_id, requested_rank, unk;
	unsigned int i;
	recv_data >> talent_id >> requested_rank >> unk;

	uint32 CurTalentPoints =  GetPlayer()->GetUInt32Value(PLAYER_CHARACTER_POINTS1);
	if(CurTalentPoints == 0)
		return;

	if (requested_rank > 4)
		return;

/*	unsigned int numRows = sTalentStore.GetNumRows();
	TalentEntry *talentInfo=NULL ;
	for (unsigned int i = 0; i < numRows; i++)		  // Loop through all talents.
	{
		TalentEntry *t= sTalentStore.LookupEntry( i );
		if(t->TalentID==talent_id)
		{
			talentInfo=t;
			break;
		}
	}*/
	TalentEntry * talentInfo = dbcTalent.LookupEntryForced(talent_id);
	if(!talentInfo)return;
  
	Player * player = GetPlayer();

	// Check if it requires another talent
	if (talentInfo->DependsOn > 0)
	{
		TalentEntry *depTalentInfo = NULL;
		/*for (unsigned int i = 0; i < numRows; i++)		  // Loop through all talents.
		{
			TalentEntry *t= sTalentStore.LookupEntry( i );
			if(t->TalentID==talentInfo->DependsOn)
			{
				depTalentInfo=t;
				break;
			}
		}*/
		depTalentInfo = dbcTalent.LookupEntryForced(talentInfo->DependsOn);
		bool hasEnoughRank = false;
		for (int i = talentInfo->DependsOnRank; i < 5; i++)
		{
			if (depTalentInfo->RankID[i] != 0)
			{
				if (player->HasSpell(depTalentInfo->RankID[i]))
				{
					hasEnoughRank = true;
					break;
				}
			}
		}
		if (!hasEnoughRank)
			return;
	}

	// Find out how many points we have in this field
	uint32 spentPoints = 0;

	uint32 tTree = talentInfo->TalentTree;
	uint32 cl = _player->getClass();

	for(i = 0; i < 3; ++i)
		if(tTree == TalentTreesPerClass[cl][i])
			break;

	if(i == 3)
	{
		// cheater!
		Disconnect();
		return;
	}

	if (talentInfo->Row > 0)
	{
		for (i = 0; i < dbcTalent.GetNumRows(); i++)		  // Loop through all talents.
		{
			// Someday, someone needs to revamp
			TalentEntry *tmpTalent = dbcTalent.LookupRow(i);
			if (tmpTalent)								  // the way talents are tracked
			{
				if (tmpTalent->TalentTree == tTree)
				{
					for (int j = 0; j < 5; j++)
					{
						if (tmpTalent->RankID[j] != 0)
						{
							if (player->HasSpell(tmpTalent->RankID[j]))
							{
								spentPoints += j + 1;
							//	break;
							}
						}
						else 
							break;
					}
				}
			}
		}
	}

	uint32 spellid = talentInfo->RankID[requested_rank];
	if( spellid == 0 )
	{
		sLog.outDetail("Talent: %u Rank: %u = 0", talent_id, requested_rank);
	}
	else
	{
		if(spentPoints < (talentInfo->Row * 5))			 // Min points spent
		{
			return;
		}

		if(requested_rank > 0)
		{
			if(talentInfo->RankID[requested_rank-1] && !_player->HasSpell(talentInfo->RankID[requested_rank-1]))
			{
				// cheater
				return;
			}
		}
		for (uint32 i=requested_rank; i<5; ++i)
			if (talentInfo->RankID[i] != 0 && GetPlayer()->HasSpell(talentInfo->RankID[i]))
				return; // cheater

		if(!(GetPlayer( )->HasSpell(spellid)))
		{
			GetPlayer( )->addSpell(spellid);			
	
			SpellEntry *spellInfo = dbcSpell.LookupEntry( spellid );	 
			
			if(requested_rank > 0 )
			{
				uint32 respellid = talentInfo->RankID[requested_rank-1];
				if(respellid)
				{
					_player->removeSpell(respellid, false, false, 0);
					_player->RemoveAura(respellid);
				}
			}

			if( (spellInfo->Attributes & ATTRIBUTES_PASSIVE || (spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL ||
															   spellInfo->Effect[1] == SPELL_EFFECT_LEARN_SPELL ||
															   spellInfo->Effect[2] == SPELL_EFFECT_LEARN_SPELL) 
				&& ( (spellInfo->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) == 0 || ( (spellInfo->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) && _player->GetSummon() ) ) )
				)
			{
				if( spellInfo->RequiredShapeShift && !( (uint32)1 << (_player->GetShapeShift()-1) & spellInfo->RequiredShapeShift ) )
				{
					// do nothing
				}
				else
				{
					Spell*sp=SpellPool.PooledNew();
					sp->Init(_player,spellInfo,true,NULL);
					SpellCastTargets tgt;
					tgt.m_unitTarget=_player->GetGUID();
					sp->prepare(&tgt);
				}
			}

			_player->SetUInt32Value(PLAYER_CHARACTER_POINTS1, CurTalentPoints-1);
		}
	}
}

void WorldSession::HandleUnlearnTalents( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint32 playerGold = GetPlayer()->GetUInt32Value( PLAYER_FIELD_COINAGE );
	uint32 price = GetPlayer()->CalcTalentResetCost(GetPlayer()->GetTalentResetTimes());

	if( playerGold < price ) return;

	GetPlayer()->SetTalentResetTimes(GetPlayer()->GetTalentResetTimes() + 1);
	GetPlayer()->SetUInt32Value( PLAYER_FIELD_COINAGE, playerGold - price );
	GetPlayer()->Reset_Talents();

/*	// Unlearn all talent spells
	WorldPacket data;
	std::list<uint32> *talentproto = GetPlayer()->getTalentproto();

	std::list<uint32>::iterator itr;
	for (itr = talentproto->begin(); itr != talentproto->end(); ++itr)
	{
		GetPlayer()->removeSpell((*itr));

		// Handled in removespell.
		//data.Initialize(SMSG_REMOVED_SPELL);
		//data << (*itr); 
		//SendPacket( &data );
	}
	talentproto->clear();
*/
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 skill_line;
	uint32 points_remaining=_player->GetUInt32Value(PLAYER_CHARACTER_POINTS2);
	recv_data >> skill_line;

	// Cheater detection
	// if(!_player->HasSkillLine(skill_line)) return;

	// Remove any spells within that line that the player has
	_player->RemoveSpellsFromLine(skill_line);
	
	// Finally, remove the skill line.
	_player->_RemoveSkillLine(skill_line);

	//added by Zack : This is probably wrong or already made elsewhere : restore skill learnability
	if(points_remaining==_player->GetUInt32Value(PLAYER_CHARACTER_POINTS2))
	{
		//we unlearned a skill so we enable a new one to be learned
		skilllineentry *sk=dbcSkillLine.LookupEntry(skill_line);
		if(!sk)
			return;
		if(sk->type==SKILL_TYPE_PROFESSION && points_remaining<2)
			_player->SetUInt32Value(PLAYER_CHARACTER_POINTS2,points_remaining+1);
	}
}

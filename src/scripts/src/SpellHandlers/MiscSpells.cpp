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

#include "Setup.h"

bool FrostWarding( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	if(!unitTarget)
		return false;

	uint32 spellId = s->GetProto()->Id;
	
	unitTarget->RemoveReflect( spellId, true );

	ReflectSpellSchool *rss = new ReflectSpellSchool;

	rss->chance = s->GetProto()->procChance;
	rss->spellId = s->GetProto()->Id;
	rss->require_aura_hash = SPELL_HASH_FROST_WARD;
	rss->school = SCHOOL_FROST;
	rss->infront = false;
	rss->charges = 0;
	
	unitTarget->m_reflectSpellSchool.push_back(rss);

	return true;
}

bool MoltenShields( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();
	
	if(!unitTarget)
		return false;
	
	unitTarget->RemoveReflect( s->GetProto()->Id, true);
	
	ReflectSpellSchool *rss = new ReflectSpellSchool;
	
	rss->chance = s->GetProto()->EffectBasePoints[0];
	rss->spellId = s->GetProto()->Id;
	rss->require_aura_hash = SPELL_HASH_FIRE_WARD;
	rss->school = SCHOOL_FIRE;
	rss->infront = false;
	rss->charges = 0;
	
	unitTarget->m_reflectSpellSchool.push_back(rss);

	return true;
}


bool ArcaneTorrentMana( uint32 i, Spell *s ){
	Unit *unitTarget = s->GetUnitTarget();

	// for each mana tap, gives you (2.17*level+9.136) mana
	if( !unitTarget || !unitTarget->isAlive() || !s->p_caster)
				return false;
	
	uint32 count = 0;

	for(uint32 x = MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; ++x)
		if(unitTarget->m_auras[x] && unitTarget->m_auras[x]->GetSpellId() == 28734)
		{
			unitTarget->m_auras[x]->Remove();
			++count;
		}
		
		uint32 gain = (uint32)(count * (2.17*s->p_caster->getLevel()+9.136));
		s->p_caster->Energize( unitTarget, 28730, gain, POWER_TYPE_MANA );

		return true;
}

bool Cannibalize( uint32 i, Spell *s ){
	if(!s->p_caster)
		return false;
	
	bool check = false;
	float rad = s->GetRadius(i);
	rad *= rad;
	
	for(Object::InRangeSet::iterator itr = s->p_caster->GetInRangeSetBegin(); itr != s->p_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsCreature())
		{
			if(static_cast<Creature *>((*itr))->getDeathState() == CORPSE)
			{
				CreatureInfo *cn = static_cast<Creature *>((*itr))->GetCreatureInfo();
				if( cn->Type == UNIT_TYPE_HUMANOID || cn->Type == UNIT_TYPE_UNDEAD )
				{
					if(s->p_caster->GetDistance2dSq((*itr)) < rad)
					{
						check = true;
						break;
					}
				}
			}
		}
	}
	
	if(check)
	{
		s->p_caster->cannibalize = true;
		s->p_caster->cannibalizeCount = 0;
		sEventMgr.AddEvent( s->p_caster, &Player::EventCannibalize, uint32(7), EVENT_CANNIBALIZE, 2000, 5, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		s->p_caster->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_CANNIBALIZE);
	}

	return true;
}

bool ArcaniteDragonLing( uint32 i, Spell *s ){
	s->u_caster->CastSpell( s->u_caster, 19804, true );
	return true;
}

bool MithrilMechanicalDragonLing( uint32 i, Spell *s ){
	s->u_caster->CastSpell( s->u_caster, 12749, true );
	return true;
}

bool MechanicalDragonLing( uint32 i, Spell *s ){
	s->u_caster->CastSpell( s->u_caster, 4073, true );
	return true;
}

bool GnomishBattleChicken( uint32 i, Spell *s ){
	s->u_caster->CastSpell( s->u_caster, 13166, true );
	return true;
}

bool GiftOfLife( uint32 i, Spell *s ){
	Player *playerTarget = s->GetPlayerTarget();

	if(!playerTarget)
		return false;
	
	SpellCastTargets tgt;
	tgt.m_unitTarget = playerTarget->GetGUID();
	SpellEntry * inf = dbcSpell.LookupEntry(23782);
	Spell * spe = new Spell( s->u_caster,inf,true,NULL);
	spe->prepare(&tgt);

	return true;
}

bool Give5kGold( uint32 i, Spell *s ){
	if( s->GetPlayerTarget() != NULL )
	{
		if( sWorld.GoldCapEnabled && ( s->GetPlayerTarget()->GetGold() + 50000000 ) > sWorld.GoldLimit )
		{
			s->GetPlayerTarget()->SetGold( sWorld.GoldLimit );
			s->GetPlayerTarget()->GetItemInterface()->BuildInventoryChangeError( NULL, NULL, INV_ERR_TOO_MUCH_GOLD );
		}
		else
		{
			s->GetPlayerTarget()->ModGold( 50000000 );
		}
	}else
		return false;

	return true;
}

bool NorthRendInscriptionResearch( uint32 i, Spell *s ){
	// http://www.wowwiki.com/Minor_Inscription_Research :
	// Minor Inscription Research is taught at 75 skill in Inscription.
	// When you perform this research, you have a very high chance of learning a new minor glyph recipe.
	// The chance to discover a new minor glyph is independent of your level, Inscription skill, and how many minor glyphs you already know.
	// The recipe has a 20-hour cooldown, similar to alchemical transmutes.
	
	// What is a "very high chance" ?  90% ?
	float chance = 90.0f;
	if( Rand(chance) )
	{
		// Type 0 = Major, 1 = Minor
		uint32 glyphType = ( s->GetProto()->Id == 61177) ? 0 : 1;
		skilllinespell* sls;
		uint32 num_sl = dbcSkillLineSpell.GetNumRows();
		std::vector<uint32> discoverableGlyphs;
		
		// how many of these are the right type (minor/major) of glyph, and learnable by the player
		for( uint32 idx = 0; idx < num_sl; ++idx )
		{
			sls = dbcSkillLineSpell.LookupRow(idx);
			if( sls->skilline == SKILL_INSCRIPTION && sls->next == 0 )
			{
				SpellEntry* se1 = dbcSpell.LookupEntryForced(sls->spell);
				if( se1 && se1->Effect[0] == SPELL_EFFECT_CREATE_ITEM )
				{
					ItemPrototype* itm = ItemPrototypeStorage.LookupEntry(se1->EffectItemType[0]);
					if( itm && (itm->Spells[0].Id != 0) )
					{
						SpellEntry* se2 = dbcSpell.LookupEntryForced( itm->Spells[0].Id );
						if( se2 && se2->Effect[0] == SPELL_EFFECT_USE_GLYPH )
						{
							GlyphPropertyEntry* gpe = dbcGlyphProperty.LookupEntryForced(se2->EffectMiscValue[0]);
							if( gpe && gpe->Type == glyphType )
							{
								if( !s->p_caster->HasSpell(sls->spell) )
								{
									discoverableGlyphs.push_back(sls->spell);
								}
							}
						}
					}
				}
			}
		}
		
		if( discoverableGlyphs.size() > 0 )
		{
			uint32 newGlyph = discoverableGlyphs.at( uint32(rand()) % discoverableGlyphs.size() );
			s->p_caster->addSpell(newGlyph);
		}
	}

	return true;
}

bool DeadlyThrowInterrupt( uint32 i, Aura *a, bool apply ){

	if( !apply )
		return true;

	Unit *m_target = a->GetTarget();

	uint32 school = 0;

	if( m_target->GetCurrentSpell() ){
		school = m_target->GetCurrentSpell()->GetProto()->School;
	}
	
	m_target->InterruptSpell();
	m_target->SchoolCastPrevent[ school ]= 3000 + getMSTime();

	return true;
}

bool WaitingToResurrect( uint32 i, Aura *a, bool apply ){
	Unit *u_target = a->GetTarget();

	if( !u_target->IsPlayer() )
		return true;

	Player *p_target = TO_PLAYER( u_target );

	if( apply )		// already applied in opcode handler
		return true;

	uint64 crtguid = p_target->m_areaSpiritHealer_guid;

	Creature* pCreature = p_target->IsInWorld() ? p_target->GetMapMgr()->GetCreature(GET_LOWGUID_PART(crtguid)) : NULL;

	if( pCreature== NULL || p_target->m_bg == NULL )
		return true;
	
	p_target->m_bg->RemovePlayerFromResurrect( p_target, pCreature );

	return true;
}

bool NegativeCrap( uint32 i, Aura *a, bool apply ){
	if( apply )
		a->SetNegative();

	return true;
}

bool DecayFlash(uint32 i, Aura* pAura, bool apply)
{
	if( apply && pAura->GetTarget()->IsPlayer() )
	{
		Player* p_target = TO_PLAYER( pAura->GetTarget() );
		p_target->SetShapeShift( 10 );//Tharon'ja Skeleton
		p_target->SetDisplayId(9784 );
	}
    return true;
}

bool ReturnFlash(uint32 i, Aura* pAura, bool apply)
{
    if( apply && pAura->GetTarget()->IsPlayer() )
	{
		Player* p_target = TO_PLAYER( pAura->GetTarget() );
		p_target->SetDisplayId(p_target->GetNativeDisplayId() );
		p_target->m_ShapeShifted = 0;
		p_target->SetShapeShift( 0 );
	}
    return true;
}

bool EatenRecently(uint32 i, Aura* pAura, bool apply)
{
	if ( pAura == NULL )
		return true;

	Unit* caster = pAura->GetUnitCaster();
	if(caster || caster->IsPlayer() )
		return true;

	Creature* NetherDrake = TO_CREATURE( caster );
	
	if (NetherDrake == NULL)
		return true;

	if ( apply )
	{
		NetherDrake->GetAIInterface()->SetAllowedToEnterCombat(false);
		NetherDrake->Emote(EMOTE_ONESHOT_EAT);
	}
	else
	{
		NetherDrake->GetAIInterface()->SetAllowedToEnterCombat(true);
		NetherDrake->GetAIInterface()->m_moveFly = true;
		NetherDrake->GetAIInterface()->MoveTo(NetherDrake->GetSpawnX(), NetherDrake->GetSpawnY(), NetherDrake->GetSpawnZ(), NetherDrake->GetSpawnO());
	}
	return true;
}

bool Temper(uint32 i, Spell *pSpell)
{
	if ( pSpell->u_caster == NULL )
		return true;

	Unit* pHated = pSpell->u_caster->GetAIInterface()->GetMostHated();

	MapScriptInterface* pMap = pSpell->u_caster->GetMapMgr()->GetInterface();
	Creature *pCreature1 = pMap->SpawnCreature( 28695, 1335.296265f, -89.237503f, 56.717800f, 1.994538f, true, true, 0, 0, 1 );
	if ( pCreature1 )
		pCreature1->GetAIInterface()->AttackReaction( pHated, 1 );

	Creature *pCreature2 = pMap->SpawnCreature( 28695, 1340.615234f, -89.083313f, 56.717800f, 0.028982f, true, true, 0, 0, 1 );
	if ( pCreature2 )
		pCreature2->GetAIInterface()->AttackReaction( pHated, 1 );

	return true;
};

//Chaos blast dummy effect
bool ChaosBlast(uint32 i, Spell*  pSpell)
{
	if ( pSpell->u_caster == NULL )
		return true;

	pSpell->u_caster->CastSpell( pSpell->GetUnitTarget(), 37675, true );
	return true;
}

bool Dummy_Solarian_WrathOfTheAstromancer(uint32 pEffectIndex, Spell* pSpell)
{
	Unit* Caster = pSpell->u_caster;
	if( !Caster ) return true;

	Unit* Target = Caster->GetAIInterface()->getNextTarget();
	if( !Target ) return true;

	SpellEntry* SpellInfo = dbcSpell.LookupEntry( 42787 );
	if( !SpellInfo ) return true;

	//Explode bomb after 6sec
	sEventMgr.AddEvent(Target, &Unit::EventCastSpell, Target, SpellInfo, EVENT_UNK, 6000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	return true;
}

bool PreparationForBattle(uint32 i, Spell* pSpell)
{
	if ( pSpell->p_caster == NULL )
	return true;

	Player* pPlayer = pSpell->p_caster;
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 12842 );
	if ( pQuest != NULL )
	{
		if ( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
			pQuest->SendQuestComplete();
		}
	}
	return true;
};

#define CN_CRYSTAL_SPIKE			27099
#define CRYSTAL_SPIKES				47958
#define CRYSTAL_SPIKES_H			57082

bool CrystalSpikes(uint32 i, Spell *pSpell)
{
	if ( pSpell->u_caster == NULL)
		return true;

	Unit* pCaster = pSpell->u_caster;

	for( int i = 1; i < 6; ++i )
	{
		pCaster->GetMapMgr()->GetInterface()->SpawnCreature( CN_CRYSTAL_SPIKE, pCaster->GetPositionX() + ( 3 * i ) + rand()%3 , pCaster->GetPositionY() + ( 3 * i ) + rand()%3 , pCaster->GetPositionZ(), pCaster->GetOrientation(), true, false, NULL, NULL );
	};
	
	for( int i = 1; i < 6; ++i )
	{
		pCaster->GetMapMgr()->GetInterface()->SpawnCreature( CN_CRYSTAL_SPIKE, pCaster->GetPositionX() - ( 3 * i ) - rand()%3 , pCaster->GetPositionY() + ( 3 * i ) + rand()%3 , pCaster->GetPositionZ(), pCaster->GetOrientation(), true, false, NULL, NULL );
	};
	
	for( int i = 1; i < 6; ++i )
	{
		pCaster->GetMapMgr()->GetInterface()->SpawnCreature( CN_CRYSTAL_SPIKE, pCaster->GetPositionX() + ( 3 * i ) + rand()%3 , pCaster->GetPositionY() - ( 3 * i ) - rand()%3 , pCaster->GetPositionZ(), pCaster->GetOrientation(), true, false, NULL, NULL );
	};
	
	for( int i = 1; i < 6; ++i )
	{
		pCaster->GetMapMgr()->GetInterface()->SpawnCreature( CN_CRYSTAL_SPIKE, pCaster->GetPositionX() - ( 3 * i ) - rand()%3 , pCaster->GetPositionY() - ( 3 * i ) - rand()%3 , pCaster->GetPositionZ(), pCaster->GetOrientation(), true, false, NULL, NULL );
	};

	return true;
}


////////////////////////////////////////////////////////////////
//Listening To Music scripted spell effect
//( SpellId 50499 )
//
//Precondition(s)
//  Casted by Player
//
//Effect(s)
//  Makes the player cast "Listening to Music"
//
//
////////////////////////////////////////////////////////////////
bool ListeningToMusicParent( uint32 i, Spell *s ){
	if( s->p_caster == NULL )
		return true;

	s->p_caster->CastSpell( s->p_caster, 50493, true );

	return true;
}

void SetupMiscSpellhandlers( ScriptMgr *mgr ){
	mgr->register_dummy_spell( 11189, &FrostWarding );
	mgr->register_dummy_spell( 28332, &FrostWarding );

	mgr->register_dummy_spell( 11094, &MoltenShields );
	mgr->register_dummy_spell( 13043, &MoltenShields );

	mgr->register_dummy_spell( 28730, &ArcaneTorrentMana );

	mgr->register_dummy_spell( 20577, &Cannibalize );

	mgr->register_dummy_spell( 23074, &ArcaniteDragonLing );

	mgr->register_dummy_spell( 23075, &MithrilMechanicalDragonLing );

	mgr->register_dummy_spell( 23076, &MechanicalDragonLing );

	mgr->register_dummy_spell( 23133, &GnomishBattleChicken );

	mgr->register_dummy_spell( 23725, &GiftOfLife );

	mgr->register_script_effect( 46642, &Give5kGold );

	mgr->register_script_effect( 61288, &NorthRendInscriptionResearch );

	mgr->register_script_effect( 61177, &NorthRendInscriptionResearch );

	mgr->register_dummy_aura( 32748, &DeadlyThrowInterrupt );

	mgr->register_dummy_aura( 2584, &WaitingToResurrect );

	uint32 negativecrapids[] = {
		26013,
		41425,
		0
	};
	mgr->register_dummy_aura( negativecrapids, &NegativeCrap );

	mgr->register_dummy_aura(49356, &DecayFlash);

	mgr->register_dummy_aura(53463, &ReturnFlash);

	mgr->register_dummy_aura(38502, &EatenRecently);
	
	mgr->register_dummy_spell( 52238, &Temper );

	mgr->register_dummy_spell( 37674, &ChaosBlast);

	mgr->register_dummy_spell( 42783, &Dummy_Solarian_WrathOfTheAstromancer);
	
	mgr->register_dummy_spell(53341, &PreparationForBattle);
	mgr->register_dummy_spell(53343, &PreparationForBattle);

	mgr->register_script_effect(CRYSTAL_SPIKES, &CrystalSpikes);
	mgr->register_script_effect(CRYSTAL_SPIKES_H, &CrystalSpikes);

	mgr->register_script_effect( 50499, &ListeningToMusicParent );

}


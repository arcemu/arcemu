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
		if((*itr)->GetTypeId() == TYPEID_UNIT)
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


}


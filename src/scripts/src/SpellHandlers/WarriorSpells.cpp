/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

bool Execute(uint32 i, Spell* pSpell)
{
	if( pSpell->p_caster == NULL || pSpell->GetUnitTarget() == NULL)
		return true;
	Player* Caster = pSpell->p_caster;
	Unit* Target = pSpell->GetUnitTarget();
	uint32 rage = Caster->GetPower( POWER_TYPE_RAGE );
	if(Caster->HasAura(58367)) // Glyph of Execution: Your Execute ability deals damage as if you had 10 additional rage.
		rage += 10; 
	uint32 toadd = 0;
	int32 dmg = 0.0f;
	uint32 multiple[] = 
	{
		0,
		3,
		6,
		9,
		12,
		15,
		18,
		21,
		30,
		38,
	};
	if(rage >= 30)
		toadd = (multiple[pSpell->GetProto()->RankNumber] * 30);
	else
		toadd = (multiple[pSpell->GetProto()->RankNumber] * rage);

	dmg = pSpell->CalculateEffect(i, pSpell->GetUnitTarget());
	dmg += (Caster->GetAttackPower() * 0.2f);
	dmg += toadd;

	Caster->Strike(Target, 0, pSpell->GetProto(), 0, 0, dmg, false, false);
	return true;
}

bool Vigilance(uint32 i, Spell* pSpell)
{
	if(!pSpell->p_caster)
		return true;

	pSpell->p_caster->ClearCooldownForSpell(355); // Taunt
	return true;
}

bool DamageShield(uint32 i, Aura *pAura, bool apply)
{
	Unit *target = pAura->GetTarget();

	if (apply)
		target->AddProcTriggerSpell(59653, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_BLOCK_VICTIM, 0, NULL, NULL);
	else
		target->RemoveProcTriggerSpell(59653, pAura->m_casterGuid);

	return true;
}

bool HeroicFury( uint32 i, Spell *s ){
	Player * p_caster = s->p_caster;

	if( !p_caster )
		return false;
	
	if( p_caster->HasSpell(20252) )
		p_caster->ClearCooldownForSpell(20252);
	
	for(uint32 x= MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; ++x)
	{
		if(p_caster->m_auras[x])
		{
			for(uint32 y = 0; y < 3; ++y)
			{
				switch(p_caster->m_auras[x]->GetSpellProto()->EffectApplyAuraName[y])
				{
				case SPELL_AURA_MOD_ROOT:
				case SPELL_AURA_MOD_DECREASE_SPEED:
					p_caster->m_auras[x]->Remove();
					goto out;
					break;}
				
				continue;
out:
				break;
			}
		}
	}

	return true;
}

bool Charge( uint32 i, Spell *s ){
	if(!s->u_caster)
		return false;
	
	uint32 rage_to_gen = s->GetProto()->EffectBasePoints[i] + 1;
	if( s->p_caster )
	{
		for(set<uint32>::iterator itr = s->p_caster->mSpells.begin(); itr != s->p_caster->mSpells.end(); ++itr)
		{
			if(*itr == 12697)
				rage_to_gen += 100;
			
			if(*itr == 12285)
				rage_to_gen += 50;
		}
	}
	
	// Add the rage to the caster
	s->u_caster->ModPower( POWER_TYPE_RAGE, rage_to_gen );
	
	return true;
}

bool LastStand( uint32 i, Spell *s ){
	Player *playerTarget = s->GetPlayerTarget();

	if(!playerTarget)
		return false;
	
	SpellCastTargets tgt;
	tgt.m_unitTarget = playerTarget->GetGUID();
	
	SpellEntry * inf =dbcSpell.LookupEntry(12976);
	Spell * spe = new Spell( s->u_caster,inf,true,NULL);
	spe->prepare(&tgt);

	return true;
}

void SetupWarriorSpells(ScriptMgr * mgr)
{
	uint32 ExecuteIds[] = 
	{
		5308,  // Rank 1
		20658, // Rank 2
		20660, // Rank 3
		20661, // Rank 4
		20662, // Rank 5
		25234, // Rank 6
		25236, // Rank 7
		47470, // Rank 8
		47471, // Rank 9
		0,
	};
    mgr->register_dummy_spell(ExecuteIds, &Execute);
	mgr->register_dummy_spell(50725, &Vigilance);

	mgr->register_dummy_aura(58872, &DamageShield);
	mgr->register_dummy_aura(58874, &DamageShield);

	mgr->register_dummy_spell( 60970, &HeroicFury );

	mgr->register_dummy_spell( 100, &Charge );
	mgr->register_dummy_spell( 6178, &Charge );
	mgr->register_dummy_spell( 11578, &Charge );

	mgr->register_dummy_spell( 12975, &LastStand );
}

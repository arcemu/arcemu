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
}

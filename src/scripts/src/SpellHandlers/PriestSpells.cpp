/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
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

bool Penance(uint32 i, Spell* pSpell)
{
	if(!pSpell->p_caster || !pSpell->p_caster->isAlive() ||
	        !pSpell->GetUnitTarget() || !pSpell->GetUnitTarget()->isAlive())
		return true;

	Unit* target = pSpell->GetUnitTarget();
	Player* player = pSpell->p_caster;

	// index 0 contains the spell for the first tick, index 1 is the peroidic cast spell.
	uint32 hostileSpell[] = {0, 0};
	uint32 friendlySpell[] = {0, 0};

	switch(pSpell->GetProto()->Id)
	{
		case 47540: //Rank 1
			hostileSpell[0] = 47666;
			hostileSpell[1] = 47758;

			friendlySpell[0] = 47750;
			friendlySpell[1] = 47757;
			break;
		case 53005:
			hostileSpell[0] = 52998;
			hostileSpell[1] = 53001;

			friendlySpell[0] = 52983;
			friendlySpell[1] = 52986;
			break;
		case 53006:
			hostileSpell[0] = 52999;
			hostileSpell[1] = 53002;

			friendlySpell[0] = 52984;
			friendlySpell[1] = 52987;
			break;
		case 53007:
			hostileSpell[0] = 53000;
			hostileSpell[1] = 53003;

			friendlySpell[0] = 52985;
			friendlySpell[1] = 52988;
			break;
	}

	if(isAttackable(player, target))   // Do holy damage
	{
		// First tick is instant.
		player->CastSpell(target, hostileSpell[0], true);
		player->CastSpell(target, hostileSpell[1], false);
	}
	else // Heal
	{
		player->CastSpell(target, friendlySpell[0], true);
		player->CastSpell(target, friendlySpell[1], false);
	}
	return true;
}

bool DivineAegis(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
		target->AddProcTriggerSpell(47753, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_SPELL_CRIT_HIT, 0, NULL, NULL);
	else
		target->RemoveProcTriggerSpell(47753, pAura->m_casterGuid);

	return true;
}

bool ImprovedDevouringPlague(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
	{
		static uint32 classMask[3] = { 0x2000000, 0, 0 };
		target->AddProcTriggerSpell(63675, pAura->GetSpellProto()->Id, pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_CAST_SPELL, 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(63675, pAura->m_casterGuid);

	return true;
}

bool VampiricEmbrace(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
		target->AddProcTriggerSpell(15290, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_ANY_HOSTILE_ACTION | PROC_TARGET_SELF, 0, NULL, NULL);
	else
		target->RemoveProcTriggerSpell(15290, pAura->m_casterGuid);

	return true;
}

bool VampiricTouch(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	switch(i)
	{
		case 0:
			if(apply)
				target->AddProcTriggerSpell(64085, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_PRE_DISPELL_AURA_VICTIM | PROC_TARGET_SELF, 0, NULL, NULL);
			else
				target->RemoveProcTriggerSpell(64085, pAura->m_casterGuid);
			break;

		case 2:
			if(apply)
				target->AddProcTriggerSpell(34919, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_SPELL_HIT_VICTIM, 0, NULL, NULL);
			else
				target->RemoveProcTriggerSpell(34919, pAura->m_casterGuid);
			break;
	}

	return true;
}

bool EmpoweredRenew(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
	{
		static uint32 classMask[3] = { 0x40, 0, 0 };
		target->AddProcTriggerSpell(63544, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_CAST_SPELL, 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(63544, pAura->m_casterGuid);

	return true;
}

bool ImprovedMindBlast(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
	{
		static uint32 classMask[3] = { 0x2000, 0, 0 };
		target->AddProcTriggerSpell(48301, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_SPELL_HIT, 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(48301, pAura->m_casterGuid);

	return true;
}

bool PainAndSufferingAura(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
	{
		static uint32 classMask[3] = { 0, 0, 0x40 };
		target->AddProcTriggerSpell(47948, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_CAST_SPELL, 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(47948, pAura->m_casterGuid);

	return true;
}

bool PainAndSufferingProc(uint32 i, Spell* pSpell)
{
	Player* caster = pSpell->p_caster;
	if(caster == NULL)
		return true;

	Unit* target = pSpell->GetUnitTarget();
	if(target == NULL)
		return true;

	Aura* aura = target->FindAuraByNameHash(SPELL_HASH_SHADOW_WORD__PAIN, caster->GetGUID());
	if(aura == NULL)
		return true;

	// Set new aura's duration, reset event timer and set client visual aura
	aura->SetDuration(aura->GetDuration());
	sEventMgr.ModifyEventTimeLeft(aura, EVENT_AURA_REMOVE, aura->GetDuration());
	target->ModVisualAuraStackCount(aura, 0);

	return true;
}

bool BodyAndSoul(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
	{
		static uint32 classMask[3] = { 0, 1, 0 };
		target->AddProcTriggerSpell(64134, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetModAmount(i), PROC_ON_CAST_SPELL | PROC_TARGET_SELF, 0, NULL, classMask);
	}
	else
		target->RemoveProcTriggerSpell(64134, pAura->m_casterGuid);

	return true;
}

bool PrayerOfMendingAura(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
		target->AddProcTriggerSpell(pAura->GetSpellProto(), pAura->GetSpellProto(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_ANY_DAMAGE_VICTIM, pAura->GetSpellProto()->procCharges, NULL, NULL);
	else
	{
		int32 count = target->GetAuraStackCount(pAura->GetSpellId());
		if(count == 1)
			target->RemoveProcTriggerSpell(pAura->GetSpellId(), pAura->m_casterGuid);
	}

	return true;
}

void SetupPriestSpells(ScriptMgr* mgr)
{
	uint32 PenanceIds[] =
	{
		47540, // Rank 1
		53005, // Rank 2
		53006, // Rank 3
		53007, // Rank 4
		0,
	};
	mgr->register_dummy_spell(PenanceIds, &Penance);

	uint32 DivineAegisIds[] = { 47509, 47511, 47515, 0 };
	mgr->register_dummy_aura(DivineAegisIds, &DivineAegis);

	uint32 ImprovedDevouringPlagueIds[] = { 63625, 63626, 63627, 0 };
	mgr->register_dummy_aura(ImprovedDevouringPlagueIds, &ImprovedDevouringPlague);

	mgr->register_dummy_aura(15286, &VampiricEmbrace);

	uint32 VampiricTouchIds[] = { 34914, 34916, 34917, 48159, 48160, 0 };
	mgr->register_dummy_aura(VampiricTouchIds, &VampiricTouch);

	uint32 EmpoweredRenewIds[] = { 63534, 63542, 63543, 0 };
	mgr->register_dummy_aura(EmpoweredRenewIds, &EmpoweredRenew);

	uint32 ImprovedMindBlastIds[] = { 15273, 15312, 15313, 15314, 15316, 0 };
	mgr->register_dummy_aura(ImprovedMindBlastIds, &ImprovedMindBlast);

	uint32 PainAndSufferingAuraIds[] = { 47580, 47581, 47582, 0 };
	mgr->register_dummy_aura(PainAndSufferingAuraIds, &PainAndSufferingAura);

	mgr->register_script_effect(47948, &PainAndSufferingProc);

	mgr->register_dummy_aura(64127, &BodyAndSoul);
	mgr->register_dummy_aura(64129, &BodyAndSoul);

	uint32 PrayerOfMendingAuraIds[] = { 41635, 48110, 48111, 0 };
	mgr->register_dummy_aura(PrayerOfMendingAuraIds, &PrayerOfMendingAura);
}

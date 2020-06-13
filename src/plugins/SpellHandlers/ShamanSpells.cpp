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

/************************************************************************/
/* Spell Defs                                                           */
/************************************************************************/

bool FlametongueWeaponPassive(uint32 i, Aura* pAura, bool apply)
{
	Unit* target = pAura->GetTarget();

	if(apply)
	{
		// target is always a player
		Item* item = TO_PLAYER(target)->GetItemInterface()->GetItemByGUID(pAura->itemCasterGUID);
		target->AddProcTriggerSpell(10444, pAura->GetSpellProto()->Id, pAura->m_casterGuid, pAura->GetSpellProto()->procChance, PROC_ON_MELEE_ATTACK, 0, NULL, NULL, item);
	}
	else
		target->RemoveProcTriggerSpell(10444, pAura->m_casterGuid, pAura->itemCasterGUID);

	return true;
}

bool SkyShatterRegalia(uint32 i, Spell* s)
{
	// Shaman - Skyshatter Regalia - Two Piece Bonus
	// it checks for earth, air, water, fire totems and triggers Totemic Mastery spell 38437.

	if(!s->p_caster)
		return false;

	if(s->p_caster->summonhandler.HasSummonInSlot(0) &&
	        s->p_caster->summonhandler.HasSummonInSlot(1) &&
	        s->p_caster->summonhandler.HasSummonInSlot(2) &&
	        s->p_caster->summonhandler.HasSummonInSlot(3))
	{
		Aura* aur = sSpellFactoryMgr.NewAura(dbcSpell.LookupEntry(38437), 5000, s->p_caster, s->p_caster, true);

		for(uint32 j = 0; j < 3; j++)
			aur->AddMod(aur->GetSpellProto()->EffectApplyAuraName[j], aur->GetSpellProto()->EffectBasePoints[j] + 1, aur->GetSpellProto()->EffectMiscValue[j], j);

		s->p_caster->AddAura(aur);
	}

	return true;
}

bool ManaTide(uint32 i, Spell* s)
{
	Unit* unitTarget = s->GetUnitTarget();

	if(unitTarget == NULL || unitTarget->IsDead() || unitTarget->getClass() == WARRIOR || unitTarget->getClass() == ROGUE)
		return false;

	uint32 gain = (uint32)(unitTarget->GetMaxPower(POWER_TYPE_MANA) * 0.06);
	unitTarget->Energize(unitTarget, 16191, gain, POWER_TYPE_MANA);

	return true;
}

bool EarthShieldDummyAura(uint32 i, Aura* pAura, bool apply)
{
	Unit* m_target = pAura->GetTarget();

	if(apply)
		m_target->AddProcTriggerSpell(379, pAura->GetSpellId(), pAura->m_casterGuid, pAura->GetSpellProto()->procChance, pAura->GetSpellProto()->procFlags & ~PROC_ON_SPELL_LAND_VICTIM, pAura->GetSpellProto()->procCharges, NULL, NULL);
	else if(m_target->GetAuraStackCount(pAura->GetSpellId()) == 1)
		m_target->RemoveProcTriggerSpell(379, pAura->m_casterGuid);

	return true;
}

bool Reincarnation(uint32 i, Aura* a, bool apply)
{
	Unit* u_target = a->GetTarget();

	if(!u_target->IsPlayer())
		return true;

	Player* p_target = TO_PLAYER(u_target);

	if(apply)
		p_target->bReincarnation = true;
	else
		p_target->bReincarnation = false;

	return true;
}

void SetupShamanSpells(ScriptMgr* mgr)
{
	uint32 FlametongueWeaponPassiveIds[] = { 10400, 15567, 15568, 15569, 16311, 16312, 16313, 58784, 58791, 58792, 0 };
	mgr->register_dummy_aura(FlametongueWeaponPassiveIds, &FlametongueWeaponPassive);

	mgr->register_dummy_spell(38443, &SkyShatterRegalia);

	mgr->register_dummy_spell(39610, &ManaTide);

	uint32 earthshielddummyauraids[] =
	{
		974,
		32593,
		32594,
		49283,
		49284,
		0
	};
	mgr->register_dummy_aura(earthshielddummyauraids, &EarthShieldDummyAura);

	mgr->register_dummy_aura(20608, &Reincarnation);
}

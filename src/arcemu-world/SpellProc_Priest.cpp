/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

class ImprovedSpiritTapSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(ImprovedSpiritTapSpellProc);

		void Init(Object* obj)
		{
			mProcFlags = PROC_ON_SPELL_CRIT_HIT;
		}

		uint32 CalcProcChance(Unit* victim, SpellEntry* CastingSpell)
		{
			if(CastingSpell == NULL)
				return 0;

			if(CastingSpell->NameHash == SPELL_HASH_MIND_FLAY)
				return 50;

			if(CastingSpell->NameHash == SPELL_HASH_MIND_BLAST || CastingSpell->NameHash == SPELL_HASH_SHADOW_WORD__DEATH)
				return 100;

			return 0;
		}
};

class HolyConcentrationSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(HolyConcentrationSpellProc);

		void Init(Object* obj)
		{
			mProcFlags = PROC_ON_SPELL_CRIT_HIT;
			mProcClassMask[0] = 0x1800;
			mProcClassMask[1] = 0x4;
			mProcClassMask[2] = 0x1000;
		}
};

class DivineAegisSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(DivineAegisSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			if(CastingSpell == NULL)
				return true;

			if(! CastingSpell->HasEffect(SPELL_EFFECT_HEAL))
				return true;

			dmg_overwrite[0] = dmg * (mOrigSpell->EffectBasePoints[0] + 1) / 100;

			return false;
		}
};

class ImprovedDevouringPlagueSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(ImprovedDevouringPlagueSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// Get dmg amt for 1 tick
			dmg = CastingSpell->EffectBasePoints[0] + 1;

			// Get total ticks
			int ticks = GetDuration(dbcSpellDuration.LookupEntry(CastingSpell->DurationIndex)) / CastingSpell->EffectAmplitude[0];

			dmg_overwrite[0] = dmg * ticks * (mOrigSpell->EffectBasePoints[0] + 1) / 100;

			return false;
		}
};

class VampiricEmbraceSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(VampiricEmbraceSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// Only proc for damaging shadow spells
			if(CastingSpell->School != SCHOOL_SHADOW || ! IsDamagingSpell(CastingSpell))
				return true;

			// Only proc for single target spells
			if(!(HasTargetType(CastingSpell, EFF_TARGET_SINGLE_ENEMY) || HasTargetType(CastingSpell, EFF_TARGET_SELECTED_ENEMY_CHANNELED)))
				return true;

			dmg_overwrite[0] = dmg;
			dmg_overwrite[1] = dmg;

			return false;
		}
};

class VampiricTouchEnergizeSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(VampiricTouchEnergizeSpellProc);

		void Init(Object* obj)
		{
			mReplenishmentSpell = dbcSpell.LookupEntryForced(57669);
		}

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// Check for Mind Blast hit from this proc caster
			if(CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_MIND_BLAST || mCaster != victim->GetGUID())
				return true;

			// Cast Replenishment
			victim->CastSpell(victim, mReplenishmentSpell, true);

			return true;
		}

	private:
		SpellEntry* mReplenishmentSpell;
};

class VampiricTouchDispelDamageSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(VampiricTouchDispelDamageSpellProc);

		void Init(Object* obj)
		{
			mDispelDmg = 8 * (mOrigSpell->EffectBasePoints[1] + 1);
		}

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// For PROC_ON_PRE_DISPELL_AURA_VICTIM, parameter dmg has aur->GetSpellId()
			SpellEntry* sp = dbcSpell.LookupEntryForced(dmg);

			if(CastingSpell == NULL || sp == NULL || sp->NameHash != SPELL_HASH_VAMPIRIC_TOUCH)
				return true;

			dmg_overwrite[0] = mDispelDmg;

			return false;
		}

	private:
		int32 mDispelDmg;
};

class EmpoweredRenewSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(EmpoweredRenewSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// Get heal amt for 1 tick
			dmg = CastingSpell->EffectBasePoints[0] + 1;

			// Get total ticks
			int ticks = GetDuration(dbcSpellDuration.LookupEntry(CastingSpell->DurationIndex)) / CastingSpell->EffectAmplitude[0];

			// Total periodic effect is a single tick amount multiplied by number of ticks
			dmg_overwrite[0] = dmg * ticks * (mOrigSpell->EffectBasePoints[0] + 1) / 100;

			return false;
		}

		void CastSpell(Unit* victim, SpellEntry* CastingSpell, int* dmg_overwrite)
		{
			SpellCastTargets targets;
			targets.m_unitTarget = victim->GetGUID();

			Spell* spell = sSpellFactoryMgr.NewSpell(mTarget, mSpell, true, NULL);
			spell->forced_basepoints[0] = dmg_overwrite[0];
			spell->forced_basepoints[1] = dmg_overwrite[1];
			spell->forced_basepoints[2] = dmg_overwrite[2];
			spell->ProcedOnSpell = CastingSpell;

			spell->prepare(&targets);
		}
};

class ImprovedMindBlastSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(ImprovedMindBlastSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// If spell is not Mind Blast (by SpellGroupType) or player is not on shadowform, don't proc
			if(!(CastingSpell->SpellGroupType[0] & mProcClassMask[0] && mTarget->IsPlayer() && TO_PLAYER(mTarget)->GetShapeShift() == FORM_SHADOW))
				return true;

			return false;
		}
};

class BodyAndSoulDummySpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(BodyAndSoulDummySpellProc);

		bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			if(victim != NULL && mTarget->GetGUID() == victim->GetGUID())
				return true;

			return false;
		}
};

class BodyAndSoulSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(BodyAndSoulSpellProc);

		void Init(Object* obj)
		{
			mProcFlags = PROC_ON_CAST_SPELL;
			mProcClassMask[0] = 1;
		}
};

class MiserySpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(MiserySpellProc);

		void Init(Object* obj)
		{
			mProcFlags = PROC_ON_CAST_SPELL;
			mProcClassMask[0] = 0x8000;
			mProcClassMask[1] = 0x400;
			mProcClassMask[2] = 0x40;
		}
};

class PrayerOfMendingProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(PrayerOfMendingProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			Aura* aura = mTarget->FindAura(mSpell->Id);
			if(aura == NULL)
				return true;

			Unit* caster = TO_PLAYER(aura->GetCaster());
			if(caster == NULL)
			{
				mTarget->RemoveAuraByNameHash(mSpell->NameHash);
				return true;
			}

			int32 value = aura->GetModAmount(0);

			caster->CastSpell(mTarget, 33110, value, true);

			int32 count = mTarget->GetAuraStackCount(mSpell->Id);

			if(count <= 1)
				return true;

			Player* plr = TO_PLAYER(mTarget);
			Group* grp = plr->GetGroup();

			if(grp == NULL)
				return true;

			Player* new_plr = grp->GetRandomPlayerInRangeButSkip(plr, 40.0f, plr);

			mTarget->RemoveAllAuraByNameHash(mSpell->NameHash);

			if(new_plr != NULL)
				caster->CastSpell(new_plr, mSpell, value, count - 1, true);

			return true;
		}
};

void SpellProcMgr::SetupPriest()
{
	AddByNameHash(SPELL_HASH_IMPROVED_SPIRIT_TAP, &ImprovedSpiritTapSpellProc::Create);
	AddByNameHash(SPELL_HASH_HOLY_CONCENTRATION, &HolyConcentrationSpellProc::Create);
	AddByNameHash(SPELL_HASH_DIVINE_AEGIS, &DivineAegisSpellProc::Create);
	AddByNameHash(SPELL_HASH_IMPROVED_DEVOURING_PLAGUE, &ImprovedDevouringPlagueSpellProc::Create);
	AddByNameHash(SPELL_HASH_VAMPIRIC_EMBRACE, &VampiricEmbraceSpellProc::Create);
	AddByNameHash(SPELL_HASH_EMPOWERED_RENEW, &EmpoweredRenewSpellProc::Create);
	AddByNameHash(SPELL_HASH_MISERY, &MiserySpellProc::Create);
	AddByNameHash(SPELL_HASH_PRAYER_OF_MENDING, &PrayerOfMendingProc::Create);

	AddById(34919, &VampiricTouchEnergizeSpellProc::Create);
	AddById(64085, &VampiricTouchDispelDamageSpellProc::Create);
	AddById(48301, &ImprovedMindBlastSpellProc::Create);

	AddById(64128, &BodyAndSoulSpellProc::Create);
	AddById(65081, &BodyAndSoulSpellProc::Create);
	AddById(64134, &BodyAndSoulDummySpellProc::Create);
}
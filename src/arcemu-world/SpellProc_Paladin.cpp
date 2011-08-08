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

class SealOfCommandSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(SealOfCommandSpellProc);

		void Init(Object* obj)
		{
			// default chance of proc
			mProcChance = 25;

			mProcFlags = PROC_ON_MELEE_ATTACK;

			/* The formula for SoC proc rate is: [ 7 / ( 60 / Weapon Speed ) - from wowwiki */
			if(! mTarget->IsPlayer())
				return;

			uint32 weapspeed = 1;
			Item* itm = TO_PLAYER(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
			if(itm != NULL)
				weapspeed = itm->GetProto()->Delay;
			mProcChance = 7 * weapspeed / 600;
			if(mProcChance >= 50)
				mProcChance = 50;
		}
};

class EyeForAnEyeSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(EyeForAnEyeSpellProc);

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// If this player died by crit damage, don't do dmg back
			if(! mTarget->isAlive())
				return true;

			// Prevent proc on healing criticals
			if(CastingSpell != NULL && !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
				return true;

			dmg_overwrite[0] = dmg * (mOrigSpell->EffectBasePoints[0] + 1) / 100;

			int max_dmg = mTarget->GetMaxHealth() / 2;

			if(dmg_overwrite[0] > max_dmg)
				dmg_overwrite[0] = max_dmg;

			return false;
		}
};

class GraceOfTheNaaruSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(GraceOfTheNaaruSpellProc);

		void Init(Object* obj)
		{
			this->mProcClassMask[0] = 0x80000000;
		}
};

class SpiritualAttunementSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(SpiritualAttunementSpellProc);

		bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			if(CastingSpell == NULL || ! IsHealingSpell(CastingSpell))
				return false;

			return true;
		}
};

class PaladinSealsSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(PaladinSealsSpellProc);

		void Init(Object* obj)
		{
			this->mProcFlags = PROC_ON_MELEE_ATTACK;
		}
};

class SealOfCorruptionSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(SealOfCorruptionSpellProc);

		bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			if(victim == NULL || victim->FindAuraCountByHash(SPELL_HASH_BLOOD_CORRUPTION) < 5)
				return false;

			return true;
		}
};

class SealOfVengeanceSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(SealOfVengeanceSpellProc);

		bool CanProc(Unit* victim, SpellEntry* CastingSpell)
		{
			if(victim == NULL || victim->FindAuraCountByHash(SPELL_HASH_HOLY_VENGEANCE) < 5)
				return false;

			return true;
		}
};

void SpellProcMgr::SetupPaladin()
{
	AddByNameHash(SPELL_HASH_SEAL_OF_COMMAND, &SealOfCommandSpellProc::Create);

	AddByNameHash(SPELL_HASH_EYE_FOR_AN_EYE, &EyeForAnEyeSpellProc::Create);

	AddByNameHash(SPELL_HASH_GRACE_OF_THE_NAARU, &GraceOfTheNaaruSpellProc::Create);

	AddByNameHash(SPELL_HASH_SPIRITUAL_ATTUNEMENT, &SpiritualAttunementSpellProc::Create);

	AddById(20167, &PaladinSealsSpellProc::Create);
	AddById(20168, &PaladinSealsSpellProc::Create);
	AddById(20170, &PaladinSealsSpellProc::Create);
	AddById(53739, &SealOfCorruptionSpellProc::Create);
	AddById(42463, &SealOfVengeanceSpellProc::Create);
}
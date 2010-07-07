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

#include "StdAfx.h"

class ImprovedSpiritTapSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(ImprovedSpiritTapSpellProc);

	void Init(Object* obj)
	{
		mProcFlags = PROC_ON_SPELL_CRIT_HIT;
	}

	uint32 CalcProcChance(Unit *victim, SpellEntry *CastingSpell)
	{
		if( CastingSpell == NULL )
			return 0;

		if( CastingSpell->NameHash == SPELL_HASH_MIND_FLAY )
			return 50;

		if( CastingSpell->NameHash == SPELL_HASH_MIND_BLAST || CastingSpell->NameHash == SPELL_HASH_SHADOW_WORD__DEATH )
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

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
	{
		if ( CastingSpell == NULL )
			return true;

		if ( ! CastingSpell->HasEffect(SPELL_EFFECT_HEAL) )
			return true;

		*dmg_overwrite = dmg * (mOrigSpell->EffectBasePoints[0] +1) / 100;

		return false;
	}
};

class ImprovedDevouringPlagueSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(ImprovedDevouringPlagueSpellProc);

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
	{
		// Get dmg amt for 1 tick
		dmg = CastingSpell->EffectBasePoints[0] +1;

		// Get total ticks
		int ticks = GetDuration(dbcSpellDuration.LookupEntry(CastingSpell->DurationIndex)) / CastingSpell->EffectAmplitude[0];

		*dmg_overwrite = dmg * ticks * (mOrigSpell->EffectBasePoints[0] +1) / 100;

		return false;
	}
};

void SpellProcMgr::SetupPriest()
{
	AddByNameHash( SPELL_HASH_IMPROVED_SPIRIT_TAP, &ImprovedSpiritTapSpellProc::Create );
	AddByNameHash( SPELL_HASH_HOLY_CONCENTRATION, &HolyConcentrationSpellProc::Create );
	AddByNameHash( SPELL_HASH_DIVINE_AEGIS, &DivineAegisSpellProc::Create );
	AddByNameHash( SPELL_HASH_IMPROVED_DEVOURING_PLAGUE, &ImprovedDevouringPlagueSpellProc::Create );
}
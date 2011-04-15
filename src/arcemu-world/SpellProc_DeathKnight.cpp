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

class ButcherySpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(ButcherySpellProc);

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
	{
		dmg_overwrite[0] = mOrigSpell->EffectBasePoints[0] +1;

		return false;
	}
};

class BladeBarrierSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(BladeBarrierSpellProc);

	void Init(Object *obj)
	{
		mProcFlags = PROC_ON_CAST_SPELL;

		mProcClassMask[0] = mOrigSpell->EffectSpellClassMask[0][0];
		mProcClassMask[1] = mOrigSpell->EffectSpellClassMask[0][1];
		mProcClassMask[2] = mOrigSpell->EffectSpellClassMask[0][2];

		dk = TO_DK(mTarget);
	}

	bool CanProc(Unit *victim, SpellEntry *CastingSpell)
	{
		if( dk->IsAllRunesOfTypeInUse(RUNE_BLOOD) )
			return true;
		return false;
	}

private:
	DeathKnight* dk;
};

void SpellProcMgr::SetupDeathKnight()
{
	AddById( 50163, &ButcherySpellProc::Create );

	AddByNameHash( SPELL_HASH_BLADE_BARRIER, &BladeBarrierSpellProc::Create );
}
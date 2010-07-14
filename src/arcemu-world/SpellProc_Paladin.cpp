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

class SealOfCommandSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(SealOfCommandSpellProc);

	void Init(Object *obj)
	{
		// default chance of proc
		mProcChance = 25;

		/* The formula for SoC proc rate is: [ 7 / ( 60 / Weapon Speed ) - from wowwiki */
		if( ! mTarget->IsPlayer() )
			return;

		uint32 weapspeed = 1;
		Item* itm = TO_PLAYER( mTarget )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
		if( itm != NULL )
			weapspeed = itm->GetProto()->Delay;
		mProcChance = 7 * weapspeed / 600;
		if( mProcChance >= 50 )
			mProcChance = 50;
	}
};

class EyeForAnEyeSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(EyeForAnEyeSpellProc);

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
	{
		// If this player died by crit damage, don't do dmg back
		if( ! mTarget->isAlive() )
			return true;

		// Prevent proc on healing criticals
		if( CastingSpell != NULL && ! (CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING) )
			return true;

		dmg_overwrite[0] = dmg * (mOrigSpell->EffectBasePoints[0] +1) / 100;

		int max_dmg = mTarget->GetMaxHealth() / 2;

		if( dmg_overwrite[0] > max_dmg )
			dmg_overwrite[0] = max_dmg;

		return false;
	}
};

void SpellProcMgr::SetupPaladin()
{
	AddByNameHash( SPELL_HASH_SEAL_OF_COMMAND, &SealOfCommandSpellProc::Create );
	
	AddByNameHash( SPELL_HASH_EYE_FOR_AN_EYE, &EyeForAnEyeSpellProc::Create );
}
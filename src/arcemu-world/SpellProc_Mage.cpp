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

class HotStreakSpellProc : public SpellProc
{
		SPELL_PROC_FACTORY_FUNCTION(HotStreakSpellProc);

		void Init(Object* obj)
		{
			mCritsInARow = 0;
		}

		bool DoEffect(Unit* victim, SpellEntry* CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int* dmg_overwrite, uint32 weapon_damage_type)
		{
			// Check for classmask. Should proc only if CastingSpell is one listed in http://www.wowhead.com/spell=44448
			if(! CheckClassMask(victim, CastingSpell))
				return true;

			// If was not a crit, reset counter and don't proc
			if(!(flag & PROC_ON_SPELL_CRIT_HIT))
			{
				mCritsInARow = 0;
				return true;
			}

			// If was not at least 2nd crit in a row, don't proc
			if(++mCritsInARow < 2)
				return true;

			return false;
		}

	private:
		int mCritsInARow;
};

void SpellProcMgr::SetupMage()
{
	AddById(48108, &HotStreakSpellProc::Create);
}
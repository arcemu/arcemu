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

class RelentlessStrikesSpellProc : public SpellProc
{
    SPELL_PROC_FACTORY_FUNCTION(RelentlessStrikesSpellProc);

	uint32 CalcProcChance(Unit *victim, SpellEntry *CastingSpell)
	{
		if( CastingSpell && CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE && mTarget->IsPlayer() )//mTarget is the caster
			return float2int32( TO_PLAYER(mTarget)->m_comboPoints * mOrigSpell->EffectPointsPerComboPoint[0] );
		else
			return 0;
	}
};

void SpellProcMgr::SetupRogue()
{
	AddByNameHash( SPELL_HASH_RELENTLESS_STRIKES_EFFECT, &RelentlessStrikesSpellProc::Create );
}
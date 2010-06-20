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

class ShadowWeavingSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(ShadowWeavingSpellProc);

	void Init(Object* obj)
	{
		mProcFlags |= PROC_ON_SPELL_HIT;
	}
};

class ImprovedSpiritTapSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(ImprovedSpiritTapSpellProc);

	void Init(Object* obj)
	{
		mProcFlags = PROC_ON_SPELL_CRIT_HIT;
	}

	bool CanProcOnTriggered(Unit *victim, SpellEntry *CastingSpell)
	{
		if ( mOrigSpell->AttributesExC & FLAGS4_CAN_PROC_ON_TRIGGERED )
			return true;
		return false;
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


void SpellProcMgr::SetupPriest()
{
	AddById( 15258, &ShadowWeavingSpellProc::Create );
	AddByNameHash( SPELL_HASH_IMPROVED_SPIRIT_TAP, &ImprovedSpiritTapSpellProc::Create );
}
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

initialiseSingleton( SpellProcMgr );

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

class TwinBladesOfAzzinothSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(TwinBladesOfAzzinothSpellProc);

	void Init(Object *obj)
	{
		if( ! mTarget->IsPlayer() )
			return;

		/* The Twin Blades of Azzinoth.
			* According to comments on wowhead, this proc has ~0.75ppm (procs-per-minute). */
		Item* mh = TO_PLAYER( mTarget )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
		Item* of = TO_PLAYER( mTarget )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
		if( mh != NULL && of != NULL )
		{
			uint32 mhs = mh->GetProto()->Delay;
			uint32 ohs = of->GetProto()->Delay;
			mProcChance = mhs * ohs / ( 800 * ( mhs + ohs ) ); // 0.75 ppm
		}
	}
};

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

class FrostBrandAttackSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(FrostBrandAttackSpellProc);

	void Init(Object *obj)
	{		
		mProcChance = TO_ITEM(obj)->GetProto()->Delay * 9 / 600;
	}
};

void SpellProcMgr::SetupItems()
{
	AddByNameHash( SPELL_HASH_THE_TWIN_BLADES_OF_AZZINOTH, &TwinBladesOfAzzinothSpellProc::Create );
}

void SpellProcMgr::SetupDeathKnight()
{
}

void SpellProcMgr::SetupDruid()
{
}

void SpellProcMgr::SetupHunter()
{
}

void SpellProcMgr::SetupMage()
{
}

void SpellProcMgr::SetupPaladin()
{
	AddByNameHash( SPELL_HASH_SEAL_OF_COMMAND, &SealOfCommandSpellProc::Create );
}

void SpellProcMgr::SetupPriest()
{
}

void SpellProcMgr::SetupRogue()
{
	AddByNameHash( SPELL_HASH_RELENTLESS_STRIKES_EFFECT, &RelentlessStrikesSpellProc::Create );
}

void SpellProcMgr::SetupShamman()
{
	AddByNameHash( SPELL_HASH_FROSTBRAND_ATTACK, &FrostBrandAttackSpellProc::Create );
}

void SpellProcMgr::SetupWarlock()
{
}

void SpellProcMgr::SetupWarrior()
{
}

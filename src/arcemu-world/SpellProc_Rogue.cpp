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

class WoundPoisonSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(WoundPoisonSpellProc);

	void Init(Object* obj)
	{
		// PPM for Wound Poison is 21
		mProcChance = TO_ITEM(obj)->GetProto()->Delay * 21 / 600;
	}
};

class InstantPoisonSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(InstantPoisonSpellProc);

	void Init(Object* obj)
	{
		// PPM for Instant Poison is 8
		mProcChance = TO_ITEM(obj)->GetProto()->Delay * 8 / 600;
	}
};

class CutToTheChaseSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(CutToTheChaseSpellProc);

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 dmg, uint32 abs)
	{
		Aura *aura = mTarget->FindAuraByNameHash(SPELL_HASH_SLICE_AND_DICE);
		if (aura)
		{
			// Duration of 5 combo maximum
			int32 dur = 21 * MSTIME_SECOND;

			SM_FIValue(mTarget->SM_FDur,&dur,aura->GetSpellProto()->SpellGroupType);
			SM_PIValue(mTarget->SM_PDur,&dur,aura->GetSpellProto()->SpellGroupType);

			// Set new aura's duration, reset event timer and set client visual aura
			aura->SetDuration(dur);
			sEventMgr.ModifyEventTimeLeft(aura, EVENT_AURA_REMOVE, aura->GetDuration());
			mTarget->ModVisualAuraStackCount(aura, 0);
		}

		return true;
	}
};

void SpellProcMgr::SetupRogue()
{
	AddByNameHash( SPELL_HASH_WOUND_POISON_VII, &WoundPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_WOUND_POISON_VI , &WoundPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_WOUND_POISON_V  , &WoundPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_WOUND_POISON_IV , &WoundPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_WOUND_POISON_III, &WoundPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_WOUND_POISON_II , &WoundPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_WOUND_POISON    , &WoundPoisonSpellProc::Create );

	AddByNameHash( SPELL_HASH_INSTANT_POISON_IX  , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_VIII, &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_VII , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_VI  , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_V   , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_IV  , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_III , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON_II  , &InstantPoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_INSTANT_POISON     , &InstantPoisonSpellProc::Create );

	AddByNameHash( SPELL_HASH_CUT_TO_THE_CHASE, &CutToTheChaseSpellProc::Create );
}
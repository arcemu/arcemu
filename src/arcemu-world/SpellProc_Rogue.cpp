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

class PoisonSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(PoisonSpellProc);

	PoisonSpellProc()
	{
		mItemGUID = 0;
		mProcPerMinute = 0;
	}

	void Init(Object* obj)
	{
		if(obj == NULL)
		{
			mDeleted = true;
			return;
		}

		mItemGUID = TO_ITEM(obj)->GetGUID();
		if ( mProcPerMinute )
			mProcChance = TO_ITEM(obj)->GetProto()->Delay * mProcPerMinute / 600;
	}

	bool CanDelete(uint32 spellId, uint64 casterGuid = 0, uint64 misc = 0)//in this case misc is the item guid.
	{
		if ( mSpell->Id == spellId && mCaster == casterGuid && misc == mItemGUID && !mDeleted )
			return true;

		return false;
	}

	// Allow proc on ability cast (like eviscerate, envenom, fan of knives, rupture)
	bool CanProcOnTriggered(Unit *victim, SpellEntry *CastingSpell)
	{
		if ( CastingSpell != NULL && (CastingSpell->SpellGroupType[0] & 0x120000 || CastingSpell->SpellGroupType[1] & 0x240008) )
			return true;

		return false;
	}

	// Allow proc only if proccing hand is the one where poison was applied
	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
	{
		Item *item;

		if ( weapon_damage_type == OFFHAND )
			item = TO_PLAYER(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
		else
			item = TO_PLAYER(mTarget)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

		if ( item != NULL && item->GetGUID() == mItemGUID )
			return false;
		
		return true;
	}

protected:
	uint64 mItemGUID;
	uint32 mProcPerMinute;
};

class WoundPoisonSpellProc : public PoisonSpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(WoundPoisonSpellProc);

	void Init(Object* obj)
	{
		mProcPerMinute = 21;

		PoisonSpellProc::Init(obj);
	}
};

class InstantPoisonSpellProc : public PoisonSpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(InstantPoisonSpellProc);

	void Init(Object* obj)
	{
		mProcPerMinute = 8;

		PoisonSpellProc::Init(obj);
	}
};

class CutToTheChaseSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(CutToTheChaseSpellProc);

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
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

class DeadlyBrewSpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(DeadlyBrewSpellProc);

	bool DoEffect(Unit *victim, SpellEntry *CastingSpell, uint32 flag, uint32 dmg, uint32 abs, int *dmg_overwrite, uint32 weapon_damage_type)
	{
		mTarget->CastSpell( static_cast< Unit* >( NULL ), 3409, true );//Spell Id 3409: Crippling Poison

		return true;
	}
};

class WaylaySpellProc : public SpellProc
{
	SPELL_PROC_FACTORY_FUNCTION(WaylaySpellProc);

	void Init(Object* obj)
	{
		mProcFlags = PROC_ON_CAST_SPELL;
		mProcClassMask[0] = 0x204;
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

	AddByNameHash( SPELL_HASH_DEADLY_POISON_IX  , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_VIII, &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_VII , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_VI  , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_V   , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_IV  , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_III , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON_II  , &PoisonSpellProc::Create );
	AddByNameHash( SPELL_HASH_DEADLY_POISON     , &PoisonSpellProc::Create );

	AddByNameHash( SPELL_HASH_CRIPPLING_POISON, &PoisonSpellProc::Create );

	AddByNameHash( SPELL_HASH_MIND_NUMBING_POISON, &PoisonSpellProc::Create );		

	AddByNameHash( SPELL_HASH_CUT_TO_THE_CHASE, &CutToTheChaseSpellProc::Create );

	AddByNameHash( SPELL_HASH_DEADLY_BREW, &DeadlyBrewSpellProc::Create );
	
	AddByNameHash( SPELL_HASH_WAYLAY, &WaylaySpellProc::Create );
}
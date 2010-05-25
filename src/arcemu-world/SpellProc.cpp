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

SpellProc* SpellProcMgr::NewSpellProc(Unit *target, uint32 spell_id, uint32 orig_spell_id, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32 *groupRelation, Object *obj)
{
	return NewSpellProc(target, dbcSpell.LookupEntryForced(spell_id), dbcSpell.LookupEntryForced(orig_spell_id), caster, procChance, procFlags, procCharges, groupRelation, obj);
}

SpellProc* SpellProcMgr::NewSpellProc(Unit *target, SpellEntry *spell, SpellEntry *orig_spell, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32 *groupRelation, Object *obj)
{
	if( spell == NULL )
		return NULL;

	SpellProc* result;
	SpellProcMap::iterator itr;
	spell_proc_factory_function ptr = NULL;

	// Search for SpellProc in hash_map
	itr = mSpellProcNameHash.find(spell->NameHash);
	if ( itr != mSpellProcNameHash.end() )
		ptr=itr->second;
	else {
		itr = mSpellProc.find(spell->Id);
		if ( itr != mSpellProc.end() )
			ptr=itr->second;
	}
				
	if (ptr)
		result = (*ptr)();      // Found. Create a new object of this specific class
	else
		result = new SpellProc; // Not found. Create a new object of generic SpellProc

	result->mSpell            = spell;
	result->mOrigSpell        = orig_spell;
	result->mTarget           = target;
	result->mCaster           = caster;
	result->mProcChance       = procChance;
	result->mProcFlags        = procFlags;
	result->mProcCharges      = procCharges;
	result->mLastTrigger      = 0;
	result->mProcType         = 0;
	result->mDeleted          = false;
	if ( groupRelation )
	{
		result->mGroupRelation[0] = groupRelation[0];
		result->mGroupRelation[1] = groupRelation[1];
		result->mGroupRelation[2] = groupRelation[2];
	}
	else
	{
		result->mGroupRelation[0] = 0;
		result->mGroupRelation[1] = 0;
		result->mGroupRelation[2] = 0;
	}

	result->Init(obj);

	return result;
}

void SpellProcMgr::Setup()
{
	SetupItems();
	SetupDeathKnight();
	SetupDruid();
	SetupHunter();
	SetupMage();
	SetupPaladin();
	SetupPriest();
	SetupRogue();
	SetupShamman();
	SetupWarlock();
	SetupWarrior();
}
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

initialiseSingleton(SpellFactoryMgr);

void SpellFactoryMgr::AddSpellByEntry(SpellEntry* info, spell_factory_function spell_func)
{
	if(info != NULL)
		info->SpellFactoryFunc = (void * (*)) spell_func;
}

void SpellFactoryMgr::AddSpellById(uint32 spellId, spell_factory_function spell_func)
{
	AddSpellByEntry(dbcSpell.LookupEntryForced(spellId), spell_func);
}

void SpellFactoryMgr::AddSpellByNameHash(uint32 name_hash, spell_factory_function spell_func)
{
	uint32 cnt = dbcSpell.GetNumRows();
	SpellEntry* sp;

	for(uint32 x = 0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);

		if(sp->NameHash != name_hash)
			continue;

		AddSpellByEntry(sp, spell_func);
	}
}

void SpellFactoryMgr::AddAuraByEntry(SpellEntry* info, aura_factory_function aura_func)
{
	if(info != NULL)
		info->AuraFactoryFunc = (void * (*)) aura_func;
}

void SpellFactoryMgr::AddAuraById(uint32 spellId, aura_factory_function aura_func)
{
	AddAuraByEntry(dbcSpell.LookupEntryForced(spellId), aura_func);
}

void SpellFactoryMgr::AddAuraByNameHash(uint32 name_hash, aura_factory_function aura_func)
{
	uint32 cnt = dbcSpell.GetNumRows();
	SpellEntry* sp;

	for(uint32 x = 0; x < cnt; x++)
	{
		sp = dbcSpell.LookupRow(x);

		if(sp->NameHash != name_hash)
			continue;

		AddAuraByEntry(sp, aura_func);
	}
}

SpellEntry* SpellFactoryMgr::GetSpellEntryByDifficulty(uint32 id, uint8 difficulty)
{
    SpellDifficultyEntry* spellDiff = dbcSpellDifficultyEntry.LookupEntry(id);

    if ( spellDiff == NULL )
        return NULL;

    if ( spellDiff->SpellId[difficulty] <= 0 )
        return NULL;

    return dbcSpell.LookupEntryForced(spellDiff->SpellId[difficulty]);
}

Spell* SpellFactoryMgr::NewSpell(Object* Caster, SpellEntry* info, bool triggered, Aura* aur)
{
	if(info->SpellFactoryFunc == NULL)
		return new Spell(Caster, info, triggered, aur);
	else
	{
		spell_factory_function ptr;
		ptr = *spell_factory_function(info->SpellFactoryFunc);
		return (*ptr)(Caster, info, triggered, aur);
	}
}

Aura* SpellFactoryMgr::NewAura(SpellEntry* proto, int32 duration, Object* caster, Unit* target, bool temporary, Item* i_caster)
{
	if(proto->AuraFactoryFunc == NULL)
		return new Aura(proto, duration, caster, target, temporary, i_caster);
	else
	{
		aura_factory_function ptr;
		ptr = *aura_factory_function(proto->AuraFactoryFunc);
		return (*ptr)(proto, duration, caster, target, temporary, i_caster);
	}
}

void SpellFactoryMgr::Setup()
{
	SetupDeathKnight();
	SetupDruid();
	SetupHunter();
	SetupMage();
	SetupPaladin();
	SetupPriest();
	SetupRogue();
	SetupShaman();
	SetupWarlock();
	SetupWarrior();
}

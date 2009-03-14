/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

initialiseSingleton( SpellMgr );

SpellMgr::SpellMgr() 
{
	spells.clear();
	auras.clear();
}

SpellMgr::~SpellMgr() 
{ 
	{
		std::list<Spell *>::iterator itr;
		for (itr = spells.begin(); itr != spells.end(); itr++)
		{
			delete (*itr);
		}
	}
	{
		std::list<Spell *>::iterator itr;
		for (itr = spells.begin(); itr != spells.end(); itr++)
		{
			delete (*itr);
		}
	}
}

Spell * SpellMgr::CreateSpell(Object* caster, SpellEntry *info, bool triggered, Aura* aur )
{
	Spell *spell = new Spell(caster, info, triggered, aur);
	spells.push_back(spell);
	return spell;
}

Aura * SpellMgr::CreateAura(SpellEntry *proto, int32 duration,Object* caster, Unit *target, bool temporary, Item* i_caster)
{
	Aura *aura = new Aura(proto, duration, caster, target, temporary, i_caster);
	auras.push_back(aura);
	return aura;
}

void SpellMgr::DestroyAura(Aura *spell)
{
	delete spell;
}

void SpellMgr::DestroySpell(Spell *spell)
{
	delete spell;
}

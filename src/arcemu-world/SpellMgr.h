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

#ifndef __SPELLMGR_H
#define __SPELLMGR_H

class Spell;
class Aura;

class SERVER_DECL SpellMgr :  public Singleton < SpellMgr >
{
private:
	std::list<Spell *> spells;
	std::list<Aura *> auras;

public:
	SpellMgr();
	~SpellMgr();

	Spell * CreateSpell(Object* caster, SpellEntry *info, bool triggered, Aura* aur );
	void DestroySpell(Spell *spell);

	Aura * CreateAura(SpellEntry *proto, int32 duration,Object* caster, Unit *target, bool temporary = false, Item* i_caster = NULL);
	void DestroyAura(Aura *spell);
};

#define sSpellMgr SpellMgr::getSingleton()

#endif
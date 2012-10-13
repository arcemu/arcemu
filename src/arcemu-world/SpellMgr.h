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

#ifndef __SPELL_MGR_H
#define __SPELL_MGR_H

#include "StdAfx.h"

class Aura;

#define SPELL_FACTORY_FUNCTION(T) \
  public: \
  static Spell* Create(Object* Caster, SpellEntry *info, bool triggered, Aura* aur) { return new T(Caster, info, triggered, aur); } \
  T(Object* Caster, SpellEntry *info, bool triggered, Aura* aur) : Spell(Caster, info, triggered, aur) {}

#define AURA_FACTORY_FUNCTION(T) \
  public: \
  static Aura* Create(SpellEntry *proto, int32 duration, Object* caster, Unit *target, bool temporary = false, Item* i_caster = NULL) { return new T(proto, duration, caster, target, temporary, i_caster); } \
  T(SpellEntry *proto, int32 duration, Object* caster, Unit *target, bool temporary = false, Item* i_caster = NULL) : Aura(proto, duration, caster, target, temporary, i_caster) {}

typedef Spell* (*spell_factory_function)(Object* Caster, SpellEntry* info, bool triggered, Aura* aur);
typedef Aura* (*aura_factory_function)(SpellEntry* proto, int32 duration, Object* caster, Unit* target, bool temporary, Item* i_caster);

class SERVER_DECL SpellFactoryMgr: public Singleton < SpellFactoryMgr >
{
	public:
		SpellFactoryMgr()
		{
			Setup();
		}

		~SpellFactoryMgr()
		{
		}

		SpellEntry* GetSpellEntryByDifficulty(uint32 id, uint8 difficulty);
		Spell* NewSpell(Object* Caster, SpellEntry* info, bool triggered, Aura* aur);
		Aura* NewAura(SpellEntry* proto, int32 duration, Object* caster, Unit* target, bool temporary = false, Item* i_caster = NULL);

	private:

		void AddSpellByEntry(SpellEntry* info, spell_factory_function spell_func);
		void AddSpellById(uint32 spellId, spell_factory_function spell_func);
		void AddSpellByNameHash(uint32 name_hash, spell_factory_function spell_func);

		void AddAuraByEntry(SpellEntry* info, aura_factory_function aura_func);
		void AddAuraById(uint32 spellId, aura_factory_function aura_func);
		void AddAuraByNameHash(uint32 name_hash, aura_factory_function aura_func);

		void Setup();

		void SetupDeathKnight();
		void SetupDruid();
		void SetupHunter();
		void SetupMage();
		void SetupPaladin();
		void SetupPriest();
		void SetupRogue();
		void SetupShaman();
		void SetupWarlock();
		void SetupWarrior();
};

#define sSpellFactoryMgr SpellFactoryMgr::getSingleton()

#endif
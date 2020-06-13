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

class InnervateSpell : public Spell
{
		SPELL_FACTORY_FUNCTION(InnervateSpell);

		int32 DoCalculateEffect(uint32 i, Unit* target, int32 value)
		{
			if(p_caster != NULL && i == 0 && target != NULL)
				value = (uint32)(p_caster->GetBaseMana() * 0.225f);

			return value;
		}
};

void SpellFactoryMgr::SetupDruid()
{
	AddSpellByNameHash(SPELL_HASH_INNERVATE, &InnervateSpell::Create);
}

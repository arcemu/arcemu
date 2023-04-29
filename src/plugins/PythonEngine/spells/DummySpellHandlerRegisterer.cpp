/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

#include "spells/DummySpellHandlerRegisterer.hpp"

#include "spells/SpellScriptHandler.hpp"

void DummySpellHandlerRegisterer::visit( unsigned long spellId, void *function )
{
	if( !mgr->has_dummy_spell_script( spellId ) )
	{
		mgr->register_dummy_spell( spellId, &SpellScriptHandler::handleDummySpell );
	}
}

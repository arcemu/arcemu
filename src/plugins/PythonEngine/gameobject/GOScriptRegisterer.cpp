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

#include "gameobject/GOScriptRegisterer.hpp"

#include "gameobject/PythonGameObjectAIScriptFactory.hpp"

void GOScriptRegisterer::visit( unsigned int id, GOFunctionTuple &tuple )
{
	/// If we already have a script factory for this GO then either it's from another source, or we're reloading.
	/// Nevertheless, let's not try to add the script factory then
	if( !mgr->has_gameobject_script( id ) )
		mgr->register_gameobject_script( id, &PythonGameObjectAIScriptFactory::createScript );
}

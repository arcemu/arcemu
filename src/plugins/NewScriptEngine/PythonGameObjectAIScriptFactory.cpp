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

#include "PythonGameObjectAIScriptFactory.hpp"

#include "PythonGameObjectAIScript.hpp"

#include "FunctionRegistry.hpp"

GameObjectAIScript* PythonGameObjectAIScriptFactory::createScript( GameObject* src )
{
	uint32 id = src->GetInfo()->ID;
	PythonGameObjectAIScript* script = NULL;
	
	GOFunctionTuple* tuple = FunctionRegistry::getGOEventFunctions( id );
	if( tuple != NULL )
	{
		script = new PythonGameObjectAIScript( src, *tuple );
	}
	else
	{
		/// This shouldn't happen
		GOFunctionTuple empty;
		script = new PythonGameObjectAIScript( src, empty );
	}

	return script;
}

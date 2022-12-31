/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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

#include <Python.h>

#include "python/PythonObject.hpp"
#include "python/PythonTuple.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"
#include "python/Python.hpp"

#include "ServerHookRegistry.hpp"

#include "python/modules/unit_module.hpp"

void python_hookOnEmote( Player* player, uint32 emote, Unit* unit )
{
	std::vector< void* > handlers;
	ServerHookRegistry::getHooksForEvent( 8, handlers );

	for( std::vector< void* >::iterator itr = handlers.begin(); itr != handlers.end(); ++itr )
	{
		PythonTuple args( 3 );
		args.setItem( 0, (long)player );
		args.setItem( 1, emote );

		if( unit != NULL )
		{
			ArcPyUnit *apu = createArcPyUnit();
			apu->unitPtr = unit;
			args.setItem( 2, PythonObject( (PyObject*)apu ) );
		}
		else
		{
			args.setItemNone( 2 );
		}

		PythonCallable callable( (PyObject*)(*itr) );
		PythonValue value = callable.call( args );
		if( value.isEmpty() )
		{
			///call failed
			Python::printError();
		}
		else
		{
			/// Value not needed, throw it away
			value.decref();
		}
	}
}


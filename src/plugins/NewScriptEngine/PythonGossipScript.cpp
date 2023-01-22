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

#include "GossipFunctionTuple.hpp"

#include "PythonGossipScript.hpp"

#include <Python.h>

#include "python/PythonObject.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"
#include "python/ArcPyTuple.hpp"

#include "python/ArcPython.hpp"

PythonGossipScript::PythonGossipScript( GossipFunctionTuple &tuple )
{
	this->functions.onHelloFunction = tuple.onHelloFunction;
	this->functions.onSelectionFunction = tuple.onSelectionFunction;
	this->functions.onEndFunction = tuple.onEndFunction;
}

void PythonGossipScript::OnHello( Object* object, Player* player )
{
	if( functions.onHelloFunction == NULL )
		return;

	ArcPyTuple args( 3 );
	
	switch( object->GetTypeId() )
	{
	case TYPEID_UNIT:
		{
			args.setItemUnit( 0, (Unit*)object );
			break;
		}
	}

	args.setItem( 1, 1 );
	args.setItemPlayer( 2, player );

	PythonCallable callable( functions.onHelloFunction );
	PythonValue value = callable.call( args );
	if( value.isEmpty() )
	{
		Python::printError();
	}
	else
	{
		value.decref();
	}
}

void PythonGossipScript::OnSelectOption( Object* object, Player* player, uint32 id, const char* enteredCode )
{
}

void PythonGossipScript::OnEnd( Object* object, Player* player )
{
}

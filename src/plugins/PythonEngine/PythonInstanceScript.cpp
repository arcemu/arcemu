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
#include "PythonInstanceScript.hpp"

#include "Python.h"

#include "python/ArcPython.hpp"
#include "python/ArcPyTuple.hpp"
#include "python/PythonValue.hpp"
#include "python/PythonCallable.hpp"

PythonInstanceScript::PythonInstanceScript( MapMgr* mgr, const InstanceFunctionTuple &tuple ) :
InstanceScript( mgr )
{
	setFunctions( tuple );
}

PythonInstanceScript::~PythonInstanceScript()
{
}

void PythonInstanceScript::setFunctions( const InstanceFunctionTuple &tuple )
{
	for( int i = 0; i < PYTHON_INSTANCE_EVENT_COUNT; i++ )
	{
		this->functions.functions[ i ] = tuple.functions[ i ];
	}
}

void PythonInstanceScript::clearFunctions()
{
	for( int i = 0; i < PYTHON_INSTANCE_EVENT_COUNT; i++ )
	{
		this->functions.functions[ i ] = NULL;
	}
}


void PythonInstanceScript::OnLoad()
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_ONLOAD;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItem( 0, GetInstance()->GetInstanceID() );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );
}

void PythonInstanceScript::Destroy()
{
	Guard g( ArcPython::getLock() );

	unsigned long eventType = PYTHON_INSTANCE_EVENT_DESTROY;
	if( !functions.hasFunction( eventType ) )
		return;

	ArcPyTuple args( 1 );
	args.setItem( 0, GetInstance()->GetInstanceID() );

	PythonCallable callable( functions.getFunction( eventType ) );
	callable.callNoReturn( args );

	delete this;
}


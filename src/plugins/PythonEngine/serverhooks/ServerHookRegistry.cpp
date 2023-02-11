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

#include "Python.h"

#include "serverhooks/ServerHookRegistry.hpp"

std::multimap< unsigned long, void* > ServerHookRegistry::hooks;

void ServerHookRegistry::addHook( unsigned long serverEvent, void* handler )
{
	hooks.insert( std::pair< unsigned long, void* >( serverEvent, handler ) );
}

bool ServerHookRegistry::hasHooksForEvent( unsigned long serverEvent )
{
	std::multimap< unsigned long, void* >::iterator lower = hooks.lower_bound( serverEvent );
	std::multimap< unsigned long, void* >::iterator upper = hooks.upper_bound( serverEvent );

	if( lower == hooks.end() )
		return false;

	if( lower == upper )
		return false;

	return true;
}

void ServerHookRegistry::getHooksForEvent( unsigned long serverEvent, std::vector< void* > &handlers )
{
	std::multimap< unsigned long, void* >::iterator lower = hooks.lower_bound( serverEvent );
	std::multimap< unsigned long, void* >::iterator upper = hooks.upper_bound( serverEvent );

	std::multimap< unsigned long, void* >::iterator itr = lower;
	while( itr != hooks.end() && itr != upper )
	{
		handlers.push_back( itr->second );
		++itr;
	}
}

void ServerHookRegistry::getHooks( std::set< void* > &handlers )
{
	std::multimap< unsigned long, void* >::iterator itr = hooks.begin();
	while( itr != hooks.end() )
	{
		handlers.insert( itr->second );
		++itr;
	}
}

void ServerHookRegistry::releaseHooks()
{
	std::multimap< unsigned long, void* >::iterator itr = hooks.begin();
	while( itr != hooks.end() )
	{
		PyObject *callback = (PyObject*)itr->second;
		Py_DecRef( callback );
		++itr;
	}
	hooks.clear();
}


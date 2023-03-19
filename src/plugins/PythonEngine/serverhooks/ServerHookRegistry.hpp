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

#ifndef SERVERHOOK_REGISTRY_H
#define SERVERHOOK_REGISTRY_H

#include <vector>
#include <map>
#include <set>

/// Organized storage for scripted server hook handlers
class ServerHookRegistry
{
public:
	///
	/// Registers a scripted server hook
	///
	/// Parameters
	///   serverEvent     -   The scripted server event for which we're registering the hook
	///   handler         -   The handler function for the hook
	///
	/// Return value
	///   None
	///
	static void addHook( unsigned long serverEvent, void* handler );
	
	///
	/// Tells if there's a handler for the specified scripted server hook
	///
	/// Parameters
	///   serverEvent   -   The scripted server event
	///
	/// Return value
	///   Return true if there's a registered hook for the server event.
	///   Returns false otherwise
	///
	static bool hasHooksForEvent( unsigned long serverEvent );
	
	///
	/// Retrieves a copy of the scripted server event hooks registered for the specified event
	///
	/// Parameters
	///   serverEvent       - The scripted server event
	///   handlers          - A vector that will be filled with the scripted server event hooks registered for the specified event (if any)
	///
	/// Return value
	///   None
	///
	static void getHooksForEvent( unsigned long serverEvent, std::vector< void* > &handlers );
	
	///
	/// Retrieves all registered server hooks
	///
	/// Parameters
	///   handlers    -   A vector that will be filled with all the hooks registered
	///
	/// Return value
	///   None
	///
	static void getHooks( std::set< void* > &handlers );
	
	/// Release / deallocate registered server hooks
	static void releaseHooks();

private:
	///
	/// A "list" of the registered server hook handlers
	///
	/// Key: event type
	/// Value: hook handler functions
	///
	static std::multimap< unsigned long, void* > hooks;
};

#endif

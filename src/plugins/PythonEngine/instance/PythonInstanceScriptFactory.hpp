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

#ifndef PYTHON_INSTANCESCRIPT_FACTORY_H
#define PYTHON_INSTANCESCRIPT_FACTORY_H

/// Creates Python Instance script instances
class PythonInstanceScriptFactory
{
public:
	///
	/// Creates a Python Instance Script for the specified Instance
	///
	/// Parameters
	///   mgr    -    The map manager of the instance that will use the Script
	///
	/// Return value
	///   An Instance Script for the specified Instance
	///
	static InstanceScript* createInstanceScript( MapMgr* mgr );

	/// Called when the Scripting Engine is shut down
	static void onShutdown();
	
	/// Called when the Scripting Engine is reloaded
	static void onReload();

	/// 
	/// Remove a script from the "list" of created scripts
	///
	/// Parameters
	///   script   -   The script that should be removed
	///
	/// Return value
	///   None
	///
	static void removeScript( InstanceScript* script );

private:
	/// The "list" of created scripts
	static HM_NAMESPACE::HM_HASH_SET< InstanceScript* > createdScripts;

	/// A lock to prevent multiple threads accessing simultaneously
	static Mutex lock;
};

#endif

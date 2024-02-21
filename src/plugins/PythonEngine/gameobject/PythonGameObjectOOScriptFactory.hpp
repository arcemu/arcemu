/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#ifndef A_PGOOSF_H_
#define A_PGOOSF_H_

class PythonGameObjectOOScriptFactory
{
public:
	///
	/// Creates an OO Python GameObject Script for the specified Creature
	///
	/// Parameters
	///   src    -    The GameObject that will use the Script
	///
	/// Return value
	///   An OO Python GameObject Script for the specified Creature
	///
	static GameObjectAIScript* createScript( GameObject* src );

	/// Called when the Scripting Engine is reloaded
	static void onReload();

	/// Called when the Scripting Engine is shut down
	static void onShutdown();

	/// 
	/// Remove a script from the "list" of created scripts
	///
	/// Parameters
	///   script   -   The script that should be removed
	///
	/// Return value
	///   None
	///
	static void removeScript( GameObjectAIScript* script );

private:
	/// The "list" of created scripts
	static HM_NAMESPACE::HM_HASH_SET< GameObjectAIScript* > createdScripts;

	/// A lock to prevent multiple threads accessing simultaneously
	static Mutex lock;
};

#endif

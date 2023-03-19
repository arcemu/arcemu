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

#ifndef P_CREATURE_AI_S_F_
#define P_CREATURE_AI_S_F_

/// Creates Python Creature AI Scripts
class PythonCreatureAIScriptFactory
{
public:
	///
	/// Creates a Python Creature AI Script for the specified Creature
	///
	/// Parameters
	///   src    -    The Creature that will use the Script
	///
	/// Return value
	///   A Python Creature AI Script for the specified Creature
	///
	static CreatureAIScript* createScript( Creature* src );

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
	static void removeScript( CreatureAIScript* script );

private:
	/// The "list" of created scripts
	static HM_NAMESPACE::HM_HASH_SET< CreatureAIScript* > createdScripts;

	/// A lock to prevent multiple threads accessing simultaneously
	static Mutex lock;
};

#endif

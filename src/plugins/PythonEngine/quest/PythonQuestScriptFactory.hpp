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

#ifndef PYTHON_QUESTSCRIPT_FACTORY_H
#define PYTHON_QUESTSCRIPT_FACTORY_H

#include "quest/PythonQuestScript.hpp"

/// Creates Python Quest script instances
class PythonQuestScriptFactory
{
public:
	///
	/// Creates a Python Quest Script for the specified quest
	///
	/// Parameters
	///   questId    -    The id of the quest that will use the Script
	///
	/// Return value
	///   A Python Quest Script for the specified Quest
	///
	static PythonQuestScript* createQuestScript( unsigned int questId );

	/// Called when the Scripting Engine is reloaded
	static void onReload();

	/// Called when the Scripting Engine is shut down
	static void onShutDown();

	/// 
	/// Remove a script from the "list" of created scripts
	///
	/// Parameters
	///   script   -   The script that should be removed
	///
	/// Return value
	///   None
	///
	static void removeScript( PythonQuestScript* script );

private:
	/// The "list" of created scripts
	static HM_NAMESPACE::HM_HASH_SET< PythonQuestScript* > createdScripts;

	/// A lock to prevent multiple threads accessing simultaneously
	static Mutex lock;
};

#endif

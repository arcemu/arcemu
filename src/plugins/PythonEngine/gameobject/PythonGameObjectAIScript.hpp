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

#ifndef P_GO_AI_SCRIPT_H
#define P_GO_AI_SCRIPT_H

#include "gameobject/GOFunctionTuple.hpp"

/// GameObject AI script that calls Python functions to handle scripted events
class PythonGameObjectAIScript : public GameObjectAIScript
{
public:
	///
	/// Initializes this Python GameObject AI Script
	///
	/// Parameters
	///   src         -   The GameObject that uses this script
	///   functions   -   A Tuple that contains the Python functions called by this script
	///
	/// Return value
	///   None
	///
	PythonGameObjectAIScript( GameObject* src, GOFunctionTuple &functions );

	virtual ~PythonGameObjectAIScript();

	///
	/// Adds / replaces the Python functions in this Python GameObject AI Script
	///
	/// Parameters
	///   functions   -   A Tuple that contains the Python functions called by this script
	///
	/// Return value
	///   None
	///
	void setFunctions( GOFunctionTuple &functions );
	
	void OnCreate();
	void OnSpawn();
	void OnDespawn();
	void OnLootTaken( Player* looter, ItemPrototype* itemInfo);
	void OnActivate( Player* player );
	void OnDamaged( uint32 damage );
	void OnDestroyed();
	void AIUpdate();

	/// Returns the gameobject that uses this script
	GameObject* getGameObject(){ return _gameobject; }

private:
	/// Contains the Python functions that this Python GameObject AI Script calls
	GOFunctionTuple functions;
};

#endif

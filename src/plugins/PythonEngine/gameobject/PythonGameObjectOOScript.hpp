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

#ifndef P_GO_OO_SCRIPT_H
#define P_GO_OO_SCRIPT_H

/// GameObject OO script that calls Python class methods
class PythonGameObjectOOScript : public GameObjectAIScript
{
private:
	PyObject *obj;

public:
	///
	/// Initializes this Python GameObject OO Script
	///
	/// Parameters
	///   src         -   The GameObject that uses this script
	///   obj         -   The Python class instance 
	///
	/// Return value
	///   None
	///
	PythonGameObjectOOScript( GameObject* src, PyObject *obj );

	virtual ~PythonGameObjectOOScript();

	void setObject( PyObject *obj );

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

};

#endif

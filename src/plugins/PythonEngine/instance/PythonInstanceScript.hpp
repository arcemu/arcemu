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

#ifndef PYTHON_INSTANCE_SCRIPT_H
#define PYTHON_INSTANCE_SCRIPT_H

#include "instance/InstanceFunctionTuple.hpp"

/// An Instance Script that calls Python functions to handle scripted events
class PythonInstanceScript : public InstanceScript
{
public:
	///
	/// Initializes this Python Instance Script
	///
	/// Parameters
	///   mgr         -   The map manager of the instance that will use this script
	///   tuple       -   A Tuple that contains the Python functions called by this script
	///
	/// Return value
	///   None
	///
	PythonInstanceScript( MapMgr* mgr, const InstanceFunctionTuple &tuple );

	virtual ~PythonInstanceScript();

	///
	/// Adds / replaces the Python functions in this Python Instance Script
	///
	/// Parameters
	///   functions   -   A Tuple that contains the Python functions called by this script
	///
	/// Return value
	///   None
	///
	void setFunctions( const InstanceFunctionTuple &tuple );

	/// Removes Python function references from this script
	void clearFunctions();

	void OnPlayerDeath( Player* victim, Unit* killer );
	void OnPlayerEnter( Player* player );
	void OnAreaTrigger( Player* player, uint32 areaId );
	void OnZoneChange( Player* player, uint32 newZone, uint32 oldZone );
	void OnCreatureDeath( Creature* victim, Unit* killer );
	void OnCreaturePushToWorld( Creature* creature );
	void OnGameObjectActivate( GameObject* gameObject, Player* player );
	void OnGameObjectPushToWorld( GameObject* gameObject );
	void OnLoad();
	void Destroy();

private:
	/// A Tuple that contains the Python functions called by this script
	InstanceFunctionTuple functions;
};

#endif

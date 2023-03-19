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

#ifndef PYTHON_GOSSIP_SCRIPT_H
#define PYTHON_GOSSIP_SCRIPT_H

/// Gossip script that calls Python functions to handle scripted events
class PythonGossipScript : public Arcemu::Gossip::Script
{
public:
	///
	/// Initializes this script
	///
	/// Parameters
	///   tuple   -  A tuple that contains references to the Python functions that handle scripted events
	PythonGossipScript( GossipFunctionTuple &tuple );

	void OnHello( Object* object, Player* player );

	void OnSelectOption( Object* object, Player* player, uint32 id, const char* enteredCode );

	void OnEnd( Object* object, Player* player );

	/// Removes Python functions references from this script
	void clearFunctions();

	///
	/// Adds / replaces references to Python functions in this script
	///
	/// Parameters
	///   functions  -  A tuple that contains Python functions that handle the scripted events in this script
	///
	/// Return value
	///
	void setFunctions( GossipFunctionTuple &functions );

private:
	/// A tuple that contains Python functions that handle the scripted events in this script
	GossipFunctionTuple functions;
};

#endif

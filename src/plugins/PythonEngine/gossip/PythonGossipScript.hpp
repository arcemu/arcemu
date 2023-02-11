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

class PythonGossipScript : public Arcemu::Gossip::Script
{
public:

	PythonGossipScript( GossipFunctionTuple &tuple );

	void OnHello( Object* object, Player* player );

	void OnSelectOption( Object* object, Player* player, uint32 id, const char* enteredCode );

	void OnEnd( Object* object, Player* player );

	void clearFunctions();
	void setFunctions( GossipFunctionTuple &functions );

private:
	GossipFunctionTuple functions;
};

#endif

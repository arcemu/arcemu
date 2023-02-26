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

#include "StdAfx.h"

#include "creature/CreatureGossipScriptRegisterer.hpp"

#include "gossip/PythonGossipScript.hpp"

void CreatureGossipScriptRegisterer::visit( unsigned int id, GossipFunctionTuple &tuple )
{
	Arcemu::Gossip::Script* script = mgr->get_creature_gossip( id );

	/// If the creature doesn't have a script yet create one, otherwise reload the functions
	if( script == NULL )
	{
		PythonGossipScript *script = new PythonGossipScript( tuple );
		mgr->register_creature_gossip( id, script );
	}
	else
	{
		PythonGossipScript* pythonScript = dynamic_cast< PythonGossipScript* >( script );
		if( pythonScript != NULL )
		{
			pythonScript->setFunctions( tuple );
		}
	}
}

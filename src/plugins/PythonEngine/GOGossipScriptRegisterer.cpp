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

#include "GOGossipScriptRegisterer.hpp"

#include "PythonGossipScript.hpp"

void GOGossipScriptRegisterer::visit( unsigned int id, GossipFunctionTuple &tuple )
{
	Arcemu::Gossip::Script* script = mgr->get_go_gossip( id );
	if( script == NULL )
	{
		PythonGossipScript *script = new PythonGossipScript( tuple );
		mgr->register_go_gossip( id, script );
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

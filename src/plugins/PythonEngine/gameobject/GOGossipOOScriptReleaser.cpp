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

#include "StdAfx.h"

#include "GOGossipOOScriptReleaser.hpp"

#include "Python.h"

#include "gossip/PythonGossipOOScript.hpp"

void GOGossipOOScriptReleaser::visit( uint32 goId, void *script )
{
	Arcemu::Gossip::Script *ags = mgr->get_go_gossip( goId );
	if( ags == NULL )
	{
		return;
	}

	PythonGossipOOScript *pgos = dynamic_cast< PythonGossipOOScript* >( ags );
	if( pgos == NULL )
	{
		return;
	}

	pgos->setObject( NULL );
}

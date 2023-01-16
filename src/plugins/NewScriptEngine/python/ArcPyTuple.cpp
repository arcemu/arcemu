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

#include "Python.h"

#include "python/PythonObject.hpp"
#include "python/PythonTuple.hpp"
#include "python/ArcPyTuple.hpp"

#include "python/modules/ArcPyGuild.hpp"
#include "python/modules/ArcPyPlayer.hpp"
#include "python/modules/ArcPyWorldSession.hpp"

ArcPyTuple::ArcPyTuple( unsigned long size ) :
PythonTuple( size )
{
}

ArcPyTuple::~ArcPyTuple()
{
}


void ArcPyTuple::setItemGuild( unsigned long idx, Guild* guild )
{
	ArcPyGuild *apg = createArcPyGuild();
	apg->guildPtr = guild;
	PyTuple_SetItem( getObject(), idx, (PyObject*)apg );
}

void ArcPyTuple::setItemPlayer( unsigned long idx, Player* player )
{
	ArcPyPlayer *app = createArcPyPlayer();
	app->playerPtr = player;
	PyTuple_SetItem( getObject(), idx, (PyObject*)app );
}

void ArcPyTuple::setItemWorldSession( unsigned long idx, WorldSession* worldSession )
{
	ArcPyWorldSession *apws = createArcPyWorldSession();
	apws->worldSessionPtr = worldSession;
	PyTuple_SetItem( getObject(), idx, (PyObject*)apws );
}


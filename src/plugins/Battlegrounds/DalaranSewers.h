/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

#ifndef DALARAN_SEWERS_H
#define DALARAN_SEWERS_H

#include "StdAfx.h"

class DalaranSewers : public Arena{
public:
	DalaranSewers( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side );
	~DalaranSewers();

	static CBattleground* Create( MapMgr* m, uint32 i, uint32 l, uint32 t, uint32 players_per_side ){
		return new DalaranSewers( m, i, l, t, players_per_side );
	}

	void OnCreate();
	LocationVector GetStartingCoords( uint32 Team );
	bool HookHandleRepop( Player *plr );
};

#endif

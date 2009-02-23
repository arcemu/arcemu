/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

void ObjectContainer::Activate(MapMgr *mgr)
{

}

void ObjectContainer::AddToWorld()
{
	for(set<Object *>::iterator itr = groupObjects.begin(); itr != groupObjects.end(); ++itr)
		(*itr)->AddToWorld();
}

void ObjectContainer::AddToWorld(MapMgr *pMapMgr)
{
	for(set<Object *>::iterator itr = groupObjects.begin(); itr != groupObjects.end(); ++itr)
		(*itr)->AddToWorld(pMapMgr);
}

void ObjectContainer::Deactivate(MapMgr *mgr)
{

}
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

#ifndef _UNIFIED_OBJECT_H
#define _UNIFIED_OBJECT_H

/*	Under Development
 *
 *	This class acts as a single object, but actually consists of multiple
 *  sub-objects that all orient around the location of the the container 
 *  objects (x, y) position. z is constant. A change in parent object
 *  orientation needs to rotate each object.
 *
 *  One application of this would be for vehicles. Another would be for
 *  NPC's on transports.
 *
 */
class SERVER_DECL ObjectContainer : public Object
{
protected:
	Object	*					primaryObject;
	std::set<Object*>			groupObjects;
	std::set<LocationVector *>	groupOffset;

public:
	void addGroupObject(Object * object, bool primary = false)
	{
		if (object->GetMapMgr() != m_mapMgr)
			return;
		//groupObjects.
	}

	void removeGroupObject(Object * object)
	{
		if (object->GetMapMgr() != m_mapMgr)
			return;

	}

	virtual void Activate(MapMgr *mgr);
	virtual void AddToWorld();
	virtual void AddToWorld(MapMgr *pMapMgr);
	virtual void Deactivate(MapMgr *mgr);
};

#endif

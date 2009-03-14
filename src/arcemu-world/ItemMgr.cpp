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

initialiseSingleton( ItemMgr );

ItemMgr::ItemMgr() 
{
	items.clear();
}

ItemMgr::~ItemMgr() 
{ 
	std::list<Item *>::iterator itr;
	for (itr = items.begin(); itr != items.end(); itr++)
	{
		delete (*itr);
	}
}

Item * ItemMgr::CreateItem(uint32 high, uint32 low)
{
	Item * item = new Item(high, low);
	items.push_back(item);
	return item;
}

Container * ItemMgr::CreateContainer(uint32 high, uint32 low)
{
	Container * item = new Container(high, low);
	items.push_back(item);
	return item;
}

void ItemMgr::DestroyItem(Item * item)
{
	delete item;
}

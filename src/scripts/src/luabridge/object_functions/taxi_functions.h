/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "StdAfx.h"
#include "LUAEngine.h"

class luataxinode : public TaxiNode
{
public:
	//a constructor object to allow taxinodes to be constructed lua side using the __call metaevent
	luataxinode(uint32 id, uint32 map_id, float x, float y, float z)
	{
		this->id = id;
		this->mapid = map_id;
		this->x = x;
		this->y = y;
		this->z = z;
	}
	//Paroxysm: Noticed taxipaths when destroyed attempt to gc their nodes. Due to this, if we construct a taxi node lua side and give it gc capabilities,
	// then later on it's added to a taxipath, when that taxi node var goes out of scope it will get collected and taxi path will be holding a freed pointer
	//	later on when the taxipath is destroyed, it will delete on this freed pointer and crash our program. So, we provide a method to explicitly gc taxi nodes
	//	so that lua scripts can delete it explicitly. However this will rarely be required since most taxi nodes created will be added to taxi paths and gc will
	//	be handled by that taxipath's destructor.
	ARCEMU_FORCEINLINE void Destroy() 
	{
		delete this;
	}
};

namespace lua_engine
{
	void bindTaxiMethods(luabridge::module & m)
	{
#define prop(name) .property_rw(#name, &luataxinode::name)
		m	.class_<luataxinode>("TaxiNode")
			.constructor< void(*)(uint32,uint32,float,float,float) >()
			prop(x)
			prop(y)
			prop(z)
			prop(id)
			prop(mapid)
			prop(alliance_mount)
			prop(horde_mount);
#undef prop
		m	.class_<TaxiPath>("TaxiPath")
			.constructor<void(*)()>()
			.method("GetID", &TaxiPath::GetID)
			.method("AddPathNode", &TaxiPath::AddPathNode)
			.method("GetNodeCount", &TaxiPath::GetNodeCount)
			//.method("GetPathNode", &TaxiPath::GetPathNode)
			.method("GetPrice", &TaxiPath::GetPrice);
			//.method("SetPrice", &TaxiPath::SetPrice);

		m	.class_<TaxiMgr>("sTaxiMgr")
			.method("GetTaxiNode", &TaxiMgr::GetTaxiNode)
			.method("GetTaxiPath", (TaxiPath*(TaxiMgr::*)(uint32) )&TaxiMgr::GetTaxiPath)
			.method("GetTaxiPathFT", (TaxiPath*(TaxiMgr::*)(uint32, uint32) )&TaxiMgr::GetTaxiPath)
			.method("GetNearestTaxiNode", &TaxiMgr::GetNearestTaxiNode);
		//overrite sTaxiMgr static table w/ the object.
		TaxiMgr * tmgr = TaxiMgr::getSingletonPtr();
		luabridge::tdstack<TaxiMgr*>::push(m.L, tmgr);
		//assign _G[sTaxiMgr] = TaxiMgr::getSingleton.
		lua_setglobal(m.L, "sTaxiMgr");
	}
}
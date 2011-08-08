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



#include "LUAEngine.h"

class luataxinode : public TaxiPathNode
{
	public:
		//a constructor object to allow taxinodes to be constructed lua side using the __call metaevent
		luataxinode(uint32 map_id, float x, float y, float z)
		{
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
		m	.class_decl<TaxiPathNode>("TaxiPathNode");
		m	.class_<luataxinode>("TaxiPathNode")
		.constructor< void(*)(uint32, float, float, float) >()
		.property_rw("x", (float(luataxinode::*))&TaxiNode::x)
		.property_rw("y", (float(luataxinode::*))&TaxiNode::y)
		.property_rw("z", (float(luataxinode::*))&TaxiNode::z)
		.property_rw("mapid", (uint32(luataxinode::*))&TaxiNode::mapid);

		m	.class_<TaxiNode>("TaxiNode")
		.property_ro("x", &TaxiNode::x)
		.property_ro("y", &TaxiNode::y)
		.property_ro("z", &TaxiNode::z)
		.property_ro("mapid", &TaxiNode::mapid)
		.property_ro("id", &TaxiNode::id)
		.property_ro("horde_mount", &TaxiNode::horde_mount)
		.property_ro("alliance_mount", &TaxiNode::alliance_mount);

		m	.class_<TaxiPath>("TaxiPath")
		.constructor<void(*)()>()
		.method(&TaxiPath::GetID, "GetID", "getID", "getid", NULL)
		.method(&TaxiPath::AddPathNode, "AddPathNode", "addPathNode", "addpathnode", NULL)
		.method(&TaxiPath::GetNodeCount, "GetNodeCount", "getNodeCount", "getnodecount", NULL)
		//.method("GetPathNode", &TaxiPath::GetPathNode)
		.method(&TaxiPath::GetPrice, "getPrice", "GetPrice", "getprice", NULL);
		//.method("SetPrice", &TaxiPath::SetPrice);

		m	.class_<TaxiMgr>("sTaxiMgr")
		.method(&TaxiMgr::GetTaxiNode, "GetTaxiNode", "getTaxiNode", "gettaxinode", NULL)
		.method((TaxiPath * (TaxiMgr::*)(uint32))&TaxiMgr::GetTaxiPath, "GetTaxiPath", "getTaxiPath", "gettaxipath", NULL)
		.method((TaxiPath * (TaxiMgr::*)(uint32, uint32))&TaxiMgr::GetTaxiPath, "GetTaxiPathFT", "getTaxiPathFT", "gettaxipathft", NULL)
		.method(&TaxiMgr::GetNearestTaxiNode, "GetNearestTaxiNode", "getNearestTaxiNode", "getnearesttaxinode", NULL);
		//overrite sTaxiMgr static table w/ the object.
		TaxiMgr* tmgr = TaxiMgr::getSingletonPtr();
		luabridge::tdstack<TaxiMgr*>::push(m.L, tmgr);
		//assign _G[sTaxiMgr] = TaxiMgr::getSingleton.
		lua_setglobal(m.L, "sTaxiMgr");
	}
}
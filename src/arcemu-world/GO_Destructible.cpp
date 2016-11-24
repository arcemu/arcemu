/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "StdAfx.h"

namespace Arcemu{

	GO_Destructible::GO_Destructible() : GameObject(){
	}

	GO_Destructible::GO_Destructible(uint64 GUID) : GameObject(GUID){
		hitpoints = 0;
		maxhitpoints = 0;
	}

	GO_Destructible::~GO_Destructible(){
	}
	void GO_Destructible::InitAI(){
		GameObject::InitAI();
		Rebuild();
	}

	void GO_Destructible::Damage(uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID){
		// If we are already destroyed there's nothing to damage!
		if(hitpoints == 0)
			return;

		if(damage >= maxhitpoints){
			// Instant destruction
			hitpoints = 0;
			SetFlags(GAMEOBJECT_FLAG_DESTROYED);
			SetDisplayId(pInfo->destructibleBuilding.DestroyedDisplayID);
		}else{
			// Simply damaging
			hitpoints -= damage;
			if(!HasFlags(GAMEOBJECT_FLAG_DAMAGED)){
				// Intact -> Damaged
				// Are we below the intact-damaged transition treshold?
				if(hitpoints <= (maxhitpoints - pInfo->destructibleBuilding.intactNumHits)){
					SetFlags(GAMEOBJECT_FLAG_DAMAGED);
					SetDisplayId(pInfo->destructibleBuilding.DamagedDisplayID);
				}
			}else{
				// Damaged -> Destroyed
				// Are we destroyed now?
				if(hitpoints == 0){
					SetFlags(GetFlags() & ~GAMEOBJECT_FLAG_DAMAGED);
					SetFlags(GAMEOBJECT_FLAG_DESTROYED);
					SetDisplayId(pInfo->destructibleBuilding.DestroyedDisplayID);
				}
			}
		}

		uint8 animprogress = static_cast< uint8 >(Arcemu::round(hitpoints / float(maxhitpoints)) * 255);

		SetAnimProgress(animprogress);
		SendDamagePacket(damage, AttackerGUID, ControllerGUID, SpellID);
	}

	void GO_Destructible::SendDamagePacket(uint32 damage, uint64 AttackerGUID, uint64 ControllerGUID, uint32 SpellID){
		WorldPacket data(SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, 29);

		data << WoWGuid(GetNewGUID());
		data << WoWGuid(AttackerGUID);
		data << WoWGuid(ControllerGUID);
		data << uint32(damage);
		data << uint32(SpellID);
		SendMessageToSet(&data, false, false);
	}

	void GO_Destructible::Rebuild(){
		SetFlags(GetFlags() & uint32(~(GAMEOBJECT_FLAG_DAMAGED | GAMEOBJECT_FLAG_DESTROYED)));
		SetDisplayId(pInfo->DisplayID);
		maxhitpoints = pInfo->destructibleBuilding.intactNumHits + pInfo->destructibleBuilding.damagedNumHits;
		hitpoints = maxhitpoints;
	}

}
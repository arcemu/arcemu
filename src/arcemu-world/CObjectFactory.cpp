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

	GameObject* CObjectFactory::CreateGameObject( uint32 Id, uint32 LowGUID ){
		GameObjectInfo *i = GameObjectNameStorage.LookupEntry(Id);
		if(i == NULL)
			return NULL;

		GameObject *go = NULL;

		uint64 GUID = uint64((uint64(HIGHGUID_TYPE_GAMEOBJECT) << 32) | LowGUID);
		
		switch(i->Type){

			case GAMEOBJECT_TYPE_DOOR:
				go = new Arcemu::GO_Door(GUID);
				break;

			case GAMEOBJECT_TYPE_BUTTON:
				go = new Arcemu::GO_Button(GUID);
				break;

			case GAMEOBJECT_TYPE_QUESTGIVER:
				go = new Arcemu::GO_QuestGiver(GUID);
				break;

			case GAMEOBJECT_TYPE_CHEST:
				go = new Arcemu::GO_Chest(GUID);
				break;

			case GAMEOBJECT_TYPE_TRAP:
				go = new Arcemu::GO_Trap(GUID);
				break;

			case GAMEOBJECT_TYPE_SPELL_FOCUS:
				go = new Arcemu::GO_SpellFocus(GUID);
				break;

			case GAMEOBJECT_TYPE_GOOBER:
				go = new Arcemu::GO_Goober(GUID);
				break;

			case GAMEOBJECT_TYPE_FISHINGNODE:
				go = new Arcemu::GO_FishingNode(GUID);
				break;

			case GAMEOBJECT_TYPE_RITUAL:
				go = new Arcemu::GO_Ritual(GUID);
				break;

			case GAMEOBJECT_TYPE_SPELLCASTER:
				go = new Arcemu::GO_SpellCaster(GUID);
				break;

			case GAMEOBJECT_TYPE_FISHINGHOLE:
				go = new Arcemu::GO_FishingHole(GUID);
				break;

			case GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING:
				go = new Arcemu::GO_Destructible(GUID);
				break;

			default:
				go = new GameObject(GUID);
				break;
		}

		go->SetInfo(i);

		return go;
	}

	void CObjectFactory::DisposeOf( Object *o ){
		delete o;
	}

}

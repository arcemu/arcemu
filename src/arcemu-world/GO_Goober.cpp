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
	GO_Door::GO_Door() : GameObject(){
	}

	GO_Door::GO_Door(uint64 GUID) : GameObject(GUID){
	}

	GO_Door::~GO_Door(){
	}

	void GO_Door::InitAI(){
		GameObject::InitAI();

		if(pInfo->door.startOpen != 0)
			SetState(0);
		else
			SetState(1);
	}

	void GO_Door::Open(){
		SetState(GAMEOBJECT_STATE_OPEN);

		if(pInfo->door.autoCloseTime != 0)
			sEventMgr.AddEvent(this, &GO_Door::Close, 0, pInfo->door.autoCloseTime, 1, 0);
	}

	void GO_Door::Close(){
		sEventMgr.RemoveEvents(this, EVENT_GAMEOBJECT_CLOSE);
		SetState(GAMEOBJECT_STATE_CLOSED);
	}

	void GO_Door::SpecialOpen(){
		SetState(GAMEOBJECT_STATE_ALTERNATIVE_OPEN);
	}
}
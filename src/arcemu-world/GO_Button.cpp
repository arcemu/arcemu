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
	GO_Button::GO_Button() : GameObject(){
	}

	GO_Button::GO_Button(uint64 GUID) : GameObject(GUID){
	}

	GO_Button::~GO_Button(){
	}

	void GO_Button::InitAI(){
		GameObject::InitAI();

		if(pInfo->button.startOpen != 0)
			SetState(GAMEOBJECT_STATE_OPEN);
	}

	void GO_Button::Open(){
		SetState(GAMEOBJECT_STATE_OPEN);

		if(pInfo->button.autoCloseTime != 0)
			sEventMgr.AddEvent(this, &GO_Button::Close, EVENT_GAMEOBJECT_CLOSE, pInfo->button.autoCloseTime, 1, 0);
	}

	void GO_Button::Close(){
		sEventMgr.RemoveEvents(this, EVENT_GAMEOBJECT_CLOSE);
		SetState(GAMEOBJECT_STATE_CLOSED);
	}
}
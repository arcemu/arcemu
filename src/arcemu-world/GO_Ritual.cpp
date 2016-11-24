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
	GO_Ritual::GO_Ritual(){
	}
	GO_Ritual::GO_Ritual(uint64 GUID) : GameObject(GUID){
		Ritual = NULL;
	}
	GO_Ritual::~GO_Ritual(){
		delete Ritual;
		Ritual = NULL;
	}
	void GO_Ritual::InitAI(){
		Ritual = new CRitual(pInfo->summoningRitual.reqParticipants);
	}
}
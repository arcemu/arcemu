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

	void CRitual::Setup(unsigned long CasterGUID, unsigned long TargetGUID, unsigned long SpellID){
		this->CasterGUID = CasterGUID;
		this->TargetGUID = TargetGUID;
		this->SpellID = SpellID;

		AddMember(CasterGUID);
	}

	bool CRitual::AddMember(unsigned long GUID){

		unsigned long i = 0;
		for(; i < MaxMembers; i++)
			if(Members[i] == 0)
				break;

		if(i == MaxMembers)
			return false;

		Members[i] = GUID;
		CurrentMembers++;

		return true;
	}

	bool CRitual::RemoveMember(unsigned long GUID){

		unsigned long i = 0;
		for(; i < MaxMembers; i++){
			if(Members[i] == GUID){
				Members[i] = 0;
				CurrentMembers--;
				return true;
			}
		}

		return false;
	}

	bool CRitual::HasMember(unsigned long GUID){

		for(unsigned long i = 0; i < MaxMembers; i++)
		if(Members[i] == GUID)
			return true;

		return false;
	}
}
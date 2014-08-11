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
	GO_SpellCaster::GO_SpellCaster() : GameObject(){
	}

	GO_SpellCaster::GO_SpellCaster(uint64 GUID) : GameObject(GUID){
		spell = NULL;
	}

	GO_SpellCaster::~GO_SpellCaster(){
	}

	void GO_SpellCaster::InitAI(){
		charges = pInfo->spellcaster.charges;

		spell = dbcSpell.LookupEntry(pInfo->spellcaster.spellId);
		if(spell == NULL)
			sLog.outError("GameObject %u ( %s ) has a nonexistant spellID in the database.", pInfo->ID, pInfo->Name);
	}

	void GO_SpellCaster::Use(uint64 GUID){
		if(spell == NULL)
			return;

		CastSpell(GUID, spell);

		if((charges > 0) && (--charges == 0))
			ExpireAndDelete();
	}
}
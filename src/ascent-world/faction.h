/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#ifndef __FACTION_H
#define __FACTION_H

#include "Unit.h"

SERVER_DECL bool isHostile(Object* objA, Object* objB); // B is hostile for A?
SERVER_DECL bool isAttackable(Object* objA, Object* objB, bool CheckStealth = true); // A can attack B?
SERVER_DECL bool isCombatSupport(Object* objA, Object* objB); // B combat supports A?;
SERVER_DECL bool isAlliance(Object* objA); // A is alliance?

ASCENT_INLINE bool isFriendly(Object* objA, Object* objB)// B is friendly to A if its not hostile
{
	return !isHostile(objA, objB);
}

ASCENT_INLINE bool isSameFaction(Object* objA, Object* objB)
{
	return (objB->m_faction->Faction == objA->m_faction->Faction);
}

#endif

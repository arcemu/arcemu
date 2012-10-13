/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef __AIEVENTS_H
#define __AIEVENTS_H

class AIInterface;
typedef void(AIInterface::*pAIEvent)(Unit* punit, uint32 misc1);

enum AiEvents
{
    EVENT_ENTERCOMBAT,
    EVENT_LEAVECOMBAT,
    EVENT_DAMAGETAKEN,
    EVENT_FEAR,
    EVENT_UNFEAR,
    EVENT_FOLLOWOWNER,
    EVENT_WANDER,
    EVENT_UNWANDER,
    EVENT_UNITDIED,
    EVENT_HOSTILEACTION,
    EVENT_FORCEREDIRECTED,
    NUM_AI_EVENTS,
};

extern pAIEvent AIEventHandlers[NUM_AI_EVENTS];



#endif

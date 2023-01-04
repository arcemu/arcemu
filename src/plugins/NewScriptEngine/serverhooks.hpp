/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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

#ifndef SERVERHOOKS_H_
#define SERVERHOOKS_H_

extern void python_hookOnEnterWorld( Player* player );
extern void python_hookOnPlayerDeath( Player* player );
extern void python_hookOnPlayerRepop( Player* player );
extern void python_hookOnEmote( Player* player, uint32 emote, Unit* unit );
extern void python_hookOnEnterCombat( Player* player, Unit* unit );

#endif

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
 
 #ifndef SERVERHOOK_HANDLER_H_
 #define SERVERHOOK_HANDLER_H_

class ServerHookHandler
{
public:
	static void hookOnKillPlayer( Player* killer, Player *victim );
	static void hookOnFirstEnterWorld( Player *player );
	static void hookOnEnterWorld( Player* player );
	static void hookOnPlayerDeath( Player* player );
	static void hookOnPlayerRepop( Player* player );
	static void hookOnEmote( Player* player, uint32 emote, Unit* unit );
	static void hookOnEnterCombat( Player* player, Unit* unit );

	static void hookOnLogoutRequest( Player* player );

	static void hookOnPreUnitDie( Unit* killer, Unit *victim );

	static void hookOnPlayerResurrect( Player* player );	
};

#endif

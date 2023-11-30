/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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

#ifndef _PLAYERMESSENGER_H
#define _PLAYERMESSENGER_H

/// Knows how to send a message to a player
class PlayerMessenger
{
public:
	/// Send a message to the player. Checks if player has a session.
	static void sendMessage( Player* player, WorldPacket& message );

	/// Send a delayed message to the player (they are sent after updates). Takes responsibility and packet is deleted by the underlying code.
	static void sendDelayedMessage( Player* player, WorldPacket* message );

	/// Makes a copy of the message and then sends a delayed message to the player (sent after the updates)
	static void sendDelayedMessage( Player* player, WorldPacket& message );
};

#endif

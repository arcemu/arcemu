/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#ifndef _PLAYERCOMMANDS_H
#define _PLAYERCOMMANDS_H

/// Gets the talent reset cost calculated and gets the talent reset confirmation dialog message sent
class SERVER_DECL SendTalentResetConfirmDialogCommand
{
private:
	Player* player;

public:
	SendTalentResetConfirmDialogCommand( Player* player );

	void execute();
};


/// Resets the player's active pet's talents.
class SERVER_DECL ResetPetTalentsCommand
{
private:
	Player* player;

public:
	ResetPetTalentsCommand( Player* player ){ this->player = player; }

	void execute();
};

#endif

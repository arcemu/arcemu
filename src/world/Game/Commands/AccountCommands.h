/*
 * ArcEmu MMORPG Server
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

#ifndef ARCEMU_ACCOUNTCOMMANDS_H_
#define ARCEMU_ACCOUNTCOMMANDS_H_

class SERVER_DECL SendAccountDataTimesCommand
{
public:
	SendAccountDataTimesCommand( WorldSession *session, uint32 mask )
	{
		this->session = session;
		this->mask = mask;
	}

	void execute();

private:
	WorldSession *session;
	uint32 mask;
};

#endif

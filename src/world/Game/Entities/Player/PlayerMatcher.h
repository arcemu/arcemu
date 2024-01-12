/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#ifndef PLAYERMATCHER_H
#define PLAYERMATCHER_H

class SERVER_DECL PlayerMatcher
{
public:
	virtual ~PlayerMatcher(){}
	virtual bool match( Player *player ) = 0;
};

class SERVER_DECL TeamAndZoneMatcher : public PlayerMatcher
{
private:
	uint32 zoneId;
	uint32 team;

public:
	TeamAndZoneMatcher( uint32 zoneId, uint32 team );
	bool match( Player *player );
};

#endif

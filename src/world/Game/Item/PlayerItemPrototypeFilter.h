/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

#ifndef ARCEMU_PLAYER_ITEMPROTO_FILTER
#define ARCEMU_PLAYER_ITEMPROTO_FILTER

/// Filters item prototypes based on player usability criteria
class PlayerItemPrototypeFilter : public ItemPrototypeFilter
{
public:
	PlayerItemPrototypeFilter( const Player *player )
	{
		_player = player;
	}

	bool filter( const ItemPrototype *proto );

private:
	const Player *_player;
};

#endif

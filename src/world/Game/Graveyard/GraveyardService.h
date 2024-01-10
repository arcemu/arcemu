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

#ifndef GRAVEYARD_SERVICE_H_
#define GRAVEYARD_SERVICE_H_

class SERVER_DECL GraveyardService : public Singleton<GraveyardService>
{
public:
	/// Finds the closest graveyard
	GraveyardTeleport* findClosest( uint32 mapId, LocationVector &location, uint32 team );
};

#define sGraveyardService GraveyardService::getSingleton()

#endif

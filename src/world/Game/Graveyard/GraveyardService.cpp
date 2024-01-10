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

#include "StdAfx.h"

initialiseSingleton( GraveyardService );

#define MAX_DISTANCE_VARIANCE 5.0f

GraveyardTeleport* GraveyardService::findClosest( uint32 mapId, LocationVector &location, uint32 team )
{
	Guard guard( lock );

	GraveyardTeleport *closestGrave = NULL;
	LocationVector temp;
	float closest = std::numeric_limits< float >::max();
	
	StorageContainerIterator< GraveyardTeleport > *itr = GraveyardStorage.MakeIterator();
	while( !itr->AtEnd() )
	{
		GraveyardTeleport *grave = itr->Get();
		
		if( ( grave->MapId == mapId ) && ( ( grave->FactionID == team ) || ( grave->FactionID == 3 ) ) )
		{
			temp.ChangeCoords( grave->X, grave->Y, grave->Z );
			float d = location.DistanceSq( temp );
			if( d < closest )
			{
				closest = d;
				closestGrave = grave;
			}
		}
		
		if( !itr->Inc() )
		{
			break;
		}
	
	}
	
	itr->Destruct();

	return closestGrave;
}

bool GraveyardService::setGraveyardOwner( uint32 mapId, LocationVector &location, uint32 newOwner )
{
	Guard guard( lock );

	GraveyardTeleport *closestGrave = NULL;
	LocationVector temp;
	float closest = std::numeric_limits< float >::max();
	
	StorageContainerIterator< GraveyardTeleport > *itr = GraveyardStorage.MakeIterator();
	while( !itr->AtEnd() )
	{
		GraveyardTeleport *grave = itr->Get();
		
		if( grave->MapId == mapId )
		{
			temp.ChangeCoords( grave->X, grave->Y, grave->Z );
			float d = location.DistanceSq( temp );
			if( d < closest )
			{
				closest = d;
				closestGrave = grave;
			}
		}
		
		if( !itr->Inc() )
		{
			break;
		}
	
	}
	
	itr->Destruct();

	if( closest < MAX_DISTANCE_VARIANCE )
	{
		closestGrave->FactionID = newOwner;
		return true;
	}

	return false;
}

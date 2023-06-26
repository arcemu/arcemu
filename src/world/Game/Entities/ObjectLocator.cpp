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

#include "StdAfx.h"

Unit* ObjectLocator::findClosestFriendly()
{
	if( object == NULL )
		return NULL;

	double closestDistance = std::numeric_limits< double >::max();
	Unit *closestUnit = NULL;

	std::set< Object* > &objects = object->GetInRangeObjects();

	std::set< Object* >::iterator itr = objects.begin();
	while( itr != objects.end() )
	{
		Object *obj = *itr;

		if( obj->IsUnit() )
		{
			if( !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE ) && !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 ) )
			{
				if( isFriendly( object, obj ) )
				{
					double distance = object->GetDistanceSq( obj );
					if( distance < closestDistance )
					{
						closestDistance = distance;
						closestUnit = TO_UNIT( obj );
					}
				}
			}
		}

		++itr;
	}

	return closestUnit;
}

Unit* ObjectLocator::getRandomFriendly()
{
	if( object == NULL )
		return NULL;

	std::vector< Unit* > units;

	std::set< Object* > &objects = object->GetInRangeObjects();

	std::set< Object* >::iterator itr = objects.begin();
	while( itr != objects.end() )
	{
		Object *obj = *itr;

		if( obj->IsUnit() )
		{
			if( !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE ) && !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 ) )
			{
				if( isFriendly( object, obj ) )
				{
					units.push_back( TO_UNIT( obj ) );
				}
			}
		}

		++itr;
	}

	if( units.empty() )
		return NULL;

	uint32 idx = RandomUInt( units.size() -1 );
	return units[ idx ];
}

Unit* ObjectLocator::findClosestEnemy()
{
	if( object == NULL )
		return NULL;

	double closestDistance = std::numeric_limits< double >::max();
	Unit *closestUnit = NULL;

	std::set< Object* > &objects = object->GetInRangeObjects();

	std::set< Object* >::iterator itr = objects.begin();
	while( itr != objects.end() )
	{
		Object *obj = *itr;

		if( obj->IsUnit() )
		{
			if( !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE ) && !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 ) )
			{
				if( isHostile( object, obj ) )
				{
					double distance = object->GetDistanceSq( obj );
					if( distance < closestDistance )
					{
						closestDistance = distance;
						closestUnit = TO_UNIT( obj );
					}
				}
			}
		}

		++itr;
	}

	return closestUnit;
}

Unit* ObjectLocator::getRandomEnemy()
{
	if( object == NULL )
		return NULL;

	std::vector< Unit* > units;

	std::set< Object* > &objects = object->GetInRangeObjects();

	std::set< Object* >::iterator itr = objects.begin();
	while( itr != objects.end() )
	{
		Object *obj = *itr;

		if( obj->IsUnit() )
		{
			if( !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE ) && !obj->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 ) )
			{
				if( isHostile( object, obj ) )
				{
					units.push_back( TO_UNIT( obj ) );
				}
			}
		}

		++itr;
	}

	if( units.empty() )
		return NULL;

	uint32 idx = RandomUInt( units.size() -1 );
	return units[ idx ];
}


Player* ObjectLocator::findClosestPlayer()
{
	if( object == NULL )
		return NULL;

	double closestDistance = std::numeric_limits< double >::max();
	Player *closestPlayer = NULL;

	std::set< Object* > &objects = object->GetInRangeObjects();

	std::set< Object* >::iterator itr = objects.begin();
	while( itr != objects.end() )
	{
		Object *obj = *itr;

		if( obj->IsPlayer() )
		{
			double distance = object->GetDistanceSq( obj );
			if( distance < closestDistance )
			{
				closestDistance = distance;
				closestPlayer = TO_PLAYER( obj );
			}
		}

		++itr;
	}

	return closestPlayer;
}


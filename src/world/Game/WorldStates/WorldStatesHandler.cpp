/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

void WorldStatesHandler::SetWorldStateForZone( uint32 zone, uint32 field, uint32 value ){
	HM_NAMESPACE::HM_HASH_MAP< uint32, HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > >::iterator itr
		= worldstates.find( zone );

	if( itr == worldstates.end() )
		return;

	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::iterator itr2 = itr->second.find( field );
	if( itr2 == itr->second.end() )
		return;

	itr2->second = value;

	if( observer != NULL )
		observer->onWorldStateUpdate( zone, field, value );
}

uint32 WorldStatesHandler::GetWorldStateForZone( uint32 zone, uint32 field ) const{
	HM_NAMESPACE::HM_HASH_MAP< uint32, HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > >::const_iterator itr
		= worldstates.find( zone );

	if( itr == worldstates.end() )
		return 0;

	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::const_iterator itr2 = itr->second.find( field );
	if( itr2 == itr->second.end() )
		return 0;
	
	return itr2->second;
}

void WorldStatesHandler::BuildInitWorldStatesForZone( uint32 zone, uint32 area, WorldPacket &data ) const{
	data << uint32( map );
	data << uint32( zone );
	data << uint32( area );

	HM_NAMESPACE::HM_HASH_MAP< uint32, HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > >::const_iterator itr
		= worldstates.find( zone );

	if( itr != worldstates.end() ){
		data << uint16( 2 + itr->second.size() );

		for( HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2 ){
			data << uint32( itr2->first );
			data << uint32( itr2->second );
		}

	}else{
		data << uint16( 2 );
	}

	data << uint32( WORLDSTATE_ARENA_SEASON );
	data << uint32( sWorld.Arena_Season );
	data << uint32( WORLDSTATE_ARENA_PROGRESS );
	data << uint32( sWorld.Arena_Progress );
}

void WorldStatesHandler::InitWorldStates( std::multimap< uint32,WorldState > *states ){
	if( states == NULL )
		return;

	for( std::multimap< uint32, WorldState >::iterator itr = states->begin(); itr != states->end(); ++itr ){
		uint32 zone = itr->first;
		worldstates[ zone ];
		worldstates[ zone ].insert( std::pair< uint32, uint32 >( itr->second.field, itr->second.value ) );
	}
}

void WorldStatesHandler::addWorldState( uint32 zoneId, WorldState state )
{
	worldstates[ zoneId ];
	worldstates[ zoneId ].insert( std::pair< uint32, uint32 >( state.field, state.value ) );
	
	if( observer != NULL )
	{
		observer->onWorldStatesReinitialization( zoneId );
	}
}

void WorldStatesHandler::removeWorldState( uint32 zoneId, uint32 field )
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > >::iterator zoneItr = worldstates.find( zoneId );
	if( zoneItr == worldstates.end() )
		return;

	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > &states = zoneItr->second;

	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::iterator statesItr = states.find( field );
	if( statesItr == states.end() )
		return;

	states.erase( statesItr );

	if( states.size() == 0 )
	{
		worldstates.erase( zoneItr );
	}
	
	if( observer != NULL )
	{
		observer->onWorldStatesReinitialization( zoneId );
	}
}

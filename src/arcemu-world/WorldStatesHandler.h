/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

#ifndef WORLDSTATEHANDLER_H
#define WORLDSTATEHANDLER_H

struct WorldState;

class SERVER_DECL WorldStatesHandler{
	public:
		WorldStatesHandler( uint32 mapid, IEventListener *listener ){
			map = mapid;
			eventlistener = listener;
		}

		~WorldStatesHandler(){}


		////////////////////////////////////////////////////////////////////////////////////////////
		//void SetWorldStateForZone( uint32 zone, uint32 area, uint32 field, uint32 value )
		//  Sets the specified worldstate's value for the specified zone
		//
		//Parameter(s)
		//  uint32 zone  -  the zone where we set the worldstate
		//  uint32 area  -  the area where we set the worldstate
		//  uint32 field -  the worldstate field we are setting
		//  uint32 value -  the value we assign to the field
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////
		void SetWorldStateForZone( uint32 zone, uint32 area, uint32 field, uint32 value );


		////////////////////////////////////////////////////////////////////////////////////////////
		//uint32 GetWorldStateForZone( uint32 zone, uint32 area, uint32 field ) const;
		//  Returns the value of the worldstate field queried.
		//
		//Parameter(s)
		//  uint32 zone  -  the zone where we are querying
		//  uint32 area  -  the area where we querying
		//  uint32 field -  the field that we querying
		//
		//Return Value
		//  Returns the value of the queried field.
		//  Returns 0 even if there is no such field.
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////
		uint32 GetWorldStateForZone( uint32 zone, uint32 area, uint32 field ) const;


		////////////////////////////////////////////////////////////////////////////////////////////
		//void BuildInitWorldStatesForZone( uint32 zone, uint32 area, WorldPacket &data ) const;
		//  Builds the initial worldstates packet, that tells the client what worldstates exist
		//
		//Parameter(s)
		//  uint32 zone        -  The zone we are building the packet for
		//  uint32 area        -  The area we are building the packet for
		//  WorldPacket &data  -  The packet we will fill with the worldstates data
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////
		void BuildInitWorldStatesForZone( uint32 zone, uint32 area, WorldPacket &data ) const;


		////////////////////////////////////////////////////////////////////////////////////////////
		//void InitWorldStates( std::multimap< uint32, WorldState > *states );
		//  Sets up this worldstate handler with the initial data
		//
		//Parameter(s)
		//  std::multimap< uint32, WorldState > *states  -  The source of the initial data
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////
		void InitWorldStates( std::multimap< uint32, WorldState > *states );

	private:
		HM_NAMESPACE::hash_map< uint32, HM_NAMESPACE::hash_map< uint32, uint32 > > worldstates;
		uint32 map;
		IEventListener *eventlistener;
};

#endif

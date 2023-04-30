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

void WorldSession::HandleDismissVehicle( WorldPacket &recv_data ){
	uint64 current_vehicle_guid = _player->GetCharmedUnitGUID();

	// wait what no vehicle
	if( current_vehicle_guid == 0 )
		return;

	Unit *v = _player->GetMapMgr()->GetUnit( current_vehicle_guid );
	if( v == NULL )
		return;

	Vehicle *vehicle = v->GetVehicleComponent();
	if( vehicle == NULL )
		return;

	if( ! vehicle->canPassengerExit( _player ) )
		return;

	vehicle->EjectPassenger( _player );
}


void WorldSession::HandleChangeVehicleSeat( WorldPacket &recv_data ){
	if( _player->GetCurrentVehicle() == NULL )
		return;

	switch( recv_data.GetOpcode() ){
		case CMSG_REQUEST_VEHICLE_PREV_SEAT:
			_player->GetCurrentVehicle()->MovePassengerToPrevSeat( _player );
			break;

		case CMSG_REQUEST_VEHICLE_NEXT_SEAT:
			_player->GetCurrentVehicle()->MovePassengerToNextSeat( _player );
			break;

		// Used when switching from a normal seat to a controlling seat, or to an accessory
		case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:{
			WoWGuid vehicle;
			uint8 seat = 0;
			
			recv_data >> vehicle;
			recv_data >> seat;

			if( vehicle.GetOldGuid() == _player->GetCurrentVehicle()->GetOwner()->GetGUID() ){
				_player->GetCurrentVehicle()->MovePassengerToSeat( _player, seat );
			}else{
				Unit *u = _player->GetMapMgr()->GetUnit( vehicle.GetOldGuid() );
				if( u == NULL )
					return;

				if( u->GetVehicleComponent() == NULL )
					return;

				// Has to be same vehicle, or an accessory of the vehicle
				if( _player->GetVehicleBase()->GetGUID() != u->GetVehicleBase()->GetGUID() )
					return;

				_player->GetCurrentVehicle()->EjectPassenger( _player );
				u->GetVehicleComponent()->AddPassengerToSeat( _player, seat );
			}

			break;}

	    // Used when switching from controlling seat to accessory, or from accessory to accessory
		case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE:{
			WoWGuid src_guid;
			WoWGuid dst_guid;
			uint8 seat = 0;			
			MovementInfo mov;

			recv_data >> src_guid;
 			mov.init( recv_data );
			recv_data >> dst_guid;
			recv_data >> seat;
			
			Unit *src_vehicle = _player->GetMapMgr()->GetUnit( src_guid.GetOldGuid() );
			if( src_vehicle == NULL )
				return;
			
			if( src_vehicle->GetVehicleComponent() == NULL )
				return;
			
			if( src_vehicle->GetGUID() != _player->GetCurrentVehicle()->GetOwner()->GetGUID() )
				return;
			
			Unit *dst_vehicle = _player->GetMapMgr()->GetUnit( dst_guid.GetOldGuid() );
			if( dst_vehicle == NULL )
				return;
			
			if( dst_vehicle->GetVehicleComponent() == NULL )
				return;
			
			if( src_vehicle->GetGUID() == dst_vehicle->GetGUID() ){
				src_vehicle->GetVehicleComponent()->MovePassengerToSeat( _player, seat );
			}else{
				// Has to be the same vehicle or an accessory of the vehicle
				if( src_vehicle->GetVehicleBase()->GetGUID() != dst_vehicle->GetVehicleBase()->GetGUID() )
					return;

				_player->GetCurrentVehicle()->EjectPassenger( _player );
				dst_vehicle->GetVehicleComponent()->AddPassengerToSeat( _player, seat );
			}

			break;}
	}
}


void WorldSession::HandleRemoveVehiclePassenger( WorldPacket &recv_data ){
	Vehicle *v = NULL;
	if( _player->IsVehicle() )
		v = _player->GetVehicleComponent();
	else
		v = _player->GetCurrentVehicle();

	if( v == NULL )
		return;

	uint64 guid = 0;
	recv_data >> guid;

	if( guid == 0 )
		return;

	Unit *passenger = _player->GetMapMgr()->GetUnit( guid );
	if( passenger == NULL )
		return;

	if( ! v->canPassengerExit( passenger ) )
		return;

	v->EjectPassenger( passenger );
}


void WorldSession::HandleLeaveVehicle( WorldPacket &recv_data ){
	Vehicle *vehicle = _player->GetCurrentVehicle();
	if( vehicle == NULL )
		return;

	if( ! vehicle->canPassengerExit( _player ) )
		return;

	vehicle->EjectPassenger( _player );
}


void WorldSession::HandleEnterVehicle( WorldPacket &recv_data ){
	uint64 guid;

	recv_data >> guid;

	Unit *v = _player->GetMapMgr()->GetUnit( guid );
	if( v == NULL )
		return;

	if( !_player->isInRange( v, MAX_INTERACTION_RANGE ) )
		return;

	if( v->GetVehicleComponent() == NULL )
		return;

	v->GetVehicleComponent()->AddPassenger( _player );
}



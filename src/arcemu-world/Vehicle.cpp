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

Vehicle::Vehicle(){
	owner = NULL;
	vehicle_info = NULL;
	passengercount = 0;
	freeseats = 0;
	std::fill( seats.begin(), seats.end(), reinterpret_cast< VehicleSeat* >( NULL ) );
	installed_accessories.clear();
}

Vehicle::~Vehicle(){
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		delete seats[ i ];

	installed_accessories.clear();
}


void Vehicle::Load( Unit *owner, uint32 creature_entry, uint32 vehicleid ){
	if( owner == NULL ){
		LOG_ERROR( "Can't load vehicle without an owner." );
		ARCEMU_ASSERT( false );
	}

	vehicle_info = dbcVehicle.LookupEntry( vehicleid );
	if( vehicle_info == NULL ){
		LOG_ERROR( "Can't load a vehicle without vehicle id or data belonging to it." );
		ARCEMU_ASSERT( false );
	}

	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ ){
		uint32 seatid = vehicle_info->seatID[ i ];

		if( seatid != 0 ){
			VehicleSeatEntry *seatinfo = dbcVehicleSeat.LookupEntry( seatid );
			if( seatinfo == NULL ){
				LOG_ERROR( "Invalid seat id %u for seat %u for vehicle id %u", seatid, i, vehicleid );
				continue;
			}

			seats[ i ] = new VehicleSeat( seatinfo );
		}			
	}

	this->creature_entry = creature_entry;
	this->owner = owner;
	
	switch( vehicle_info->powerType ){
	case POWER_TYPE_STEAM:
	case POWER_TYPE_HEAT:
	case POWER_TYPE_BLOOD:
	case POWER_TYPE_OOZE:
	case POWER_TYPE_WRATH:
		owner->SetPowerType( POWER_TYPE_ENERGY );
		owner->SetMaxPower( POWER_TYPE_ENERGY, 100 );
		owner->SetPower( POWER_TYPE_ENERGY, 100 );
		break;
	
	case POWER_TYPE_PYRITE:
		owner->SetPowerType( POWER_TYPE_ENERGY );
		owner->SetMaxPower( POWER_TYPE_ENERGY, 50 );
		owner->SetPower( POWER_TYPE_ENERGY, 50 );
		break;
	}

	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && seats[ i ]->Usable() && ( !seats[ i ]->HasPassenger() ) )
			freeseats++;

}

bool Vehicle::HasEmptySeat(){
	if( freeseats > 0 )
		return true;
	else
		return false;
}

void Vehicle::AddPassenger( Unit *passenger ){

	// find seat
	uint32 seatid = MAX_VEHICLE_SEATS;
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && seats[ i ]->Usable() && ( !seats[ i ]->HasPassenger() ) ){
			seatid = i;
			break;
		}

	// There wasn't one :(
	if( seatid == MAX_VEHICLE_SEATS )
		return;

	AddPassengerToSeat( passenger, seatid );
}

void Vehicle::AddPassengerToSeat( Unit *passenger, uint32 seatid ){
	if( seats[ seatid ]->HasPassenger() )
		return;

	if( !seats[ seatid ]->Usable() )
		return;

	passenger->RemoveAllAuraType( SPELL_AURA_MOUNTED );

	if( passenger->IsPlayer() )
		static_cast< Player* >( passenger )->DismissActivePets();

	if( passenger->GetCurrentVehicle() != NULL )
		passenger->GetCurrentVehicle()->EjectPassenger( passenger );

	// set moveflags
	// set movement info
	
	// root passenger
	passenger->Root();

	WorldPacket ack( 0x049D );
	passenger->SendPacket( &ack );

	passenger->SendHopOnVehicle( owner, seatid );

	LocationVector v( owner->GetPosition() );
	v.x += seats[ seatid ]->GetSeatInfo()->attachmentOffsetX;
	v.y += seats[ seatid ]->GetSeatInfo()->attachmentOffsetY;
	v.z += seats[ seatid ]->GetSeatInfo()->attachmentOffsetZ;

	passenger->SetPosition( v, false );

	// Player's client sets these
	if( passenger->IsCreature() ){
		passenger->transporter_info.guid = owner->GetGUID();
		passenger->transporter_info.seat = seatid;
	}

	if( passenger->IsPlayer() ){
		WorldPacket ack( SMSG_CONTROL_VEHICLE, 0);
		passenger->SendPacket( &ack );

		passenger->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE );

		static_cast< Player* >( passenger )->SetFarsightTarget( owner->GetGUID() );

		if( seats[ seatid ]->Controller() ){
			ack.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
			ack << owner->GetNewGUID() << uint8(1);
			passenger->SendPacket(&ack);

			passenger->SetCharmedUnitGUID( owner->GetGUID() );
			owner->SetCharmedByGUID( passenger->GetGUID() );
			owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );

			WorldPacket spells( SMSG_PET_SPELLS, 100 );
			owner->BuildPetSpellList( spells );
			passenger->SendPacket( &spells );
		}		
	}

	seats[ seatid ]->AddPassenger( passenger->GetGUID() );
	passenger->SetCurrentVehicle( this );

	if( seats[ seatid ]->HidesPassenger() )
		passenger->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 );

	passengercount++;
	freeseats--;

	// remove spellclick flag if full
	if( !HasEmptySeat() ){
		owner->RemoveFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK );
		owner->RemoveFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE );

	}

	if( passenger->IsCreature() ){
		Creature *c = static_cast< Creature* >( passenger );
		
		if( c->GetScript() != NULL ){
			c->GetScript()->OnEnterVehicle();
		}
	}

	if( owner->IsCreature() ){
		Creature *c = static_cast< Creature* >( owner );

		if( c->GetScript() != NULL ){
			if( passengercount == 1 )
				c->GetScript()->OnFirstPassengerEntered( passenger );

			if( !HasEmptySeat() )
				c->GetScript()->OnVehicleFull();
		}
	}
}

void Vehicle::EjectPassenger( Unit *passenger ){
	if( passenger->GetCurrentVehicle() == NULL )
		return;

	if( passenger->GetCurrentVehicle() != this )
		return;

	// find the seat the passenger is on
	uint32 seatid = MAX_VEHICLE_SEATS;
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != 0 ) && seats[ i ]->Usable() && seats[ i ]->HasPassenger() && ( seats[ i ]->GetPassengerGUID() == passenger->GetGUID() ) ){
			seatid = i;
			break;
		}

	if( seatid == MAX_VEHICLE_SEATS )
		return;

	EjectPassengerFromSeat( seatid );
}

void Vehicle::EjectPassengerFromSeat( uint32 seatid ){
	if( !seats[ seatid ]->Usable() )
		return;

	if( !seats[ seatid ]->HasPassenger() )
		return;

	Unit *passenger = owner->GetMapMgrUnit( seats[ seatid ]->GetPassengerGUID() );
	if( passenger == NULL )
		return;

	// set moveflags
	// set movement info

	// remove charmed by if passenger was controller
	if( seats[ seatid ]->Controller() ){
		passenger->SetCharmedUnitGUID( 0 );
		owner->SetCharmedByGUID( 0 );
		
		if( passenger->IsPlayer() ){

			owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE | UNIT_FLAG_PVP_ATTACKABLE );

			WorldPacket ack( SMSG_CLIENT_CONTROL_UPDATE, 16 );
			ack << owner->GetNewGUID();
			ack << uint8( 0 );
			passenger->SendPacket(&ack);

			// send null spells if needed
			static_cast< Player* >( passenger )->SendEmptyPetSpellList();
		}
	}	

	if( passenger->IsPlayer() )
		static_cast< Player* >( passenger )->SetFarsightTarget( 0 );

	// if we are on a flying vehicle, add a parachute!
	if( owner->HasAuraWithName( SPELL_AURA_ENABLE_FLIGHT ) || owner->HasAuraWithName( SPELL_AURA_ENABLE_FLIGHT2 ) )
		passenger->CastSpell( passenger, 45472, false );

	// re-add spellclick flag if needed	
	// despawn vehicle if it was spawned by spell?
	LocationVector landposition( owner->GetPosition() );

	passenger->SendHopOffVehicle( owner, landposition );
	passenger->SetPosition( landposition );	
	passenger->Unroot();
	seats[ seatid ]->RemovePassenger();
	passenger->SetCurrentVehicle( NULL );
	passenger->RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_ATTACKABLE_2 );

	passengercount--;
	freeseats++;

	if( HasEmptySeat() ){
		if( owner->IsPlayer() )
			owner->SetFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE );
		else
			owner->SetFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK );
	}

	if( passenger->IsPlayer() )
		static_cast< Player* >( passenger )->SpawnActivePet();

	if( passenger->IsCreature() ){
		Creature *c = static_cast< Creature* >( passenger );

		if( c->GetScript() != NULL ){
			c->GetScript()->OnExitVehicle();
		}
	}
	if( owner->IsCreature() ){
		Creature *c = static_cast< Creature* >( owner );

		if( c->GetScript() != NULL ){
			if( passengercount == 0 )
				c->GetScript()->OnLastPassengerLeft( passenger );
		}else{
			// The passenger summoned the vehicle, and we have no script to remove it, so we remove it here
			if( ( passengercount == 0 ) && ( c->GetSummonedByGUID() == passenger->GetGUID() ) )
				c->Despawn( 1 * 1000, 0 );
		}
	}
}

void Vehicle::EjectAllPassengers(){
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() != 0 ) ){
			Unit *u = owner->GetMapMgr()->GetUnit( seats[ i ]->GetPassengerGUID() );
			if( u == NULL ){
				seats[ i ]->RemovePassenger();
				continue;
			}

			if( u->GetVehicleComponent() != NULL )
				u->GetVehicleComponent()->EjectAllPassengers();
			else
				EjectPassengerFromSeat( i );
		}
}

void Vehicle::MovePassengerToSeat( Unit *passenger, uint32 seat ){
	uint32 oldseatid = 0;
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() == passenger->GetGUID() ) ){
			oldseatid = i;
			break;
		}

	// Passenger is not in this vehicle
	if( oldseatid == MAX_VEHICLE_SEATS )
		return;

	if( seats[ seat ] == NULL )
		return;

	if( !seats[ seat ]->Usable() )
		return;

	if( seats[ seat ]->HasPassenger() )
		return;

	EjectPassengerFromSeat( oldseatid );
	AddPassengerToSeat( passenger, seat );
}

void Vehicle::MovePassengerToNextSeat( Unit *passenger ){
	uint32 oldseatid = 0;
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() == passenger->GetGUID() ) ){
			oldseatid = i;
			break;
		}

	// Passenger is not in this vehicle
	if( oldseatid == MAX_VEHICLE_SEATS )
		return;

	// Now find a next seat if possible
	uint32 newseatid = oldseatid;
	for( uint32 i = oldseatid + 1; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->Usable() ) && ( !seats[ i ]->HasPassenger() ) ){
			newseatid = i;
			break;
		}

	// There's no suitable seat :(
	if( newseatid == oldseatid )
		return;

	EjectPassengerFromSeat( oldseatid );
	AddPassengerToSeat( passenger, newseatid );
}

void Vehicle::MovePassengerToPrevSeat( Unit *passenger ){
	uint32 oldseatid = MAX_VEHICLE_SEATS;
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() == passenger->GetGUID() ) ){
			oldseatid = i;
			break;
		}

	// Passenger is not in this vehicle
	if( oldseatid == MAX_VEHICLE_SEATS )
		return;

	// Now find a previous seat if possible
	uint32 newseatid = oldseatid;
	for( int32 i = static_cast< int32 >( oldseatid ) - 1; i >= 0; i-- )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->Usable() ) && ( !seats[ i ]->HasPassenger() ) ){
			newseatid = static_cast< uint32 >( i );
			break;
		}

	// There's no suitable seat :(
	if( newseatid == oldseatid )
		return;

	EjectPassengerFromSeat( oldseatid );
	AddPassengerToSeat( passenger, newseatid );
}

uint32 Vehicle::GetSeatEntryForPassenger( Unit *passenger ){
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ )
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() == passenger->GetGUID() ) )
			return seats[ i ]->GetSeatInfo()->ID;

	return 0;
}

void Vehicle::MovePassengers( float x, float y, float z, float o ){
	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ ){
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() != 0 ) ){
			Unit *passenger = owner->GetMapMgrUnit( seats[ i ]->GetPassengerGUID() );
			if( passenger == NULL )
				continue;

			passenger->SetPosition( x, y, z, o );
		}
	}
}


uint32 Vehicle::GetPassengerCount() const{
	uint32 count = 0;

	for( uint32 i = 0; i < MAX_VEHICLE_SEATS; i++ ){
		if( ( seats[ i ] != NULL ) && ( seats[ i ]->GetPassengerGUID() != 0 ) ){
			Unit *passenger = owner->GetMapMgrUnit( seats[ i ]->GetPassengerGUID() );
			if( passenger == NULL )
				continue;

			if( passenger->GetVehicleComponent() == NULL )
				count++;
			else
				count += passenger->GetVehicleComponent()->GetPassengerCount();
		}
	}

	return count;
}

uint16 Vehicle::GetMoveFlags2() const{
	uint16 flags2 = 0;

	if( vehicle_info->flags & VEHICLE_FLAG_NO_STRAFE )
		flags2 |= MOVEFLAG2_NO_STRAFING;

	if( vehicle_info->flags & VEHICLE_FLAG_NO_JUMPING )
		flags2 |= MOVEFLAG2_NO_JUMPING;
	if( vehicle_info->flags & VEHICLE_FLAG_FULLSPEEDTURNING )
		flags2 |= MOVEFLAG2_FULLSPEED_TURNING;
	if( vehicle_info->flags & VEHICLE_FLAG_ALLOW_PITCHING )
		flags2 |= MOVEFLAG2_ALLOW_PITCHING;
	if( vehicle_info->flags & VEHICLE_FLAG_FULLSPEEDPITCHING )
		flags2 |= MOVEFLAG2_FULLSPEED_PITCHING;

	return flags2;
}


void Vehicle::InstallAccessories(){
	if( !installed_accessories.empty() )
		return;

	std::vector< VehicleAccessoryEntry* >* v = objmgr.GetVehicleAccessories( creature_entry );
	if( v == NULL )
		return;
	
	for( std::vector< VehicleAccessoryEntry* >::iterator itr = v->begin(); itr != v->end(); ++itr ){
		VehicleAccessoryEntry *accessory = *itr;

		if( seats[ accessory->seat ] == NULL )
			continue;

		if( seats[ accessory->seat ]->HasPassenger() )
			EjectPassengerFromSeat( accessory->seat );

		CreatureInfo  *ci = CreatureNameStorage.LookupEntry( accessory->accessory_entry );
		if( ci == NULL )
			continue;
		
		CreatureProto *cp = CreatureProtoStorage.LookupEntry( accessory->accessory_entry );
		if( cp == NULL )
			continue;
		
		Creature *c = owner->GetMapMgr()->CreateCreature( accessory->accessory_entry );
		c->Load( cp, owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), owner->GetOrientation() );
		c->transporter_info.guid = owner->GetGUID();
		c->transporter_info.seat = accessory->seat;
		c->Phase( PHASE_SET, owner->GetPhase() );
		c->SetFaction( owner->GetFaction() );
		c->PushToWorld( owner->GetMapMgr() );
		
		AddPassengerToSeat( c, accessory->seat );
		installed_accessories.push_back( c->GetGUID() );
	}
}

void Vehicle::RemoveAccessories(){
	for( std::vector< uint64 >::iterator itr = installed_accessories.begin(); itr != installed_accessories.end(); ++itr ){
		Unit *u = owner->GetMapMgr()->GetUnit( *itr );
		if( u == NULL )
			continue;

		if( u->GetVehicleComponent() != NULL )
			u->GetVehicleComponent()->EjectAllPassengers();

		EjectPassenger( u );
		u->Delete();
	}
	
	installed_accessories.clear();
}

bool Vehicle::HasAccessoryWithGUID( uint64 guid ){
	std::vector< uint64 >::iterator itr =
		std::find( installed_accessories.begin(), installed_accessories.end(), guid );

	if( itr == installed_accessories.end() )
		return false;
	else
		return true;
}

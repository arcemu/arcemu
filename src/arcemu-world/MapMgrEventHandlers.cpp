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

#include "StdAfx.h"

static MapMgrEventHandler mapmgreventhandlers[ MAX_MAPMGR_EVENTS ];

void REGISTER_MAPMGR_EVENT_HANDLER( uint32 type, MapMgrEventHandler handler ){
	if( type < MAX_MAPMGR_EVENTS )
		mapmgreventhandlers[ type ] = handler;
}

void CALL_MAPMGR_EVENT_HANDLER( MapMgr *mgr, uint32 type, uint32 data1, uint32 data2, uint32 data3 ){
	if( type >= MAX_MAPMGR_EVENTS )
		return;

	mapmgreventhandlers[ type ]( mgr, data1, data2, data3 );
}


void HandleNULLMapMgrEvent( MapMgr *mgr, uint32 data1, uint32 data2, uint32 data3 ){}


void HandleWorldStateUpdate( MapMgr *mgr, uint32 data1, uint32 data2, uint32 data3 ){
	uint32 zone = data1;
	uint32 field = data2;
	uint32 value = data3;
	
	WorldPacket data( SMSG_UPDATE_WORLD_STATE, 8 );
	data << uint32( field );
	data << uint32( value );
	
	mgr->SendPacketToPlayersInZone( zone, &data );
}


void REGISTER_MAPMGR_EVENT_HANDLERS(){
	std::fill( mapmgreventhandlers, mapmgreventhandlers + MAX_MAPMGR_EVENTS, &HandleNULLMapMgrEvent );

	REGISTER_MAPMGR_EVENT_HANDLER( 0, HandleNULLMapMgrEvent );
	REGISTER_MAPMGR_EVENT_HANDLER( 1, HandleWorldStateUpdate );
}
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

#include "MiscPacketHandlers.h"

enum FarSightCommands
{
	FARSIGHT_CMD_CLEAR = 0,
	FARSIGHT_CMD_SET = 1
};

DEFINE_PACKET_HANDLER_METHOD( FarSightHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	uint8 cmd;
	recv_data >> cmd;

	LOG_DETAIL( "CMSG_FAR_SIGHT cmd: %u", cmd );

	switch( cmd )
	{
	case FARSIGHT_CMD_CLEAR:
		_player->bindSight( NULL );
		break;

	case FARSIGHT_CMD_SET:
		{
			uint64 targetGuid = _player->GetFarsightTarget();
			if( targetGuid == 0 )
				return;
		
			MapMgr *mapMgr = _player->GetMapMgr();
			Object *target = mapMgr->GetObject( targetGuid );
			if( target == NULL )
				return;
			if( target->getFarsightViewer() != NULL )
				return;
			_player->bindSight( target );		
			break;
		}

	default:
		LOG_DETAIL( "Unimplemented farsight command: ", cmd );
		break;
	}

}

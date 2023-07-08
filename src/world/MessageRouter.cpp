/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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
#include "MessageRouter.h"
#include "PlayerMessenger.h"


MessageRouter::MessageRouter()
{
}

MessageRouter::MessageRouter( Object *object )
{
	this->object = object;
}

void MessageRouter::sendMessageToPlayersInRange( WorldPacket* packet, bool sendToObject, bool sendToSameTeamOnly  )
{
	if( ! object->IsInWorld() )
		return;

	if( ! object->IsPlayer() )
	{
		for(std::set< Object* >::iterator itr = object->GetInRangePlayerSetBegin(); itr != object->GetInRangePlayerSetEnd(); ++itr)
		{
			Player* p = TO< Player* >( *itr );

			if((p->GetPhase() & object->GetPhase()) == 0)
				continue;

			if( ( p->GetFarsightTarget() != 0 ) && ( object->GetGUID() != p->GetFarsightTarget() ) )
				continue;

			PlayerMessenger::sendMessage( p, *packet );
		}
	}
	else
	{
		Player *player = TO< Player* >( object );

		if( sendToObject )
		{
			PlayerMessenger::sendMessage( player, *packet );
		}

		for( std::set< Object* >::iterator itr = player->GetInRangePlayerSetBegin(); itr != player->GetInRangePlayerSetEnd(); ++itr )
		{
			Player* p = TO< Player* >( *itr );
			WorldSession *session = p->GetSession();

			if( session == NULL )
				continue;

			if( sendToSameTeamOnly && ( p->GetTeam() != player->GetTeam() ) )
				continue;

			if( ( p->GetPhase() & player->GetPhase() ) == 0 )
				continue;

			if( packet->GetOpcode() == SMSG_MESSAGECHAT )
			{
				if( p->Social_IsIgnoring( player->GetLowGUID() ) )
					continue;
			}
			else
			{
				if( player->m_isGmInvisible && ( session->GetPermissionCount() <= 0 ) )
					continue;

				if( ! p->IsVisible( player->GetGUID() ) )
					continue;
			}

			if( ( p->GetFarsightTarget() != 0 ) && ( object->GetGUID() != p->GetFarsightTarget() ) )
				continue;

			PlayerMessenger::sendMessage( p, *packet );
		}
	}

	MapMgr *mgr = object->GetMapMgr();

	std::set< Object* > &objects = object->getFarsightBoundInRange();
	for( std::set< Object* >::iterator itr = objects.begin(); itr != objects.end(); ++itr )
	{
		Object *o = (*itr);
		Player *p = TO_PLAYER( o->getFarsightViewer() );
		PlayerMessenger::sendMessage( p, *packet );
	}
}

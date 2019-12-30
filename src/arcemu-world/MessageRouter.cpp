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


MessageRouter::MessageRouter()
{
	sendToObject = true;
	sendToSameTeamOnly = false;
}

MessageRouter::MessageRouter( Object *object, bool sendToObject, bool sendToSameTeamOnly )
{
	this->object = object;
	this->sendToObject = sendToObject;
	this->sendToSameTeamOnly = sendToSameTeamOnly;
}

void MessageRouter::sendMessageToPlayersAround( WorldPacket* packet )
{
	if( ! object->IsInWorld() )
		return;

	if( ! object->IsPlayer() )
	{

		uint32 myphase = object->GetPhase();
		for(std::set< Object* >::iterator itr = object->GetInRangePlayerSetBegin(); itr != object->GetInRangePlayerSetEnd(); ++itr)
		{
			Object* o = *itr;
			if((o->GetPhase() & myphase) != 0)
				o->SendPacket(packet);
		}

		return;
	}
	else
	{
		Player *player = TO< Player* >( object );
		bool gminvis = false;

		if( sendToObject )
		{
			player->SendPacket(packet);
		}

		gminvis = player->m_isGmInvisible;
		uint32 myphase = player->GetPhase();

		if(sendToSameTeamOnly)
		{
			uint32 myteam = player->GetTeam();

			if( packet->GetOpcode() != SMSG_MESSAGECHAT)
			{
				for(std::set< Object* >::iterator itr = player->GetInRangePlayerSetBegin(); itr != player->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* p = TO< Player* >(*itr);

					if( gminvis && ( ( p->GetSession() == NULL ) || ( p->GetSession()->GetPermissionCount() <= 0 ) ) )
						continue;

					if( p->GetTeam() == myteam
						&& (p->GetPhase() & myphase) != 0
						&& p->IsVisible( player->GetGUID() ) )
						p->SendPacket(packet);
				}
			}
			else
			{
				for(std::set< Object* >::iterator itr = player->GetInRangePlayerSetBegin(); itr != player->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* p = TO< Player* >(*itr);

					if(p->GetSession()
						&& p->GetTeam() == myteam
						&& !p->Social_IsIgnoring(player->GetLowGUID())
						&& (p->GetPhase() & myphase) != 0 )
						p->SendPacket(packet);
				}
			}
		}
		else
		{
			if( packet->GetOpcode() != SMSG_MESSAGECHAT)
			{
				for(std::set< Object* >::iterator itr = player->GetInRangePlayerSetBegin(); itr != player->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* p = TO< Player* >(*itr);

					if( gminvis && ( ( p->GetSession() == NULL ) || ( p->GetSession()->GetPermissionCount() <= 0 ) ) )
						continue;

					if( (p->GetPhase() & myphase) != 0
						&& p->IsVisible( player->GetGUID() ) )
						p->SendPacket(packet);
				}
			}
			else
			{
				for(std::set< Object* >::iterator itr = player->GetInRangePlayerSetBegin(); itr != player->GetInRangePlayerSetEnd(); ++itr)
				{
					Player* p = TO< Player* >(*itr);

					if(p->GetSession()
						&& !p->Social_IsIgnoring(player->GetLowGUID())
						&& (p->GetPhase() & myphase) != 0 )
						p->SendPacket(packet);
				}
			}
		}
	}
}

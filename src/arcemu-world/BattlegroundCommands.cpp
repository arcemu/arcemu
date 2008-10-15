/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

bool ChatHandler::HandleInitializeAllQueuedBattlegroundsCommand(const char *args, WorldSession *m_session)
{
	SystemMessage(m_session, "Forcing initialitation of all battlegrounds...");
	BattlegroundManager.EventQueueUpdate(true);
	SystemMessage(m_session, "...done");
	return true;
}

bool ChatHandler::HandleGetBattlegroundQueueCommand(const char *args, WorldSession *m_session)
{
	SystemMessage(m_session, "Getting battleground queues...");
	BattlegroundManager.HandleGetBattlegroundQueueCommand(m_session);
	SystemMessage(m_session, "...done");
	return true;
}

bool ChatHandler::HandleSetBGScoreCommand(const char* args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleStartBGCommand(const char *args, WorldSession *m_session)
{
	if(!m_session->GetPlayer()->m_bg)
	{
		SystemMessage(m_session,"You're not in a battleground!");
		return true;
	}
	m_session->GetPlayer()->m_bg->SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, m_session->LocalizedWorldSrv(49), m_session->LocalizedWorldSrv(m_session->GetPlayer()->m_bg->GetNameID()) );
	sEventMgr.RemoveEvents(m_session->GetPlayer()->m_bg, EVENT_BATTLEGROUND_COUNTDOWN);
	m_session->GetPlayer()->m_bg->Start();
	return true;
}

bool ChatHandler::HandlePauseBGCommand(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleBGInfoCommnad(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleBattlegroundCommand(const char* args, WorldSession *m_session)
{
	uint32 type = atoi(args);
	if(type != 2 && type != 3 && type != 4 && type != 5 && type != 7)
		return false;

	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;
	BattlegroundManager.HandleBattlegroundListPacket(plr->GetSession(), atoi(args));
	return true;
}

bool ChatHandler::HandleSetWorldStateCommand(const char* args, WorldSession *m_session)
{
	uint32 id,val;
	if(sscanf(args,"%u %u", &id,&val)!=2)
		return false;

	if(m_session->GetPlayer()->m_bg)
		m_session->GetPlayer()->m_bg->SetWorldState(id,val);

	return true;
}

bool ChatHandler::HandleSetWorldStatesCommand(const char* args, WorldSession *m_session)
{
	uint32 first,last,val;
	if(sscanf(args,"%u %u %u", &first, &last, &val)!=3)
		return false;

	if(m_session->GetPlayer()->m_bg)
	{
		for (uint32 i=first; i<last; i++)
		{
			m_session->GetPlayer()->m_bg->SetWorldState(i,val);
		}
	}

	return true;
}

bool ChatHandler::HandlePlaySoundCommand(const char* args, WorldSession *m_session)
{
	if(!m_session->GetPlayer()->m_bg)
	{
		SystemMessage(m_session,"You're not in a battleground!");
		return true;
	}
	m_session->GetPlayer()->m_bg->PlaySoundToAll(atoi(args));
	return true;
}

bool ChatHandler::HandleSetBattlefieldStatusCommand(const char* args, WorldSession *m_session)
{
	uint32 type = atoi(args);
	BattlegroundManager.SendBattlefieldStatus(m_session->GetPlayer(), 1, type, 0 , 0, m_session->GetPlayer()->GetMapId(),0);
	return true;
}

bool ChatHandler::HandleBattlegroundExitCommand(const char* args, WorldSession* m_session)
{
	if(!m_session->GetPlayer()->m_bg)
	{
		SystemMessage(m_session,"You're not in a battleground!");
		return true;
	}
	m_session->GetPlayer()->m_bg->Close();
	return true;
}

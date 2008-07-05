/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#include "StdAfx.h"

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
	m_session->GetPlayer()->m_bg->SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The battle for %s has begun!", m_session->GetPlayer()->m_bg->GetName() );
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

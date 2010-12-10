/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

extern std::string LogFileName;
extern bool bLogChat;

static const uint32 LanguageSkills[NUM_LANGUAGES] = {
	0,				// UNIVERSAL		0x00
	109,			// ORCISH			0x01
	113,			// DARNASSIAN		0x02
	115,			// TAURAHE			0x03
	0,				// -				0x04
	0,				// -				0x05
	111,			// DWARVISH			0x06
	98,				// COMMON			0x07
	139,			// DEMON TONGUE		0x08
	140,			// TITAN			0x09
	137,			// THALSSIAN		0x0A
	138,			// DRACONIC			0x0B
	0,				// KALIMAG			0x0C
	313,			// GNOMISH			0x0D
	315,			// TROLL			0x0E
	0,				// -				0x0F
	0,				// -				0x10
	0,				// -				0x11
	0,				// -				0x12
	0,				// -				0x13
	0,				// -				0x14
	0,				// -				0x15
	0,				// -				0x16
	0,				// -				0x17
	0,				// -				0x18
	0,				// -				0x19
	0,				// -				0x1A
	0,				// -				0x1B
	0,				// -				0x1C
	0,				// -				0x1D
	0,				// -				0x1E
	0,				// -				0x1F
	0,				// -				0x20
	673,			// -				0x21
	0,				// -				0x22
	759,			// -				0x23
};

void WorldSession::HandleMessagechatOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 9);
	WorldPacket *data;

	uint32 type;
	int32 lang;

	const char * pMisc = 0;
	const char * pMsg = 0;
	recv_data >> type;
	recv_data >> lang;

	if( lang >= NUM_LANGUAGES )
		return;

	if(GetPlayer()->IsBanned())
	{
		GetPlayer()->BroadcastMessage("You cannot do that when banned.");
		return;
	}

	// Flood protection
	if(lang != -1 && !GetPermissionCount() && sWorld.flood_lines != 0)
	{
		/* flood detection, wheeee! */
		if(UNIXTIME >= floodTime)
		{
			floodLines = 0;
			floodTime = UNIXTIME + sWorld.flood_seconds;
		}

		if((++floodLines) > sWorld.flood_lines)
		{
			if(sWorld.flood_message)
				_player->BroadcastMessage("Your message has triggered serverside flood protection. You can speak again in %u seconds.", floodTime - UNIXTIME);

			return;
		}
	}

	switch(type)
	{
	case CHAT_MSG_EMOTE:
	case CHAT_MSG_SAY:
	case CHAT_MSG_YELL:
	case CHAT_MSG_WHISPER:
	case CHAT_MSG_CHANNEL:
		{
			if( m_muted && m_muted >= (uint32)UNIXTIME )
			{
				SystemMessage("Your voice is currently muted by a moderator.");
				return;
			}
		}break;
	}

	std::string msg, to = "", channel = "", tmp;
	msg.reserve(256);

	// Process packet
	switch(type)
	{
		case CHAT_MSG_SAY:
		case CHAT_MSG_EMOTE:
		case CHAT_MSG_PARTY:
		case CHAT_MSG_PARTY_LEADER:
		case CHAT_MSG_RAID:
		case CHAT_MSG_RAID_LEADER:
		case CHAT_MSG_RAID_WARNING:
		case CHAT_MSG_GUILD:
		case CHAT_MSG_OFFICER:
		case CHAT_MSG_YELL:
			recv_data >> msg;
			pMsg=msg.c_str();
			//g_chatFilter->ParseEscapeCodes((char*)pMsg,true);
			pMisc= 0;
			break;
		case CHAT_MSG_WHISPER:
			recv_data >> to >> msg;
			pMsg=msg.c_str();
			pMisc=to.c_str();
			break;
		case CHAT_MSG_CHANNEL:
			recv_data >> channel;
			recv_data >> msg;
			pMsg=msg.c_str();
			pMisc=channel.c_str();
			break;
		case CHAT_MSG_AFK:
		case CHAT_MSG_DND:
			break;
		case CHAT_MSG_BATTLEGROUND:
		case CHAT_MSG_BATTLEGROUND_LEADER:
			recv_data >> msg;
			pMsg = msg.c_str();
			break;
		default:
			sLog.outError("CHAT: unknown msg type %u, lang: %u", type, lang);
	}
	

	if (int(msg.find("|T")) > -1 )
	{
		GetPlayer()->BroadcastMessage("Don't even THINK about doing that again");
		return;
	}
	
	// HookInterface OnChat event
	if (pMsg && !sHookInterface.OnChat(_player, type, lang, pMsg, pMisc))
		return;

	Channel* chn = NULL;
	// Main chat message processing
	switch(type)
	{
	case CHAT_MSG_EMOTE:
		{
			if(sWorld.interfaction_chat && lang > 0)
				lang= 0;

			if(GetPlayer()->m_modlanguage >= 0)
				data = sChatHandler.FillMessageData( CHAT_MSG_EMOTE, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
			else if (lang== 0 && sWorld.interfaction_chat)
				data = sChatHandler.FillMessageData( CHAT_MSG_EMOTE, CanUseCommand('0') ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
			else	
				data = sChatHandler.FillMessageData( CHAT_MSG_EMOTE, CanUseCommand('c') ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
			GetPlayer()->SendMessageToSet( data, true ,true );

			//sLog.outString("[emote] %s: %s", _player->GetName(), msg.c_str());
			delete data;
			
		}break;
	case CHAT_MSG_SAY:
		{
			if(sWorld.interfaction_chat && lang > 0)
				lang= 0;

			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			if(GetPlayer()->m_modlanguage >= 0)
			{
				data = sChatHandler.FillMessageData( CHAT_MSG_SAY, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
				GetPlayer()->SendMessageToSet( data, true );
			}
			else
			{
				if(lang > 0 && LanguageSkills[lang] && _player->_HasSkillLine(LanguageSkills[lang]) == false)
					return;

				if(lang== 0 && !CanUseCommand('c') && !sWorld.interfaction_chat)
					return;

				data = sChatHandler.FillMessageData( CHAT_MSG_SAY, lang, msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
				
                SendChatPacket(data, 1, lang, this);

				for(set<Object*>::iterator itr = _player->m_inRangePlayers.begin(); itr != _player->m_inRangePlayers.end(); ++itr)
				{

                    Player *p = TO< Player* >( (*itr) );

					if ( _player->GetPhase() & (*itr)->GetPhase() )
						p->GetSession()->SendChatPacket(data, 1, lang, this);
				}
			}
			delete data;

		} break;
	case CHAT_MSG_PARTY:
	case CHAT_MSG_PARTY_LEADER:
	case CHAT_MSG_RAID:
	case CHAT_MSG_RAID_LEADER:
	case CHAT_MSG_RAID_WARNING:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(sWorld.interfaction_chat && lang > 0)
				lang= 0;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			Group *pGroup = _player->GetGroup();
			if(pGroup == NULL) break;
			
			if(GetPlayer()->m_modlanguage >= 0)
				data=sChatHandler.FillMessageData( type, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
			else if(lang== 0 && sWorld.interfaction_chat)
				data=sChatHandler.FillMessageData( type, (CanUseCommand('0') && lang != -1) ? LANG_UNIVERSAL : lang, msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0);
			else
				data=sChatHandler.FillMessageData( type, (CanUseCommand('c') && lang != -1) ? LANG_UNIVERSAL : lang, msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0);
			if(type == CHAT_MSG_PARTY && pGroup->GetGroupType() == GROUP_TYPE_RAID)
			{
				// only send to that subgroup
				SubGroup * sgr = _player->GetGroup() ?
					_player->GetGroup()->GetSubGroup(_player->GetSubGroup()) : 0;

				if(sgr)
				{
					_player->GetGroup()->Lock();
					for(GroupMembersSet::iterator itr = sgr->GetGroupMembersBegin(); itr != sgr->GetGroupMembersEnd(); ++itr)
					{
						if((*itr)->m_loggedInPlayer)
							(*itr)->m_loggedInPlayer->GetSession()->SendChatPacket(data, 1, lang, this);
					}
					_player->GetGroup()->Unlock();
				}
			}
			else
			{
				SubGroup * sgr;
				for(uint32 i = 0; i < _player->GetGroup()->GetSubGroupCount(); ++i)
				{
					sgr = _player->GetGroup()->GetSubGroup(i);
					_player->GetGroup()->Lock();
					for(GroupMembersSet::iterator itr = sgr->GetGroupMembersBegin(); itr != sgr->GetGroupMembersEnd(); ++itr)
					{
						if((*itr)->m_loggedInPlayer)
							(*itr)->m_loggedInPlayer->GetSession()->SendChatPacket(data, 1, lang, this);
					}
					_player->GetGroup()->Unlock();

				}
			}
			//sLog.outString("[party] %s: %s", _player->GetName(), msg.c_str());
			delete data;
		} break;
	case CHAT_MSG_GUILD:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
			{
				break;
			}

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			if(_player->m_playerInfo->guild)
				_player->m_playerInfo->guild->GuildChat(msg.c_str(), this, lang);

		} break;
	case CHAT_MSG_OFFICER:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			if(_player->m_playerInfo->guild)
				_player->m_playerInfo->guild->OfficerChat(msg.c_str(), this, lang);

		} break;
	case CHAT_MSG_YELL:
		{
			if(sWorld.interfaction_chat && lang > 0)
				lang= 0;

			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}
			if(lang > 0 && LanguageSkills[lang] && _player->_HasSkillLine(LanguageSkills[lang]) == false)
				return;

			if(lang== 0 && sWorld.interfaction_chat)
				data = sChatHandler.FillMessageData( CHAT_MSG_YELL, (CanUseCommand('0') && lang != -1) ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );

			else if(lang== 0 && !CanUseCommand('c'))
				return;

			else if(GetPlayer()->m_modlanguage >= 0)
				data = sChatHandler.FillMessageData( CHAT_MSG_YELL, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
			else
				data = sChatHandler.FillMessageData( CHAT_MSG_YELL, (CanUseCommand('c') && lang != -1) ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );

			_player->GetMapMgr()->SendChatMessageToCellPlayers(_player, data, 2, 1, lang, this);
			delete data;
		} break;
	case CHAT_MSG_WHISPER:
		{
			if(lang != -1)
				lang = LANG_UNIVERSAL; //All whispers are universal
			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			PlayerCache* playercache = objmgr.GetPlayerCache(to.c_str(), false);
			if(playercache == NULL)
			{
				data = new WorldPacket(SMSG_CHAT_PLAYER_NOT_FOUND, to.length() + 1);
				*data << to;
				SendPacket(data);
				delete data;
				break;
			}

			if (_player->GetTeamInitial() != playercache->GetUInt32Value(CACHE_PLAYER_INITIALTEAM) && !sWorld.interfaction_chat && !playercache->HasFlag(CACHE_PLAYER_FLAGS, PLAYER_FLAG_GM) && !_player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
			{
				WorldPacket response(SMSG_CHAT_PLAYER_NOT_FOUND, to.length() + 1);
				response << to;
				SendPacket(&response);
				playercache->DecRef();
				break;
			}

			// Check that the player isn't a gm with his status on
			// TODO: Game Master's on retail are able to have block whispers after they close the ticket with the current packet.
			// When a Game Master is visible to your player it says "This player is unavailable for whisper" I need to figure out how this done.
			if( !HasPermissions() && playercache->HasFlag(CACHE_PLAYER_FLAGS, PLAYER_FLAG_GM) && playercache->CountValue64(CACHE_GM_TARGETS, _player->GetGUID()) == 0)
			{
				// Build automated reply
				string Reply = "SYSTEM: This Game Master does not currently have an open ticket from you and did not receive your whisper. Please submit a new GM Ticket request if you need to speak to a GM. This is an automatic message.";
				data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER_INFORM, LANG_UNIVERSAL, Reply.c_str(), playercache->GetGUID(), 4);
				SendPacket(data);
				delete data;
				playercache->DecRef();
				break;
			}

			if (playercache->CountValue64(CACHE_SOCIAL_IGNORELIST, _player->GetLowGUID()) > 0)
			{
				data = sChatHandler.FillMessageData( CHAT_MSG_IGNORED, LANG_UNIVERSAL,  msg.c_str(), playercache->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
				SendPacket(data);
				delete data;
				playercache->DecRef();
				break;
			}
			else
			{
				data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, lang,  msg.c_str(), _player->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0 );
				playercache->SendPacket(data);
			}


			//Sent the to Users id as the channel, this should be fine as it's not used for whisper
			if(lang != -1) //DO NOT SEND if its an addon message!
			{
				data = sChatHandler.FillMessageData(CHAT_MSG_WHISPER_INFORM, LANG_UNIVERSAL,msg.c_str(), playercache->GetGUID(), playercache->HasFlag(CACHE_PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0  );
				SendPacket(data);
				delete data;
			}

			if(playercache->HasFlag(CACHE_PLAYER_FLAGS, PLAYER_FLAG_AFK))
			{
				// Has AFK flag, autorespond.
				std::string reason;
				playercache->GetStringValue(CACHE_AFK_DND_REASON, reason);

				data = sChatHandler.FillMessageData(CHAT_MSG_AFK, LANG_UNIVERSAL, reason.c_str(), playercache->GetGUID(), _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0);
				SendPacket(data);
				delete data;
			}
			else if(playercache->HasFlag(CACHE_PLAYER_FLAGS, PLAYER_FLAG_DND))
			{
				// Has DND flag, autorespond.
				std::string reason;
				playercache->GetStringValue(CACHE_AFK_DND_REASON, reason);
				data = sChatHandler.FillMessageData(CHAT_MSG_DND, LANG_UNIVERSAL, reason.c_str(), playercache->GetGUID(), playercache->HasFlag(CACHE_PLAYER_FLAGS, PLAYER_FLAG_GM) ? 4 : 0);
				SendPacket(data);
				delete data;
			}

			playercache->DecRef();

		} break;
	case CHAT_MSG_CHANNEL:
		{
			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			chn = channelmgr.GetChannel(channel.c_str(),GetPlayer());
			if(chn)
			{
				//g_chatFilter->ParseEscapeCodes((char*)pMsg, (chn->m_flags & CHANNEL_PACKET_ALLOWLINKS)>0 );
				chn->Say(GetPlayer(),msg.c_str(), NULL, false);
			}
		} break;
	case CHAT_MSG_AFK:
		{
			std::string reason = "";
			recv_data >> reason;

			GetPlayer()->SetAFKReason(reason);

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			/* WorldPacket *data, WorldSession* session, uint32 type, uint32 language, const char *channelName, const char *message*/
			if( GetPlayer()->HasFlag( PLAYER_FLAGS, PLAYER_FLAG_AFK ) )
			{
				GetPlayer()->RemoveFlag( PLAYER_FLAGS, PLAYER_FLAG_AFK );
				if( sWorld.GetKickAFKPlayerTime() )
					sEventMgr.RemoveEvents( GetPlayer(),EVENT_PLAYER_SOFT_DISCONNECT );
			}
			else
			{
				GetPlayer()->SetFlag( PLAYER_FLAGS, PLAYER_FLAG_AFK );

                if( GetPlayer()->m_bg )
                    GetPlayer()->m_bg->RemovePlayer( GetPlayer(), false );

				if(sWorld.GetKickAFKPlayerTime())
					sEventMgr.AddEvent(GetPlayer(),&Player::SoftDisconnect,EVENT_PLAYER_SOFT_DISCONNECT,sWorld.GetKickAFKPlayerTime(),1,0);
			}
		} break;
	case CHAT_MSG_DND:
		{
			std::string reason;
			recv_data >> reason;
			GetPlayer()->SetAFKReason(reason);

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			if( GetPlayer()->HasFlag( PLAYER_FLAGS, PLAYER_FLAG_DND ) )
				GetPlayer()->RemoveFlag( PLAYER_FLAGS, PLAYER_FLAG_DND );
			else
			{
				GetPlayer()->SetFlag( PLAYER_FLAGS, PLAYER_FLAG_DND );
			}
		} break;

	case CHAT_MSG_BATTLEGROUND:
	case CHAT_MSG_BATTLEGROUND_LEADER:
		{
			if( sChatHandler.ParseCommands( msg.c_str(), this ) > 0 )
				break;

			if( g_chatFilter->Parse( msg ) == true )
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}
			if( _player->m_bg != NULL )
			{
				data = sChatHandler.FillMessageData( type, lang, msg.c_str(), _player->GetGUID() );
				_player->m_bg->DistributePacketToTeam( data, _player->GetTeam() );
				delete data;
			}
		}break;
	}
}

void WorldSession::HandleEmoteOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data,4);

	if(!_player->isAlive())
		return;

	uint32 emote;
	recv_data >> emote;
	_player->Emote((EmoteType)emote);
#ifdef ENABLE_ACHIEVEMENTS
	_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE, emote, 0, 0);
#endif
	uint64 guid = _player->GetGUID();
	sQuestMgr.OnPlayerEmote(_player, emote, guid);
}

void WorldSession::HandleTextEmoteOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 16);
	if(!_player->isAlive())
		return;

	uint64 guid;
	uint32
		text_emote,
		unk,
		namelen =1;
	const char* name =" ";

	recv_data >> text_emote;
	recv_data >> unk;
	recv_data >> guid;
	if( m_muted && m_muted >= (uint32)UNIXTIME )
	{
		SystemMessage("Your voice is currently muted by a moderator.");
		return;
	}

	if(!GetPermissionCount() && sWorld.flood_lines)
	{
		/* flood detection, wheeee! */
		if(UNIXTIME >= floodTime)
		{
			floodLines = 0;
			floodTime = UNIXTIME + sWorld.flood_seconds;
		}

		if((++floodLines) > sWorld.flood_lines)
		{
			return;
		}
	}
	Unit * pUnit = _player->GetMapMgr()->GetUnit(guid);
	if(pUnit)
	{
		if( pUnit->IsPlayer() )
		{
			name = TO< Player* >( pUnit )->GetName();
			namelen = (uint32)strlen(name) + 1;
		}
		else if ( pUnit->IsPet() )
		{
			name = TO< Pet* >( pUnit )->GetName().c_str();
			namelen = (uint32)strlen(name) + 1;
		}
		else
		{
			Creature * p = TO< Creature* >(pUnit);
			name = p->GetCreatureInfo()->Name;
			namelen = (uint32)strlen(name) + 1;
		}
	}

	emoteentry *em = dbcEmoteEntry.LookupEntryForced(text_emote);
	if(em)
	{
		WorldPacket data(SMSG_EMOTE, 28 + namelen);

		sHookInterface.OnEmote(_player, (EmoteType)em->textid, pUnit);
		if(pUnit)
			CALL_SCRIPT_EVENT(pUnit,OnEmote)(_player,(EmoteType)em->textid);

        switch(em->textid)
        {
            case EMOTE_STATE_SLEEP:
            case EMOTE_STATE_SIT:
            case EMOTE_STATE_KNEEL:
			case EMOTE_STATE_DANCE:
				{
					_player->SetEmoteState(em->textid);
				}break;
		}

		data << (uint32)em->textid;
		data << (uint64)GetPlayer()->GetGUID();
		GetPlayer()->SendMessageToSet(&data, true); //If player receives his own emote, his animation stops.

		data.Initialize(SMSG_TEXT_EMOTE);
		data << (uint64)GetPlayer()->GetGUID();
		data << (uint32)text_emote;
		data << unk;
		data << (uint32)namelen;
		if( namelen > 1 )   data.append(name, namelen);
		else				data << (uint8)0x00;

		GetPlayer()->SendMessageToSet(&data, true);
#ifdef ENABLE_ACHIEVEMENTS
		_player->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE, text_emote, 0, 0);
#endif		
		sQuestMgr.OnPlayerEmote(_player, text_emote, guid);
	}
}

void WorldSession::HandleReportSpamOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1+8);
	sLog.outDebug("WORLD: CMSG_REPORT_SPAM");

	uint8 spam_type;                                        // 0 - mail, 1 - chat
	uint64 spammer_guid;
	uint32 unk1 = 0, unk2 = 0, unk3 = 0, unk4 = 0;
	std::string description = "";
	recv_data >> spam_type;                                 // unk 0x01 const, may be spam type (mail/chat)
	recv_data >> spammer_guid;                              // player guid
	switch(spam_type)
	{
		case 0:
			CHECK_PACKET_SIZE(recv_data, recv_data.rpos()+4+4+4);
			recv_data >> unk1;                              // const 0
			recv_data >> unk2;                              // probably mail id
			recv_data >> unk3;                              // const 0
			break;
		case 1:
			CHECK_PACKET_SIZE(recv_data, recv_data.rpos()+4+4+4+4+1);
			recv_data >> unk1;                              // probably language
			recv_data >> unk2;                              // message type?
			recv_data >> unk3;                              // probably channel id
			recv_data >> unk4;                              // unk random value
			recv_data >> description;                       // spam description string (messagetype, channel name, player name, message)
			break;
	}
	// NOTE: all chat messages from this spammer automatically ignored by spam reporter until logout in case chat spam.
	// if it's mail spam - ALL mails from this spammer automatically removed by client

	// Complaint Received message
	WorldPacket data(SMSG_COMPLAIN_RESULT, 1);
	data << uint8(0);
	SendPacket(&data);

	sLog.outDebug("REPORT SPAM: type %u, guid %u, unk1 %u, unk2 %u, unk3 %u, unk4 %u, message %s", spam_type, Arcemu::Util::GUID_LOPART(spammer_guid), unk1, unk2, unk3, unk4, description.c_str());
}

void WorldSession::HandleChatIgnoredOpcode(WorldPacket & recvPacket )
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recvPacket, 8+1);

	uint64 iguid;
	uint8 unk;

	recvPacket >> iguid;
	recvPacket >> unk; // probably related to spam reporting

	Player *player = objmgr.GetPlayer(uint32(iguid));
	if(!player || !player->GetSession())
		return;

	WorldPacket * data = sChatHandler.FillMessageData(CHAT_MSG_IGNORED, LANG_UNIVERSAL, _player->GetName(), _player->GetGUID());
	player->GetSession()->SendPacket(data);
}

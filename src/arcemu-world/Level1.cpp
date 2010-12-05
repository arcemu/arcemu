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

/////////////////////////////////////////////////
//  GM Chat Commands
//

#include "StdAfx.h"

uint16 GetItemIDFromLink(const char* itemlink, uint32* itemid)
{
	if(itemlink== NULL)
	{
		*itemid = 0;
		return 0;
	}
	uint16 slen = (uint16)strlen(itemlink);

	const char* ptr = strstr(itemlink, "|Hitem:");
	if(ptr == NULL)
	{
		*itemid = 0;
		return slen;
	}

	ptr += 7; // item id is just past "|Hitem:" (7 bytes)
	*itemid = atoi(ptr);
	
	ptr = strstr(itemlink, "|r"); // the end of the item link
	if(ptr == NULL) // item link was invalid
	{
		*itemid = 0;
		return slen;
	}

	ptr += 2;
	return (ptr-itemlink) & 0x0000ffff;
}

bool ChatHandler::HandleAnnounceCommand(const char* args, WorldSession *m_session)
{
	if( !*args || strlen(args) < 4 || strchr(args, '%'))
	{
		m_session->SystemMessage("Announces cannot contain the %% character and must be at least 4 characters.");
		return true;
	}

	char msg[1024];
	string input2;
	input2 = sWorld.ann_tagcolor;
	input2 += "[";
	input2 += sWorld.announce_tag;
	input2 += "]";
	input2 += sWorld.ann_gmtagcolor;
	if(sWorld.GMAdminTag)
	{
		if(m_session->CanUseCommand('z')) input2+="<Admin>";
		else if(m_session->GetPermissionCount()) input2+="<GM>";
	}
	if(sWorld.NameinAnnounce)
	{
		input2+="|r"+sWorld.ann_namecolor+"|Hplayer:";
	input2+=m_session->GetPlayer()->GetName();
	input2+="|h[";
	input2+=m_session->GetPlayer()->GetName();
	input2+="]|h:|r "+sWorld.ann_msgcolor;
	}
	else if(!sWorld.NameinAnnounce) {input2+= ": "; input2+= sWorld.ann_msgcolor;}
	snprintf((char*)msg, 1024, "%s%s", input2.c_str(), args);
	sWorld.SendWorldText(msg); // send message
	sGMLog.writefromsession(m_session, "used announce command, [%s]", args);
	return true;
}

bool ChatHandler::HandleGMAnnounceCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
	{
		sLog.outError("HandleGMAnnounceCommand !args = failed");
		return false;
	}

	char GMAnnounce[1024];
	snprintf(GMAnnounce, 1024, MSG_COLOR_RED"[Team]"MSG_COLOR_GREEN" |Hplayer:%s|h[%s]|h:"MSG_COLOR_YELLOW" %s", m_session->GetPlayer()->GetName(), m_session->GetPlayer()->GetName(), args);
	sWorld.SendGMWorldText(GMAnnounce);
	sGMLog.writefromsession(m_session, "used team announce command, [%s]", args);
	return true;
}

bool ChatHandler::HandleWAnnounceCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char pAnnounce[1024];
	string input3;
	input3 = sWorld.ann_tagcolor;
	input3 += "[";
	input3 += sWorld.announce_tag;
	input3 += "]";
	input3 += sWorld.ann_gmtagcolor;
	if(sWorld.GMAdminTag)
	{
		if(m_session->CanUseCommand('z')) input3+="<Admin>";
		else if(m_session->GetPermissionCount()) input3+="<GM>";
	}
	if(sWorld.NameinWAnnounce)
	{
	input3+="|r"+sWorld.ann_namecolor+"[";
	input3+=m_session->GetPlayer()->GetName();
	input3+="]:|r "+sWorld.ann_msgcolor;
	}
	else if(!sWorld.NameinWAnnounce) {input3+= ": "; input3+= sWorld.ann_msgcolor;}
	snprintf((char*)pAnnounce, 1024, "%s%s", input3.c_str(), args);

	sWorld.SendWorldWideScreenText(pAnnounce); // send message
	sGMLog.writefromsession(m_session, "used wannounce command [%s]", args);
	return true;
}

bool ChatHandler::HandleGMOnCommand(const char* args, WorldSession *m_session)
{
	GreenSystemMessage(m_session, "Setting GM Flag on yourself.");

	Player * _player = m_session->GetPlayer();
	if(_player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
		RedSystemMessage(m_session, "GM Flag is already set on. Use .gm off to disable it.");
	else
	{
		_player->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);	// <GM>

		_player->SetFaction( 35 );
		_player->RemovePvPFlag();

		BlueSystemMessage(m_session, "GM flag set. It will now appear above your name and in chat messages until you use .gm off.");

		_player->UpdateVisibility();
	}

	return true;
}


bool ChatHandler::HandleGMOffCommand(const char* args, WorldSession *m_session)
{
	Player * _player = m_session->GetPlayer();
	if( _player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM) ){

		_player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);	// <GM>

		_player->SetFaction( _player->GetInitialFactionId() );
		_player->UpdatePvPArea();

		BlueSystemMessage(m_session, "GM Flag Removed. <GM> Will no longer show in chat messages or above your name.");

		_player->UpdateVisibility();
	}

	return true;
}

bool ChatHandler::HandleDeveloperCommand( const char *args, WorldSession *m_session )
{

	HandleGMOffCommand( args, m_session );

	Player * _player = m_session->GetPlayer();

	if( _player->HasFlag( PLAYER_FLAGS, PLAYER_FLAG_DEVELOPER ) )
		_player->RemoveFlag( PLAYER_FLAGS, PLAYER_FLAG_DEVELOPER );
	else
		_player->SetFlag( PLAYER_FLAGS, PLAYER_FLAG_DEVELOPER );

	BlueSystemMessage( m_session, "Developer flag toggled." );

	return true;
}


bool ChatHandler::HandleGPSCommand(const char* args, WorldSession *m_session)
{
	Object *obj;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		if((obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid)) == 0)
		{
			SystemMessage(m_session, "You should select a character or a creature.");
			return true;
		}
	}
	else
		obj = m_session->GetPlayer();

	char buf[328];
	AreaTable * at = dbcArea.LookupEntryForced(obj->GetMapMgr()->GetAreaID(obj->GetPositionX(), obj->GetPositionY()));
	if(!at)
	{
		snprintf((char*)buf, 328, "|cff00ff00Current Position: |cffffffffMap: |cff00ff00%d |cffffffffX: |cff00ff00%f |cffffffffY: |cff00ff00%f |cffffffffZ: |cff00ff00%f |cffffffffOrientation: |cff00ff00%f|r",
			(unsigned int)obj->GetMapId(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation());
		SystemMessage(m_session, buf);
		return true;
	}

	snprintf((char*)buf, 328, "|cff00ff00Current Position: |cffffffffMap: |cff00ff00%d |cffffffffZone: |cff00ff00%u |cffffffffArea: |cff00ff00%u  |cffffffffX: |cff00ff00%f |cffffffffY: |cff00ff00%f |cffffffffZ: |cff00ff00%f |cffffffffOrientation: |cff00ff00%f |cffffffffArea Name: |cff00ff00%s |r",
		(unsigned int)obj->GetMapId(), at->ZoneId,at->AreaId, obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation(),at->name);
	
	
	SystemMessage(m_session, buf);
// ".gps 1" will save gps info to file logs/gps.log - This probably isn't very multithread safe so don't have many gms spamming it!
	if(args != NULL && *args=='1')
	{
		FILE* gpslog = fopen(FormatOutputString("logs","gps",false).c_str(), "at");
		if(gpslog)
		{
			fprintf(gpslog, "%d, %u, %u, %f, %f, %f, %f, \'%s\'",
				(unsigned int)obj->GetMapId(), at->ZoneId,at->AreaId, obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation(),at->name);
// ".gps 1 comment" will save comment after the gps data
			if(*(args+1) == ' ')
				fprintf(gpslog,",%s\n",args+2);
			else
				fprintf(gpslog,"\n");
			fclose(gpslog);
		}
	}
	return true;
}


bool ChatHandler::HandleKickCommand(const char* args, WorldSession *m_session)
{

	if(!*args)
	return false;
	char *pname = strtok((char*)args, " ");
	if(!pname)
	{
		RedSystemMessage(m_session, "No name specified.");
		return true;
	}
	Player *chr = objmgr.GetPlayer((const char*)pname, false);
	if (chr)
	{
		char *reason = strtok(NULL, "\n");
		std::string kickreason = "No reason";
		if(reason)
			kickreason = reason;

		BlueSystemMessage(m_session, "Attempting to kick %s from the server for \'%s\'.", chr->GetName(), kickreason.c_str());
		sGMLog.writefromsession(m_session, "Kicked player %s from the server for %s", chr->GetName(), kickreason.c_str());
		if(!m_session->CanUseCommand('z') && chr->GetSession()->CanUseCommand('z'))
		{
			RedSystemMessage(m_session, "You cannot kick %s, a GM whose permissions outrank yours.", chr->GetName());
			return true;
		}
		/*if(m_session->GetSecurity() < chr->GetSession()->GetSecurity())
		{
			SystemMessage(m_session, "You cannot kick %s, as he is a higher GM level than you.", chr->GetName());
			return true;
		}*/ // we might have to re-work this

		char msg[200];
		snprintf(msg, 200, "%sGM: %s was kicked from the server by %s. Reason: %s", MSG_COLOR_RED, chr->GetName(), m_session->GetPlayer()->GetName(), kickreason.c_str());
		sWorld.SendWorldText(msg, NULL);
		//sWorld.SendIRCMessage(msg);
		SystemMessageToPlr(chr, "You are being kicked from the server by %s. Reason: %s", m_session->GetPlayer()->GetName(), kickreason.c_str());

		chr->Kick(6000);
		
		return true;
	} 
	else 
	{
		RedSystemMessage(m_session, "Player is not online at the moment.");
		return true;
	}
}

bool ChatHandler::HandleAddInvItemCommand(const char *args, WorldSession *m_session)
{
	uint32 itemid, count=1;
	int32 randomprop= 0;
	int32 numadded = 0;

	if(strlen(args) < 1)
	{
		return false;
	}

	if(sscanf(args, "%u %u %d", &itemid, &count, &randomprop) < 1)
	{
		// check for item link
		uint16 ofs = GetItemIDFromLink(args, &itemid);
		if(itemid == 0)
			return false;
		sscanf(args+ofs,"%u %d", &count, &randomprop); // these may be empty
	}

	Player * chr = getSelectedChar( m_session, false );
	if ( chr == NULL )
		chr = m_session->GetPlayer();
	
	ItemPrototype* it = ItemPrototypeStorage.LookupEntry(itemid);
	if(it)
	{
		numadded -= chr->GetItemInterface()->GetItemCount( itemid );
        bool result = false;
        result = chr->GetItemInterface()->AddItemById( itemid, count, randomprop );
		numadded += chr->GetItemInterface()->GetItemCount( itemid );
        if( result == true ){
            if( count == 0 ){
                sGMLog.writefromsession(m_session, "used add item command, item id %u [%s], quantity %u, to %s", it->ItemId, it->Name1, numadded, chr->GetName());
            }else{
                sGMLog.writefromsession(m_session, "used add item command, item id %u [%s], quantity %u (only %lu added due to full inventory), to %s", it->ItemId, it->Name1, numadded, numadded, chr->GetName());
            }
            
            char messagetext[512];
            
            snprintf(messagetext, 512, "Added item %s (id: %d), quantity %u, to %s's inventory.", GetItemLinkByProto(it, m_session->language).c_str(), (unsigned int)it->ItemId, numadded, chr->GetName());
            SystemMessage(m_session, messagetext);
            //snprintf(messagetext, 128, "%s added item %d (%s) to your inventory.", m_session->GetPlayer()->GetName(), (unsigned int)itemid, it->Name1);
            snprintf(messagetext, 512, "%s added item %s, quantity %u, to your inventory.", m_session->GetPlayer()->GetName(), GetItemLinkByProto(it, chr->GetSession()->language).c_str(), numadded);
            
            SystemMessageToPlr(chr,  messagetext);
        }
        return true;
    
    } else{
        RedSystemMessage(m_session, "Item %d is not a valid item!", itemid);
		return true;
	}
}

bool ChatHandler::HandleSummonCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	// Summon Blocking
	if (!stricmp(args, "on"))
	{
		if(m_session->GetPlayer()->IsSummonDisabled())
		{
			BlueSystemMessage(m_session, "Summon blocking is already enabled");
		}
		else
		{
			m_session->GetPlayer()->DisableSummon(true);
			GreenSystemMessage(m_session, "Summon blocking is now enabled");
		}
		return true;
	}
	else if (!stricmp(args, "off"))
	{
		if(m_session->GetPlayer()->IsSummonDisabled())
		{
			m_session->GetPlayer()->DisableSummon(false);
			GreenSystemMessage(m_session, "Summon blocking is now disabled");
		}
		else
		{
			BlueSystemMessage(m_session, "Summon blocking is already disabled");
		}
		return true;
	}

	Player *chr = objmgr.GetPlayer(args, false);
	if (chr)
	{
		// send message to user
		char buf[256];
		char buf0[256];

		if (!m_session->CanUseCommand('z') && chr->IsSummonDisabled())
		{
			snprintf((char*)buf,256, "%s has blocked other GMs from summoning them.", chr->GetName());
			SystemMessage(m_session, buf);
			return true;
		}

		if (chr->GetMapMgr() == NULL)
		{
			snprintf((char*)buf,256, "%s is already being teleported.", chr->GetName());
			SystemMessage(m_session, buf);
			return true;
		}
		snprintf((char*)buf,256, "You are summoning %s.", chr->GetName());
		SystemMessage(m_session, buf);

		// Don't summon the dead, lol, I see dead people. :P
		// If you do, we better bring them back to life
		if (chr->getDeathState() == 1) // Just died
			chr->RemoteRevive();
		if (chr->getDeathState() != 0) // Not alive
			chr->ResurrectPlayer();

		if (!m_session->GetPlayer()->m_isGmInvisible)
		{
			// send message to player
			snprintf((char*)buf0,256, "You are being summoned by %s.", m_session->GetPlayer()->GetName());
			SystemMessageToPlr(chr, buf0);
		}

		Player * plr = m_session->GetPlayer();

		if (plr->GetMapMgr()==chr->GetMapMgr())
			chr->_Relocate(plr->GetMapId(),plr->GetPosition(),false,false,plr->GetInstanceID());
		else
		{
			sEventMgr.AddEvent(chr,&Player::EventPortToGM,plr,0,1,1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
	}
	else
	{
		PlayerInfo * pinfo = objmgr.GetPlayerInfoByName(args);
		if (!pinfo)
		{
			char buf[256];
			snprintf((char*)buf,256,"Player (%s) does not exist.", args);
			SystemMessage(m_session, buf);
			return true;
		}
		else
		{
			Player * pPlayer = m_session->GetPlayer();
			char query[512];
			snprintf((char*) &query,512, "UPDATE characters SET mapId = %u, positionX = %f, positionY = %f, positionZ = %f, zoneId = %u WHERE guid = %u;",	pPlayer->GetMapId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetZoneId(), pinfo->guid);
			CharacterDatabase.Execute(query);
			char buf[256];
			snprintf((char*)buf,256,"(Offline) %s has been summoned.", pinfo->name);
			SystemMessage(m_session, buf);
			return true;
		}
	}

	sGMLog.writefromsession(m_session, "summoned %s on map %u, %f %f %f", args, m_session->GetPlayer()->GetMapId(),m_session->GetPlayer()->GetPositionX(),m_session->GetPlayer()->GetPositionY(),m_session->GetPlayer()->GetPositionZ());
	return true;
}


bool ChatHandler::HandleAppearCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	// Appear Blocking
	if (!stricmp(args, "on"))
	{
		if(m_session->GetPlayer()->IsAppearDisabled())
		{
			BlueSystemMessage(m_session, "Appear blocking is already enabled");
		}
		else
		{
			m_session->GetPlayer()->DisableAppear(true);
			GreenSystemMessage(m_session, "Appear blocking is now enabled");
		}
		return true;
	}
	else if (!stricmp(args, "off"))
	{
		if(m_session->GetPlayer()->IsAppearDisabled())
		{
			m_session->GetPlayer()->DisableAppear(false);
			GreenSystemMessage(m_session, "Appear blocking is now disabled");
		}
		else
		{
			BlueSystemMessage(m_session, "Appear blocking is already disabled");
		}
		return true;
	}

	Player *chr = objmgr.GetPlayer(args, false);
	if (chr)
	{
		char buf[256];

		if (!m_session->CanUseCommand('z') && chr->IsAppearDisabled())
		{
			snprintf((char*)buf,256, "%s has blocked other GMs from appearing to them.", chr->GetName());
			SystemMessage(m_session, buf);
			return true;
		}

		if (chr->GetMapMgr() == NULL)
		{
			snprintf((char*)buf,256, "%s is already being teleported.", chr->GetName());
			SystemMessage(m_session, buf);
			return true;
		}
		snprintf((char*)buf,256, "Appearing at %s's location.", chr->GetName());  // -- europa
		SystemMessage(m_session, buf);

		if (!m_session->GetPlayer()->m_isGmInvisible)
		{
			char buf0[256];
			snprintf((char*)buf0,256, "%s is appearing to your location.", m_session->GetPlayer()->GetName());
			SystemMessageToPlr(chr, buf0);
		}

		//m_session->GetPlayer()->SafeTeleport(chr->GetMapId(), chr->GetInstanceID(), chr->GetPosition());
		//If the GM is on the same map as the player, use the normal safeteleport method
		if ( m_session->GetPlayer()->GetMapId() == chr->GetMapId() && m_session->GetPlayer()->GetInstanceID() == chr->GetInstanceID() )
			m_session->GetPlayer()->SafeTeleport(chr->GetMapId(),chr->GetInstanceID(),chr->GetPosition());
		else
			m_session->GetPlayer()->SafeTeleport(chr->GetMapMgr(), chr->GetPosition());
		//The player and GM are not on the same map. We use this method so we can port to BG's (Above method doesn't support them)
	}
	else
	{
		char buf[256];
		snprintf((char*)buf,256, "Player (%s) does not exist or is not logged in.", args);
		SystemMessage(m_session, buf);
	}

	return true;
}

bool ChatHandler::HandleTaxiCheatCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL) 
		return true;

	if(stricmp(args, "on") == 0)
	{
		GreenSystemMessage(m_session, "%s has all taxi nodes now.", chr->GetName());
		SystemMessage(m_session, "%s has given you all taxi nodes.", m_session->GetPlayer()->GetName());
	}
	else if(stricmp(args, "off") == 0)
	{
		GreenSystemMessage(m_session, "%s has no more taxi nodes now.", chr->GetName());
		SystemMessage(chr->GetSession(), "%s has deleted all your taxi nodes.", m_session->GetPlayer()->GetName());
	}
	else
		return false;

	for (uint8 i= 0; i<12; i++)
	{
		if(stricmp(args, "on") == 0)
		{
			chr->SetTaximask(i, 0xFFFFFFFF);
		}
		else if(stricmp(args, "off") == 0)
		{
			chr->SetTaximask(i, 0);
		}
	}
	return true;
}

bool ChatHandler::HandleModifySpeedCommand(const char* args, WorldSession *m_session)
{
	WorldPacket data;

	if (!*args)
		return false;

	float Speed = (float)atof((char*)args);

	if (Speed > 255 || Speed < 1)
	{
		RedSystemMessage(m_session, "Incorrect value. Range is 1..255");
		return true;
	}

	Player *chr = getSelectedChar(m_session);
	if( chr == NULL )
		return true;

	if ( chr!=m_session->GetPlayer() )
		sGMLog.writefromsession( m_session, "modified speed of %s to %2.2f.",chr->GetName(), Speed );


	char buf[256];

	// send message to user
	BlueSystemMessage(m_session, "You set the speed of %s to %2.2f.", chr->GetName(), Speed);

	// send message to player
	snprintf((char*)buf,256, "%s set your speed to %2.2f.", m_session->GetPlayer()->GetName(), Speed);
	SystemMessage(chr->GetSession(), buf);

	chr->SetPlayerSpeed(RUN, Speed);
	chr->SetPlayerSpeed(SWIM, Speed);
	chr->SetPlayerSpeed(RUNBACK, Speed / 2); // Backwards slower, it's more natural :P
	chr->SetPlayerSpeed(FLY, Speed * 2); // Flying is faster :P

	return true;
}

bool ChatHandler::HandleLearnSkillCommand(const char *args, WorldSession *m_session)
{
	uint32 skill, min, max;
	min = max = 1;
	char *pSkill = strtok((char*)args, " ");
	if(!pSkill)
		return false;
	else
		skill = atol(pSkill);

	BlueSystemMessage(m_session, "Adding skill line %d", skill);

	char *pMin = strtok(NULL, " ");
	if(pMin)
	{
		min = atol(pMin);
		char *pMax = strtok(NULL, "\n");
		if(pMax)
			max = atol(pMax);
	} else {
		return false;
	}

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;
	if(!plr->IsPlayer()) return false;
	sGMLog.writefromsession(m_session, "used add skill of %u %u %u on %s", skill, min, max, plr->GetName());

	plr->_AddSkillLine(skill, min, max);   

	return true;
}

bool ChatHandler::HandleModifySkillCommand(const char *args, WorldSession *m_session)
{
	uint32 skill, min, max;
	min = max = 1;
	char *pSkill = strtok((char*)args, " ");
	if(!pSkill)
		return false;
	else
		skill = atol(pSkill);
	
	char *pMin = strtok(NULL, " ");
	uint32 cnt = 0;
	if(!pMin)
		cnt = 1;
	else
		cnt = atol(pMin);

	skill = atol(pSkill);
	
	BlueSystemMessage(m_session, "Modifying skill line %d. Advancing %d times.", skill, cnt);

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) plr = m_session->GetPlayer();
	if(!plr) return false;
	sGMLog.writefromsession(m_session, "used modify skill of %u %u on %s", skill, cnt,plr->GetName());

	if(!plr->_HasSkillLine(skill))
	{
		SystemMessage(m_session, "Does not have skill line, adding.");
		plr->_AddSkillLine(skill, 1, 300);   
	} else {
		plr->_AdvanceSkillLine(skill,cnt);
	}	   

	return true;
}

/// DGM: Get skill level command for getting information about a skill
bool ChatHandler::HandleGetSkillLevelCommand(const char *args, WorldSession *m_session)
{
	uint32 skill = 0;
	char *pSkill = strtok((char*)args, " ");
	if(!pSkill)
		return false;
	else 
		skill = atol(pSkill);

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;

	if(skill > SkillNameManager->maxskill)
	{
		BlueSystemMessage(m_session, "Skill: %u does not exists", skill);
		return false;
	}

    char * SkillName = SkillNameManager->SkillNames[skill];

    if (SkillName== 0)
    {
        BlueSystemMessage(m_session, "Skill: %u does not exists", skill);
        return false;
    }
    
    if (!plr->_HasSkillLine(skill))
    {
        BlueSystemMessage(m_session, "Player does not have %s skill.", SkillName);
        return false;
    }

	uint32 nobonus = plr->_GetSkillLineCurrent(skill,false);
	uint32 bonus = plr->_GetSkillLineCurrent(skill,true) - nobonus;
    uint32 max = plr->_GetSkillLineMax(skill);

    BlueSystemMessage(m_session, "Player's %s skill has level: %u maxlevel: %u. (+ %u bonus)", SkillName, nobonus, max, bonus);
	return true;
}

bool ChatHandler::HandleGetSkillsInfoCommand(const char *args, WorldSession *m_session)
{
    Player *plr = getSelectedChar(m_session, true);
    if(!plr) return false;
    
    uint32 nobonus = 0;
    int32  bonus = 0;
    uint32 max = 0;

    BlueSystemMessage(m_session, "Player: %s has skills", plr->GetName() );

    for (uint32 SkillId = 0; SkillId <= SkillNameManager->maxskill; SkillId++)
    {
        if (plr->_HasSkillLine(SkillId))
        {
            char * SkillName = SkillNameManager->SkillNames[SkillId];
            if (!SkillName)
            {
                RedSystemMessage(m_session, "Invalid skill: %u", SkillId);
                continue;
            }

            nobonus = plr->_GetSkillLineCurrent(SkillId,false);
            bonus = plr->_GetSkillLineCurrent(SkillId,true) - nobonus;
            max = plr->_GetSkillLineMax(SkillId);

            BlueSystemMessage(m_session, "  %s: Value: %u, MaxValue: %u. (+ %d bonus)", SkillName, nobonus, max, bonus);
        }
    }

    return true;
}


bool ChatHandler::HandleRemoveSkillCommand(const char *args, WorldSession *m_session)
{
	uint32 skill = 0;
	char *pSkill = strtok((char*)args, " ");
	if(!pSkill)
		return false;
	else 
		skill = atol(pSkill);
	BlueSystemMessage(m_session, "Removing skill line %d", skill);

	Player *plr = getSelectedChar(m_session, true);
	if(plr && plr->_HasSkillLine(skill) ) //fix bug; removing skill twice will mess up skills
	{
		plr->_RemoveSkillLine(skill);
		sGMLog.writefromsession(m_session, "used remove skill of %u on %s", skill, plr->GetName());
		SystemMessageToPlr(plr, "%s removed skill line %d from you. ", m_session->GetPlayer()->GetName(), skill);
	}
	else
	{
		BlueSystemMessage(m_session, "Player doesn't have skill line %d", skill);
	}
	return true;
}


bool ChatHandler::HandleEmoteCommand(const char* args, WorldSession *m_session)
{
	uint32 emote = atoi((char*)args);
	Unit* target = this->getSelectedCreature(m_session);
	if(!target) return false;
	if(target) target->SetEmoteState(emote);

	return true;
}

bool ChatHandler::HandleModifyGoldCommand(const char* args, WorldSession *m_session)
{
//	WorldPacket data;

	if ( *args == 0 )
		return false;

	Player *chr = getSelectedChar( m_session, true );
	if( chr == NULL ) return true;

	int32 total   = atoi( (char*)args );

	// gold = total / 10000;
	// silver = (total / 100) % 100;
	// copper = total % 100;
	uint32 gold   = (uint32) std::floor( (float)int32abs( total ) / 10000.0f );
	uint32 silver = (uint32) std::floor( ((float)int32abs( total ) / 100.0f) ) % 100;
	uint32 copper = int32abs2uint32( total ) % 100;
	
	sGMLog.writefromsession( m_session, "used modify gold on %s, gold: %d", chr->GetName(), total );

	int32 newgold = chr->GetGold() + total;

	if(newgold < 0)
	{
		BlueSystemMessage( m_session, "Taking all gold from %s's backpack...", chr->GetName() );
		GreenSystemMessageToPlr(chr, "%s took the all gold from your backpack.", m_session->GetPlayer()->GetName());
		newgold = 0;
	}
	else
	{
		if(total >= 0) {
			BlueSystemMessage( m_session,
				"Adding %u gold, %u silver, %u copper to %s's backpack...",
				gold, silver, copper,
				chr->GetName() );

			GreenSystemMessageToPlr( chr, "%s added %u gold, %u silver, %u copper to your backpack.",
				m_session->GetPlayer()->GetName(),
				gold, silver, copper );
		}
		else
		{
			BlueSystemMessage( m_session,
				"Taking %u gold, %u silver, %u copper from %s's backpack...",
				gold, silver, copper,
				chr->GetName() );

			GreenSystemMessageToPlr( chr, "%s took %u gold, %u silver, %u copper from your backpack.",
				m_session->GetPlayer()->GetName(),
				gold, silver, copper );
		}
	}

	// Check they don't have more than the max gold
	if(sWorld.GoldCapEnabled)
    {
        if( (chr->GetGold() + newgold) > sWorld.GoldLimit)
        {
			RedSystemMessage(m_session, "Maximum amount of gold is %u and %s already has %u", (sWorld.GoldLimit/10000), chr->GetName(), (chr->GetGold()/10000));
            return true;
        }
    }

	chr->SetGold( newgold );
	
	return true;
}

bool ChatHandler::HandleTriggerCommand(const char* args, WorldSession* m_session)
{
	if(!args)
	{
		RedSystemMessage(m_session, "No information was provided.");
		return true;
	}
	int32 instance_id;
	uint32 trigger_id;
	int valcount = sscanf(args, "%u %d", (unsigned int*)&trigger_id, (int*)&instance_id);
	if(valcount < 1)
		return false;
	if(valcount == 1)
		instance_id = 0;

	AreaTriggerEntry *entry = dbcAreaTrigger.LookupEntryForced(trigger_id);
	if(trigger_id == 0 || entry == NULL)
	{
		RedSystemMessage(m_session, "Could not find trigger %s", args);
		return true;
	}

	m_session->GetPlayer()->SafeTeleport(entry->mapid, instance_id, LocationVector(entry->x, entry->y,
			entry->z, entry->o));

	BlueSystemMessage(m_session, "Teleported to trigger %u on [%u][%.2f][%.2f][%.2f]", entry->id,
		entry->mapid, entry->x, entry->y, entry->z);
	return true;
}

bool ChatHandler::HandleUnlearnCommand(const char* args, WorldSession * m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(plr == 0)
		return true;

	uint32 SpellId = atol(args);
	if(SpellId == 0)
	{
		SpellId = GetSpellIDFromLink(args);
		if(SpellId == 0)
		{
			RedSystemMessage(m_session, "You must specify a spell id.");
			return true;
		}
	}

	sGMLog.writefromsession(m_session, "removed spell %u from %s", SpellId, plr->GetName());

	if(plr->HasSpell(SpellId))
	{
		GreenSystemMessageToPlr(plr, "Removed spell %u.", SpellId);
		GreenSystemMessage(m_session, "Removed spell %u from %s.", SpellId, plr->GetName());
		plr->removeSpell(SpellId, false, false, 0);
	}
	else
	{
		RedSystemMessage(m_session, "That player does not have spell %u learnt.", SpellId);
	}

	return true;
}

bool ChatHandler::HandleNpcSpawnLinkCommand(const char* args, WorldSession *m_session)
{
	uint32 id;
	char sql[512];
	Creature* target = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if (!target)
		return false;

	int valcount = sscanf(args, "%u", (unsigned int*)&id);
	if(valcount == 1)
	{
		snprintf(sql, 512, "UPDATE creature_spawns SET npc_respawn_link = '%u' WHERE id = '%u'", (unsigned int)id, (unsigned int)target->GetSQL_id());
		WorldDatabase.Execute( sql );
		BlueSystemMessage(m_session, "Spawn linking for this NPC has been updated: %u", id);
	}
	else
	{
		RedSystemMessage(m_session, "Sql entry invalid %u", id);
	}

	return true;
}

bool ChatHandler::HandleModifyTPsCommand(const char* args, WorldSession *m_session)
{
	if(!args)
		return false;

	Player * Pl = getSelectedChar(m_session, false);
	if(!Pl)
	{
		SystemMessage(m_session, "Invalid or no target provided, please target a player to modify its talentpoints.");
		return true;
	}

	uint32 TP1 = 0;
	uint32 TP2 = 0;
	if(sscanf(args, "%u %u", &TP1, &TP2) != 2)
	{
		SystemMessage(m_session, "Enter two amounts to modify your target's both specs to (enter 0 to that spec at default).");
		return true;
	}
	
	Pl->m_specs[SPEC_PRIMARY].m_customTalentPointOverride = TP1;
	Pl->m_specs[SPEC_SECONDARY].m_customTalentPointOverride = TP2;
	Pl->smsg_TalentsInfo(false);
	return true;
}

#ifdef ENABLE_ACHIEVEMENTS
/**
	Handles .achieve complete
	.achieve complete id                : completes achievement "id" (can be an achievement link) for the selected player
*/
bool ChatHandler::HandleAchievementCompleteCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 achievement_id = atol(args);
	if(achievement_id== 0)
	{
		achievement_id = GetAchievementIDFromLink(args);
		if(achievement_id== 0)
		{
			if( stricmp(args,"all") == 0 )
			{
				plr->GetAchievementMgr().GMCompleteAchievement(m_session, -1);
				SystemMessage(m_session,"All achievements have now been completed for that player.");
				sGMLog.writefromsession(m_session,"completed all achievements for player %s", plr->GetName());
				return true;
			}
			return false;
		}
	}

	if(plr->GetAchievementMgr().GMCompleteAchievement(m_session, achievement_id))
	{
		SystemMessage(m_session,"The achievement has now been completed for that player.");
		sGMLog.writefromsession( m_session, "completed achievement %u for player %s", achievement_id, plr->GetName() );
	}
	return true;
}

/**
	Handles .achieve criteria
	.achieve criteria id                : completes achievement criteria "id" for the selected player
*/
bool ChatHandler::HandleAchievementCriteriaCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 criteria_id = atol(args);
	if( criteria_id == 0 )
	{
		if( stricmp(args,"all")== 0 )
		{
			plr->GetAchievementMgr().GMCompleteCriteria(m_session, -1);
			SystemMessage(m_session,"All achievement criteria have now been completed for that player.");
			sGMLog.writefromsession(m_session,"completed all achievement criteria for player %s", plr->GetName());
			return true;
		}
		return false;
	}

	if(plr->GetAchievementMgr().GMCompleteCriteria(m_session, criteria_id))
	{
		SystemMessage(m_session,"The achievement criteria has now been completed for that player.");
		sGMLog.writefromsession( m_session, "completed achievement criteria %u for player %s", criteria_id, plr->GetName() );
	}
	return true;
}

/**
	Handles .achieve reset
	.achieve reset id                   : removes achievement "id" (can be an achievement link) from the selected player
	.achieve reset criteria id          : removes achievement criteria "id" from the selected player
	.achieve reset all                  : removes all achievement and criteria data from the selected player
*/
bool ChatHandler::HandleAchievementResetCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	bool resetAch = true, resetCri = false;
	int32 achievement_id;
	if(strnicmp(args, "criteria ", 9) == 0)
	{
		achievement_id = atol(args+9);
		if(achievement_id== 0)
		{
			if(stricmp(args+9,"all") != 0)
			{
				return false;
			}
			achievement_id = -1;
		}
		resetCri = true;
		resetAch = false;
	}
	else if(stricmp(args,"all") == 0)
	{
		achievement_id = -1;
		resetCri = true;
	}
	else
	{
		achievement_id = atol(args);
		if(achievement_id== 0)
		{
			achievement_id = GetAchievementIDFromLink(args);
			if(achievement_id== 0)
				return false;
		}
	}

	if(resetAch)
		plr->GetAchievementMgr().GMResetAchievement(achievement_id);
	if(resetCri)
		plr->GetAchievementMgr().GMResetCriteria(achievement_id);
	return true;
}

/**
	Handles .lookup achievement
	GM achievement lookup command usage:
	.lookup achievement string          : searches for "string" in achievement name
	.lookup achievement desc string     : searches for "string" in achievement description
	.lookup achievement reward string   : searches for "string" in achievement reward name
	.lookup achievement criteria string : searches for "string" in achievement criteria name
	.lookup achievement all string      : searches for "string" in achievement name, description, reward, and critiera
*/
bool ChatHandler::HandleLookupAchievementCmd(const char* args, WorldSession* m_session)
{
	if(!*args)
		return false;

	string x;
	bool lookupname = true, lookupdesc = false, lookupcriteria = false, lookupreward = false;
	if( strnicmp(args,"name ",5) == 0 )
	{
		x = string(args+5);
	}
	else if( strnicmp(args,"desc ",5) == 0 )
	{
		lookupname = false;
		lookupdesc = true;
		x = string(args+5);
	}
	else if( strnicmp(args,"criteria ",9) == 0 )
	{
		lookupname = false;
		lookupcriteria = true;
		x = string(args+9);
	}
	else if( strnicmp(args,"reward ",7) == 0 )
	{
		lookupname = false;
		lookupreward = true;
		x = string(args+7);
	}
	else if( strnicmp(args,"all ",4) == 0 )
	{
		lookupdesc = true;
		lookupcriteria = true;
		lookupreward = true;
		x = string(args+4);
	}
	else
	{
		x = string(args);
	}
	if( x.length() < 4 )
	{
		RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
		return true;
	}
	arcemu_TOLOWER(x);
	GreenSystemMessage(m_session, "Starting search of achievement `%s`...", x.c_str());
	uint32 t = getMSTime();
	uint32 i, j, numFound= 0;
	string y, recout;
	char playerGUID[17];
	snprintf(playerGUID,17,I64FMT,m_session->GetPlayer()->GetGUID());

	if( lookupname || lookupdesc || lookupreward )
	{
		std::set<uint32> foundList;
		j = dbcAchievementStore.GetNumRows();
		bool foundmatch;
		for( i = 0; i < j && numFound < 25; ++i )
		{
			AchievementEntry const* achievement = dbcAchievementStore.LookupRowForced(i);
			if(achievement)
			{
				if( foundList.find(achievement->ID) != foundList.end() )
				{
					// already listed this achievement (some achievements have multiple entries in dbc)
					continue;
				}
				foundmatch = false;
				if( lookupname )
				{
					y = string(achievement->name);
					arcemu_TOLOWER(y);
					foundmatch = FindXinYString(x,y);
				}
				if( !foundmatch && lookupdesc )
				{
					y = string(achievement->description);
					arcemu_TOLOWER(y);
					foundmatch = FindXinYString(x,y);
				}
				if( !foundmatch && lookupreward )
				{
					y = string(achievement->rewardName);
					arcemu_TOLOWER(y);
					foundmatch = FindXinYString(x,y);
				}
				if( !foundmatch )
				{
					continue;
				}
				foundList.insert(achievement->ID);
				std::stringstream strm;
				strm << achievement->ID;
				// create achievement link
				recout = "|cffffffffAchievement ";
				recout += strm.str();
				recout += ": |cfffff000|Hachievement:";
				recout += strm.str();
				recout += ":";
				recout += (const char*)playerGUID;
				time_t completetime = m_session->GetPlayer()->GetAchievementMgr().GetCompletedTime(achievement);
				if( completetime )
				{
					// achievement is completed
					struct tm* ct;
					ct = localtime(&completetime);
					strm.str("");
					strm << ":1:" << ct->tm_mon + 1 << ":" << ct->tm_mday << ":" << ct->tm_year - 100 << ":-1:-1:-1:-1|h[";
					recout += strm.str();
				}
				else
				{
					// achievement is not completed
					recout += ":0:0:0:-1:0:0:0:0|h[";
				}
				recout += achievement->name;
				if( !lookupreward )
				{
					recout += "]|h|r";
				}
				else
				{
					recout += "]|h |cffffffff";
					recout += achievement->rewardName;
					recout += "|r";
				}
				strm.str("");
				SendMultilineMessage(m_session,recout.c_str());
				if( ++numFound >= 25 )
				{
					RedSystemMessage(m_session,"More than 25 results found.");
					break;
				}
			}
		} // for loop (number of rows, up to 25)
	} // lookup name or description

	if( lookupcriteria && numFound < 25 )
	{
		std::set<uint32> foundList;
		j = dbcAchievementCriteriaStore.GetNumRows();
		for( i = 0; i < j && numFound < 25; ++i )
		{
			AchievementCriteriaEntry const* criteria = dbcAchievementCriteriaStore.LookupRowForced(i);
			if( criteria )
			{
				if( foundList.find(criteria->ID) != foundList.end() )
				{
					// already listed this achievement (some achievements have multiple entries in dbc)
					continue;
				}
				y = string(criteria->name);
				arcemu_TOLOWER(y);
				if( !FindXinYString(x,y) )
				{
					continue;
				}
				foundList.insert(criteria->ID);
				std::stringstream strm;
				strm << criteria->ID;
				recout = "|cffffffffCriteria ";
				recout += strm.str();
				recout += ": |cfffff000";
				recout += criteria->name;
				strm.str("");
				AchievementEntry const* achievement = dbcAchievementStore.LookupEntryForced(criteria->referredAchievement);
				if( achievement )
				{
					// create achievement link
					recout += " |cffffffffAchievement ";
					strm << achievement->ID;
					recout +=  strm.str();
					recout += ": |cfffff000|Hachievement:";
					recout += strm.str();
					recout += ":";
					recout += (const char*)playerGUID;
					time_t completetime = m_session->GetPlayer()->GetAchievementMgr().GetCompletedTime(achievement);
					if( completetime )
					{
						// achievement is completed
						struct tm* ct;
						ct = localtime(&completetime);
						strm.str("");
						strm << ":1:" << ct->tm_mon + 1 << ":" << ct->tm_mday << ":" << ct->tm_year - 100 << ":-1:-1:-1:-1|h[";
						recout += strm.str();
					}
					else
					{
						// achievement is not completed
						recout += ":0:0:0:-1:0:0:0:0|h[";
					}
					recout += achievement->name;
					if( !lookupreward )
					{
						recout += "]|h|r";
					}
					else
					{
						recout += "]|h |cffffffff";
						recout += achievement->rewardName;
						recout += "|r";
					}
					strm.str("");
				}
				SendMultilineMessage(m_session,recout.c_str());
				if( ++numFound >= 25 )
				{
					RedSystemMessage(m_session,"More than 25 results found.");
					break;
				}
			}
		} // for loop (number of rows, up to 25)
	} // lookup criteria

	if( numFound == 0 )
	{
		recout = "|cff00ccffNo matches found.";
		SendMultilineMessage(m_session,recout.c_str());
	}

	BlueSystemMessage(m_session,"Search completed in %u ms.",getMSTime()-t);

	return true;
}



#endif

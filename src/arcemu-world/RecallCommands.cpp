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

bool ChatHandler::HandleRecallGoCommand(const char* args, WorldSession *m_session)
{
	if( args == NULL )
		return false;

	if( !*args )
		return false;

	if( m_session == NULL )
		return false;

	QueryResult *result = WorldDatabase.Query( "SELECT * FROM recall ORDER BY name" );

	if( result == NULL)
		return false;

	do
	{
		Field* fields = result->Fetch();
		const char* locname = fields[1].GetString();
		uint32 locmap = fields[2].GetUInt32();
		float x = fields[3].GetFloat();
		float y = fields[4].GetFloat();
		float z = fields[5].GetFloat();

		if( strnicmp( const_cast< char* >( args ), locname, strlen( args ) ) == 0 )
		{
			if( m_session->GetPlayer() != NULL )
			{
				m_session->GetPlayer()->SafeTeleport(locmap, 0, LocationVector(x, y, z));
				delete result;
				return true;
			}
			else
			{
				delete result;
				return false;
			}
		}

	}while (result->NextRow());

	delete result;
	return false;
}

bool ChatHandler::HandleRecallAddCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;
	
	QueryResult *result = WorldDatabase.Query( "SELECT name FROM recall" );
	if(!result)
		return false;
	do
	{
		Field *fields = result->Fetch();
		const char * locname = fields[0].GetString();
		if (strncmp((char*)args,locname,strlen(locname))==0)
		{
			RedSystemMessage(m_session, "Name in use, please use another name for your location.");
			delete result;
			return true;
		}
	}while (result->NextRow());
	delete result;

	Player* plr = m_session->GetPlayer();
	std::stringstream ss;
	
	string rc_locname = string(args);

	ss << "INSERT INTO recall (name, mapid, positionX, positionY, positionZ) VALUES ('"
	<< WorldDatabase.EscapeString(rc_locname).c_str() << "' , "
	<< plr->GetMapId() << ", "
	<< plr->GetPositionX() << ", " 
	<< plr->GetPositionY() << ", "
	<< plr->GetPositionZ() << ");";
	WorldDatabase.Execute( ss.str( ).c_str( ) );

	char buf[256]; 
	snprintf((char*)buf, 256, "Added location to DB with MapID: %d, X: %f, Y: %f, Z: %f",
		(unsigned int)plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ());
	GreenSystemMessage(m_session, buf);
	sGMLog.writefromsession(m_session, "used recall add, added \"%s\" location to database.", rc_locname.c_str());

	return true;
}

bool ChatHandler::HandleRecallDelCommand(const char* args, WorldSession *m_session)
{
	   if(!*args)
		return false;

	QueryResult *result = WorldDatabase.Query( "SELECT id,name FROM recall" );
	if(!result)
		return false;

	do
	{
		Field *fields = result->Fetch();
		float id = fields[0].GetFloat();
		const char * locname = fields[1].GetString();

		if (strnicmp((char*)args,locname,strlen(locname))==0)
		{
			std::stringstream ss;
			ss << "DELETE FROM recall WHERE id = "<< (int)id <<";";
			WorldDatabase.Execute( ss.str( ).c_str( ) );
			GreenSystemMessage(m_session, "Recall location removed.");
			sGMLog.writefromsession(m_session, "used recall delete, removed \"%s\" location from database.", args);
			delete result;
			return true;
		}

	}while (result->NextRow());

	delete result;
	return false;
}

bool ChatHandler::HandleRecallListCommand(const char* args, WorldSession *m_session)
{
	QueryResult *result = WorldDatabase.Query( "SELECT id,name FROM recall ORDER BY name" );
	if(!result)
		return false;
	std::string recout;
	uint32 count = 0;

	recout = "|cff00ff00Recall locations|r:\n\n";
	do
	{
		Field *fields = result->Fetch();
		//float id = fields[0].GetFloat();
		const char * locname = fields[1].GetString();
		recout += "|cff00ccff";
		recout += locname;
		recout += "|r, ";
		count++;
		
		if(count == 5)
		{
			recout += "\n";
			count = 0;
		}
	}while (result->NextRow());
	SendMultilineMessage(m_session, recout.c_str());

	delete result;
	return true;
}

bool ChatHandler::HandleRecallPortPlayerCommand(const char* args, WorldSession * m_session)
{
	char location[255];
	char player[255];
	if(sscanf(args, "%s %s", player, location) != 2)
		return false;

	Player * plr = objmgr.GetPlayer(player, false);
	if(!plr) return false;

	QueryResult *result = WorldDatabase.Query( "SELECT * FROM recall ORDER BY name" );
	if(!result)
		return false;

	do
	{
		Field *fields = result->Fetch();
		const char * locname = fields[1].GetString();
		uint32 locmap = fields[2].GetUInt32();
		float x = fields[3].GetFloat();
		float y = fields[4].GetFloat();
		float z = fields[5].GetFloat();

		if (strnicmp((char*)location,locname,strlen(args))==0)
		{
			sGMLog.writefromsession( m_session, "ported %s to %s ( map: %u, x: %f, y: %f, z: %f )", plr->GetName(), locname, locmap, x, y, z );
			if(plr->GetInstanceID() != m_session->GetPlayer()->GetInstanceID())
				sEventMgr.AddEvent(plr, &Player::EventSafeTeleport, locmap, uint32(0), LocationVector(x, y, z), EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			else
				plr->SafeTeleport(locmap, 0, LocationVector(x, y, z));
			delete result;
			return true;
		}

	}while (result->NextRow());

	delete result;
	return false;
}

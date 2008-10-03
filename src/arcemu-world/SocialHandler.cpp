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

void WorldSession::HandleFriendListOpcode( WorldPacket & recv_data )
{
	uint32 flag;
	recv_data >> flag;
	_player->Social_SendFriendList( flag );
}

void WorldSession::HandleAddFriendOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_ADD_FRIEND"  );

	string name, note;
	recv_data >> name;
	recv_data >> note;

	_player->Social_AddFriend( name.c_str(), note.size() ? note.c_str() : NULL );
}

void WorldSession::HandleDelFriendOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_DEL_FRIEND"  );

	uint64 FriendGuid;
	recv_data >> FriendGuid;

	_player->Social_RemoveFriend( (uint32)FriendGuid );
}

void WorldSession::HandleAddIgnoreOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_ADD_IGNORE" );

	std::string ignoreName = "UNKNOWN";
	recv_data >> ignoreName;

	_player->Social_AddIgnore( ignoreName.c_str() );
}

void WorldSession::HandleDelIgnoreOpcode( WorldPacket & recv_data )
{
	sLog.outDebug( "WORLD: Received CMSG_DEL_IGNORE" );

	uint64 IgnoreGuid;
	recv_data >> IgnoreGuid;

	_player->Social_RemoveIgnore( (uint32)IgnoreGuid );
}

void WorldSession::HandleSetFriendNote(WorldPacket & recv_data)
{
	uint64 guid;
	string note;

	recv_data >> guid >> note;
	_player->Social_SetNote( (uint32)guid, note.size() ? note.c_str() : NULL );
}

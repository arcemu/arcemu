/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LUAPACKET_H
#define _LUAPACKET_H

#include "StdAfx.h"
#include "LUAEngine.h"
namespace luPacket
{
	/*
		NORMAL OPERATIONS
		*/

	int CreatePacket(lua_State* L, WorldPacket* packet)
	{
		int opcode = luaL_checkint(L, 1);
		int size = luaL_checkint(L, 2);
		if(opcode >= NUM_MSG_TYPES)
			luaL_error(L, "CreatePacket got opcode %d greater than max opcode %d.", opcode, NUM_MSG_TYPES);
		else
		{
			WorldPacket* npacket = new WorldPacket(opcode, size);
			PUSH_PACKET(L, npacket);
		}
		return 1;
	}
	int GetOpcode(lua_State* L, WorldPacket* packet)
	{
		if(packet == NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L, packet->GetOpcode());
		return 1;
	}
	int GetSize(lua_State* L, WorldPacket* packet)
	{
		if(packet == NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L, packet->size());
		return 1;
	}
	/*
		READ OPERATIONS
		*/
	int ReadUByte(lua_State* L, WorldPacket* packet)
	{
		if(packet == NULL)
			lua_pushnil(L);
		else
		{
			uint8 byte;
			(*packet) >> byte;
			lua_pushinteger(L, byte);
		}
		return 1;
	}
	int ReadByte(lua_State* L, WorldPacket* packet)
	{
		if(packet == NULL)
			lua_pushnil(L);
		else
		{
			int8 byte;
			(*packet) >> byte;
			lua_pushinteger(L, byte);
		}
		return 1;
	}
	int ReadShort(lua_State* L, WorldPacket* packet)
	{
		int16 val;
		if(packet != NULL)
		{
			(*packet) >> val;
			lua_pushinteger(L, val);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadUShort(lua_State* L, WorldPacket* packet)
	{
		uint16 val;
		if(packet != NULL)
		{
			(*packet) >> val;
			lua_pushinteger(L, val);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadLong(lua_State* L, WorldPacket* packet)
	{
		int32 val;
		if(packet != NULL)
		{
			(*packet) >> val;
			lua_pushinteger(L, val);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadULong(lua_State* L, WorldPacket* packet)
	{
		uint32 val;
		if(packet != NULL)
		{
			(*packet) >> val;
			lua_pushinteger(L, val);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadFloat(lua_State* L, WorldPacket* packet)
	{
		float val;
		if(packet != NULL)
		{
			(*packet) >> val;
			lua_pushnumber(L, val);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadDouble(lua_State* L, WorldPacket* packet)
	{
		double val;
		if(packet != NULL)
		{
			(*packet) >> val;
			lua_pushnumber(L, val);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadGUID(lua_State* L, WorldPacket* packet)
	{
		uint64 guid;
		if(packet != NULL)
		{
			(*packet) >> guid;
			PUSH_GUID(L, guid);
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadWoWGuid(lua_State* L, WorldPacket* packet)
	{
		WoWGuid nGuid;
		if(packet != NULL)
		{
			(*packet) >> nGuid;
			PUSH_GUID(L, nGuid.GetOldGuid());
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int ReadString(lua_State* L, WorldPacket* packet)
	{
		string str;
		if(packet != NULL)
		{
			(*packet) >> str;
			lua_pushstring(L, str.c_str());
		}
		else
			lua_pushnil(L);
		return 1;
	}

	/*
		WRITE OPERATIONS
		*/

	int WriteByte(lua_State* L, WorldPacket* packet)
	{
		int8 byte = (int8)luaL_checkint(L, 1);
		(*packet) << byte;
		return 0;
	}
	int WriteUByte(lua_State* L, WorldPacket* packet)
	{
		uint8 byte = (uint8)luaL_checkint(L, 1);
		(*packet) << byte;
		return 0;
	}
	int WriteShort(lua_State* L, WorldPacket* packet)
	{
		int16 val = (int16)luaL_checkint(L, 1);
		(*packet) << val;
		return 0;
	}
	int WriteUShort(lua_State* L, WorldPacket* packet)
	{
		uint16 val = (uint16)luaL_checkint(L, 1);
		(*packet) << val;
		return 0;
	}
	int WriteLong(lua_State* L, WorldPacket* packet)
	{
		int32 val = (int32)luaL_checkint(L, 1);
		(*packet) << val;
		return 0;
	}
	int WriteULong(lua_State* L, WorldPacket* packet)
	{
		uint32 val = (uint32)luaL_checkint(L, 1);
		(*packet) << val;
		return 0;
	}
	int WriteFloat(lua_State* L, WorldPacket* packet)
	{
		float val = (float)luaL_checknumber(L, 1);
		(*packet) << val;
		return 0;
	}
	int WriteDouble(lua_State* L, WorldPacket* packet)
	{
		double val = luaL_checknumber(L, 1);
		(*packet) << val;
		return 0;
	}
	int WriteGUID(lua_State* L, WorldPacket* packet)
	{
		uint64 guid = CHECK_GUID(L, 1);
		(*packet) << guid;
		return 0;
	}
	int WriteWoWGuid(lua_State* L, WorldPacket* packet)
	{
		Object* target = CHECK_OBJECT(L, 1);
		if(packet != NULL)
			(*packet) << target->GetNewGUID();
		return 0;
	}
	int WriteString(lua_State* L, WorldPacket* packet)
	{
		string str = string(luaL_checkstring(L, 1));
		(*packet) << str;
		return 0;
	}

	int GetObjectType(lua_State* L, WorldPacket* packet)
	{
		if(!packet)
		{
			lua_pushnil(L);
			return 1;
		}
		lua_pushstring(L, "Packet");
		return 1;
	}
};

#endif




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

#ifndef LUA_SQL_H
#define LUA_SQL_H

namespace luaSql
{
	//QueryResult methods
	int GetColumn(lua_State* L, QueryResult* res)
	{
		if(res != NULL)
		{
			uint32 column = CHECK_ULONG(L, 1);
			uint32 fields = res->GetFieldCount();
			if(column > fields)
				luaL_error(L, "GetColumn, Column %d bigger than max column %d", column, res->GetFieldCount());
			else
			{
				Field* field = &(res->Fetch()[column]);
				if(field == NULL)
					lua_pushnil(L);
				else
					PUSH_SQLFIELD(L, field);
			}
		}
		else
			lua_pushnil(L);
		return 1;
	}
	int NextRow(lua_State* L, QueryResult* res)
	{
		if(res != NULL)
		{
			if(res->NextRow())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		else
			lua_pushboolean(L, 0);
		return 1;
	}
	int GetColumnCount(lua_State* L, QueryResult* res)
	{
		if(res == NULL)
			lua_pushnil(L);
		else
			lua_pushnumber(L, res->GetFieldCount());
		return 1;
	}

	int GetRowCount(lua_State* L, QueryResult* res)
	{
		if(res == NULL)
			lua_pushnil(L);
		else
			lua_pushnumber(L, res->GetRowCount());
		return 1;
	}

	// Field api
	int GetString(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushstring(L, field->GetString());
		return 1;
	}
	int GetFloat(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushnumber(L, field->GetFloat());
		return 1;
	}
	int GetBool(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
		{
			if(field->GetBool())
				lua_pushboolean(L, 1);
			else
				lua_pushboolean(L, 0);
		}
		return 1;
	}
	int GetUByte(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L, field->GetUInt8());
		return 1;
	}
	int GetByte(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L, field->GetInt8());
		return 1;
	}
	int GetUShort(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L, field->GetUInt16());
		return 1;
	}
	int GetShort(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushinteger(L, field->GetInt16());
		return 1;
	}
	int GetULong(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushnumber(L, field->GetUInt32());
		return 1;
	}
	int GetLong(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
			lua_pushnumber(L, field->GetInt32());
		return 1;
	}
	int GetGUID(lua_State* L, Field* field)
	{
		if(field == NULL)
			lua_pushnil(L);
		else
		{
			uint64 guid = field->GetUInt64();
			PUSH_GUID(L, guid);
		}
		return 1;
	}
}

#endif
/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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

#include "LUAEngine.h"

//Quick class wrapper method that pushes field objects to lua.
class LuaResult : public QueryResult
{
	public:
		LuaResult() : QueryResult(0, 0) {}
		LuaResult(const LuaResult & other) : QueryResult(other.GetFieldCount(), other.GetRowCount()) {}
		Field* GetColumn(int clm)
		{
			if((uint32)clm <= GetFieldCount())
				return &Fetch()[clm];
			return NULL;
		}
};

namespace lua_engine
{
	void bindSQLMethods(luabridge::module & m)
	{
		m	.class_decl<QueryResult>("QueryResult");
		m	.class_<QueryResult>("QueryResult")
		.method(&QueryResult::NextRow, "NextRow", "nextRow", "nextrow", NULL)
		.method(&QueryResult::GetFieldCount, "GetFieldCount", "getFieldCount", "getfieldcount", NULL)
		.method((Field * (LuaResult::*)(int))&LuaResult::GetColumn, "GetColumn", "getColumn" , "getcolumn", NULL)
		.method(&QueryResult::GetRowCount, "GetRowCount", "getRowCount", "getrowcount", NULL);

		m	.class_<Field>("SQL_Field")
		.method(&Field::GetString, "GetString", "getString", "getstring", NULL)
		.method(&Field::GetInt8, "GetByte", "getbyte", "GetInt8", "getInt8", "getint8", NULL)
		.method(&Field::GetUInt8, "GetUInt8", "getuint8", "GetUByte", "getubyte", "getUByte", NULL)
		.method(&Field::GetInt16, "GetInt16", "getint16", "GetShort", "getShort", NULL)
		.method(&Field::GetUInt16, "GetUInt16", "getUInt16", "getuint16", "getushort", "GetUShort", "getUShort", NULL)
		.method(&Field::GetInt32, "GetInt32", "getInt32", "getint32", "getlong", "GetLong", "getLong", NULL)
		.method(&Field::GetUInt32, "GetUInt32", "getUInt32", "getuint32", "GetULong", "getULong", "getulong", NULL)
		.method(&Field::GetFloat, "GetFloat", "getFloat", "getfloat", NULL)
		.method(&Field::GetUInt64, "GetGUID", "getGUID", "getguid", "GetUInt64", "getUInt64", "getuint64", NULL);
	}
}
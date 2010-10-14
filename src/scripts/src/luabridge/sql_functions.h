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

#pragma once

//Quick class wrapper method that pushes field objects to lua.

namespace lua_engine
{
	void bindSQLMethods(luabridge::module & m)
	{
		m	.class_<LuaResult>("SQL_Result")
			.method("NextRow", &QueryResult::NextRow)
			.method("GetFieldCount", &QueryResult::GetFieldCount)
			.method("GetColumn", &LuaResult::GetColumn)
			.method("GetRowCount", &QueryResult::GetRowCount);

		m	.class_<Field>("SQL_Field")
			.method("GetString", &Field::GetString)
			.method("GetByte", &Field::GetInt8)
			.method("GetInt8", &Field::GetInt8)
			.method("GetUByte", &Field::GetUInt8)
			.method("GetUInt8", &Field::GetUInt8)
			.method("GetShort", &Field::GetInt16)
			.method("GetUInt16", &Field::GetUInt16)
			.method("GetInt16", &Field::GetInt16)
			.method("GetUShort", &Field::GetUInt16)
			.method("GetLong", &Field::GetInt32)
			.method("GetInt32", &Field::GetInt32)
			.method("GetULong", &Field::GetUInt32)
			.method("GetUInt32", &Field::GetUInt32);
	}
}
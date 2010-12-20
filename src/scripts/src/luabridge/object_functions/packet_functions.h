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
#include "StdAfx.h"
#include "LUAEngine.h"
class luapacket : public WorldPacket
{
public:
	luapacket(int16 op, size_t size) : WorldPacket(op, size) {}
#define define_method(type) void write##type(type val) { *this << val; }
	define_method(int8)
	define_method(uint8)
	define_method(int16)
	define_method(uint16)
	define_method(int32)
	define_method(uint32)
	void writestring(const char * str)
	{
		*this << string(str);
	}
	void writewowguid(WoWGuid * guid)
	{
		*this << *guid;
	}
};

namespace lua_engine
{

	void bindPacketMethods(luabridge::module & m)
	{
		m	.class_<luapacket>("LuaPacket",true)
			.constructor<void (*)(int16,size_t res)>()
			.method("opcode", &WorldPacket::GetOpcode)
			.method("clear", &WorldPacket::clear)
			.method("WriteInt8", &luapacket::writeint8)
			.method("WriteUInt8", &luapacket::writeuint8)
			.method("WriteInt16", &luapacket::writeint16)
			.method("WriteUInt16", &luapacket::writeuint16)
			.method("WriteInt32", &luapacket::writeint32)
			.method("WriteUInt32", &luapacket::writeuint32)
			.method("WriteWoWGuid", &luapacket::writewowguid)
			.method("size", &luapacket::size);
	}
}


		

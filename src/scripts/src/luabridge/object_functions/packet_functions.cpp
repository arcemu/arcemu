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

class luapacket : public WorldPacket
{
	public:
		luapacket(uint16 op, size_t size) : WorldPacket(op, size) {}
#define define_method(type) void write##type(type val) { *this << val; }
		define_method(int8)
		define_method(uint8)
		define_method(int16)
		define_method(uint16)
		define_method(int32)
		define_method(uint32)
		define_method(float)
		define_method(double)
		define_method(uint64)
#undef define_method
		void writestring(const char* str)
		{
			*this << string(str);
		}
		void writewowguid(WoWGuid* guid)
		{
			*this << *guid;
		}
};

namespace lua_engine
{

	void bindPacketMethods(luabridge::module & m)
	{
		m	.class_decl<luapacket>("LuaPacket");
		m	.class_<WorldPacket>("LuaPacket", true)
		.constructor<void (*)(uint16, uint32)>()
		.method(&WorldPacket::GetOpcode, "GetOpcode", "getOpcode", "getopcode", "opcode", "Opcode", NULL)
		.method(&WorldPacket::clear, "Clear", "clear", "Reset", NULL)
		.method(&luapacket::writeint8, "WriteInt8", "writeint8", "writebyte", "WriteByte", NULL)
		.method(&luapacket::writeuint8, "WriteUInt8", "writeuint8", "writeubyte", "WriteUByte", NULL)
		.method(&luapacket::writeint16, "WriteInt16", "writeint16", "WriteShort", "writeshort", NULL)
		.method(&luapacket::writeuint16, "WriteUInt16", "writeuint16", "writeushort", "WriteUShort", NULL)
		.method(&luapacket::writeint32, "WriteInt32", "writeint32", "writelong", "WriteLong", NULL)
		.method(&luapacket::writeuint32, "WriteUInt32", "writeuint32", "writeulong", "WriteULong", NULL)
		.method(&luapacket::writewowguid, "WriteWowGUID", "writewowguid", "writeWoWGUID", NULL)
		.method(&luapacket::size, "Size", "size", NULL)
		.method(&luapacket::writestring, "WriteString", "writestring", NULL)
		.method(&luapacket::writefloat, "WriteFloat", "writeFloat", "writefloat", NULL)
		.method(&luapacket::writedouble, "WriteDouble", "writeDouble", "writedouble", NULL)
		.method(&luapacket::writeuint64, "WriteGUID", "writeGUID", "writeguid", NULL);
	}
}




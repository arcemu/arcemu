/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef WOWSERVER_WORLDPACKET_H
#define WOWSERVER_WORLDPACKET_H

#include "System/Platform.h"
#include "Container/ByteBuffer.h"

class SERVER_DECL PacketBuffer : public ByteBuffer
{
	public:
		static const uint32 DEFAULT_PACKET_SIZE = 128u;

	public:
		__inline PacketBuffer() : ByteBuffer(DEFAULT_PACKET_SIZE), m_opcode(0) { }
		__inline PacketBuffer(uint16 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode) {}
		__inline PacketBuffer(size_t res) : ByteBuffer(res), m_opcode(0) { }
		__inline PacketBuffer(const PacketBuffer & packet) : ByteBuffer(packet), m_opcode(packet.m_opcode) {}

		//! Clear packet and set opcode all in one mighty blow
		__inline void Initialize(uint16 opcode)
		{
			clear();
			m_opcode = opcode;
		}

		__inline uint16 GetOpcode() const { return m_opcode; }
		__inline void SetOpcode(uint16 opcode) { m_opcode = opcode; }

	protected:
		uint16 m_opcode;
};

#endif

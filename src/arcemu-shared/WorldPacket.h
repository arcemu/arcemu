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

#include "Common.h"
#include "ByteBuffer.h"
#include "StackBuffer.h"

class SERVER_DECL WorldPacket : public ByteBuffer
{
	public:
		__inline WorldPacket() : ByteBuffer(), m_opcode(0) { }
		__inline WorldPacket(uint16 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode) {}
		__inline WorldPacket(size_t res) : ByteBuffer(res), m_opcode(0) { }
		__inline WorldPacket(const WorldPacket & packet) : ByteBuffer(packet), m_opcode(packet.m_opcode) {}

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

template<uint32 Size>
class SERVER_DECL StackWorldPacket : public StackBuffer<Size>
{
		uint16 m_opcode;
	public:
		__inline StackWorldPacket(uint16 opcode) : StackBuffer<Size>(), m_opcode(opcode) { }

		//! Clear packet and set opcode all in one mighty blow
		__inline void Initialize(uint16 opcode)
		{
			StackBuffer<Size>::Clear();
			m_opcode = opcode;
		}

		uint16 GetOpcode() { return m_opcode; }
		__inline void SetOpcode(uint16 opcode) { m_opcode = opcode; }
};

#endif

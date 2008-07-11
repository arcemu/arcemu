/*
 * ArcEmu MMORPG Server
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

#ifndef _STACKBUFFER_H
#define _STACKBUFFER_H

#include "Common.h"
#include "WoWGuid.h"
#include "LocationVector.h"

/* This is needed because of template gayness. */
class SERVER_DECL StackBufferBase
{
public:
	virtual ~StackBufferBase() {}
	virtual uint8 * GetBufferPointer() = 0;
	virtual uint16 GetOpcode() { return 0; }
	virtual uint32 GetSize() = 0;
};

template<uint32 Size>
class SERVER_DECL StackBuffer : public StackBufferBase
{
protected:
	uint8 m_stackBuffer[Size];
        uint32 m_readPos;
        uint32 m_writePos;
	uint8 * m_bufferPointer;
	uint8 * m_heapBuffer;
	uint32 m_space;

public:

	/** Constructor, sets buffer pointers and zeros write/read positions.
	 */
	StackBuffer() : m_readPos(0), m_writePos(0), m_bufferPointer(&m_stackBuffer[0]), m_heapBuffer(0), m_space(Size) {}

	/** Destructor, frees heap buffer if it exists
	 */
	~StackBuffer() { if(m_heapBuffer) free(m_heapBuffer); }

	/** Re-allocates the buffer on the heap. This allows it to expand past the original specified size.
	 * This is only a failsafe and should be avoided at all costs, as it is quite heavy. 
	 */
	void ReallocateOnHeap()
	{
		if(m_heapBuffer)			// Reallocate with 200 bytes larger size
			m_heapBuffer = (uint8*)realloc(m_heapBuffer, 200);
		else
		{
			m_heapBuffer = (uint8*)malloc(m_space + 200);
			memcpy(m_heapBuffer, m_stackBuffer, m_writePos);
			m_space += 200;
			m_bufferPointer = m_heapBuffer;
		}
	}

	/** Gets the buffer pointer
	 * @return the buffer pointer
	 */
	uint8 * GetBufferPointer() { return m_bufferPointer; }

	/** Gets the buffer size.
	 */
	uint32 GetBufferSize() { return m_writePos; }

	/** Reads sizeof(T) bytes from the buffer
	 * @return the bytes read
	 */
	template<typename T>
		T Read()
	{
		if(m_readPos + sizeof(T) <= m_writePos)
			return (T)0;
		T ret = *(T*)&m_bufferPointer[m_readPos];
		m_readPos += sizeof(T);
		return ret;
	}

	/** Writes sizeof(T) bytes to the buffer, while checking for overflows.
	 * @param T data The data to be written
	 */
	template<typename T>
		void Write(T data)
	{
		if(m_writePos + sizeof(T) > m_space)
		{
			// Whoooops. We have to reallocate on the heap.
			ReallocateOnHeap();
		}

		*(T*)&m_bufferPointer[m_writePos] = data;
		m_writePos += sizeof(T);
	}

	/** writes x bytes to the buffer, while checking for overflows
	 * @param ptr the data to be written
	 * @param size byte count
	 */
	void Write(uint8 * data, size_t size)
	{
		if(m_writePos + size > m_space)
			ReallocateOnHeap();

		memcpy(&m_bufferPointer[m_writePos], data, size);
		m_writePos += uint32(size);
	}

	/** Ensures the buffer is big enough to fit the specified number of bytes.
	 * @param bytes number of bytes to fit
	 */
	ARCEMU_INLINE void EnsureBufferSize(uint32 Bytes)
	{
		if(m_writePos + Bytes > m_space)
			ReallocateOnHeap();
	}

	/** These are the default read/write operators.
	 */
#define DEFINE_BUFFER_READ_OPERATOR(type) void operator >> (type& dest) { dest = Read<type>(); }
#define DEFINE_BUFFER_WRITE_OPERATOR(type) void operator << (type src) { Write<type>(src); }

	/** Fast read/write operators without using the templated read/write functions.
	 */
#define DEFINE_FAST_READ_OPERATOR(type, size) StackBuffer<Size>& operator >> (type& dest) { if(m_readPos + size > m_writePos) { dest = (type)0; } else { dest = *(type*)&m_bufferPointer[m_readPos]; m_readPos += size; return *this; } }
#define DEFINE_FAST_WRITE_OPERATOR(type, size) StackBuffer<Size>& operator << (type src) { if(m_writePos + size > m_space) { ReallocateOnHeap(); } *(type*)&m_bufferPointer[m_writePos] = src; m_writePos += size; return *this; }

	/** Integer/float r/w operators
	 */
	DEFINE_FAST_READ_OPERATOR(uint64, 8);
	DEFINE_FAST_READ_OPERATOR(uint32, 4);
	DEFINE_FAST_READ_OPERATOR(uint16, 2);
	DEFINE_FAST_READ_OPERATOR(uint8, 1);
	DEFINE_FAST_READ_OPERATOR(float, 4);
	DEFINE_FAST_READ_OPERATOR(double, 8);

	DEFINE_FAST_WRITE_OPERATOR(uint64, 8);
	DEFINE_FAST_WRITE_OPERATOR(uint32, 4);
	DEFINE_FAST_WRITE_OPERATOR(uint16, 2);
	DEFINE_FAST_WRITE_OPERATOR(uint8, 1);
	DEFINE_FAST_WRITE_OPERATOR(float, 4);
	DEFINE_FAST_WRITE_OPERATOR(double, 8);

	/** boolean (1-byte) read/write operators
	 */
	DEFINE_FAST_WRITE_OPERATOR(bool, 1);
	StackBuffer<Size>& operator >> (bool & dst) { dst = (Read<char>() > 0 ? true : false); return *this; }

	/** string (null-terminated) operators
	 */
	StackBuffer<Size>& operator << (std::string & value) { EnsureBufferSize(value.length() + 1); memcpy(&m_bufferPointer[m_writePos], value.c_str(), value.length()+1); m_writePos += (value.length() + 1); return *this; }
	StackBuffer<Size>& operator >> (std::string & dest)
	{
		dest.clear();
		char c;
		for(;;)
		{
			c = Read<char>();
			if(c == 0) break;
			dest += c;
		}
		return *this;
	}

	/** WoWGuid read/write operators
	 */
	StackBuffer<Size>& operator << (const WoWGuid & value)
	{
		EnsureBufferSize(value.GetNewGuidLen() + 1);
		Write<uint8>(value.GetNewGuidMask());
		memcpy(&m_bufferPointer[m_writePos], value.GetNewGuid(), value.GetNewGuidLen());
		m_writePos += value.GetNewGuidLen();
		return *this;
	}
	
	StackBuffer<Size>& operator >> (WoWGuid & value)
	{
		uint8 mask = Read<uint8>();
		value.Init(mask);
		for(uint32 i = 0; i < BitCount8(mask); ++i)
			value.AppendField(Read<uint8>());
		return *this;
	}

	/** LocationVector read/write operators
	 */
	StackBuffer<Size>& operator << (LocationVector & val)
	{
		// espire: I would've done this as one memcpy.. but we don't know how the struct alignment is gonna come out :/
		Write<float>(val.x);
		Write<float>(val.y);
		Write<float>(val.z);
		return *this;
	}

	StackBuffer<Size>& operator >> (LocationVector & dst)
	{
		dst.x = Read<float>();
		dst.y = Read<float>();
		dst.z = Read<float>();
		return *this;
	}

	/** Clears the buffer
	 */
	void Clear() { m_writePos = m_readPos = 0; }

	/** Gets the write position
	 * @return buffer size
	 */
	uint32 GetSize() { return m_writePos; }
};

#endif

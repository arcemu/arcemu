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

#ifndef _PREALLOCATED_QUEUE_H
#define _PREALLOCATED_QUEUE_H

template<typename Type>
class PreallocatedQueue
{
public:
	PreallocatedQueue(uint32 size, uint32 realloc_size = 100)
	{
		// create buffer
		Buffer = (uint8*)malloc(size * sizeof(Type));
		m_readpos = m_writepos = 0;
		m_buffersize = size * sizeof(Type);
		type_size = sizeof(Type);
		m_reallocsize = realloc_size;
	}

	~PreallocatedQueue()
	{
		free(Buffer);
	}

	void push_back(Type ptr)
	{
		if((type_size + m_writepos) > m_buffersize)
			reallocate(m_buffersize + (m_reallocsize * type_size));

		memcpy(&Buffer[m_writepos], &ptr, type_size);
		m_writepos += type_size;
	}

	Type pop_front()
	{
		assert((m_readpos + type_size) <= m_writepos);
		Type returner;
		memcpy(&returner, &Buffer[m_readpos], type_size);
		m_readpos += type_size;

		// clear buffer completely if we're at the end of the buffer
		if(m_readpos == m_writepos)
			clear();

		return returner;
	}

	void clear()
	{
		m_readpos = m_writepos = 0;
	}

	bool empty()
	{
		if(m_readpos == m_writepos)
			return true;
		return false;
	}

	void reallocate(uint32 size)
	{
		Buffer = (uint8*)realloc(Buffer, size);
		assert(Buffer);
		m_buffersize = size;
	}

protected:

	uint8 * Buffer;

	uint32 m_readpos;
	uint32 m_writepos;
	uint32 m_writesize;
	uint32 m_buffersize;
	uint32 type_size;
	uint32 m_reallocsize;
};

#endif
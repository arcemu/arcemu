/*
* Ascent MMORPG Server
* Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
*
* Circular Buffer Class
* Based on the Bip Buffer concept, from http://www.codeproject.com/KB/IP/bipbuffer.aspx
* Implementation Copyright (c) 2008 Burlex
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

#ifndef _NETLIB_CIRCULARBUFFER_H
#define _NETLIB_CIRCUALRBUFFER_H

class CircularBuffer
{
	// allocated whole block pointer
	uint8 * m_buffer;
	uint8 * m_bufferEnd;

	// region A pointer, and size
	uint8 * m_regionAPointer;
	size_t m_regionASize;

	// region size
	uint8 * m_regionBPointer;
	size_t m_regionBSize;

	// pointer magic!
	inline size_t GetAFreeSpace() { return (m_bufferEnd - m_regionAPointer - m_regionASize); }
	inline size_t GetSpaceBeforeA() { return (m_regionAPointer - m_buffer); }
	inline size_t GetSpaceAfterA() { return (m_bufferEnd - m_regionAPointer - m_regionASize); }
	inline size_t GetBFreeSpace() { if(m_regionBPointer == NULL) { return 0; } return (m_regionAPointer - m_regionBPointer - m_regionBSize); }

public:

	/** Constructor
	*/
	CircularBuffer();

	/** Destructor
	*/
	~CircularBuffer();

	/** Read bytes from the buffer
	* @param destination pointer to destination where bytes will be written
	* @param bytes number of bytes to read
	* @return true if there was enough data, false otherwise
	*/
	bool Read(void * destination, size_t bytes);
	void AllocateB();

	/** Write bytes to the buffer
	* @param data pointer to the data to be written
	* @param bytes number of bytes to be written
	* @return true if was successful, otherwise false
	*/
	bool Write(const void * data, size_t bytes);

	/** Returns the number of available bytes left.
	*/
	size_t GetSpace();

	/** Returns the number of bytes currently stored in the buffer.
	*/
	size_t GetSize();

	/** Returns the number of contiguous bytes (that can be pushed out in one operation)
	*/
	size_t GetContiguiousBytes();

	/** Removes len bytes from the front of the buffer
	* @param len the number of bytes to "cut"
	*/
	void Remove(size_t len);

	/** Returns a pointer at the "end" of the buffer, where new data can be written
	*/
	void * GetBuffer();

	/** Allocate the buffer with room for size bytes
	* @param size the number of bytes to allocate
	*/
	void Allocate(size_t size);

	/** Increments the "writen" pointer forward len bytes
	* @param len number of bytes to step
	*/
	void IncrementWritten(size_t len);			// known as "commit"

	/** Returns a pointer at the "beginning" of the buffer, where data can be pulled from
	*/
	void * GetBufferStart();
};

#endif		// _NETLIB_CIRCULARBUFFER_H


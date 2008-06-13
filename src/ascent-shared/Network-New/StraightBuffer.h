/****************************************************************************
 *
 * Multiplatform High-Performance Async Network Library
 * Straight Buffer Implemented Class
 * Copyright (c) 2007 Burlex
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _NETLIB_STRAIGHTBUFFER_H
#define _NETLIB_STRAIGHTBUFFER_H

class StraightBuffer : public BaseBuffer
{
	/** Pointer to buffer
	 */
	char * m_buffer;

	/** How many bytes were allocated
	 */
	size_t space;

	/** How many bytes are currently stored
	 */
	size_t written;

public:

	/** Constructor
	 */
	StraightBuffer()
	{
        space = written = 0;
		m_buffer = 0;
	}

	/** Destructor
	 */
	~StraightBuffer()
	{
		free(m_buffer);
	}

	/** Read bytes from the buffer
	 * @param destination pointer to destination where bytes will be written
	 * @param bytes number of bytes to read
	 * @return true if there was enough data, false otherwise
	 */
	bool Read(void * destination, size_t bytes)
	{
		if(written < bytes)
		{
			/* copy what we have */
			memcpy(destination, m_buffer, bytes);
			bytes = written = 0;
			return false;
		}
		else
		{
			/* read everything */
			memcpy(destination, m_buffer, bytes);
			written -= bytes;

			/* move the buffer "backwards" */
			memcpy(&m_buffer[0], &m_buffer[bytes], written);
			return true;
		}
	}

	/** Write bytes to the buffer
	 * @param data pointer to the data to be written
	 * @param bytes number of bytes to be written
	 * @return true if was successful, otherwise false
	 */
	bool Write(const void * data, size_t bytes)
	{
		if((written + bytes) > space)
		{
			/* write what we can */
			bytes = space - written;
			if(bytes)
				memcpy(&m_buffer[written], data, bytes);
			written = space;
			return false;
		}
		else
		{
			/* write everything */
			memcpy(&m_buffer[written], data, bytes);
			written += bytes;
			return true;
		}
	}

	/** Returns the number of available bytes left.
	 */
	size_t GetSpace()
	{
		return (space - written);
	}

	/** Returns the number of bytes currently stored in the buffer.
	 */
	size_t GetSize()
	{
		return written;
	}

	/** Removes len bytes from the front of the buffer
	 * @param len the number of bytes to "cut"
	 */
	void Remove(size_t len)
	{
		ASSERT(written >= len);
		written -= len;
		if(written)
			memcpy(&m_buffer[0], &m_buffer[len], written);
	}

	/** Returns a pointer at the "end" of the buffer, where new data can be written
	 */
	void * GetBuffer()
	{
		return &m_buffer[written];
	}

	/** Allocate the buffer with room for size bytes
	 * @param size the number of bytes to allocate
	 */
	void Allocate(size_t size)
	{
		m_buffer = (char*)malloc(size);
		space = size;
	}

	/** Increments the "writen" pointer forward len bytes
	 * @param len number of bytes to step
	 */
	void IncrementWritten(size_t len)
	{
		written += len;
		ASSERT(space >= written);
	}

	/** Returns a pointer at the "beginning" of the buffer, where data can be pulled from
	 */
	void * GetBufferOffset()
	{
		return m_buffer;
	}
};

#endif		// _NETLIB_STRAIGHTBUFFER_H

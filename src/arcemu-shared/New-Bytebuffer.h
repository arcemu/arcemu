/****************************************************************************
 *
 * ByteBuffer Class
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "Common.h"
#include "WoWGuid.h"
#include "LocationVector.h"

class SERVER_DECL ByteBuffer
{
#define DEFAULT_SIZE 0x1000
#define DEFAULT_INCREASE_SIZE 200

	uint8 * m_buffer;
	size_t m_readPos;
	size_t m_writePos;
	uint32 m_buffersize;

public:
	/** Allocates/reallocates buffer with specified size.
	 */
	void reserve(size_t res)
	{
		if(m_buffer)
			m_buffer = (uint8*)realloc(m_buffer, res);
		else
			m_buffer = (uint8*)malloc(res);

		m_buffersize = res;
	}

	/** Creates a bytebuffer with the default size
	 */
	ByteBuffer() : m_readPos(0), m_writePos(0), m_buffer(0)
	{
		reserve(DEFAULT_SIZE);
	}

	/** Creates a bytebuffer with the specified size
	 */
	ByteBuffer(size_t res) : m_readPos(0), m_writePos(0), m_buffer(0)
	{
        reserve(DEFAULT_SIZE);
	}

	/** Frees the allocated buffer
	 */
	~ByteBuffer()
	{
		free(m_buffer);
	}

	/** Resets read/write indexes
	 */
	inline void clear()
	{
		m_readPos = m_writePos = 0;
	}

	/** Sets write position
	 */
	inline void resize(size_t size)
	{
		m_writePos = size;
	}

	/** Returns the buffer pointer
	 */
	inline const uint8 * contents()
	{
		return m_buffer;
	}

	/** Gets the buffer size.
	 */
	uint32 GetBufferSize() { return m_writePos; }

	/** Reads sizeof(T) bytes from the buffer
	 * @return the bytes read
	 */
	template<typename T>
		T Read()
	{
		if(m_readPos + sizeof(T) > m_writePos)
			return (T)0;
		T ret = *(T*)&m_buffer[m_readPos];
		m_readPos += sizeof(T);
		return ret;
	}

	/** Reads x bytes from the buffer
	 */
	void read(uint8 * buffer, size_t len)
	{
		if(m_readPos + len > m_writePos)
			len = (m_writePos - m_readPos);

		memcpy(buffer, &m_buffer[m_readPos], len);
		m_readPos += len;
	}

	/** Writes sizeof(T) bytes to the buffer, while checking for overflows.
	 * @param T data The data to be written
	 */
	template<typename T>
		void Write(const T data)
	{
		if(m_writePos + sizeof(T) > m_buffersize)
			reserve(m_buffersize + DEFAULT_INCREASE_SIZE);

		*(T*)&m_buffer[m_writePos] = data;
		m_writePos += sizeof(T);
	}

	/** writes x bytes to the buffer, while checking for overflows
	 * @param ptr the data to be written
	 * @param size byte count
	*/
	void Write(const uint8 * data, size_t size)
	{
		if(m_writePos + size > m_buffersize)
			reserve(m_buffersize + DEFAULT_INCREASE_SIZE);

		memcpy(&m_buffer[m_writePos], data, size);
		m_writePos += size;
	}

	/** Ensures the buffer is big enough to fit the specified number of bytes.
	 * @param bytes number of bytes to fit
	 */
	inline void EnsureBufferSize(uint32 Bytes)
	{
		if(m_writePos + Bytes > m_buffersize)
			reserve(m_buffersize + DEFAULT_INCREASE_SIZE);
	}

	/** These are the default read/write operators.
	 */
#define DEFINE_BUFFER_READ_OPERATOR(type) void operator >> (type& dest) { dest = Read<type>(); }
#define DEFINE_BUFFER_WRITE_OPERATOR(type) void operator << (const type src) { Write<type>(src); }

	/** Fast read/write operators without using the templated read/write functions.
	 */
#define DEFINE_FAST_READ_OPERATOR(type, size) ByteBuffer& operator >> (type& dest) { if(m_readPos + size > m_writePos) { dest = (type)0; return *this; } else { dest = *(type*)&m_buffer[m_readPos]; m_readPos += size; return *this; } }
#define DEFINE_FAST_WRITE_OPERATOR(type, size) ByteBuffer& operator << (const type src) { if(m_writePos + size > m_buffersize) { reserve(m_buffersize + DEFAULT_INCREASE_SIZE); } *(type*)&m_buffer[m_writePos] = src; m_writePos += size; return *this; }

	/** Integer/float r/w operators
	*/
	DEFINE_FAST_READ_OPERATOR(uint64, 8);
	DEFINE_FAST_READ_OPERATOR(uint32, 4);
	DEFINE_FAST_READ_OPERATOR(uint16, 2);
	DEFINE_FAST_READ_OPERATOR(uint8, 1);
	DEFINE_FAST_READ_OPERATOR(int64, 8);
	DEFINE_FAST_READ_OPERATOR(int32, 4);
	DEFINE_FAST_READ_OPERATOR(int16, 2);
	DEFINE_FAST_READ_OPERATOR(int8, 1);
	DEFINE_FAST_READ_OPERATOR(float, 4);
	DEFINE_FAST_READ_OPERATOR(double, 8);

	DEFINE_FAST_WRITE_OPERATOR(uint64, 8);
	DEFINE_FAST_WRITE_OPERATOR(uint32, 4);
	DEFINE_FAST_WRITE_OPERATOR(uint16, 2);
	DEFINE_FAST_WRITE_OPERATOR(uint8, 1);
	DEFINE_FAST_WRITE_OPERATOR(int64, 8);
	DEFINE_FAST_WRITE_OPERATOR(int32, 4);
	DEFINE_FAST_WRITE_OPERATOR(int16, 2);
	DEFINE_FAST_WRITE_OPERATOR(int8, 1);
	DEFINE_FAST_WRITE_OPERATOR(float, 4);
	DEFINE_FAST_WRITE_OPERATOR(double, 8);

	/** boolean (1-byte) read/write operators
	 */
	DEFINE_FAST_WRITE_OPERATOR(bool, 1);
	ByteBuffer& operator >> (bool & dst) { dst = (Read<char>() > 0 ? true : false); return *this; }

	/** string (null-terminated) operators
	 */
	ByteBuffer& operator << (const std::string & value) { EnsureBufferSize(value.length() + 1); memcpy(&m_buffer[m_writePos], value.c_str(), value.length()+1); m_writePos += (value.length() + 1); return *this; }
	ByteBuffer& operator >> (std::string & dest)
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
	ByteBuffer& operator << (const WoWGuid & value)
	{
		EnsureBufferSize(value.GetNewGuidLen() + 1);
		Write<uint8>(value.GetNewGuidMask());
		memcpy(&m_buffer[m_writePos], value.GetNewGuid(), value.GetNewGuidLen());
		m_writePos += value.GetNewGuidLen();
		return *this;
	}

	ByteBuffer& operator >> (WoWGuid & value)
	{
		uint8 mask = Read<uint8>();
		value.Init(mask);
		for(uint32 i = 0; i < BitCount8(mask); ++i)
			value.AppendField(Read<uint8>());
		return *this;
	}

	/** LocationVector read/write operators
	 */
	ByteBuffer& operator << (const LocationVector & val)
	{
		// burlex: I would've done this as one memcpy.. but we don't know how the struct alignment is gonna come out :/
		Write<float>(val.x);
		Write<float>(val.y);
		Write<float>(val.z);
		return *this;
	}

	ByteBuffer& operator >> (LocationVector & dst)
	{
		dst.x = Read<float>();
		dst.y = Read<float>();
		dst.z = Read<float>();
		return *this;
	}

	/** Gets the write position
	 * @return buffer size
	 */
	inline size_t size() { return m_writePos; }

	/** read/write position setting/getting
	 */
	inline size_t rpos() { return m_readPos; }
	inline size_t wpos() { return m_writePos; }
	inline void rpos(size_t p) { ASSERT(p <= m_writePos); m_readPos = p; }
	inline void wpos(size_t p) { ASSERT(p <= m_buffersize); m_writePos = p; }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
	b << (uint32)v.size();
	for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); i++) {
		b << *i;
	}
	return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while(vsize--) {
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
	b << (uint32)v.size();
	for (typename std::list<T>::iterator i = v.begin(); i != v.end(); i++) {
		b << *i;
	}
	return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while(vsize--) {
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename K, typename V> ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
	b << (uint32)m.size();
	for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); i++) {
		b << i->first << i->second;
	}
	return b;
}

template <typename K, typename V> ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
	uint32 msize;
	b >> msize;
	m.clear();
	while(msize--) {
		K k;
		V v;
		b >> k >> v;
		m.insert(make_pair(k, v));
	}
	return b;
}

#endif

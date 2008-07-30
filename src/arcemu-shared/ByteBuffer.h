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

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "Common.h"
#include "WoWGuid.h"
#include "LocationVector.h"

class SERVER_DECL ByteBuffer {
public:
	class error {
	};

	const static size_t DEFAULT_SIZE = 0x1000;

	ByteBuffer(): _rpos(0), _wpos(0) {
		_storage.reserve(DEFAULT_SIZE);
	}
	ByteBuffer(size_t res): _rpos(0), _wpos(0) {
		_storage.reserve(res);
	}
	ByteBuffer(const ByteBuffer &buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }
	virtual ~ByteBuffer() {}

	void clear() {
		_storage.clear();
		_rpos = _wpos = 0;
	}

	//template <typename T> void insert(size_t pos, T value) {
	//  insert(pos, (uint8 *)&value, sizeof(value));
	//}
	template <typename T> void append(T value) {
		append((uint8 *)&value, sizeof(value));
	}
	template <typename T> void put(size_t pos,T value) {
		put(pos,(uint8 *)&value,sizeof(value));
	}

	// stream like operators for storing data
	ByteBuffer &operator<<(bool value) {
		append<char>((char)value);
		return *this;
	}
	// unsigned
	ByteBuffer &operator<<(uint8 value) {
		append<uint8>(value);
		return *this;
	}
	ByteBuffer &operator<<(uint16 value) {
#ifdef USING_BIG_ENDIAN
		append<uint16>(swap16(value));
#else
		append<uint16>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(uint32 value) {
#ifdef USING_BIG_ENDIAN
		append<uint32>(swap32(value));
#else
		append<uint32>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(uint64 value) {
#ifdef USING_BIG_ENDIAN
		append<uint64>(swap64(value));
#else
		append<uint64>(value);
#endif
		return *this;
	}
	// signed as in 2e complement
	ByteBuffer &operator<<(int8 value) {
		append<int8>(value);
		return *this;
	}
	ByteBuffer &operator<<(int16 value) {
#ifdef USING_BIG_ENDIAN
		append<int16>(swap16(value));
#else
		append<int16>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(int32 value) {
#ifdef USING_BIG_ENDIAN
		append<int32>(swap32(value));
#else
		append<int32>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(int64 value) {
#ifdef USING_BIG_ENDIAN
		append<int64>(swap64(value));
#else
		append<int64>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(float value) {
#ifdef USING_BIG_ENDIAN
		append<float>(swapfloat(value));
#else
		append<float>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(double value) {
#ifdef USING_BIG_ENDIAN
		append<double>(swapdouble(value));
#else
		append<double>(value);
#endif
		return *this;
	}
	ByteBuffer &operator<<(const std::string &value) {
		append((uint8 *)value.c_str(), value.length());
		append((uint8)0);
		return *this;
	}
	ByteBuffer &operator<<(const char *str) {
		append((uint8 *)str, strlen(str));
		append((uint8)0);
		return *this;
	}
	ByteBuffer &operator<<(const WoWGuid &value) {
		append<uint8>(value.GetNewGuidMask());
		append((uint8 *)value.GetNewGuid(), value.GetNewGuidLen());
		return *this;
	}

	// stream like operators for reading data
	ByteBuffer &operator>>(bool &value) {
		value = read<char>() > 0 ? true : false;
		return *this;
	}
	//unsigned
	ByteBuffer &operator>>(uint8 &value) {
		value = read<uint8>();
		return *this;
	}
	ByteBuffer &operator>>(uint16 &value) {
#ifdef USING_BIG_ENDIAN
		value = swap16(read<uint16>());
#else
		value = read<uint16>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(uint32 &value) {
#ifdef USING_BIG_ENDIAN
		value = swap32(read<uint32>());
#else
		value = read<uint32>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(uint64 &value) {
#ifdef USING_BIG_ENDIAN
		value = swap64(read<uint64>());
#else
		value = read<uint64>();
#endif
		return *this;
	}
	//signed as in 2e complement
	ByteBuffer &operator>>(int8 &value) {
		value = read<int8>();
		return *this;
	}
	ByteBuffer &operator>>(int16 &value) {
#ifdef USING_BIG_ENDIAN
		value = swap16(read<int16>());
#else
		value = read<int16>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(int32 &value) {
#ifdef USING_BIG_ENDIAN
		value = swap32(read<int32>());
#else
		value = read<int32>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(int64 &value) {
#ifdef USING_BIG_ENDIAN
		value = swap64(read<uint64>());
#else
		value = read<int64>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(float &value) {
#ifdef USING_BIG_ENDIAN
		value = swapfloat(read<float>());
#else
		value = read<float>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(double &value) {
#ifdef USING_BIG_ENDIAN
		value = swapdouble(read<double>());
#else
		value = read<double>();
#endif
		return *this;
	}
	ByteBuffer &operator>>(std::string& value) {
		value.clear();
		while (true) {
			char c=read<char>();
			if (c==0)
				break;
			value+=c;
		}
		return *this;
	}
	//! Only does X,Y,Z!
	ByteBuffer & operator << (const LocationVector & vec)
	{
#ifdef USING_BIG_ENDIAN
		append<float>(swapfloat(vec.x));
		append<float>(swapfloat(vec.y));
		append<float>(swapfloat(vec.z));
#else
		append<float>(vec.x);
		append<float>(vec.y);
		append<float>(vec.z);
#endif
		return *this;
	}

	//! Only does X,Y,Z!
	ByteBuffer & operator >> (LocationVector & vec)
	{
#ifdef USING_BIG_ENDIAN
		vec.x = swapfloat(read<float>());
		vec.y = swapfloat(read<float>());
		vec.z = swapfloat(read<float>());
#else
		vec.x = read<float>();
		vec.y = read<float>();
		vec.z = read<float>();
#endif
		return * this;
	}

	ByteBuffer &operator>>(WoWGuid &value) {
		uint8 field, mask = read<uint8>();
		value.Init((uint8)mask);
		for(int i = 0; i < BitCount8(mask); i++)
		{
			field = read<uint8>();
			value.AppendField(field);
		}
		return *this;
	}

	uint8 operator[](size_t pos) {
		return read<uint8>(pos);
	}

	size_t rpos() {
		return _rpos;
	};

	size_t rpos(size_t rpos) {
		_rpos = rpos;
		return _rpos;
	};

	size_t wpos() {
		return _wpos;
	}

	size_t wpos(size_t wpos) {
		_wpos = wpos;
		return _wpos;
	}

	template <typename T> T read() {
		T r=read<T>(_rpos);
		_rpos += sizeof(T);
		return r;
	};
	template <typename T> T read(size_t pos) const {
		//ASSERT(pos + sizeof(T) <= size());
		if(pos + sizeof(T) > size())
		{
			return (T)0;
		} else {
			return *((T*)&_storage[pos]);
		}
	}

	void read(uint8 *dest, size_t len) {
		if (_rpos + len <= size()) {
			memcpy(dest, &_storage[_rpos], len);
		} else {
			//throw error();
			memset(dest, 0, len);
		}
		_rpos += len;
	}

	const uint8 *contents() const { return &_storage[0]; };

	ARCEMU_INLINE size_t size() const { return _storage.size(); };
	// one should never use resize probably
	void resize(size_t newsize) {
		_storage.resize(newsize);
		_rpos = 0;
		_wpos = size();
	};
	void reserve(size_t ressize) {
		if (ressize > size()) _storage.reserve(ressize);
	};

		// appending to the end of buffer
	void append(const std::string& str) {
		append((uint8 *)str.c_str(),str.size() + 1);
	}
	void append(const char *src, size_t cnt) {
		return append((const uint8 *)src, cnt);
	}
	void append(const uint8 *src, size_t cnt) {
		if (!cnt) return;

		// noone should even need uint8buffer longer than 10mb
		// if you DO need, think about it
		// then think some more
		// then use something else
		// -- qz
		ASSERT(size() < 10000000);

		if (_storage.size() < _wpos + cnt)
			_storage.resize(_wpos + cnt);
		memcpy(&_storage[_wpos], src, cnt);
		_wpos += cnt;
	}
	void append(const ByteBuffer& buffer) {
		if(buffer.size() > 0) append(buffer.contents(),buffer.size());
	}

	void appendPackGUID(uint64 guid)
        {
            size_t mask_position = wpos();
            *this << uint8(0);
            for(uint8 i = 0; i < 8; i++)
            {
                if(guid & 0xFF)
                {
                    _storage[mask_position] |= (1<<i);
                    *this << ((uint8)(guid & 0xFF));
                }

                guid >>= 8;
            }
        }

	void put(size_t pos, const uint8 *src, size_t cnt) {
		ASSERT(pos + cnt <= size());
		memcpy(&_storage[pos], src, cnt);
	}
	//void insert(size_t pos, const uint8 *src, size_t cnt) {
	//  std::copy(src, src + cnt, inserter(_storage, _storage.begin() + pos));
	//}

	void hexlike()
		{
			uint32 j = 1, k = 1;
			printf("STORAGE_SIZE: %u\n", (unsigned int)size() );
			for(uint32 i = 0; i < size(); i++)
			{
				if ((i == (j*8)) && ((i != (k*16))))
				{
					if (read<uint8>(i) <= 0x0F)
					{
						printf("| 0%X ", read<uint8>(i) );
					}
					else
					{
						printf("| %X ", read<uint8>(i) );
					}

					j++;
				}
				else if (i == (k*16))
				{
					rpos(rpos()-16);	// move read pointer 16 places back
					printf(" | ");	  // write split char

					for (int x = 0; x < 16; x++)
					{
						printf("%c", read<uint8>(i-16 + x) );
					}

					if (read<uint8>(i) <= 0x0F)
					{
						printf("\n0%X ", read<uint8>(i) );
					}
					else
					{
						printf("\n%X ", read<uint8>(i) );
					}

					k++;
					j++;
				}
				else
				{
					if (read<uint8>(i) <= 0x0F)
					{
						printf("0%X ", read<uint8>(i) );
					}
					else
					{
						printf("%X ", read<uint8>(i) );
					}
				}
			}
			printf("\n");
		}

	ARCEMU_INLINE void reverse()
	{
		std::reverse(_storage.begin(), _storage.end());
	}

protected:
	// read and write positions
	size_t _rpos, _wpos;
	std::vector<uint8> _storage;
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

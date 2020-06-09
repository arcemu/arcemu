/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#ifndef BYTEBUFFER_EXTENSION_H_
#define BYTEBUFFER_EXTENSION_H_

#include "Container/ByteBuffer.h"
#include "WoWGuid.h"
#include "LocationVector.h"

ARCEMU_INLINE ByteBuffer & operator<<(ByteBuffer & b, const WoWGuid & value)
{
	b.append<uint8>(value.GetNewGuidMask());
	b.append((uint8*)value.GetNewGuid(), value.GetNewGuidLen());
	return b;
}

ARCEMU_INLINE ByteBuffer & operator>>(ByteBuffer & b, WoWGuid & value)
{
	uint8 field, mask = b.read<uint8>();
	value.Init((uint8)mask);
	for(int i = 0; i < BitCount8(mask); i++)
	{
		field = b.read<uint8>();
		value.AppendField(field);
	}
	return b;
}

//! Only does X,Y,Z!
ARCEMU_INLINE ByteBuffer & operator << (ByteBuffer & b, const LocationVector & vec)
{

	b.append<float>(vec.x);
	b.append<float>(vec.y);
	b.append<float>(vec.z);

	return b;
}

//! Only does X,Y,Z!
ARCEMU_INLINE ByteBuffer & operator >> (ByteBuffer & b, LocationVector & vec)
{

	vec.x = b.read<float>();
	vec.y = b.read<float>();
	vec.z = b.read<float>();

	return b;
}

#endif

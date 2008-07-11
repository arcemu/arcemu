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

#ifndef _WOWGUID_H
#define _WOWGUID_H

#include "Common.h"

#define BitCount1(x) ((x) & 1)
#define BitCount2(x) ( BitCount1(x) + BitCount1((x)>>1) )
#define BitCount4(x) ( BitCount2(x) + BitCount2((x)>>2) )
#define BitCount8(x) ( BitCount4(x) + BitCount4((x)>>4) )

class SERVER_DECL WoWGuid {
public:
	
	WoWGuid() 
	{
		Clear();
	}

	WoWGuid(uint64 guid) 
	{
		Clear();
		Init((uint64)guid);
	}

   WoWGuid(uint8 mask) 
	{
		Clear();
		Init((uint8)mask);
	}

	WoWGuid(uint8 mask, uint8 *fields) 
	{
		Clear();
		Init(mask, fields);
	}

	~WoWGuid() 
	{
		Clear();
	}

	void Clear()
	{
		oldguid = 0;
		guidmask = 0;

		*((uint32*)guidfields)=0;
		*((uint32*)&guidfields[4])=0;
		compiled = false;
		fieldcount = 0;
	}

	void Init(uint64 guid)
	{
		Clear();

		oldguid = guid;

		_CompileByOld();
	}

	void Init(uint8 mask)
	{
		Clear();

		guidmask = mask;

		if (!guidmask)
			_CompileByNew();
	}

	void Init(uint8 mask, uint8 *fields)
	{
		Clear();

		guidmask = mask;

		if (!BitCount8(guidmask))
			return;

		for(int i = 0; i < BitCount8(guidmask); i++)	 
			guidfields[i] = (fields[i]);
		
		fieldcount = BitCount8(guidmask);

		_CompileByNew();
	}

	const uint64 GetOldGuid() const { return oldguid; }
	const uint8* GetNewGuid() const { return guidfields; }
	const uint8 GetNewGuidLen() const { return BitCount8(guidmask); }
	const uint8 GetNewGuidMask() const { return guidmask; }

	void AppendField(uint8 field)
	{
		ASSERT(!compiled);
		ASSERT(fieldcount < BitCount8(guidmask));

		guidfields[fieldcount++] = field;
   
		if (fieldcount == BitCount8(guidmask))
			_CompileByNew();
	}

private:

	uint64 oldguid;
	uint8 guidmask;

	uint8 guidfields[8];

	uint8 fieldcount;
	bool compiled;

	void _CompileByOld()
	{
#ifdef USING_BIG_ENDIAN
		uint64 t = swap64(oldguid);
#endif
		ASSERT(!compiled);

		fieldcount = 0;

		for(uint32 x=0;x<8;x++)
		{
#ifdef USING_BIG_ENDIAN
			uint8 p = ((uint8*)&t)[x];
#else
			uint8 p =((uint8*)&oldguid)[x];
#endif
			if(p)
			{
				guidfields[fieldcount++]=p;
				guidmask|=1<<x;
			}
		}
		compiled = true;
	}
	void _CompileByNew()
	{
		ASSERT(!compiled || fieldcount == BitCount8(guidmask));

		int j = 0;

		if (guidmask & 0x01) //1
			{
				oldguid |= ((uint64)guidfields[j]);
				j++;
			}
		if (guidmask & 0x02) //2
			{
				oldguid |= ((uint64)guidfields[j] << 8);
				j++;
			}
		if (guidmask & 0x04)//4
			{
				oldguid |= ((uint64)guidfields[j] << 16);
				j++;
			}
		if (guidmask & 0x08) //8
			{
				oldguid |= ((uint64)guidfields[j] << 24);
				j++;
			}
		if (guidmask & 0x10)//16
			{
				oldguid |= ((uint64)guidfields[j] << 32);
				j++;
			}
		if (guidmask & 0x20)//32
			{
				oldguid |= ((uint64)guidfields[j] << 40);
				j++;
			}
		if (guidmask & 0x40)//64
			{
				oldguid |= ((uint64)guidfields[j] << 48);
				j++;
			}
		if (guidmask & 0x80) //128
			{
				oldguid |= ((uint64)guidfields[j] << 56);
				j++;
			}
		compiled = true;
	}
};

#endif

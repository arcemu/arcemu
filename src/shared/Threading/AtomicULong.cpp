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

#include "System/Platform.h"

namespace Arcemu
{
	namespace Threading
	{

		unsigned long AtomicULong::SetVal(unsigned long NewValue)
		{
			unsigned long ret = 0;
#ifdef WIN32
			ret = InterlockedExchange(reinterpret_cast< volatile LONG* >(&Value), LONG(NewValue));
#else
#ifdef __GNUC__
			ret = __sync_val_compare_and_swap(&Value, Value, NewValue);
#else
#error Your platform (architecture and compiler) is NOT supported. Arcemu requires little endian architecture, and at least GCC 4.1
#endif
#endif
			return ret;

		}

	}
}
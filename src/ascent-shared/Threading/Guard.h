/*
* Ascent MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef _THREADING_GUARD_H
#define _THREADING_GUARD_H

#include "Mutex.h"

/************************************************************************/
/* Guard class, unlocks mutex on destroy								*/
/************************************************************************/
// crossplatform :)

class SERVER_DECL Guard
{
public:
	Guard(Mutex& mutex) : target(mutex)
	{
		target.Acquire();
	}

	~Guard()
	{
		target.Release();
	}

	Guard& operator=(Guard& src)
	{
		this->target = src.target;
		return *this;
	}

protected:
	Mutex& target;
};

#endif


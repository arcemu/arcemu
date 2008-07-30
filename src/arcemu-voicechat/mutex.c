/*
 * OpenAscent MMORPG Server
 * Copyright (C) 2008 <http://www.openascent.com/>
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

#include "common.h"
#include "mutex.h"

#ifndef WIN32

/* this is done slightly differently on bsd-variants */
#if defined(__FreeBSD__) ||  defined(__APPLE_CC__) || defined(__OpenBSD__)
#define recursive_mutex_flag PTHREAD_MUTEX_RECURSIVE
#else
#define recursive_mutex_flag PTHREAD_MUTEX_RECURSIVE_NP
#endif

static int attr_init = 0;
static pthread_mutexattr_t attr;

void mutex_initialize(mutex* mut)
{
	if(!attr_init)
	{
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, recursive_mutex_flag);
		attr_init= 1;
	}
}

#endif


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

#ifndef __MUTEX_H
#define __MUTEX_H

#include "common.h"

#ifdef WIN32

typedef CRITICAL_SECTION mutex;
static void mutex_lock(mutex* mut) { EnterCriticalSection(mut); }
static void mutex_unlock(mutex* mut) { LeaveCriticalSection(mut); }
static void mutex_initialize(mutex* mut) { InitializeCriticalSection(mut); }
static void mutex_free(mutex* mut) { DeleteCriticalSection(mut); }

#else

#include <pthread.h>
typedef pthread_mutex_t mutex;
static void mutex_lock(mutex* mut) { pthread_mutex_lock(mut); }
static void mutex_unlock(mutex* mut) { pthread_mutex_unlock(mut); }
void mutex_initialize(mutex* mut);
static void mutex_free(mutex* mut) { pthread_mutex_destroy(mut); }

#endif

#endif

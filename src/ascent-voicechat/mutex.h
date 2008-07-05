/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
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

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

#include "StdAfx.h"

initialiseSingleton( EventMgr );

TimedEvent * TimedEvent::Allocate(void* object, CallbackBase* callback, uint32 flags, time_t time, uint32 repeat)
{
	return new TimedEvent(object, callback, flags, time, repeat, 0);
}

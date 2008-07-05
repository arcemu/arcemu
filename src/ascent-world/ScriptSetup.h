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

#ifndef SCRIPTSETUP_H
#define SCRIPTSETUP_H

#ifdef SCRIPTLIB

#include <svn_revision.h>
#ifndef SKIP_ALLOCATOR_SHARING
#include "CoreMemoryAllocator.cpp"
#endif

#define MAKE_SCRIPT_VERSION(major, minor) (uint32)(((uint16)major << 16) | ((uint16)minor))

extern "C" SCRIPT_DECL uint32 _exp_get_version()
{
	return MAKE_SCRIPT_VERSION(BUILD_REVISION / 1000, BUILD_REVISION % 1000);
}

#endif

#endif


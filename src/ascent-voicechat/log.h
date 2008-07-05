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

#ifndef __LOG_H
#define __LOG_H

// logging functions
// level 0 = notice/error
// level 1 = information
// level 2 = debug

enum LOGLEVEL
{
#ifdef WIN32
#undef ERROR
#endif
	ERROR		= 0,
	NOTICE		= 0,
	WARNING		= 0,
	INFO		= 1,
	DEBUG		= 2,
};

void log_setloglevel(int new_level);
void log_setlogfile(const char * filename);
void log_write(int level, const char * format, ...);
void log_open();
void log_close();

#endif

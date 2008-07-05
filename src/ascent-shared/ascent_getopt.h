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

#ifndef _ASCENT_GETOPT_H
#define _ASCENT_GETOPT_H

/* getopt() wrapper */
#define ascent_no_argument            0
#define ascent_required_argument      1
#define ascent_optional_argument      2
struct ascent_option
{
	const char *name;
	int has_arg;
	int *flag;
	int val;
};
extern char ascent_optarg[514];
int ascent_getopt_long_only (int ___argc, char *const *___argv, const char *__shortopts, const struct ascent_option *__longopts, int *__longind);

#endif

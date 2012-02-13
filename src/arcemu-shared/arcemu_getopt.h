/*
 * ArcEmu, a MMORPG Server
 * 
 * Copyright (C) 2008-2012 ArcEmu Developers <http://arcemu.org/>
 *
 * See former copyright holders in CREDITS
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _arcemu_GETOPT_H
#define _arcemu_GETOPT_H

/* getopt() wrapper */
#define arcemu_no_argument            0
#define arcemu_required_argument      1
#define arcemu_optional_argument      2
struct arcemu_option
{
	const char* name;
	int has_arg;
	int* flag;
	int val;
};
extern char arcemu_optarg[514];
int arcemu_getopt_long_only(int ___argc, char* const* ___argv, const char* __shortopts, const struct arcemu_option* __longopts, int* __longind);

#endif

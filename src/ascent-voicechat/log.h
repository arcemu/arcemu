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

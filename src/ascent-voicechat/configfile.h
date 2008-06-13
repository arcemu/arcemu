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

#ifndef __CONFIGFILE_H
#define __CONFIGFILE_H

#include "common.h"

typedef struct
{
	int tcp_listen_port;
	char * tcp_listen_host;

	int udp_listen_port;
	char * udp_listen_host;

	int log_loglevel;
	char * log_logfile;

	int daemonize;
	// max channels maybe?
} server_config;

// global accessable server config struct
extern server_config g_serverConfig;

// load/parse/reload the config file
int configfile_load_config(const char * filename);
int configfile_init();
int configfile_parsecmdoverride(int argc, char* argv[]);

#endif

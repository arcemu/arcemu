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

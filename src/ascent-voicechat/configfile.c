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

#include "configfile.h"
#include "log.h"

enum server_config_types
{
	SERVER_CONFIG_TYPE_INT			= 0,
	SERVER_CONFIG_TYPE_STRING		= 1,
};

typedef struct 
{
	const char * key;
	int type;
	void * dest;
	int done;
	int required;
} server_config_directive;

// global whee
server_config g_serverConfig;

// config parser function
int configfile_init()
{
	// initialize the defaults
	g_serverConfig.tcp_listen_host = NULL;
	g_serverConfig.tcp_listen_port = 0;
	g_serverConfig.udp_listen_host = NULL;
	g_serverConfig.udp_listen_port = 0;
	g_serverConfig.log_logfile = NULL;
	g_serverConfig.log_loglevel = 2;
	g_serverConfig.daemonize = 0;

	return 0;
}

int configfile_load_config(const char * filename)
{
	static server_config_directive config_directives[] = {
		"server.tcp-listen-port",		SERVER_CONFIG_TYPE_INT,		&g_serverConfig.tcp_listen_port,		0,		1,
		"server.tcp-listen-host",		SERVER_CONFIG_TYPE_STRING,	&g_serverConfig.tcp_listen_host,		0,		1,
		"server.udp-listen-port",		SERVER_CONFIG_TYPE_INT,		&g_serverConfig.udp_listen_port,		0,		1,
		"server.udp-listen-host",		SERVER_CONFIG_TYPE_STRING,	&g_serverConfig.udp_listen_host,		0,		1,
		"server.daemonize",				SERVER_CONFIG_TYPE_INT,		&g_serverConfig.daemonize,				0,		0,
		"log.file",						SERVER_CONFIG_TYPE_STRING,	&g_serverConfig.log_logfile,			0,		1,
		"log.level",					SERVER_CONFIG_TYPE_INT,		&g_serverConfig.log_loglevel,			0,		1,
		NULL,							0,							NULL,									0,		0,
	};

	FILE * pConfigFile;
	char line[1000];
	char * p, *q;
	char * pKey;
	char * pValue;
	int line_counter = 0;
	server_config_directive * dir;

	// try to open
	pConfigFile = fopen(filename, "r");
	if( pConfigFile == NULL )
	{
		log_write(ERROR, "FATAL: Config file %s could not be read.", filename);
		return -1;
	}

	// read the file in line by lien
	while( fgets(line, 1000, pConfigFile) )
	{
		++line_counter;

		if( line[0] == '#' || ( line[0] == '/' && line[1] == '/' ) )
			continue;

		// try to find the config key
		p = strchr( line, '=' );
		if( p == NULL )
		{
			/*log_write(ERROR, "FATAL: Config file has a parsing error at line %u.", line_counter);
			fclose(pConfigFile);
			return -1;*/

			// non-key line
			continue;
		}

		*p = 0;

		// trim shit before and after the key
		pKey = line;
		while( !isalpha(*pKey) )
			++pKey;

		// trim shit after it
		q = pKey;
		while( *q == '.' || isalpha(*q) || *q == '-' )
			++q;

		*q = 0;

		// now we have to trim the crap after the equals
		++p;
		while( !isalpha(*p) && !isdigit(*p) && *p != '\'')
			++p;

		// trim the stuff after the value
		pValue = p;
		if( *p == '\'' )
		{
			// string, copy until we have a '
			++p;
			pValue = p;
			while( *p != '\'' )
				++p;
		}
		else
		{
			// non-string, follow normal rules
			while( *p != '\n' && *p != ' ' )
				++p;
		}

		// null it
		*p = 0;

		if( *pKey == 0 )
		{
			log_write(ERROR, "FATAL: Config file has a parsing error at line %u.", line_counter);
			fclose(pConfigFile);
			return -1;
		}

		log_write(DEBUG, "CONFIG: %s = %s", pKey, pValue );

		// now we have the key and value pair, iterate over possible options
		for( dir = config_directives; dir->key != NULL; ++dir )
		{
			if( !stricmp( dir->key, pKey ) )
			{
				if( dir->done != 0 )
				{
					log_write(ERROR, "FATAL: Key %s is defined in config file twice.", dir->key);
					fclose(pConfigFile);
					return -1;
				}

				dir->done = 1;
				switch( dir->type )
				{
				case SERVER_CONFIG_TYPE_INT:
					{
						*(int*)dir->dest = atoi(pValue);
					}break;

				case SERVER_CONFIG_TYPE_STRING:
					{
						if( *(char**)dir->dest != NULL )
							free( *(char**)dir->dest );
	
						*(char**)dir->dest = strdup( pValue );
					}break;
				}

				break;
			}
		}

		// if we didn't find a key, it's probably invalid
		if( dir->key == NULL )
		{
			log_write(ERROR, "FATAL: Config file contains invalid key (%s) at line %u", pKey, line_counter);
			fclose(pConfigFile);
			return -1;
		}
	}

	// close off the file, EOF
	fclose(pConfigFile);

	// make sure we got all the options
	for( dir = config_directives; dir->key != NULL; ++dir )
	{
		if( dir->done == 0 && dir->required )
		{
			log_write(ERROR, "FATAL: Config file does not define required directive %s.", dir->key);
			return -1;
		}
	}

	// set the new log level
	log_setloglevel(g_serverConfig.log_loglevel);
	log_setlogfile(g_serverConfig.log_logfile);

	// success
	return 0;
}

int configfile_parsecmdoverride(int argc, char* argv[])
{
	return 0;
}

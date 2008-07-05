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

#include "log.h"
#include "configfile.h"
#include "mutex.h"
#include "linkedlist.h"
#include "network.h"
#include "network_handlers.h"
#include "voice_channel.h"
#include <signal.h>

int running = 1;
void signal_handler(int s)
{
	running = 0;
}

void status_updater_thread(void* unused)
{
	float bandwidth_in = 0.0f;
	float bandwidth_out = 0.0f;
	int channels = 0;
	int servers = 0;

	while( running )
	{
		network_get_bandwidth_statistics( &bandwidth_in, &bandwidth_out );
		channels = voice_get_channel_count();
		servers = get_server_count();

		// remember these are in bytes, so convert to kb
		bandwidth_in /= 1000.0f;
		bandwidth_out /= 1000.0f;

		// dump to console
		printf("\r| %02u servers | %05u channels | in: %04.2f KB/s | out: %04.2f KB/s |",
			servers, channels, bandwidth_in, bandwidth_out );

		vc_sleep(1000);
	}
}

int main(int argc, char* argv[])
{
	configfile_init();

	if( configfile_parsecmdoverride(argc, argv) < 0 )
		return -1;

	log_open();

	// read config file
	if( configfile_load_config("ascent-voicechat.conf") < 0 )
	{
		// this needs to be cmd-line overrideable
		log_write(ERROR, "FATAL: Could not open a valid config file. Exiting.");
		return -1;
	}

	printf("Ascent Voicechat Server\n");
	printf("Starting...\n");

	if( network_init() < 0 )
	{
		log_write(ERROR, "FATAL: Network failed initialization.");
		return -1;
	}

	printf("Binding sockets...\n");
	if( voicechat_init_clientsocket() < 0 || voicechat_init_serversocket() < 0 )
	{
		log_write(ERROR, "FATAL: Could not bind sockets.");
		return -1;
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
#ifdef WIN32	
	signal(SIGBREAK, signal_handler);
#endif

	printf("I/O Loop running...\n");
	start_thread(status_updater_thread, NULL);
	while(running)
	{
		network_io_poll();
	}

	printf("Shutting down...\n");
	network_shutdown();
	log_close();

	return 0;
}


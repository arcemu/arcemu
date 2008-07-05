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

#ifndef __VOICE_CHANNEL_H
#define __VOICE_CHANNEL_H

typedef struct
{
	struct sockaddr_in client_address;
	uint8 used;
	uint8 active;
} voice_channel_member;

typedef struct  
{
	int channel_id;
	int member_count;
	int member_slots;
	voice_channel_member * members;
	void* server_owner;
} voice_channel;

enum VOICE_CHANNEL_TYPE
{
	VOICE_CHANNEL_TYPE_CHANNEL			= 0,
	VOICE_CHANNEL_TYPE_PARTY			= 2,
	VOICE_CHANNEL_TYPE_RAID				= 3,
	VOICE_CHANNEL_TYPE_COUNT			= 4,
};

void voice_channel_init();
voice_channel * voice_channel_create(int channeltype, void* server_owner);
voice_channel * voice_channel_get(int channelid);
int voice_channel_remove(int channelid);
void voice_remove_channels(void* socket_ptr);
int voice_get_channel_count();

#endif

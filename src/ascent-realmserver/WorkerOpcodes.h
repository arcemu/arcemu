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

#ifndef _WORKER_OPCODES_H
#define _WORKER_OPCODES_H

enum WorkerServerOpcodes
{
	ISMSG_AUTH_REQUEST					= 1,
	ICMSG_AUTH_REPLY					= 2,
	ISMSG_AUTH_RESULT					= 3,
	ICMSG_REGISTER_WORKER				= 4,
	ISMSG_REGISTER_RESULT				= 5,
	ISMSG_CREATE_INSTANCE				= 6,
	ISMSG_DESTROY_INSTANCE				= 7,
	ISMSG_PLAYER_LOGIN					= 8,
	ISMSG_PLAYER_INFO					= 9,
	ICMSG_PLAYER_INFO					= 10,
	ISMSG_WOW_PACKET					= 11,
	ICMSG_WOW_PACKET					= 12,
	ICMSG_WHISPER						= 13,
    ICMSG_CHAT							= 14,
	ISMSG_WHISPER						= 15,
	ISMSG_CHAT							= 16,
	ISMSG_PACKED_PLAYER_INFO			= 17,
	ICMSG_PLAYER_LOGIN_RESULT			= 18,
	ICMSG_PLAYER_LOGOUT					= 19,
	ISMSG_DESTROY_PLAYER_INFO			= 20,
	ICMSG_TELEPORT_REQUEST				= 21,
	ISMSG_TELEPORT_RESULT				= 22,
	ISMSG_PLAYER_CHANGED_SERVERS		= 23,
	ISMSG_PLAYER_CHANGE_INSTANCES		= 24,
	ISMSG_CREATE_PLAYER					= 25,
	ICMSG_PLAYER_CHANGE_SERVER_INFO		= 26,

	IMSG_NUM_TYPES,
};

#endif		// _WORKER_OPCODES_H



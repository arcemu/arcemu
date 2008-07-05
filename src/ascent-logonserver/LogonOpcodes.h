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

#ifndef __LOGONOPCODES_H
#define __LOGONOPCODES_H

enum RealmListOpcodes
{
	// Initialization of server/client connection...
	RCMSG_REGISTER_REALM						= 1,
	RSMSG_REALM_REGISTERED						= 2,

	// Upon client connect (for WS)
	RCMSG_REQUEST_SESSION						= 3,
	RSMSG_SESSION_RESULT						= 4,

	// Ping/Pong
	RCMSG_PING									= 5,
	RSMSG_PONG									= 6,

	// SQL Query Execute
	RCMSG_SQL_EXECUTE							= 7,
	RCMSG_RELOAD_ACCOUNTS						= 8,

	// Authentication
	RCMSG_AUTH_CHALLENGE						= 9,
	RSMSG_AUTH_RESPONSE							= 10,

	// Character->Account Transmission
	RSMSG_REQUEST_ACCOUNT_CHARACTER_MAPPING		= 11,
	RCMSG_ACCOUNT_CHARACTER_MAPPING_REPLY		= 12,

	// Update Character->Account Mapping
	RCMSG_UPDATE_CHARACTER_MAPPING_COUNT		= 13,
	RSMSG_DISCONNECT_ACCOUNT					= 14,

	// Console auth
	RCMSG_TEST_CONSOLE_LOGIN					= 15,
	RSMSG_CONSOLE_LOGIN_RESULT					= 16,

	// DB modifying
	RCMSG_MODIFY_DATABASE						= 17,

	// count
	RMSG_COUNT									= 18,
};

#endif

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

#ifndef __ASCENT_OPCODES_H
#define __ASCENT_OPCODES_H

enum VoiceChatOpcodes
{
	VOICECHAT_CMSG_CREATE_CHANNEL			= 1,
	VOICECHAT_SMSG_CHANNEL_CREATED			= 2,
	VOICECHAT_CMSG_ADD_MEMBER				= 3,
	VOICECHAT_CMSG_REMOVE_MEMBER			= 4,
	VOICECHAT_CMSG_DELETE_CHANNEL			= 5,
	VOICECHAT_CMSG_PING						= 6,
	VOICECHAT_SMSG_PONG						= 7,
	VOICECHAT_NUM_OPCODES					= 8,
};

#endif


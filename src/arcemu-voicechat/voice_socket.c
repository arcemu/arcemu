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

#include "common.h"
#include "network.h"
#include "log.h"
#include "network_handlers.h"
#include "configfile.h"
#include "arcemu_packet.h"
#include "arcemu_opcodes.h"
#include "voice_channel.h"
#include "voice_channelmgr.h"

int voicechat_client_socket_read_handler(network_socket *s, int act)
{
	struct sockaddr_in read_address;
	char buffer[10000] = {0};			// people needing a bigger buffer, again can go to hell
	int bytes;

	uint16 channel_id;
	uint8 user_id;
	int i;

	voice_channel * chn;
	voice_channel_member * memb;

	if( act == IOEVENT_ERROR )
	{
		log_write(ERROR, "UDP Socket read an error!");
		return 0;
	}

	if( (bytes = network_read_data(s, buffer, 10000, (struct sockaddr*)&read_address)) < 0 )
	{
		log_write(ERROR, "UDP socket read error bytes!");
		return 0;
	}

	log_write(DEBUG, "udp socket got %d bytes from address %s", bytes, inet_ntoa(read_address.sin_addr));
	channel_id = *(uint16*)(&buffer[5]);
	user_id = buffer[4];

	chn = voice_channel_get((int)channel_id);
	log_write(DEBUG, "channel %u userid %u", (int)channel_id, (int)user_id);
	if( chn == NULL )
	{
		log_write(ERROR, "udp client sent invalid voice channel.");
		return 0;
	}

	if( user_id >= chn->member_slots )
	{
		log_write(ERROR, "udp client sent out of range user id.");
		return 0;
	}

	memb = &chn->members[user_id];

	// client initial packet check
	if( bytes == 7 )
	{
		if( memb->used && memcmp(&memb->client_address, &read_address, sizeof(struct sockaddr)) )
		{
			log_write(ERROR, "udp client is sending a different read address than it should be. desync maybe?");
		}

		memcpy(&memb->client_address, &read_address, sizeof(struct sockaddr));
		//printf("client %u address set to %s\n", (int)user_id, inet_ntoa(chn->members[user_id].client_address.sin_addr));
		memb->used = 1;		
	}
	else
	{
		// distribute the packet
		for( i = 0; i < chn->member_slots; ++i )
		{
			if( i == user_id || !chn->members[i].used || !chn->members[i].active )
				continue;			// don't send to yourself :P
			
			//printf("writing to client %u address %s\n", i, inet_ntoa(chn->members[i].client_address.sin_addr));
			if( network_write_data( s, buffer, bytes, (struct sockaddr*)&chn->members[i].client_address ) < 0 )
			{
				log_write(ERROR, "network_write_data to UDP client %s failed.", inet_ntoa(chn->members[i].client_address.sin_addr));
			}
		}
	}
	
	return 0;
}

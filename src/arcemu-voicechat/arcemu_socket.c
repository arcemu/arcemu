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

#define ASCENTSOCKET_RBUF_LEN 5000
volatile int g_serverCount = 0;
volatile int g_channelCount = 0;

int get_server_count() { return g_serverCount; }
int voice_get_channel_count() { return g_channelCount; }

typedef struct  
{
	char * read_buf;
	int read_buf_len;
	int read_buf_sz;
	network_socket * sock;
	int channelcount;
} ascent_socket;

ascent_socket * ascentsocket_allocate(network_socket *s)
{
	ascent_socket * ret = (ascent_socket*)vc_malloc(sizeof(ascent_socket));
	ret->read_buf = (char*)vc_malloc(ASCENTSOCKET_RBUF_LEN);
	ret->read_buf_len = 0;
	ret->read_buf_sz = ASCENTSOCKET_RBUF_LEN;
	ret->sock = s;
	ret->channelcount = 0;
	++g_serverCount;
	return ret;
}

void ascentsocket_free(ascent_socket* s)
{
	if( s->channelcount > 0 )
		voice_remove_channels((void*)s);

	free(s->read_buf);
	free(s);
	--g_serverCount;
}

void ascentsocket_send(ascent_socket *s, ascent_packet *p)
{
	// the zero isnt needed, just to shut the compiler up
	unsigned char buffer[10000] = {0};			// if you have a packet bigger than this you can go to hell
	*(uint16*)(&buffer[0]) = p->opcode;
	*(uint16*)(&buffer[2]) = p->size;

	if( p->wpos > 0 )
		memcpy(&buffer[4], p->buffer, p->wpos);

	network_write_data(s->sock, buffer, 4 + p->wpos, NULL);
}

// the listen handler is pretty easy, so we can just stick it here
int voicechat_ascent_listen_socket_read_handler(network_socket *s, int act)
{
	struct sockaddr_in new_address;
	int slen = sizeof(struct sockaddr_in);
	char buffer[100] = {'N','/','A'};
	network_socket * ns;
	int new_fd = accept(s->fd, (struct sockaddr*)&new_address, &slen);

	if( new_fd < 0 )
	{
		log_write(ERROR, "FATAL: accept() returned < %d", new_fd);
		abort();
		return -1;
	}

	//inet_ntop(AF_INET, &new_address.sin_addr, buffer, 100);
	//log_write(DEBUG, "Incoming TCP connection from %s port %u", buffer, (int)ntohs(new_address.sin_port));
	log_write(DEBUG, "Incoming TCP connection from %s port %u", inet_ntoa(new_address.sin_addr), (int)ntohs(new_address.sin_port));

	// create the socket structure.
	ns = (network_socket*)vc_malloc(sizeof(network_socket));
	network_init_socket(ns, new_fd, 5000);
	ns->event_handler = voicechat_ascent_socket_read_handler;
	ns->miscdata = (void*)ascentsocket_allocate(ns);		// used for voicechat struct later
	ns->write_handler = default_tcp_write_handler;

	// add it
	if( network_add_socket(ns) < 0 )
	{
		log_write(ERROR, "FD array is full!");
		ascentsocket_free((ascent_socket*)ns->miscdata);
		network_close(ns);
	}

	return 0;
}

// client handlers
void vc_handler_createchannel(ascent_socket *s, ascent_packet *p)
{
	uint8 type;
	uint32 request_id;
	ascent_packet reply;
	voice_channel * chn;
	uint8 error;
	uint16 reply_id;

	type = ascentpacket_readu8(p);
	request_id = ascentpacket_readu32(p);

	// attempt to create the channel
	chn = voice_channel_create((int)type, (void*)s);
	if( chn == NULL )
	{
		// channel creation failed
		error = 1;

		ascentpacket_init(VOICECHAT_SMSG_CHANNEL_CREATED, 5, &reply);
		ascentpacket_writeu32(&reply, request_id);
		ascentpacket_writeu8(&reply, error);

		// send reply
		ascentsocket_send(s, &reply);

		// free buffer
		ascentpacket_free(&reply);
	}
	else
	{
		// channel creation successful
		error = 0;
		reply_id = chn->channel_id;

		ascentpacket_init(VOICECHAT_SMSG_CHANNEL_CREATED, 7, &reply);
		ascentpacket_writeu32(&reply, request_id);
		ascentpacket_writeu8(&reply, error);
		ascentpacket_writeu16(&reply, reply_id);

		// send reply
		ascentsocket_send(s, &reply);

		// free buffer
		ascentpacket_free(&reply);

		// increment this
		++s->channelcount;
		++g_channelCount;
	}
}

void vc_handler_deletechannel(ascent_socket *s, ascent_packet *p)
{
	uint16 channel_id;
	voice_channel *ch;

	channel_id = ascentpacket_readu16(p);

	ch = voice_channel_get((int)channel_id);
	if( ch == NULL )
	{
		log_write(ERROR, "client is requesting us to delete a nonexistant channel");
		return;
	}

	log_write(DEBUG, "deleting channel id %u", (int)channel_id);
	if( voice_channel_remove(ch->channel_id) < 0 )
	{
		--s->channelcount;
		--g_channelCount;
	}
}

void vc_handler_addmember(ascent_socket *s, ascent_packet *p)
{
	uint16 channel_id;
	uint8 member_id;
	voice_channel *ch;

	channel_id = ascentpacket_readu16(p);
	member_id = ascentpacket_readu8(p);

	ch = voice_channel_get((int)channel_id);
	
	if( ch == NULL )
	{
		log_write(ERROR, "client is requesting us to add a member from a nonexistant channel");
		return;
	}

	if( member_id >= ch->member_slots )
	{
		log_write(ERROR, "client sent out of range voicechat member id");
		return;
	}

	log_write(DEBUG, "channel id %u slot %u is now enabled.", (int)channel_id, (int)member_id);
	ch->members[member_id].used = 0;
	ch->members[member_id].active = 1;
}

void vc_handler_deletemember(ascent_socket *s, ascent_packet *p)
{
	uint16 channel_id;
	uint8 member_id;
	voice_channel *ch;

	channel_id = ascentpacket_readu16(p);
	member_id = ascentpacket_readu8(p);

	ch = voice_channel_get((int)channel_id);
	if( ch == NULL )
	{
		log_write(ERROR, "client is requesting us to delete a member from a nonexistant channel");
		return;
	}

	if( member_id >= ch->member_slots )
	{
		log_write(ERROR, "client sent out of range voicechat member id");
		return;
	}

	log_write(DEBUG, "channel id %u slot %u is now disabled.", (int)channel_id, (int)member_id);
	ch->members[member_id].active = 0;
	ch->members[member_id].used = 0;
}

void vc_handler_ping(ascent_socket *s, ascent_packet *p)
{
	ascent_packet bp;
	uint32 pi;

	printf("ping!\n");
	pi = ascentpacket_readu32(p);

	ascentpacket_init(VOICECHAT_SMSG_PONG, 4, &bp);
	ascentpacket_writeu32(&bp, pi);
	ascentsocket_send(s, &bp);
	ascentpacket_free(&bp);
}

// handler table
typedef void(*vc_handler)(ascent_socket*,ascent_packet*);
static vc_handler vc_handler_table[VOICECHAT_NUM_OPCODES] = {
	NULL,											// 0
	vc_handler_createchannel,						// VOICECHAT_CMSG_CREATE_CHANNEL
	NULL,											// VOICECHAT_SMSG_CHANNEL_CREATED
	vc_handler_addmember,							// VOICECHAT_CMSG_ADD_MEMBER
	vc_handler_deletemember,						// VOICECHAT_CMSG_REMOVE_MEMBER
	vc_handler_deletechannel,						// VOICECHAT_CMSG_DELETE_CHANNEL
	vc_handler_ping,								// VOICECHAT_CMSG_PING
	NULL,											// VOICECHAT_SMSG_PONG
};

// client reader
int voicechat_ascent_socket_read_handler(network_socket *s, int act)
{
	ascent_socket * mysock = (ascent_socket*)s->miscdata;
	uint16 opcode;
	uint16 len;
	int readcount;
	ascent_packet * pck;

	if( s->miscdata == NULL )
		return -1;

	if( act == IOEVENT_ERROR )
	{
		ascentsocket_free(mysock);
		return -1;			// network code will clean up s pointer
	}

	readcount = network_read_data(s, &mysock->read_buf[mysock->read_buf_len], mysock->read_buf_sz - mysock->read_buf_len, NULL);
	if( readcount <= 0 )
	{
		log_write(DEBUG, "ascent socket %u failed reading. probably dead.", s->fd);
		// channel_delete_by_owner(mysock);
		ascentsocket_free(mysock);
		return -1;
	}

	mysock->read_buf_len += readcount;

	// read packetz, munch munch gobble gobble
	for(;;)
	{
		if( mysock->read_buf_len < 4 )
		{
			// size of the header.
			// packet must have been fragmented something nasty :P
			//printf("no full packet\n");
			break;
		}

		// got a full header, lets pull the details
		opcode = *(uint16*)&mysock->read_buf[0];
		len = *(uint16*)&mysock->read_buf[2];

		// do we have the full packet?
		if( mysock->read_buf_len < (len + 2) )
		{
			// wait for the full packet
			//printf("no full packet 2\n");
			break;
		}

		// create ascentpacket structure
		pck = ascentpacket_create(opcode, len);
		memcpy(pck->buffer, mysock->read_buf+4, len);
		mysock->read_buf_len -= 4;
		mysock->read_buf_len -= len;

		// move the buffer bytes back
		if( mysock->read_buf_len != 0 )
			memmove(mysock->read_buf, &mysock->read_buf[len+4], mysock->read_buf_len);

		// execute the handler
		if( opcode >= VOICECHAT_NUM_OPCODES )
		{
			log_write(ERROR, "Voicechat client with fd %u sent an invalid opcode (%u)", mysock->sock->fd, opcode);
		}
		else
		{
			// find and execute the handler
			if( vc_handler_table[opcode] == NULL )
				log_write(ERROR, "Voicechat client with fd %u sent an unhandled opcode (%u)", mysock->sock->fd, opcode);
			else
				vc_handler_table[opcode](mysock, pck);
		}
		
		// free the packet
		ascentpacket_free(pck);
	}
	
	return 0;
}


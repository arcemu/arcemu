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

#ifndef __ASCENT_PACKET_H
#define __ASCENT_PACKET_H

typedef struct  
{
	uint32 opcode;
	uint32 size;
	uint8* buffer;
	uint32 buffer_size;
	uint32 rpos;
	uint32 wpos;
	int stack;
} ascent_packet;

static ascent_packet* ascentpacket_create(uint32 opcode, uint32 size)
{
	ascent_packet * p;

	p = (ascent_packet*)vc_malloc(sizeof(ascent_packet));
	p->buffer = (uint8*)vc_malloc(size);
	p->buffer_size = size;
	p->opcode = opcode;
	p->size = size;
	p->wpos = p->rpos = 0;
	p->stack = 0;

	return p;
}

static void ascentpacket_init(uint32 opcode, uint32 size, ascent_packet *p)
{
	p->buffer = (uint8*)vc_malloc(size);
	p->buffer_size = size;
	p->opcode = opcode;
	p->size = size;
	p->wpos = p->rpos = 0;
	p->stack = 1;
}

static uint8* ascentpacket_getbuf(ascent_packet* p)
{
	return &p->buffer[p->wpos];
}

static void ascentpacket_free(ascent_packet* p)
{
	free(p->buffer);
	if( !p->stack )
		free(p);
}


/*
static uint32 ascentpacket_readu32(ascent_packet* p)
{
	uint32 orpos = p->rpos;
	p->rpos += sizeof(uint32);
	return *(uint32*)&p->buffer[orpos];
}
*/

#define DECLARE_ASCENTPACKET_OPERATOR(t, name) static t name(ascent_packet* p) { \
	uint32 orpos = p->rpos; \
	p->rpos += sizeof(t); \
	return *(t*)&p->buffer[orpos]; }

DECLARE_ASCENTPACKET_OPERATOR(uint32, ascentpacket_readu32);
DECLARE_ASCENTPACKET_OPERATOR(int32, ascentpacket_readi32);
DECLARE_ASCENTPACKET_OPERATOR(uint16, ascentpacket_readu16);
DECLARE_ASCENTPACKET_OPERATOR(int16, ascentpacket_readi16);
DECLARE_ASCENTPACKET_OPERATOR(uint8, ascentpacket_readu8);
DECLARE_ASCENTPACKET_OPERATOR(int8, ascentpacket_readi8);

/*
static void ascentpacket_writeu32(ascent_packet* p, uint32 v)
{
	*(uint32*)&p->buffer[p->wpos] = v;
	p->wpos += sizeof(uint32);
}
*/

#define DECLARE_ASCENTPACKET_WRITE_OPERATOR(t, name) static void name(ascent_packet* p, t v) { \
	*(t*)&p->buffer[p->wpos] = v; \
	p->wpos += sizeof(t); } 

DECLARE_ASCENTPACKET_WRITE_OPERATOR(uint32, ascentpacket_writeu32);
DECLARE_ASCENTPACKET_WRITE_OPERATOR(int32, ascentpacket_writei32);
DECLARE_ASCENTPACKET_WRITE_OPERATOR(uint16, ascentpacket_writeu16);
DECLARE_ASCENTPACKET_WRITE_OPERATOR(int16, ascentpacket_writei16);
DECLARE_ASCENTPACKET_WRITE_OPERATOR(uint8, ascentpacket_writeu8);
DECLARE_ASCENTPACKET_WRITE_OPERATOR(int8, ascentpacket_writei8);


#endif

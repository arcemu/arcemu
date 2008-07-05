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

#ifndef __NPCHANDLER_H
#define __NPCHANDLER_H

enum GOSSIP_SPECIAL
{
	GOSSIP_NO_SPECIAL		   = 0x00,
	GOSSIP_POI				  = 0x01,
	GOSSIP_SPIRIT_HEALER_ACTIVE = 0x02,
	GOSSIP_VENDOR			   = 0x03,
	GOSSIP_TRAINER			  = 0x04,
	GOSSIP_TABARD_VENDOR		= 0x05,
	GOSSIP_INNKEEPER			= 0x06,
	GOSSIP_PETITIONER		   = 0x07,
	GOSSIP_TAXI				 = 0x08,
	GOSSIP_TEXT				 = 0x09,
	GOSSIP_MENU				 = 0x0A,
	GOSSIP_BATTLEMASTER		 = 0x0B,
	GOSSIP_BANKEER			  = 0x0C,
};

enum TrainerServiceType
{
	TRAINER_SERVICE_AVAILABLE		=0x0,
	TRAINER_SERVICE_UNAVAILABLE	  =0x1,
	TRAINER_SERVICE_USED			 =0x2,
	TRAINER_SERVICE_NOT_SHOWN		=0x3,
	TRAINER_SERVICE_NEVER			=0x4,
	TRAINER_SERVICE_NO_PET		   =0x5
};
#define NUM_TRAINER_SERVICE_TYPES		0x6

enum TrainerType
{
	TRAINER_TYPE_GENERAL			 =0x0,
	TRAINER_TYPE_TALENTS			 =0x1,
	TRAINER_TYPE_TRADESKILLS		 =0x2,
	TRAINER_TYPE_PET				 =0x3
};
#define NUM_TRAINER_TYPES				0x4

#pragma pack(push,1)
struct GossipText_Text
{
	float Prob;
	char * Text[2];
	uint32 Lang;
	uint32 Emote[6];
};

struct GossipText
{
	uint32 ID;
	GossipText_Text Texts[8];
};
#pragma pack(pop)

struct GossipOptions
{
	uint32 ID;
	uint32 GossipID;
	uint16 Icon;
	std::string OptionText;
	uint32 NextTextID;
	uint32 Special;
	float PoiX;
	float PoiY;
	uint32 PoiIcon;
	uint32 PoiFlags;
	uint32 PoiData;
	std::string PoiName;
	uint32 BgMapId;
};

struct GossipNpc
{
	GossipNpc() { pOptions = NULL;  }
	uint32 ID;
	uint32 EntryId;
	uint32 TextID;
	uint32 OptionCount;
	GossipOptions *pOptions;
};

struct trainertype
{
	const char* partialname;
	uint32 type;
};

#define TRAINER_TYPE_MAX 16
extern trainertype trainer_types[TRAINER_TYPE_MAX];

#define TRAINER_STATUS_LEARNABLE		0
#define TRAINER_STATUS_NOT_LEARNABLE	1
#define TRAINER_STATUS_ALREADY_HAVE		2

#endif

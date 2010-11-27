/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

#ifndef __UNIT_H
#define __UNIT_H

class AIInterface;
class DynamicObject;

//these refer to visibility ranges. We need to store each stack of the aura and not just visible count.
#define MAX_POSITIVE_VISUAL_AURAS_START 0 // 
#define MAX_POSITIVE_VISUAL_AURAS_END 40 // 
#define MAX_NEGATIVE_VISUAL_AURAS_START MAX_POSITIVE_VISUAL_AURAS_END // 40 buff slots, 16 debuff slots.
#define MAX_NEGATIVE_VISUAL_AURAS_END ( MAX_POSITIVE_VISUAL_AURAS_END + 16 ) // 40 buff slots, 16 debuff slots.
//you hardly get to this but since i was testing i got to it :) : 20 items * 11 (enchants) + 61 talents
#define MAX_PASSIVE_AURAS_START 0   // these are reserved for talents. No need to check them for removes ?
#define MAX_PASSIVE_AURAS_END (MAX_PASSIVE_AURAS_START + 140)   // these are reserved for talents. No need to check them for removes ?
#define MAX_POSITIVE_AURAS_EXTEDED_START MAX_PASSIVE_AURAS_END   //these are not talents.These are stacks from visible auras
#define MAX_POSITIVE_AURAS_EXTEDED_END (MAX_POSITIVE_AURAS_EXTEDED_START + 100)   //these are not talents.These are stacks from visible auras
#define MAX_NEGATIVE_AURAS_EXTEDED_START MAX_POSITIVE_AURAS_EXTEDED_END   //these are not talents.These are stacks from visible auras
#define MAX_NEGATIVE_AURAS_EXTEDED_END (MAX_NEGATIVE_AURAS_EXTEDED_START + 100)   //these are not talents.These are stacks from visible auras
#define MAX_REMOVABLE_AURAS_START (MAX_POSITIVE_AURAS_EXTEDED_START) //do we need to handle talents at all ?
#define MAX_REMOVABLE_AURAS_END (MAX_NEGATIVE_AURAS_EXTEDED_END) //do we need to handle talents at all ?
#define MAX_TOTAL_AURAS_START (MAX_PASSIVE_AURAS_START)
#define MAX_TOTAL_AURAS_END (MAX_REMOVABLE_AURAS_END)

bool SERVER_DECL Rand(float);

#define UF_TARGET_DIED  1
#define UF_ATTACKING	2 // this unit is attacking it's selection
#define SPELL_GROUPS	96//This is actually on 64 bits !
#define DIMINISHING_GROUP_COUNT 15

#define UNIT_TYPE_HUMANOID_BIT (1 << (UNIT_TYPE_HUMANOID-1)) //should get computed by precompiler ;)

class Aura;
class Spell;
class AIInterface;
class GameObject;

struct CreatureInfo;
struct FactionTemplateDBC;
struct FactionDBC;

typedef HM_NAMESPACE::hash_map<uint32, uint64> UniqueAuraTargetMap;

#pragma pack(push, 1)
struct DisplayBounding
{
	uint32 displayid;
	float low[3];
	float high[3];
	float boundradius;
};
#pragma pack(pop)

struct ReflectSpellSchool
{
	uint32 spellId;
	uint32 charges;
	int32 school;
	int32 chance;
	int32 require_aura_hash;
	bool infront;
};

typedef struct
{
	uint32 spellid;
	uint64 caster;//not yet in use
	int32 amt;
}Absorb;

typedef std::list<Absorb*> SchoolAbsorb;

typedef struct 
{
	uint32 spellid;
	uint32 mindmg;
	uint32 maxdmg;
} OnHitSpell;

struct AreaAura
{
	uint32 auraid;
	Unit* caster;
};

typedef struct {
	SpellEntry *spell_info;
	uint32 charges;
} ExtraStrike;

enum DeathState
{
	ALIVE = 0,  // Unit is alive and well
	JUST_DIED,  // Unit has JUST died
	CORPSE,	 // Unit has died but remains in the world as a corpse
	DEAD		// Unit is dead and his corpse is gone from the world
};

#define HIGHEST_FACTION = 46
enum Factions {
	FACTION_BLOODSAIL_BUCCANEERS,
	FACTION_BOOTY_BAY,
	FACTION_GELKIS_CLAN_CENTAUR,
	FACTION_MAGRAM_CLAN_CENTAUR,
	FACTION_THORIUM_BROTHERHOOD,
	FACTION_RAVENHOLDT,
	FACTION_SYNDICATE,
	FACTION_GADGETZAN,
	FACTION_WILDHAMMER_CLAN,
	FACTION_RATCHET,
	FACTION_STEAMWHEEDLE_CARTEL,
	FACTION_ALLIANCE,
	FACTION_HORDE,
	FACTION_ARGENT_DAWN,
	FACTION_ORGRIMMAR,
	FACTION_DARKSPEAR_TROLLS,
	FACTION_THUNDER_BLUFF,
	FACTION_UNDERCITY,
	FACTION_GNOMEREGAN_EXILES,
	FACTION_STORMWIND,
	FACTION_IRONFORGE,
	FACTION_DARNASSUS,
	FACTION_LEATHERWORKING_DRAGON,
	FACTION_LEATHERWORKING_ELEMENTAL,
	FACTION_LEATHERWORKING_TRIBAL,
	FACTION_ENGINEERING_GNOME,
	FACTION_ENGINEERING_GOBLIN,
	FACTION_WINTERSABER_TRAINERS,
	FACTION_EVERLOOK,
	FACTION_BLACKSMITHING_ARMOR,
	FACTION_BLACKSMITHING_WEAPON,
	FACTION_BLACKSMITHING_AXE,
	FACTION_BLACKSMITHING_SWORD,
	FACTION_BLACKSMITHING_HAMMER,
	FACTION_CAER_DARROW,
	FACTION_TIMBERMAW_HOLD,
	FACTION_CENARION_CIRCLE,
	FACTION_THRALLMAR,
	FACTION_HONOR_HOLD,
	FACTION_THE_SHA_TAR,
	FACTION_STORMPIKE_GUARDS,
	FACTION_FROSTWOLF_CLAN,
	FACTION_HYDRAXIAN_WATERLORDS,
	FACTION_OUTLAND,
	FACTION_SHEN_DRALAR,
	FACTION_SILVERWING_SENTINELS,
	FACTION_WARSONG_OUTRIDERS,
	FACTION_ALLIANCE_FORCES,
	FACTION_HORDE_FORCES,
	FACTION_EXODAR,
	FACTION_DARKMOON_FAIRE,
	FACTION_ZANDALAR_TRIBE,
	FACTION_THE_DEFILERS,
	FACTION_THE_LEAGUE_OF_ARATHOR,
	FACTION_BROOD_OF_NOZDORMU,
	FACTION_SILVERMOON_CITY,
	FACTION_TRANQUILLIEN,
	FACTION_THE_SCALE_OF_THE_SANDS,
	FACTION_THE_ALDOR,
	FACTION_SHATTRATH_CITY,
	FACTION_THE_CONSORTIUM,
	FACTION_THE_MAG_HAR,
	FACTION_THE_SCRYERS,
	FACTION_THE_VIOLET_EYE,
	FACTION_CENARION_EXPEDITION,
	FACTION_SPOREGGAR,
	FACTION_KURENAI,
	FACTION_KEEPERS_OF_TIME,
	FACTION_FRIENDLY_HIDDEN,
	FACTION_LOWER_CITY,
	FACTION_ASHTONGUE_DEATHSWORN,
	FACTION_NETHERWING,
	FACTION_SHATARI_SKYGUARD,
	FACTION_OGRI_LA,
	FACTION_SHATTERED_SUN_OFFENSIVE = 80
};

typedef enum
{
	TEXTEMOTE_AGREE				= 1,
	TEXTEMOTE_AMAZE				= 2,
	TEXTEMOTE_ANGRY				= 3,
	TEXTEMOTE_APOLOGIZE			= 4,
	TEXTEMOTE_APPLAUD			= 5,
	TEXTEMOTE_BASHFUL			= 6,
	TEXTEMOTE_BECKON			= 7,
	TEXTEMOTE_BEG				= 8,
	TEXTEMOTE_BITE				= 9,
	TEXTEMOTE_BLEED				= 10,
	TEXTEMOTE_BLINK				= 11,
	TEXTEMOTE_BLUSH				= 12,
	TEXTEMOTE_BONK				= 13,
	TEXTEMOTE_BORED				= 14,
	TEXTEMOTE_BOUNCE			= 15,
	TEXTEMOTE_BRB				= 16,
	TEXTEMOTE_BOW				= 17,
	TEXTEMOTE_BURP				= 18,
	TEXTEMOTE_BYE				= 19,
	TEXTEMOTE_CACKLE			= 20,
	TEXTEMOTE_CHEER				= 21,
	TEXTEMOTE_CHICKEN			= 22,
	TEXTEMOTE_CHUCKLE			= 23,
	TEXTEMOTE_CLAP				= 24,
	TEXTEMOTE_CONFUSED			= 25,
	TEXTEMOTE_CONGRATULATE		= 26,
	TEXTEMOTE_COUGH				= 27,
	TEXTEMOTE_COWER				= 28,
	TEXTEMOTE_CRACK				= 29,
	TEXTEMOTE_CRINGE			= 30,
	TEXTEMOTE_CRY				= 31,
	TEXTEMOTE_CURIOUS			= 32,
	TEXTEMOTE_CURTSEY			= 33,
	TEXTEMOTE_DANCE				= 34,
	TEXTEMOTE_DRINK				= 35,
	TEXTEMOTE_DROOL				= 36,
	TEXTEMOTE_EAT				= 37,
	TEXTEMOTE_EYE				= 38,
	TEXTEMOTE_FART				= 39,
	TEXTEMOTE_FIDGET			= 40,
	TEXTEMOTE_FLEX				= 41,
	TEXTEMOTE_FROWN				= 42,
	TEXTEMOTE_GASP				= 43,
	TEXTEMOTE_GAZE				= 44,
	TEXTEMOTE_GIGGLE			= 45,
	TEXTEMOTE_GLARE				= 46,
	TEXTEMOTE_GLOAT				= 47,
	TEXTEMOTE_GREET				= 48,
	TEXTEMOTE_GRIN				= 49,
	TEXTEMOTE_GROAN				= 50,
	TEXTEMOTE_GROVEL			= 51,
	TEXTEMOTE_GUFFAW			= 52,
	TEXTEMOTE_HAIL				= 53,
	TEXTEMOTE_HAPPY				= 54,
	TEXTEMOTE_HELLO				= 55,
	TEXTEMOTE_HUG				= 56,
	TEXTEMOTE_HUNGRY			= 57,
	TEXTEMOTE_KISS				= 58,
	TEXTEMOTE_KNEEL				= 59,
	TEXTEMOTE_LAUGH				= 60,
	TEXTEMOTE_LAYDOWN			= 61,
	TEXTEMOTE_MASSAGE			= 62,
	TEXTEMOTE_MOAN				= 63,
	TEXTEMOTE_MOON				= 64,
	TEXTEMOTE_MOURN				= 65,
	TEXTEMOTE_NO				= 66,
	TEXTEMOTE_NOD				= 67,
	TEXTEMOTE_NOSEPICK			= 68,
	TEXTEMOTE_PANIC				= 69,
	TEXTEMOTE_PEER				= 70,
	TEXTEMOTE_PLEAD				= 71,
	TEXTEMOTE_POINT				= 72,
	TEXTEMOTE_POKE				= 73,
	TEXTEMOTE_PRAY				= 74,
	TEXTEMOTE_ROAR				= 75,
	TEXTEMOTE_ROFL				= 76,
	TEXTEMOTE_RUDE				= 77,
	TEXTEMOTE_SALUTE			= 78,
	TEXTEMOTE_SCRATCH			= 79,
	TEXTEMOTE_SEXY				= 80,
	TEXTEMOTE_SHAKE				= 81,
	TEXTEMOTE_SHOUT				= 82,
	TEXTEMOTE_SHRUG				= 83,
	TEXTEMOTE_SHY				= 84,
	TEXTEMOTE_SIGH				= 85,
	TEXTEMOTE_SIT				= 86,
	TEXTEMOTE_SLEEP				= 87,
	TEXTEMOTE_SNARL				= 88,
	TEXTEMOTE_SPIT				= 89,
	TEXTEMOTE_STARE				= 90,
	TEXTEMOTE_SURPRISED			= 91,
	TEXTEMOTE_SURRENDER			= 92,
	TEXTEMOTE_TALK				= 93,
	TEXTEMOTE_TALKEX			= 94,
	TEXTEMOTE_TALKQ				= 95,
	TEXTEMOTE_TAP				= 96,
	TEXTEMOTE_THANK				= 97,
	TEXTEMOTE_THREATEN			= 98,
	TEXTEMOTE_TIRED				= 99,
	TEXTEMOTE_VICTORY			= 100,
	TEXTEMOTE_WAVE				= 101,
	TEXTEMOTE_WELCOME			= 102,
	TEXTEMOTE_WHINE				= 103,
	TEXTEMOTE_WHISTLE			= 104,
	TEXTEMOTE_WORK				= 105,
	TEXTEMOTE_YAWN				= 106,
	TEXTEMOTE_BOGGLE			= 107,
	TEXTEMOTE_CALM				= 108,
	TEXTEMOTE_COLD				= 109,
	TEXTEMOTE_COMFORT			= 110,
	TEXTEMOTE_CUDDLE			= 111,
	TEXTEMOTE_DUCK				= 112,
	TEXTEMOTE_INSULT			= 113,
	TEXTEMOTE_INTRODUCE			= 114,
	TEXTEMOTE_JK				= 115,
	TEXTEMOTE_LICK				= 116,
	TEXTEMOTE_LISTEN			= 117,
	TEXTEMOTE_LOST				= 118,
	TEXTEMOTE_MOCK				= 119,
	TEXTEMOTE_PONDER			= 120,
	TEXTEMOTE_POUNCE			= 121,
	TEXTEMOTE_PRAISE			= 122,
	TEXTEMOTE_PURR				= 123,
	TEXTEMOTE_PUZZLE			= 124,
	TEXTEMOTE_RAISE				= 125,
	TEXTEMOTE_READY				= 126,
	TEXTEMOTE_SHIMMY			= 127,
	TEXTEMOTE_SHIVER			= 128,
	TEXTEMOTE_SHOO				= 129,
	TEXTEMOTE_SLAP				= 130,
	TEXTEMOTE_SMIRK				= 131,
	TEXTEMOTE_SNIFF				= 132,
	TEXTEMOTE_SNUB				= 133,
	TEXTEMOTE_SOOTHE			= 134,
	TEXTEMOTE_STINK				= 135,
	TEXTEMOTE_TAUNT				= 136,
	TEXTEMOTE_TEASE				= 137,
	TEXTEMOTE_THIRSTY			= 138,
	TEXTEMOTE_VETO				= 139,
	TEXTEMOTE_SNICKER			= 140,
	TEXTEMOTE_STAND				= 141,
	TEXTEMOTE_TICKLE			= 142,
	TEXTEMOTE_VIOLIN			= 143,
	TEXTEMOTE_SMILE				= 163,
	TEXTEMOTE_RASP				= 183,
	TEXTEMOTE_PITY				= 203,
	TEXTEMOTE_GROWL				= 204,
	TEXTEMOTE_BARK				= 205,
	TEXTEMOTE_SCARED			= 223,
	TEXTEMOTE_FLOP				= 224,
	TEXTEMOTE_LOVE				= 225,
	TEXTEMOTE_MOO				= 226,
	TEXTEMOTE_COMMEND			= 243,
	TEXTEMOTE_TRAIN				= 264,
	TEXTEMOTE_HELPME			= 303,
	TEXTEMOTE_INCOMING			= 304,
	TEXTEMOTE_CHARGE			= 305,
	TEXTEMOTE_FLEE				= 306,
	TEXTEMOTE_ATTACKMYTARGET	= 307,
	TEXTEMOTE_OOM				= 323,
	TEXTEMOTE_FOLLOW			= 324,
	TEXTEMOTE_WAIT				= 325,
	TEXTEMOTE_HEALME			= 326,
	TEXTEMOTE_OPENFIRE			= 327,
	TEXTEMOTE_FLIRT				= 328,
	TEXTEMOTE_JOKE				= 329,
	TEXTEMOTE_GOLFCLAP			= 343,
	TEXTEMOTE_WINK				= 363,
	TEXTEMOTE_PAT				= 364,
	TEXTEMOTE_SERIOUS			= 365,
	TEXTEMOTE_MOUNTSPECIAL		= 366,
	TEXTEMOTE_GOODLUCK			= 367,
	TEXTEMOTE_BLAME				= 368,
	TEXTEMOTE_BLANK				= 369,
	TEXTEMOTE_BRANDISH			= 370,
	TEXTEMOTE_BREATH			= 371,
	TEXTEMOTE_DISAGREE			= 372,
	TEXTEMOTE_DOUBT				= 373,
	TEXTEMOTE_EMBARRASS			= 374,
	TEXTEMOTE_ENCOURAGE			= 375,
	TEXTEMOTE_ENEMY				= 376,
	TEXTEMOTE_EYEBROW			= 377,
	TEXTEMOTE_TOAST				= 378,
	TEXTEMOTE_FAIL				= 379,
	TEXTEMOTE_HIGHFIVE			= 380,
	TEXTEMOTE_ABSENT			= 381,
	TEXTEMOTE_ARM				= 382,
	TEXTEMOTE_AWE				= 383,
	TEXTEMOTE_BACKPACK			= 384,
	TEXTEMOTE_BADFEELING		= 385,
	TEXTEMOTE_CHALLENGE			= 386,
	TEXTEMOTE_CHUG				= 387,
	TEXTEMOTE_DING				= 389,
	TEXTEMOTE_FACEPALM			= 390,
	TEXTEMOTE_FAINT				= 391,
	TEXTEMOTE_GO				= 392,
	TEXTEMOTE_GOING				= 393,
	TEXTEMOTE_GLOWER			= 394,
	TEXTEMOTE_HEADACHE			= 395,
	TEXTEMOTE_HICCUP			= 396,
	TEXTEMOTE_HISS				= 398,
	TEXTEMOTE_HOLDHAND			= 399,
	TEXTEMOTE_HURRY				= 401,
	TEXTEMOTE_IDEA				= 402,
	TEXTEMOTE_JEALOUS			= 403,
	TEXTEMOTE_LUCK				= 404,
	TEXTEMOTE_MAP				= 405,
	TEXTEMOTE_MERCY				= 406,
	TEXTEMOTE_MUTTER			= 407,
	TEXTEMOTE_NERVOUS			= 408,
	TEXTEMOTE_OFFER				= 409,
	TEXTEMOTE_PET				= 410,
	TEXTEMOTE_PINCH				= 411,
	TEXTEMOTE_PROUD				= 413,
	TEXTEMOTE_PROMISE			= 414,
	TEXTEMOTE_PULSE				= 415,
	TEXTEMOTE_PUNCH				= 416,
	TEXTEMOTE_POUT				= 417,
	TEXTEMOTE_REGRET			= 418,
	TEXTEMOTE_REVENGE			= 420,
	TEXTEMOTE_ROLLEYES			= 421,
	TEXTEMOTE_RUFFLE			= 422,
	TEXTEMOTE_SAD				= 423,
	TEXTEMOTE_SCOFF				= 424,
	TEXTEMOTE_SCOLD				= 425,
	TEXTEMOTE_SCOWL				= 426,
	TEXTEMOTE_SEARCH			= 427,
	TEXTEMOTE_SHAKEFIST			= 428,
	TEXTEMOTE_SHIFTY			= 429,
	TEXTEMOTE_SHUDDER			= 430,
	TEXTEMOTE_SIGNAL			= 431,
	TEXTEMOTE_SILENCE			= 432,
	TEXTEMOTE_SING				= 433,
	TEXTEMOTE_SMACK				= 434,
	TEXTEMOTE_SNEAK				= 435,
	TEXTEMOTE_SNEEZE			= 436,
	TEXTEMOTE_SNORT				= 437,
	TEXTEMOTE_SQUEAL			= 438,
	TEXTEMOTE_STOPATTACK		= 439,
	TEXTEMOTE_SUSPICIOUS		= 440,
	TEXTEMOTE_THINK				= 441,
	TEXTEMOTE_TRUCE				= 442,
	TEXTEMOTE_TWIDDLE			= 443,
	TEXTEMOTE_WARN				= 444,
	TEXTEMOTE_SNAP				= 445,
	TEXTEMOTE_CHARM				= 446,
	TEXTEMOTE_COVEREARS			= 447,
	TEXTEMOTE_CROSSARMS			= 448,
	TEXTEMOTE_LOOK				= 449,
	TEXTEMOTE_OBJECT			= 450,
	TEXTEMOTE_SWEAT				= 451,
} TextEmoteType;

typedef enum
{
	EMOTE_ONESHOT_NONE					= 0,
	EMOTE_ONESHOT_TALK					= 1, // DNR
	EMOTE_ONESHOT_BOW					= 2,
	EMOTE_ONESHOT_WAVE					= 3, // DNR
	EMOTE_ONESHOT_CHEER					= 4, // DNR
	EMOTE_ONESHOT_EXCLAMATION			= 5, // DNR
	EMOTE_ONESHOT_QUESTION				= 6,
	EMOTE_ONESHOT_EAT					= 7,
	EMOTE_STATE_DANCE					= 10,
	EMOTE_ONESHOT_LAUGH					= 11,
	EMOTE_STATE_SLEEP					= 12,
	EMOTE_STATE_SIT						= 13,
	EMOTE_ONESHOT_RUDE					= 14, // DNR
	EMOTE_ONESHOT_ROAR					= 15, // DNR
	EMOTE_ONESHOT_KNEEL					= 16,
	EMOTE_ONESHOT_KISS					= 17,
	EMOTE_ONESHOT_CRY					= 18,
	EMOTE_ONESHOT_CHICKEN				= 19,
	EMOTE_ONESHOT_BEG					= 20,
	EMOTE_ONESHOT_APPLAUD				= 21,
	EMOTE_ONESHOT_SHOUT					= 22, // DNR
	EMOTE_ONESHOT_FLEX					= 23,
	EMOTE_ONESHOT_SHY					= 24, // DNR
	EMOTE_ONESHOT_POINT					= 25, // DNR
	EMOTE_STATE_STAND					= 26,
	EMOTE_STATE_READYUNARMED			= 27,
	EMOTE_STATE_WORK_SHEATHED			= 28,
	EMOTE_STATE_POINT					= 29, // DNR
	EMOTE_STATE_NONE					= 30,
	EMOTE_ONESHOT_WOUND					= 33,
	EMOTE_ONESHOT_WOUNDCRITICAL			= 34,
	EMOTE_ONESHOT_ATTACKUNARMED			= 35,
	EMOTE_ONESHOT_ATTACK1H				= 36,
	EMOTE_ONESHOT_ATTACK2HTIGHT			= 37,
	EMOTE_ONESHOT_ATTACK2HLOOSE			= 38,
	EMOTE_ONESHOT_PARRYUNARMED			= 39,
	EMOTE_ONESHOT_PARRYSHIELD			= 43,
	EMOTE_ONESHOT_READYUNARMED			= 44,
	EMOTE_ONESHOT_READY1H				= 45,
	EMOTE_ONESHOT_READYBOW				= 48,
	EMOTE_ONESHOT_SPELLPRECAST			= 50,
	EMOTE_ONESHOT_SPELLCAST				= 51,
	EMOTE_ONESHOT_BATTLEROAR			= 53,
	EMOTE_ONESHOT_SPECIALATTACK1H		= 54,
	EMOTE_ONESHOT_KICK					= 60,
	EMOTE_ONESHOT_ATTACKTHROWN			= 61,
	EMOTE_STATE_STUN					= 64,
	EMOTE_STATE_DEAD					= 65,
	EMOTE_ONESHOT_SALUTE				= 66,
	EMOTE_STATE_KNEEL					= 68,
	EMOTE_STATE_USESTANDING				= 69,
	EMOTE_ONESHOT_WAVE_NOSHEATHE		= 70,
	EMOTE_ONESHOT_CHEER_NOSHEATHE		= 71,
	EMOTE_ONESHOT_EAT_NOSHEATHE			= 92,
	EMOTE_STATE_STUN_NOSHEATHE			= 93,
	EMOTE_ONESHOT_DANCE					= 94,
	EMOTE_ONESHOT_SALUTE_NOSHEATH		= 113,
	EMOTE_STATE_USESTANDING_NOSHEATHE	= 133,
	EMOTE_ONESHOT_LAUGH_NOSHEATHE		= 153,
	EMOTE_STATE_WORK					= 173,
	EMOTE_STATE_SPELLPRECAST			= 193,
	EMOTE_ONESHOT_READYRIFLE			= 213,
	EMOTE_STATE_READYRIFLE				= 214,
	EMOTE_STATE_WORK_MINING				= 233,
	EMOTE_STATE_WORK_CHOPWOOD			= 234,
	EMOTE_STATE_APPLAUD					= 253,
	EMOTE_ONESHOT_LIFTOFF				= 254,
	EMOTE_ONESHOT_YES					= 273, // DNR
	EMOTE_ONESHOT_NO					= 274, // DNR
	EMOTE_ONESHOT_TRAIN					= 275, // DNR
	EMOTE_ONESHOT_LAND					= 293,
	EMOTE_STATE_AT_EASE					= 313,
	EMOTE_STATE_READY1H					= 333,
	EMOTE_STATE_SPELLKNEELSTART			= 353,
	//EMOTE_STATE_SUBMERGED				= 373,
	EMOTE_ONESHOT_SUBMERGE				= 374,
	EMOTE_STATE_READY2H					= 375,
	EMOTE_STATE_READYBOW				= 376,
	EMOTE_ONESHOT_MOUNTSPECIAL			= 377,
	EMOTE_STATE_TALK					= 378,
	EMOTE_STATE_FISHING					= 379,
	EMOTE_ONESHOT_FISHING				= 380,
	EMOTE_ONESHOT_LOOT					= 381,
	EMOTE_STATE_WHIRLWIND				= 382,
	EMOTE_STATE_DROWNED					= 383,
	EMOTE_STATE_HOLD_BOW				= 384,
	EMOTE_STATE_HOLD_RIFLE				= 385,
	EMOTE_STATE_HOLD_THROWN				= 386,
	EMOTE_ONESHOT_DROWN					= 387,
	EMOTE_ONESHOT_STOMP					= 388,
	EMOTE_ONESHOT_ATTACKOFF				= 389,
	EMOTE_ONESHOT_ATTACKOFFPIERCE		= 390,
	EMOTE_STATE_ROAR					= 391,
	EMOTE_STATE_LAUGH					= 392,
	EMOTE_ONESHOT_CREATURE_SPECIAL		= 393,
	EMOTE_ONESHOT_JUMPLANDRUN			= 394,
	EMOTE_ONESHOT_JUMPEND				= 395,
	EMOTE_ONESHOT_TALK_NOSHEATHE		= 396,
	EMOTE_ONESHOT_POINT_NOSHEATHE		= 397,
	EMOTE_STATE_CANNIBALIZE				= 398,
	EMOTE_ONESHOT_JUMPSTART				= 399,
	EMOTE_STATE_DANCESPECIAL			= 400,
	EMOTE_ONESHOT_DANCESPECIAL			= 401,
	EMOTE_ONESHOT_CUSTOMSPELL01			= 402,
	EMOTE_ONESHOT_CUSTOMSPELL02			= 403,
	EMOTE_ONESHOT_CUSTOMSPELL03			= 404,
	EMOTE_ONESHOT_CUSTOMSPELL04			= 405,
	EMOTE_ONESHOT_CUSTOMSPELL05			= 406,
	EMOTE_ONESHOT_CUSTOMSPELL06			= 407,
	EMOTE_ONESHOT_CUSTOMSPELL07			= 408,
	EMOTE_ONESHOT_CUSTOMSPELL08			= 409,
	EMOTE_ONESHOT_CUSTOMSPELL09			= 410,
	EMOTE_ONESHOT_CUSTOMSPELL10			= 411,
	EMOTE_STATE_EXCLAIM					= 412,
	EMOTE_STATE_DANCE_CUSTOM			= 413,
	EMOTE_STATE_SIT_CHAIR_MED			= 415,
	EMOTE_STATE_CUSTOM_SPELL_01			= 416,
	EMOTE_STATE_CUSTOM_SPELL_02			= 417,
	EMOTE_STATE_EAT						= 418,
	EMOTE_STATE_CUSTOM_SPELL_04			= 419,
	EMOTE_STATE_CUSTOM_SPELL_03			= 420,
	EMOTE_STATE_CUSTOM_SPELL_05			= 421,
	EMOTE_STATE_SPELLEFFECT_HOLD		= 422,
	EMOTE_STATE_EAT_NO_SHEATHE			= 423,
	EMOTE_STATE_MOUNT					= 424,
	EMOTE_STATE_READY2HL				= 425,
	EMOTE_STATE_SIT_CHAIR_HIGH			= 426,
	EMOTE_STATE_FALL					= 427,
	EMOTE_STATE_LOOT					= 428,
	EMOTE_STATE_SUBMERGED				= 429, // NEW
	EMOTE_ONESHOT_COWER					= 430, // DNR
	EMOTE_STATE_COWER					= 431,
	EMOTE_ONESHOT_USESTANDING			= 432,
	EMOTE_STATE_STEALTH_STAND			= 433,
	EMOTE_ONESHOT_OMNICAST_GHOUL		= 434, // W/SOUND
	EMOTE_ONESHOT_ATTACKBOW				= 435,
	EMOTE_ONESHOT_ATTACKRIFLE			= 436,
	EMOTE_STATE_SWIM_IDLE				= 437,
	EMOTE_STATE_ATTACK_UNARMED			= 438,
	//EMOTE_ONESHOT_SPELLCAST				= 439, // W/SOUND
	EMOTE_ONESHOT_DODGE					= 440,
	EMOTE_ONESHOT_PARRY1H				= 441,
	EMOTE_ONESHOT_PARRY2H				= 442,
	EMOTE_ONESHOT_PARRY2HL				= 443,
	EMOTE_STATE_FLYFALL					= 444,
	EMOTE_ONESHOT_FLYDEATH				= 445,
	EMOTE_STATE_FLY_FALL				= 446,
	EMOTE_ONESHOT_FLY_SIT_GROUND_DOWN	= 447,
	EMOTE_ONESHOT_FLY_SIT_GROUND_UP		= 448,
	EMOTE_ONESHOT_EMERGE				= 449,
	EMOTE_ONESHOT_DRAGONSPIT			= 450,
	EMOTE_STATE_SPECIALUNARMED			= 451,
	EMOTE_ONESHOT_FLYGRAB				= 452,
	EMOTE_STATE_FLYGRABCLOSED			= 453,
	EMOTE_ONESHOT_FLYGRABTHROWN			= 454,
	EMOTE_STATE_FLY_SIT_GROUND			= 455,
	EMOTE_STATE_WALKBACKWARDS			= 456,
	EMOTE_ONESHOT_FLYTALK				= 457,
	EMOTE_ONESHOT_FLYATTACK1H			= 458,
	EMOTE_STATE_CUSTOMSPELL08			= 459,
	EMOTE_ONESHOT_FLY_DRAGONSPIT		= 460,
	EMOTE_STATE_SIT_CHAIR_LOW			= 461,
	EMOTE_ONE_SHOT_STUN					= 462,
	EMOTE_ONESHOT_SPELLCAST_OMNI		= 463,
	EMOTE_STATE_READYTHROWN				= 465,
	EMOTE_ONESHOT_WORK_CHOPWOOD			= 466,
	EMOTE_ONESHOT_WORK_MINING			= 467,
	EMOTE_STATE_SPELL_CHANNEL_OMNI		= 468,
	EMOTE_STATE_SPELL_CHANNEL_DIRECTED	= 469,
	EMOTE_ZZOLD_STATE_KNEEL_NO_INTERRUPT= 470,
} EmoteType;

enum StandState
{
	STANDSTATE_STAND			 = 0,
	STANDSTATE_SIT				 = 1,
	STANDSTATE_SIT_CHAIR		 = 2,
	STANDSTATE_SLEEP			 = 3,
	STANDSTATE_SIT_LOW_CHAIR	 = 4,
	STANDSTATE_SIT_MEDIUM_CHAIR  = 5,
	STANDSTATE_SIT_HIGH_CHAIR    = 6,
	STANDSTATE_DEAD				 = 7,
	STANDSTATE_KNEEL			 = 8,
	STANDSTATE_FORM_ALL          = 0x00FF0000,
    STANDSTATE_FLAG_ALWAYS_STAND = 0x01,                  // byte 4
    STANDSTATE_FLAG_CREEP        = 0x02000000,
    STANDSTATE_FLAG_UNTRACKABLE  = 0x04000000,
    STANDSTATE_FLAG_ALL          = 0xFF000000,
};

enum UnitSpecialStates
{
	UNIT_STATE_NORMAL    = 0x0000,
	UNIT_STATE_DISARMED  = 0X0001,
	UNIT_STATE_CHARM     = 0x0002,
	UNIT_STATE_FEAR      = 0x0004,
	UNIT_STATE_ROOT      = 0x0008,
	UNIT_STATE_SLEEP     = 0x0010,  // never set
	UNIT_STATE_SNARE     = 0x0020,  // never set
	UNIT_STATE_STUN      = 0x0040,
	UNIT_STATE_KNOCKOUT  = 0x0080,  // not used
	UNIT_STATE_BLEED     = 0x0100,  // not used
	UNIT_STATE_POLYMORPH = 0x0200,  // not used
	UNIT_STATE_BANISH    = 0x0400,  // not used
	UNIT_STATE_CONFUSE   = 0x0800,
	UNIT_STATE_PACIFY    = 0x1000,
	UNIT_STATE_SILENCE   = 0x2000,
	UNIT_STATE_FROZEN    = 0x4000,
};

enum UnitFieldBytes1
{
	U_FIELD_BYTES_ANIMATION_FROZEN = 0x01,
};

enum UnitFieldBytes2
{
	U_FIELD_BYTES_FLAG_PVP     = 0x01,
	U_FIELD_BYTES_FLAG_FFA_PVP = 0x04,
	U_FIELD_BYTES_FLAG_SANCTUARY  = 0x08,
	U_FIELD_BYTES_FLAG_AURAS   = 0x10,
};

enum UnitFieldFlags // UNIT_FIELD_FLAGS #46 - these are client flags
{	//                                            Hex    Bit     Decimal  Comments
	UNIT_FLAG_UNKNOWN_1                  = 0x00000001, // 1            1
	UNIT_FLAG_NOT_ATTACKABLE_2           = 0x00000002, // 2            2  client won't let you attack them
	UNIT_FLAG_LOCK_PLAYER                = 0x00000004, // 3            4  ? does nothing to client (probably wrong) - only taxi code checks this
	UNIT_FLAG_PLAYER_CONTROLLED          = 0x00000008, // 4            8  makes players and NPCs attackable / not attackable
	UNIT_FLAG_UNKNOWN_5                  = 0x00000010, // 5           16  ? some NPCs have this
	UNIT_FLAG_UNKNOWN_6                  = 0x00000020, // 6           32
	UNIT_FLAG_PLUS_MOB                   = 0x00000040, // 7           64  ? some NPCs have this (Rare/Elite/Boss?)
	UNIT_FLAG_UNKNOWN_8                  = 0x00000080, // 8          128  ? can change attackable status 
	UNIT_FLAG_NOT_ATTACKABLE_9           = 0x00000100, // 9          256  changes attackable status
	UNIT_FLAG_UNKNOWN_10                 = 0x00000200, // 10         512  ? some NPCs have this
	UNIT_FLAG_LOOTING                    = 0x00000400, // 11        1024
	UNIT_FLAG_SELF_RES                   = 0x00000800, // 12        2048  ? some NPCs have this
	UNIT_FLAG_PVP                        = 0x00001000, // 13        4096  sets PvP flag
	UNIT_FLAG_SILENCED                   = 0x00002000, // 14        8192
	UNIT_FLAG_DEAD                       = 0x00004000, // 15       16384  used for special "dead" NPCs like Withered Corpses
	UNIT_FLAG_UNKNOWN_16                 = 0x00008000, // 16       32768  ? some NPCs have this
	UNIT_FLAG_ALIVE                      = 0x00010000, // 17       65536  ?
	UNIT_FLAG_PACIFIED                   = 0x00020000, // 18      131072
	UNIT_FLAG_STUNNED                    = 0x00040000, // 19      262144
	UNIT_FLAG_COMBAT                     = 0x00080000, // 20      524288  sets combat flag
	UNIT_FLAG_MOUNTED_TAXI               = 0x00100000, // 21     1048576  mounted on a taxi
	UNIT_FLAG_DISARMED                   = 0x00200000, // 22     2097152
	UNIT_FLAG_CONFUSED                   = 0x00400000, // 23     4194304
	UNIT_FLAG_FLEEING                    = 0x00800000, // 24     8388608  fear
	UNIT_FLAG_PLAYER_CONTROLLED_CREATURE = 0x01000000, // 25    16777216
	UNIT_FLAG_NOT_SELECTABLE             = 0x02000000, // 26    33554432  cannot select the unit
	UNIT_FLAG_SKINNABLE                  = 0x04000000, // 27    67108864
	UNIT_FLAG_UNKNOWN_28                 = 0x08000000, // 28   134217728  ? was MAKE_CHAR_UNTOUCHABLE (probably wrong), nothing ever set it
	UNIT_FLAG_UNKNOWN_29                 = 0x10000000, // 29   268435456
	UNIT_FLAG_FEIGN_DEATH                = 0x20000000, // 30   536870912
	UNIT_FLAG_UNKNOWN_31                 = 0x40000000, // 31  1073741824  ? was WEAPON_OFF and being used for disarm
	UNIT_FLAG_UNKNOWN_32                 = 0x80000000, // 32  2147483648
};

enum UnitFieldFlags2
{
	UNIT_FLAG2_ENABLE_POWER_REGEN				= 0x800,
};

enum UnitDynamicFlags
{
	U_DYN_FLAG_LOOTABLE				 = 0x01,
	U_DYN_FLAG_UNIT_TRACKABLE		   = 0x02,
	U_DYN_FLAG_TAGGED_BY_OTHER		  = 0x04,
	U_DYN_FLAG_TAPPED_BY_PLAYER		 = 0x08,
	U_DYN_FLAG_PLAYER_INFO			  = 0x10,
	U_DYN_FLAG_DEAD					 = 0x20,
};

enum DamageFlags
{
	DAMAGE_FLAG_MELEE   = 1,
	DAMAGE_FLAG_HOLY	= 2,
	DAMAGE_FLAG_FIRE	= 4,
	DAMAGE_FLAG_NATURE  = 8,
	DAMAGE_FLAG_FROST   = 16,
	DAMAGE_FLAG_SHADOW  = 32,
	DAMAGE_FLAG_ARCANE  = 64
};

enum WeaponDamageType // this is NOT the same as SPELL_ENTRY_Spell_Dmg_Type, or Spell::GetType(), or SPELL_ENTRY_School !!
{
	MELEE   = 0,
	OFFHAND = 1,
	RANGED  = 2,
};

enum VisualState
{
	ATTACK = 1,
	DODGE,
	PARRY,
	INTERRUPT,
	BLOCK,
	EVADE,
	IMMUNE,
	DEFLECT
};

enum HitStatus
{
	HITSTATUS_UNK				= 0x01,
	HITSTATUS_HITANIMATION		= 0x02,
	HITSTATUS_DUALWIELD			= 0x04,
	HITSTATUS_MISS				= 0x10,
	HITSTATUS_ABSORBED			= 0x20 | 0x40,
	HITSTATUS_RESIST			= 0x80 | 0x100,
	HITSTATUS_CRICTICAL			= 0x200,
	HITSTATUS_BLOCK				= 0x2000,
	HITSTATUS_CRUSHINGBLOW		= 0x8000,
	HITSTATUS_GLANCING			= 0x10000,
	HITSTATUS_NOACTION			= 0x10000,
	HITSTATUS_SWINGNOHITSOUND	= 0x80000, // as in miss?
	HITSTATUS_UNK2              = 0x00800000
};

enum INVIS_FLAG
{
	INVIS_FLAG_NORMAL, // players and units with no special invisibility flags
	INVIS_FLAG_SPIRIT1,
	INVIS_FLAG_SPIRIT2,
	INVIS_FLAG_TRAP,
	INVIS_FLAG_QUEST,
	INVIS_FLAG_GHOST,
	INVIS_FLAG_UNKNOWN6,
	INVIS_FLAG_UNKNOWN7,
	INVIS_FLAG_SHADOWMOON,
	INVIS_FLAG_NETHERSTORM,
	INVIS_FLAG_BASHIR,
	INVIS_FLAG_UNKNOWN8,
	INVIS_FLAG_TOTAL
};

enum FIELD_PADDING//Since this field isn't used you can expand it for you needs
{
	PADDING_NONE
};

struct AuraCheckResponse
{
	uint32 Error;
	uint32 Misc;
};

enum AURA_CHECK_RESULT
{
	AURA_CHECK_RESULT_NONE				  = 1,
	AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT   = 2,
	AURA_CHECK_RESULT_LOWER_BUFF_PRESENT	= 3,
};

enum School
{
	SCHOOL_NORMAL = 0,
	SCHOOL_HOLY   = 1,
	SCHOOL_FIRE   = 2,
	SCHOOL_NATURE = 3,
	SCHOOL_FROST  = 4,
	SCHOOL_SHADOW = 5,
	SCHOOL_ARCANE = 6,
	SCHOOL_COUNT
};

typedef std::list<struct ProcTriggerSpellOnSpell> ProcTriggerSpellOnSpellList;

/************************************************************************/
/* "In-Combat" Handler                                                  */
/************************************************************************/

class Unit;
class SERVER_DECL CombatStatusHandler
{
	typedef set<uint64> AttackerMap;
	typedef set<uint32> HealedSet;		// Must Be Players!
	HealedSet m_healers;
	HealedSet m_healed;
	Unit* m_Unit;
	bool m_lastStatus;
	AttackerMap m_attackTargets;
	uint64 m_primaryAttackTarget;

public:
	CombatStatusHandler() : m_lastStatus(false), m_primaryAttackTarget(0) {}
	AttackerMap m_attackers;
	void AddAttackTarget(const uint64& guid);						// this means we clicked attack, not actually striked yet, so they shouldn't be in combat.
	void ClearPrimaryAttackTarget();								// means we deselected the unit, stopped attacking it.

	void OnDamageDealt(Unit * pTarget);								// this is what puts the other person in combat.
	void WeHealed(Unit * pHealTarget);								// called when a player heals another player, regardless of combat state.

	void RemoveAttacker(Unit * pAttacker, const uint64& guid);		// this means we stopped attacking them totally. could be because of deaggro, etc.
	void RemoveAttackTarget(Unit * pTarget);						// means our DoT expired.

	void UpdateFlag();												// detects if we have changed combat state (in/out), and applies the flag.

	bool IsInCombat();												// checks if we are in combat or not.

	void OnRemoveFromWorld();										// called when we are removed from world, kills all references to us.
	
	void Vanished()
	{
		ClearAttackers();
		ClearHealers();
	}

	const uint64& GetPrimaryAttackTarget() { return m_primaryAttackTarget; }
	void SetUnit(Unit * p) { m_Unit = p; }
	void TryToClearAttackTargets();									// for pvp timeout
	void AttackersForgetHate();										// used right now for Feign Death so attackers go home

protected:
	bool InternalIsInCombat();										// called by UpdateFlag, do not call from anything else!
	bool IsAttacking(Unit * pTarget);								// internal function used to determine if we are still attacking target x.
	void AddAttacker(const uint64& guid);							// internal function to add an attacker
	void RemoveHealed(Unit * pHealTarget);							// usually called only by updateflag
	void ClearHealers();											// this is called on instance change.
	void ClearAttackers();											// means we vanished, or died.
	void ClearMyHealers();
};

//====================================================================
//  Unit
//  Base class for Players and Creatures
//====================================================================

class SERVER_DECL Unit : public Object
{
public:

    void CombatStatusHandler_UpdatePvPTimeout();
	void CombatStatusHandler_ResetPvPTimeout();

	virtual ~Unit ( );

	friend class AIInterface;
	friend class Aura;

	virtual void Update( uint32 time );
	virtual void RemoveFromWorld(bool free_guid);
	virtual void OnPushToWorld();

    virtual bool IsPvPFlagged() = 0;
	virtual void SetPvPFlag() = 0;
	virtual void RemovePvPFlag() = 0;

    virtual bool IsFFAPvPFlagged() = 0;
    virtual void SetFFAPvPFlag() = 0;
    virtual void RemoveFFAPvPFlag() = 0;

	virtual bool IsSanctuaryFlagged() = 0;
	virtual void SetSanctuaryFlag() = 0;
	virtual void RemoveSanctuaryFlag() = 0;


    void setAttackTimer(int32 time, bool offhand);
	bool isAttackReady(bool offhand);
	
	void SetDualWield(bool enabled);

	bool  canReachWithAttack(Unit *pVictim);

	/// State flags are server-only flags to help me know when to do stuff, like die, or attack
	void addStateFlag(uint32 f) { m_state |= f; };
	bool hasStateFlag(uint32 f) { return (m_state & f ? true : false); }
	void clearStateFlag(uint32 f) { m_state &= ~f; };

	/// Stats
	uint32 getLevel() { return m_uint32Values[ UNIT_FIELD_LEVEL ]; };
	void setLevel(uint32 level) { SetUInt32Value(UNIT_FIELD_LEVEL, level); };
	void modLevel(int32 mod) { ModUnsigned32Value(UNIT_FIELD_LEVEL, mod); };
	uint32 getClassMask() { return 1 << (getClass() - 1); }
	uint32 getRaceMask() { return 1 << (getRace() - 1); }
	uint8 getStandState() { return ((uint8)m_uint32Values[UNIT_FIELD_BYTES_1]); }
 
	//// Combat
	uint32 GetSpellDidHitResult( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability );
	void Strike( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 add_damage, int32 pct_dmg_mod, uint32 exclusive_damage, bool disable_proc, bool skip_hit_check, bool force_crit = false );
	uint32 m_procCounter;
	uint32 HandleProc(uint32 flag, Unit* Victim, SpellEntry* CastingSpell, bool is_triggered = false, uint32 dmg = -1, uint32 abs = 0, uint32 weapon_damage_type = 0);
	void HandleProcDmgShield(uint32 flag, Unit* attacker);//almost the same as handleproc :P
	bool IsCriticalDamageForSpell(Object *victim, SpellEntry *spell);
	float GetCriticalDamageBonusForSpell(Object *victim, SpellEntry *spell, float amount);
	bool IsCriticalHealForSpell(Object *victim, SpellEntry *spell);
	float GetCriticalHealBonusForSpell(Object *victim, SpellEntry *spell, float amount);

	void RemoveExtraStrikeTarget(SpellEntry *spell_info);
	void AddExtraStrikeTarget(SpellEntry *spell_info, uint32 charges);

	int32 GetAP();
	int32 GetRAP();

	uint8 CastSpell(Unit* Target, uint32 SpellID, bool triggered);
	uint8 CastSpell(Unit* Target, SpellEntry* Sp, bool triggered);
	uint8 CastSpell(uint64 targetGuid, uint32 SpellID, bool triggered);
	uint8 CastSpell(uint64 targetGuid, SpellEntry* Sp, bool triggered);
	void CastSpellAoF(float x,float y,float z,SpellEntry* Sp, bool triggered);
	void EventCastSpell(Unit * Target, SpellEntry * Sp);

	bool IsCasting();
	bool IsInInstance();
    void CalculateResistanceReduction(Unit *pVictim,dealdamage *dmg,SpellEntry* ability, float ArmorPctReduce) ;
	void RegenerateHealth();
    void RegeneratePower(bool isinterrupted);
	void setHRegenTimer(uint32 time) {m_H_regenTimer = static_cast<uint16>( time ); }
	void setPRegenTimer(uint32 time) {m_P_regenTimer = static_cast<uint16>( time ); }
	void DelayPowerRegeneration(uint32 time) { m_P_regenTimer = static_cast<uint16>( time ); if (!m_interruptedRegenTime) m_interruptedRegenTime = 2000; }
	void DeMorph();
	uint32 ManaShieldAbsorb(uint32 dmg);
	void smsg_AttackStart(Unit* pVictim);
	void smsg_AttackStop(Unit* pVictim);
	void smsg_AttackStop(uint64 victimGuid);
	
	bool IsDazed();
	//this function is used for creatures to get chance to daze for another unit
	float get_chance_to_daze(Unit *target);

	// Stealth  
	int32 GetStealthLevel() { return m_stealthLevel; }
	int32 GetStealthDetectBonus() { return m_stealthDetectBonus; }
	void SetStealth(uint32 id) { m_stealth = id; }
	bool IsStealth() { return (m_stealth!= 0 ? true : false); }
	float detectRange;

	// Invisibility
	void SetInvisibility(uint32 id) { m_invisibility = id; }
	bool IsInvisible() { return (m_invisible!= 0 ? true : false); }
	uint32 m_invisibility;
	bool m_invisible;
	uint8 m_invisFlag;
	int32 m_invisDetect[INVIS_FLAG_TOTAL];

	bool HasAura(uint32 spellid); //this checks passive auras too
	uint16 GetAuraStackCount(uint32 spellid);
	bool HasAuraVisual(uint32 visualid);//not spell id!!!
	bool HasBuff(uint32 spelllid); //this does not check passive auras & it was visible auras
	bool HasBuff(uint32 spelllid, uint64 guid);//this does not check passive auras & it was visible auras
	bool HasVisialPosAurasOfNameHashWithCaster(uint32 namehash, Unit * caster);
	bool HasAuraWithMechanics(uint32 mechanic); //this checks passive auras too

	uint32 FindAuraCountByHash(uint32 HashName, uint32 maxcount = 0);
	
	void GiveGroupXP(Unit *pVictim, Player *PlayerInGroup);

	/// Combat / Death Status
	bool isAlive() { return m_deathState == ALIVE; };
	bool IsDead() { return  m_deathState !=ALIVE; };
	virtual void setDeathState(DeathState s) {
		m_deathState = s;
		if ( m_deathState==JUST_DIED ) DropAurasOnDeath();
	};
	DeathState getDeathState() { return m_deathState; }
	void OnDamageTaken();

	//! Add Aura to unit
	void AddAura(Aura *aur);
	//! Remove aura from unit
	bool RemoveAura(Aura *aur);
	bool RemoveAura(uint32 spellId);
	bool RemoveAura(uint32 spellId,uint64 guid);
	bool RemoveAuraByItemGUID(uint32 spellId,uint64 guid);
	bool RemoveAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAuras(uint32 * SpellIds);
	bool RemoveAurasByHeal();


	////////////////////////////////////////////////////////////////
	//void ClearAllAreaAuraTargets()
	//  Removes all area auras casted by us from the targets, and
	//  clears the target sets.
	//
	//Parameters
	//  None
	//
	//Return Value
	//  None
	//
	//
	////////////////////////////////////////////////////////////////
	void ClearAllAreaAuraTargets();

	//////////////////////////////////////////////////////////////////
	//void RemoveAllAreaAuraByOther()
	//  Removes all Area Auras that are from other Units.
	//
	//Parameters
	//  None
	//
	//Return Value
	//  None
	//
	//
	/////////////////////////////////////////////////////////////////
	void RemoveAllAreaAuraByOther();


	void EventRemoveAura(uint32 SpellId)
	{
		RemoveAura(SpellId);
	}

	//! Remove all auras
	void RemoveAllAuras();
    void RemoveAllNonPersistentAuras();
	bool RemoveAllAuras(uint32 spellId,uint64 guid); //remove stacked auras but only if they come from the same caster. Shaman purge If GUID = 0 then removes all auras with this spellid
    void RemoveAllAuraType(uint32 auratype);//ex:to remove morph spells
    void RemoveAllAuraFromSelfType2(uint32 auratype, uint32 butskip_hash);//ex:to remove morph spells
	uint32 RemoveAllAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	uint32 RemoveAllAuraById(uint32 Id); // DuKJIoHuyC: Remove an aura by it's id
	bool RemoveAllAurasByMechanic( uint32 MechanicType , uint32 MaxDispel , bool HostileOnly ); // Removes all (de)buffs on unit of a specific mechanic type.
	void RemoveAllMovementImpairing();
	void RemoveAllAurasByRequiredShapeShift(uint32 mask);

	void RemoveNegativeAuras();
	// Temporary remove all auras
	   // Find auras
	Aura *FindAuraByNameHash(uint32 namehash);
	Aura *FindAuraByNameHash(uint32 namehash, uint64 guid);
	Aura* FindAura(uint32 spellId);
	Aura* FindAura(uint32* spellId);
	Aura* FindAura(uint32 spellId, uint64 guid);
	Aura* FindAuraWithAuraEffect(uint32 effect, uint32* x);
	bool SetAurDuration(uint32 spellId,Unit* caster,uint32 duration);
	bool SetAurDuration(uint32 spellId,uint32 duration);
	void DropAurasOnDeath();
	   
	void castSpell(Spell * pSpell);
	void InterruptSpell();

	//caller is the caster
	int32 GetSpellDmgBonus(Unit *pVictim, SpellEntry *spellInfo,int32 base_dmg, bool isdot);
   
    uint32 m_addDmgOnce;
	Creature *m_TotemSlots[4];
	uint32 m_ObjectSlots[4];
	uint32 m_triggerSpell;
	uint32 m_triggerDamage;
	uint32 m_canMove;
	
	// Spell Effect Variables
	int32 m_silenced;
	bool m_damgeShieldsInUse;
	std::list<struct DamageProc> m_damageShields;
	std::list<struct ReflectSpellSchool*> m_reflectSpellSchool;
	void RemoveReflect( uint32 spellid , bool apply);
	struct DamageSplitTarget *m_damageSplitTarget;
 
	/********************************************************/
	/*   ProcTrigger                                        */
	/********************************************************/
	std::list<SpellProc*> m_procSpells;
	void AddProcTriggerSpell(uint32 spell_id, uint32 orig_spell_id, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32 *groupRelation, uint32 *procClassMask = NULL, Object *obj = NULL);
	void AddProcTriggerSpell(SpellEntry *spell, SpellEntry *orig_spell, uint64 caster, uint32 procChance, uint32 procFlags, uint32 procCharges, uint32 *groupRelation, uint32 *procClassMask = NULL, Object *obj = NULL);
	void AddProcTriggerSpell(SpellEntry *sp, uint64 caster, uint32 *groupRelation, uint32 *procClassMask = NULL, Object *obj = NULL);
	void RemoveProcTriggerSpell(uint32 spellId, uint64 casterGuid = 0, uint64 misc = 0);
	std::map<uint32,struct SpellCharge> m_chargeSpells;
	deque<uint32> m_chargeSpellRemoveQueue;
	bool m_chargeSpellsInUse;
	void SetOnMeleeSpell( uint32 spell, uint8 ecn = 0 ) { m_meleespell = spell; m_meleespell_ecn = ecn;  }
	uint32 GetOnMeleeSpell() { return m_meleespell; }
	uint8 GetOnMeleeSpellEcn() { return m_meleespell_ecn; }

	uint32 DoDamageSplitTarget(uint32 res, uint32 school_type, bool melee_dmg);

	// Spell Crit
	float spellcritperc;

	// AIInterface
	AIInterface *GetAIInterface() { return m_aiInterface; }
	void ReplaceAIInterface(AIInterface *new_interface) ;
	void ClearHateList();
	void WipeHateList();
	void WipeTargetList();
	void setAItoUse(bool value){m_useAI = value;}

	int32 GetThreatModifyer() { return m_threatModifyer; }
	void ModThreatModifyer(int32 mod) { m_threatModifyer += mod; }
	int32 GetGeneratedThreatModifyer(uint32 school) { return m_generatedThreatModifyer[school]; }
	void ModGeneratedThreatModifyer(uint32 school, int32 mod) { m_generatedThreatModifyer[school] += mod; }

	void SetHitFromMeleeSpell(float value) { m_hitfrommeleespell = value; }
	float GetHitFromMeleeSpell() { return m_hitfrommeleespell; }
	float m_hitfrommeleespell;

	// DK:Affect
	uint32 IsPacified() { return m_pacified; }
	uint32 IsStunned() { return m_stunned; }
	uint32 IsFeared() { return m_fearmodifiers; }
	uint32 GetResistChanceMod() { return m_resistChance; }
	void SetResistChanceMod(uint32 amount) { m_resistChance=amount; }
	
	uint16 HasNoInterrupt() { return m_noInterrupt; }
	bool setDetectRangeMod(uint64 guid, int32 amount);
	void unsetDetectRangeMod(uint64 guid);
	int32 getDetectRangeMod(uint64 guid);
	void Heal(Unit* target,uint32 SpellId, uint32 amount);
	void Energize(Unit* target,uint32 SpellId, uint32 amount, uint32 type);

	Loot loot;
	uint32 SchoolCastPrevent[SCHOOL_COUNT];
	int32 MechanicDurationPctMod[28];
	int32 GetDamageDoneMod(uint32 school);
	float GetDamageDonePctMod(uint32 school);
	float DamageDoneModPCT[SCHOOL_COUNT];
	int32 DamageTakenMod[SCHOOL_COUNT];
	float DamageTakenPctMod[SCHOOL_COUNT];
	float DamageTakenPctModOnHP35;
	float CritMeleeDamageTakenPctMod[SCHOOL_COUNT];
	float CritRangedDamageTakenPctMod[SCHOOL_COUNT];
	int32 RangedDamageTaken;
	void CalcDamage();
	float BaseDamage[2];
	float BaseOffhandDamage[2];
	float BaseRangedDamage[2];
	SchoolAbsorb Absorbs[SCHOOL_COUNT];
	uint32 AbsorbDamage(uint32 School,uint32 * dmg);//returns amt of absorbed dmg, decreases dmg by absorbed value
	int32 RAPvModifier;
	int32 APvModifier;
	uint64 stalkedby;
	uint32 dispels[10];
	bool trackStealth;
	uint32 MechanicsDispels[32];
	float MechanicsResistancesPCT[32];
	float ModDamageTakenByMechPCT[32];
	int32 DoTPctIncrease[SCHOOL_COUNT];
	float AOEDmgMod;
	float m_ignoreArmorPctMaceSpec;
	float m_ignoreArmorPct;

	//SM
	int32 * SM_FDamageBonus;//flat
	int32 * SM_PDamageBonus;//pct

	int32 * SM_FDur;//flat
	int32 * SM_PDur;//pct

	int32 * SM_FThreat;//flat
	int32 * SM_PThreat;//Pct

	int32 * SM_FEffect1_Bonus;//flat
	int32 * SM_PEffect1_Bonus;//Pct

	int32 * SM_FCharges;//flat
	int32 * SM_PCharges;//Pct

	int32 * SM_FRange;//flat
	int32 * SM_PRange;//pct

	int32 * SM_FRadius;//flat
	int32 * SM_PRadius;//pct
	
	int32 * SM_CriticalChance;//flat

	int32 * SM_FMiscEffect;//flat
	int32 * SM_PMiscEffect;//pct
	
	int32 * SM_PNonInterrupt;//Pct

	int32 * SM_FCastTime;//flat
	int32 * SM_PCastTime;//pct
	
	int32 * SM_FCooldownTime;//flat
	int32 * SM_PCooldownTime;//Pct
	
	int32 * SM_FEffect2_Bonus;//flat
	int32 * SM_PEffect2_Bonus;//Pct

	int32 * SM_FCost;//flat
	int32 * SM_PCost;//Pct
	
	int32 * SM_PCriticalDamage;//Pct

	int32 * SM_FHitchance;//flat

	int32 * SM_FAdditionalTargets;//flat

	int32 * SM_FChanceOfSuccess;//flat

	int32 * SM_FAmptitude;//flat
	int32 * SM_PAmptitude;//Pct

	int32 * SM_PJumpReduce;//Pct

	int32 * SM_FGlobalCooldown;//flat
	int32 * SM_PGlobalCooldown;//pct

	int32 * SM_FDOT;//flat
	int32 * SM_PDOT;//pct
	
	int32 * SM_FEffect3_Bonus;//flat
	int32 * SM_PEffect3_Bonus;//Pct

	int32 * SM_FPenalty;//flat
	int32 * SM_PPenalty;//Pct

	int32 * SM_FEffectBonus;//flat
	int32 * SM_PEffectBonus;//pct	
	
	int32 * SM_FRezist_dispell;//flat
	int32 * SM_PRezist_dispell;//Pct	

	void InheritSMMods(Unit *inherit_from);

	//Events
	void Emote (EmoteType emote);
	void EventAddEmote(EmoteType emote, uint32 time);
	void EmoteExpire();
	void setEmoteState(uint8 emote) { m_emoteState = emote; };
	uint32 GetOldEmote() { return m_oldEmote; }
	void EventAurastateExpire(uint32 aurastateflag){RemoveFlag(UNIT_FIELD_AURASTATE,aurastateflag);} //hmm this looks like so not necessary :S
	void EventHealthChangeSinceLastUpdate();

    /************************************************************************/
    /* Stun Immobilize                                                      */
    /************************************************************************/
	uint32	    trigger_on_stun;        //bah, warrior talent but this will not get triggered on triggered spells if used on proc so I'm forced to used a special variable
	uint32	    trigger_on_stun_chance;
	uint32	    trigger_on_stun_victim;
	uint32	    trigger_on_stun_chance_victim;

	void SetTriggerStunOrImmobilize(uint32 newtrigger,uint32 new_chance,bool is_victim=false)
    {
		if( is_victim == false )
		{
			trigger_on_stun = newtrigger;
			trigger_on_stun_chance = new_chance;
		}
		else
		{
			trigger_on_stun_victim = newtrigger;
			trigger_on_stun_chance_victim = new_chance;
		}
    }
    void EventStunOrImmobilize(Unit *proc_target,bool is_victim=false);

	// TODO: Remove this hack
    /************************************************************************/
    /* Chill                                                                */
    /************************************************************************/
	uint32	    trigger_on_chill;         //mage "Frostbite" talent chill
	uint32	    trigger_on_chill_chance;
	uint32	    trigger_on_chill_victim;
	uint32	    trigger_on_chill_chance_victim;

	void SetTriggerChill(uint32 newtrigger,uint32 new_chance,bool is_victim=false)
    {
		if( is_victim == false )
		{
			trigger_on_chill = newtrigger;
			trigger_on_chill_chance = new_chance;
		}
		else
		{
			trigger_on_chill_victim = newtrigger;
			trigger_on_chill_chance_victim = new_chance;
		}
    }
    void EventChill(Unit *proc_target,bool is_victim=false);

	void SetStandState (uint8 standstate);

	StandState GetStandState()
	{
		uint32 bytes1 = GetUInt32Value (UNIT_FIELD_BYTES_1);
		return StandState (uint8 (bytes1));
	}

	uint32 GetFaction() { return GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE); }

	void SetFaction(uint32 factionId)
	{
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, factionId );
		_setFaction();
	}

	virtual void SendChatMessage(uint8 type, uint32 lang, const char *msg, uint32 delay = 0) = 0;
	void SendChatMessageToPlayer(uint8 type, uint32 lang, const char *msg, Player *plr);
	void SendChatMessageAlternateEntry(uint32 entry, uint8 type, uint32 lang, const char * msg);
	void RegisterPeriodicChatMessage(uint32 delay, uint32 msgid, std::string message, bool sendnotify);

	int GetHealthPct()
	{
		//shitty db? pet/guardian bug?
		if (GetUInt32Value(UNIT_FIELD_HEALTH) == 0 || GetUInt32Value(UNIT_FIELD_MAXHEALTH) == 0)
			return 0;

		return (int)(GetUInt32Value(UNIT_FIELD_HEALTH) * 100 / GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	};

    void SetHealthPct(uint32 val) { if (val>0) SetHealth(float2int32(val*0.01f*GetUInt32Value(UNIT_FIELD_MAXHEALTH))); };

	int GetManaPct()
	{
		if (GetPower(0) == 0 || GetMaxPower(0) == 0) //POWER_TYPE_MANA
			return 0;

		return (int)(GetPower(0) * 100 / GetMaxPower(0));
	};
		
	//In-Range
	virtual void AddInRangeObject(Object* pObj);
	virtual void OnRemoveInRangeObject(Object* pObj);
	void ClearInRangeSet();

	uint32 m_CombatUpdateTimer;

	void setcanparry(bool newstatus){can_parry=newstatus;}
		
	std::map<uint32,Aura*> tmpAura;

	uint32 BaseResistance[SCHOOL_COUNT]; //there are resistances for silence, fear, mechanics ....
	uint32 BaseStats[5];
	int32 HealDoneMod[SCHOOL_COUNT];
	float HealDonePctMod[SCHOOL_COUNT];
	int32 HealTakenMod[SCHOOL_COUNT];
	float HealTakenPctMod[SCHOOL_COUNT];
	uint32 SchoolImmunityList[SCHOOL_COUNT];
	float SpellCritChanceSchool[SCHOOL_COUNT];
	int32 PowerCostMod[SCHOOL_COUNT];
	float PowerCostPctMod[SCHOOL_COUNT]; // armor penetration & spell penetration
	int32 AttackerCritChanceMod[SCHOOL_COUNT];
	uint32 SpellDelayResist[SCHOOL_COUNT];
	int32 CreatureAttackPowerMod[12];
	int32 CreatureRangedAttackPowerMod[12];

	int32 PctRegenModifier;
	float PctPowerRegenModifier[4];
	
	void UpdatePowerAmm();

	void RemoveAurasByInterruptFlag(uint32 flag);
	void RemoveAurasByInterruptFlagButSkip(uint32 flag, uint32 skip);
	// Auras Modifiers
	int32 m_pacified;
	int32 m_interruptRegen;
	int32 m_resistChance;
	int32 m_powerRegenPCT;
	int32 m_stunned;
	int32 m_extraattacks;   
	bool m_extrastriketarget;
	int32 m_extrastriketargetc;
	std::list<ExtraStrike*> m_extraStrikeTargets;
	int32 m_fearmodifiers;
	int64 m_magnetcaster; // Unit who acts as a magnet for this unit
	
	//Combat Mod Results:
	int32 m_CombatResult_Dodge;
	int32 m_CombatResult_Parry; //is not implented yet

	// aurastate counters
	int8 asc_frozen;
	int8 asc_enraged;
	int8 asc_seal;
	int8 asc_bleed;

	uint16 m_noInterrupt;
	int32 m_rooted;
	bool disarmed;
	uint64 m_detectRangeGUID[5];
	int32  m_detectRangeMOD[5];
	// Affect Speed
	int32 m_speedModifier;
	int32 m_slowdown;
	float m_maxSpeed;
	map< uint32, int32 > speedReductionMap;
	bool GetSpeedDecrease();
	int32 m_mountedspeedModifier;
	int32 m_flyspeedModifier;
	void UpdateSpeed();
	void EnableFlight();
	void DisableFlight();

	// Escort Quests

	void MoveToWaypoint(uint32 wp_id);	
	void PlaySpellVisual(uint64 target, uint32 spellVisual);

	void RemoveStealth()
	{
		if( m_stealth != 0 )
		{
			RemoveAura( m_stealth );
			m_stealth = 0;
		}
	}

	void RemoveInvisibility()
	{
		if( m_invisibility != 0 )
		{
			RemoveAura( m_invisibility );
			m_invisibility = 0;
		}
		else if ( m_mageInvisibility != 0 )
		{
			m_mageInvisibility = false;
		}
	}

	uint32 m_stealth;
	bool m_can_stealth;

	Aura* m_auras[MAX_TOTAL_AURAS_END];   

	int32 m_modlanguage;
	
	Creature *critterPet;
	
	/************************************************************************/
    /* Guardians                                                            */
    /************************************************************************/

	//guardians are temporary spawn that will inherit master faction and will follow them. Apart from that they have their own mind	
	std::set<Creature*> m_Guardians;
	Creature* create_guardian( uint32 guardian_entry, uint32 duration, float angle, uint32 lvl = 0, GameObject * obj = NULL, LocationVector * Vec = NULL, uint32 spellid = 0 ); 
	void AddGuardianRef( Creature* guard ){ Arcemu::Util::ARCEMU_ASSERT(    guard != NULL );  m_Guardians.insert( guard );	}
	void RemoveGuardianRef( Creature* g );
	void RemoveAllGuardians( bool remove_from_world = true );

	/************************************************************************/

	uint32 GetCharmTempVal() { return m_charmtemp; }
	void SetCharmTempVal(uint32 val) { m_charmtemp = val; }

	void DisableAI() { m_useAI = false; }
	void EnableAI() { m_useAI = true; }

	bool IsSpiritHealer()
	{
		switch(GetEntry())
		{
		case 6491:  // Spirit Healer
		case 13116: // Alliance Spirit Guide
		case 13117: // Horde Spirit Guide
		case 9299:  // Gaeriyan (Qnpc)
		case 8888:  // Franclorn Forgewright (Qnpc)
		case 29259: // Scarlet Enclave
			{
				return true;
			}break;
		}
		return false;
	}

	void Phase(uint8 command=PHASE_SET, uint32 newphase=1);

	bool Tagged;
	uint64 TaggerGuid;
	void Tag( uint64 TaggerGUID );
	void UnTag();
	bool IsTagged();
	bool IsTaggable();
	uint64 GetTaggerGUID();
	bool isLootable();

	void Root();
	void Unroot();
    bool isRooted(){

        if( m_rooted )
            return true;
        else
            return false;
    }


	virtual bool isTrainingDummy(){ return false; }

	void SetFacing(float newo);//only working if creature is idle

	void RemoveAurasByBuffType(uint32 buff_type, const uint64 &guid,uint32 skip);
	bool HasAurasOfBuffType(uint32 buff_type, const uint64 &guid,uint32 skip);
	int	 HasAurasWithNameHash(uint32 name_hash);
	bool HasAuraWithName( uint32 name );
	uint32 GetAuraCountWithName( uint32 name );
	bool IsPoisoned();

	AuraCheckResponse AuraCheck(SpellEntry *proto, Object *caster= NULL);
	AuraCheckResponse AuraCheck(SpellEntry *proto, Aura* aur, Object *caster= NULL);

	uint16 m_diminishCount[DIMINISHING_GROUP_COUNT];
	uint8  m_diminishAuraCount[DIMINISHING_GROUP_COUNT];
	uint16 m_diminishTimer[DIMINISHING_GROUP_COUNT];
	bool   m_diminishActive;

	void SetDiminishTimer(uint32 index)
	{
		m_diminishTimer[index] = 15000;
	}

	DynamicObject * dynObj;
	
	//! returns: aura stack count
	uint8 m_auraStackCount[MAX_NEGATIVE_VISUAL_AURAS_END];

	void SendFullAuraUpdate();
	void SendAuraUpdate( uint32 AuraSlot, bool remove );
	uint32 ModVisualAuraStackCount(Aura *aur, int32 count);
	uint8 FindVisualSlot(uint32 SpellId,bool IsPos);
	uint32 m_auravisuals[MAX_NEGATIVE_VISUAL_AURAS_END];

	void RemoveAurasOfSchool(uint32 School, bool Positive, bool Immune);
	SpellEntry * pLastSpell;
	bool bProcInUse;
	bool bInvincible;
	Player * m_redirectSpellPackets;
	void UpdateVisibility();

	//solo target auras
	uint32 polySpell;
	uint32 m_special_state; //flags for special states (stunned,rooted etc)

	struct {
		int32 amt;
		int32 max;
	} m_soulSiphon;

	uint32 m_cTimer;
	void EventUpdateFlag();
	CombatStatusHandler CombatStatus;
	bool m_temp_summon;
	bool m_mageInvisibility;

	void CancelSpell(Spell * ptr);
	void EventStrikeWithAbility(uint64 guid, SpellEntry * sp, uint32 damage);
	void DispelAll(bool positive);

	void SendPowerUpdate(bool self);
	void SendPeriodicAuraLog( const WoWGuid& CasterGUID, const WoWGuid& casterGUID, uint32 SpellID, uint32 School, uint32 Amount, uint32 abs_dmg, uint32 resisted_damage, uint32 Flags, bool is_critical );
	void SendPeriodicHealAuraLog( const WoWGuid& CasterGUID, const WoWGuid& TargetGUID, uint32 SpellID, uint32 healed, uint32 over_healed, bool is_critical );

	void EventModelChange();
	ARCEMU_INLINE float GetModelHalfSize() { return m_modelhalfsize * GetScale(); }

	void RemoveFieldSummon();

	float GetBlockFromSpell() { return m_blockfromspell; }
	float GetParryFromSpell() { return m_parryfromspell; }
	float GetDodgeFromSpell() { return m_dodgefromspell; }
	void SetBlockFromSpell(float value) { m_blockfromspell = value; }
	void SetParryFromSpell(float value) { m_parryfromspell = value; }
	void SetDodgeFromSpell(float value) { m_dodgefromspell = value; }
	
	void AggroPvPGuards();

/////////////////////////////////////////////////////// Unit properties ///////////////////////////////////////////////////
    void SetCharmedUnitGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_CHARM, GUID ); }
    void SetSummonedUnitGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_SUMMON, GUID ); }
    void SetSummonedCritterGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_CRITTER, GUID ); }

    void SetCharmedByGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_CHARMEDBY, GUID ); }
    void SetSummonedByGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_SUMMONEDBY, GUID ); }
    void SetCreatedByGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_CREATEDBY, GUID ); }


    uint64 GetCharmedUnitGUID(){ return GetUInt64Value( UNIT_FIELD_CHARM ); }
    uint64 GetSummonedUnitGUID(){ return GetUInt64Value( UNIT_FIELD_SUMMON ); }
    uint64 GetSummonedCritterGUID(){ return GetUInt64Value( UNIT_FIELD_CRITTER ); }

    uint64 GetCharmedByGUID(){ return GetUInt64Value( UNIT_FIELD_CHARMEDBY ); }
    uint64 GetSummonedByGUID(){ return GetUInt64Value( UNIT_FIELD_SUMMONEDBY ); }
    uint64 GetCreatedByGUID(){ return GetUInt64Value( UNIT_FIELD_CREATEDBY ); }

    void SetTargetGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_TARGET, GUID ); }
    uint64 GetTargetGUID(){ return GetUInt64Value( UNIT_FIELD_TARGET ); }

    void SetChannelSpellTargetGUID( uint64 GUID ){ SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, GUID ); }
    void SetChannelSpellId( uint32 SpellId ){ SetUInt32Value(UNIT_CHANNEL_SPELL, SpellId); }
    
    uint64 GetChannelSpellTargetGUID(){ return GetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT ); }
    uint32 GetChannelSpellId(){ return GetUInt32Value(UNIT_CHANNEL_SPELL); }

	void SetEquippedItem( uint8 slot, uint32 id ) { SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+slot, id); }
	uint32 GetEquippedItem( uint8 slot ) { return GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+slot); }

	void SetBaseAttackTime( uint8 slot, uint32 time ) { SetUInt32Value(UNIT_FIELD_BASEATTACKTIME+slot, time); }
	uint32 GetBaseAttackTime( uint8 slot ) { return GetUInt32Value(UNIT_FIELD_BASEATTACKTIME+slot); }
	void ModBaseAttackTime( uint8 slot, int32 mod ) { ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME+slot, mod); }

	void SetBoundingRadius( float rad ) { SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, rad); }
	float GetBoundingRadius() { return GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); }

	void SetCombatReach( float len ) { SetFloatValue(UNIT_FIELD_COMBATREACH, len); }
	float GetCombatReach() { return GetFloatValue(UNIT_FIELD_COMBATREACH); }

	void SetDisplayId( uint32 id ) { SetUInt32Value(UNIT_FIELD_DISPLAYID, id); }
	uint32 GetDisplayId() { return GetUInt32Value(UNIT_FIELD_DISPLAYID); }

	void SetNativeDisplayId( uint32 id ) { SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, id); }
	uint32 GetNativeDisplayId() { return GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID); }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetMinDamage( float amt ) { SetFloatValue(UNIT_FIELD_MINDAMAGE, amt); }
	float GetMinDamage() { return GetFloatValue(UNIT_FIELD_MINDAMAGE); }

	void SetMaxDamage( float amt ) { SetFloatValue(UNIT_FIELD_MAXDAMAGE, amt); }
	float GetMaxDamage() { return GetFloatValue(UNIT_FIELD_MAXDAMAGE); }

	void SetMinOffhandDamage( float amt ) { SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, amt); }
	float GetMinOffhandDamage() { return GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE); }

	void SetMaxOffhandDamage( float amt ) { SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, amt); }
	float GetMaxOffhandDamage() { return GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE); }

	void SetMinRangedDamage( float amt ) { SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, amt); }
	float GetMinRangedDamage() { return GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE); }

	void SetMaxRangedDamage( float amt ) { SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, amt); }
	float GetMaxRangedDamage() { return GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE); }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetMount( uint32 id ) { SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, id); }
	uint32 GetMount() { return GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID); }

	void SetCastSpeedMod( float amt ) { SetFloatValue(UNIT_MOD_CAST_SPEED, amt); }
	float GetCastSpeedMod() { return GetFloatValue(UNIT_MOD_CAST_SPEED); }
	void ModCastSpeedMod( float mod ) { ModFloatValue(UNIT_MOD_CAST_SPEED, mod); }

	void SetCreatedBySpell( uint32 id ) { SetUInt32Value(UNIT_CREATED_BY_SPELL, id); }
	uint32 GetCreatedBySpell() { return GetUInt32Value(UNIT_CREATED_BY_SPELL); }

	void SetEmoteState( uint32 id ) { SetUInt32Value(UNIT_NPC_EMOTESTATE, id); }
	uint32 GetEmoteState() { return GetUInt32Value(UNIT_NPC_EMOTESTATE); }

	void SetStat( uint32 stat, uint32 amt ) { SetUInt32Value(UNIT_FIELD_STAT0+stat, amt); }
	uint32 GetStat( uint32 stat ) { return GetUInt32Value(UNIT_FIELD_STAT0+stat); }

	void SetResistance( uint32 type, uint32 amt ) { SetUInt32Value(UNIT_FIELD_RESISTANCES+type, amt); }
	uint32 GetResistance( uint32 type ) { return GetUInt32Value(UNIT_FIELD_RESISTANCES+type); }

	void SetBaseMana( uint32 amt ) { SetUInt32Value(UNIT_FIELD_BASE_MANA, amt); }
	uint32 GetBaseMana() { return GetUInt32Value(UNIT_FIELD_BASE_MANA); }

	void SetBaseHealth( uint32 amt ) { SetUInt32Value(UNIT_FIELD_BASE_HEALTH, amt); }
	uint32 GetBaseHealth() { return GetUInt32Value(UNIT_FIELD_BASE_HEALTH); }

	void SetPowerCostMultiplier( uint32 school, float amt ) { SetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+school, amt); }
	void ModPowerCostMultiplier( uint32 school, float amt ) { ModFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+school, amt); }
	float GetPowerCostMultiplier( uint32 school ) { return GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+school); }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetAttackPower( uint32 amt ) { SetUInt32Value(UNIT_FIELD_ATTACK_POWER, amt); }
	uint32 GetAttackPower() { return GetUInt32Value(UNIT_FIELD_ATTACK_POWER); }

	void SetAttackPowerMods( uint32 amt ) { SetUInt32Value(UNIT_FIELD_ATTACK_POWER_MODS, amt); }
	uint32 GetAttackPowerMods() { return GetUInt32Value(UNIT_FIELD_ATTACK_POWER_MODS); }
	void ModAttackPowerMods( uint32 amt ) { ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, amt); }

	void SetAttackPowerMultiplier( float amt ) { SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, amt); }
	float GetAttackPowerMultiplier() { return GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER); }
	void ModAttackPowerMultiplier( float amt ) { ModFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, amt); }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetRangedAttackPower( uint32 amt ) { SetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, amt); }
	uint32 GetRangedAttackPower() { return GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER); }

	void SetRangedAttackPowerMods( uint32 amt ) { SetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, amt); }
	uint32 GetRangedAttackPowerMods() { return GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS); }
	void ModRangedAttackPowerMods( uint32 amt ) { ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, amt); }

	void SetRangedAttackPowerMultiplier( float amt ) { SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, amt); }
	float GetRangedAttackPowerMultiplier() { return GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER); }
	void ModRangedAttackPowerMultiplier( float amt ) { ModFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, amt); }
    
    //////////////////////////////////////////////////// bytes 0 //////////////////////////////////////////////////////
    
    void setRace( uint8 race ){ SetByte(UNIT_FIELD_BYTES_0, 0, race ); }
    uint8 getRace() { return GetByte(UNIT_FIELD_BYTES_0, 0 ); }
    
    void setClass( uint8 class_ ){ SetByte( UNIT_FIELD_BYTES_0, 1, class_ ); }
	uint8 getClass(){ return GetByte(UNIT_FIELD_BYTES_0, 1 ); }
    
    uint8 getGender(){ return GetByte(UNIT_FIELD_BYTES_0,2 ); }
	void setGender( uint8 gender ){ SetByte(UNIT_FIELD_BYTES_0, 2, gender ); }
    
    void SetPowerType( uint8 type ){ SetByte(UNIT_FIELD_BYTES_0, 3, type ); }
    uint8 GetPowerType(){ return GetByte( UNIT_FIELD_BYTES_0, 3 ); }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void SetHealth( uint32 val ) { SetUInt32Value( UNIT_FIELD_HEALTH,  val ); } 
    void SetMaxHealth( uint32 val ) { SetUInt32Value( UNIT_FIELD_MAXHEALTH,  val ); } 
    uint32 GetHealth()    const { return GetUInt32Value( UNIT_FIELD_HEALTH ); }
    uint32 GetMaxHealth() const { return GetUInt32Value( UNIT_FIELD_MAXHEALTH ); }
	void ModHealth( int32 val ) { ModUnsigned32Value( UNIT_FIELD_HEALTH, val ); }
	void ModMaxHealth( int32 val ) { ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, val ); }

    void SetPower(uint32 type, int32 value);

    void ModPower( uint32 index, int32 value ){
        int32 power = static_cast< int32 >( m_uint32Values[ UNIT_FIELD_POWER1 + index ] );
        int32 maxpower = static_cast< int32 >( m_uint32Values[ UNIT_FIELD_MAXPOWER1 + index ] );

        if( value <= power )
            SetUInt32Value( UNIT_FIELD_POWER1 + index, 0 ); 
        else
            SetUInt32Value( UNIT_FIELD_POWER1 + index, power + value ); 

        if( ( value + power ) > maxpower )
            SetUInt32Value( UNIT_FIELD_POWER1 + index, maxpower ); 
        else
            SetUInt32Value( UNIT_FIELD_POWER1 + index, power + value ); 
    }

    uint32 GetPower( uint32 index ){ return GetUInt32Value( UNIT_FIELD_POWER1 + index ); }
    
    void SetMaxPower( uint32 index, uint32 value ){ SetUInt32Value( UNIT_FIELD_MAXPOWER1 + index, value ); }
    
    void ModMaxPower( uint32 index, int32 value ){ ModUnsigned32Value(UNIT_FIELD_MAXPOWER1 + index, value);
    }

    uint32 GetMaxPower( uint32 index ){ return GetUInt32Value( UNIT_FIELD_MAXPOWER1 + index ); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void TakeDamage(Unit *pAttacker, uint32 damage, uint32 spellid, bool no_remove_auras = false );
	virtual void Die( Unit *pAttacker, uint32 damage, uint32 spellid );
	virtual bool isCritter(){ return false; }

	void AddGarbagePet( Pet *pet );

	//******************************************************
	// Auras that can affect only one target at a time
	//******************************************************
	uint64 GetCurrentUnitForSingleTargetAura(SpellEntry* spell);
	uint64 GetCurrentUnitForSingleTargetAura(uint32* name_hashes, uint32* index);
	void SetCurrentUnitForSingleTargetAura(SpellEntry* spell, uint64 guid);
	void RemoveCurrentUnitForSingleTargetAura(SpellEntry* spell);
	void RemoveCurrentUnitForSingleTargetAura(uint32 name_hash);

protected:
	Unit ();
    void RemoveGarbage();
    void AddGarbageAura( Aura *aur );
    void AddGarbageSpell( Spell *sp );

	uint32 m_meleespell;
	uint8 m_meleespell_ecn; // extra_cast_number
	void _UpdateSpells(uint32 time);

	uint16 m_H_regenTimer;
	uint16 m_P_regenTimer;
	uint32 m_interruptedRegenTime; //PowerInterruptedegenTimer.
	uint32 m_state;		 // flags for keeping track of some states
	uint32 m_attackTimer;   // timer for attack
	uint32 m_attackTimer_1;
	bool m_dualWield;

    std::list< Aura* > m_GarbageAuras;
    std::list< Spell* > m_GarbageSpells;
	std::list< Pet* > m_GarbagePets;

	/// Combat
	DeathState m_deathState;

	// Stealth
	uint32 m_stealthLevel;
	uint32 m_stealthDetectBonus;	
	
	// DK:pet

	// AI
	AIInterface *m_aiInterface;
	bool m_useAI;
	bool can_parry;//will be enabled by block spell
	int32 m_threatModifyer;
	int32 m_generatedThreatModifyer[SCHOOL_COUNT];

	int32 m_manashieldamt;
	uint32 m_manaShieldId;

	// Quest emote
	uint8 m_emoteState;
	uint32 m_oldEmote;

	// Some auras can only be cast on one target at a time
	// This will map aura spell id to target guid
	UniqueAuraTargetMap m_singleTargetAura;

	uint32 m_charmtemp;

	bool m_extraAttackCounter;

	float m_modelhalfsize; // used to calculate if something is in range of this unit

	float m_blockfromspell;
	float m_dodgefromspell;
	float m_parryfromspell;
	uint32 m_BlockModPct; // is % but does not need float and does not need /100!


public:
	//Andy's crap

	bool InParty(Unit* u);
	bool InRaid(Unit* u);
};


#endif

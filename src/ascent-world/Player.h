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

#ifndef _PLAYER_H
#define _PLAYER_H
struct BGScore;
class Channel;
class Creature;
class Battleground;
class TaxiPath;
class GameObject;
class Transporter;
class Corpse;
class Guild;
struct GuildRank;
class Pet;
class Charter;
class LFGMatch;
struct LevelInfo;
class SpeedDetector;
#define MAX_PET_NO 3
#define PLAYER_NORMAL_RUN_SPEED 7.0f
#define PLAYER_NORMAL_SWIM_SPEED 4.722222f
#define PLAYER_NORMAL_FLIGHT_SPEED 7.0f
#define PLAYER_HONORLESS_TARGET_SPELL 2479
#define MONSTER_NORMAL_RUN_SPEED 8.0f
/* action button defines */
#define PLAYER_ACTION_BUTTON_COUNT 132
#define PLAYER_ACTION_BUTTON_SIZE PLAYER_ACTION_BUTTON_COUNT * sizeof(ActionButton)
//====================================================================
//  Inventory
//  Holds the display id and item type id for objects in
//  a character's inventory
//====================================================================

enum Classes
{
	WARRIOR = 1,
	PALADIN = 2,
	HUNTER = 3,
	ROGUE = 4,
	PRIEST = 5,
	SHAMAN = 7,
	MAGE = 8,
	WARLOCK = 9,
	DRUID = 11,
};

enum Races
{
	RACE_HUMAN = 1,
	RACE_ORC = 2,
	RACE_DWARF = 3,
	RACE_NIGHTELF = 4,
	RACE_UNDEAD = 5,
	RACE_TAUREN = 6,
	RACE_GNOME = 7,
	RACE_TROLL = 8,
	RACE_BLOODELF = 10,
	RACE_DRAENEI = 11,
};

enum PlayerStatus
{
	NONE			 = 0,
	TRANSFER_PENDING = 1,
};

enum RankTitles
{
	PVPTITLE_NONE                   = 0x0,
	PVPTITLE_PRIVATE                = 0x02,
	PVPTITLE_CORPORAL               = 0x04,
	PVPTITLE_SERGEANT               = 0x08,
	PVPTITLE_MASTER_SERGEANT        = 0x10,
	PVPTITLE_SERGEANT_MAJOR         = 0x20,
	PVPTITLE_KNIGHT                 = 0x40,
	PVPTITLE_KNIGHT_LIEUTENANT      = 0x80,
	PVPTITLE_KNIGHT_CAPTAIN         = 0x100,
	PVPTITLE_KNIGHT_CHAMPION        = 0x200,
	PVPTITLE_LIEUTENANT_COMMANDER   = 0x400,
	PVPTITLE_COMMANDER              = 0x800,
	PVPTITLE_MARSHAL                = 0x1000,
	PVPTITLE_FIELD_MARSHAL          = 0x2000,
	PVPTITLE_GRAND_MARSHAL          = 0x4000,
	PVPTITLE_SCOUT                  = 0x8000,
	PVPTITLE_GRUNT                  = 0x10000,
	PVPTITLE_HSERGEANT              = 0x20000,
	PVPTITLE_SENIOR_SERGEANT        = 0x40000,
	PVPTITLE_FIRST_SERGEANT         = 0x80000,
	PVPTITLE_STONE_GUARD            = 0x100000,
	PVPTITLE_BLOOD_GUARD            = 0x200000,
	PVPTITLE_LEGIONNAIRE            = 0x400000,
	PVPTITLE_CENTURION              = 0x800000,
	PVPTITLE_CHAMPION               = 0x1000000,
	PVPTITLE_LIEUTENANT_GENERAL     = 0x2000000,
	PVPTITLE_GENERAL                = 0x4000000,
	PVPTITLE_WARLORD                = 0x8000000,
	PVPTITLE_HIGH_WARLORD           = 0x10000000,
};

enum PvPAreaStatus
{
    AREA_ALLIANCE = 1,
    AREA_HORDE = 2,
    AREA_CONTESTED = 3,
    AREA_PVPARENA = 4,
};

enum PlayerMovementType
{
    MOVE_ROOT	    = 1,
    MOVE_UNROOT	    = 2,
    MOVE_WATER_WALK = 3,
    MOVE_LAND_WALK  = 4,
};

enum PlayerSpeedType
{
    RUN	            = 1,
    RUNBACK         = 2,
    SWIM	        = 3,
    SWIMBACK        = 4,
    WALK	        = 5,
    FLY	            = 6,
};

enum Standing
{
    STANDING_HATED,
    STANDING_HOSTILE,
    STANDING_UNFRIENDLY,
    STANDING_NEUTRAL,
    STANDING_FRIENDLY,
    STANDING_HONORED,
    STANDING_REVERED,
    STANDING_EXALTED
};

enum ReputationRank			// scriptdev2
{
    REP_HATED       = 0,
    REP_HOSTILE     = 1,
    REP_UNFRIENDLY  = 2,
    REP_NEUTRAL     = 3,
    REP_FRIENDLY    = 4,
    REP_HONORED     = 5,
    REP_REVERED     = 6,
    REP_EXALTED     = 7
};


enum PlayerFlags
{
    PLAYER_FLAG_PARTY_LEADER		= 0x01,
    PLAYER_FLAG_AFK					= 0x02,
    PLAYER_FLAG_DND					= 0x04,
    PLAYER_FLAG_GM					= 0x08,
    PLAYER_FLAG_DEATH_WORLD_ENABLE  = 0x10,
    PLAYER_FLAG_RESTING				= 0x20,
    PLAYER_FLAG_UNKNOWN1            = 0x40,
    PLAYER_FLAG_FREE_FOR_ALL_PVP	= 0x80,
    PLAYER_FLAG_UNKNOWN2            = 0x100,
    PLAYER_FLAG_PVP_TOGGLE			= 0x200,
    PLAYER_FLAG_NOHELM				= 0x400,
    PLAYER_FLAG_NOCLOAK				= 0x800,
    PLAYER_FLAG_NEED_REST_3_HOURS	= 0x1000,
    PLAYER_FLAG_NEED_REST_5_HOURS	= 0x2000,
};

enum CharterTypes
{
	CHARTER_TYPE_GUILD			= 0,
	CHARTER_TYPE_ARENA_2V2		= 1,
	CHARTER_TYPE_ARENA_3V3		= 2,
	CHARTER_TYPE_ARENA_5V5		= 3,
	NUM_CHARTER_TYPES			= 4,
};

enum ArenaTeamTypes
{
	ARENA_TEAM_TYPE_2V2			= 0,
	ARENA_TEAM_TYPE_3V3			= 1,
	ARENA_TEAM_TYPE_5V5			= 2,
	NUM_ARENA_TEAM_TYPES		= 3,
};

enum CooldownTypes
{
	COOLDOWN_TYPE_SPELL			= 0,
	COOLDOWN_TYPE_CATEGORY		= 1,
	NUM_COOLDOWN_TYPES,
};

enum LootType
{
    LOOT_CORPSE                 = 1,
    LOOT_SKINNING               = 2,
    LOOT_FISHING                = 3,
    LOOT_PICKPOCKETING          = 2,                        // 4 unsupported by client, sending LOOT_SKINNING instead
    LOOT_DISENCHANTING          = 2,                        // 5 unsupported by client, sending LOOT_SKINNING instead
    LOOT_PROSPECTING            = 2,                        // 6 unsupported by client, sending LOOT_SKINNING instead
    LOOT_INSIGNIA               = 2                         // 7 unsupported by client, sending LOOT_SKINNING instead
};

struct spells
{
	uint16  spellId;
	uint16  slotId;
};

#pragma pack(push,1)
struct ActionButton
{
	uint16  Action;
	uint8   Type;
	uint8   Misc;
};
#pragma pack(pop)

struct CreateInfo_ItemStruct
{
	uint32  protoid;
	uint8   slot;
	uint32  amount;
};

struct CreateInfo_SkillStruct
{
	uint32  skillid;
	uint32  currentval;
	uint32  maxval;
};

struct CreateInfo_ActionBarStruct
{
	uint32  button;
	uint32  action;
	uint32  type;
	uint32  misc;
};

struct PlayerCreateInfo{
	uint8   index;
	uint8   race;
	uint32  factiontemplate;
	uint8   class_;
	uint32  mapId;
	uint32  zoneId;
	float   positionX;
	float   positionY;
	float   positionZ;
	uint16  displayId;
	uint8   strength;
	uint8   ability;
	uint8   stamina;
	uint8   intellect;
	uint8   spirit;
	uint32  health;
	uint32  mana;
	uint32  rage;
	uint32  focus;
	uint32  energy;
	uint32  attackpower;
	float   mindmg;
	float   maxdmg;
	std::list<CreateInfo_ItemStruct> items;
	std::list<CreateInfo_SkillStruct> skills;
	std::list<CreateInfo_ActionBarStruct> actionbars;
	std::set<uint32> spell_list;
	//uint32 item[10];
	//uint8 item_slot[10];
	//uint16 spell[10];
};

struct DamageSplit
{
	Player* caster;
	Aura*   aura;
	uint32  miscVal;
	union
	{
		uint32 damage;
		float damagePCT;
	};
};

struct LoginAura
{
	uint32 id;
	uint32 dur;
	bool positive;
	uint32 charges;
};

// Dodge ( class base ) - UNUSED, Warrior, Paladin, Hunter, Rogue, Priest, UNUSED, Shaman, Mage, Warlock, UNUSED, Druid
const float baseDodge[12] = { 0.0f, 0.7580f, 0.6520f, -5.4500f, -0.5900f, 3.1830f, 0.0f, 1.6750f, 3.4575f, 2.0350f, 0.0f, -1.8720f };

// Dodge ( class ratio ) - UNUSED, Warrior, Paladin, Hunter, Rogue, Priest, UNUSED, Shaman, Mage, Warlock, UNUSED, Druid
// TODO: get proper ratios for all levels, we only have values for level 70 currently
const float dodgeRatio[70][12] = {
{0.000000f , 5.000000f , 5.000000f , 5.000000f , 5.000000f , 5.000000f , 0.000000f , 5.000000f , 5.000000f , 5.000000f , 0.000000f , 5.000000f , } , // Level 1 
{0.000000f , 5.000000f , 5.000000f , 5.000000f , 5.000000f , 5.000000f , 0.000000f , 5.000000f , 5.000000f , 5.000000f , 0.000000f , 5.000000f , } , // Level 2 
{0.000000f , 5.157590f , 5.000000f , 5.000000f , 5.000000f , 5.000000f , 0.000000f , 5.000000f , 5.000000f , 5.000000f , 0.000000f , 5.000000f , } , // Level 3 
{0.000000f , 6.017188f , 5.014323f , 5.014323f , 5.000000f , 5.014323f , 0.000000f , 5.014323f , 5.014323f , 5.014323f , 0.000000f , 5.000000f , } , // Level 4 
{0.000000f , 6.793599f , 5.661333f , 5.661333f , 5.000000f , 5.661333f , 0.000000f , 5.661333f , 5.661333f , 5.661333f , 0.000000f , 5.000000f , } , // Level 5 
{0.000000f , 7.508715f , 6.257262f , 6.257262f , 5.005810f , 6.257262f , 0.000000f , 6.257262f , 6.257262f , 6.257262f , 0.000000f , 5.000000f , } , // Level 6 
{0.000000f , 8.176156f , 6.813463f , 6.813463f , 5.450771f , 6.813463f , 0.000000f , 6.813463f , 6.813463f , 6.813463f , 0.000000f , 5.000000f , } , // Level 7 
{0.000000f , 8.805091f , 7.337576f , 7.337576f , 5.870061f , 7.337576f , 0.000000f , 7.337576f , 7.337576f , 7.337576f , 0.000000f , 5.000000f , } , // Level 8 
{0.000000f , 9.402047f , 7.835039f , 7.835039f , 6.268031f , 7.835039f , 0.000000f , 7.835039f , 7.835039f , 7.835039f , 0.000000f , 5.000000f , } , // Level 9 
{0.000000f , 9.971868f , 8.309890f , 8.309890f , 6.647912f , 8.309890f , 0.000000f , 8.309890f , 8.309890f , 8.309890f , 0.000000f , 5.000000f , } , // Level 10 
{0.000000f , 10.518271f , 8.765226f , 8.765226f , 7.012181f , 8.765226f , 0.000000f , 8.765226f , 8.765226f , 8.765226f , 0.000000f , 5.156022f , } , // Level 11 
{0.000000f , 11.044185f , 9.203487f , 9.203487f , 7.362790f , 9.203487f , 0.000000f , 9.203487f , 9.203487f , 9.203487f , 0.000000f , 5.413823f , } , // Level 12 
{0.000000f , 11.551964f , 9.626636f , 9.626636f , 7.701309f , 9.626636f , 0.000000f , 9.626636f , 9.626636f , 9.626636f , 0.000000f , 5.662734f , } , // Level 13 
{0.000000f , 12.043536f , 10.036280f , 10.036280f , 8.029024f , 10.036280f , 0.000000f , 10.036280f , 10.036280f , 10.036280f , 0.000000f , 5.903701f , } , // Level 14 
{0.000000f , 12.520508f , 10.433757f , 10.433757f , 8.347005f , 10.433757f , 0.000000f , 10.433757f , 10.433757f , 10.433757f , 0.000000f , 6.137511f , } , // Level 15 
{0.000000f , 12.984231f , 10.820192f , 10.820192f , 8.656154f , 10.820192f , 0.000000f , 10.820192f , 10.820192f , 10.820192f , 0.000000f , 6.364827f , } , // Level 16 
{0.000000f , 13.435856f , 11.196547f , 11.196547f , 8.957237f , 11.196547f , 0.000000f , 11.196547f , 11.196547f , 11.196547f , 0.000000f , 6.586212f , } , // Level 17 
{0.000000f , 13.876376f , 11.563647f , 11.563647f , 9.250917f , 11.563647f , 0.000000f , 11.563647f , 11.563647f , 11.563647f , 0.000000f , 6.802153f , } , // Level 18 
{0.000000f , 14.306651f , 11.922209f , 11.922209f , 9.537767f , 11.922209f , 0.000000f , 11.922209f , 11.922209f , 11.922209f , 0.000000f , 7.013073f , } , // Level 19 
{0.000000f , 14.727435f , 12.272863f , 12.272863f , 9.818290f , 12.272863f , 0.000000f , 12.272863f , 12.272863f , 12.272863f , 0.000000f , 7.219340f , } , // Level 20 
{0.000000f , 15.139391f , 12.616159f , 12.616159f , 10.092928f , 12.616159f , 0.000000f , 12.616159f , 12.616159f , 12.616159f , 0.000000f , 7.421279f , } , // Level 21 
{0.000000f , 15.543108f , 12.952590f , 12.952590f , 10.362072f , 12.952590f , 0.000000f , 12.952590f , 12.952590f , 12.952590f , 0.000000f , 7.619180f , } , // Level 22 
{0.000000f , 15.939111f , 13.282593f , 13.282593f , 10.626074f , 13.282593f , 0.000000f , 13.282593f , 13.282593f , 13.282593f , 0.000000f , 7.813299f , } , // Level 23 
{0.000000f , 16.327870f , 13.606558f , 13.606558f , 10.885247f , 13.606558f , 0.000000f , 13.606558f , 13.606558f , 13.606558f , 0.000000f , 8.003867f , } , // Level 24 
{0.000000f , 16.709808f , 13.924840f , 13.924840f , 11.139872f , 13.924840f , 0.000000f , 13.924840f , 13.924840f , 13.924840f , 0.000000f , 8.191092f , } , // Level 25 
{0.000000f , 17.085310f , 14.237758f , 14.237758f , 11.390207f , 14.237758f , 0.000000f , 14.237758f , 14.237758f , 14.237758f , 0.000000f , 8.375162f , } , // Level 26 
{0.000000f , 17.454722f , 14.545602f , 14.545602f , 11.636481f , 14.545602f , 0.000000f , 14.545602f , 14.545602f , 14.545602f , 0.000000f , 8.556246f , } , // Level 27 
{0.000000f , 17.818362f , 14.848635f , 14.848635f , 11.878908f , 14.848635f , 0.000000f , 14.848635f , 14.848635f , 14.848635f , 0.000000f , 8.734502f , } , // Level 28 
{0.000000f , 18.176520f , 15.147100f , 15.147100f , 12.117680f , 15.147100f , 0.000000f , 15.147100f , 15.147100f , 15.147100f , 0.000000f , 8.910069f , } , // Level 29 
{0.000000f , 18.529462f , 15.441218f , 15.441218f , 12.352975f , 15.441218f , 0.000000f , 15.441218f , 15.441218f , 15.441218f , 0.000000f , 9.083081f , } , // Level 30 
{0.000000f , 18.877433f , 15.731194f , 15.731194f , 12.584955f , 15.731194f , 0.000000f , 15.731194f , 15.731194f , 15.731194f , 0.000000f , 9.253655f , } , // Level 31 
{0.000000f , 19.220659f , 16.017216f , 16.017216f , 12.813773f , 16.017216f , 0.000000f , 16.017216f , 16.017216f , 16.017216f , 0.000000f , 9.421903f , } , // Level 32 
{0.000000f , 19.559349f , 16.299458f , 16.299458f , 13.039566f , 16.299458f , 0.000000f , 16.299458f , 16.299458f , 16.299458f , 0.000000f , 9.587928f , } , // Level 33 
{0.000000f , 19.893697f , 16.578081f , 16.578081f , 13.262465f , 16.578081f , 0.000000f , 16.578081f , 16.578081f , 16.578081f , 0.000000f , 9.751824f , } , // Level 34 
{0.000000f , 20.223883f , 16.853236f , 16.853236f , 13.482589f , 16.853236f , 0.000000f , 16.853236f , 16.853236f , 16.853236f , 0.000000f , 9.913680f , } , // Level 35 
{0.000000f , 20.550075f , 17.125062f , 17.125062f , 13.700050f , 17.125062f , 0.000000f , 17.125062f , 17.125062f , 17.125062f , 0.000000f , 10.073578f , } , // Level 36 
{0.000000f , 20.872429f , 17.393691f , 17.393691f , 13.914953f , 17.393691f , 0.000000f , 17.393691f , 17.393691f , 17.393691f , 0.000000f , 10.231595f , } , // Level 37 
{0.000000f , 21.191092f , 17.659243f , 17.659243f , 14.127395f , 17.659243f , 0.000000f , 17.659243f , 17.659243f , 17.659243f , 0.000000f , 10.387803f , } , // Level 38 
{0.000000f , 21.506201f , 17.921834f , 17.921834f , 14.337467f , 17.921834f , 0.000000f , 17.921834f , 17.921834f , 17.921834f , 0.000000f , 10.542268f , } , // Level 39 
{0.000000f , 21.817885f , 18.181571f , 18.181571f , 14.545257f , 18.181571f , 0.000000f , 18.181571f , 18.181571f , 18.181571f , 0.000000f , 10.695055f , } , // Level 40 
{0.000000f , 22.126265f , 18.438554f , 18.438554f , 14.750843f , 18.438554f , 0.000000f , 18.438554f , 18.438554f , 18.438554f , 0.000000f , 10.846221f , } , // Level 41 
{0.000000f , 22.431455f , 18.692879f , 18.692879f , 14.954303f , 18.692879f , 0.000000f , 18.692879f , 18.692879f , 18.692879f , 0.000000f , 10.995824f , } , // Level 42 
{0.000000f , 22.733562f , 18.944635f , 18.944635f , 15.155708f , 18.944635f , 0.000000f , 18.944635f , 18.944635f , 18.944635f , 0.000000f , 11.143916f , } , // Level 43 
{0.000000f , 23.032688f , 19.193907f , 19.193907f , 15.355125f , 19.193907f , 0.000000f , 19.193907f , 19.193907f , 19.193907f , 0.000000f , 11.290547f , } , // Level 44 
{0.000000f , 23.328928f , 19.440774f , 19.440774f , 15.552619f , 19.440774f , 0.000000f , 19.440774f , 19.440774f , 19.440774f , 0.000000f , 11.435763f , } , // Level 45 
{0.000000f , 23.622374f , 19.685312f , 19.685312f , 15.748249f , 19.685312f , 0.000000f , 19.685312f , 19.685312f , 19.685312f , 0.000000f , 11.579609f , } , // Level 46 
{0.000000f , 23.913111f , 19.927592f , 19.927592f , 15.942074f , 19.927592f , 0.000000f , 19.927592f , 19.927592f , 19.927592f , 0.000000f , 11.722127f , } , // Level 47 
{0.000000f , 24.201221f , 20.167684f , 20.167684f , 16.134147f , 20.167684f , 0.000000f , 20.167684f , 20.167684f , 20.167684f , 0.000000f , 11.863358f , } , // Level 48 
{0.000000f , 24.486781f , 20.405651f , 20.405651f , 16.324520f , 20.405651f , 0.000000f , 20.405651f , 20.405651f , 20.405651f , 0.000000f , 12.003338f , } , // Level 49 
{0.000000f , 24.769865f , 20.641554f , 20.641554f , 16.513243f , 20.641554f , 0.000000f , 20.641554f , 20.641554f , 20.641554f , 0.000000f , 12.142105f , } , // Level 50 
{0.000000f , 25.050543f , 20.875453f , 20.875453f , 16.700362f , 20.875453f , 0.000000f , 20.875453f , 20.875453f , 20.875453f , 0.000000f , 12.279693f , } , // Level 51 
{0.000000f , 25.328883f , 21.107402f , 21.107402f , 16.885922f , 21.107402f , 0.000000f , 21.107402f , 21.107402f , 21.107402f , 0.000000f , 12.416134f , } , // Level 52 
{0.000000f , 25.604947f , 21.337456f , 21.337456f , 17.069964f , 21.337456f , 0.000000f , 21.337456f , 21.337456f , 21.337456f , 0.000000f , 12.551460f , } , // Level 53 
{0.000000f , 25.878796f , 21.565664f , 21.565664f , 17.252531f , 21.565664f , 0.000000f , 21.565664f , 21.565664f , 21.565664f , 0.000000f , 12.685700f , } , // Level 54 
{0.000000f , 26.150490f , 21.792075f , 21.792075f , 17.433660f , 21.792075f , 0.000000f , 21.792075f , 21.792075f , 21.792075f , 0.000000f , 12.818883f , } , // Level 55 
{0.000000f , 26.420082f , 22.016735f , 22.016735f , 17.613388f , 22.016735f , 0.000000f , 22.016735f , 22.016735f , 22.016735f , 0.000000f , 12.951036f , } , // Level 56 
{0.000000f , 26.687628f , 22.239690f , 22.239690f , 17.791752f , 22.239690f , 0.000000f , 22.239690f , 22.239690f , 22.239690f , 0.000000f , 13.082186f , } , // Level 57 
{0.000000f , 26.953176f , 22.460980f , 22.460980f , 17.968784f , 22.460980f , 0.000000f , 22.460980f , 22.460980f , 22.460980f , 0.000000f , 13.212357f , } , // Level 58 
{0.000000f , 27.216777f , 22.680647f , 22.680647f , 18.144518f , 22.680647f , 0.000000f , 22.680647f , 22.680647f , 22.680647f , 0.000000f , 13.341573f , } , // Level 59 
{0.000000f , 27.478477f , 22.898731f , 22.898731f , 18.318984f , 22.898731f , 0.000000f , 22.898731f , 22.898731f , 22.898731f , 0.000000f , 13.469858f , } , // Level 60 
{0.000000f , 27.738320f , 23.115267f , 23.115267f , 18.492214f , 23.115267f , 0.000000f , 23.115267f , 23.115267f , 23.115267f , 0.000000f , 13.597232f , } , // Level 61 
{0.000000f , 27.996351f , 23.330293f , 23.330293f , 18.664234f , 23.330293f , 0.000000f , 23.330293f , 23.330293f , 23.330293f , 0.000000f , 13.723718f , } , // Level 62 
{0.000000f , 28.252611f , 23.543842f , 23.543842f , 18.835074f , 23.543842f , 0.000000f , 23.543842f , 23.543842f , 23.543842f , 0.000000f , 13.849336f , } , // Level 63 
{0.000000f , 28.507139f , 23.755949f , 23.755949f , 19.004759f , 23.755949f , 0.000000f , 23.755949f , 23.755949f , 23.755949f , 0.000000f , 13.974104f , } , // Level 64 
{0.000000f , 28.759974f , 23.966645f , 23.966645f , 19.173316f , 23.966645f , 0.000000f , 23.966645f , 23.966645f , 23.966645f , 0.000000f , 14.098043f , } , // Level 65 
{0.000000f , 29.011153f , 24.175960f , 24.175960f , 19.340768f , 24.175960f , 0.000000f , 24.175960f , 24.175960f , 24.175960f , 0.000000f , 14.221170f , } , // Level 66 
{0.000000f , 29.260711f , 24.383926f , 24.383926f , 19.507141f , 24.383926f , 0.000000f , 24.383926f , 24.383926f , 24.383926f , 0.000000f , 14.343503f , } , // Level 67 
{0.000000f , 29.508683f , 24.590569f , 24.590569f , 19.672455f , 24.590569f , 0.000000f , 24.590569f , 24.590569f , 24.590569f , 0.000000f , 14.465058f , } , // Level 68 
{0.000000f , 29.755102f , 24.795918f , 24.795918f , 19.836735f , 24.795918f , 0.000000f , 24.795918f , 24.795918f , 24.795918f , 0.000000f , 14.585852f , } , // Level 69 
{0.000000f , 30.000000f , 25.000000f , 25.000000f , 20.000000f , 25.000000f , 0.000000f , 25.000000f , 25.000000f , 25.000000f , 0.000000f , 14.705900f , } , // Level 70  
};

/*
Exalted	        1,000	 Access to racial mounts. Capped at 999.7
Revered	        21,000	 Heroic mode keys for Outland dungeons
Honored	        12,000	 10% discount from faction vendors
Friendly	    6,000
Neutral	        3,000
Unfriendly	    3,000	 Cannot buy, sell or interact.
Hostile	        3,000	 You will always be attacked on sight
Hated	        36,000 
*/
enum FactionRating
{
	HATED,
	HOSTILE,
	UNFRIENDLY,
	NEUTRAL,
	FRIENDLY,
	HONORED,
	REVERED,
	EXALTED
};
struct FactionReputation
{
	int32 standing;
	uint8 flag;
	int32 baseStanding;
	ASCENT_INLINE int32 CalcStanding() { return standing - baseStanding; }
	ASCENT_INLINE bool Positive() { return standing >= 0; }
};
struct PlayerInfo
{
	~PlayerInfo();
	uint32 guid;
	uint32 acct;
	char * name;
	uint32 race;
	uint32 gender;
	uint32 cl;
	uint32 team;
	
	time_t lastOnline;
	uint32 lastZone;
	uint32 lastLevel;
	Group * m_Group;
	int8 subGroup;
#ifdef VOICE_CHAT
	int8 groupVoiceId;
#endif

	Player * m_loggedInPlayer;
	Guild * guild;
	GuildRank * guildRank;
	GuildMember * guildMember;
};
struct PlayerPet
{
	string name;
	uint32 entry;
	string fields;
	uint32 xp;
	bool active;
	char stablestate;
	uint32 number;
	uint32 level;
	uint32 loyaltyxp;
	uint32 happinessupdate;
	string actionbar;
	bool summon;
	uint32 loyaltypts;
	uint32 loyaltyupdate;
	char loyaltylvl;
	time_t reset_time;
	uint32 reset_cost;
};
enum MeetingStoneQueueStatus
{
	MEETINGSTONE_STATUS_NONE								= 0,
	MEETINGSTONE_STATUS_JOINED_MEETINGSTONE_QUEUE_FOR	   = 1,
	MEETINGSTONE_STATUS_PARTY_MEMBER_LEFT_LFG			   = 2,
	MEETINGSTONE_STATUS_PARTY_MEMBER_REMOVED_PARTY_REMOVED  = 3,
	MEETINGSTONE_STATUS_LOOKING_FOR_NEW_PARTY_IN_QUEUE	  = 4,
	MEETINGSTONE_STATUS_NONE_UNK							= 5,
};
enum ItemPushResultTypes
{
	ITEM_PUSH_TYPE_LOOT			 = 0x00000000,
	ITEM_PUSH_TYPE_RECEIVE		  = 0x00000001,
	ITEM_PUSH_TYPE_CREATE		   = 0x00000002,
};
struct WeaponModifier
{
	uint32 wclass;
	uint32 subclass;
	float value;
};
struct PetActionBar
{
	uint32 spell[10];
};
struct classScriptOverride
{
	uint32 id;
	uint32 effect;
	uint32 aura;
	uint32 damage;
	bool percent;
};
class Spell;
class Item;
class Container;
class WorldSession;
class ItemInterface;
class GossipMenu;
class SpeedCheatDetector;
struct TaxiPathNode;

#define RESTSTATE_RESTED			 1
#define RESTSTATE_NORMAL			 2
#define RESTSTATE_TIRED100		     3
#define RESTSTATE_TIRED50			 4
#define RESTSTATE_EXHAUSTED		     5
#define UNDERWATERSTATE_NONE		 0
#define UNDERWATERSTATE_SWIMMING	 1
#define UNDERWATERSTATE_UNDERWATER   2
#define UNDERWATERSTATE_RECOVERING   4
#define UNDERWATERSTATE_TAKINGDAMAGE 8
#define UNDERWATERSTATE_FATIGUE	     16
#define UNDERWATERSTATE_LAVA		 32
#define UNDERWATERSTATE_SLIME		 64

enum TRADE_STATUS
{
	TRADE_STATUS_PLAYER_BUSY	    = 0x00,
	TRADE_STATUS_PROPOSED		    = 0x01,
	TRADE_STATUS_INITIATED		    = 0x02,
	TRADE_STATUS_CANCELLED		    = 0x03,
	TRADE_STATUS_ACCEPTED		    = 0x04,
	TRADE_STATUS_ALREADY_TRADING    = 0x05,
	TRADE_STATUS_PLAYER_NOT_FOUND   = 0x06,
	TRADE_STATUS_STATE_CHANGED	    = 0x07,
	TRADE_STATUS_COMPLETE		    = 0x08,
	TRADE_STATUS_UNACCEPTED		    = 0x09,
	TRADE_STATUS_TOO_FAR_AWAY	    = 0x0A,
	TRADE_STATUS_WRONG_FACTION	    = 0x0B,
	TRADE_STATUS_FAILED			    = 0x0C,
	TRADE_STATUS_DEAD			    = 0x0D,
	TRADE_STATUS_PETITION		    = 0x0E,
	TRADE_STATUS_PLAYER_IGNORED	    = 0x0F,
};
enum TRADE_DATA
{
	TRADE_GIVE		= 0x00,
	TRADE_RECEIVE	 = 0x01,
};
enum DUEL_STATUS
{
	DUEL_STATUS_OUTOFBOUNDS,
	DUEL_STATUS_INBOUNDS
};
enum DUEL_STATE
{
	DUEL_STATE_REQUESTED,
	DUEL_STATE_STARTED,
	DUEL_STATE_FINISHED
};
enum DUEL_WINNER
{
	DUEL_WINNER_KNOCKOUT,
	DUEL_WINNER_RETREAT,
};
#define PLAYER_ATTACK_TIMEOUT_INTERVAL	5000
#define PLAYER_FORCED_RESURECT_INTERVAL	360000 // 1000*60*6= 6 minutes 

struct PlayerSkill
{
	skilllineentry * Skill;
	uint32 CurrentValue;
	uint32 MaximumValue;
	uint32 BonusValue;
	float GetSkillUpChance();
	void Reset(uint32 Id);
};

enum SPELL_INDEX
{
	SPELL_TYPE_INDEX_MARK			= 1,
	SPELL_TYPE_INDEX_POLYMORPH		= 2,
	SPELL_TYPE_INDEX_FEAR			= 3,
	SPELL_TYPE_INDEX_SAP			= 4,
	SPELL_TYPE_INDEX_SCARE_BEAST	= 5,
	SPELL_TYPE_INDEX_HIBERNATE		= 6,
	SPELL_TYPE_INDEX_EARTH_SHIELD	= 7,
	SPELL_TYPE_INDEX_CYCLONE		= 8,
	SPELL_TYPE_INDEX_BANISH			= 9,
	SPELL_TYPE_INDEX_JUDGEMENT		= 10,	
	NUM_SPELL_TYPE_INDEX			= 11,
};

#define PLAYER_RATING_MODIFIER_RANGED_SKILL						PLAYER_FIELD_COMBAT_RATING_1
#define PLAYER_RATING_MODIFIER_DEFENCE							PLAYER_FIELD_COMBAT_RATING_01
#define PLAYER_RATING_MODIFIER_DODGE							PLAYER_FIELD_COMBAT_RATING_02
#define PLAYER_RATING_MODIFIER_PARRY							PLAYER_FIELD_COMBAT_RATING_03
#define PLAYER_RATING_MODIFIER_BLOCK							PLAYER_FIELD_COMBAT_RATING_04
#define PLAYER_RATING_MODIFIER_MELEE_HIT						PLAYER_FIELD_COMBAT_RATING_05
#define PLAYER_RATING_MODIFIER_RANGED_HIT						PLAYER_FIELD_COMBAT_RATING_06
#define PLAYER_RATING_MODIFIER_SPELL_HIT						PLAYER_FIELD_COMBAT_RATING_07
#define PLAYER_RATING_MODIFIER_MELEE_CRIT						PLAYER_FIELD_COMBAT_RATING_08
#define PLAYER_RATING_MODIFIER_RANGED_CRIT						PLAYER_FIELD_COMBAT_RATING_09
#define PLAYER_RATING_MODIFIER_SPELL_CRIT						PLAYER_FIELD_COMBAT_RATING_10
#define PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE				PLAYER_FIELD_COMBAT_RATING_11 // GUESSED
#define PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE				PLAYER_FIELD_COMBAT_RATING_12 // GUESSED
#define PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE				PLAYER_FIELD_COMBAT_RATING_13 // GUESSED
#define PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE			PLAYER_FIELD_COMBAT_RATING_14
#define PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE			PLAYER_FIELD_COMBAT_RATING_15
#define PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE			PLAYER_FIELD_COMBAT_RATING_16
#define PLAYER_RATING_MODIFIER_MELEE_HASTE						PLAYER_FIELD_COMBAT_RATING_17
#define PLAYER_RATING_MODIFIER_RANGED_HASTE						PLAYER_FIELD_COMBAT_RATING_18
#define PLAYER_RATING_MODIFIER_SPELL_HASTE						PLAYER_FIELD_COMBAT_RATING_19
#define PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL			PLAYER_FIELD_COMBAT_RATING_20
#define PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL				PLAYER_FIELD_COMBAT_RATING_21
// #define UNKNOWN												PLAYER_FIELD_COMBAT_RATING_22
#define PLAYER_RATING_MODIFIER_EXPERTISE						PLAYER_FIELD_COMBAT_RATING_23

class ArenaTeam;
struct PlayerCooldown
{
	uint32 ExpireTime;
	uint32 ItemId;
	uint32 SpellId;
};

//====================================================================
//  Player
//  Class that holds every created character on the server.
//
//  TODO:  Attach characters to user accounts
//====================================================================
typedef std::set<uint32>	                        SpellSet;
typedef std::list<classScriptOverride*>             ScriptOverrideList;
typedef std::set<uint32>                            SaveSet;
typedef std::map<uint64, ByteBuffer*>               SplineMap;
typedef std::map<uint32, ScriptOverrideList* >      SpellOverrideMap;
typedef std::map<uint32, uint32>                    SpellOverrideExtraAuraMap;
typedef std::map<uint32, FactionReputation*>        ReputationMap;
typedef std::map<uint32, uint64>                    SoloSpells;
typedef std::map<SpellEntry*, pair<uint32, uint32> >StrikeSpellMap;
typedef std::map<uint32, OnHitSpell >               StrikeSpellDmgMap;
typedef std::map<uint32, PlayerSkill>				SkillMap;
typedef std::set<Player**>							ReferenceSet;
typedef std::map<uint32, PlayerCooldown>			PlayerCooldownMap;

//#define OPTIMIZED_PLAYER_SAVING

class SERVER_DECL Player : public Unit
{
	friend class WorldSession;
	friend class Pet;
	friend class SkillIterator;

public:

	Player ( uint32 guid );
	~Player ( );

	ASCENT_INLINE Guild * GetGuild() { return m_playerInfo->guild; }
	ASCENT_INLINE GuildMember * GetGuildMember() { return m_playerInfo->guildMember; }
	ASCENT_INLINE GuildRank * GetGuildRankS() { return m_playerInfo->guildRank; }

	void EventGroupFullUpdate();

	/************************************************************************/
	/* Skill System															*/
	/************************************************************************/

	void _AdvanceSkillLine(uint32 SkillLine, uint32 Count = 1);
	void _AddSkillLine(uint32 SkillLine, uint32 Current, uint32 Max);
	uint32 _GetSkillLineMax(uint32 SkillLine);
	uint32 _GetSkillLineCurrent(uint32 SkillLine, bool IncludeBonus = true);
	void _RemoveSkillLine(uint32 SkillLine);
	void _UpdateMaxSkillCounts();
	void _ModifySkillBonus(uint32 SkillLine, int32 Delta);
	void _ModifySkillBonusByType(uint32 SkillType, int32 Delta);
	bool _HasSkillLine(uint32 SkillLine);
	void RemoveSpellsFromLine(uint32 skill_line);
	void _RemoveAllSkills();
	void _RemoveLanguages();
	void _AddLanguages(bool All);
	void _AdvanceAllSkills(uint32 count);
	void _ModifySkillMaximum(uint32 SkillLine, uint32 NewMax);


	void RecalculateHonor();

	LfgMatch * m_lfgMatch;
	uint32 m_lfgInviterGuid;

	void EventTimeoutLfgInviter();

protected:

	void _UpdateSkillFields();

	SkillMap m_skills;


	// COOLDOWNS
	PlayerCooldownMap m_cooldownMap[NUM_COOLDOWN_TYPES];
	uint32 m_globalCooldown;
	
public:
	void Cooldown_AddStart(SpellEntry * pSpell);
	void Cooldown_Add(SpellEntry * pSpell, Item * pItemCaster);
	void Cooldown_AddItem(ItemPrototype * pProto, uint32 x);
	bool Cooldown_CanCast(SpellEntry * pSpell);
	bool Cooldown_CanCast(ItemPrototype * pProto, uint32 x);

protected:
	void _Cooldown_Add(uint32 Type, uint32 Misc, uint32 Time, uint32 SpellId, uint32 ItemId);
	void _LoadPlayerCooldowns(QueryResult * result);
	void _SavePlayerCooldowns(QueryBuffer * buf);

	// END COOLDOWNS

public:

	bool ok_to_remove;
	uint64 m_spellIndexTypeTargets[NUM_SPELL_TYPE_INDEX];
	void OnLogin();//custom stuff on player login.
	void RemoveSpellTargets(uint32 Type);
	void RemoveSpellIndexReferences(uint32 Type);
	void SetSpellTargetType(uint32 Type, Unit* target);
	void SendMeetingStoneQueue(uint32 DungeonId, uint8 Status);

	void AddToWorld();
	void AddToWorld(MapMgr * pMapMgr);
	void RemoveFromWorld();
	bool Create ( WorldPacket &data );
	
	void Update( uint32 time );
	void BuildEnumData( WorldPacket * p_data );
    void BuildFlagUpdateForNonGroupSet(uint32 index, uint32 flag);
	std::string m_afk_reason;
	void SetAFKReason(std::string reason) { m_afk_reason = reason; };
	ASCENT_INLINE const char* GetName() { return m_name.c_str(); }
	ASCENT_INLINE std::string* GetNameString() { return &m_name; }
	void Die();
	//void KilledMonster(uint32 entry, const uint64 &guid);
	void GiveXP(uint32 xp, const uint64 &guid, bool allowbonus);   // to stop rest xp being given
	void ModifyBonuses(uint32 type,int32 val);
	std::map<uint32, uint32> m_wratings;

	ArenaTeam * m_arenaTeams[NUM_ARENA_TEAM_TYPES];
	
    /************************************************************************/
    /* Taxi                                                                 */
    /************************************************************************/
    ASCENT_INLINE TaxiPath*    GetTaxiPath() { return m_CurrentTaxiPath; }
    ASCENT_INLINE bool         GetTaxiState() { return m_onTaxi; }
    const uint32&       GetTaximask( uint8 index ) const { return m_taximask[index]; }
    void                LoadTaxiMask(const char* data);
    void                TaxiStart(TaxiPath* path, uint32 modelid, uint32 start_node);
    void                JumpToEndTaxiNode(TaxiPath * path);
    void                EventDismount(uint32 money, float x, float y, float z);
    void                EventTaxiInterpolate();

    ASCENT_INLINE void         SetTaxiState    (bool state) { m_onTaxi = state; }
    ASCENT_INLINE void         SetTaximask     (uint8 index, uint32 value ) { m_taximask[index] = value; }
    ASCENT_INLINE void         SetTaxiPath     (TaxiPath *path) { m_CurrentTaxiPath = path; }
    ASCENT_INLINE void         SetTaxiPos()	{m_taxi_pos_x = m_position.x; m_taxi_pos_y = m_position.y; m_taxi_pos_z = m_position.z;}
    ASCENT_INLINE void         UnSetTaxiPos()	{m_taxi_pos_x = 0; m_taxi_pos_y = 0; m_taxi_pos_z = 0; }
 
	// Taxi related variables
	vector<TaxiPath*>   m_taxiPaths;
    TaxiPath*           m_CurrentTaxiPath;
    uint32              taxi_model_id;
	uint32              lastNode;
    uint32              m_taxi_ride_time;
    uint32              m_taximask[8];
    float               m_taxi_pos_x;
    float               m_taxi_pos_y;
    float               m_taxi_pos_z;
    bool                m_onTaxi;
	uint32				m_taxiMapChangeNode;

    /************************************************************************/
    /* Quests                                                               */
    /************************************************************************/
	bool HasQuests() 
	{
		for(int i = 0; i < 25; ++i)
		{
			if(m_questlog[i] != 0)
				return true;
		}
		return false;
	}

	int32                GetOpenQuestSlot();
	QuestLogEntry*       GetQuestLogForEntry(uint32 quest);
	ASCENT_INLINE QuestLogEntry*GetQuestLogInSlot(uint32 slot)  { return m_questlog[slot]; }
    ASCENT_INLINE uint32        GetQuestSharer()                { return m_questSharer; }
    
    ASCENT_INLINE void         SetQuestSharer(uint32 guid)     { m_questSharer = guid; }
    void                SetQuestLogSlot(QuestLogEntry *entry, uint32 slot);
    
    ASCENT_INLINE void         PushToRemovedQuests(uint32 questid)	{ m_removequests.insert(questid);}
	ASCENT_INLINE void			PushToFinishedDailies(uint32 questid) { DailyMutex.Acquire(); m_finishedDailies.insert(questid); DailyMutex.Release();}
	ASCENT_INLINE bool		HasFinishedDaily(uint32 questid) { return (m_finishedDailies.find(questid) == m_finishedDailies.end() ? false : true); }
    void                AddToFinishedQuests(uint32 quest_id);
    void                EventTimedQuestExpire(Quest *qst, QuestLogEntry *qle, uint32 log_slot);
	void				AreaExploredOrEventHappens( uint32 questId ); // scriptdev2

	bool                HasFinishedQuest(uint32 quest_id);
	bool				GetQuestRewardStatus(uint32 quest_id);
	bool                HasQuestForItem(uint32 itemid);
    bool                CanFinishQuest(Quest* qst);
	bool                HasQuestSpell(uint32 spellid);
	void                RemoveQuestSpell(uint32 spellid);
	bool                HasQuestMob(uint32 entry);
	bool                HasQuest(uint32 entry);
	void                RemoveQuestMob(uint32 entry);

    //Quest related variables
	uint32 m_questbarrier1[25];
    QuestLogEntry*      m_questlog[25];
	uint32 m_questbarrier2[25];
    std::set<uint32>    m_QuestGOInProgress;
    std::set<uint32>    m_removequests;
    std::set<uint32>    m_finishedQuests;
	Mutex				DailyMutex;
	std::set<uint32>	m_finishedDailies;
    uint32              m_questSharer;
    uint32              timed_quest_slot;
	std::set<uint32>    quest_spells;
	std::set<uint32>    quest_mobs;

    /************************************************************************/
    /* Stun Immobilize                                                      */
    /************************************************************************/
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

    
    void EventPortToGM(Player *p);
	ASCENT_INLINE uint32 GetTeam() { return m_team; }
	ASCENT_INLINE void SetTeam(uint32 t) { m_team = t; m_bgTeam=t; }
	ASCENT_INLINE void ResetTeam() { m_team = myRace->team_id==7 ? 0 : 1; m_bgTeam=m_team; }

	ASCENT_INLINE bool IsInFeralForm()
	{
		int s = GetShapeShift();
		if( s <= 0 )
			return false;

		//Fight forms that do not use player's weapon
		return ( s == 1 || s == 5 || s == 8 );
	}
	void CalcDamage();
	uint32 GetMainMeleeDamage(uint32 AP_owerride); //i need this for windfury

    const uint64& GetSelection( ) const { return m_curSelection; }
	const uint64& GetTarget( ) const { return m_curTarget; }
	void SetSelection(const uint64 &guid) { m_curSelection = guid; }
	void SetTarget(const uint64 &guid) { m_curTarget = guid; }
	
    /************************************************************************/
    /* Spells                                                               */
    /************************************************************************/
	bool HasSpell(uint32 spell);
	bool HasSpellwithNameHash(uint32 hash);
	bool HasDeletedSpell(uint32 spell);
	void smsg_InitialSpells();
	void addSpell(uint32 spell_idy);
	void removeSpellByHashName(uint32 hash);
	bool removeSpell(uint32 SpellID, bool MoveToDeleted, bool SupercededSpell, uint32 SupercededSpellID);

    // PLEASE DO NOT INLINE!
    void AddOnStrikeSpell(SpellEntry* sp, uint32 delay)
    {
        m_onStrikeSpells.insert( map< SpellEntry*, pair<uint32, uint32> >::value_type( sp, make_pair( delay, 0 ) ) );
    }
    void RemoveOnStrikeSpell(SpellEntry *sp)
    {
        m_onStrikeSpells.erase(sp);
    }
    void AddOnStrikeSpellDamage(uint32 spellid, uint32 mindmg, uint32 maxdmg)
    {
        OnHitSpell sp;
        sp.spellid = spellid;
        sp.mindmg = mindmg;
        sp.maxdmg = maxdmg;
        m_onStrikeSpellDmg[spellid] = sp;
    }
    void RemoveOnStrikeSpellDamage(uint32 spellid)
    {
        m_onStrikeSpellDmg.erase(spellid);
    }

    //Spells variables
    StrikeSpellMap      m_onStrikeSpells;
    StrikeSpellDmgMap   m_onStrikeSpellDmg;
    SpellOverrideMap    mSpellOverrideMap;
    SpellSet            mSpells;
    SpellSet            mDeletedSpells;
	SpellSet			mShapeShiftSpells;

	void AddShapeShiftSpell(uint32 id);
	void RemoveShapeShiftSpell(uint32 id);


    /************************************************************************/
    /* Actionbar                                                            */
    /************************************************************************/
	void                setAction(uint8 button, uint16 action, uint8 type, uint8 misc);
	void                SendInitialActions();
    bool                m_actionsDirty;
	
    /************************************************************************/
    /* Reputation                                                           */
    /************************************************************************/
	void                ModStanding(uint32 Faction, int32 Value);
	int32               GetStanding(uint32 Faction);
	int32               GetBaseStanding(uint32 Faction);
	void                SetStanding(uint32 Faction, int32 Value);
	void                SetAtWar(uint32 Faction, bool Set);
	bool                IsAtWar(uint32 Faction);
	Standing            GetStandingRank(uint32 Faction);
	ReputationRank		GetReputationRank(uint32 Faction);  // scriptdev2
	bool                IsHostileBasedOnReputation(FactionDBC * dbc);
	void                UpdateInrangeSetsBasedOnReputation();
	void                Reputation_OnKilledUnit(Unit * pUnit, bool InnerLoop);
	void                Reputation_OnTalk(FactionDBC * dbc);
	static Standing     GetReputationRankFromStanding(int32 Standing_);
	
    /************************************************************************/
    /* Factions                                                             */
    /************************************************************************/
	void smsg_InitialFactions();
	uint32 GetFactionId();
    // factions variables
    int32 pctReputationMod;

    /************************************************************************/
    /* PVP                                                                  */
    /************************************************************************/
	ASCENT_INLINE uint8 GetPVPRank()
	{
		return (uint8)((GetUInt32Value(PLAYER_BYTES_3) >> 24) & 0xFF);
	}
	ASCENT_INLINE void SetPVPRank(int newrank)
	{
		SetUInt32Value(PLAYER_BYTES_3, ((GetUInt32Value(PLAYER_BYTES_3) & 0x00FFFFFF) | (uint8(newrank) << 24)));
	}

    /************************************************************************/
    /* Groups                                                               */
    /************************************************************************/
	void                SetInviter(uint32 pInviter) { m_GroupInviter = pInviter; }
	ASCENT_INLINE uint32       GetInviter() { return m_GroupInviter; }
	ASCENT_INLINE bool         InGroup() { return (m_playerInfo->m_Group != NULL && !m_GroupInviter); }
	bool                IsGroupLeader()
	{
		if(m_playerInfo->m_Group != NULL)
		{
			if(m_playerInfo->m_Group->GetLeader() == m_playerInfo)
				return true;
		}
		return false;
	}
	ASCENT_INLINE int          HasBeenInvited() { return m_GroupInviter != 0; }
	ASCENT_INLINE Group*       GetGroup() { return m_playerInfo ? m_playerInfo->m_Group : NULL; }
	ASCENT_INLINE int8		   GetSubGroup() { return m_playerInfo->subGroup; }
    bool                IsGroupMember(Player *plyr);
	ASCENT_INLINE bool         IsBanned()
	{
		if(m_banned)
		{
			if(m_banned < 100 || (uint32)UNIXTIME < m_banned)
				return true;
		}
		return false;
	}
    ASCENT_INLINE void         SetBanned() { m_banned = 4;}
	ASCENT_INLINE void         SetBanned(string Reason) { m_banned = 4; m_banreason = Reason;}
	ASCENT_INLINE void         SetBanned(uint32 timestamp, string& Reason) { m_banned = timestamp; m_banreason = Reason; }
	ASCENT_INLINE void         UnSetBanned() { m_banned = 0; }
	ASCENT_INLINE string       GetBanReason() {return m_banreason;}

    /************************************************************************/
    /* Guilds                                                               */
    /************************************************************************/
	ASCENT_INLINE  bool        IsInGuild() {return (m_uint32Values[PLAYER_GUILDID] != 0) ? true : false;}
	ASCENT_INLINE uint32       GetGuildId() { return m_uint32Values[PLAYER_GUILDID]; }
	void                SetGuildId(uint32 guildId);
	ASCENT_INLINE uint32       GetGuildRank() { return m_uint32Values[PLAYER_GUILDRANK]; }
	void                SetGuildRank(uint32 guildRank);
	uint32              GetGuildInvitersGuid() { return m_invitersGuid; }
	void                SetGuildInvitersGuid( uint32 guid ) { m_invitersGuid = guid; }
	void                UnSetGuildInvitersGuid() { m_invitersGuid = 0; }
  
    /************************************************************************/
    /* Duel                                                                 */
    /************************************************************************/
    void                RequestDuel(Player *pTarget);
	void                DuelBoundaryTest();
	void                EndDuel(uint8 WinCondition);
	void                DuelCountdown();
	void                SetDuelStatus(uint8 status) { m_duelStatus = status; }
	ASCENT_INLINE uint8        GetDuelStatus() { return m_duelStatus; }
	void                SetDuelState(uint8 state) { m_duelState = state; }
	ASCENT_INLINE uint8        GetDuelState() { return m_duelState; }
    // duel variables
    Player*             DuelingWith;

    /************************************************************************/
    /* Trade                                                                */
    /************************************************************************/
	void                SendTradeUpdate(void);
	void         ResetTradeVariables()
	{
		mTradeGold = 0;
		memset(&mTradeItems, 0, sizeof(Item*) * 8);
		mTradeStatus = 0;
		mTradeTarget = 0;
		m_tradeSequence = 2;
	}
	
    /************************************************************************/
    /* Pets                                                                 */
    /************************************************************************/
	ASCENT_INLINE void			SetSummon(Pet *pet) { m_Summon = pet; }
	ASCENT_INLINE Pet*			GetSummon(void) { return m_Summon; }
	uint32						GeneratePetNumber(void);
	void						RemovePlayerPet(uint32 pet_number);
	ASCENT_INLINE void			AddPlayerPet(PlayerPet* pet, uint32 index) { m_Pets[index] = pet; }
	ASCENT_INLINE PlayerPet*	GetPlayerPet(uint32 idx)
	{
		std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(idx);
		if(itr != m_Pets.end()) return itr->second;
		else
			return NULL;
	}
	void						SpawnPet(uint32 pet_number);
	void						DespawnPet();
	uint32						GetFirstPetNumber(void)
	{
		if(m_Pets.size() == 0) return 0;
		std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin();
		return itr->first;
	}
	ASCENT_INLINE PlayerPet*	GetFirstPet(void) { return GetPlayerPet(GetFirstPetNumber()); }
	ASCENT_INLINE void			SetStableSlotCount(uint8 count) { m_StableSlotCount = count; }
	ASCENT_INLINE uint8			GetStableSlotCount(void) { return m_StableSlotCount; }
	uint32						GetUnstabledPetNumber(void)
	{
		if(m_Pets.size() == 0) return 0;
		std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin();
		for(;itr != m_Pets.end();itr++)
			if(itr->second->stablestate == STABLE_STATE_ACTIVE)
				return itr->first;
		return 0;
	}
	void						EventSummonPet(Pet *new_pet); //if we charmed or simply summoned a pet, this function should get called
	void						EventDismissPet(); //if pet/charm died or whatever happned we should call this function

    /************************************************************************/
    /* Item Interface                                                       */
    /************************************************************************/
	ASCENT_INLINE ItemInterface* GetItemInterface() { return m_ItemInterface; } // Player Inventory Item storage
	ASCENT_INLINE void			ApplyItemMods(Item *item, int8 slot, bool apply,bool justdrokedown=false) {  _ApplyItemMods(item, slot, apply,justdrokedown); }
	ASCENT_INLINE bool			HasItemCount( uint32 item, uint32 count, bool inBankAlso = false ) const;
    // item interface variables
    ItemInterface *     m_ItemInterface;
	
    /************************************************************************/
    /* Loot                                                                 */
    /************************************************************************/
	ASCENT_INLINE const uint64& GetLootGUID() const { return m_lootGuid; }
	ASCENT_INLINE void         SetLootGUID(const uint64 &guid) { m_lootGuid = guid; }
	void                SendLoot(uint64 guid,uint8 loot_type);
    // loot variables
    uint64              m_lootGuid;
    uint64              m_currentLoot;
    bool                bShouldHaveLootableOnCorpse;

    /************************************************************************/
    /* World Session                                                        */
    /************************************************************************/
	ASCENT_INLINE WorldSession* GetSession() const { return m_session; }
	void SetSession(WorldSession *s) { m_session = s; }
	void SetBindPoint(float x, float y, float z, uint32 m, uint32 v) { m_bind_pos_x = x; m_bind_pos_y = y; m_bind_pos_z = z; m_bind_mapid = m; m_bind_zoneid = v;}
	void SendDelayedPacket(WorldPacket *data, bool bDeleteOnSend)
	{
		if(data == NULL) return;
		if(GetSession() != NULL) GetSession()->SendPacket(data);
		if(bDeleteOnSend) delete data;
	}
	float offhand_dmg_mod;
	float GetSpellTimeMod(uint32 id);
	int GetSpellDamageMod(uint32 id);
	int32 GetSpellManaMod(uint32 id);
	
	// Talents
	// These functions build a specific type of A9 packet
	uint32 __fastcall BuildCreateUpdateBlockForPlayer( ByteBuffer *data, Player *target );
	void DestroyForPlayer( Player *target ) const;
	void SetTalentHearthOfWildPCT(int value){hearth_of_wild_pct=value;}
	void EventTalentHearthOfWildChange(bool apply);
	
	std::list<LoginAura> loginauras;

    std::set<uint32> OnMeleeAuras;

    /************************************************************************/
    /* Player loading and savings                                           */
    /* Serialize character to db                                            */
    /************************************************************************/
	void SaveToDB(bool bNewCharacter);
	void SaveAuras(stringstream&);
	bool LoadFromDB(uint32 guid);
	void LoadFromDBProc(QueryResultVector & results);

	void LoadNamesFromDB(uint32 guid);
	bool m_FirstLogin;

    /************************************************************************/
    /* Death system                                                         */
    /************************************************************************/
	void SpawnCorpseBody();
	void SpawnCorpseBones();
	void CreateCorpse();
	void KillPlayer();
	void ResurrectPlayer();
	void BuildPlayerRepop();
	void RepopRequestedPlayer();
	void DeathDurabilityLoss(double percent);
	void RepopAtGraveyard(float ox, float oy, float oz, uint32 mapid);

	uint64 m_resurrecter;
	
    /************************************************************************/
    /* Movement system                                                      */
    /************************************************************************/
	void SetMovement(uint8 pType, uint32 flag);
	void SetPlayerSpeed(uint8 SpeedType, float value);
	float GetPlayerSpeed(){return m_runSpeed;}
	uint8 m_currentMovement;
	bool m_isMoving;
	//Invisibility stuff
	bool m_isGmInvisible;
	
    /************************************************************************/
    /* Channel stuff                                                        */
    /************************************************************************/
	void JoinedChannel(Channel *c);
	void LeftChannel(Channel *c);
	void CleanupChannels();
	//Attack stuff
	void EventAttackStart();
	void EventAttackStop();
	void EventAttackUpdateSpeed() { }
	void EventDeath();
	//Note:ModSkillLine -> value+=amt;ModSkillMax -->value=amt; --wierd
	float GetSkillUpChance(uint32 id);
	//ASCENT_INLINE std::list<struct skilllines>getSkillLines() { return m_skilllines; }
	float SpellCrtiticalStrikeRatingBonus;
	float SpellHasteRatingBonus;
	void UpdateAttackSpeed();
	void UpdateChances();
	void UpdateStats();
	void UpdateHit(int32 hit);
   
	bool canCast(SpellEntry *m_spellInfo);
	ASCENT_INLINE float GetBlockFromSpell() { return m_blockfromspell; }
	ASCENT_INLINE float GetSpellCritFromSpell() { return m_spellcritfromspell; }
	ASCENT_INLINE float GetHitFromSpell() { return m_hitfromspell; }
	ASCENT_INLINE float GetParryFromSpell() { return m_parryfromspell; }
	ASCENT_INLINE float GetDodgeFromSpell() { return m_dodgefromspell; }
	void SetBlockFromSpell(float value) { m_blockfromspell = value; }
	void SetSpellCritFromSpell(float value) { m_spellcritfromspell = value; }
	void SetParryFromSpell(float value) { m_parryfromspell = value; }
	void SetDodgeFromSpell(float value) { m_dodgefromspell = value; }
	void SetHitFromSpell(float value) { m_hitfromspell = value; }
	ASCENT_INLINE uint32 GetHealthFromSpell() { return m_healthfromspell; }
	ASCENT_INLINE uint32 GetManaFromSpell() { return m_manafromspell; }
	void SetHealthFromSpell(uint32 value) { m_healthfromspell = value;}
	void SetManaFromSpell(uint32 value) { m_manafromspell = value;}
	uint32 CalcTalentResetCost(uint32 resetnum);
	void SendTalentResetConfirm();
	void SendPetUntrainConfirm();
	uint32 GetTalentResetTimes() { return m_talentresettimes; }
	ASCENT_INLINE void SetTalentResetTimes(uint32 value) { m_talentresettimes = value; }
	void SetPlayerStatus(uint8 pStatus) { m_status = pStatus; }
	ASCENT_INLINE uint8 GetPlayerStatus() { return m_status; }
	const float& GetBindPositionX( ) const { return m_bind_pos_x; }
	const float& GetBindPositionY( ) const { return m_bind_pos_y; }
	const float& GetBindPositionZ( ) const { return m_bind_pos_z; }
	const uint32& GetBindMapId( ) const { return m_bind_mapid; }
	const uint32& GetBindZoneId( ) const { return m_bind_zoneid; }
	ASCENT_INLINE uint8 GetShapeShift()
	{
		return GetByte(UNIT_FIELD_BYTES_2,3);
	}

	
	void delayAttackTimer(int32 delay)
	{
		if(!delay)
			return;

		m_attackTimer += delay;
		m_attackTimer_1 += delay;
	}
	
	void SetShapeShift(uint8 ss);

	uint32 m_furorChance;

    //Showing Units WayPoints
	AIInterface* waypointunit;
	
	uint32 m_nextSave;
	//Tutorials
	uint32 GetTutorialInt(uint32 intId );
	void SetTutorialInt(uint32 intId, uint32 value);
	//Base stats calculations
	//void CalcBaseStats();
	// Rest
	uint32 SubtractRestXP(uint32 amount);
	void AddCalculatedRestXP(uint32 seconds);
	void ApplyPlayerRestState(bool apply);
	void UpdateRestState();
	bool m_noFallDamage;
	float z_axisposition;
	int32 m_safeFall;
	// Gossip
	GossipMenu* CurrentGossipMenu;
	void CleanupGossipMenu();
	void Gossip_Complete();
	int m_lifetapbonus;
	uint32 m_lastShotTime;
	
	// scriptdev2
	GossipMenu* PlayerTalkClass;
	void PrepareQuestMenu( uint64 guid );
	void SendGossipMenu( uint32 TitleTextId, uint64 npcGUID );
	void CloseGossip();
	QuestStatus GetQuestStatus( uint32 quest_id );


	bool m_bUnlimitedBreath;
	uint32 m_UnderwaterTime;
	uint32 m_UnderwaterState;
	uint32 m_SwimmingTime;
	uint32 m_BreathDamageTimer;
	// Visible objects
	bool CanSee(Object* obj);
	ASCENT_INLINE bool IsVisible(Object* pObj) { return !(m_visibleObjects.find(pObj) == m_visibleObjects.end()); }
	void AddInRangeObject(Object* pObj);
	void OnRemoveInRangeObject(Object* pObj);
	void ClearInRangeSet();
	ASCENT_INLINE void AddVisibleObject(Object* pObj) { m_visibleObjects.insert(pObj); }
	ASCENT_INLINE void RemoveVisibleObject(Object* pObj) { m_visibleObjects.erase(pObj); }
	ASCENT_INLINE void RemoveVisibleObject(InRangeSet::iterator itr) { m_visibleObjects.erase(itr); }
	ASCENT_INLINE InRangeSet::iterator FindVisible(Object * obj) { return m_visibleObjects.find(obj); }
	ASCENT_INLINE void RemoveIfVisible(Object * obj)
	{
		InRangeSet::iterator itr = m_visibleObjects.find(obj);
		if(itr == m_visibleObjects.end())
			return;

		m_visibleObjects.erase(obj);
		PushOutOfRange(obj->GetNewGUID());
	}

	ASCENT_INLINE bool GetVisibility(Object * obj, InRangeSet::iterator *itr)
	{
		*itr = m_visibleObjects.find(obj);
		return ((*itr) != m_visibleObjects.end());
	}

	ASCENT_INLINE InRangeSet::iterator GetVisibleSetBegin() { return m_visibleObjects.begin(); }
	ASCENT_INLINE InRangeSet::iterator GetVisibleSetEnd() { return m_visibleObjects.end(); }
	
	//Transporters
	bool m_lockTransportVariables;
	uint64 m_TransporterGUID;
	float m_TransporterX;
	float m_TransporterY;
	float m_TransporterZ;
	float m_TransporterO;
	float m_TransporterTime;
	// Misc
	void EventCannibalize(uint32 amount);
	void EventReduceDrunk(bool full);
	bool m_AllowAreaTriggerPort;
	void EventAllowTiggerPort(bool enable);
	int32 m_rangedattackspeedmod;
	int32 m_meleeattackspeedmod;
	uint32 m_modblockabsorbvalue;
	uint32 m_modblockvaluefromspells;
	void SendInitialLogonPackets();
	void Reset_Spells();
	void Reset_Talents();
	void Reset_ToLevel1();
	// Battlegrounds xD
	CBattleground * m_bg;
	CBattleground * m_pendingBattleground;
	uint32 m_bgEntryPointMap;
	float m_bgEntryPointX;	
	float m_bgEntryPointY;
	float m_bgEntryPointZ;
	float m_bgEntryPointO;
	int32 m_bgEntryPointInstance;
	bool m_bgHasFlag;
	bool m_bgIsQueued;
	uint32 m_bgQueueType;
	uint32 m_bgQueueInstanceId;
	void EventRepeatSpell();
	void EventCastRepeatedSpell(uint32 spellid, Unit *target);
	int32 CanShootRangedWeapon(uint32 spellid, Unit *target, bool autoshot);
	uint32 m_AutoShotDuration;
	uint32 m_AutoShotAttackTimer;
	bool m_onAutoShot;
	uint64 m_AutoShotTarget;
	SpellEntry *m_AutoShotSpell;
	void _InitialReputation();
	void EventActivateGameObject(GameObject* obj);
	void EventDeActivateGameObject(GameObject* obj);
	void UpdateNearbyGameObjects();
	
	void CalcResistance(uint32 type);
	ASCENT_INLINE float res_M_crit_get(){return m_resist_critical[0];}
	ASCENT_INLINE void res_M_crit_set(float newvalue){m_resist_critical[0]=newvalue;}
	ASCENT_INLINE float res_R_crit_get(){return m_resist_critical[1];}
	ASCENT_INLINE void res_R_crit_set(float newvalue){m_resist_critical[1]=newvalue;}
	uint32 FlatResistanceModifierPos[7];
	uint32 FlatResistanceModifierNeg[7];
	uint32 BaseResistanceModPctPos[7];
	uint32 BaseResistanceModPctNeg[7];
	uint32 ResistanceModPctPos[7];
	uint32 ResistanceModPctNeg[7];
	float m_resist_critical[2];//when we are a victim we can have talents to decrease chance for critical hit. This is a negative value and it's added to critchances
	float m_resist_hit[3]; // 0 = melee; 1= ranged; 2=spells
	float SpellDmgDoneByAttribute[5][7];
	float SpellHealDoneByAttribute[5][7];
	uint32 m_modphyscritdmgPCT;
	uint32 m_RootedCritChanceBonus; // Class Script Override: Shatter

	uint32 m_ModInterrMRegenPCT;
	int32 m_ModInterrMRegen;
	float m_RegenManaOnSpellResist;
	uint32 m_casted_amount[7]; //Last casted spells amounts. Need for some spells. Like Ignite etc. DOesn't count HoTs and DoTs. Only directs
	
	uint32 FlatStatModPos[5];
	uint32 FlatStatModNeg[5];
	uint32 StatModPctPos[5];
	uint32 StatModPctNeg[5];
	uint32 TotalStatModPctPos[5];
	uint32 TotalStatModPctNeg[5];
	int32 IncreaseDamageByType[12]; //mod dmg by creature type
	float IncreaseDamageByTypePCT[12];
	float IncreaseCricticalByTypePCT[12];
	int32 DetectedRange;
	float PctIgnoreRegenModifier;
	uint32 m_retainedrage;
/*	
	union {
		float mRatingToPct[37];
		uint32 mRatingToPoint[37]; //block, skill.. cant be decimal values
	};
*/
	ASCENT_INLINE uint32* GetPlayedtime() { return m_playedtime; };
	void CalcStat(uint32 t);
	float CalcRating(uint32 t);
	void RecalcAllRatings();
	void RegenerateMana(bool is_interrupted);
	void RegenerateHealth(bool inCombat);
	void RegenerateEnergy();
	void LooseRage(int32 value);
	
    uint32 SoulStone;
	uint32 SoulStoneReceiver;
	void removeSoulStone();

    ASCENT_INLINE uint32 GetSoulStoneReceiver(){return SoulStoneReceiver;}
    ASCENT_INLINE void SetSoulStoneReceiver(uint32 StoneGUID){SoulStoneReceiver = StoneGUID;}
    ASCENT_INLINE uint32 GetSoulStone(){return SoulStone;}
    ASCENT_INLINE void SetSoulStone(uint32 StoneID){SoulStone = StoneID;}

	bool bReincarnation;

	map<uint32, WeaponModifier> damagedone;
	map<uint32, WeaponModifier> tocritchance;
	uint32 Seal;
	uint32 judgespell;
	bool cannibalize;
	uint8 cannibalizeCount;
	int32 rageFromDamageDealt;
	// GameObject commands
	inline GameObject* GetSelectedGo()
	{ 
		if( m_GM_SelectedGO ) 
			return GetMapMgr()->GetGameObject( (uint32)m_GM_SelectedGO );
		return NULL;
	}
	uint64 m_GM_SelectedGO;
	
#ifndef CLUSTERING
	void _Relocate(uint32 mapid,const LocationVector & v, bool sendpending, bool force_new_world, uint32 instance_id);
#else
	void RelocateCallback(uint32 instance_were_going_to);
#endif
	void AddItemsToWorld();
	void RemoveItemsFromWorld();
	
	uint32 m_ShapeShifted;
	uint32 m_MountSpellId;
    
	ASCENT_INLINE bool IsMounted() {return (m_MountSpellId!=0 ? true : false); }
	
    bool bHasBindDialogOpen;
	bool bGMTagOn;
	uint32 TrackingSpell;
	void _EventCharmAttack();
	void _Kick();
	void Kick(uint32 delay = 0);
	void SoftDisconnect();
	uint32 m_KickDelay;
	uint64 m_CurrentCharm;
	Transporter * m_CurrentTransporter;
	
	Object * GetSummonedObject () {return m_SummonedObject;};
	void SetSummonedObject (Object * t_SummonedObject) {m_SummonedObject = t_SummonedObject;};
	uint32 roll;

	void ClearCooldownsOnLine(uint32 skill_line, uint32 called_from);
	void ResetAllCooldowns();
	void ClearCooldownForSpell(uint32 spell_id);

	bool bProcessPending;
	Mutex _bufferS;
	void PushUpdateData(ByteBuffer *data, uint32 updatecount);
    void PushCreationData(ByteBuffer *data, uint32 updatecount);
	void PushOutOfRange(const WoWGuid & guid);
	void ProcessPendingUpdates();
	bool __fastcall CompressAndSendUpdateBuffer(uint32 size, const uint8* update_buffer);
	void ClearAllPendingUpdates();
	
	uint32 GetArmorProficiency() { return armor_proficiency; }
	uint32 GetWeaponProficiency() { return weapon_proficiency; }

	void SpeedCheatDelay(uint32 ms_delay);
	void SpeedCheatReset();

	void AddSplinePacket(uint64 guid, ByteBuffer* packet);
	ByteBuffer* GetAndRemoveSplinePacket(uint64 guid);
	void ClearSplinePackets();
	bool ExitInstance();
	void SaveEntryPoint(uint32 mapId);
	bool CooldownCheat;
	bool CastTimeCheat;
	bool GodModeCheat;
	bool PowerCheat;
	bool FlyCheat;
	void ZoneUpdate(uint32 ZoneId);
	ASCENT_INLINE uint32 GetAreaID() { return m_AreaID; }
	void SetAreaID(uint32 area) { m_AreaID = area; }
	
	
	std::string Lfgcomment;
	uint16 LfgDungeonId[3];
	uint8 LfgType[3];
	uint16 LfmDungeonId;
	uint8 LfmType;
	bool m_Autojoin;
	bool m_AutoAddMem;
	void StopMirrorTimer(uint32 Type);
	BGScore m_bgScore;
	uint32 m_bgTeam;
	void UpdateChanceFields();
	//Honor Variables
	time_t m_fallDisabledUntil;
	uint32 m_honorToday;
	uint32 m_honorYesterday;
	
	void RolloverHonor();
	uint32 m_honorPoints;
	uint32 m_honorRolloverTime;
	uint32 m_killsToday;
	uint32 m_killsYesterday;
	uint32 m_killsLifetime;
	uint32 m_arenaPoints;
	uint32 m_honorless;
	uint32 m_lastSeenWeather;
	set<Object*> m_visibleFarsightObjects;
	void EventTeleport(uint32 mapid, float x, float y, float z);
	void ApplyLevelInfo(LevelInfo* Info, uint32 Level);
	void BroadcastMessage(const char* Format, ...);
	map<uint32, set<uint32> > SummonSpells;
	map<uint32, PetSpellMap*> PetSpells;
	void AddSummonSpell(uint32 Entry, uint32 SpellID);
	void RemoveSummonSpell(uint32 Entry, uint32 SpellID);
	set<uint32>* GetSummonSpells(uint32 Entry);
	LockedQueue<WorldPacket*> delayedPackets;
	set<Player *> gmTargets;
	uint32 m_UnderwaterMaxTime;
	uint32 m_UnderwaterLastDmg;
	ASCENT_INLINE void setMyCorpse(Corpse * corpse) { myCorpse = corpse; }
	ASCENT_INLINE Corpse * getMyCorpse() { return myCorpse; }
	bool bCorpseCreateable;
	uint32 m_resurrectHealth, m_resurrectMana;
	uint32 m_resurrectInstanceID, m_resurrectMapId;
	LocationVector m_resurrectPosition;
	bool blinked;
	uint32 m_explorationTimer;
	// DBC stuff
	CharRaceEntry * myRace;
	CharClassEntry * myClass;
	Unit * linkTarget;
	bool stack_cheat;
	bool triggerpass_cheat;
	bool SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O);
	bool SafeTeleport(uint32 MapID, uint32 InstanceID, const LocationVector & vec);
	void SafeTeleport(MapMgr * mgr, const LocationVector & vec);
	void EjectFromInstance();
	bool raidgrouponlysent;
	
	void EventSafeTeleport(uint32 MapID, uint32 InstanceID, LocationVector vec)
	{
		SafeTeleport(MapID, InstanceID, vec);
	}

	// Hack fix here!
	void ForceZoneUpdate();

	/*****************
	  PVP Stuff
	******************/
	uint32 m_pvpTimer;
	
	//! Is PVP flagged?
	ASCENT_INLINE bool IsPvPFlagged() { return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP); }
	ASCENT_INLINE void SetPvPFlag()
	{
		// reset the timer as well..
		StopPvPTimer();
		SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP);
	}
	//! Removal
	ASCENT_INLINE void RemovePvPFlag()
	{
		StopPvPTimer();
		RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP);
	}
	//! Do this on /pvp off
	ASCENT_INLINE void ResetPvPTimer();
	//! Stop the timer for pvp off
	ASCENT_INLINE void StopPvPTimer() { m_pvpTimer = 0; }
	
	//! Called at login to add the honorless buff, etc.
	void LoginPvPSetup();
	//! Update our pvp area (called when zone changes)
	void UpdatePvPArea();
	//! PvP Toggle (called on /pvp)
	void PvPToggle();

	ASCENT_INLINE uint32 LastHonorResetTime() const { return m_lastHonorResetTime; }
	ASCENT_INLINE void LastHonorResetTime(uint32 val) { m_lastHonorResetTime = val; }
	uint32 OnlineTime;
	bool tutorialsDirty;
	LevelInfo * lvlinfo;
	void CalculateBaseStats();
	uint32 load_health;
	uint32 load_mana;
	void CompleteLoading();
	set<SpellEntry *> castSpellAtLogin;
	void OnPushToWorld();
	void OnPrePushToWorld();
	void OnWorldPortAck();
	uint32 m_TeleportState;
	set<Unit*> visiblityChangableSet;
	bool m_beingPushed;
	bool CanSignCharter(Charter * charter, Player * requester);
	Charter * m_charters[NUM_CHARTER_TYPES];
	uint32 flying_aura;
	stringstream LoadAuras;
	bool resend_speed;
	bool rename_pending;
	uint32 iInstanceType;
	ASCENT_INLINE void SetName(string& name) { m_name = name; }
	// spell to (delay, last time)
	
	FactionReputation * reputationByListId[128];
	
	uint64 m_comboTarget;
	int8 m_comboPoints;
	bool m_retainComboPoints;
	int8 m_spellcomboPoints; // rogue talent Ruthlessness will change combopoints while consuming them. solutions 1) add post cast prochandling, 2) delay adding the CP
	void UpdateComboPoints();

	ASCENT_INLINE void AddComboPoints(uint64 target, uint8 count)
	{
        if(m_comboTarget == target)
			m_comboPoints += count;
		else
		{
			m_comboTarget = target;
			m_comboPoints = count;
		}
		UpdateComboPoints();
	}

	ASCENT_INLINE void NullComboPoints() { if(!m_retainComboPoints) { m_comboTarget = 0; m_comboPoints = 0; m_spellcomboPoints=0; } UpdateComboPoints(); }
	uint32 m_speedChangeCounter;

	void SendAreaTriggerMessage(const char * message, ...);
        
	// Trade Target
	//Player *getTradeTarget() {return mTradeTarget;};

	ASCENT_INLINE Player * GetTradeTarget()
	{
		if(!IsInWorld()) return 0;
		return m_mapMgr->GetPlayer((uint32)mTradeTarget);
	}

	Item *getTradeItem(uint32 slot) {return mTradeItems[slot];};
        
	// Water level related stuff (they are public because they need to be accessed fast)
	// Nose level of the character (needed for proper breathing)
	float m_noseLevel;

	/* Mind Control */
	void Possess(Unit * pTarget);
	void UnPossess();

	/* Last Speeds */
	ASCENT_INLINE void UpdateLastSpeeds()
	{
		m_lastRunSpeed = m_runSpeed;
		m_lastRunBackSpeed = m_backWalkSpeed;
		m_lastSwimSpeed = m_swimSpeed;
		m_lastRunBackSpeed = m_backSwimSpeed;
		m_lastFlySpeed = m_flySpeed;
	}

	void RemoteRevive()
	{
		ResurrectPlayer();
		SetMovement(MOVE_UNROOT, 5);
		SetPlayerSpeed(RUN, (float)7);
		SetPlayerSpeed(SWIM, (float)4.9);
		SetMovement(MOVE_LAND_WALK, 8);
		SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH) );
	}

	LocationVector m_last_group_position;
	int32 m_rap_mod_pct;
	void SummonRequest(uint32 Requestor, uint32 ZoneID, uint32 MapID, uint32 InstanceID, const LocationVector & Position);
#ifdef OPTIMIZED_PLAYER_SAVING
	void save_LevelXP();
	void save_Skills();
	void save_ExploreData();
	void save_Gold();
	void save_Misc();
	void save_PositionHP();
	void save_BindPosition();
	void save_Honor();
	void save_EntryPoint();
	void save_Taxi();
	void save_Transporter();
	void save_Spells();
	void save_Actions();
	void save_Reputation();
	void save_Auras();
	void save_InstanceType();
	void save_Zone();
	void save_PVP();
#endif

#ifdef CLUSTERING
	void EventRemoveAndDelete();
	void PackPlayerData(ByteBuffer & data);
	bool UnpackPlayerData(ByteBuffer & data);
#endif

	Creature * m_tempSummon;
	bool m_deathVision;
	SpellEntry * last_heal_spell;
	LocationVector m_sentTeleportPosition;

	void RemoveFromBattlegroundQueue();
	void FullHPMP();
	uint32 m_arenateaminviteguid;

    /************************************************************************/
    /* Spell Packet wrapper Please keep this separated                      */
    /************************************************************************/
    void SendLevelupInfo(uint32 level, uint32 Hp, uint32 Mana, uint32 Stat0, uint32 Stat1, uint32 Stat2, uint32 Stat3, uint32 Stat4);
    void SendLogXPGain(uint64 guid, uint32 NormalXP, uint32 RestedXP, bool type);
    void SendEnvironmentalDamageLog(const uint64 & guid, uint8 type, uint32 damage);
	void SendWorldStateUpdate(uint32 WorldState, uint32 Value);
	void SendCastResult(uint32 SpellId, uint8 ErrorMessage, uint8 MultiCast, uint32 Extra);
	void Gossip_SendPOI(float X, float Y, uint32 Icon, uint32 Flags, uint32 Data, const char* Name);
    /************************************************************************/
    /* End of SpellPacket wrapper                                           */
    /************************************************************************/

	Mailbox m_mailBox;
	bool m_waterwalk;
	bool m_setwaterwalk;
	bool m_setflycheat;
	uint64 m_areaSpiritHealer_guid;
	bool m_finishingmovesdodge;

	ASCENT_INLINE bool IsAttacking() {return m_attacking; }

	static void InitVisibleUpdateBits();
	static UpdateMask m_visibleUpdateMask;

	void CopyAndSendDelayedPacket(WorldPacket * data);
	void PartLFGChannel();
	SpeedCheatDetector	*SDetector;
protected:
	LocationVector m_summonPos;
	uint32 m_summonInstanceId;
	uint32 m_summonMapId;
	uint32 m_summoner;

	uint32 iActivePet;
	void _SetCreateBits(UpdateMask *updateMask, Player *target) const;
	void _SetUpdateBits(UpdateMask *updateMask, Player *target) const;

	/* Update system components */
	ByteBuffer bUpdateBuffer;
    ByteBuffer bCreationBuffer;
	uint32 mUpdateCount;
    uint32 mCreationCount;
	uint32 mOutOfRangeIdCount;
	ByteBuffer mOutOfRangeIds;
	SplineMap _splineMap;
	/* End update system */

	void _LoadTutorials(QueryResult * result);
	void _SaveTutorials(QueryBuffer * buf);
	void _SaveInventory(bool firstsave);
	void _SaveQuestLogEntry(QueryBuffer * buf);
	void _LoadQuestLogEntry(QueryResult * result);

	void _LoadPet(QueryResult * result);
	void _LoadPetNo();
	void _LoadPetSpells(QueryResult * result);
	void _SavePet(QueryBuffer * buf);
	void _SavePetSpells(QueryBuffer * buf);
	void _ApplyItemMods( Item* item, int8 slot, bool apply, bool justdrokedown = false, bool skip_stat_apply = false );
	void _EventAttack( bool offhand );
	void _EventExploration();

	// Water level related stuff
	void SetNoseLevel();

	/************************************************************************/
	/* Trade																*/
	/************************************************************************/
	Item* mTradeItems[8];
	uint32 mTradeGold;
	uint32 mTradeTarget;
	uint32 mTradeStatus;

    /************************************************************************/
    /* Player Class systems, info and misc things                           */
    /************************************************************************/
    PlayerCreateInfo *info;
	uint32      m_AttackMsgTimer;	// "too far away" and "wrong facing" timer
	bool        m_attacking;
	std::string m_name;	// max 21 character name
	uint32      m_Tutorials[8];

    // Character Ban
	uint32      m_banned;
	string      m_banreason;
	uint32      m_AreaID;
	Pet*        m_Summon;
	uint32      m_PetNumberMax;
	std::map<uint32, PlayerPet*> m_Pets;
	
    uint32      m_invitersGuid; // It is guild inviters guid ,0 when its not used
    

    // bind
	float m_bind_pos_x;
	float m_bind_pos_y;
	float m_bind_pos_z;
	uint32 m_bind_mapid;
	uint32 m_bind_zoneid;
	std::list<ItemSet> m_itemsets;
	//Duel
	uint32 m_duelCountdownTimer;
	uint8 m_duelStatus;
	uint8 m_duelState;
	// Rested State Stuff
	uint32 m_timeLogoff;
	// Played time
	uint32 m_playedtime[3];
	uint8 m_isResting;
	uint8 m_restState;
	uint32 m_restAmount;
	//combat mods
	float m_blockfromspell;
	float m_blockfromspellPCT;
	float m_critfromspell;
	float m_spellcritfromspell;
	float m_dodgefromspell;
	float m_parryfromspell;
	float m_hitfromspell;
	//stats mods
	uint32 m_healthfromspell;
	uint32 m_manafromspell;
	uint32 m_healthfromitems;
	uint32 m_manafromitems;
	
	uint32 armor_proficiency;
	uint32 weapon_proficiency;
	// Talents
	uint32 m_talentresettimes;
	// STATUS
	uint8 m_status;
	// guid of current target
	uint64 m_curTarget;
	// guid of current selection
	uint64 m_curSelection;
	// Raid
	uint8 m_targetIcon;
	//Player Action Bar
	ActionButton mActions[PLAYER_ACTION_BUTTON_SIZE];
	// Player Reputation
	ReputationMap m_reputation;
	// Pointer to this char's game client
	WorldSession *m_session;
	// Channels
	std::set<Channel*> m_channels;
	// Visible objects
	std::set<Object*> m_visibleObjects;
	// Groups/Raids
	uint32 m_GroupInviter;
	uint8 m_StableSlotCount;

    // Fishing related
	Object *m_SummonedObject;

    // other system
	Corpse *    myCorpse;

	uint32      m_lastHonorResetTime;
	uint32      _fields[PLAYER_END];
	uint32	    trigger_on_stun;        //bah, warrior talent but this will not get triggered on triggered spells if used on proc so i'm forced to used a special variable
	uint32	    trigger_on_stun_chance; //also using this for mage "Frostbite" talent
	uint32	    trigger_on_stun_victim;        //bah, warrior talent but this will not get triggered on triggered spells if used on proc so i'm forced to used a special variable
	uint32	    trigger_on_stun_chance_victim; //also using this for mage "Frostbite" talent
	int			hearth_of_wild_pct;		//druid hearth of wild talent used on shapeshifting. We eighter know what is last talent level or memo on learn

	uint32 m_team;
	float       m_lastRunSpeed;
	float       m_lastRunBackSpeed;
	float       m_lastSwimSpeed;
	float       m_lastBackSwimSpeed;
	float       m_lastFlySpeed;

	uint32 m_mountCheckTimer;
	void RemovePendingPlayer();
public:
#ifdef ENABLE_COMPRESSED_MOVEMENT
	void EventDumpCompressedMovement();
	void AppendMovementData(uint32 op, uint32 sz, const uint8* data);
	Mutex m_movementBufferLock;
	ByteBuffer m_movementBuffer;
#endif

	void addDeletedSpell(uint32 id) { mDeletedSpells.insert( id ); }

	map<uint32, uint32> m_forcedReactions;

#ifdef COLLISION
	uint32 m_flyhackCheckTimer;
	void _FlyhackCheck();
#endif

	bool m_passOnLoot;
	uint32 m_tradeSequence;
	bool m_changingMaps;

	/************************************************************************/
	/* SOCIAL                                                               */
	/************************************************************************/
private:
	/* we may have multiple threads on this(chat) - burlex */
	Mutex m_socialLock;
	map<uint32, char*> m_friends;
	set<uint32> m_ignores;
	set<uint32> m_hasFriendList;

	void Social_SendFriendList(uint32 flag);
	
	void Social_AddFriend(const char * name, const char * note);
	void Social_RemoveFriend(uint32 guid);
	
	void Social_AddIgnore(const char * name);
	void Social_RemoveIgnore(uint32 guid);

	void Social_SetNote(uint32 guid, const char * note);

public:
	bool Social_IsIgnoring(PlayerInfo * m_info);
	bool Social_IsIgnoring(uint32 guid);

	void Social_TellFriendsOnline();
	void Social_TellFriendsOffline();

	/************************************************************************/
	/* end social                                                           */
	/************************************************************************/

	PlayerInfo * m_playerInfo;
	uint32 m_outStealthDamageBonusPct;
	uint32 m_outStealthDamageBonusPeriod;
	uint32 m_outStealthDamageBonusTimer;

	int16 m_vampiricEmbrace;
	int16 m_vampiricTouch;
	void VampiricSpell(uint32 dmg, Unit* pTarget);
};

class SkillIterator
{
	SkillMap::iterator m_itr;
	SkillMap::iterator m_endItr;
	bool m_searchInProgress;
	Player * m_target;
public:
	SkillIterator(Player* target) : m_searchInProgress(false),m_target(target) {}
	~SkillIterator() { if(m_searchInProgress) { EndSearch(); } }

	void BeginSearch()
	{
		// iteminterface doesn't use mutexes, maybe it should :P
		ASSERT(!m_searchInProgress);
		m_itr = m_target->m_skills.begin();
		m_endItr = m_target->m_skills.end();
		m_searchInProgress=true;
	}

	void EndSearch()
	{
		// nothing here either
		ASSERT(m_searchInProgress);
		m_searchInProgress=false;
	}

	PlayerSkill* operator*() const
	{
		return &m_itr->second;
	}

	PlayerSkill* operator->() const
	{
		return &m_itr->second;
	}

	void Increment()
	{
		if(!m_searchInProgress)
			BeginSearch();

		if(m_itr==m_endItr)
			return;

		++m_itr;
	}

	ASCENT_INLINE PlayerSkill* Grab() { return &m_itr->second; }
	ASCENT_INLINE bool End() { return (m_itr==m_endItr)?true:false; }
};

#ifdef ENABLE_COMPRESSED_MOVEMENT

class CMovementCompressorThread : public ThreadBase
{
	bool running;
	Mutex m_listLock;
	set<Player*> m_players;
public:
	CMovementCompressorThread() { running = true; }

	void AddPlayer(Player * pPlayer);
	void RemovePlayer(Player * pPlayer);

	void OnShutdown() { running = false; }
	bool run();
};

extern CMovementCompressorThread * MovementCompressor;

#endif

#endif

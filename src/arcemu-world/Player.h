/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

#ifndef _PLAYER_H
#define _PLAYER_H

#include "PlayerCache.h"

struct BGScore;
#ifdef ENABLE_ACHIEVEMENTS
class AchievementMgr;
#endif
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
class SpeedCheatDetector;

#define PLAYER_NORMAL_RUN_SPEED 7.0f
#define PLAYER_NORMAL_SWIM_SPEED 4.722222f
#define PLAYER_NORMAL_FLIGHT_SPEED 7.0f
#define PLAYER_HONORLESS_TARGET_SPELL 2479
#define MONSTER_NORMAL_RUN_SPEED 8.0f
/* action button defines */
#define PLAYER_ACTION_BUTTON_COUNT 136
#define PLAYER_ACTION_BUTTON_SIZE PLAYER_ACTION_BUTTON_COUNT * sizeof(ActionButton)

#define MAX_SPEC_COUNT 2
#define GLYPHS_COUNT 6

#define PLAYER_LEVEL_CAP                80
#define PLAYER_ARENA_MIN_LEVEL  70

#define PLAYER_EXPLORED_ZONES_LENGTH 128
#ifdef ENABLE_ACHIEVEMENTS
#define ACHIEVEMENT_SEND_DELAY 1000 //we have this delay of sending auras to other players so client will have time to create object first
#endif
#define LOGIN_CIENT_SEND_DELAY 1000 //we have this delay of sending auras to other players so client will have time to create object first

enum PlayerTeams{
	TEAM_ALLIANCE = 0,
	TEAM_HORDE    = 1,
	MAX_PLAYER_TEAMS
};

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
    DEATHKNIGHT = 6,
    SHAMAN = 7,
    MAGE = 8,
    WARLOCK = 9,
    DRUID = 11,
	MAX_PLAYER_CLASSES
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
    PVPTITLE_NONE							= 0,
    PVPTITLE_PRIVATE						= 1,
    PVPTITLE_CORPORAL						= 2,
    PVPTITLE_SERGEANT						= 3,
    PVPTITLE_MASTER_SERGEANT				= 4,
    PVPTITLE_SERGEANT_MAJOR					= 5,
    PVPTITLE_KNIGHT							= 6,
    PVPTITLE_KNIGHT_LIEUTENANT				= 7,
    PVPTITLE_KNIGHT_CAPTAIN					= 8,
    PVPTITLE_KNIGHT_CHAMPION				= 9,
    PVPTITLE_LIEUTENANT_COMMANDER			= 10,
    PVPTITLE_COMMANDER						= 11,
    PVPTITLE_MARSHAL						= 12,
    PVPTITLE_FIELD_MARSHAL					= 13,
    PVPTITLE_GRAND_MARSHAL					= 14,
    PVPTITLE_SCOUT							= 15,
    PVPTITLE_GRUNT							= 16,
    PVPTITLE_HSERGEANT						= 17,
    PVPTITLE_SENIOR_SERGEANT				= 18,
    PVPTITLE_FIRST_SERGEANT					= 19,
    PVPTITLE_STONE_GUARD					= 20,
    PVPTITLE_BLOOD_GUARD					= 21,
    PVPTITLE_LEGIONNAIRE					= 22,
    PVPTITLE_CENTURION						= 23,
    PVPTITLE_CHAMPION						= 24,
    PVPTITLE_LIEUTENANT_GENERAL				= 25,
    PVPTITLE_GENERAL						= 26,
    PVPTITLE_WARLORD						= 27,
    PVPTITLE_HIGH_WARLORD					= 28,
    PVPTITLE_GLADIATOR						= 29,
    PVPTITLE_DUELIST						= 30,
    PVPTITLE_RIVAL							= 31,
    PVPTITLE_CHALLENGER						= 32,
    PVPTITLE_SCARAB_LORD					= 33,
    PVPTITLE_CONQUEROR						= 34,
    PVPTITLE_JUSTICAR						= 35,
    PVPTITLE_CHAMPION_OF_THE_NAARU			= 36,
    PVPTITLE_MERCILESS_GLADIATOR			= 37,
    PVPTITLE_OF_THE_SHATTERED_SUN			= 38,
    PVPTITLE_HAND_OF_ADAL					= 39,
    PVPTITLE_VENGEFUL_GLADIATOR				= 40,
    PVPTITLE_BATTLEMASTER					= 41,
    PVPTITLE_THE_SEEKER						= 42,
    PVPTITLE_ELDER							= 43,
    PVPTITLE_FLAME_WARDEN					= 44,
    PVPTITLE_FLAME_KEEPER					= 45,
    PVPTITLE_THE_EXALTED					= 46,
    PVPTITLE_THE_EXPLORER					= 47,
    PVPTITLE_THE_DIPLOMAT					= 48,
    PVPTITLE_BRUTAL_GLADIATOR				= 49,
    PVPTITLE_ARENA_MASTER					= 50,
    PVPTITLE_SALTY							= 51,
    PVPTITLE_CHEF							= 52,
    PVPTITLE_THE_SUPREME					= 53,
    PVPTITLE_OF_THE_TEN_STORMS				= 54,
    PVPTITLE_OF_THE_EMERALD_DREAM			= 55,
    PVPTITLE_DEADLY_GLADIATOR				= 56,
    PVPTITLE_PROPHET						= 57,
    PVPTITLE_THE_MALEFIC					= 58,
    PVPTITLE_STALKER						= 59,
    PVPTITLE_OF_THE_EBON_BLADE				= 60,
    PVPTITLE_ARCHMAGE						= 61,
    PVPTITLE_WARBRINGER						= 62,
    PVPTITLE_ASSASSIN						= 63,
    PVPTITLE_GRAND_MASTER_ALCHEMIST			= 64,
    PVPTITLE_GRAND_MASTER_BLACKSMITH		= 65,
    PVPTITLE_IRON_CHEF						= 66,
    PVPTITLE_GRAND_MASTER_ENCHANTER			= 67,
    PVPTITLE_GRAND_MASTER_ENGINEER			= 68,
    PVPTITLE_DOCTOR							= 69,
    PVPTITLE_GRAND_MASTER_ANGLER			= 70,
    PVPTITLE_GRAND_MASTER_HERBALIST			= 71,
    PVPTITLE_GRAND_MASTER_SCRIBE			= 72,
    PVPTITLE_GRAND_MASTER_JEWLCRAFTER		= 73,
    PVPTITLE_GRAND_MASTER_LETHERWORKER		= 74,
    PVPTITLE_GRAND_MASTER_MINER				= 75,
    PVPTITLE_GRAND_MASTER_SKINNER			= 76,
    PVPTITLE_GRAND_MASTER_TAILOR			= 77,
    PVPTITLE_OF_QUEL_THALAS					= 78,
    PVPTITLE_OF_ARGUS						= 79,
    PVPTITLE_OF_KHAZ_MODAN					= 80,
    PVPTITLE_OF_GNOMEREGAN					= 81,
    PVPTITLE_THE_LION_HEARTHED				= 82,
    PVPTITLE_CHAMPION_OF_ELUNE				= 83,
    PVPTITLE_HERO_OF_ORGIMMAR				= 84,
    PVPTITLE_PLAINSRUNNER					= 85,
    PVPTITLE_OF_THE_DARKSPEARTRIPE			= 86,
    PVPTITLE_THE_FORSAKEN					= 87,
    PVPTITLE_THE_MAGIC_SEEKER				= 88,
    PVPTITLE_TWILIGHT_VANQUISHER			= 89,
    PVPTITLE_CONQUEROR_OF_NAXXRAMAS			= 90,
    PVPTITLE_HERO_OF_NORTHREND				= 91,
    PVPTITLE_THE_HALLOWED					= 92,
    PVPTITLE_LOREMASTER						= 93,
    PVPTITLE_OF_THE_ALLIANCE				= 94,
    PVPTITLE_OF_THE_HORDE					= 95,
    PVPTITLE_THE_FLAWLESS_VICTOR			= 96,
    PVPTITLE_CHAMPION_OF_THE_FROZEN_WASTES	= 97,
    PVPTITLE_AMBASSADOR						= 98,
    PVPTITLE_THE_ARGENT_CHAMPION			= 99,
    PVPTITLE_GUARDIAN_OF_CENARIUS			= 100,
    PVPTITLE_BREWMASTER						= 101,
    PVPTITLE_MERRYMAKER						= 102,
    PVPTITLE_THE_LOVE_FOOL					= 103,
    PVPTITLE_MATRON							= 104,
    PVPTITLE_PATRON							= 105,
    PVPTITLE_OBSIDIAN_SLAYER				= 106,
    PVPTITLE_OF_THE_NIGHTFALL				= 107,
    PVPTITLE_THE_IMMORTAL					= 108,
    PVPTITLE_THE_UNDYING					= 109,
    PVPTITLE_JENKINS						= 110,
    PVPTITLE_BLOODSAIL_ADMIRAL				= 111,
    PVPTITLE_THE_INSANE						= 112,
    PVPTITLE_OF_THE_EXODAR					= 113,
    PVPTITLE_OF_DARNASSUS					= 114,
    PVPTITLE_OF_IRONFORGE					= 115,
    PVPTITLE_OF_STORMWIND					= 116,
    PVPTITLE_OF_ORGRIMMAR					= 117,
    PVPTITLE_OF_SENJIN						= 118,
    PVPTITLE_OF_SILVERMOON					= 119,
    PVPTITLE_OF_TUNDERBLUFF					= 120,
    PVPTITLE_OF_THE_UNDERCITY				= 121,
    PVPTITLE_THE_NOBLE						= 122,
    PVPTITLE_CRUSADER						= 123,
    PVPTITLE_DEATHS_DEMISE					= 124,
    PVPTITLE_CELESTIAL_DEFENDER				= 125,
    PVPTITLE_CONQUEROR_OF_ULDUAR			= 126,
    PVPTITLE_CHAMPION_OF_ULDUAR				= 127,
    PVPTITLE_VANQUISHER						= 128,
    PVPTITLE_STARCALLER						= 129,
    PVPTITLE_THE_ASTRAL_WALKER				= 130,
    PVPTITLE_HERALD_OF_THE_TITANS			= 131,
    PVPTITLE_FURIOUS_GLADIATOR				= 132,
    PVPTITLE_THE_PILGRIM					= 133,
    PVPTITLE_RELENTLESS_GLADIATOR			= 134,
    PVPTITLE_GRAND_CRUSADER					= 135,
    PVPTITLE_THE_ARGENT_DEFENDER			= 136,
    PVPTITLE_THE_PATIENT                    = 137,
    PVPTITLE_THE_LIGHT_OF_THE_DAWN          = 138,
    PVPTITLE_BANE_OF_THE_FALLEN_KING        = 139,
    PVPTITLE_THE_KINGSLAYER                 = 140,
    PVPTITLE_OF_THE_ASHEN_VERDICT           = 141,
    PVPTITLE_WRATHFUL_GLADIATOR             = 142,
    PVPTITLE_END                            = 143
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

enum PlayerFlags
{
    PLAYER_FLAG_PARTY_LEADER		= 0x01,
    PLAYER_FLAG_AFK					= 0x02,
    PLAYER_FLAG_DND					= 0x04,
    PLAYER_FLAG_GM					= 0x08,
    PLAYER_FLAG_DEATH_WORLD_ENABLE  = 0x10,
    PLAYER_FLAG_RESTING				= 0x20,
    PLAYER_FLAG_ADMIN				= 0x40,
    PLAYER_FLAG_FREE_FOR_ALL_PVP	= 0x80,
    PLAYER_FLAG_UNKNOWN2            = 0x100,
    PLAYER_FLAG_PVP_TOGGLE			= 0x200,
    PLAYER_FLAG_NOHELM				= 0x400,
    PLAYER_FLAG_NOCLOAK				= 0x800,
    PLAYER_FLAG_NEED_REST_3_HOURS	= 0x1000,
    PLAYER_FLAG_NEED_REST_5_HOURS	= 0x2000,
    PLAYER_FLAG_DEVELOPER			= 0x8000,
    PLAYER_FLAG_PVP					= 0x40000,
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
    LOOT_MILLING				= 2,
    LOOT_INSIGNIA               = 2                         // 7 unsupported by client, sending LOOT_SKINNING instead
};

enum ModType
{
    MOD_MELEE	= 0,
    MOD_RANGED	= 1,
    MOD_SPELL	= 2
};

struct spells
{
	uint16  spellId;
	uint16  slotId;
};

enum DrunkenState
{
    DRUNKEN_SOBER	= 0,
    DRUNKEN_TIPSY	= 1,
    DRUNKEN_DRUNK	= 2,
    DRUNKEN_SMASHED	= 3
};

/**
	TalentTree table

	mage - arcane - 81
	mage - fire - 41
	mage - frost - 61

	rogue - assassination - 182
	rogue - combat - 181
	rogue - subelty - 183

	warlock - affliction - 302
	warlock - demonology - 303
	warlock - destruction - 301

	warrior - arms - 161
	warrior - fury - 163
	warrior - protection - 164

	shaman - elemental - 261
	shaman - enchantment - 263
	shaman - restoration - 262

	paladin - holy - 382
	paladin - protection - 383
	paladin - retribution - 381

	death knight - blood - 398
	death knight - frost - 399
	death knight - unholy - 400

	priest - discipline - 201
	priest - holy - 202
	priest - shadow - 203

	hunter - beast - 361
	hunter - marksmanship - 363
	hunter - survival - 362

	druid - balance - 283
	druid - feral combat - 281
	druid - restoration - 282
*/

static const uint32 TalentTreesPerClass[DRUID + 1][3] =
{
	{ 0, 0, 0 },        // NONE
	{ 161, 163, 164 },  // WARRIOR
	{ 382, 383, 381 },  // PALADIN
	{ 361, 363, 362 },  // HUNTER
	{ 182, 181, 183 },  // ROGUE
	{ 201, 202, 203 },  // PRIEST
	{ 398, 399, 400 },  // DEATH KNIGHT
	{ 261, 263, 262 },  // SHAMAN
	{ 81, 41, 61 },     // MAGE
	{ 302, 303, 301 },  // WARLOCK
	{ 0, 0, 0 },        // NONE
	{ 283, 281, 282 },  // DRUID
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
	uint32  action; //VLack: Aspire has this as 32bit
	uint32  type;
	uint32  misc;
};

struct PlayerCreateInfo
{
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
	uint32  introid;
	uint32 taximask[12];
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

struct FactionReputation
{
	int32 standing;
	uint8 flag;
	int32 baseStanding;
	int32 CalcStanding() { return standing - baseStanding; }
	bool Positive() { return standing >= 0; }
};

typedef HM_NAMESPACE::hash_map<uint32, uint32> PlayerInstanceMap;
class SERVER_DECL PlayerInfo
{
	public:
		~PlayerInfo();

		uint32 guid;
		uint32 acct;
		char* name;
		uint32 race;
		uint32 gender;
		uint32 cl;
		uint32 team;

		time_t lastOnline;
		uint32 lastZone;
		uint32 lastLevel;
		Group* m_Group;
		int8 subGroup;
		Mutex savedInstanceIdsLock;
		PlayerInstanceMap savedInstanceIds[NUM_INSTANCE_MODES];

		Player* m_loggedInPlayer;
		Guild* guild;
		GuildRank* guildRank;
		GuildMember* guildMember;
};

struct PlayerPet
{
	string name;
	uint32 entry;
	uint32 xp;
	bool active;
	bool alive;
	char stablestate;
	uint32 number;
	uint32 level;
	uint32 happinessupdate;
	string actionbar;
	time_t reset_time;
	uint32 reset_cost;
	uint32 spellid;
	uint32 petstate;
	uint32 talentpoints;
	uint32 current_power;
	uint32 current_hp;
	uint32 current_happiness;
	uint32 renamable;
	uint32 type;
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
#ifdef ENABLE_ACHIEVEMENTS
class AchievementMgr;
#endif
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
#define PLAYER_FORCED_RESURRECT_INTERVAL	360000 // 1000*60*6= 6 minutes

struct PlayerSkill
{
	skilllineentry* Skill;
	uint32 CurrentValue;
	uint32 MaximumValue;
	uint32 BonusValue;
	float GetSkillUpChance();
	void Reset(uint32 Id);
};

enum SPELL_INDEX2
{
    SPELL_TYPE2_PALADIN_AURA			= 1,
    SPELL_TYPE3_DEATH_KNIGHT_AURA		= 1,
};

#define PLAYER_RATING_MODIFIER_RANGED_SKILL						PLAYER_FIELD_COMBAT_RATING_1
#define PLAYER_RATING_MODIFIER_DEFENCE							PLAYER_FIELD_COMBAT_RATING_1+1
#define PLAYER_RATING_MODIFIER_DODGE							PLAYER_FIELD_COMBAT_RATING_1+2
#define PLAYER_RATING_MODIFIER_PARRY							PLAYER_FIELD_COMBAT_RATING_1+3
#define PLAYER_RATING_MODIFIER_BLOCK							PLAYER_FIELD_COMBAT_RATING_1+4
#define PLAYER_RATING_MODIFIER_MELEE_HIT						PLAYER_FIELD_COMBAT_RATING_1+5
#define PLAYER_RATING_MODIFIER_RANGED_HIT						PLAYER_FIELD_COMBAT_RATING_1+6
#define PLAYER_RATING_MODIFIER_SPELL_HIT						PLAYER_FIELD_COMBAT_RATING_1+7
#define PLAYER_RATING_MODIFIER_MELEE_CRIT						PLAYER_FIELD_COMBAT_RATING_1+8
#define PLAYER_RATING_MODIFIER_RANGED_CRIT						PLAYER_FIELD_COMBAT_RATING_1+9
#define PLAYER_RATING_MODIFIER_SPELL_CRIT						PLAYER_FIELD_COMBAT_RATING_1+10
#define PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE				PLAYER_FIELD_COMBAT_RATING_1+11 // Not 100% sure but the numbers line up
#define PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE				PLAYER_FIELD_COMBAT_RATING_1+12 // GUESSED
#define PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE				PLAYER_FIELD_COMBAT_RATING_1+13 // GUESSED
#define PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE			PLAYER_FIELD_COMBAT_RATING_1+14
#define PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE			PLAYER_FIELD_COMBAT_RATING_1+15
#define PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE			PLAYER_FIELD_COMBAT_RATING_1+16
#define PLAYER_RATING_MODIFIER_MELEE_HASTE						PLAYER_FIELD_COMBAT_RATING_1+17
#define PLAYER_RATING_MODIFIER_RANGED_HASTE						PLAYER_FIELD_COMBAT_RATING_1+18
#define PLAYER_RATING_MODIFIER_SPELL_HASTE						PLAYER_FIELD_COMBAT_RATING_1+19
#define PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL			PLAYER_FIELD_COMBAT_RATING_1+20
#define PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL				PLAYER_FIELD_COMBAT_RATING_1+21
#define PLAYER_RATING_MODIFIER_MELEE_RANGED_SKILL				PLAYER_FIELD_COMBAT_RATING_1+22
#define PLAYER_RATING_MODIFIER_EXPERTISE						PLAYER_FIELD_COMBAT_RATING_1+23
#define PLAYER_RATING_MODIFIER_ARMOR_PENETRATION_RATING			PLAYER_FIELD_COMBAT_RATING_1+24

class ArenaTeam;
struct PlayerCooldown
{
	uint32 ExpireTime;
	uint32 ItemId;
	uint32 SpellId;
};

class PlayerSpec{
public:
	PlayerSpec(){
		tp = 0;
	}

	void SetTP( uint32 points ){ tp = points; }

	uint32 GetTP() const{ return tp; }

	void Reset(){
		tp += talents.size();
		talents.clear();
	}

	void AddTalent(uint32 talentid, uint8 rankid);

	std::map<uint32, uint8> talents;
	uint16 glyphs[GLYPHS_COUNT];
	ActionButton mActions[PLAYER_ACTION_BUTTON_COUNT];
	
private:
	uint32 tp;
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

class SERVER_DECL Player : public Unit
{
		friend class WorldSession;
		friend class Pet;
		friend class SkillIterator;

	public:
		Player(uint32 guid);
		~Player();

		PlayerCache* m_cache;

		virtual bool IsMage() { return false; }
		virtual bool IsDeathKnight() { return false; }
		virtual bool IsPriest() { return false; }
		virtual bool IsRogue() { return false; }
		virtual bool IsShaman() { return false; }
		virtual bool IsHunter() { return false; }
		virtual bool IsWarlock() { return false; }
		virtual bool IsWarrior() { return false; }
		virtual bool IsPaladin() { return false; }
		virtual bool IsDruid() { return false; }

		void HandleUpdateFieldChanged(uint32 index)
		{
			if(index == PLAYER_FLAGS)
				m_cache->SetUInt32Value(CACHE_PLAYER_FLAGS, GetUInt32Value(PLAYER_FLAGS));
		}

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
		void _LearnSkillSpells(uint32 SkillLine, uint32 Current);


		void RecalculateHonor();

		LfgMatch* m_lfgMatch;
		uint32 m_lfgInviterGuid;

		void EventTimeoutLfgInviter();

		// Summon and Appear Blocking
		void DisableSummon(bool disable) { disableSummon = disable; }
		bool IsSummonDisabled() { return disableSummon; }
		void DisableAppear(bool disable) { disableAppear = disable; }
		bool IsAppearDisabled() { return disableAppear; }

		// Unstuck Cooldown, used in ExtraScripts
		time_t GetLastUnstuckTime() { return m_unstuckCooldown; }
		void SetLastUnstuckTime(time_t t) { m_unstuckCooldown = t; }

		// Scripting
		void SendChatMessage(uint8 type, uint32 lang, const char* msg, uint32 delay = 0);
		void SendChatMessageToPlayer(uint8 type, uint32 lang, const char* msg, Player* plr);

	protected:

		void _UpdateSkillFields();

		SkillMap m_skills;

		// Summon and Appear Blocking
		bool disableAppear;
		bool disableSummon;

		// COOLDOWNS
		uint32 m_lastPotionId;
		PlayerCooldownMap m_cooldownMap[NUM_COOLDOWN_TYPES];
		uint32 m_globalCooldown;
		time_t m_unstuckCooldown; // For the Unstuck Script in ExtraScripts module

		/***********************************************************************************
			AFTER THIS POINT, public and private ARE PASSED AROUND LIKE A CHEAP WH*RE :P
			Let's keeps thing clean (use encapsulation) above this line. Thanks.
		***********************************************************************************/

	public:
		void SetLastPotion(uint32 itemid) { m_lastPotionId = itemid; }
		void Cooldown_AddStart(SpellEntry* pSpell);
		void Cooldown_Add(SpellEntry* pSpell, Item* pItemCaster);
		void Cooldown_AddItem(ItemPrototype* pProto, uint32 x);
		bool Cooldown_CanCast(SpellEntry* pSpell);
		bool Cooldown_CanCast(ItemPrototype* pProto, uint32 x);
		void UpdatePotionCooldown();
		bool HasSpellWithAuraNameAndBasePoints( uint32 auraname, uint32 basepoints );

	protected:
		void _Cooldown_Add(uint32 Type, uint32 Misc, uint32 Time, uint32 SpellId, uint32 ItemId);
		void _LoadPlayerCooldowns(QueryResult* result);
		void _SavePlayerCooldowns(QueryBuffer* buf);

		// END COOLDOWNS

	public:
		void RemoveItemByGuid(uint64 GUID);

		//! Okay to remove from world
		bool ok_to_remove;
		void OnLogin();//custom stuff on player login.
		void SendMeetingStoneQueue(uint32 DungeonId, uint8 Status);
		void SendDungeonDifficulty();
		void SendRaidDifficulty();
		void SendInstanceDifficulty(uint32 difficulty);
		void SendExploreXP(uint32 areaid, uint32 xp);
		void SendDestroyObject(uint64 GUID);
		void SendEquipmentSetList();
		void SendEquipmentSetSaved(uint32 setID, uint32 setGUID);
		void SendEquipmentSetUseResult(uint8 result);
		void SendEmptyPetSpellList();

		/////////////////////////////////////////////////////////////////////////////////////////
		//void SendTotemCreated( uint8 slot, uint64 GUID, uint32 duration, uint32 spellid )
		//  Notifies the client about the creation of a Totem/Summon
		//  ( client will show a right-clickable icon with a timer that can cancel the summon )
		//
		//Parameter(s)
		//  uint8 slot       -  Summon slot number
		//  uint64 GUID      -  GUID of the summon
		//  uint32 duration  -  Duration of the summon ( the timer of the icon )
		//  uint32 spellid   -  ID of the spell that created this summon
		//
		//Return Value
		//  None
		//
		//
		/////////////////////////////////////////////////////////////////////////////////////////
		void SendTotemCreated(uint8 slot, uint64 GUID, uint32 duration, uint32 spellid);

		void SendInitialWorldstates();

		void OutPacket(uint16 opcode, uint16 len, const void* data);
		void SendPacket(WorldPacket* packet);
		void SendMessageToSet(WorldPacket* data, bool self, bool myteam_only = false);
		void OutPacketToSet(uint16 Opcode, uint16 Len, const void* Data, bool self);

		void AddToWorld();
		void AddToWorld(MapMgr* pMapMgr);
		void RemoveFromWorld();
		bool Create(WorldPacket & data);

		void Update(uint32 time);
		void BuildFlagUpdateForNonGroupSet(uint32 index, uint32 flag);
		void BuildPetSpellList(WorldPacket & data);
		void SetAFKReason(std::string reason) { m_cache->SetStringValue(CACHE_AFK_DND_REASON, reason); };
		const char* GetName() { return m_name.c_str(); }
		std::string* GetNameString() { return &m_name; }
		void Die();
		//void KilledMonster(uint32 entry, const uint64 &guid);
		void GiveXP(uint32 xp, const uint64 & guid, bool allowbonus);  // to stop rest xp being given
		void ModifyBonuses(uint32 type, int32 val, bool apply);
		void CalcExpertise();
		std::map<uint32, uint32> m_wratings;

		ArenaTeam* m_arenaTeams[NUM_ARENA_TEAM_TYPES];

		/************************************************************************/
		/* Taxi                                                                 */
		/************************************************************************/
		TaxiPath*    GetTaxiPath() { return m_CurrentTaxiPath; }
		bool         GetTaxiState() { return m_onTaxi; }
		const uint32    &   GetTaximask(uint8 index) const { return m_taximask[index]; }
		void                LoadTaxiMask(const char* data);
		void                TaxiStart(TaxiPath* path, uint32 modelid, uint32 start_node);
		void                JumpToEndTaxiNode(TaxiPath* path);
		void                EventDismount(uint32 money, float x, float y, float z);
		void                EventTaxiInterpolate();

		void         SetTaxiState(bool state) { m_onTaxi = state; }
		void         SetTaximask(uint8 index, uint32 value) { m_taximask[index] = value; }
		void         SetTaxiPath(TaxiPath* path) { m_CurrentTaxiPath = path; }
		void         SetTaxiPos()	{m_taxi_pos_x = m_position.x; m_taxi_pos_y = m_position.y; m_taxi_pos_z = m_position.z;}
		void         UnSetTaxiPos()	{m_taxi_pos_x = 0; m_taxi_pos_y = 0; m_taxi_pos_z = 0; }

		// Taxi related variables
		vector<TaxiPath*>   m_taxiPaths;
		TaxiPath*           m_CurrentTaxiPath;
		uint32              taxi_model_id;
		uint32              lastNode;
		uint32              m_taxi_ride_time;
		uint32              m_taximask[12];
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
		QuestLogEntry* GetQuestLogInSlot(uint32 slot)  { return m_questlog[slot]; }
		uint32        GetQuestSharer()                { return m_questSharer; }

		void         SetQuestSharer(uint32 guid)     { m_questSharer = guid; }
		void                SetQuestLogSlot(QuestLogEntry* entry, uint32 slot);

		void         PushToRemovedQuests(uint32 questid)	{ m_removequests.insert(questid);}
		void			PushToFinishedDailies(uint32 questid) { DailyMutex.Acquire(); m_finishedDailies.insert(questid); DailyMutex.Release();}
		bool		HasFinishedDaily(uint32 questid) { return (m_finishedDailies.find(questid) == m_finishedDailies.end() ? false : true); }
		void                AddToFinishedQuests(uint32 quest_id);
		void				AreaExploredOrEventHappens(uint32 questId);   // scriptdev2

		bool                HasFinishedQuest(uint32 quest_id);

		
		void EventTimedQuestExpire( uint32 questid );


		/////////////////////////////////////////////////////
		//bool HasTimedQuest()
		//  Tells if the Player has a timed quest already
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns true if the Player already has a timed quest
		//  Returns false otherwise
		//
		//
		/////////////////////////////////////////////////////
		bool HasTimedQuest();


		/////////////////////////////////////////////////////
		//void ClearQuest( uint32 id )
		//  Clears the finished status of a quest
		//
		//Parameters
		//  uint32 id  -  Identifier of the quest
		//
		//Return Value
		//  None
		//
		/////////////////////////////////////////////////////
		void ClearQuest(uint32 id);

		bool				GetQuestRewardStatus(uint32 quest_id);
		bool                HasQuestForItem(uint32 itemid);
		bool                CanFinishQuest(Quest* qst);
		bool                HasQuestSpell(uint32 spellid);
		void                RemoveQuestSpell(uint32 spellid);
		bool                HasQuestMob(uint32 entry);
		bool                HasQuest(uint32 entry);
		void                RemoveQuestMob(uint32 entry);
		void                AddQuestKill(uint32 questid, uint8 reqid, uint32 delay = 0);


		/////////////////////////////////////////////////////////////
		//void AcceptQuest( uint64 guid, uint32 quest_id )
		//  Checks if the quest is acceptable from that questgiver.
		//  and accepts it.
		//
		//Parameters
		//  uin64 guid      -  guid of the questgiver
		//  uint32 quest_id -  id of the quest
		//
		//Return Value
		//  None.
		//
		////////////////////////////////////////////////////////////
		void AcceptQuest(uint64 guid, uint32 quest_id);

		//Quest related variables
		QuestLogEntry*      m_questlog[ MAX_QUEST_LOG_SIZE ];
		std::set<uint32>    m_removequests;
		std::set<uint32>    m_finishedQuests;
		Mutex				DailyMutex;
		std::set<uint32>	m_finishedDailies;
		uint32              m_questSharer;
		std::set<uint32>    quest_spells;
		std::set<uint32>    quest_mobs;

		void EventPortToGM(Player* p);
		uint32 GetTeam() { return m_team; }
		uint32 GetTeamInitial() { return myRace->team_id == 7 ? TEAM_ALLIANCE : TEAM_HORDE; }
		void SetTeam(uint32 t) { m_team = t; m_bgTeam = t; }
		void ResetTeam() { m_team = myRace->team_id == 7 ? TEAM_ALLIANCE : TEAM_HORDE; m_bgTeam = m_team; }
		bool IsTeamHorde() { return m_team == TEAM_HORDE; }
		bool IsTeamAlliance() { return m_team == TEAM_ALLIANCE; }

		bool IsInFeralForm()
		{
			int s = GetShapeShift();
			if(s <= 0)
				return false;

			//Fight forms that do not use player's weapon
			return (s == FORM_BEAR || s == FORM_DIREBEAR || s == FORM_CAT);
			//Shady: actually ghostwolf form doesn't use weapon too.
		}

		void CalcDamage();

		int32 GetDamageDoneMod(uint32 school)
		{
			if(school >= SCHOOL_COUNT)
				return 0;

			return static_cast< int32 >(GetPosDamageDoneMod(school)) - static_cast< int32 >(GetNegDamageDoneMod(school));
		}

		float GetDamageDonePctMod(uint32 school)
		{
			if(school >= SCHOOL_COUNT)
				return 0;

			return m_floatValues[ PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + school ];
		}

		uint32 GetMainMeleeDamage(uint32 AP_owerride); // I need this for windfury
		uint32 GetMaxLevel() { return GetUInt32Value(PLAYER_FIELD_MAX_LEVEL); }

		const uint64 & GetSelection() const { return m_curSelection; }
		const uint64 & GetTarget() const { return m_curTarget; }
		void SetSelection(const uint64 & guid) { m_curSelection = guid; }
		void SetTarget(const uint64 & guid) { m_curTarget = guid; }

		/************************************************************************/
		/* Spells                                                               */
		/************************************************************************/
		bool HasSpell(uint32 spell);
		bool HasSpellwithNameHash(uint32 hash);
		bool HasDeletedSpell(uint32 spell);
		void smsg_InitialSpells();
		void smsg_TalentsInfo(bool SendPetTalents);
		void ActivateSpec(uint8 spec);
		void addSpell(uint32 spell_idy);
		void removeSpellByHashName(uint32 hash);
		bool removeSpell(uint32 SpellID, bool MoveToDeleted, bool SupercededSpell, uint32 SupercededSpellID);
		bool removeDeletedSpell(uint32 SpellID);
		void SendPreventSchoolCast(uint32 SpellSchool, uint32 unTimeMs);

		// PLEASE DO NOT INLINE!
		void AddOnStrikeSpell(SpellEntry* sp, uint32 delay)
		{
			m_onStrikeSpells.insert(map< SpellEntry*, pair<uint32, uint32> >::value_type(sp, make_pair(delay, 0)));
		}
		void RemoveOnStrikeSpell(SpellEntry* sp)
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
		bool                IsHostileBasedOnReputation(FactionDBC* dbc);
		void                UpdateInrangeSetsBasedOnReputation();
		void                Reputation_OnKilledUnit(Unit* pUnit, bool InnerLoop);
		void                Reputation_OnTalk(FactionDBC* dbc);
		static Standing     GetReputationRankFromStanding(int32 Standing_);
		void SetFactionInactive(uint32 faction, bool set);
		bool AddNewFaction(FactionDBC* dbc, int32 standing, bool base);
		void OnModStanding(FactionDBC* dbc, FactionReputation* rep);
		uint32              GetExaltedCount(void);

		/************************************************************************/
		/* Factions                                                             */
		/************************************************************************/
		void smsg_InitialFactions();
		uint32 GetInitialFactionId();
		// factions variables
		int32 pctReputationMod;

		/************************************************************************/
		/* PVP                                                                  */
		/************************************************************************/
		uint8 GetPVPRank()
		{
			return (uint8)((GetUInt32Value(PLAYER_BYTES_3) >> 24) & 0xFF);
		}
		void SetPVPRank(int newrank)
		{
			SetUInt32Value(PLAYER_BYTES_3, ((GetUInt32Value(PLAYER_BYTES_3) & 0x00FFFFFF) | (uint8(newrank) << 24)));
		}
		uint32 GetMaxPersonalRating();

		bool HasTitle(RankTitles title)
		{
			return (GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES + ((title >> 6) << 1)) & (uint64(1) << (title % 64))) != 0;
		}
		void SetKnownTitle(RankTitles title, bool set);
		void SendAvailSpells(SpellShapeshiftForm* ssf, bool active);

		/************************************************************************/
		/* Groups                                                               */
		/************************************************************************/
		void                SetInviter(uint32 pInviter) { m_GroupInviter = pInviter; }
		uint32       GetInviter() { return m_GroupInviter; }
		bool         InGroup() { return (m_playerInfo->m_Group != NULL && !m_GroupInviter); }
		bool                IsGroupLeader()
		{
			if(m_playerInfo->m_Group != NULL)
			{
				if(m_playerInfo->m_Group->GetLeader() == m_playerInfo)
					return true;
			}
			return false;
		}
		int          HasBeenInvited() { return m_GroupInviter != 0; }
		Group*       GetGroup() { return m_playerInfo ? m_playerInfo->m_Group : NULL; }
		int8		   GetSubGroup() { return m_playerInfo->subGroup; }
		bool                IsGroupMember(Player* plyr);
		bool         IsBanned()
		{
			if(m_banned)
			{
				if(m_banned < 100 || (uint32)UNIXTIME < m_banned)
					return true;
			}
			return false;
		}
		void         SetBanned() { m_banned = 4;}
		void         SetBanned(string Reason) { m_banned = 4; m_banreason = Reason;}
		void         SetBanned(uint32 timestamp, string & Reason) { m_banned = timestamp; m_banreason = Reason; }
		void         UnSetBanned() { m_banned = 0; }
		string       GetBanReason() {return m_banreason;}

		/************************************************************************/
		/* Guilds                                                               */
		/************************************************************************/
		Guild* 		GetGuild() { return m_playerInfo->guild; }
		bool			IsInGuild() {return (m_uint32Values[PLAYER_GUILDID] != 0) ? true : false;}
		uint32		GetGuildId() { return m_uint32Values[PLAYER_GUILDID]; }
		void						SetGuildId(uint32 guildId);
		uint32		GetGuildRank() { return m_uint32Values[PLAYER_GUILDRANK]; }
		GuildRank*	GetGuildRankS() { return m_playerInfo->guildRank; }
		void						SetGuildRank(uint32 guildRank);
		uint32						GetGuildInvitersGuid() { return m_invitersGuid; }
		void						SetGuildInvitersGuid(uint32 guid) { m_invitersGuid = guid; }
		void						UnSetGuildInvitersGuid() { m_invitersGuid = 0; }
		GuildMember* GetGuildMember() { return m_playerInfo->guildMember; }

		/************************************************************************/
		/* Duel                                                                 */
		/************************************************************************/
		void                RequestDuel(Player* pTarget);
		void                DuelBoundaryTest();
		void                EndDuel(uint8 WinCondition);
		void                DuelCountdown();
		void                SetDuelStatus(uint8 status) { m_duelStatus = status; }
		uint8        GetDuelStatus() { return m_duelStatus; }
		void                SetDuelState(uint8 state) { m_duelState = state; }
		uint8        GetDuelState() { return m_duelState; }
		// duel variables
		Player*             DuelingWith;
		void				SetDuelArbiter(uint64 guid) { SetUInt64Value(PLAYER_DUEL_ARBITER, guid); }
		uint64				GetDuelArbiter() { return GetUInt64Value(PLAYER_DUEL_ARBITER); }
		void				SetDuelTeam(uint32 team) { SetUInt32Value(PLAYER_DUEL_TEAM, team); }
		uint32				GetDuelTeam() { return GetUInt32Value(PLAYER_DUEL_TEAM); }

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
		void			AddSummon(Pet* pet) { m_Summons.push_front(pet); }
		Pet*			GetSummon(void) //returns 1st summon
		{
			if(!m_Summons.empty())
				return m_Summons.front();
			else
				return NULL;
		}
		std::list<Pet*> GetSummons(void) { return m_Summons; }
		void			RemoveSummon(Pet* pet)
		{
			for(std::list<Pet*>::iterator itr = m_Summons.begin(); itr != m_Summons.end(); ++itr)
			{
				if((*itr)->GetGUID() == pet->GetGUID())
				{
					m_Summons.erase(itr);
					break;
				}
			}
		}
		uint32						GeneratePetNumber(void);
		void						RemovePlayerPet(uint32 pet_number);
		void			AddPlayerPet(PlayerPet* pet, uint32 index) { m_Pets[index] = pet; }
		PlayerPet*	GetPlayerPet(uint32 idx)
		{
			std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(idx);
			if(itr != m_Pets.end()) return itr->second;
			else
				return NULL;
		}
		void						SpawnPet(uint32 pet_number);
		void						SpawnActivePet();
		void						DismissActivePets();
		uint8         GetPetCount(void) { return (uint8)m_Pets.size(); }
		void			SetStableSlotCount(uint8 count) { m_StableSlotCount = count; }
		uint8			GetStableSlotCount(void) { return m_StableSlotCount; }
		uint32						GetUnstabledPetNumber(void)
		{
			if(m_Pets.size() == 0) return 0;
			std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin();
			for(; itr != m_Pets.end(); itr++)
				if(itr->second->stablestate == STABLE_STATE_ACTIVE)
					return itr->first;
			return 0;
		}
		void						EventSummonPet(Pet* new_pet); //if we charmed or simply summoned a pet, this function should get called
		void						EventDismissPet(); //if pet/charm died or whatever happened we should call this function

		/************************************************************************/
		/* Item Interface                                                       */
		/************************************************************************/
		ItemInterface* GetItemInterface() { return m_ItemInterface; } // Player Inventory Item storage
		void			ApplyItemMods(Item* item, int16 slot, bool apply, bool justdrokedown = false) {  _ApplyItemMods(item, slot, apply, justdrokedown); }
		bool			HasItemCount(uint32 item, uint32 count, bool inBankAlso = false) const;
		// item interface variables
		ItemInterface*      m_ItemInterface;
		int32 GetVisibleBase(int16 slot) { return (PLAYER_VISIBLE_ITEM_1_ENTRYID + (slot * PLAYER_VISIBLE_ITEM_LENGTH)); }

		/************************************************************************/
		/* Loot                                                                 */
		/************************************************************************/
		const uint64 & GetLootGUID() const { return m_lootGuid; }
		void SetLootGUID(const uint64 & guid) { m_lootGuid = guid; }
		void SendLoot(uint64 guid, uint8 loot_type, uint32 mapid);
		void SendLootUpdate(Object* o);
		void TagUnit(Object* o);
		void SendPartyKillLog(uint64 GUID);
		// loot variables
		uint64              m_lootGuid;
		uint64              m_currentLoot;
		bool                bShouldHaveLootableOnCorpse;

		/************************************************************************/
		/* World Session                                                        */
		/************************************************************************/
		WorldSession* GetSession() const { return m_session; }
		void SetSession(WorldSession* s) { m_session = s; }
		void SetBindPoint(float x, float y, float z, uint32 m, uint32 v) { m_bind_pos_x = x; m_bind_pos_y = y; m_bind_pos_z = z; m_bind_mapid = m; m_bind_zoneid = v;}
		void SendDelayedPacket(WorldPacket* data, bool bDeleteOnSend)
		{
			if(data == NULL) return;
			if(GetSession() != NULL) GetSession()->SendPacket(data);
			if(bDeleteOnSend) delete data;
		}
		float offhand_dmg_mod;
		int GetSpellDamageMod(uint32 id);
		int32 GetSpellManaMod(uint32 id);

		// Talents
		// These functions build a specific type of A9 packet
		uint32  BuildCreateUpdateBlockForPlayer(ByteBuffer* data, Player* target);
		void DestroyForPlayer(Player* target) const;
		void SetTalentHearthOfWildPCT(int value) {hearth_of_wild_pct = value;}
		void EventTalentHearthOfWildChange(bool apply);

		std::list<LoginAura> loginauras;

		/************************************************************************/
		/* Player loading and savings                                           */
		/* Serialize character to db                                            */
		/************************************************************************/
		void SaveToDB(bool bNewCharacter);
		void SaveAuras(stringstream &);
		bool LoadFromDB(uint32 guid);
		void LoadFromDBProc(QueryResultVector & results);

		bool LoadSpells(QueryResult* result);
		bool SaveSpells(bool NewCharacter, QueryBuffer* buf);

		bool LoadDeletedSpells(QueryResult* result);
		bool SaveDeletedSpells(bool NewCharacter, QueryBuffer* buf);

		bool LoadReputations( QueryResult *result );
		bool SaveReputations( bool NewCharacter, QueryBuffer *buf );

		bool LoadSkills(QueryResult* result);
		bool SaveSkills(bool NewCharacter, QueryBuffer* buf);

		void LoadNamesFromDB(uint32 guid);
		bool m_FirstLogin;

		/************************************************************************/
		/* Drunk system                                                         */
		/************************************************************************/
		void SetDrunkValue(uint16 newDrunkValue, uint32 itemid = 0);
		uint16 GetDrunkValue() const { return m_drunk; }
		static DrunkenState GetDrunkenstateByValue(uint16 value);
		void HandleSobering();

		uint32 m_drunkTimer;
		uint16 m_drunk;

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
		void SetSpeeds( uint8 type, float speed );
		float GetPlayerSpeed() {return m_runSpeed;}
		uint8 m_currentMovement;
		bool m_isMoving; // moving + strafing + jumping
		bool moving;
		bool strafing;
		bool isTurning;
		bool jumping;
		//Invisibility stuff
		bool m_isGmInvisible;

		/************************************************************************/
		/* Channel stuff                                                        */
		/************************************************************************/
		void JoinedChannel(Channel* c);
		void LeftChannel(Channel* c);
		void CleanupChannels();

		/************************************************************************/
		/* Attack stuff															*/
		/************************************************************************/
		void EventAttackStart();
		void EventAttackStop();
		void EventAttackUpdateSpeed() { }
		void EventDeath();
		//Note:ModSkillLine -> value+=amt;ModSkillMax -->value=amt; --weird
		float GetSkillUpChance(uint32 id);
		// std::list<struct skilllines>getSkillLines() { return m_skilllines; }
		float SpellHasteRatingBonus;
		void ModAttackSpeed(int32 mod, ModType type);
		void UpdateAttackSpeed();
		float GetDefenseChance(uint32 opLevel);
		float GetDodgeChance();
		float GetBlockChance();
		float GetParryChance();
		void UpdateChances();
		void UpdateStats();
		uint32 GetBlockDamageReduction();
		void ApplyFeralAttackPower(bool apply, Item* item = NULL);

		bool canCast(SpellEntry* m_spellInfo);
		float GetSpellCritFromSpell() { return m_spellcritfromspell; }
		float GetHitFromSpell() { return m_hitfromspell; }
		void SetSpellCritFromSpell(float value) { m_spellcritfromspell = value; }
		void SetHitFromSpell(float value) { m_hitfromspell = value; }
		uint32 GetHealthFromSpell() { return m_healthfromspell; }
		uint32 GetManaFromSpell() { return m_manafromspell; }
		void SetHealthFromSpell(uint32 value) { m_healthfromspell = value;}
		void SetManaFromSpell(uint32 value) { m_manafromspell = value;}
		uint32 CalcTalentResetCost(uint32 resetnum);
		void SendTalentResetConfirm();
		void SendPetUntrainConfirm();
		uint32 GetTalentResetTimes() { return m_talentresettimes; }
		void SetTalentResetTimes(uint32 value) { m_talentresettimes = value; }
		void SetPlayerStatus(uint8 pStatus) { m_status = pStatus; }
		uint8 GetPlayerStatus() { return m_status; }
		const float & GetBindPositionX() const { return m_bind_pos_x; }
		const float & GetBindPositionY() const { return m_bind_pos_y; }
		const float & GetBindPositionZ() const { return m_bind_pos_z; }
		const uint32 & GetBindMapId() const { return m_bind_mapid; }
		const uint32 & GetBindZoneId() const { return m_bind_zoneid; }

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
		uint32 GetTutorialInt(uint32 intId);
		void SetTutorialInt(uint32 intId, uint32 value);
		//Base stats calculations
		//void CalcBaseStats();
		// Rest
		uint32 SubtractRestXP(uint32 amount);
		void AddCalculatedRestXP(uint32 seconds);
		void ApplyPlayerRestState(bool apply);
		void UpdateRestState();

		// Gossip
		GossipMenu* CurrentGossipMenu;
		void CleanupGossipMenu();
		void Gossip_Complete();
		int m_lifetapbonus;
		bool m_requiresNoAmmo;

		// scriptdev2
		GossipMenu* PlayerTalkClass;
		void PrepareQuestMenu(uint64 guid);
		void SendGossipMenu(uint32 TitleTextId, uint64 npcGUID);
		void CloseGossip();


		bool m_bUnlimitedBreath;
		uint32 m_UnderwaterTime;
		uint32 m_UnderwaterState;
		// Visible objects
		bool CanSee(Object* obj);
		bool IsVisible(uint64 pObj) { return !(m_visibleObjects.find(pObj) == m_visibleObjects.end()); }
		void AddInRangeObject(Object* pObj);
		void OnRemoveInRangeObject(Object* pObj);
		void ClearInRangeSet();
		void AddVisibleObject(uint64 pObj) { m_visibleObjects.insert(pObj); }
		void RemoveVisibleObject(uint64 pObj) { m_visibleObjects.erase(pObj); }
		void RemoveVisibleObject(std::set< uint64 >::iterator itr) { m_visibleObjects.erase(itr); }
		std::set< uint64 >::iterator FindVisible(uint64 obj) { return m_visibleObjects.find(obj); }
		void RemoveIfVisible(uint64 obj);

		// Misc
		void EventCannibalize(uint32 amount);
		bool m_AllowAreaTriggerPort;
		void EventAllowTiggerPort(bool enable);
		void UpdatePowerAmm();
		uint32 m_modblockabsorbvalue;
		uint32 m_modblockvaluefromspells;
		void SendInitialLogonPackets();
		void Reset_Spells();
		void Reset_Talents();
		void Reset_AllTalents();
		// Battlegrounds xD
		CBattleground* m_bg;
		CBattleground* m_pendingBattleground;
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
		void EventCastRepeatedSpell(uint32 spellid, Unit* target);
		int32 CanShootRangedWeapon(uint32 spellid, Unit* target, bool autoshot);
		uint32 m_AutoShotDuration;
		uint32 m_AutoShotAttackTimer;
		bool m_onAutoShot;
		uint64 m_AutoShotTarget;
		SpellEntry* m_AutoShotSpell;
		void _InitialReputation();
		void EventActivateGameObject(GameObject* obj);
		void EventDeActivateGameObject(GameObject* obj);
		void UpdateNearbyGameObjects();

		void CalcResistance(uint32 type);
		float res_M_crit_get() {return m_resist_critical[0];}
		void res_M_crit_set(float newvalue) {m_resist_critical[0] = newvalue;}
		float res_R_crit_get() {return m_resist_critical[1];}
		void res_R_crit_set(float newvalue) {m_resist_critical[1] = newvalue;}
		uint32 FlatResistanceModifierPos[SCHOOL_COUNT];
		uint32 FlatResistanceModifierNeg[SCHOOL_COUNT];
		uint32 BaseResistanceModPctPos[SCHOOL_COUNT];
		uint32 BaseResistanceModPctNeg[SCHOOL_COUNT];
		uint32 ResistanceModPctPos[SCHOOL_COUNT];
		uint32 ResistanceModPctNeg[SCHOOL_COUNT];
		float m_resist_critical[2];//when we are a victim we can have talents to decrease chance for critical hit. This is a negative value and it's added to critchances
		float m_resist_hit[2];			// 0 = melee; 1= ranged;
		int32 m_resist_hit_spell[SCHOOL_COUNT];	// spell resist per school
		float m_attack_speed[3];
		float SpellHealDoneByAttribute[5][SCHOOL_COUNT];
		uint32 m_modphyscritdmgPCT;
		uint32 m_RootedCritChanceBonus; // Class Script Override: Shatter
		uint32 m_IncreaseDmgSnaredSlowed;
		//uint32 FieryPaybackModHP35;		// for Fiery Payback

		void ApplyStatByAttrMod(uint8 dstType, uint8 dstIdx, uint8 srcType, uint8 srcIdx);

		uint32 m_ModInterrMRegenPCT;
		int32 m_ModInterrMRegen;
		float m_RegenManaOnSpellResist;
		uint32 m_casted_amount[SCHOOL_COUNT]; //Last casted spells amounts. Need for some spells. Like Ignite etc. DOesn't count HoTs and DoTs. Only directs

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
		uint32* GetPlayedtime() { return m_playedtime; };
		void CalcStat(uint32 t);
		float CalcRating(uint32 t);
		void RegenerateMana(bool is_interrupted);
		void RegenerateHealth(bool inCombat);
		void RegenerateEnergy();
		void LooseRage(int32 value);

		uint32 SoulStone;
		uint32 SoulStoneReceiver;
		void removeSoulStone();

		uint32 GetSoulStoneReceiver() {return SoulStoneReceiver;}
		void SetSoulStoneReceiver(uint32 StoneGUID) {SoulStoneReceiver = StoneGUID;}
		uint32 GetSoulStone() {return SoulStone;}
		void SetSoulStone(uint32 StoneID) {SoulStone = StoneID;}

		uint64 misdirectionTarget;

		uint64 GetMisdirectionTarget() {return misdirectionTarget;}
		void SetMisdirectionTarget(uint64 PlayerGUID) {misdirectionTarget = PlayerGUID;}

		bool bReincarnation;
		bool ignoreShapeShiftChecks;
		bool ignoreAuraStateCheck;

		map<uint32, WeaponModifier> damagedone;
		map<uint32, WeaponModifier> tocritchance;
		bool cannibalize;
		uint8 cannibalizeCount;
		int32 rageFromDamageDealt;
		int32 rageFromDamageTaken;
		// GameObject commands
		inline GameObject* GetSelectedGo()
		{
			if(m_GM_SelectedGO)
				return GetMapMgr()->GetGameObject((uint32)m_GM_SelectedGO);
			return NULL;
		}
		uint64 m_GM_SelectedGO;

		void _Relocate(uint32 mapid, const LocationVector & v, bool sendpending, bool force_new_world, uint32 instance_id);

		void AddItemsToWorld();
		void RemoveItemsFromWorld();
		void UpdateKnownCurrencies(uint32 itemId, bool apply);

		uint32 m_ShapeShifted;
		uint32 m_MountSpellId;
		uint32 mountvehicleid;

		bool IsMounted()
		{
			if(m_MountSpellId != 0)
				return true;
			else
				return false;
		}

		void Dismount()
		{
			if(m_MountSpellId != 0)
			{
				RemoveAura(m_MountSpellId);
				m_MountSpellId = 0;
			}
		}
		
		bool IsVehicle(){
			if( mountvehicleid != 0 )
				return true;
			else
				return false;
		}


		void AddVehicleComponent( uint32 creature_entry, uint32 vehicleid );

		void RemoveVehicleComponent();

		void SendMountResult(uint32 result)
		{
			WorldPacket data(SMSG_MOUNTRESULT, 4);
			data << (uint32)result;
			GetSession()->SendPacket(&data);
		}

		void SendDismountResult(uint32 result)
		{
			WorldPacket data(SMSG_DISMOUNTRESULT, 4);
			data << (uint32)result;
			GetSession()->SendPacket(&data);
		}

		bool bHasBindDialogOpen;
		uint32 TrackingSpell;
		void _EventCharmAttack();
		void _Kick();
		void Kick(uint32 delay = 0);
		void SoftDisconnect();
		uint32 m_KickDelay;
		uint64 m_CurrentCharm;
		Transporter* m_CurrentTransporter;

		Object* GetSummonedObject() {return m_SummonedObject;};
		void SetSummonedObject(Object* t_SummonedObject) {m_SummonedObject = t_SummonedObject;};

		void ClearCooldownsOnLine(uint32 skill_line, uint32 called_from);
		void ResetAllCooldowns();
		void ClearCooldownForSpell(uint32 spell_id);

		void Phase(uint8 command = PHASE_SET, uint32 newphase = 1);

		bool bProcessPending;
		Mutex _bufferS;
		void PushUpdateData(ByteBuffer* data, uint32 updatecount);
		void PushCreationData(ByteBuffer* data, uint32 updatecount);
		void PushOutOfRange(const WoWGuid & guid);
		void ProcessPendingUpdates();
		bool  CompressAndSendUpdateBuffer(uint32 size, const uint8* update_buffer);
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
		void UpdateChannels(uint16 AreaID);
		uint32 GetAreaID() { return m_AreaID; }
		void SetAreaID(uint32 area) { m_AreaID = area; }
		bool IsInCity();

		// Instance IDs

		uint32 GetPersistentInstanceId(uint32 mapId, uint32 difficulty)
		{
			if(mapId >= NUM_MAPS || difficulty >= NUM_INSTANCE_MODES || m_playerInfo == NULL)
				return 0;
			m_playerInfo->savedInstanceIdsLock.Acquire();
			PlayerInstanceMap::iterator itr = m_playerInfo->savedInstanceIds[difficulty].find(mapId);
			if(itr == m_playerInfo->savedInstanceIds[difficulty].end())
			{
				m_playerInfo->savedInstanceIdsLock.Release();
				return 0;
			}
			m_playerInfo->savedInstanceIdsLock.Release();
			return (*itr).second;
		}

		void SetPersistentInstanceId(Instance* pInstance);
		//Use this method carefully..
		void SetPersistentInstanceId(uint32 mapId, uint32 difficulty, uint32 instanceId);

		void SendAchievmentStatus(uint32 criteriaid, uint32 new_value, uint32 at_stamp);
		void SendTriggerMovie(uint32 movieID);

		// DualWield2H (ex: Titan's grip)
		bool DualWield2H;
		void ResetDualWield2H();
	public:

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
		void EventTeleportTaxi(uint32 mapid, float x, float y, float z);
		void ApplyLevelInfo(LevelInfo* Info, uint32 Level);
		void BroadcastMessage(const char* Format, ...);
		map<uint32, set<uint32> > SummonSpells;
		map<uint32, PetSpellMap*> PetSpells;
		void AddSummonSpell(uint32 Entry, uint32 SpellID);
		void RemoveSummonSpell(uint32 Entry, uint32 SpellID);
		set<uint32>* GetSummonSpells(uint32 Entry);
		LockedQueue<WorldPacket*> delayedPackets;
		uint32 m_UnderwaterMaxTime;
		uint32 m_UnderwaterLastDmg;
		LocationVector getMyCorpseLocation() const { return myCorpseLocation; }
		bool bCorpseCreateable;
		uint32 m_resurrectHealth, m_resurrectMana;
		uint32 m_resurrectInstanceID, m_resurrectMapId;
		LocationVector m_resurrectPosition;
		bool blinked;
		uint32 m_explorationTimer;
		// DBC stuff
		CharRaceEntry* myRace;
		CharClassEntry* myClass;
		Creature* linkTarget;
		bool ItemStackCheat;
		bool AuraStackCheat;
		bool TriggerpassCheat;
		bool SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O);
		bool SafeTeleport(uint32 MapID, uint32 InstanceID, const LocationVector & vec);
		void SafeTeleport(MapMgr* mgr, const LocationVector & vec);
		void EjectFromInstance();
		bool raidgrouponlysent;

		void SetDungeonDifficulty(uint32 diff);
		uint32 GetDungeonDifficulty();

		void SetRaidDifficulty(uint32 diff);
		uint32 GetRaidDifficulty();

		void EventSafeTeleport(uint32 MapID, uint32 InstanceID, LocationVector vec)
		{
			SafeTeleport(MapID, InstanceID, vec);
		}

		// Hack fix here!
		void ForceZoneUpdate();

		bool HasAreaExplored(AreaTable const*);
		bool HasOverlayUncovered(uint32 overlayID);

		/*****************
		  PVP Stuff
		******************/
		uint32 m_pvpTimer;

		bool IsPvPFlagged();
		void SetPvPFlag();
		void RemovePvPFlag();

		bool IsFFAPvPFlagged();
		void SetFFAPvPFlag();
		void RemoveFFAPvPFlag();

		bool IsSanctuaryFlagged();
		void SetSanctuaryFlag();
		void RemoveSanctuaryFlag();

		/////////////////////////////////////////////////
		// EASY FUNCTIONS - PLAYER GOLD
		/////////////////////////////////////////////////
		void SetGold(int32 coins)
		{
			SetUInt32Value(PLAYER_FIELD_COINAGE, coins);
		}
		void ModGold(int32 coins)
		{
			ModUnsigned32Value(PLAYER_FIELD_COINAGE, coins);
		}
		uint32 GetGold()
		{
			return GetUInt32Value(PLAYER_FIELD_COINAGE);
		}
		bool HasGold(uint32 coins)
		{
			return (GetUInt32Value(PLAYER_FIELD_COINAGE) >= coins);
		}

		/////////////////////////////////////////////////
		// DEPRICATED FUNCTIONS USE
		// SetGold and ModGold
		/////////////////////////////////////////////////
		void GiveGold(int32 coins)
		{
			ModUnsigned32Value(PLAYER_FIELD_COINAGE , coins);
		}
		void TakeGold(int32 coins)
		{
			ModUnsigned32Value(PLAYER_FIELD_COINAGE, -coins);
		}

		/////////////////////////////////////////////////
		// EASY FUNCTIONS - MISC
		/////////////////////////////////////////////////

		void SetChosenTitle(uint32 id) { SetUInt32Value(PLAYER_CHOSEN_TITLE, id); }

		void SetInventorySlot(uint32 slot, uint64 guid) { SetUInt64Value(PLAYER_FIELD_INV_SLOT_HEAD + (slot * 2), guid); }

		void SetFarsightTarget(uint64 guid) { SetUInt64Value(PLAYER_FARSIGHT, guid); }
		uint64 GetFarsightTarget() { return GetUInt64Value(PLAYER_FARSIGHT); }

		void SetXp(uint32 xp) { SetUInt32Value(PLAYER_XP, xp); }
		uint32 GetXp() { return GetUInt32Value(PLAYER_XP); }
		void SetNextLevelXp(uint32 xp) { SetUInt32Value(PLAYER_NEXT_LEVEL_XP, xp); }

		void SetTalentPointsForAllSpec( uint32 amt ){
			m_specs[ 0 ].SetTP( amt );
			m_specs[ 1 ].SetTP( amt );
			SetUInt32Value( PLAYER_CHARACTER_POINTS1, amt );
			smsg_TalentsInfo( false );
		}

		void AddTalentPointsToAllSpec( uint32 amt ){
			m_specs[ 0 ].SetTP( m_specs[ 0 ].GetTP() + amt );
			m_specs[ 1 ].SetTP( m_specs[ 1 ].GetTP() + amt );
			SetUInt32Value( PLAYER_CHARACTER_POINTS1, GetUInt32Value( PLAYER_CHARACTER_POINTS1 ) + amt );
			smsg_TalentsInfo( false );
		}

		void SetCurrentTalentPoints( uint32 points ){
			m_specs[ m_talentActiveSpec ].SetTP( points );
			SetUInt32Value( PLAYER_CHARACTER_POINTS1, points );
			smsg_TalentsInfo( false );
		}

		uint32 GetCurrentTalentPoints(){
			uint32 points = GetUInt32Value( PLAYER_CHARACTER_POINTS1 );
			Arcemu::Util::ArcemuAssert( points == m_specs[ m_talentActiveSpec ].GetTP() );
			return points;
		}

		void SetPrimaryProfessionPoints(uint32 amt) { SetUInt32Value(PLAYER_CHARACTER_POINTS2, amt); }
		void ModPrimaryProfessionPoints(int32 amt) { ModUnsigned32Value(PLAYER_CHARACTER_POINTS2, amt); }
		uint32 GetPrimaryProfessionPoints() { return GetUInt32Value(PLAYER_CHARACTER_POINTS2); }

		void ModPosDamageDoneMod(uint32 school, uint32 value) { ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, value); }
		uint32 GetPosDamageDoneMod(uint32 school) { return GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school); }

		void ModNegDamageDoneMod(uint32 school, uint32 value) { ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + school, value); }
		uint32 GetNegDamageDoneMod(uint32 school) { return GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + school); }

		void ModHealingDoneMod(uint32 value) { ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, value); }
		uint32 GetHealingDoneMod() { return GetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS); }

		void SetAmmoId(uint32 id) { SetUInt32Value(PLAYER_AMMO_ID, id); }
		uint32 GetAmmoId() { return GetUInt32Value(PLAYER_AMMO_ID); }

		void SetHonorCurrency(uint32 value) { SetUInt32Value(PLAYER_FIELD_HONOR_CURRENCY, value); }
		void ModHonorCurrency(uint32 value) { ModUnsigned32Value(PLAYER_FIELD_HONOR_CURRENCY, value); }
		uint32 GetHonorCurrency() { return GetUInt32Value(PLAYER_FIELD_HONOR_CURRENCY); }

		void SetArenaCurrency(uint32 value) { SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY, value); }
		void ModArenaCurrency(uint32 value) { ModUnsigned32Value(PLAYER_FIELD_ARENA_CURRENCY, value); }
		uint32 GetArenaCurrency() { return GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY); }

		void SetGlyph(uint32 slot, uint32 id) { SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot, id); }
		uint32 GetGlyph(uint32 slot) { return GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot); }

		//! Do this on /pvp off
		void ResetPvPTimer();
		//! Stop the timer for pvp off
		void StopPvPTimer() { m_pvpTimer = 0; }

		//! Called at login to add the honorless buff, etc.
		void LoginPvPSetup();
		//! Update our pvp area (called when zone changes)
		void UpdatePvPArea();
		//! PvP Toggle (called on /pvp)
		void PvPToggle();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// void HandleSpellLoot( uint32 itemid )
		//  Generates loot for the spell loot item (clams for example) , then adds the generated loot to the Player
		//
		// Parameters:
		//  uint32 itemid   -  unique numerical identifier of the item the Player is looting
		//
		// Return Value:
		//  None.
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void HandleSpellLoot(uint32 itemid);



		////////////////////////////////////////////////////////////////////////////////////////////////
		//  void LearnTalent( uint32 talentid, uint32 rank, bool isPreviewed )
		//    Teaches a talentspell to the Player and decreases the available talent points
		//
		//
		//  Parameters:
		//   uint32 talentid     -   unique numeric identifier of the talent (index of talent.dbc )
		//   uint32 rank         -   rank of the talent
		//   bool isPreviewed	 -   true if called from the preview system
		//
		//  Return Value:
		//   None.
		//
		////////////////////////////////////////////////////////////////////////////////////////////////
		void LearnTalent(uint32 talentid, uint32 rank, bool isPreviewed = false);

		void DealDamage(Unit* pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);
		void TakeDamage(Unit* pAttacker, uint32 damage, uint32 spellid, bool no_remove_auras = false);
		void Die(Unit* pAttacker, uint32 damage, uint32 spellid);
		void HandleKnockback(Object* caster, float horizontal, float vertical);

		uint32 LastHonorResetTime() const { return m_lastHonorResetTime; }
		void LastHonorResetTime(uint32 val) { m_lastHonorResetTime = val; }
		uint32 OnlineTime;
		bool tutorialsDirty;
		LevelInfo* lvlinfo;
		void CalculateBaseStats();
		uint32 load_health;
		uint32 load_mana;
		void CompleteLoading();
		void OnPushToWorld();
		void OnPrePushToWorld();
		void OnWorldPortAck();
		uint32 m_TeleportState;
		bool m_beingPushed;
		bool CanSignCharter(Charter* charter, Player* requester);
		Charter* m_charters[NUM_CHARTER_TYPES];
		uint32 flying_aura;
		bool resend_speed;
		bool rename_pending;
		uint32 iInstanceType;
		void SetName(string & name) { m_name = name; }
		// spell to (delay, last time)

		FactionReputation* reputationByListId[128];

		uint64 m_comboTarget;
		int8 m_comboPoints;
		bool m_retainComboPoints;
		int8 m_spellcomboPoints; // rogue talent Ruthlessness will change combopoints while consuming them. solutions 1) add post cast prochandling, 2) delay adding the CP
		void UpdateComboPoints();

		void AddComboPoints(uint64 target, int8 count);

		void NullComboPoints() { if(!m_retainComboPoints) { m_comboTarget = 0; m_comboPoints = 0; m_spellcomboPoints = 0; } UpdateComboPoints(); }
		uint32 m_speedChangeCounter;

		void SendAreaTriggerMessage(const char* message, ...);

		// Trade Target
		//Player *getTradeTarget() {return mTradeTarget;};

		Player* GetTradeTarget()
		{
			if(!IsInWorld()) return 0;
			return m_mapMgr->GetPlayer((uint32)mTradeTarget);
		}

		Item* getTradeItem(uint32 slot) {return mTradeItems[slot];};

		// Water level related stuff (they are public because they need to be accessed fast)
		// Nose level of the character (needed for proper breathing)
		float m_noseLevel;

		/* Mind Control */
		void Possess(uint64 GUID, uint32 delay = 0);
		void UnPossess();

		/* Last Speeds */
		void UpdateLastSpeeds()
		{
			m_lastRunSpeed = m_runSpeed;
			m_lastRunBackSpeed = m_backWalkSpeed;
			m_lastSwimSpeed = m_swimSpeed;
			m_lastBackSwimSpeed = m_backSwimSpeed;
			m_lastFlySpeed = m_flySpeed;
		}

		void RemoteRevive()
		{
			ResurrectPlayer();
			SetMovement(MOVE_UNROOT, 5);
			SetSpeeds(RUN, PLAYER_NORMAL_RUN_SPEED);
			SetSpeeds(SWIM, PLAYER_NORMAL_SWIM_SPEED);
			SetMovement(MOVE_LAND_WALK, 8);
			SetHealth(GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		}

		LocationVector m_last_group_position;
		int32 m_rap_mod_pct;
		void SummonRequest(uint32 Requestor, uint32 ZoneID, uint32 MapID, uint32 InstanceID, const LocationVector & Position);

		bool m_deathVision;
		SpellEntry* last_heal_spell;
		LocationVector m_sentTeleportPosition;

		void RemoveFromBattlegroundQueue();
		void FullHPMP();
		void RemoveTempEnchantsOnArena();
		uint32 m_arenateaminviteguid;

		/************************************************************************/
		/* Spell Packet wrapper Please keep this separated                      */
		/************************************************************************/
		void SendLevelupInfo(uint32 level, uint32 Hp, uint32 Mana, uint32 Stat0, uint32 Stat1, uint32 Stat2, uint32 Stat3, uint32 Stat4);
		void SendLogXPGain(uint64 guid, uint32 NormalXP, uint32 RestedXP, bool type);
		void SendWorldStateUpdate(uint32 WorldState, uint32 Value);
		void SendCastResult(uint32 SpellId, uint8 ErrorMessage, uint8 MultiCast, uint32 Extra);
		void Gossip_SendPOI(float X, float Y, uint32 Icon, uint32 Flags, uint32 Data, const char* Name);
		void SendSpellCooldownEvent(uint32 SpellId);
		void SendSpellModifier(uint8 spellgroup, uint8 spelltype, int32 v, bool is_pct);
		void SendItemPushResult(bool created, bool recieved, bool sendtoset, bool newitem,  uint8 destbagslot, uint32 destslot, uint32 count, uint32 entry, uint32 suffix, uint32 randomprop, uint32 stack);
		void SendSetProficiency(uint8 ItemClass, uint32 Proficiency);
		void SendLoginVerifyWorld(uint32 MapId, float X, float Y, float Z, float O);
		void SendPlaySpellVisual(uint64 guid, uint32 visualid);
		void SendNewDrunkState(uint32 state, uint32 itemid);

		/************************************************************************/
		/* End of SpellPacket wrapper                                           */
		/************************************************************************/

		Mailbox m_mailBox;
		bool m_waterwalk;
		bool m_setwaterwalk;
		bool m_setflycheat;
		uint64 m_areaSpiritHealer_guid;
		bool m_finishingmovesdodge;

		bool IsAttacking() {return m_attacking; }

		static void InitVisibleUpdateBits();
		static UpdateMask m_visibleUpdateMask;

		void CopyAndSendDelayedPacket(WorldPacket* data);
		void PartLFGChannel();
		SpeedCheatDetector*	SDetector;
	protected:
		LocationVector m_summonPos;
		uint32 m_summonInstanceId;
		uint32 m_summonMapId;
		uint32 m_summoner;

		void _SetCreateBits(UpdateMask* updateMask, Player* target) const;
		void _SetUpdateBits(UpdateMask* updateMask, Player* target) const;

		/* Update system components */
		ByteBuffer bUpdateBuffer;
		ByteBuffer bCreationBuffer;
		uint32 mUpdateCount;
		uint32 mCreationCount;
		uint32 mOutOfRangeIdCount;
		ByteBuffer mOutOfRangeIds;
		SplineMap _splineMap;
		/* End update system */

		void _LoadTutorials(QueryResult* result);
		void _SaveTutorials(QueryBuffer* buf);
		void _SaveInventory(bool firstsave);
		void _SaveQuestLogEntry(QueryBuffer* buf);
		void _LoadQuestLogEntry(QueryResult* result);

		void _LoadPet(QueryResult* result);
		void _LoadPetNo();
		void _LoadPetSpells(QueryResult* result);
		void _SavePet(QueryBuffer* buf);
		void _SavePetSpells(QueryBuffer* buf);
		void _ApplyItemMods(Item* item, int16 slot, bool apply, bool justdrokedown = false, bool skip_stat_apply = false);
		void _EventAttack(bool offhand);
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
		PlayerCreateInfo* info;
		uint32      m_AttackMsgTimer;	// "too far away" and "wrong facing" timer
		bool        m_attacking;
		std::string m_name;	// max 21 character name
		uint32      m_Tutorials[8];

		// Character Ban
		uint32      m_banned;
		string      m_banreason;
		uint32      m_AreaID;
		std::list< Pet* >  m_Summons;
		uint32      m_PetNumberMax;
		std::map<uint32, PlayerPet*> m_Pets;

		uint32      m_invitersGuid; // It is guild inviters guid ,0 when its not used

#ifdef TRACK_IMMUNITY_BUG
		uint32		m_immunityTime;
#endif

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
		float m_blockfromspellPCT;
		float m_critfromspell;
		float m_spellcritfromspell;
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
		// ActionButton mActions[PLAYER_ACTION_BUTTON_SIZE]; // Moved to dual spec
		// Player Reputation
		ReputationMap m_reputation;
		// Pointer to this char's game client
		WorldSession* m_session;
		// Channels
		std::set<Channel*> m_channels;
		// Visible objects
		std::set< uint64 > m_visibleObjects;
		// Groups/Raids
		uint32 m_GroupInviter;
		uint8 m_StableSlotCount;

		// Fishing related
		Object* m_SummonedObject;

		// other system
		LocationVector myCorpseLocation;
		int32		myCorpseInstanceId;

		uint32      m_lastHonorResetTime;
		uint32      _fields[PLAYER_END];
		int			hearth_of_wild_pct;		//druid hearth of wild talent used on shapeshifting. We either know what is last talent level or memo on learn

		uint32 m_team;
		float       m_lastRunSpeed;
		float       m_lastRunBackSpeed;
		float       m_lastSwimSpeed;
		float       m_lastBackSwimSpeed;
		float       m_lastFlySpeed;

		uint32 m_indoorCheckTimer;
		void RemovePendingPlayer();
	public:
#ifdef ENABLE_COMPRESSED_MOVEMENT
		void EventDumpCompressedMovement();
		void AppendMovementData(uint32 op, uint32 sz, const uint8* data);
		Mutex m_movementBufferLock;
		ByteBuffer m_movementBuffer;
#endif

		void addDeletedSpell(uint32 id) { mDeletedSpells.insert(id); }

		map<uint32, uint32> m_forcedReactions;

		uint32 m_flyhackCheckTimer;
		void _FlyhackCheck();

		bool m_passOnLoot;
		uint32 m_tradeSequence;
		bool m_changingMaps;

		void PlaySound(uint32 sound_id);

		/************************************************************************/
		/* SOCIAL                                                               */
		/************************************************************************/
	private:
		void Social_SendFriendList(uint32 flag);

		void Social_AddFriend(const char* name, const char* note);
		void Social_RemoveFriend(uint32 guid);

		void Social_AddIgnore(const char* name);
		void Social_RemoveIgnore(uint32 guid);

		void Social_SetNote(uint32 guid, const char* note);

	public:
		bool Social_IsIgnoring(PlayerInfo* m_info);
		bool Social_IsIgnoring(uint32 guid);

		void Social_TellFriendsOnline();
		void Social_TellFriendsOffline();

		/************************************************************************/
		/* end social                                                           */
		/************************************************************************/

		bool m_castFilterEnabled;
		uint32 m_castFilter[3];	// spell group relation of only spells that player can currently cast

		uint32 m_outStealthDamageBonusPct;
		uint32 m_outStealthDamageBonusPeriod;
		uint32 m_outStealthDamageBonusTimer;

		//ToDo: sort out where all the publics and privates go. This will do for now..
	private:
		PlayerInfo* m_playerInfo;
		uint32 m_RaidDifficulty;
		bool m_XpGain;
		bool resettalents;
		std::list< Item* > m_GarbageItems;

		void RemoveGarbageItems();

		uint32 ChampioningFactionID;

	public:
		void SetChampioningFaction(uint32 f) { ChampioningFactionID = f; }
		void AddGarbageItem(Item* it);
		uint32 CheckDamageLimits(uint32 dmg, uint32 spellid);

		PlayerInfo* getPlayerInfo() const { return m_playerInfo; }

		void LoadFieldsFromString(const char* string, uint32 firstField, uint32 fieldsNum);
		void UpdateGlyphs();

		// Avenging Wrath
		bool mAvengingWrath;
		void AvengingWrath() { mAvengingWrath = true; }

		void ToggleXpGain();
		bool CanGainXp();

#ifdef ENABLE_ACHIEVEMENTS
		AchievementMgr & GetAchievementMgr() { return m_achievementMgr; }
		AchievementMgr m_achievementMgr;
#endif
		/************************************************************************/
		/* Player Achievements - end				                            */
		/************************************************************************/

		/************************************************************************/
		/* Talent Specs                                                         */
		/************************************************************************/
		// VLack: Talent Specs from Aspire, now just the values for an empty packet
		uint16 m_maxTalentPoints;
		uint8 m_talentSpecsCount;
		uint8 m_talentActiveSpec;

		PlayerSpec m_specs[MAX_SPEC_COUNT];

	public:
		void SendTeleportAckMsg(const LocationVector & v);
		void SendUpdateDataToSet(ByteBuffer* groupbuf, ByteBuffer* nongroupbuf, bool sendtoself);

		bool CanBuyAt(VendorRestrictionEntry* vendor);
		bool CanTrainAt(Trainer*);

		Object* GetPlayerOwner() { return this; };
};

class SkillIterator
{
		SkillMap::iterator m_itr;
		SkillMap::iterator m_endItr;
		bool m_searchInProgress;
		Player* m_target;
	public:
		SkillIterator(Player* target) : m_searchInProgress(false), m_target(target) {}
		~SkillIterator() { if(m_searchInProgress) { EndSearch(); } }

		void BeginSearch()
		{
			// iteminterface doesn't use mutexes, maybe it should :P
			ARCEMU_ASSERT(!m_searchInProgress);
			m_itr = m_target->m_skills.begin();
			m_endItr = m_target->m_skills.end();
			m_searchInProgress = true;
		}

		void EndSearch()
		{
			// nothing here either
			ARCEMU_ASSERT(m_searchInProgress);
			m_searchInProgress = false;
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

			if(m_itr == m_endItr)
				return;

			++m_itr;
		}

		PlayerSkill* Grab() { return &m_itr->second; }
		bool End() { return (m_itr == m_endItr) ? true : false; }

};

#ifdef ENABLE_COMPRESSED_MOVEMENT

class CMovementCompressorThread : public ThreadBase
{
		bool running;
		Mutex m_listLock;
		set<Player*> m_players;
	public:
		CMovementCompressorThread() { running = true; }

		void AddPlayer(Player* pPlayer);
		void RemovePlayer(Player* pPlayer);

		void OnShutdown() { running = false; }
		bool run();
};

extern CMovementCompressorThread* MovementCompressor;

#endif

#endif

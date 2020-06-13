--[[
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
 *]]
 
--[[

	LCF(Lua's convinient functions(or wrappers).
	AUTHOR: Paroxysm.
			
			]]
assert(type(LCF) ~= "table")
--We predefine these values so they are valid during initializing the tables. We reconstruct them again in LCF.vars so that
-- it's still possible to do LCF.OBJECT_FIELD_GUID/TYPE/ENTRY etc.
local OBJECT_FIELD_GUID = 0x0000
local OBJECT_FIELD_TYPE = 0x0002
local OBJECT_FIELD_ENTRY =0x0003
local OBJECT_FIELD_SCALE_X =0x0004
local OBJECT_FIELD_PADDING =0x0005
local OBJECT_END = 0x0006
local LOWGUID  = OBJECT_FIELD_GUID
local HIGHGUID = LOWGUID+1
local ITEM_END = OBJECT_END + 0x003A
local CONTAINER_END =ITEM_END + 0x004A
local UNIT_END = OBJECT_END + 0x008E
local PLAYER_END = UNIT_END + 0x0480
local GAMEOBJECT_END = OBJECT_END + 0x000C
local DYNAMICOBJECT_END = OBJECT_END + 0x0006
local CORPSE_END = OBJECT_END + 0x001E
LCF = {
		CreatureMethods = {},
		GOMethods = {},
		PlayerMethods = {}
	}
LCF.vars  = {
		OBJECT_FIELD_GUID = 0x0000,
		OBJECT_FIELD_TYPE = 0x0002,
		OBJECT_FIELD_ENTRY =0x0003,
		OBJECT_FIELD_SCALE_X =0x0004,
		OBJECT_FIELD_PADDING =0x0005,
		OBJECT_END = 0x0006,
		LOWGUID  = OBJECT_FIELD_GUID,
		HIGHGUID = LOWGUID+1,
		ITEM_END = OBJECT_END + 0x003A,
		CONTAINER_END =ITEM_END + 0x004A,
		UNIT_END = OBJECT_END + 0x008E,
		PLAYER_END = UNIT_END + 0x0480,
		GAMEOBJECT_END = OBJECT_END + 0x000C,
		DYNAMICOBJECT_END = OBJECT_END + 0x0006,
		CORPSE_END = OBJECT_END + 0x001E
	}
	
LCF.vars.ITEM_FIELDS = {
		ITEM_FIELD_OWNER                          =OBJECT_END + 0x0000, --  Size: 2, Type: LONG, Flags: PUBLIC
		ITEM_FIELD_CONTAINED                      =OBJECT_END + 0x0002, --  Size: 2, Type: LONG, Flags: PUBLIC
		ITEM_FIELD_CREATOR                        =OBJECT_END + 0x0004, --  Size: 2, Type: LONG, Flags: PUBLIC
		ITEM_FIELD_GIFTCREATOR                    =OBJECT_END + 0x0006, --  Size: 2, Type: LONG, Flags: PUBLIC
		ITEM_FIELD_STACK_COUNT                    =OBJECT_END + 0x0008, --  Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
		ITEM_FIELD_DURATION                       =OBJECT_END + 0x0009, --  Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
		ITEM_FIELD_SPELL_CHARGES                  =OBJECT_END + 0x000A, --  Size: 5, Type: INT, Flags: OWNER, ITEM_OWNER
		ITEM_FIELD_FLAGS                          =OBJECT_END + 0x000F, --  Size: 1, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_1_1                =OBJECT_END + 0x0010, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_1_3                =OBJECT_END + 0x0012, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_2_1                =OBJECT_END + 0x0013, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_2_3                =OBJECT_END + 0x0015, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_3_1                =OBJECT_END + 0x0016, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_3_3                =OBJECT_END + 0x0018, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_4_1                =OBJECT_END + 0x0019, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_4_3                =OBJECT_END + 0x001B, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_5_1                =OBJECT_END + 0x001C, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_5_3                =OBJECT_END + 0x001E, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_6_1                =OBJECT_END + 0x001F, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_6_3                =OBJECT_END + 0x0021, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_7_1                =OBJECT_END + 0x0022, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_7_3                =OBJECT_END + 0x0024, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_8_1                =OBJECT_END + 0x0025, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_8_3                =OBJECT_END + 0x0027, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_9_1                =OBJECT_END + 0x0028, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_9_3                =OBJECT_END + 0x002A, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_10_1               =OBJECT_END + 0x002B, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_10_3               =OBJECT_END + 0x002D, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_11_1               =OBJECT_END + 0x002E, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_11_3               =OBJECT_END + 0x0030, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_12_1               =OBJECT_END + 0x0031, --  Size: 2, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ENCHANTMENT_12_3               =OBJECT_END + 0x0033, --  Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		ITEM_FIELD_PROPERTY_SEED                  =OBJECT_END + 0x0034, --  Size: 1, Type: INT, Flags: PUBLIC
		ITEM_FIELD_RANDOM_PROPERTIES_ID           =OBJECT_END + 0x0035, --  Size: 1, Type: INT, Flags: PUBLIC
		ITEM_FIELD_ITEM_TEXT_ID                   =OBJECT_END + 0x0036, --  Size: 1, Type: INT, Flags: OWNER
		ITEM_FIELD_DURABILITY                     =OBJECT_END + 0x0037, --  Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
		ITEM_FIELD_MAXDURABILITY                  =OBJECT_END + 0x0038, --  Size: 1, Type: INT, Flags: OWNER, ITEM_OWNER
		ITEM_FIELD_CREATE_PLAYED_TIME             =OBJECT_END + 0x0039, --  Size: 1, Type: INT, Flags: PUBLIC
	}
LCF.vars.CONTAINER_FIELDS = {
		CONTAINER_FIELD_NUM_SLOTS                 =ITEM_END + 0x0000, -- Size: 1, Type: INT, Flags: PUBLIC
		CONTAINER_ALIGN_PAD                       =ITEM_END + 0x0001, -- Size: 1, Type: BYTES, Flags: NONE
		CONTAINER_FIELD_SLOT_1                    =ITEM_END + 0x0002, -- Size: 72, Type: LONG, Flags: PUBLIC
		CONTAINER_END                             =ITEM_END + 0x004A
	}
LCF.vars.UNIT_FIELDS = {
		UNIT_FIELD_CHARM                         = OBJECT_END + 0x0000, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_SUMMON                        = OBJECT_END + 0x0002, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_CRITTER                       = OBJECT_END + 0x0004, -- Size: 2, Type: LONG, Flags: PRIVATE
		UNIT_FIELD_CHARMEDBY                     = OBJECT_END + 0x0006, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_SUMMONEDBY                    = OBJECT_END + 0x0008, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_CREATEDBY                     = OBJECT_END + 0x000A, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_TARGET                        = OBJECT_END + 0x000C, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_CHANNEL_OBJECT                = OBJECT_END + 0x000E, -- Size: 2, Type: LONG, Flags: PUBLIC
		UNIT_FIELD_BYTES_0                       = OBJECT_END + 0x0010, -- Size: 1, Type: BYTES, Flags: PUBLIC
		UNIT_FIELD_HEALTH                        = OBJECT_END + 0x0011, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER1                        = OBJECT_END + 0x0012, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER2                        = OBJECT_END + 0x0013, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER3                        = OBJECT_END + 0x0014, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER4                        = OBJECT_END + 0x0015, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER5                        = OBJECT_END + 0x0016, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER6                        = OBJECT_END + 0x0017, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER7                        = OBJECT_END + 0x0018, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXHEALTH                     = OBJECT_END + 0x0019, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER1                     = OBJECT_END + 0x001A, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER2                     = OBJECT_END + 0x001B, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER3                     = OBJECT_END + 0x001C, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER4                     = OBJECT_END + 0x001D, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER5                     = OBJECT_END + 0x001E, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER6                     = OBJECT_END + 0x001F, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MAXPOWER7                     = OBJECT_END + 0x0020, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER     = OBJECT_END + 0x0021, -- Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER= OBJECT_END + 0x0028, -- Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_LEVEL                         = OBJECT_END + 0x002F, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_FACTIONTEMPLATE               = OBJECT_END + 0x0030, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_VIRTUAL_ITEM_SLOT_ID                = OBJECT_END + 0x0031, -- Size: 3, Type: INT, Flags: PUBLIC
		UNIT_FIELD_FLAGS                         = OBJECT_END + 0x0034, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_FLAGS_2                       = OBJECT_END + 0x0035, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_AURASTATE                     = OBJECT_END + 0x0036, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_BASEATTACKTIME                = OBJECT_END + 0x0037, -- Size: 2, Type: INT, Flags: PUBLIC
		UNIT_FIELD_RANGEDATTACKTIME              = OBJECT_END + 0x0039, -- Size: 1, Type: INT, Flags: PRIVATE
		UNIT_FIELD_BOUNDINGRADIUS                = OBJECT_END + 0x003A, -- Size: 1, Type: FLOAT, Flags: PUBLIC
		UNIT_FIELD_COMBATREACH                   = OBJECT_END + 0x003B, -- Size: 1, Type: FLOAT, Flags: PUBLIC
		UNIT_FIELD_DISPLAYID                     = OBJECT_END + 0x003C, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_NATIVEDISPLAYID               = OBJECT_END + 0x003D, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MOUNTDISPLAYID                = OBJECT_END + 0x003E, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_MINDAMAGE                     = OBJECT_END + 0x003F, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
		UNIT_FIELD_MAXDAMAGE                     = OBJECT_END + 0x0040, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
		UNIT_FIELD_MINOFFHANDDAMAGE              = OBJECT_END + 0x0041, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
		UNIT_FIELD_MAXOFFHANDDAMAGE              = OBJECT_END + 0x0042, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER, PARTY_LEADER
		UNIT_FIELD_BYTES_1                       = OBJECT_END + 0x0043, -- Size: 1, Type: BYTES, Flags: PUBLIC
		UNIT_FIELD_PETNUMBER                     = OBJECT_END + 0x0044, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_PET_NAME_TIMESTAMP            = OBJECT_END + 0x0045, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_PETEXPERIENCE                 = OBJECT_END + 0x0046, -- Size: 1, Type: INT, Flags: OWNER
		UNIT_FIELD_PETNEXTLEVELEXP               = OBJECT_END + 0x0047, -- Size: 1, Type: INT, Flags: OWNER
		UNIT_DYNAMIC_FLAGS                       = OBJECT_END + 0x0048, -- Size: 1, Type: INT, Flags: DYNAMIC
		UNIT_CHANNEL_SPELL                       = OBJECT_END + 0x0049, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_MOD_CAST_SPEED                      = OBJECT_END + 0x004A, -- Size: 1, Type: FLOAT, Flags: PUBLIC
		UNIT_CREATED_BY_SPELL                    = OBJECT_END + 0x004B, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_NPC_FLAGS                           = OBJECT_END + 0x004C, -- Size: 1, Type: INT, Flags: DYNAMIC
		UNIT_NPC_EMOTESTATE                      = OBJECT_END + 0x004D, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_STAT0                         = OBJECT_END + 0x004E, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_STAT1                         = OBJECT_END + 0x004F, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_STAT2                         = OBJECT_END + 0x0050, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_STAT3                         = OBJECT_END + 0x0051, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_STAT4                         = OBJECT_END + 0x0052, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POSSTAT0                      = OBJECT_END + 0x0053, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POSSTAT1                      = OBJECT_END + 0x0054, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POSSTAT2                      = OBJECT_END + 0x0055, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POSSTAT3                      = OBJECT_END + 0x0056, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POSSTAT4                      = OBJECT_END + 0x0057, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_NEGSTAT0                      = OBJECT_END + 0x0058, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_NEGSTAT1                      = OBJECT_END + 0x0059, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_NEGSTAT2                      = OBJECT_END + 0x005A, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_NEGSTAT3                      = OBJECT_END + 0x005B, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_NEGSTAT4                      = OBJECT_END + 0x005C, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_RESISTANCES                   = OBJECT_END + 0x005D, -- Size: 7, Type: INT, Flags: PRIVATE, OWNER, PARTY_LEADER
		UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE    = OBJECT_END + 0x0064, -- Size: 7, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE    = OBJECT_END + 0x006B, -- Size: 7, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_BASE_MANA                     = OBJECT_END + 0x0072, -- Size: 1, Type: INT, Flags: PUBLIC
		UNIT_FIELD_BASE_HEALTH                   = OBJECT_END + 0x0073, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_BYTES_2                       = OBJECT_END + 0x0074, -- Size: 1, Type: BYTES, Flags: PUBLIC
		UNIT_FIELD_ATTACK_POWER                  = OBJECT_END + 0x0075, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_ATTACK_POWER_MODS             = OBJECT_END + 0x0076, -- Size: 1, Type: TWO_SHORT, Flags: PRIVATE, OWNER
		UNIT_FIELD_ATTACK_POWER_MULTIPLIER       = OBJECT_END + 0x0077, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_RANGED_ATTACK_POWER           = OBJECT_END + 0x0078, -- Size: 1, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_RANGED_ATTACK_POWER_MODS      = OBJECT_END + 0x0079, -- Size: 1, Type: TWO_SHORT, Flags: PRIVATE, OWNER
		UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER= OBJECT_END + 0x007A, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_MINRANGEDDAMAGE               = OBJECT_END + 0x007B, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_MAXRANGEDDAMAGE               = OBJECT_END + 0x007C, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POWER_COST_MODIFIER           = OBJECT_END + 0x007D, -- Size: 7, Type: INT, Flags: PRIVATE, OWNER
		UNIT_FIELD_POWER_COST_MULTIPLIER         = OBJECT_END + 0x0084, -- Size: 7, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_MAXHEALTHMODIFIER             = OBJECT_END + 0x008B, -- Size: 1, Type: FLOAT, Flags: PRIVATE, OWNER
		UNIT_FIELD_HOVERHEIGHT                   = OBJECT_END + 0x008C, -- Size: 1, Type: FLOAT, Flags: PUBLIC
		UNIT_FIELD_PADDING                       = OBJECT_END + 0x008D, -- Size: 1, Type: INT, Flags: NONE
	}
LCF.vars.PLAYER_FIELDS = {
		PLAYER_DUEL_ARBITER                      = UNIT_END + 0x0000, -- Size: 2, Type: LONG, Flags: PUBLIC
		PLAYER_FLAGS                             = UNIT_END + 0x0002, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_GUILDID                           = UNIT_END + 0x0003, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_GUILDRANK                         = UNIT_END + 0x0004, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_BYTES                             = UNIT_END + 0x0005, -- Size: 1, Type: BYTES, Flags: PUBLIC
		PLAYER_BYTES_2                           = UNIT_END + 0x0006, -- Size: 1, Type: BYTES, Flags: PUBLIC
		PLAYER_BYTES_3                           = UNIT_END + 0x0007, -- Size: 1, Type: BYTES, Flags: PUBLIC
		PLAYER_DUEL_TEAM                         = UNIT_END + 0x0008, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_GUILD_TIMESTAMP                   = UNIT_END + 0x0009, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_QUEST_LOG_1_1                     = UNIT_END + 0x000A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_1_2                     = UNIT_END + 0x000B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_1_3                     = UNIT_END + 0x000C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_1_4                     = UNIT_END + 0x000D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_2_1                     = UNIT_END + 0x000E, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_2_2                     = UNIT_END + 0x000F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_2_3                     = UNIT_END + 0x0010, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_2_4                     = UNIT_END + 0x0011, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_3_1                     = UNIT_END + 0x0012, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_3_2                     = UNIT_END + 0x0013, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_3_3                     = UNIT_END + 0x0014, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_3_4                     = UNIT_END + 0x0015, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_4_1                     = UNIT_END + 0x0016, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_4_2                     = UNIT_END + 0x0017, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_4_3                     = UNIT_END + 0x0018, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_4_4                     = UNIT_END + 0x0019, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_5_1                     = UNIT_END + 0x001A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_5_2                     = UNIT_END + 0x001B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_5_3                     = UNIT_END + 0x001C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_5_4                     = UNIT_END + 0x001D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_6_1                     = UNIT_END + 0x001E, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_6_2                     = UNIT_END + 0x001F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_6_3                     = UNIT_END + 0x0020, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_6_4                     = UNIT_END + 0x0021, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_7_1                     = UNIT_END + 0x0022, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_7_2                     = UNIT_END + 0x0023, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_7_3                     = UNIT_END + 0x0024, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_7_4                     = UNIT_END + 0x0025, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_8_1                     = UNIT_END + 0x0026, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_8_2                     = UNIT_END + 0x0027, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_8_3                     = UNIT_END + 0x0028, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_8_4                     = UNIT_END + 0x0029, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_9_1                     = UNIT_END + 0x002A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_9_2                     = UNIT_END + 0x002B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_9_3                     = UNIT_END + 0x002C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_9_4                     = UNIT_END + 0x002D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_10_1                    = UNIT_END + 0x002E, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_10_2                    = UNIT_END + 0x002F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_10_3                    = UNIT_END + 0x0030, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_10_4                    = UNIT_END + 0x0031, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_11_1                    = UNIT_END + 0x0032, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_11_2                    = UNIT_END + 0x0033, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_11_3                    = UNIT_END + 0x0034, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_11_4                    = UNIT_END + 0x0035, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_12_1                    = UNIT_END + 0x0036, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_12_2                    = UNIT_END + 0x0037, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_12_3                    = UNIT_END + 0x0038, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_12_4                    = UNIT_END + 0x0039, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_13_1                    = UNIT_END + 0x003A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_13_2                    = UNIT_END + 0x003B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_13_3                    = UNIT_END + 0x003C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_13_4                    = UNIT_END + 0x003D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_14_1                    = UNIT_END + 0x003E, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_14_2                    = UNIT_END + 0x003F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_14_3                    = UNIT_END + 0x0040, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_14_4                    = UNIT_END + 0x0041, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_15_1                    = UNIT_END + 0x0042, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_15_2                    = UNIT_END + 0x0043, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_15_3                    = UNIT_END + 0x0044, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_15_4                    = UNIT_END + 0x0045, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_16_1                    = UNIT_END + 0x0046, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_16_2                    = UNIT_END + 0x0047, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_16_3                    = UNIT_END + 0x0048, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_16_4                    = UNIT_END + 0x0049, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_17_1                    = UNIT_END + 0x004A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_17_2                    = UNIT_END + 0x004B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_17_3                    = UNIT_END + 0x004C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_17_4                    = UNIT_END + 0x004D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_18_1                    = UNIT_END + 0x004E, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_18_2                    = UNIT_END + 0x004F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_18_3                    = UNIT_END + 0x0050, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_18_4                    = UNIT_END + 0x0051, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_19_1                    = UNIT_END + 0x0052, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_19_2                    = UNIT_END + 0x0053, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_19_3                    = UNIT_END + 0x0054, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_19_4                    = UNIT_END + 0x0055, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_20_1                    = UNIT_END + 0x0056, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_20_2                    = UNIT_END + 0x0057, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_20_3                    = UNIT_END + 0x0058, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_20_4                    = UNIT_END + 0x0059, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_21_1                    = UNIT_END + 0x005A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_21_2                    = UNIT_END + 0x005B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_21_3                    = UNIT_END + 0x005C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_21_4                    = UNIT_END + 0x005D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_22_1                    = UNIT_END + 0x005E, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_22_2                    = UNIT_END + 0x005F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_22_3                    = UNIT_END + 0x0060, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_22_4                    = UNIT_END + 0x0061, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_23_1                    = UNIT_END + 0x0062, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_23_2                    = UNIT_END + 0x0063, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_23_3                    = UNIT_END + 0x0064, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_23_4                    = UNIT_END + 0x0065, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_24_1                    = UNIT_END + 0x0066, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_24_2                    = UNIT_END + 0x0067, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_24_3                    = UNIT_END + 0x0068, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_24_4                    = UNIT_END + 0x0069, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_25_1                    = UNIT_END + 0x006A, -- Size: 1, Type: INT, Flags: PARTY_MEMBER
		PLAYER_QUEST_LOG_25_2                    = UNIT_END + 0x006B, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_QUEST_LOG_25_3                    = UNIT_END + 0x006C, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_QUEST_LOG_25_4                    = UNIT_END + 0x006D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_VISIBLE_ITEM_1_ENTRYID            = UNIT_END + 0x006E, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_1_ENCHANTMENT        = UNIT_END + 0x006F, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_2_ENTRYID            = UNIT_END + 0x0070, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_2_ENCHANTMENT        = UNIT_END + 0x0071, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_3_ENTRYID            = UNIT_END + 0x0072, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_3_ENCHANTMENT        = UNIT_END + 0x0073, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_4_ENTRYID            = UNIT_END + 0x0074, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_4_ENCHANTMENT        = UNIT_END + 0x0075, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_5_ENTRYID            = UNIT_END + 0x0076, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_5_ENCHANTMENT        = UNIT_END + 0x0077, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_6_ENTRYID            = UNIT_END + 0x0078, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_6_ENCHANTMENT        = UNIT_END + 0x0079, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_7_ENTRYID            = UNIT_END + 0x007A, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_7_ENCHANTMENT        = UNIT_END + 0x007B, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_8_ENTRYID            = UNIT_END + 0x007C, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_8_ENCHANTMENT        = UNIT_END + 0x007D, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_9_ENTRYID            = UNIT_END + 0x007E, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_9_ENCHANTMENT        = UNIT_END + 0x007F, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_10_ENTRYID           = UNIT_END + 0x0080, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_10_ENCHANTMENT       = UNIT_END + 0x0081, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_11_ENTRYID           = UNIT_END + 0x0082, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_11_ENCHANTMENT       = UNIT_END + 0x0083, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_12_ENTRYID           = UNIT_END + 0x0084, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_12_ENCHANTMENT       = UNIT_END + 0x0085, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_13_ENTRYID           = UNIT_END + 0x0086, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_13_ENCHANTMENT       = UNIT_END + 0x0087, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_14_ENTRYID           = UNIT_END + 0x0088, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_14_ENCHANTMENT       = UNIT_END + 0x0089, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_15_ENTRYID           = UNIT_END + 0x008A, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_15_ENCHANTMENT       = UNIT_END + 0x008B, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_16_ENTRYID           = UNIT_END + 0x008C, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_16_ENCHANTMENT       = UNIT_END + 0x008D, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_17_ENTRYID           = UNIT_END + 0x008E, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_17_ENCHANTMENT       = UNIT_END + 0x008F, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_18_ENTRYID           = UNIT_END + 0x0090, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_18_ENCHANTMENT       = UNIT_END + 0x0091, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_19_ENTRYID           = UNIT_END + 0x0092, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_VISIBLE_ITEM_19_ENCHANTMENT       = UNIT_END + 0x0093, -- Size: 1, Type: TWO_SHORT, Flags: PUBLIC
		PLAYER_CHOSEN_TITLE                      = UNIT_END + 0x0094, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_FAKE_INEBRIATION                  = UNIT_END + 0x0095, -- Size: 1, Type: INT, Flags: PUBLIC
		PLAYER_FIELD_INV_SLOT_HEAD               = UNIT_END + 0x0096, -- Size: 46, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_PACK_SLOT_1                 = UNIT_END + 0x00C4, -- Size: 32, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_BANK_SLOT_1                 = UNIT_END + 0x00E4, -- Size: 56, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_BANKBAG_SLOT_1              = UNIT_END + 0x011C, -- Size: 14, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_VENDORBUYBACK_SLOT_1        = UNIT_END + 0x012A, -- Size: 24, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_KEYRING_SLOT_1              = UNIT_END + 0x0142, -- Size: 64, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_CURRENCYTOKEN_SLOT_1        = UNIT_END + 0x0182, -- Size: 64, Type: LONG, Flags: PRIVATE
		PLAYER_FARSIGHT                          = UNIT_END + 0x01C2, -- Size: 2, Type: LONG, Flags: PRIVATE
		PLAYER__FIELD_KNOWN_TITLES               = UNIT_END + 0x01C4, -- Size: 2, Type: LONG, Flags: PRIVATE
		PLAYER__FIELD_KNOWN_TITLES1              = UNIT_END + 0x01C6, -- Size: 2, Type: LONG, Flags: PRIVATE
		PLAYER__FIELD_KNOWN_TITLES2              = UNIT_END + 0x01C8, -- Size: 2, Type: LONG, Flags: PRIVATE
		PLAYER_FIELD_KNOWN_CURRENCIES            = UNIT_END + 0x01CA, -- Size: 2, Type: LONG, Flags: PRIVATE
		PLAYER_XP                                = UNIT_END + 0x01CC, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_NEXT_LEVEL_XP                     = UNIT_END + 0x01CD, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_SKILL_INFO_1_1                    = UNIT_END + 0x01CE, -- Size: 384, Type: TWO_SHORT, Flags: PRIVATE
		PLAYER_CHARACTER_POINTS1                 = UNIT_END + 0x034E, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_CHARACTER_POINTS2                 = UNIT_END + 0x034F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_TRACK_CREATURES                   = UNIT_END + 0x0350, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_TRACK_RESOURCES                   = UNIT_END + 0x0351, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_BLOCK_PERCENTAGE                  = UNIT_END + 0x0352, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_DODGE_PERCENTAGE                  = UNIT_END + 0x0353, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_PARRY_PERCENTAGE                  = UNIT_END + 0x0354, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_EXPERTISE                         = UNIT_END + 0x0355, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_OFFHAND_EXPERTISE                 = UNIT_END + 0x0356, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_CRIT_PERCENTAGE                   = UNIT_END + 0x0357, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_RANGED_CRIT_PERCENTAGE            = UNIT_END + 0x0358, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_OFFHAND_CRIT_PERCENTAGE           = UNIT_END + 0x0359, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_SPELL_CRIT_PERCENTAGE1            = UNIT_END + 0x035A, -- Size: 7, Type: FLOAT, Flags: PRIVATE
		PLAYER_SHIELD_BLOCK                      = UNIT_END + 0x0361, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_SHIELD_BLOCK_CRIT_PERCENTAGE      = UNIT_END + 0x0362, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_EXPLORED_ZONES_1                  = UNIT_END + 0x0363, -- Size: 128, Type: BYTES, Flags: PRIVATE
		PLAYER_REST_STATE_EXPERIENCE             = UNIT_END + 0x03E3, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_COINAGE                     = UNIT_END + 0x03E4, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MOD_DAMAGE_DONE_POS         = UNIT_END + 0x03E5, -- Size: 7, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MOD_DAMAGE_DONE_NEG         = UNIT_END + 0x03EC, -- Size: 7, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MOD_DAMAGE_DONE_PCT         = UNIT_END + 0x03F3, -- Size: 7, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MOD_HEALING_DONE_POS        = UNIT_END + 0x03FA, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MOD_HEALING_PCT             = UNIT_END + 0x03FB, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_FIELD_MOD_HEALING_DONE_PCT        = UNIT_END + 0x03FC, -- Size: 1, Type: FLOAT, Flags: PRIVATE
		PLAYER_FIELD_MOD_TARGET_RESISTANCE       = UNIT_END + 0x03FD, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE= UNIT_END + 0x03FE, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_BYTES                       = UNIT_END + 0x03FF, -- Size: 1, Type: BYTES, Flags: PRIVATE
		PLAYER_AMMO_ID                           = UNIT_END + 0x0400, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_SELF_RES_SPELL                    = UNIT_END + 0x0401, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_PVP_MEDALS                  = UNIT_END + 0x0402, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_BUYBACK_PRICE_1             = UNIT_END + 0x0403, -- Size: 12, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_BUYBACK_TIMESTAMP_1         = UNIT_END + 0x040F, -- Size: 12, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_KILLS                       = UNIT_END + 0x041B, -- Size: 1, Type: TWO_SHORT, Flags: PRIVATE
		PLAYER_FIELD_TODAY_CONTRIBUTION          = UNIT_END + 0x041C, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_YESTERDAY_CONTRIBUTION      = UNIT_END + 0x041D, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_LIFETIME_HONORBALE_KILLS    = UNIT_END + 0x041E, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_BYTES2                      = UNIT_END + 0x041F, -- Size: 1, Type: 6, Flags: PRIVATE
		PLAYER_FIELD_WATCHED_FACTION_INDEX       = UNIT_END + 0x0420, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_COMBAT_RATING_1             = UNIT_END + 0x0421, -- Size: 25, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_ARENA_TEAM_INFO_1_1         = UNIT_END + 0x043A, -- Size: 21, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_HONOR_CURRENCY              = UNIT_END + 0x044F, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_ARENA_CURRENCY              = UNIT_END + 0x0450, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_MAX_LEVEL                   = UNIT_END + 0x0451, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_DAILY_QUESTS_1              = UNIT_END + 0x0452, -- Size: 25, Type: INT, Flags: PRIVATE
		PLAYER_RUNE_REGEN_1                      = UNIT_END + 0x046B, -- Size: 4, Type: FLOAT, Flags: PRIVATE
		PLAYER_NO_REAGENT_COST_1                 = UNIT_END + 0x046F, -- Size: 3, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_GLYPH_SLOTS_1               = UNIT_END + 0x0472, -- Size: 6, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_GLYPHS_1                    = UNIT_END + 0x0478, -- Size: 6, Type: INT, Flags: PRIVATE
		PLAYER_GLYPHS_ENABLED                    = UNIT_END + 0x047E, -- Size: 1, Type: INT, Flags: PRIVATE
		PLAYER_FIELD_PADDING                     = UNIT_END + 0x047F, -- Size: 1, Type: INT, Flags: NONE
	}
LCF.vars.GAMEOBJECT_FIELDS = {
		GAMEOBJECT_PARENTROTATION_01              = OBJECT_END + 0x0005, -- Size: 4, Type: FLOAT, Flags: PUBLIC
		GAMEOBJECT_PARENTROTATION_02              = OBJECT_END + 0x0006, -- Size: 4, Type: FLOAT, Flags: PUBLIC
		GAMEOBJECT_PARENTROTATION_03              = OBJECT_END + 0x0007, -- Size: 4, Type: FLOAT, Flags: PUBLIC
		OBJECT_FIELD_CREATED_BY                   = OBJECT_END + 0x0000, -- Size: 2, Type: LONG, Flags: PUBLIC
		GAMEOBJECT_DISPLAYID                      = OBJECT_END + 0x0002, -- Size: 1, Type: INT, Flags: PUBLIC
		GAMEOBJECT_FLAGS                          = OBJECT_END + 0x0003, -- Size: 1, Type: INT, Flags: PUBLIC
		GAMEOBJECT_PARENTROTATION                 = OBJECT_END + 0x0004, -- Size: 4, Type: FLOAT, Flags: PUBLIC
		GAMEOBJECT_DYNAMIC                        = OBJECT_END + 0x0008, -- Size: 1, Type: TWO_SHORT, Flags: DYNAMIC
		GAMEOBJECT_FACTION                        = OBJECT_END + 0x0009, -- Size: 1, Type: INT, Flags: PUBLIC
		GAMEOBJECT_LEVEL                          = OBJECT_END + 0x000A, -- Size: 1, Type: INT, Flags: PUBLIC
		GAMEOBJECT_BYTES_1                        = OBJECT_END + 0x000B, -- Size: 1, Type: BYTES, Flags: PUBLIC
	}
LCF.vars.DYNOBJ_FIELDS = {
		DYNAMICOBJECT_CASTER                      = OBJECT_END + 0x0000, -- Size: 2, Type: LONG, Flags: PUBLIC
		DYNAMICOBJECT_BYTES                       = OBJECT_END + 0x0002, -- Size: 1, Type: BYTES, Flags: PUBLIC
		DYNAMICOBJECT_SPELLID                     = OBJECT_END + 0x0003, -- Size: 1, Type: INT, Flags: PUBLIC
		DYNAMICOBJECT_RADIUS                      = OBJECT_END + 0x0004, -- Size: 1, Type: FLOAT, Flags: PUBLIC
		DYNAMICOBJECT_CASTTIME                    = OBJECT_END + 0x0005, -- Size: 1, Type: INT, Flags: PUBLIC
	}
LCF.vars.CORPSE_FIELDS = {
		CORPSE_FIELD_OWNER                        = OBJECT_END + 0x0000, -- Size: 2, Type: LONG, Flags: PUBLIC
		CORPSE_FIELD_PARTY                        = OBJECT_END + 0x0002, -- Size: 2, Type: LONG, Flags: PUBLIC
		CORPSE_FIELD_DISPLAY_ID                   = OBJECT_END + 0x0004, -- Size: 1, Type: INT, Flags: PUBLIC
		CORPSE_FIELD_ITEM                         = OBJECT_END + 0x0005, -- Size: 19, Type: INT, Flags: PUBLIC
		CORPSE_FIELD_BYTES_1                      = OBJECT_END + 0x0018, -- Size: 1, Type: BYTES, Flags: PUBLIC
		CORPSE_FIELD_BYTES_2                      = OBJECT_END + 0x0019, -- Size: 1, Type: BYTES, Flags: PUBLIC
		CORPSE_FIELD_GUILD                        = OBJECT_END + 0x001A, -- Size: 1, Type: INT, Flags: PUBLIC
		CORPSE_FIELD_FLAGS                        = OBJECT_END + 0x001B, -- Size: 1, Type: INT, Flags: PUBLIC
		CORPSE_FIELD_DYNAMIC_FLAGS                = OBJECT_END + 0x001C, -- Size: 1, Type: INT, Flags: DYNAMIC
		CORPSE_FIELD_PAD                          = OBJECT_END + 0x001D, -- Size: 1, Type: INT, Flags: NONE
	}
LCF.vars.MSG_FIELDS = {
		LANG_UNIVERSAL                              = 0x00,
		LANG_ORCISH                                 = 0x01,
		LANG_DARNASSIAN                             = 0x02,
		LANG_TAURAHE                                = 0x03,
		LANG_DWARVISH                               = 0x06,
		LANG_COMMON                                 = 0x07,
		LANG_DEMONIC                                = 0x08,
		LANG_TITAN                                  = 0x09,
		LANG_THELASSIAN                             = 0x0A,
		LANG_DRACONIC                               = 0x0B,
		LANG_KALIMAG                                = 0x0C,
		LANG_GNOMISH                                = 0x0D,
		LANG_TROLL                                  = 0x0E,
		LANG_GUTTERSPEAK                            = 0x21,
		LANG_DRAENEI                                = 0x23,
		NUM_LANGUAGES                               = 0x24,
		
		CHAT_MSG_ADDON									= -1,
		CHAT_MSG_SYSTEM                                 = 0,    --28,   CHAT_MSG_SYSTEM                 = 0x00,         0
		CHAT_MSG_SAY									= 1,
		CHAT_MSG_PARTY									= 2,
		CHAT_MSG_RAID									= 3,
		CHAT_MSG_GUILD									= 4,
		CHAT_MSG_OFFICER								= 5,
		CHAT_MSG_YELL									= 6,
		CHAT_MSG_WHISPER								= 7,
		CHAT_MSG_WHISPER_MOB							= 8,--CHAT_MSG_WHISPER_INFORM
		CHAT_MSG_WHISPER_INFORM							= 9,--CHAT_MSG_REPLY
		CHAT_MSG_EMOTE									= 10,
		CHAT_MSG_TEXT_EMOTE								= 11,
		CHAT_MSG_MONSTER_SAY							= 12,
		CHAT_MSG_MONSTER_PARTY							= 13,
		CHAT_MSG_MONSTER_YELL							= 14,
		CHAT_MSG_MONSTER_WHISPER						= 15,
		CHAT_MSG_MONSTER_EMOTE							= 16,
		CHAT_MSG_CHANNEL								= 17,
		CHAT_MSG_CHANNEL_JOIN							= 18,
		CHAT_MSG_CHANNEL_LEAVE							= 19,
		CHAT_MSG_CHANNEL_LIST							= 20,
		CHAT_MSG_CHANNEL_NOTICE							= 21,
		CHAT_MSG_CHANNEL_NOTICE_USER					= 22,
		CHAT_MSG_AFK									= 23,
		CHAT_MSG_DND									= 24,
		CHAT_MSG_IGNORED								= 25,
		CHAT_MSG_SKILL									= 26,
		CHAT_MSG_LOOT									= 27,
		CHAT_MSG_MONEY                                  = 28,
		CHAT_MSG_OPENING                                = 29,
		CHAT_MSG_TRADESKILLS                            = 30,
		CHAT_MSG_PET_INFO                               = 31,
		CHAT_MSG_COMBAT_MISC_INFO                       = 32,
		CHAT_MSG_COMBAT_XP_GAIN                         = 33,
		CHAT_MSG_COMBAT_HONOR_GAIN                      = 34,
		CHAT_MSG_COMBAT_FACTION_CHANGE                  = 35,
		CHAT_MSG_BG_EVENT_NEUTRAL						= 36,
		CHAT_MSG_BG_EVENT_ALLIANCE						= 37,
		CHAT_MSG_BG_EVENT_HORDE							= 38,
		CHAT_MSG_RAID_LEADER							= 39,
		CHAT_MSG_RAID_WARNING							= 40,
		CHAT_MSG_RAID_WARNING_WIDESCREEN				= 41,
		CHAT_MSG_RAID_BOSS_EMOTE                        = 42,
		CHAT_MSG_FILTERED								= 43,
		CHAT_MSG_BATTLEGROUND							= 44,
		CHAT_MSG_BATTLEGROUND_LEADER					= 45,
		CHAT_MSG_RESTRICTED								= 46,
		CHAT_MSG_ACHIEVEMENT							= 48,
		CHAT_MSG_GUILD_ACHIEVEMENT						= 49,
	}
LCF.vars.COLOR_FIELDS = {
		MSG_COLOR_LIGHTRED       = "|cffff6060",
		MSG_COLOR_LIGHTBLUE      = "|cff00ccff",
		MSG_COLOR_TORQUISEBLUE	 = "|cff00C78C",
		MSG_COLOR_SPRINGGREEN	 = "|cff00FF7F",
		MSG_COLOR_GREENYELLOW	 = "|cffADFF2F",
		MSG_COLOR_BLUE           = "|cff0000ff",
		MSG_COLOR_PURPLE		 = "|cffDA70D6",
		MSG_COLOR_GREEN	         = "|cff00ff00",
		MSG_COLOR_RED            = "|cffff0000",
		MSG_COLOR_GOLD           = "|cffffcc00",
		MSG_COLOR_GOLD2			 = "|cffFFC125",
		MSG_COLOR_GREY           = "|cff888888",
		MSG_COLOR_WHITE          = "|cffffffff",
		MSG_COLOR_SUBWHITE       = "|cffbbbbbb",
		MSG_COLOR_MAGENTA        = "|cffff00ff",
		MSG_COLOR_YELLOW         = "|cffffff00",
		MSG_COLOR_ORANGEY		 = "|cffFF4500",
		MSG_COLOR_CHOCOLATE		 = "|cffCD661D",
		MSG_COLOR_CYAN           = "|cff00ffff",
		MSG_COLOR_IVORY			 = "|cff8B8B83",
		MSG_COLOR_LIGHTYELLOW	 = "|cffFFFFE0",
		MSG_COLOR_SEXGREEN		 = "|cff71C671",
		MSG_COLOR_SEXTEAL		 = "|cff388E8E",
		MSG_COLOR_SEXPINK		 = "|cffC67171",
		MSG_COLOR_SEXBLUE		 = "|cff00E5EE",
		MSG_COLOR_SEXHOTPINK	 = "|cffFF6EB4"
	}
LCF.vars.UNIT_FLAGS = {
		UNIT_FLAG_UNKNOWN_1                  = 0x00000001, -- 1            1
		UNIT_FLAG_NOT_ATTACKABLE_2           = 0x00000002, -- 2            2  client won't let you attack them
		UNIT_FLAG_LOCK_PLAYER                = 0x00000004, -- 3            4  ? does nothing to client (probably wrong) - only taxi code checks this
		UNIT_FLAG_PLAYER_CONTROLLED          = 0x00000008, -- 4            8  makes players and NPCs attackable / not attackable
		UNIT_FLAG_UNKNOWN_5                  = 0x00000010, -- 5           16  ? some NPCs have this
		UNIT_FLAG_UNKNOWN_6                  = 0x00000020, -- 6           32
		UNIT_FLAG_PLUS_MOB                   = 0x00000040, -- 7           64  ? some NPCs have this (Rare/Elite/Boss?)
		UNIT_FLAG_UNKNOWN_8                  = 0x00000080, -- 8          128  ? can change attackable status 
		UNIT_FLAG_NOT_ATTACKABLE_9           = 0x00000100, -- 9          256  changes attackable status
		UNIT_FLAG_UNKNOWN_10                 = 0x00000200, -- 10         512  ? some NPCs have this
		UNIT_FLAG_LOOTING                    = 0x00000400, -- 11        1024
		UNIT_FLAG_SELF_RES                   = 0x00000800, -- 12        2048  ? some NPCs have this
		UNIT_FLAG_PVP                        = 0x00001000, -- 13        4096  sets PvP flag
		UNIT_FLAG_SILENCED                   = 0x00002000, -- 14        8192
		UNIT_FLAG_DEAD                       = 0x00004000, -- 15       16384  used for special "dead" NPCs like Withered Corpses
		UNIT_FLAG_UNKNOWN_16                 = 0x00008000, -- 16       32768  ? some NPCs have this
		UNIT_FLAG_ALIVE                      = 0x00010000, -- 17       65536  ?
		UNIT_FLAG_PACIFIED                   = 0x00020000, -- 18      131072
		UNIT_FLAG_STUNNED                    = 0x00040000, -- 19      262144
		UNIT_FLAG_COMBAT                     = 0x00080000, -- 20      524288  sets combat flag
		UNIT_FLAG_MOUNTED_TAXI               = 0x00100000, -- 21     1048576  mounted on a taxi
		UNIT_FLAG_DISARMED                   = 0x00200000, -- 22     2097152
		UNIT_FLAG_CONFUSED                   = 0x00400000, -- 23     4194304
		UNIT_FLAG_FLEEING                    = 0x00800000, -- 24     8388608  fear
		UNIT_FLAG_PLAYER_CONTROLLED_CREATURE = 0x01000000, -- 25    16777216
		UNIT_FLAG_NOT_SELECTABLE             = 0x02000000, -- 26    33554432  cannot select the unit
		UNIT_FLAG_SKINNABLE                  = 0x04000000, -- 27    67108864
		UNIT_FLAG_UNKNOWN_28                 = 0x08000000, -- 28   134217728  ? was MAKE_CHAR_UNTOUCHABLE (probably wrong), nothing ever set it
		UNIT_FLAG_UNKNOWN_29                 = 0x10000000, -- 29   268435456
		UNIT_FLAG_FEIGN_DEATH                = 0x20000000, -- 30   536870912
		UNIT_FLAG_UNKNOWN_31                 = 0x40000000, -- 31  1073741824  ? was WEAPON_OFF and being used for disarm
		UNIT_FLAG_UNKNOWN_32                 = 0x80000000, -- 32  2147483648
	}
LCF.vars.EMOTE_FIELDS = {
		EMOTE_ONESHOT_NONE					= 0,
		EMOTE_ONESHOT_TALK					= 1, -- DNR
		EMOTE_ONESHOT_BOW					= 2,
		EMOTE_ONESHOT_WAVE					= 3, -- DNR
		EMOTE_ONESHOT_CHEER					= 4, -- DNR
		EMOTE_ONESHOT_EXCLAMATION			= 5, -- DNR
		EMOTE_ONESHOT_QUESTION				= 6,
		EMOTE_ONESHOT_EAT					= 7,
		EMOTE_STATE_DANCE					= 10,
		EMOTE_ONESHOT_LAUGH					= 11,
		EMOTE_STATE_SLEEP					= 12,
		EMOTE_STATE_SIT						= 13,
		EMOTE_ONESHOT_RUDE					= 14, -- DNR
		EMOTE_ONESHOT_ROAR					= 15, -- DNR
		EMOTE_ONESHOT_KNEEL					= 16,
		EMOTE_ONESHOT_KISS					= 17,
		EMOTE_ONESHOT_CRY					= 18,
		EMOTE_ONESHOT_CHICKEN				= 19,
		EMOTE_ONESHOT_BEG					= 20,
		EMOTE_ONESHOT_APPLAUD				= 21,
		EMOTE_ONESHOT_SHOUT					= 22, -- DNR
		EMOTE_ONESHOT_FLEX					= 23,
		EMOTE_ONESHOT_SHY					= 24, -- DNR
		EMOTE_ONESHOT_POINT					= 25, -- DNR
		EMOTE_STATE_STAND					= 26,
		EMOTE_STATE_READYUNARMED			= 27,
		EMOTE_STATE_WORK_SHEATHED			= 28,
		EMOTE_STATE_POINT					= 29, -- DNR
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
		EMOTE_ONESHOT_YES					= 273, -- DNR
		EMOTE_ONESHOT_NO					= 274, -- DNR
		EMOTE_ONESHOT_TRAIN					= 275, -- DNR
		EMOTE_ONESHOT_LAND					= 293,
		EMOTE_STATE_AT_EASE					= 313,
		EMOTE_STATE_READY1H					= 333,
		EMOTE_STATE_SPELLKNEELSTART			= 353,
		--EMOTE_STATE_SUBMERGED				= 373,
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
		EMOTE_STATE_SUBMERGED				= 429, -- NEW
		EMOTE_ONESHOT_COWER					= 430, -- DNR
		EMOTE_STATE_COWER					= 431,
		EMOTE_ONESHOT_USESTANDING			= 432,
		EMOTE_STATE_STEALTH_STAND			= 433,
		EMOTE_ONESHOT_OMNICAST_GHOUL		= 434, -- W/SOUND
		EMOTE_ONESHOT_ATTACKBOW				= 435,
		EMOTE_ONESHOT_ATTACKRIFLE			= 436,
		EMOTE_STATE_SWIM_IDLE				= 437,
		EMOTE_STATE_ATTACK_UNARMED			= 438,
		--EMOTE_ONESHOT_SPELLCAST				= 439, -- W/SOUND
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
		EMOTE_ZZOLD_STATE_KNEEL_NO_INTERRUPT= 470
	}
function LCF.__index(obj,key)
	if(type(obj) == "table") then
		return LCF.vars[key]
	elseif(type(obj) == "userdata") then
		local objtype = obj:GetObjectType()
		assert(objtype)
		if(objtype == "Unit") then
			return LCF.CreatureMethods[key]
		elseif(objtype == "Player") then
			return LCF.PlayerMethods[key]
		elseif(objtype == "GameObject") then
			return LCF.GOMethods[key]
		end
	end
	return nil
end
function LCF.vars.__index(tbl,key)
	if(LCF.vars.UNIT_FIELDS[key] ~= nil) then
		return LCF.vars.UNIT_FIELDS[key]
	elseif(LCF.vars.PLAYER_FIELDS[key] ~= nil) then
		return LCF.vars.PLAYER_FIELDS[key]
	elseif(LCF.vars.MSG_FIELDS[key] ~= nil) then
		return LCF.vars.MSG_FIELDS[key]
	elseif(LCF.vars.GAMEOBJECT_FIELDS[key] ~= nil) then
		return LCF.vars.GAMEOBJECT_FIELDS[key]
	elseif(LCF.vars.ITEM_FIELDS[key] ~= nil) then
		return LCF.vars.ITEM_FIELDS[key]
	elseif(LCF.vars.COLOR_FIELDS[key] ~= nil) then
		return LCF.vars.COLOR_FIELDS[key]
	elseif(LCF.vars.UNIT_FLAGS[key] ~= nil) then
		return LCF.vars.UNIT_FLAGS[key]
	elseif(LCF.vars.EMOTE_FIELDS[key] ~= nil) then
		return LCF.vars.EMOTE_FIELDS[key]
	elseif(LCF.vars.CONTAINER_FIELDS[key] ~= nil) then
		return LCF.vars.CONTAINER_FIELDS[key]
	elseif(LCF.vars.CORPSE_FIELDS[key] ~= nil) then
		return LCF.vars.CORPSE_FIELDS[key]
	elseif(LCF.vars.DYNOBJ_FIELDS[key] ~= nil) then
		return LCF.vars.DYNOBJ_FIELDS[key]
	end
	return nil
end
-- Now we set the metatables of our wow objects.
setmetatable(Unit,LCF)
setmetatable(GameObject,LCF)
setmetatable(LCF,LCF)
setmetatable(LCF.vars,LCF.vars)
--Might expand these to the other wow objects if they really do need wrappers

	
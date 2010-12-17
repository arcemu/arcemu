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

//! 4-bit flag
enum AURA_FLAGS
{
	AFLAG_EMPTY			= 0x0,
	AFLAG_EFFECT_1		= 0x1,
	AFLAG_EFFECT_2		= 0x2,
	AFLAG_EFFECT_3		= 0x4,
	AFLAG_NOT_CASTER	= 0x8,
	AFLAG_SET			= 0x9,
	AFLAG_CANCELLABLE	= 0x10,
	AFLAG_DURATION		= 0x20,
	AFLAG_HIDE			= 0x40, // Seems to hide the aura and tell client the aura was removed
	AFLAG_NEGATIVE		= 0x80
};

enum AURA_INTERNAL_USAGE_FLAGS
{
	//if all 3 mods are resisted then we can send client as a fully resisted spell.
	//don't change the value of these !
	MOD_0_RESISTED	= 1,
	MOD_1_RESISTED	= 2,
	MOD_2_RESISTED	= 4,
};

enum AURA_STATE_FLAGS
{
	AURASTATE_FLAG_DODGE_BLOCK			= 0x000001,	//1
	AURASTATE_FLAG_HEALTH20				= 0x000002,	//2
	AURASTATE_FLAG_BERSERK				= 0x000004,	//3
	AURASTATE_FLAG_FROZEN				= 0x000008,	//4
	AURASTATE_FLAG_JUDGEMENT			= 0x000010,	//5
	AURASTATE_FLAG_PARRY				= 0x000040,	//7
	AURASTATE_FLAG_LASTKILLWITHHONOR	= 0x000200,	//10
	AURASTATE_FLAG_CRITICAL				= 0x000400,	//11
	AURASTATE_FLAG_HEALTH35				= 0x001000,	//13
	AURASTATE_FLAG_IMMOLATE				= 0x002000,	//14
	AURASTATE_FLAG_REJUVENATE			= 0x004000,	//15
	AURASTATE_FLAG_POISON				= 0x008000,	//16
	AURASTATE_FLAG_ENRAGED				= 0x010000,	//17
	AURASTATE_FLAG_BLEED				= 0x020000,	//18
	AURASTATE_FLAG_EVASIVE_CHARGE		= 0x200000,	//22
	AURASTATE_FLAG_HEALTH75				= 0x400000,	//23 (Health ABOVE 75%)
};

enum MOD_TYPES
{
    SPELL_AURA_NONE = 0,                                // None
    SPELL_AURA_BIND_SIGHT = 1,                          // Bind Sight
    SPELL_AURA_MOD_POSSESS = 2,                         // Mod Possess
    SPELL_AURA_PERIODIC_DAMAGE = 3,                     // Periodic Damage
    SPELL_AURA_DUMMY = 4,                               // Script Aura
    SPELL_AURA_MOD_CONFUSE = 5,                         // Mod Confuse
    SPELL_AURA_MOD_CHARM = 6,                           // Mod Charm
    SPELL_AURA_MOD_FEAR = 7,                            // Mod Fear
    SPELL_AURA_PERIODIC_HEAL = 8,                       // Periodic Heal
    SPELL_AURA_MOD_ATTACKSPEED = 9,                     // Mod Attack Speed
    SPELL_AURA_MOD_THREAT = 10,                         // Mod Threat
    SPELL_AURA_MOD_TAUNT = 11,                          // Taunt
    SPELL_AURA_MOD_STUN = 12,                           // Stun
    SPELL_AURA_MOD_DAMAGE_DONE = 13,                    // Mod Damage Done
    SPELL_AURA_MOD_DAMAGE_TAKEN = 14,                   // Mod Damage Taken
    SPELL_AURA_DAMAGE_SHIELD = 15,                      // Damage Shield
    SPELL_AURA_MOD_STEALTH = 16,                        // Mod Stealth
    SPELL_AURA_MOD_DETECT = 17,                         // Mod Detect
    SPELL_AURA_MOD_INVISIBILITY = 18,                   // Mod Invisibility
    SPELL_AURA_MOD_INVISIBILITY_DETECTION = 19,         // Mod Invisibility Detection
    SPELL_AURA_MOD_TOTAL_HEALTH_REGEN_PCT = 20,
    SPELL_AURA_MOD_TOTAL_MANA_REGEN_PCT = 21,
    SPELL_AURA_MOD_RESISTANCE = 22,                     // Mod Resistance
    SPELL_AURA_PERIODIC_TRIGGER_SPELL = 23,             // Periodic Trigger
    SPELL_AURA_PERIODIC_ENERGIZE = 24,                  // Periodic Energize
    SPELL_AURA_MOD_PACIFY = 25,                         // Pacify
    SPELL_AURA_MOD_ROOT = 26,                           // Root
    SPELL_AURA_MOD_SILENCE = 27,                        // Silence
    SPELL_AURA_REFLECT_SPELLS = 28,                     // Reflect Spells %
    SPELL_AURA_MOD_STAT = 29,                           // Mod Stat
    SPELL_AURA_MOD_SKILL = 30,                          // Mod Skill
    SPELL_AURA_MOD_INCREASE_SPEED = 31,                 // Mod Speed
    SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED = 32,         // Mod Speed Mounted
    SPELL_AURA_MOD_DECREASE_SPEED = 33,                 // Mod Speed Slow
    SPELL_AURA_MOD_INCREASE_HEALTH = 34,                // Mod Increase Health
    SPELL_AURA_MOD_INCREASE_ENERGY = 35,                // Mod Increase Energy
    SPELL_AURA_MOD_SHAPESHIFT = 36,                     // Shapeshift
    SPELL_AURA_EFFECT_IMMUNITY = 37,                    // Immune Effect
    SPELL_AURA_STATE_IMMUNITY = 38,                     // Immune State
    SPELL_AURA_SCHOOL_IMMUNITY = 39,                    // Immune School
    SPELL_AURA_DAMAGE_IMMUNITY = 40,                    // Immune Damage
    SPELL_AURA_DISPEL_IMMUNITY = 41,                    // Immune Dispel Type
    SPELL_AURA_PROC_TRIGGER_SPELL = 42,                 // Proc Trigger Spell
    SPELL_AURA_PROC_TRIGGER_DAMAGE = 43,                // Proc Trigger Damage
    SPELL_AURA_TRACK_CREATURES = 44,                    // Track Creatures
    SPELL_AURA_TRACK_RESOURCES = 45,                    // Track Resources
    SPELL_AURA_MOD_PARRY_SKILL = 46,                    // Mod Parry Skill
    SPELL_AURA_MOD_PARRY_PERCENT = 47,                  // Mod Parry Percent
    SPELL_AURA_MOD_DODGE_SKILL = 48,                    // Mod Dodge Skill
    SPELL_AURA_MOD_DODGE_PERCENT = 49,                  // Mod Dodge Percent
    SPELL_AURA_MOD_BLOCK_SKILL = 50,                    // Mod Block Skill
    SPELL_AURA_MOD_BLOCK_PERCENT = 51,                  // Mod Block Percent
    SPELL_AURA_MOD_CRIT_PERCENT = 52,                   // Mod Crit Percent
    SPELL_AURA_PERIODIC_LEECH = 53,                     // Periodic Leech
    SPELL_AURA_MOD_HIT_CHANCE = 54,                     // Mod Hit Chance
    SPELL_AURA_MOD_SPELL_HIT_CHANCE = 55,               // Mod Spell Hit Chance
    SPELL_AURA_TRANSFORM = 56,                          // Transform
    SPELL_AURA_MOD_SPELL_CRIT_CHANCE = 57,              // Mod Spell Crit Chance
    SPELL_AURA_MOD_INCREASE_SWIM_SPEED = 58,            // Mod Speed Swim
    SPELL_AURA_MOD_DAMAGE_DONE_CREATURE = 59,           // Mod Creature Dmg Done
    SPELL_AURA_MOD_PACIFY_SILENCE = 60,                 // Pacify & Silence
    SPELL_AURA_MOD_SCALE = 61,                          // Mod Scale
    SPELL_AURA_PERIODIC_HEALTH_FUNNEL = 62,             // Periodic Health Funnel
    SPELL_AURA_PERIODIC_MANA_FUNNEL = 63,               // Periodic Mana Funnel
    SPELL_AURA_PERIODIC_MANA_LEECH = 64,                // Periodic Mana Leech
    SPELL_AURA_MOD_CASTING_SPEED = 65,                  // Haste - Spells
    SPELL_AURA_FEIGN_DEATH = 66,                        // Feign Death
    SPELL_AURA_MOD_DISARM = 67,                         // Disarm
    SPELL_AURA_MOD_STALKED = 68,                        // Mod Stalked
    SPELL_AURA_SCHOOL_ABSORB = 69,                      // School Absorb
    SPELL_AURA_EXTRA_ATTACKS = 70,                      // Extra Attacks
    SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL = 71,       // Mod School Spell Crit Chance
    SPELL_AURA_MOD_POWER_COST = 72,                     // Mod Power Cost
    SPELL_AURA_MOD_POWER_COST_SCHOOL = 73,              // Mod School Power Cost
    SPELL_AURA_REFLECT_SPELLS_SCHOOL = 74,              // Reflect School Spells %
    SPELL_AURA_MOD_LANGUAGE = 75,                       // Mod Language
    SPELL_AURA_FAR_SIGHT = 76,                          // Far Sight
    SPELL_AURA_MECHANIC_IMMUNITY = 77,                  // Immune Mechanic
    SPELL_AURA_MOUNTED = 78,                            // Mounted
    SPELL_AURA_MOD_DAMAGE_PERCENT_DONE = 79,            // Mod Dmg %
    SPELL_AURA_MOD_PERCENT_STAT = 80,                   // Mod Stat %
    SPELL_AURA_SPLIT_DAMAGE = 81,                       // Split Damage
    SPELL_AURA_WATER_BREATHING = 82,                    // Water Breathing
    SPELL_AURA_MOD_BASE_RESISTANCE = 83,                // Mod Base Resistance
    SPELL_AURA_MOD_REGEN = 84,                          // Mod Health Regen
    SPELL_AURA_MOD_POWER_REGEN = 85,                    // Mod Power Regen
    SPELL_AURA_CHANNEL_DEATH_ITEM = 86,                 // Create Death Item
    SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN = 87,           // Mod Dmg % Taken
    SPELL_AURA_MOD_PERCENT_REGEN = 88,                  // Mod Health Regen Percent
    SPELL_AURA_PERIODIC_DAMAGE_PERCENT = 89,            // Periodic Damage Percent
    SPELL_AURA_MOD_RESIST_CHANCE = 90,                  // Mod Resist Chance
    SPELL_AURA_MOD_DETECT_RANGE = 91,                   // Mod Detect Range
    SPELL_AURA_PREVENTS_FLEEING = 92,                   // Prevent Fleeing
	SPELL_AURA_MOD_UNATTACKABLE = 93,                   // Mod Unintractable
    SPELL_AURA_INTERRUPT_REGEN = 94,                    // Interrupt Regen
    SPELL_AURA_GHOST = 95,                              // Ghost
    SPELL_AURA_SPELL_MAGNET = 96,                       // Spell Magnet
    SPELL_AURA_MANA_SHIELD = 97,                        // Mana Shield
    SPELL_AURA_MOD_SKILL_TALENT = 98,                   // Mod Skill Talent
    SPELL_AURA_MOD_ATTACK_POWER = 99,                   // Mod Attack Power
    SPELL_AURA_AURAS_VISIBLE = 100,                     // Auras Visible
    SPELL_AURA_MOD_RESISTANCE_PCT = 101,                // Mod Resistance %
    SPELL_AURA_MOD_CREATURE_ATTACK_POWER = 102,         // Mod Creature Attack Power
    SPELL_AURA_MOD_TOTAL_THREAT = 103,                  // Mod Total Threat (Fade)
    SPELL_AURA_WATER_WALK = 104,                        // Water Walk
    SPELL_AURA_FEATHER_FALL = 105,                      // Feather Fall
    SPELL_AURA_HOVER = 106,                             // Hover
    SPELL_AURA_ADD_FLAT_MODIFIER = 107,                 // Add Flat Modifier
    SPELL_AURA_ADD_PCT_MODIFIER = 108,                  // Add % Modifier
    SPELL_AURA_ADD_CLASS_TARGET_TRIGGER = 109,          // Add Class Target Trigger
    SPELL_AURA_MOD_POWER_REGEN_PERCENT = 110,           // Mod Power Regen %
    SPELL_AURA_ADD_CASTER_HIT_TRIGGER = 111,            // Add Class Caster Hit Trigger
    SPELL_AURA_OVERRIDE_CLASS_SCRIPTS = 112,            // Override Class Scripts
    SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN = 113,           // Mod Ranged Dmg Taken
    SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT = 114,       // Mod Ranged % Dmg Taken
    SPELL_AURA_MOD_HEALING = 115,                       // Mod Healing
    SPELL_AURA_IGNORE_REGEN_INTERRUPT = 116,            // Regen During Combat
    SPELL_AURA_MOD_MECHANIC_RESISTANCE = 117,           // Mod Mechanic Resistance
    SPELL_AURA_MOD_HEALING_PCT = 118,                   // Mod Healing %
    SPELL_AURA_SHARE_PET_TRACKING = 119,                // Share Pet Tracking
    SPELL_AURA_UNTRACKABLE = 120,                       // Untrackable
    SPELL_AURA_EMPATHY = 121,                           // Empathy (Lore, whatever)
    SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT = 122,            // Mod Offhand Dmg %
    SPELL_AURA_MOD_POWER_COST_PCT = 123,                // Mod Power Cost % --> armor penetration & spell penetration
    SPELL_AURA_MOD_RANGED_ATTACK_POWER = 124,           // Mod Ranged Attack Power
    SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN = 125,            // Mod Melee Dmg Taken
    SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT = 126,        // Mod Melee % Dmg Taken
    SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS = 127,// Rngd Atk Pwr Attckr Bonus
    SPELL_AURA_MOD_POSSESS_PET = 128,                   // Mod Possess Pet
    SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS = 129,         // Mod Speed Always
    SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS = 130,          // Mod Mounted Speed Always
    SPELL_AURA_MOD_CREATURE_RANGED_ATTACK_POWER = 131,  // Mod Creature Ranged Attack Power
    SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT = 132,       // Mod Increase Energy %
    SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT = 133,       // Mod Max Health %
    SPELL_AURA_MOD_MANA_REGEN_INTERRUPT = 134,          // Mod Interrupted Mana Regen
    SPELL_AURA_MOD_HEALING_DONE = 135,                  // Mod Healing Done
    SPELL_AURA_MOD_HEALING_DONE_PERCENT = 136,          // Mod Healing Done %
    SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE = 137,         // Mod Total Stat %
    SPELL_AURA_MOD_HASTE = 138,                         // Haste - Melee
    SPELL_AURA_FORCE_REACTION = 139,                    // Force Reaction
    SPELL_AURA_MOD_RANGED_HASTE = 140,                  // Haste - Ranged
    SPELL_AURA_MOD_RANGED_AMMO_HASTE = 141,             // Haste - Ranged (Ammo Only)
    SPELL_AURA_MOD_BASE_RESISTANCE_PCT = 142,           // Mod Base Resistance %
    SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE = 143,          // Mod Resistance Exclusive
    SPELL_AURA_SAFE_FALL = 144,                         // Safe Fall
    SPELL_AURA_CHARISMA = 145,                          // Charisma
    SPELL_AURA_PERSUADED = 146,                         // Persuaded
    SPELL_AURA_ADD_CREATURE_IMMUNITY = 147,             // Add Creature Immunity
    SPELL_AURA_RETAIN_COMBO_POINTS = 148,               // Retain Combo Points
    SPELL_AURA_RESIST_PUSHBACK = 149,                   // Resist Pushback
    SPELL_AURA_MOD_SHIELD_BLOCK_PCT = 150,              // Mod Shield Block %
    SPELL_AURA_TRACK_STEALTHED = 151,                   // Track Stealthed
    SPELL_AURA_MOD_DETECTED_RANGE = 152,                // Mod Detected Range
    SPELL_AURA_SPLIT_DAMAGE_FLAT = 153,                 // Split Damage Flat
    SPELL_AURA_MOD_STEALTH_LEVEL = 154,                 // Stealth Level Modifier
    SPELL_AURA_MOD_WATER_BREATHING = 155,               // Mod Water Breathing
    SPELL_AURA_MOD_REPUTATION_ADJUST = 156,             // Mod Reputation Gain
    SPELL_AURA_PET_DAMAGE_MULTI = 157,                  // Mod Pet Damage
    SPELL_AURA_MOD_SHIELD_BLOCK = 158,                  // Mod Shield Block
    SPELL_AURA_NO_PVP_CREDIT = 159,                     // No PVP Credit
    SPELL_AURA_MOD_SIDE_REAR_PDAE_DAMAGE_TAKEN = 160,   // Mod Side/Rear PBAE Damage Taken
    SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT = 161,        // Mod Health Regen In Combat
    SPELL_AURA_POWER_BURN = 162,                        // Power Burn
    SPELL_AURA_MOD_CRIT_DAMAGE_BONUS_MELEE = 163,       // Mod Critical Damage Bonus (Physical)
    SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS = 165, // Melee AP Attacker Bonus
    SPELL_AURA_MOD_ATTACK_POWER_PCT = 166,              // Mod Attack Power
    SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT = 167,       // Mod Ranged Attack Power %
    SPELL_AURA_INCREASE_DAMAGE = 168,                   // Increase Damage Type
    SPELL_AURA_INCREASE_CRITICAL = 169,                 // Increase Critical Type
    SPELL_AURA_DETECT_AMORE = 170,                      // Detect Amore
    SPELL_AURA_INCREASE_MOVEMENT_AND_MOUNTED_SPEED = 172,// Increase Movement and Mounted Speed (Non-Stacking)
    SPELL_AURA_INCREASE_SPELL_DAMAGE_PCT = 174,         // Increase Spell Damage by % status
    SPELL_AURA_INCREASE_SPELL_HEALING_PCT = 175,        // Increase Spell Healing by % status
    SPELL_AURA_SPIRIT_OF_REDEMPTION = 176,              // Spirit of Redemption Auras
    SPELL_AURA_AREA_CHARM = 177,                        // Area Charm
    SPELL_AURA_INCREASE_ATTACKER_SPELL_CRIT = 179,      // Increase Attacker Spell Crit Type
    SPELL_AURA_INCREASE_SPELL_DAMAGE_VS_TYPE = 180,     // Increase Spell Damage Type
    SPELL_AURA_INCREASE_ARMOR_BASED_ON_INTELLECT_PCT = 182, // Increase Armor based on Intellect
    SPELL_AURA_DECREASE_CRIT_THREAT = 183,              // Decrease Critical Threat by
    SPELL_AURA_DECREASE_ATTACKER_CHANCE_TO_HIT_MELEE = 184,//Reduces Attacker Chance to Hit with Melee
    SPELL_AURA_DECREASE_ATTACKER_CHANGE_TO_HIT_RANGED = 185,// Reduces Attacker Chance to Hit with Ranged
    SPELL_AURA_DECREASE_ATTACKER_CHANGE_TO_HIT_SPELLS = 186,// Reduces Attacker Chance to Hit with Spells
    SPELL_AURA_DECREASE_ATTACKER_CHANGE_TO_CRIT_MELEE = 187,// Reduces Attacker Chance to Crit with Melee (Ranged?)
    SPELL_AURA_DECREASE_ATTACKER_CHANGE_TO_CRIT_RANGED = 188,// Reduces Attacker Chance to Crit with Ranged (Melee?)
    SPELL_AURA_INCREASE_REPUTATION = 190,               // Increases reputation from killed creatures
    SPELL_AURA_SPEED_LIMIT = 191,                       // speed limit
    SPELL_AURA_MELEE_SLOW_PCT = 192,
    SPELL_AURA_INCREASE_TIME_BETWEEN_ATTACKS = 193,
    SPELL_AURA_INREASE_SPELL_DAMAGE_PCT_OF_INTELLECT = 194,  // NOT USED ANYMORE - 174 used instead
    SPELL_AURA_INCREASE_HEALING_PCT_OF_INTELLECT = 195,		// NOT USED ANYMORE - 175 used instead
    SPELL_AURA_MOD_ALL_WEAPON_SKILLS = 196,
    SPELL_AURA_REDUCE_ATTACKER_CRICTICAL_HIT_CHANCE_PCT = 197,
	SPELL_AURA_198 = 198,
    SPELL_AURA_INCREASE_SPELL_HIT_PCT = 199,
	SPELL_AURA_CANNOT_BE_DODGED = 201,
	SPELL_AURA_FINISHING_MOVES_CANNOT_BE_DODGED = 202,
    SPELL_AURA_REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_MELEE_PCT = 203,
    SPELL_AURA_REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_RANGED_PCT = 204,
	SPELL_AURA_ENABLE_FLIGHT = 206,
	SPELL_AURA_ENABLE_FLIGHT2 = 207,
	SPELL_AURA_ENABLE_FLIGHT_WITH_UNMOUNTED_SPEED = 208,
    SPELL_AURA_MOD_RANGED_ATTACK_POWER_BY_STAT_PCT = 212,
    SPELL_AURA_INCREASE_RAGE_FROM_DAMAGE_DEALT_PCT = 213,
    SPELL_AURA_INCREASE_CASTING_TIME_PCT = 216,
    SPELL_AURA_REGEN_MANA_STAT_PCT=219,
    SPELL_AURA_HEALING_STAT_PCT=220,
	SPELL_AURA_PERIODIC_TRIGGER_DUMMY=226,
	SPELL_AURA_PERIODIC_TRIGGER_SPELL_WITH_VALUE=227, // Used by Mind Flay etc
	SPELL_AURA_REDUCE_AOE_DAMAGE_TAKEN = 229,
    SPELL_AURA_INCREASE_MAX_HEALTH=230,//Used by Commanding Shout
	SPELL_AURA_MECHANIC_DURATION_MOD = 232,
	SPELL_AURA_MOD_HEALING_BY_AP=237,
	SPELL_AURA_MOD_SPELL_DAMAGE_BY_AP=238,
	SPELL_AURA_EXPERTISE = 240,
	SPELL_AURA_241 = 241,
    SPELL_AURA_MOD_SPELL_DAMAGE_FROM_HEALING = 242,
    SPELL_AURA_243 = 243,
    SPELL_AURA_244 = 244,
    SPELL_AURA_MOD_DURATION_OF_MAGIC_EFFECTS = 245,
    SPELL_AURA_246 = 246,
    SPELL_AURA_247 = 247,
    SPELL_AURA_MOD_COMBAT_RESULT_CHANCE = 248,
    SPELL_AURA_249 = 249,
    SPELL_AURA_MOD_INCREASE_HEALTH_2 = 250,
    SPELL_AURA_MOD_ENEMY_DODGE = 251,
	SPELL_AURA_252 = 252,
	SPELL_AURA_BLOCK_MULTIPLE_DAMAGE = 253,
	SPELL_AURA_254 = 254,
	SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT = 255,
	SPELL_AURA_256 = 256,
	SPELL_AURA_257 = 257,
	SPELL_AURA_258 = 258,
	SPELL_AURA_259 = 259,
	SPELL_AURA_260 = 260,
	SPELL_AURA_PHASE = 261,
	SPELL_AURA_IGNORE_TARGET_AURA_STATE = 262,
	SPELL_AURA_ALLOW_ONLY_ABILITY = 263,
	SPELL_AURA_264 = 264,
	SPELL_AURA_265 = 265,
	SPELL_AURA_266 = 266,
	SPELL_AURA_267 = 267,
	SPELL_AURA_MOD_ATTACK_POWER_BY_STAT_PCT = 268,
	SPELL_AURA_269 = 269,
	SPELL_AURA_270 = 270,
	SPELL_AURA_INCREASE_SPELL_DOT_DAMAGE_PCT = 271,
	SPELL_AURA_272 = 272,
	SPELL_AURA_273 = 273,
	SPELL_AURA_CONSUMES_NO_AMMO = 274,
	SPELL_AURA_275 = 275,
	SPELL_AURA_276 = 276,
	SPELL_AURA_277 = 277,
	SPELL_AURA_278 = 278,
	SPELL_AURA_279 = 279,
	SPELL_AURA_IGNORE_ARMOR_PCT = 280,
	SPELL_AURA_281 = 281,
	SPELL_AURA_MOD_BASE_HEALTH = 282,
	SPELL_AURA_283 = 283,
	SPELL_AURA_284 = 284,
	SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR = 285,
	SPELL_AURA_ALLOW_DOT_TO_CRIT = 286,
	SPELL_AURA_REFLECT_INFRONT = 287,
	SPELL_AURA_288 = 288,
	SPELL_AURA_289 = 289,
	SPELL_AURA_290 = 290,
	SPELL_AURA_291 = 291,
	SPELL_AURA_292 = 292,
	SPELL_AURA_293 = 293,
	SPELL_AURA_294 = 294,
	SPELL_AURA_295 = 295,
	SPELL_AURA_296 = 296,
	SPELL_AURA_297 = 297,
	SPELL_AURA_298 = 298,
	SPELL_AURA_299 = 299,
	SPELL_AURA_300 = 300,
	SPELL_AURA_301 = 301,
	SPELL_AURA_302 = 302,
	SPELL_AURA_303 = 303,
	SPELL_AURA_304 = 304,
	SPELL_AURA_305 = 305,
	SPELL_AURA_306 = 306,
    SPELL_AURA_307 = 307,
    SPELL_AURA_308 = 308,
    SPELL_AURA_309 = 309,
    SPELL_AURA_310 = 310,
    SPELL_AURA_311 = 311,
    SPELL_AURA_312 = 312,
    SPELL_AURA_313 = 313,
    SPELL_AURA_314 = 314,
    SPELL_AURA_315 = 315,
    SPELL_AURA_316 = 316,
    TOTAL_SPELL_AURAS = 317,
};

enum AuraTickFlags
{
    FLAG_PERIODIC_DAMAGE            = 2,
    FLAG_PERIODIC_TRIGGER_SPELL     = 4,
    FLAG_PERIODIC_HEAL              = 8,
    FLAG_PERIODIC_LEECH             = 16,
    FLAG_PERIODIC_ENERGIZE          = 32,
};

struct Modifier
{
//    uint32 m_actamt;       // actual amt, for percent and stuff
    uint32 m_type;           // What does it modify? (str,int,hp)
    int32 m_amount;          // By how much does it mod?
    int32 m_miscValue;       // Misc Value
    uint32 i;

    ///needed for per level effect
    int32 realamount;
	//need this to store % values or they cannot be reverted correctly (i think :D )
    signed int fixed_amount[SCHOOL_COUNT];
};


struct ProcTriggerSpellOnSpell
{
    uint32 origId;
    uint32 spellId;
    uint64 caster;
    uint32 procChance;
    uint32 procFlags;
    uint32 RemainingCharges;
    uint32 LastTrigger;
    void  *owner;//mark the owner of this proc to know which one to delete
};

struct DamageProc
{
    uint32 m_spellId;
    uint32 m_damage;
  //  uint64 m_caster;//log is: some reflects x arcane/nature damage to 'attacker' no matter who casted
    uint32 m_school;
    uint32 m_flags;
    void  *owner;//mark the owner of this proc to know which one to delete
};

struct DamageSplitTarget
{
	uint64 m_target; // we store them
	uint32 m_spellId;
	float m_pctDamageSplit; // % of taken damage to transfer (i.e. Soul Link)
	uint32 m_flatDamageSplit; // flat damage to transfer (i.e. Blessing of Sacrifice)
	uint8 damage_type; // bitwise 0-127 thingy
	void * creator;
};

#ifdef NEW_PROCFLAGS
struct ProcTriggerSpell
{
	uint32 spellId;
	uint32 parentId;
	uint32 procFlags;
	uint32 procChance;
	uint32 procCharges;
	uint32 LastTrigger;
};
#endif

struct SpellCharge
{
    uint32 spellId;
    uint32 count;
    uint32 ProcFlag;
    uint32 lastproc;
    uint32 procdiff;
};

typedef set< uint64 > AreaAuraList;

class SERVER_DECL Aura : public EventableObject
{
	uint64 periodic_target;
public:
    Aura( SpellEntry *proto, int32 duration,Object* caster, Unit *target, bool temporary = false, Item* i_caster = NULL );
	~Aura();

	void ExpireRemove();
    void Remove();
    void Expire();
    void AddMod(uint32 t, int32 a,uint32 miscValue,uint32 i);

    ARCEMU_INLINE SpellEntry* GetSpellProto() const { return m_spellProto; }
    ARCEMU_INLINE uint32 GetSpellId() const {return m_spellProto->Id; }
    ARCEMU_INLINE bool IsPassive(){ if(!m_spellProto) return false; return (m_spellProto->Attributes & ATTRIBUTES_PASSIVE && !m_areaAura);}

	void ResetDuration();

    ARCEMU_INLINE int32 GetDuration() const { return m_duration; }
	void SetDuration(int32 duration)
	{
		m_duration = duration;
		ResetDuration();
	}

    ARCEMU_INLINE uint16 GetAuraSlot() const { return m_auraSlot; }
	void SetAuraSlot( uint16 slot ) { m_auraSlot = slot; }

	ARCEMU_INLINE bool IsPositive() { return m_positive>0; }
	void SetNegative(signed char value=1) { m_positive -= value; }
	void SetPositive(signed char value=1) { m_positive += value; }

	Object* GetCaster();
	ARCEMU_INLINE uint64 GetCasterGUID(){return m_casterGuid;}
	Unit* GetUnitCaster();
	Player* GetPlayerCaster();
	ARCEMU_INLINE Unit* GetTarget() { return m_target; }

	Aura* StrongerThat(Aura *aur);
	void ApplyModifiers(bool apply);
	void UpdateModifiers();


//////////////////////////////////////////////////////////// Area Auras //////////////////////////////////////////////////////


	void EventUpdateAA( float r );

	void EventUpdateGroupAA( float r );
	void EventUpdateRaidAA( float r );
	void EventUpdatePetAA( float r );
	void EventUpdateFriendAA( float r );
	void EventUpdateEnemyAA( float r );
	void EventUpdateOwnerAA( float r );

	/////////////////////////////////////////////////////////
	//void ClearAATargets()
	//  Removes the Area Aura from all targets and clears
	//  the target set.
	//
	//Parameters
	//  None
	//
	//Return Value
	//  None
	//
	//
	////////////////////////////////////////////////////////
	void ClearAATargets();

	////////////////////////////////////////////////////////
	//bool IsAreaAura()
	//  Tells if the Aura is an area Aura.
	//
	//Parameters
	//  None
	//
	//Return Value
	//  Returns true if it is.
	//  Returns false otherwise.
	//
	///////////////////////////////////////////////////////
	bool IsAreaAura();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool DotCanCrit();

	//! GetTimeLeft() milliseconds
	uint32 GetTimeLeft() 
	{
		if(m_duration==-1)return (uint32)-1;
		int32 n=int32((UNIXTIME-time_t(timeleft))*1000);
		if(n>=m_duration) return 0;
		else
		return (m_duration-n);
	}

	bool HasModType( uint32 type )
	{
		for( uint8 x = 0; x < m_modcount; ++x )
			if( m_modList[x].m_type == type )
				return true;
		return false;
	}
	// Aura Handlers
	void SpellAuraNULL(bool apply);
	void SpellAuraBindSight(bool apply);
	void SpellAuraModPossess(bool apply);
	void SpellAuraPeriodicDamage(bool apply);
	void SpellAuraDummy(bool apply);
	void SpellAuraModConfuse(bool apply);
	void SpellAuraModCharm(bool apply);
	void SpellAuraModFear(bool apply);
	void SpellAuraPeriodicHeal(bool apply);
	void SpellAuraModAttackSpeed(bool apply);
	void SpellAuraModThreatGenerated(bool apply);
	void SpellAuraModTaunt(bool apply);
	void SpellAuraModStun(bool apply);
	void SpellAuraModDamageDone(bool apply);
	void SpellAuraModDamageTaken(bool apply);
	void SpellAuraDamageShield(bool apply);
	void SpellAuraModStealth(bool apply);
	void SpellAuraModDetect(bool apply);
	void SpellAuraModInvisibility(bool apply);
	void SpellAuraModInvisibilityDetection(bool apply);
	void SpellAuraModTotalHealthRegenPct(bool apply);
	void SpellAuraModTotalManaRegenPct(bool apply);
	void SpellAuraModResistance(bool apply);
	void SpellAuraPeriodicTriggerSpell(bool apply);
	void SpellAuraPeriodicEnergize(bool apply);
	void SpellAuraModPacify(bool apply);
	void SpellAuraModRoot(bool apply);
	void SpellAuraModSilence(bool apply);
	void SpellAuraReflectSpells(bool apply);
	void SpellAuraModStat(bool apply);
	void SpellAuraModSkill(bool apply);
	void SpellAuraModIncreaseSpeed(bool apply);
	void SpellAuraModDecreaseSpeed(bool apply);
	void SpellAuraModIncreaseHealth(bool apply);
	void SpellAuraModIncreaseEnergy(bool apply);
	void SpellAuraModShapeshift(bool apply);
	void SpellAuraModEffectImmunity(bool apply);
	void SpellAuraModStateImmunity(bool apply);
	void SpellAuraModSchoolImmunity(bool apply);
	void SpellAuraModDmgImmunity(bool apply);
	void SpellAuraModDispelImmunity(bool apply);
	void SpellAuraProcTriggerSpell(bool apply);
	void SpellAuraProcTriggerDamage(bool apply);
	void SpellAuraTrackCreatures(bool apply);
	void SpellAuraTrackResources(bool apply);
	void SpellAuraModParryPerc(bool apply);
	void SpellAuraModDodgePerc(bool apply);
	void SpellAuraModBlockPerc(bool apply);
	void SpellAuraModCritPerc(bool apply);
	void SpellAuraPeriodicLeech(bool apply);
	void SpellAuraModHitChance(bool apply);
	void SpellAuraModSpellHitChance(bool apply);
	void SpellAuraTransform(bool apply);
	void SpellAuraModSpellCritChance(bool apply);
	void SpellAuraIncreaseSwimSpeed(bool apply);
	void SpellAuraModCratureDmgDone(bool apply);
	void SpellAuraPacifySilence(bool apply);
	void SpellAuraModScale(bool apply);
	void SpellAuraPeriodicHealthFunnel(bool apply);
	void SpellAuraPeriodicManaLeech(bool apply);
	void SpellAuraModCastingSpeed(bool apply);
	void SpellAuraFeignDeath(bool apply);
	void SpellAuraModDisarm(bool apply);
	void SpellAuraModStalked(bool apply);
	void SpellAuraSchoolAbsorb(bool apply);
	void SpellAuraModSpellCritChanceSchool(bool apply);
	void SpellAuraModPowerCost(bool apply);
	void SpellAuraModPowerCostSchool(bool apply);
	void SpellAuraReflectSpellsSchool(bool apply);
	void SpellAuraModLanguage(bool apply);
	void SpellAuraAddFarSight(bool apply);
	void SpellAuraMechanicImmunity(bool apply);
	void SpellAuraMounted(bool apply);
	void SpellAuraModDamagePercDone(bool apply);
	void SpellAuraModPercStat(bool apply);
	void SpellAuraSplitDamage(bool apply);
	void SpellAuraWaterBreathing(bool apply);
	void SpellAuraModBaseResistance(bool apply);
	void SpellAuraModRegen(bool apply);
	void SpellAuraModPowerRegen(bool apply);
	void SpellAuraModHealingPct(bool apply);
	void SpellAuraChannelDeathItem(bool apply);
	void SpellAuraModDamagePercTaken(bool apply);
	void SpellAuraModRegenPercent(bool apply);
	void SpellAuraPeriodicDamagePercent(bool apply);
	void SpellAuraModResistChance(bool apply);
	void SpellAuraModDetectRange(bool apply);
	void SpellAuraPreventsFleeing(bool apply);
	void SpellAuraModUnattackable(bool apply);
	void SpellAuraInterruptRegen(bool apply);
	void SpellAuraGhost(bool apply);
	void SpellAuraMagnet(bool apply);
	void SpellAuraManaShield(bool apply);
	void SpellAuraSkillTalent(bool apply);
	void SpellAuraModAttackPower(bool apply);
	void SpellAuraVisible(bool apply);
	void SpellAuraModResistancePCT(bool apply);
	void SpellAuraModCreatureAttackPower(bool apply);
	void SpellAuraModTotalThreat(bool apply);
	void SpellAuraWaterWalk(bool apply);
	void SpellAuraFeatherFall(bool apply);
	void SpellAuraHover(bool apply);
	void SpellAuraAddFlatModifier(bool apply);
	void SpellAuraAddPctMod(bool apply);
	void SpellAuraAddClassTargetTrigger(bool apply);
	void SpellAuraModPowerRegPerc(bool apply);
	void SpellAuraOverrideClassScripts(bool apply);
	void SpellAuraModRangedDamageTaken(bool apply);
	void SpellAuraModHealing(bool apply);
	void SpellAuraIgnoreRegenInterrupt(bool apply);
	void SpellAuraModMechanicResistance(bool apply);
	void SpellAuraModHealingPCT(bool apply);
	void SpellAuraUntrackable(bool apply);
	void SpellAuraEmphaty(bool apply);
	void SpellAuraModOffhandDamagePCT(bool apply);
	void SpellAuraModPenetration(bool apply);
	void SpellAuraModRangedAttackPower(bool apply);
	void SpellAuraModMeleeDamageTaken(bool apply);
	void SpellAuraModMeleeDamageTakenPct(bool apply);
	void SpellAuraRAPAttackerBonus(bool apply);
	void SpellAuraModIncreaseSpeedAlways(bool apply);
	void SpellAuraModIncreaseMountedSpeed(bool apply);
	void SpellAuraModCreatureRangedAttackPower(bool apply);
	void SpellAuraModIncreaseEnergyPerc(bool apply);
	void SpellAuraModIncreaseHealthPerc(bool apply);
	void SpellAuraModManaRegInterrupt(bool apply);
	void SpellAuraModHealingDone(bool apply);
	void SpellAuraModHealingDonePct(bool apply);
	void SpellAuraModTotalStatPerc(bool apply);
	void SpellAuraModHaste(bool apply);
	void SpellAuraForceReaction(bool apply);
	void SpellAuraModRangedHaste(bool apply);
	void SpellAuraModRangedAmmoHaste(bool apply);
	void SpellAuraModBaseResistancePerc(bool apply);
	void SpellAuraModResistanceExclusive(bool apply);
	void SpellAuraSafeFall(bool apply);
	void SpellAuraRetainComboPoints(bool apply);
	void SpellAuraResistPushback(bool apply);
	void SpellAuraModShieldBlockPCT(bool apply);
	void SpellAuraTrackStealthed(bool apply);
	void SpellAuraModDetectedRange(bool apply);
	void SpellAuraSplitDamageFlat(bool apply);
	void SpellAuraModStealthLevel(bool apply);
	void SpellAuraModUnderwaterBreathing(bool apply);
	void SpellAuraModReputationAdjust(bool apply);
	void SpellAuraNoPVPCredit(bool apply);
	void SpellAuraModHealthRegInCombat(bool apply);
	void SpellAuraPowerBurn(bool apply);
	void SpellAuraModCritDmgPhysical(bool apply);
	void SpellAuraModPAttackPower(bool apply);
	void SpellAuraModRangedAttackPowerPct(bool apply);
	void SpellAuraAPAttackerBonus(bool apply);
	void SpellAuraIncreaseDamageTypePCT(bool apply);
	void SpellAuraIncreaseCricticalTypePCT(bool apply);
	void SpellAuraIncreasePartySpeed(bool apply);
	void SpellAuraIncreaseSpellDamageByAttribute(bool apply);
	void SpellAuraIncreaseHealingByAttribute(bool apply);
	void SpellAuraIncreaseArmorByPctInt(bool apply);
	void SpellAuraReduceAttackerMHitChance(bool apply);
	void SpellAuraReduceAttackerRHitChance(bool apply);
	void SpellAuraReduceAttackerSHitChance(bool apply);
	void SpellAuraReduceEnemyMCritChance(bool apply);
	void SpellAuraReduceEnemyRCritChance(bool apply);
	void SpellAuraLimitSpeed(bool apply);
	void SpellAuraIncreaseTimeBetweenAttacksPCT(bool apply);
//	void SpellAuraIncreaseSpellDamageByInt(bool apply);
//	void SpellAuraIncreaseHealingByInt(bool apply);
	void SpellAuraIncreaseAllWeaponSkill(bool apply);
	void SpellAuraModAttackerCritChance(bool apply);
	void SpellAuraIncreaseHitRate(bool apply);
	void SpellAuraReduceCritMeleeAttackDmg(bool apply);
	void SpellAuraReduceCritRangedAttackDmg(bool apply);
	void SpellAuraEnableFlight(bool apply);
	void SpellAuraEnableFlightWithUnmountedSpeed(bool apply);
	void SpellAuraIncreaseRageFromDamageDealtPCT(bool apply);
	void SpellAuraIncreaseFlightSpeed(bool apply);
	void SpellAuraIncreaseMovementAndMountedSpeed(bool apply);
	void SpellAuraIncreaseRating(bool apply);
	//void SpellAuraIncreaseCastTimePCT(bool apply);
	void SpellAuraRegenManaStatPCT(bool apply);
	void SpellAuraSpellHealingStatPCT(bool apply);
	void SpellAuraModStealthDetection(bool apply);
	void SpellAuraReduceAOEDamageTaken(bool apply);
	void SpellAuraIncreaseMaxHealth(bool apply);
	void SpellAuraSpiritOfRedemption(bool apply);
	void SpellAuraIncreaseAttackerSpellCrit(bool apply);
	void SpellAuraIncreaseRepGainPct(bool apply);
	void SpellAuraIncreaseRAPbyStatPct(bool apply);
	//void SpellAuraModRangedDamageTakenPCT(bool apply);
	void SpellAuraModBlockValue(bool apply);
	void SpellAuraAllowFlight(bool apply);
	void SpellAuraFinishingMovesCannotBeDodged(bool apply);
	void SpellAuraExpertise(bool apply);
	void SpellAuraPeriodicTriggerDummy(bool apply);
	void SpellAuraModPossessPet(bool apply);
	void SpellAuraModHealingByAP(bool apply);
	void SpellAuraModSpellDamageByAP(bool apply);
	void SpellAuraMeleeHaste(bool apply);
	void SpellAuraReduceEffectDuration(bool apply);
	void HandleAuraControlVehicle(bool apply);
	void EventPeriodicDrink(uint32 amount);
	void SpellAuraModCombatResultChance(bool apply);
	void SpellAuraAddHealth(bool apply);
	void SpellAuraRemoveReagentCost(bool apply);
	void SpellAuraPeriodicTriggerSpellWithValue(bool apply);
	void SpellAuraModMechanicDmgTakenPct(bool apply);
	void SpellAuraBlockMultipleDamage(bool apply);
	void SpellAuraIgnoreTargetAuraState(bool apply);
	void SpellAuraAllowOnlyAbility(bool apply);
	void SpellAuraIncreaseAPbyStatPct(bool apply);
	void SpellAuraModSpellDamageDOTPct(bool apply);
	void SpellAuraConsumeNoAmmo( bool apply );
	void SpellAuraIgnoreShapeshift(bool apply);
	void SpellAuraPhase(bool apply);
	void SpellAuraModIgnoreArmorPct(bool apply);
	void SpellAuraModBaseHealth(bool apply);
	void SpellAuraModAttackPowerOfArmor(bool apply);
	void SpellAuraReflectSpellsInfront(bool apply);
	void SpellAuraCallStabledPet(bool apply);
	void UpdateAuraModDecreaseSpeed();

	void SendModifierLog(int32 ** m,int32 v,uint32* mask,uint8 type,bool pct = false);
	void SendDummyModifierLog(std::map<SpellEntry*,uint32> * m,SpellEntry * spellInfo,uint32 i,bool apply,bool pct = false);

	// Events
	void EventPeriodicDamage(uint32);
	void EventPeriodicDamagePercent(uint32);
	void EventPeriodicHeal(uint32);
	void EventPeriodicTriggerSpell(SpellEntry* spellInfo, bool overridevalues, int32 overridevalue);
	void EventPeriodicTrigger(uint32 amount, uint32 type);
	void EventPeriodicEnergize(uint32,uint32);
	void EventPeriodicEnergizeVariable(uint32,uint32);
	void EventPeriodicHeal1(uint32);
	void EventPeriodicLeech(uint32);
	void EventPeriodicBurn(uint32,uint32);
	void EventPeriodicHealthFunnel(uint32);
	void EventPeriodicManaLeech(uint32);
	void EventPeriodicHealPct(float);
	void EventPeriodicManaPct(float);
	void EventPeriodicRegenManaStatPct(uint32 perc,uint32 stat);
	void EventPeriodicTriggerDummy();

	void RelocateEvents();
	int32 event_GetInstanceID();
	bool WasCastInDuel() { return m_castInDuel; }

	SpellEntry * m_spellProto;
	// This is only used for AA
	AreaAuraList targets;
	uint64 m_casterGuid;

	uint16 m_auraSlot;

	uint32 m_castedItemId;
	uint64 itemCasterGUID;
	// Area aura stuff -> never passive.
	bool m_areaAura;		
	uint8 m_visualSlot;
	// This represents the triggering spell id
	uint32 pSpellId; 

	// This stuff can be cached in spellproto.
	bool IsCombatStateAffecting();

	bool m_castInDuel;
	inline bool TargetWasImuneToMods()
	{
		return (m_modcount && ((( m_flags & MOD_0_RESISTED)+( m_flags & MOD_1_RESISTED)+( m_flags & MOD_2_RESISTED)) == m_modcount) );
	}

	int32 GetModAmount( uint32 i ) { if(i<3) return m_modList[i].m_amount; return 0; }
	int32 GetModAmountByMod() { return mod->m_amount; };
	uint32 GetAuraFlags() { return m_flags; }

private:
	uint32 GetCasterFaction() { return m_casterfaction; }
	void SetCasterFaction(uint32 faction){ m_casterfaction = faction; }

	ARCEMU_INLINE bool IsInrange(float x1,float y1, float z1, Object * o,float square_r)
	{
		float t;
		float r;
		t=x1-o->GetPositionX();
		r=t*t;
		t=y1-o->GetPositionY();
		r+=t*t;
		t=z1-o->GetPositionZ();
		r+=t*t;
		return ( r<=square_r);
	}

	Unit* m_target;
	Player * p_target;
	uint32 timeleft;
	// In Milliseconds
	int32 m_duration;
//	bool m_positive;
	signed char m_positive;

	uint32 m_modcount;
	Modifier m_modList[3];
	Modifier * mod;

	uint32	m_dynamicValue;
	uint32	m_flags;

protected:
	uint32 m_casterfaction;

	void SendInterrupted(uint8 result, Object * m_caster);
	void SendChannelUpdate(uint32 time, Object * m_caster);
	void SendTickImmune(Unit * target, Unit *caster);
	
public:
	bool m_temporary;	// Skip saving
	bool m_deleted;
	int16 m_interrupted;
	bool m_ignoreunapply; // \\\"special\\\" case, for unapply

	ARCEMU_INLINE bool IsInterrupted() { return ( m_interrupted >= 0 ); }
};

typedef void(Aura::*pSpellAura)(bool apply);

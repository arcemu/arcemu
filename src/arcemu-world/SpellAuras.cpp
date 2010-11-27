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

#include "StdAfx.h"

pSpellAura SpellAuraHandler[TOTAL_SPELL_AURAS]={
		&Aura::SpellAuraNULL,//SPELL_AURA_NONE = 0
		&Aura::SpellAuraBindSight,//SPELL_AURA_BIND_SIGHT = 1
		&Aura::SpellAuraModPossess,//SPELL_AURA_MOD_POSSESS = 2,
		&Aura::SpellAuraPeriodicDamage,//SPELL_AURA_PERIODIC_DAMAGE = 3,
		&Aura::SpellAuraDummy,//SPELL_AURA_DUMMY = 4,
		&Aura::SpellAuraModConfuse,//SPELL_AURA_MOD_CONFUSE = 5,
		&Aura::SpellAuraModCharm,//SPELL_AURA_MOD_CHARM = 6,
		&Aura::SpellAuraModFear,//SPELL_AURA_MOD_FEAR = 7,
		&Aura::SpellAuraPeriodicHeal,//SPELL_AURA_PERIODIC_HEAL = 8,
		&Aura::SpellAuraModAttackSpeed,//SPELL_AURA_MOD_ATTACKSPEED = 9,
		&Aura::SpellAuraModThreatGenerated,//SPELL_AURA_MOD_THREAT = 10,
		&Aura::SpellAuraModTaunt,//SPELL_AURA_MOD_TAUNT = 11,
		&Aura::SpellAuraModStun,//SPELL_AURA_MOD_STUN = 12,
		&Aura::SpellAuraModDamageDone,//SPELL_AURA_MOD_DAMAGE_DONE = 13,
		&Aura::SpellAuraModDamageTaken,//SPELL_AURA_MOD_DAMAGE_TAKEN = 14,
		&Aura::SpellAuraDamageShield,//SPELL_AURA_DAMAGE_SHIELD = 15,
		&Aura::SpellAuraModStealth,//SPELL_AURA_MOD_STEALTH = 16,
		&Aura::SpellAuraModDetect,//SPELL_AURA_MOD_DETECT = 17,
		&Aura::SpellAuraModInvisibility,//SPELL_AURA_MOD_INVISIBILITY = 18,
		&Aura::SpellAuraModInvisibilityDetection,//SPELL_AURA_MOD_INVISIBILITY_DETECTION = 19,
		&Aura::SpellAuraModTotalHealthRegenPct,// SPELL_AURA_MOD_TOTAL_HEALTH_REGEN_PCT = 20
		&Aura::SpellAuraModTotalManaRegenPct,// SPELL_AURA_MOD_TOTAL_MANA_REGEN_PCT = 21
		&Aura::SpellAuraModResistance,//SPELL_AURA_MOD_RESISTANCE = 22,
		&Aura::SpellAuraPeriodicTriggerSpell,//SPELL_AURA_PERIODIC_TRIGGER_SPELL = 23,
		&Aura::SpellAuraPeriodicEnergize,//SPELL_AURA_PERIODIC_ENERGIZE = 24,
		&Aura::SpellAuraModPacify,//SPELL_AURA_MOD_PACIFY = 25,
		&Aura::SpellAuraModRoot,//SPELL_AURA_MOD_ROOT = 26,
		&Aura::SpellAuraModSilence,//SPELL_AURA_MOD_SILENCE = 27,
		&Aura::SpellAuraReflectSpells,//SPELL_AURA_REFLECT_SPELLS = 28,
		&Aura::SpellAuraModStat,//SPELL_AURA_MOD_STAT = 29,
		&Aura::SpellAuraModSkill,//SPELL_AURA_MOD_SKILL = 30,
		&Aura::SpellAuraModIncreaseSpeed,//SPELL_AURA_MOD_INCREASE_SPEED = 31,
		&Aura::SpellAuraModIncreaseMountedSpeed,//SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED = 32,
		&Aura::SpellAuraModDecreaseSpeed,//SPELL_AURA_MOD_DECREASE_SPEED = 33,
		&Aura::SpellAuraModIncreaseHealth,//SPELL_AURA_MOD_INCREASE_HEALTH = 34,
		&Aura::SpellAuraModIncreaseEnergy,//SPELL_AURA_MOD_INCREASE_ENERGY = 35,
		&Aura::SpellAuraModShapeshift,//SPELL_AURA_MOD_SHAPESHIFT = 36,
		&Aura::SpellAuraModEffectImmunity,//SPELL_AURA_EFFECT_IMMUNITY = 37,
		&Aura::SpellAuraModStateImmunity,//SPELL_AURA_STATE_IMMUNITY = 38,
		&Aura::SpellAuraModSchoolImmunity,//SPELL_AURA_SCHOOL_IMMUNITY = 39,
		&Aura::SpellAuraModDmgImmunity,//SPELL_AURA_DAMAGE_IMMUNITY = 40,
		&Aura::SpellAuraModDispelImmunity,//SPELL_AURA_DISPEL_IMMUNITY = 41,
		&Aura::SpellAuraProcTriggerSpell,//SPELL_AURA_PROC_TRIGGER_SPELL = 42,
		&Aura::SpellAuraProcTriggerDamage,//SPELL_AURA_PROC_TRIGGER_DAMAGE = 43,
		&Aura::SpellAuraTrackCreatures,//SPELL_AURA_TRACK_CREATURES = 44,
		&Aura::SpellAuraTrackResources,//SPELL_AURA_TRACK_RESOURCES = 45,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_PARRY_SKILL = 46, obsolete? not used in 1.12.1 spell.dbc
		&Aura::SpellAuraModParryPerc,//SPELL_AURA_MOD_PARRY_PERCENT = 47,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_DODGE_SKILL = 48, obsolete?
		&Aura::SpellAuraModDodgePerc,//SPELL_AURA_MOD_DODGE_PERCENT = 49,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_BLOCK_SKILL = 50, obsolete?,
		&Aura::SpellAuraModBlockPerc,//SPELL_AURA_MOD_BLOCK_PERCENT = 51,
		&Aura::SpellAuraModCritPerc,//SPELL_AURA_MOD_CRIT_PERCENT = 52,
		&Aura::SpellAuraPeriodicLeech,//SPELL_AURA_PERIODIC_LEECH = 53,
		&Aura::SpellAuraModHitChance,//SPELL_AURA_MOD_HIT_CHANCE = 54,
		&Aura::SpellAuraModSpellHitChance,//SPELL_AURA_MOD_SPELL_HIT_CHANCE = 55,
		&Aura::SpellAuraTransform,//SPELL_AURA_TRANSFORM = 56,
		&Aura::SpellAuraModSpellCritChance,//SPELL_AURA_MOD_SPELL_CRIT_CHANCE = 57,
		&Aura::SpellAuraIncreaseSwimSpeed,//SPELL_AURA_MOD_INCREASE_SWIM_SPEED = 58,
		&Aura::SpellAuraModCratureDmgDone,//SPELL_AURA_MOD_DAMAGE_DONE_CREATURE = 59,
		&Aura::SpellAuraPacifySilence,//SPELL_AURA_MOD_PACIFY_SILENCE = 60,
		&Aura::SpellAuraModScale,//SPELL_AURA_MOD_SCALE = 61,
		&Aura::SpellAuraPeriodicHealthFunnel,//SPELL_AURA_PERIODIC_HEALTH_FUNNEL = 62,
		&Aura::SpellAuraNULL,//SPELL_AURA_PERIODIC_MANA_FUNNEL = 63,//obsolete?
		&Aura::SpellAuraPeriodicManaLeech,//SPELL_AURA_PERIODIC_MANA_LEECH = 64,
		&Aura::SpellAuraModCastingSpeed,//SPELL_AURA_MOD_CASTING_SPEED = 65,
		&Aura::SpellAuraFeignDeath,//SPELL_AURA_FEIGN_DEATH = 66,
		&Aura::SpellAuraModDisarm,//SPELL_AURA_MOD_DISARM = 67,
		&Aura::SpellAuraModStalked,//SPELL_AURA_MOD_STALKED = 68,
		&Aura::SpellAuraSchoolAbsorb,//SPELL_AURA_SCHOOL_ABSORB = 69,
		&Aura::SpellAuraNULL,//SPELL_AURA_EXTRA_ATTACKS = 70,//obsolete?
		&Aura::SpellAuraModSpellCritChanceSchool,//SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL = 71,
		&Aura::SpellAuraModPowerCost,//SPELL_AURA_MOD_POWER_COST = 72,
		&Aura::SpellAuraModPowerCostSchool,//SPELL_AURA_MOD_POWER_COST_SCHOOL = 73,
		&Aura::SpellAuraReflectSpellsSchool,//SPELL_AURA_REFLECT_SPELLS_SCHOOL = 74,
		&Aura::SpellAuraModLanguage,//SPELL_AURA_MOD_LANGUAGE = 75,
		&Aura::SpellAuraAddFarSight,//SPELL_AURA_FAR_SIGHT = 76,
		&Aura::SpellAuraMechanicImmunity,//SPELL_AURA_MECHANIC_IMMUNITY = 77,
		&Aura::SpellAuraMounted,//SPELL_AURA_MOUNTED = 78,
		&Aura::SpellAuraModDamagePercDone,//SPELL_AURA_MOD_DAMAGE_PERCENT_DONE = 79,
		&Aura::SpellAuraModPercStat,//SPELL_AURA_MOD_PERCENT_STAT = 80,
		&Aura::SpellAuraSplitDamage,//SPELL_AURA_SPLIT_DAMAGE = 81,
		&Aura::SpellAuraWaterBreathing,//SPELL_AURA_WATER_BREATHING = 82,
		&Aura::SpellAuraModBaseResistance,//SPELL_AURA_MOD_BASE_RESISTANCE = 83,
		&Aura::SpellAuraModRegen,//SPELL_AURA_MOD_REGEN = 84,
		&Aura::SpellAuraModPowerRegen,//SPELL_AURA_MOD_POWER_REGEN = 85,
		&Aura::SpellAuraChannelDeathItem,//SPELL_AURA_CHANNEL_DEATH_ITEM = 86,
		&Aura::SpellAuraModDamagePercTaken,//SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN = 87,
		&Aura::SpellAuraModRegenPercent,//SPELL_AURA_MOD_PERCENT_REGEN = 88,
		&Aura::SpellAuraPeriodicDamagePercent,//SPELL_AURA_PERIODIC_DAMAGE_PERCENT = 89,
		&Aura::SpellAuraModResistChance,//SPELL_AURA_MOD_RESIST_CHANCE = 90,
		&Aura::SpellAuraModDetectRange,//SPELL_AURA_MOD_DETECT_RANGE = 91,
		&Aura::SpellAuraPreventsFleeing,//SPELL_AURA_PREVENTS_FLEEING = 92,
		&Aura::SpellAuraModUnattackable,//SPELL_AURA_MOD_UNATTACKABLE = 93,
		&Aura::SpellAuraInterruptRegen,//SPELL_AURA_INTERRUPT_REGEN = 94,
		&Aura::SpellAuraGhost,//SPELL_AURA_GHOST = 95,
		&Aura::SpellAuraMagnet,//SPELL_AURA_SPELL_MAGNET = 96,
		&Aura::SpellAuraManaShield,//SPELL_AURA_MANA_SHIELD = 97,
		&Aura::SpellAuraSkillTalent,//SPELL_AURA_MOD_SKILL_TALENT = 98,
		&Aura::SpellAuraModAttackPower,//SPELL_AURA_MOD_ATTACK_POWER = 99,
		&Aura::SpellAuraVisible,//SPELL_AURA_AURAS_VISIBLE = 100,
		&Aura::SpellAuraModResistancePCT,//SPELL_AURA_MOD_RESISTANCE_PCT = 101,
		&Aura::SpellAuraModCreatureAttackPower,//SPELL_AURA_MOD_CREATURE_ATTACK_POWER = 102,
		&Aura::SpellAuraModTotalThreat,//SPELL_AURA_MOD_TOTAL_THREAT = 103,
		&Aura::SpellAuraWaterWalk,//SPELL_AURA_WATER_WALK = 104,
		&Aura::SpellAuraFeatherFall,//SPELL_AURA_FEATHER_FALL = 105,
		&Aura::SpellAuraHover,//SPELL_AURA_HOVER = 106,
		&Aura::SpellAuraAddFlatModifier,//SPELL_AURA_ADD_FLAT_MODIFIER = 107,
		&Aura::SpellAuraAddPctMod,//SPELL_AURA_ADD_PCT_MODIFIER = 108,
		&Aura::SpellAuraAddClassTargetTrigger,//SPELL_AURA_ADD_CLASS_TARGET_TRIGGER = 109,
		&Aura::SpellAuraModPowerRegPerc,//SPELL_AURA_MOD_POWER_REGEN_PERCENT = 110,
		&Aura::SpellAuraNULL,//SPELL_AURA_ADD_CASTER_HIT_TRIGGER = 111,
		&Aura::SpellAuraOverrideClassScripts,//SPELL_AURA_OVERRIDE_CLASS_SCRIPTS = 112,
		&Aura::SpellAuraModRangedDamageTaken,//SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN = 113,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT = 114, obadac na mangosie
		&Aura::SpellAuraModHealing,//SPELL_AURA_MOD_HEALING = 115,
		&Aura::SpellAuraIgnoreRegenInterrupt,//SPELL_AURA_IGNORE_REGEN_INTERRUPT = 116,
		&Aura::SpellAuraModMechanicResistance,//SPELL_AURA_MOD_MECHANIC_RESISTANCE = 117,
		&Aura::SpellAuraModHealingPCT,//SPELL_AURA_MOD_HEALING_PCT = 118,
		&Aura::SpellAuraNULL,//SPELL_AURA_SHARE_PET_TRACKING = 119,//obsolete
		&Aura::SpellAuraUntrackable,//SPELL_AURA_UNTRACKABLE = 120,
		&Aura::SpellAuraEmphaty,//SPELL_AURA_EMPATHY = 121,
		&Aura::SpellAuraModOffhandDamagePCT,//SPELL_AURA_MOD_OFFHAND_DAMAGE_PCT = 122,
		&Aura::SpellAuraModPenetration,//SPELL_AURA_MOD_POWER_COST_PCT = 123, --> armor penetration & spell penetration, NOT POWER COST!
		&Aura::SpellAuraModRangedAttackPower,//SPELL_AURA_MOD_RANGED_ATTACK_POWER = 124,
		&Aura::SpellAuraModMeleeDamageTaken,//SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN = 125,
		&Aura::SpellAuraModMeleeDamageTakenPct,//SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT = 126,
		&Aura::SpellAuraRAPAttackerBonus,//SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS = 127,
		&Aura::SpellAuraModPossessPet,//SPELL_AURA_MOD_POSSESS_PET = 128,
		&Aura::SpellAuraModIncreaseSpeedAlways,//SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS = 129,
		&Aura::SpellAuraModIncreaseMountedSpeed,//SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS = 130,
		&Aura::SpellAuraModCreatureRangedAttackPower,//SPELL_AURA_MOD_CREATURE_RANGED_ATTACK_POWER = 131,
		&Aura::SpellAuraModIncreaseEnergyPerc,//SPELL_AURA_MOD_INCREASE_ENERGY_PERCENT = 132,
		&Aura::SpellAuraModIncreaseHealthPerc,//SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT = 133,
		&Aura::SpellAuraModManaRegInterrupt,//SPELL_AURA_MOD_MANA_REGEN_INTERRUPT = 134,
		&Aura::SpellAuraModHealingDone,//SPELL_AURA_MOD_HEALING_DONE = 135,
		&Aura::SpellAuraModHealingDonePct,//SPELL_AURA_MOD_HEALING_DONE_PERCENT = 136,
		&Aura::SpellAuraModTotalStatPerc,//SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE = 137,
		&Aura::SpellAuraModHaste,//SPELL_AURA_MOD_HASTE = 138,
		&Aura::SpellAuraForceReaction,//SPELL_AURA_FORCE_REACTION = 139,
		&Aura::SpellAuraModRangedHaste,//SPELL_AURA_MOD_RANGED_HASTE = 140,
		&Aura::SpellAuraModRangedAmmoHaste,//SPELL_AURA_MOD_RANGED_AMMO_HASTE = 141,
		&Aura::SpellAuraModBaseResistancePerc,//SPELL_AURA_MOD_BASE_RESISTANCE_PCT = 142,
		&Aura::SpellAuraModResistanceExclusive,//SPELL_AURA_MOD_RESISTANCE_EXCLUSIVE = 143,
		&Aura::SpellAuraSafeFall,//SPELL_AURA_SAFE_FALL = 144,
		&Aura::SpellAuraNULL,//SPELL_AURA_CHARISMA = 145,//obsolete?
		&Aura::SpellAuraNULL,//SPELL_AURA_PERSUADED = 146,//obsolete
		&Aura::SpellAuraNULL,//SPELL_AURA_ADD_CREATURE_IMMUNITY = 147,//http://wow.allakhazam.com/db/spell.html?wspell=36798
		&Aura::SpellAuraRetainComboPoints,//SPELL_AURA_RETAIN_COMBO_POINTS = 148,
		&Aura::SpellAuraResistPushback,//SPELL_AURA_RESIST_PUSHBACK = 149,//	Resist Pushback //Simply resist spell casting delay
		&Aura::SpellAuraModShieldBlockPCT,//SPELL_AURA_MOD_SHIELD_BLOCK_PCT = 150,//	Mod Shield Absorbed dmg %
		&Aura::SpellAuraTrackStealthed,//SPELL_AURA_TRACK_STEALTHED = 151,//	Track Stealthed
		&Aura::SpellAuraModDetectedRange,//SPELL_AURA_MOD_DETECTED_RANGE = 152,//	Mod Detected Range
		&Aura::SpellAuraSplitDamageFlat,//SPELL_AURA_SPLIT_DAMAGE_FLAT= 153,//	Split Damage Flat
		&Aura::SpellAuraModStealthLevel,//SPELL_AURA_MOD_STEALTH_LEVEL = 154,//	Stealth Level Modifier
		&Aura::SpellAuraModUnderwaterBreathing,//SPELL_AURA_MOD_WATER_BREATHING = 155,//	Mod Water Breathing
		&Aura::SpellAuraModReputationAdjust,//SPELL_AURA_MOD_REPUTATION_ADJUST = 156,//	Mod Reputation Gain
		&Aura::SpellAuraNULL,//SPELL_AURA_PET_DAMAGE_MULTI = 157,//	Mod Pet Damage
		&Aura::SpellAuraModBlockValue,//SPELL_AURA_MOD_SHIELD_BLOCKVALUE = 158//used Apply Aura: Mod Shield Block //http://www.thottbot.com/?sp=25036
		&Aura::SpellAuraNoPVPCredit,//SPELL_AURA_NO_PVP_CREDIT = 159 //used Apply Aura: No PVP Credit http://www.thottbot.com/?sp=2479
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_AOE_AVOIDANCE = 160 //Apply Aura: Mod Side/Rear PBAE Damage Taken %//used http://www.thottbot.com/?sp=23198
		&Aura::SpellAuraModHealthRegInCombat,//SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT = 161,
		&Aura::SpellAuraPowerBurn,//missing = 162 //used //Apply Aura: Power Burn (Mana) //http://www.thottbot.com/?sp=19659
		&Aura::SpellAuraModCritDmgPhysical,//missing = 163 //Apply Aura: Mod Crit Damage Bonus (Physical)
		&Aura::SpellAuraNULL,//missing = 164 //used //test spell
		&Aura::SpellAuraAPAttackerBonus,//SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS = 165,	// Melee AP Attacker Bonus
		&Aura::SpellAuraModPAttackPower,//missing = 166 //used //Apply Aura: Mod Attack Power % // http://www.thottbot.com/?sp=30803
		&Aura::SpellAuraModRangedAttackPowerPct,//missing = 167 //http://www.thottbot.com/s34485
		&Aura::SpellAuraIncreaseDamageTypePCT,//missing = 168 //used //Apply Aura: Increase Damage % *type* //http://www.thottbot.com/?sp=24991
		&Aura::SpellAuraIncreaseCricticalTypePCT,//missing = 169 //used //Apply Aura: Increase Critical % *type* //http://www.thottbot.com/?sp=24293
		&Aura::SpellAuraNULL,//SPELL_AURA_DETECT_AMORE = 170 //used //Apply Aura: Detect Amore //http://www.thottbot.com/?sp=26802
		&Aura::SpellAuraIncreasePartySpeed,//SPELL_AURA_MOD_SPEED_NOT_STACK = 171
		&Aura::SpellAuraIncreaseMovementAndMountedSpeed,//SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK = 172 //used //Apply Aura: Increase Movement and Mounted Speed (Non-Stacking) //http://www.thottbot.com/?sp=26022 2e effect
		&Aura::SpellAuraNULL,//SPELL_AURA_ALLOW_CHAMPION_SPELLS = 173 // Apply Aura: Allow Champion Spells
		&Aura::SpellAuraIncreaseSpellDamageByAttribute,//SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT = 174 //used //Apply Aura: Increase Spell Damage by % Spirit (Spells) //http://www.thottbot.com/?sp=15031
		&Aura::SpellAuraIncreaseHealingByAttribute,//SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT = 175 //used //Apply Aura: Increase Spell Healing by % Spirit //http://www.thottbot.com/?sp=15031
		&Aura::SpellAuraSpiritOfRedemption,//SPELL_AURA_SPIRIT_OF_REDEMPTION = 176 //used // Apply Aura: Spirit of Redemption
		&Aura::SpellAuraNULL,//SPELL_AURA_AOE_CHARM = 177 //used //Apply Aura: Area Charm // http://www.thottbot.com/?sp=26740
		&Aura::SpellAuraNULL,//missing = 178 //Apply Aura: Increase Debuff Resistance mangos
		&Aura::SpellAuraIncreaseAttackerSpellCrit,//SPELL_AURA_INCREASE_ATTACKER_SPELL_CRIT//Apply Aura: Increase Attacker Spell Crit % *type* //http://www.thottbot.com/?sp=12579
		&Aura::SpellAuraNULL,//missing = 180 //used //Apply Aura: Increase Spell Damage *type* //http://www.thottbot.com/?sp=29113 mangos
		&Aura::SpellAuraNULL,//missing = 181
		&Aura::SpellAuraIncreaseArmorByPctInt,//missing = 182 //used //Apply Aura: Increase Armor by % of Intellect //http://www.thottbot.com/?sp=28574  SPELL_AURA_INC_ARMOR_BY_PCT_INT
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_CRITICAL_THREAT = 183 //used //Apply Aura: Decrease Critical Threat by % (Spells) //http://www.thottbot.com/?sp=28746
		&Aura::SpellAuraReduceAttackerMHitChance,//184//Apply Aura: Reduces Attacker Chance to Hit with Melee //http://www.thottbot.com/s31678
		&Aura::SpellAuraReduceAttackerRHitChance,//185//Apply Aura: Reduces Attacker Chance to Hit with Ranged //http://www.thottbot.com/?sp=30895
		&Aura::SpellAuraReduceAttackerSHitChance,//186//Apply Aura: Reduces Attacker Chance to Hit with Spells (Spells) //http://www.thottbot.com/?sp=30895
		&Aura::SpellAuraReduceEnemyMCritChance,//missing = 187 //used //Apply Aura: Reduces Attacker Chance to Crit with Melee (Ranged?) //http://www.thottbot.com/?sp=30893
		&Aura::SpellAuraReduceEnemyRCritChance,//missing = 188 //used //Apply Aura: Reduces Attacker Chance to Crit with Ranged (Melee?) //http://www.thottbot.com/?sp=30893
		&Aura::SpellAuraIncreaseRating,//missing = 189 //Apply Aura: Increases Rating
		&Aura::SpellAuraIncreaseRepGainPct,//SPELL_AURA_MOD_FACTION_REPUTATION_GAIN //used // Apply Aura: Increases Reputation Gained by % //http://www.thottbot.com/?sp=30754
		&Aura::SpellAuraLimitSpeed,//missing = 191 //used // noname //http://www.thottbot.com/?sp=29894
		&Aura::SpellAuraMeleeHaste,//192 Apply Aura: Melee Slow %
		&Aura::SpellAuraIncreaseTimeBetweenAttacksPCT,//193 Apply Aura: Increase Time Between Attacks (Melee, Ranged and Spell) by %
		&Aura::SpellAuraNULL,//194 //&Aura::SpellAuraIncreaseSpellDamageByInt,//194 Apply Aura: Increase Spell Damage by % of Intellect (All)
		&Aura::SpellAuraNULL,//195 //&Aura::SpellAuraIncreaseHealingByInt,//195 Apply Aura: Increase Healing by % of Intellect
		&Aura::SpellAuraNULL,//196 Apply Aura: Mod All Weapon Skills (6)
		&Aura::SpellAuraModAttackerCritChance,//197 Apply Aura: Reduce Attacker Critical Hit Chance by %
		&Aura::SpellAuraIncreaseAllWeaponSkill,//198
		&Aura::SpellAuraIncreaseHitRate,//199 Apply Aura: Increases Spell % To Hit (Fire, Nature, Frost)
		&Aura::SpellAuraNULL,//200 // Increases experience earned by $s1%.  Lasts $d.
		&Aura::SpellAuraAllowFlight,//SPELL_AURA_FLY 201 isn't it same like 206 and 207?
		&Aura::SpellAuraFinishingMovesCannotBeDodged,//202 // Finishing moves cannot be dodged - 32601, 44452
		&Aura::SpellAuraReduceCritMeleeAttackDmg,//203 Apply Aura: Reduces Attacker Critical Hit Damage with Melee by %
		&Aura::SpellAuraReduceCritRangedAttackDmg,//204 Apply Aura: Reduces Attacker Critical Hit Damage with Ranged by %
		&Aura::SpellAuraNULL,//205 // "School" Vulnerability
		&Aura::SpellAuraEnableFlight,//206 // Take flight on a worn old carpet. - Spell 43343
		&Aura::SpellAuraEnableFlight,//207 set fly mod flight speed?
		&Aura::SpellAuraEnableFlightWithUnmountedSpeed,//208 mod flight speed?
		&Aura::SpellAuraNULL,//209  // mod flight speed?
		&Aura::SpellAuraNULL,//210	// commentator's command - spell 42009
		&Aura::SpellAuraIncreaseFlightSpeed,//211
		&Aura::SpellAuraIncreaseRAPbyStatPct,//SPELL_AURA_MOD_RANGED_ATTACK_POWER_BY_STAT_PCT //212 Apply Aura: Increase Ranged Atk Power by % of stat
		&Aura::SpellAuraIncreaseRageFromDamageDealtPCT, //213 Apply Aura: Increase Rage from Damage Dealt by %
		&Aura::SpellAuraNULL,//214 // Tamed Pet Passive (DND)
		&Aura::SpellAuraRemoveReagentCost,//215 // arena preparation buff - cancel soul shard requirement?
		&Aura::SpellAuraModCastingSpeed,//216 Increases casting time %, reuse existing handler...
		&Aura::SpellAuraNULL,//217 // not used
		&Aura::SpellAuraNULL,//218 // increases time between ranged attacks
		&Aura::SpellAuraRegenManaStatPCT,//219 Regenerate mana equal to $s1% of your Intellect every 5 sec, even while casting
		&Aura::SpellAuraSpellHealingStatPCT,//220 Increases your healing spells  by up to $s1% of your Strength // increases your critical strike rating by 35% of your spirit // Molten Armor only?
		&Aura::SpellAuraNULL,//221 Detaunt "Ignores an enemy, forcing the caster to not attack it unless there is no other target nearby. When the effect wears off, the creature will attack the most threatening target."
		&Aura::SpellAuraNULL,//222 // not used
		&Aura::SpellAuraNULL,//223 // used in one spell, cold stare 43593
		&Aura::SpellAuraNULL,//224 // not used
		&Aura::SpellAuraNULL,//225 // Prayer of Mending "Places a spell on the target that heals them for $s1 the next time they take damage.  When the heal occurs, Prayer of Mending jumps to a raid member within $a1 yards.  Jumps up to $n times and lasts $d after each jump.  This spell can only be placed on one target at a time."
		&Aura::SpellAuraPeriodicTriggerDummy,//226 // used in brewfest spells, headless horseman, Aspect of the Viper
		&Aura::SpellAuraPeriodicTriggerSpellWithValue,//227 // Used by Mind Flay, Siege Turrets 'Machine gun' and a few other spells.
		&Aura::SpellAuraModStealthDetection,//228 Stealth Detection. http://www.thottbot.com/s34709
		&Aura::SpellAuraReduceAOEDamageTaken,//229 Apply Aura:Reduces the damage your pet takes from area of effect attacks http://www.thottbot.com/s35694
		&Aura::SpellAuraIncreaseMaxHealth,//230 Increase Max Health (commanding shout);
        &Aura::SpellAuraNULL,//231 curse a target http://www.thottbot.com/s40303
        &Aura::SpellAuraReduceEffectDuration,//232 // Reduces duration of Magic effects by $s2%. SPELL_AURA_MECHANIC_DURATION_MOD
        &Aura::SpellAuraNULL,//233 // Beer Goggles
        &Aura::SpellAuraReduceEffectDuration,//234 Apply Aura: Reduces Silence or Interrupt effects, Item spell magic http://www.thottbot.com/s42184
		&Aura::SpellAuraNULL,//235 33206 Instantly reduces a friendly target's threat by $44416s1%, reduces all damage taken by $s1% and increases resistance to Dispel mechanics by $s2% for $d.
		&Aura::HandleAuraControlVehicle,
		&Aura::SpellAuraModHealingByAP,//237	//increase spell healing by X pct from attack power
		&Aura::SpellAuraModSpellDamageByAP,//238	//increase spell dmg by X pct from attack power
		&Aura::SpellAuraModScale,//239
		&Aura::SpellAuraExpertise,// SPELL_AURA_EXPERTISE = 240 
		&Aura::SpellAuraNULL,//241
		&Aura::SpellAuraNULL,//242
		&Aura::SpellAuraNULL,//243
		&Aura::SpellAuraNULL,//244
		&Aura::SpellAuraNULL,//245
		&Aura::SpellAuraNULL,//246
		&Aura::SpellAuraNULL,//247
		&Aura::SpellAuraModCombatResultChance,//248 SPELL_AURA_MOD_COMBAT_RESULT_CHANCE
		&Aura::SpellAuraNULL,//249 - SpellAuraConvertRune
		&Aura::SpellAuraAddHealth,//250
		&Aura::SpellAuraNULL,//251  Mod Enemy Dodge
		&Aura::SpellAuraNULL,//252 Reduces the target's ranged, melee attack, and casting speed by X pct for Y sec.
		&Aura::SpellAuraBlockMultipleDamage,//253 SPELL_AURA_BLOCK_MULTIPLE_DAMAGE
		&Aura::SpellAuraNULL,//254
		&Aura::SpellAuraModMechanicDmgTakenPct, //255 SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT
		&Aura::SpellAuraRemoveReagentCost,//256 Remove reagent cost
		&Aura::SpellAuraNULL,//257 Mod Target Resist By Spell Class ( does damage in the form of X damage, ignoring all resistances, absorption, and immunity mechanics. - http://thottbot.com/s47271 )
		&Aura::SpellAuraNULL,//258 Mod Spell Visual
		&Aura::SpellAuraNULL,//259 Mod Periodic Damage Taken Pct - Periodic Shadow damage taken increased by 3% ( http://thottbot.com/s60448 )
		&Aura::SpellAuraNULL,//260 Screen Effect
		&Aura::SpellAuraPhase,//261
		&Aura::SpellAuraIgnoreTargetAuraState, //262 SPELL_AURA_IGNORE_TARGET_AURA_STATE
		&Aura::SpellAuraAllowOnlyAbility,//263 SPELL_AURA_ALLOW_ONLY_ABILITY
		&Aura::SpellAuraNULL,//264
		&Aura::SpellAuraNULL,//265
		&Aura::SpellAuraNULL,//266
		&Aura::SpellAuraNULL,//267 Prevent the application of harmful magical effects
		&Aura::SpellAuraIncreaseAPbyStatPct, //268 Increase attack power by % of stat
		&Aura::SpellAuraNULL,//269 Damage reduction effects ignored. (?) - http://thottbot.com/s57318
		&Aura::SpellAuraNULL,//270 Ignore target resist
		&Aura::SpellAuraModSpellDamageDOTPct,//271
		&Aura::SpellAuraNULL,//272 Unknown
		&Aura::SpellAuraNULL,//273 Some sort of dummy aura? (http://thottbot.com/s54844 + http://thottbot.com/s26659)
		&Aura::SpellAuraConsumeNoAmmo,//274 Consumes no ammo
		&Aura::SpellAuraIgnoreShapeshift,//275 Ignore unit states
		&Aura::SpellAuraNULL,//276 Mod Damage % Mechanic
		&Aura::SpellAuraNULL,//277 SPELL_AURA_REDIRECT_THREAT or SPELL_AURA_MOD_MAX_AFFECTED_TARGETS ?
		&Aura::SpellAuraNULL,//278 Mod Disarm Ranged
		&Aura::SpellAuraNULL,//279 Modify models(?)
		&Aura::SpellAuraModIgnoreArmorPct,//280 SPELL_AURA_IGNORE_ARMOR_PCT
		&Aura::SpellAuraNULL,//281 Mod Honor gain increased by X pct. Final Reward Honor increased by X pct for Y Rank and above. (http://thottbot.com/s58560 && http://thottbot.com/s58557)
		&Aura::SpellAuraModBaseHealth,//282 SPELL_AURA_MOD_BASE_HEALTH
		&Aura::SpellAuraModHealingPCT,//283 Increases all healing received by X pct
		&Aura::SpellAuraNULL,//284 not used by any spells (3.08a)
		&Aura::SpellAuraModAttackPowerOfArmor,//285 SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR
		&Aura::SpellAuraNULL,//286 SPELL_AURA_ALLOW_DOT_TO_CRIT
		&Aura::SpellAuraReflectSpellsInfront,//287 SPELL_AURA_DEFLECT_SPELLS
		&Aura::SpellAuraNULL,//288 not used by any spells (3.09) except 1 test spell.
		&Aura::SpellAuraNULL,//289 unused
		&Aura::SpellAuraNULL,//290 unused
		&Aura::SpellAuraNULL,//291 unused
		&Aura::SpellAuraCallStabledPet,//292 call stabled pet
		&Aura::SpellAuraNULL,//293 2 test spells
		&Aura::SpellAuraNULL,//294 2 spells, possible prevent mana regen
		&Aura::SpellAuraNULL,//295
		&Aura::SpellAuraNULL,//296
		&Aura::SpellAuraNULL,//297
		&Aura::SpellAuraNULL,//298
		&Aura::SpellAuraNULL,//299
		&Aura::SpellAuraNULL,//300
		&Aura::SpellAuraNULL,//301
		&Aura::SpellAuraNULL,//302
		&Aura::SpellAuraNULL,//303
		&Aura::SpellAuraNULL,//304
		&Aura::SpellAuraNULL,//305
		&Aura::SpellAuraNULL,//306
        &Aura::SpellAuraNULL,//307
        &Aura::SpellAuraNULL,//308
        &Aura::SpellAuraNULL,//309
        &Aura::SpellAuraNULL,//310
        &Aura::SpellAuraNULL,//311
        &Aura::SpellAuraNULL,//312
        &Aura::SpellAuraNULL,//313
        &Aura::SpellAuraNULL,//314
        &Aura::SpellAuraNULL,//315
        &Aura::SpellAuraNULL //316

};

const char* SpellAuraNames[TOTAL_SPELL_AURAS] = {
    "NONE",												//   0 None
    "BIND_SIGHT",										//   1 Bind Sight
    "MOD_POSSESS",										//   2 Mod Possess
    "PERIODIC_DAMAGE",									//   3 Periodic Damage
    "DUMMY",											//   4 Script Aura
    "MOD_CONFUSE",										//   5 Mod Confuse
    "MOD_CHARM",										//   6 Mod Charm
    "MOD_FEAR",											//   7 Mod Fear
    "PERIODIC_HEAL",									//   8 Periodic Heal
    "MOD_ATTACKSPEED",									//   9 Mod Attack Speed
    "MOD_THREAT",										//  10 Mod Threat
    "MOD_TAUNT",										//  11 Taunt
    "MOD_STUN",											//  12 Stun
    "MOD_DAMAGE_DONE",									//  13 Mod Damage Done
    "MOD_DAMAGE_TAKEN",									//  14 Mod Damage Taken
    "DAMAGE_SHIELD",									//  15 Damage Shield
    "MOD_STEALTH",										//  16 Mod Stealth
    "MOD_DETECT",										//  17 Mod Detect
    "MOD_INVISIBILITY",									//  18 Mod Invisibility
    "MOD_INVISIBILITY_DETECTION",						//  19 Mod Invisibility Detection
    "MOD_TOTAL_HEALTH_REGEN_PCT",						//  20
    "MOD_TOTAL_MANA_REGEN_PCT",							//  21
    "MOD_RESISTANCE",									//  22 Mod Resistance
    "PERIODIC_TRIGGER_SPELL",							//  23 Periodic Trigger
    "PERIODIC_ENERGIZE",								//  24 Periodic Energize
    "MOD_PACIFY",										//  25 Pacify
    "MOD_ROOT",											//  26 Root
    "MOD_SILENCE",										//  27 Silence
    "REFLECT_SPELLS",									//  28 Reflect Spells %
    "MOD_STAT",											//  29 Mod Stat
    "MOD_SKILL",										//  30 Mod Skill
    "MOD_INCREASE_SPEED",								//  31 Mod Speed
    "MOD_INCREASE_MOUNTED_SPEED",						//  32 Mod Speed Mounted
    "MOD_DECREASE_SPEED",								//  33 Mod Speed Slow
    "MOD_INCREASE_HEALTH",								//  34 Mod Increase Health
    "MOD_INCREASE_ENERGY",								//  35 Mod Increase Energy
    "MOD_SHAPESHIFT",									//  36 Shapeshift
    "EFFECT_IMMUNITY",									//  37 Immune Effect
    "STATE_IMMUNITY",									//  38 Immune State
    "SCHOOL_IMMUNITY",									//  39 Immune School
    "DAMAGE_IMMUNITY",									//  40 Immune Damage
    "DISPEL_IMMUNITY",									//  41 Immune Dispel Type
    "PROC_TRIGGER_SPELL",								//  42 Proc Trigger Spell
    "PROC_TRIGGER_DAMAGE",								//  43 Proc Trigger Damage
    "TRACK_CREATURES",									//  44 Track Creatures
    "TRACK_RESOURCES",									//  45 Track Resources
    "MOD_PARRY_SKILL",									//  46 Mod Parry Skill
    "MOD_PARRY_PERCENT",								//  47 Mod Parry Percent
    "MOD_DODGE_SKILL",									//  48 Mod Dodge Skill
    "MOD_DODGE_PERCENT",								//  49 Mod Dodge Percent
    "MOD_BLOCK_SKILL",									//  50 Mod Block Skill
    "MOD_BLOCK_PERCENT",								//  51 Mod Block Percent
    "MOD_CRIT_PERCENT",									//  52 Mod Crit Percent
    "PERIODIC_LEECH",									//  53 Periodic Leech
    "MOD_HIT_CHANCE",									//  54 Mod Hit Chance
    "MOD_SPELL_HIT_CHANCE",								//  55 Mod Spell Hit Chance
    "TRANSFORM",										//  56 Transform
    "MOD_SPELL_CRIT_CHANCE",							//  57 Mod Spell Crit Chance
    "MOD_INCREASE_SWIM_SPEED",							//  58 Mod Speed Swim
    "MOD_DAMAGE_DONE_CREATURE",							//  59 Mod Creature Dmg Done
    "MOD_PACIFY_SILENCE",								//  60 Pacify & Silence
    "MOD_SCALE",										//  61 Mod Scale
    "PERIODIC_HEALTH_FUNNEL",							//  62 Periodic Health Funnel
    "PERIODIC_MANA_FUNNEL",								//  63 Periodic Mana Funnel
    "PERIODIC_MANA_LEECH",								//  64 Periodic Mana Leech
    "MOD_CASTING_SPEED",								//  65 Haste - Spells
    "FEIGN_DEATH",										//  66 Feign Death
    "MOD_DISARM",										//  67 Disarm
    "MOD_STALKED",										//  68 Mod Stalked
    "SCHOOL_ABSORB",									//  69 School Absorb
    "EXTRA_ATTACKS",									//  70 Extra Attacks
    "MOD_SPELL_CRIT_CHANCE_SCHOOL",						//  71 Mod School Spell Crit Chance
    "MOD_POWER_COST",									//  72 Mod Power Cost
    "MOD_POWER_COST_SCHOOL",							//  73 Mod School Power Cost
    "REFLECT_SPELLS_SCHOOL",							//  74 Reflect School Spells %
    "MOD_LANGUAGE",										//  75 Mod Language
    "FAR_SIGHT",										//  76 Far Sight
    "MECHANIC_IMMUNITY",								//  77 Immune Mechanic
    "MOUNTED",											//  78 Mounted
    "MOD_DAMAGE_PERCENT_DONE",							//  79 Mod Dmg %
    "MOD_PERCENT_STAT",									//  80 Mod Stat %
    "SPLIT_DAMAGE",										//  81 Split Damage
    "WATER_BREATHING",									//  82 Water Breathing
    "MOD_BASE_RESISTANCE",								//  83 Mod Base Resistance
    "MOD_REGEN",										//  84 Mod Health Regen
    "MOD_POWER_REGEN",									//  85 Mod Power Regen
    "CHANNEL_DEATH_ITEM",								//  86 Create Death Item
    "MOD_DAMAGE_PERCENT_TAKEN",							//  87 Mod Dmg % Taken
    "MOD_PERCENT_REGEN",								//  88 Mod Health Regen Percent
    "PERIODIC_DAMAGE_PERCENT",							//  89 Periodic Damage Percent
    "MOD_RESIST_CHANCE",								//  90 Mod Resist Chance
    "MOD_DETECT_RANGE",									//  91 Mod Detect Range
    "PREVENTS_FLEEING",									//  92 Prevent Fleeing
    "MOD_UNATTACKABLE",									//  93 Mod Uninteractible
    "INTERRUPT_REGEN",									//  94 Interrupt Regen
    "GHOST",											//  95 Ghost
    "SPELL_MAGNET",										//  96 Spell Magnet
    "MANA_SHIELD",										//  97 Mana Shield
    "MOD_SKILL_TALENT",									//  98 Mod Skill Talent
    "MOD_ATTACK_POWER",									//  99 Mod Attack Power
    "AURAS_VISIBLE",									// 100 Auras Visible
    "MOD_RESISTANCE_PCT",								// 101 Mod Resistance %
    "MOD_CREATURE_ATTACK_POWER",						// 102 Mod Creature Attack Power
    "MOD_TOTAL_THREAT",									// 103 Mod Total Threat (Fade)
    "WATER_WALK",										// 104 Water Walk
    "FEATHER_FALL",										// 105 Feather Fall
    "HOVER",											// 106 Hover
    "ADD_FLAT_MODIFIER",								// 107 Add Flat Modifier
    "ADD_PCT_MODIFIER",									// 108 Add % Modifier
    "ADD_TARGET_TRIGGER",								// 109 Add Class Target Trigger
    "MOD_POWER_REGEN_PERCENT",							// 110 Mod Power Regen %
    "ADD_CASTER_HIT_TRIGGER",							// 111 Add Class Caster Hit Trigger
    "OVERRIDE_CLASS_SCRIPTS",							// 112 Override Class Scripts
    "MOD_RANGED_DAMAGE_TAKEN",							// 113 Mod Ranged Dmg Taken
    "MOD_RANGED_DAMAGE_TAKEN_PCT",						// 114 Mod Ranged % Dmg Taken
    "MOD_HEALING",										// 115 Mod Healing
    "IGNORE_REGEN_INTERRUPT",							// 116 Regen During Combat
    "MOD_MECHANIC_RESISTANCE",							// 117 Mod Mechanic Resistance
    "MOD_HEALING_PCT",									// 118 Mod Healing %
    "SHARE_PET_TRACKING",								// 119 Share Pet Tracking
    "UNTRACKABLE",										// 120 Untrackable
    "EMPATHY",											// 121 Empathy (Lore, whatever)
    "MOD_OFFHAND_DAMAGE_PCT",							// 122 Mod Offhand Dmg %
    "MOD_POWER_COST_PCT",								// 123 Mod Power Cost % --> armor penetration & spell penetration
    "MOD_RANGED_ATTACK_POWER",							// 124 Mod Ranged Attack Power
    "MOD_MELEE_DAMAGE_TAKEN",							// 125 Mod Melee Dmg Taken
    "MOD_MELEE_DAMAGE_TAKEN_PCT",						// 126 Mod Melee % Dmg Taken
    "RANGED_ATTACK_POWER_ATTACKER_BONUS",				// 127 Rngd Atk Pwr Attckr Bonus
    "MOD_POSSESS_PET",									// 128 Mod Possess Pet
    "MOD_INCREASE_SPEED_ALWAYS",						// 129 Mod Speed Always
    "MOD_MOUNTED_SPEED_ALWAYS",							// 130 Mod Mounted Speed Always
    "MOD_CREATURE_RANGED_ATTACK_POWER",					// 131 Mod Creature Ranged Attack Power
    "MOD_INCREASE_ENERGY_PERCENT",						// 132 Mod Increase Energy %
    "MOD_INCREASE_HEALTH_PERCENT",						// 133 Mod Max Health %
    "MOD_MANA_REGEN_INTERRUPT",							// 134 Mod Interrupted Mana Regen
    "MOD_HEALING_DONE",									// 135 Mod Healing Done
    "MOD_HEALING_DONE_PERCENT",							// 136 Mod Healing Done %
    "MOD_TOTAL_STAT_PERCENTAGE",						// 137 Mod Total Stat %
    "MOD_HASTE",										// 138 Haste - Melee
    "FORCE_REACTION",									// 139 Force Reaction
    "MOD_RANGED_HASTE",									// 140 Haste - Ranged
    "MOD_RANGED_AMMO_HASTE",							// 141 Haste - Ranged (Ammo Only)
    "MOD_BASE_RESISTANCE_PCT",							// 142 Mod Base Resistance %
    "MOD_RESISTANCE_EXCLUSIVE",							// 143 Mod Resistance Exclusive
    "SAFE_FALL",										// 144 Safe Fall
    "CHARISMA",											// 145 Charisma
    "PERSUADED",										// 146 Persuaded
    "ADD_CREATURE_IMMUNITY",							// 147 Add Creature Immunity
    "RETAIN_COMBO_POINTS",								// 148 Retain Combo Points
    "RESIST_PUSHBACK",									// 149 Resist Pushback
    "MOD_SHIELD_BLOCK_PCT",								// 150 Mod Shield Block %
    "TRACK_STEALTHED",									// 151 Track Stealthed
    "MOD_DETECTED_RANGE",								// 152 Mod Detected Range
    "SPLIT_DAMAGE_FLAT",								// 153 Split Damage Flat
    "MOD_STEALTH_LEVEL",								// 154 Stealth Level Modifier
    "MOD_WATER_BREATHING",								// 155 Mod Water Breathing
    "MOD_REPUTATION_ADJUST",							// 156 Mod Reputation Gain
    "PET_DAMAGE_MULTI",									// 157 Mod Pet Damage
    "MOD_SHIELD_BLOCK",									// 158 Mod Shield Block
    "NO_PVP_CREDIT",									// 159 No PVP Credit
    "MOD_SIDE_REAR_PDAE_DAMAGE_TAKEN",					// 160 Mod Side/Rear PBAE Damage Taken
    "MOD_HEALTH_REGEN_IN_COMBAT",						// 161 Mod Health Regen In Combat
    "POWER_BURN",										// 162 Power Burn
    "MOD_CRIT_DAMAGE_BONUS_MELEE",						// 163 Mod Critical Damage Bonus (Physical)
	"",													// 164
    "MELEE_ATTACK_POWER_ATTACKER_BONUS",				// 165 Melee AP Attacker Bonus
    "MOD_ATTACK_POWER_PCT",								// 166 Mod Attack Power
    "MOD_RANGED_ATTACK_POWER_PCT",						// 167 Mod Ranged Attack Power %
    "INCREASE_DAMAGE",									// 168 Increase Damage Type
    "INCREASE_CRITICAL",								// 169 Increase Critical Type
    "DETECT_AMORE",										// 170 Detect Amore
	"",													// 171
    "INCREASE_MOVEMENT_AND_MOUNTED_SPEED",				// 172 Increase Movement and Mounted Speed (Non-Stacking)
	"",													// 173
    "INCREASE_SPELL_DAMAGE_PCT",						// 174 Increase Spell Damage by % status
    "INCREASE_SPELL_HEALING_PCT",						// 175 Increase Spell Healing by % status
    "SPIRIT_OF_REDEMPTION",								// 176 Spirit of Redemption Auras
    "AREA_CHARM",										// 177 Area Charm
	"",													// 178
    "INCREASE_ATTACKER_SPELL_CRIT",						// 179 Increase Attacker Spell Crit Type
    "INCREASE_SPELL_DAMAGE_VS_TYPE",					// 180 Increase Spell Damage Type
	"",													// 181
    "INCREASE_ARMOR_BASED_ON_INTELLECT_PCT",			// 182 Increase Armor based on Intellect
    "DECREASE_CRIT_THREAT",								// 183 Decrease Critical Threat by
    "DECREASE_ATTACKER_CHANCE_TO_HIT_MELEE",			// 184 Reduces Attacker Chance to Hit with Melee
    "DECREASE_ATTACKER_CHANGE_TO_HIT_RANGED",			// 185 Reduces Attacker Chance to Hit with Ranged
    "DECREASE_ATTACKER_CHANGE_TO_HIT_SPELLS",			// 186 Reduces Attacker Chance to Hit with Spells
    "DECREASE_ATTACKER_CHANGE_TO_CRIT_MELEE",			// 187 Reduces Attacker Chance to Crit with Melee (Ranged?)
    "DECREASE_ATTACKER_CHANGE_TO_CRIT_RANGED",			// 188 Reduces Attacker Chance to Crit with Ranged (Melee?)
	"",													// 189
    "INCREASE_REPUTATION",								// 190 Increases reputation from killed creatures
    "SPEED_LIMIT",										// 191 speed limit
    "MELEE_SLOW_PCT",									// 192
    "INCREASE_TIME_BETWEEN_ATTACKS",					// 193
	"INCREASE_SPELL_DAMAGE_PCT_OF_INTELLECT",			// 194 NOT USED ANYMORE - 174 used instead
    "INCREASE_HEALING_PCT_OF_INTELLECT",				// 195 NOT USED ANYMORE - 175 used instead
    "MOD_ALL_WEAPON_SKILLS",							// 196
    "REDUCE_ATTACKER_CRICTICAL_HIT_CHANCE_PCT",			// 197
	"",													// 198
    "INCREASE_SPELL_HIT_PCT",							// 199
	"",													// 200
    "CANNOT_BE_DODGED",									// 201
	"FINISHING_MOVES_CANNOT_BE_DODGED",					// 202
    "REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_MELEE_PCT",	// 203
    "REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_RANGED_PCT",	// 204
	"",													// 205
	"",													// 206
	"",													// 207
	"",													// 208
	"",													// 209
	"",													// 210
	"",													// 211
    "INCREASE_RANGED_ATTACK_POWER_PCT_OF_INTELLECT",	// 212
    "INCREASE_RAGE_FROM_DAMAGE_DEALT_PCT",				// 213
	"",													// 214
	"",													// 215
    "INCREASE_CASTING_TIME_PCT",						// 216
	"",													// 217
	"",													// 218
    "REGEN_MANA_STAT_PCT",								// 219
    "HEALING_STAT_PCT",									// 220
	"",													// 221
	"",													// 222
	"",													// 223
	"",													// 224
	"",													// 225
	"PERIODIC_TRIGGER_DUMMY",							// 226
	"TRIGGER_SPELL_WITH_VALUE",							// 227 Used by Mind Flay and some other spells.
	"",													// 228
	"SPELL_AURA_REDUCE_AOE_DAMAGE_TAKEN",				// 229
    "INCREASE_MAX_HEALTH",								// 230 Used by Commanding Shout
	"",													// 231
	"SPELL_AURA_MECHANIC_DURATION_MOD",					// 232
	"",													// 233
	"",													// 234
	"",													// 235
	"CONTROL_VEHICLE",									// 236
    "SPELL_AURA_MOD_HEALING_FROM_AP",                   // 237
    "SPELL_AURA_MOD_SPELL_DAMAGE_FROM_AP",              // 238
	"",													// 239
	"EXPERTISE",										// 240
	"241",												// 241
    "MOD_SPELL_DAMAGE_FROM_HEALING",					// 242
    "243",												// 243
    "244",												// 244
    "MOD_DURATION_OF_MAGIC_EFFECTS",					// 245
    "246",												// 246
    "247",												// 247
    "MOD_COMBAT_RESULT_CHANCE",							// 248
    "249",												// 249
    "MOD_INCREASE_HEALTH_2",							// 250
    "MOD_ENEMY_DODGE",									// 251
	"",													// 252
	"SPELL_AURA_BLOCK_MULTIPLE_DAMAGE",					// 253
	"",													// 254
	"SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT",		// 255
	"",													// 256
	"",													// 257
	"",													// 258
	"",													// 259
	"",													// 260
	"SPELL_AURA_SET_PHASE",								// 261
	"SPELL_AURA_IGNORE_TARGET_AURA_STATE",				// 262
	"SPELL_AURA_ALLOW_ONLY_ABILITY",					// 263
	"",													// 264
	"",													// 265
	"",													// 266
	"",													// 267
	"MOD_SHAMMY_ATTACK_POWER",                          // 268 Mental Dexterity (increases ap by x% of intellect)
	"",													// 269
	"",													// 270
	"SPELL_AURA_INCREASE_SPELL_DOT_DAMAGE_PCT",			// 271
	"",													// 272
	"",													// 273
	"SPELL_AURA_CONSUMES_NO_AMMO",						// 274
	"",													// 275
	"",													// 276
	"",													// 277
	"",													// 278
	"",													// 279
	"SPELL_AURA_IGNORE_ARMOR_PCT",						// 280
	"",													// 281
	"SPELL_AURA_MOD_BASE_HEALTH",						// 282
	"",													// 283
	"",													// 284
	"SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR",				// 285
	"SPELL_AURA_ALLOW_CRIT_PERIODIC_DAMAGE",			// 286
	"SPELL_AURA_DEFLECT_SPELLS",						// 287
	"",													// 288
	"",													// 289
	"",													// 290
	"",													// 291
	"SPELL_AURA_CALL_STABLED_PET",						// 292
	"",													// 293
	"",													// 294
	"",													// 295
	"",													// 296
	"",													// 297
	"",													// 298
	"",													// 299
	"",													// 300
	"",													// 301
	"",													// 302
	"",													// 303
	"",													// 304
	"",													// 305
	"",													// 306
    "",													// 307
    "",													// 308
    "",													// 309
    "",													// 310
    "",													// 311
    "",													// 312
    "",													// 313
    "",													// 314
    "",													// 315
    ""													// 316
};


Player * Aura::GetPlayerCaster()
{
	//caster and target are the same
	if( m_casterGuid == m_target->GetGUID() )
	{
		if( m_target->IsPlayer() )
			return TO_PLAYER(m_target);
		else//caster is not a player
			return NULL;
	}

	if( m_target->GetMapMgr() )
		return m_target->GetMapMgr()->GetPlayer( Arcemu::Util::GUID_LOPART(m_casterGuid) );
	else
		return NULL;
}

Unit * Aura::GetUnitCaster()
{
	if( m_casterGuid == m_target->GetGUID() )
		return m_target;

	if( m_target->GetMapMgr() )
		return m_target->GetMapMgr()->GetUnit( m_casterGuid );
	else
		return NULL;
}

Object* Aura::GetCaster()
{
	if( m_casterGuid == m_target->GetGUID() )
		return m_target;
	if( m_target->GetMapMgr() )
		return m_target->GetMapMgr()->_GetObject( m_casterGuid );
	else
		return NULL;
}

Aura::Aura( SpellEntry* proto, int32 duration, Object* caster, Unit* target, bool temporary, Item* i_caster)
{
	m_castInDuel = false;
	m_spellProto = proto;
	m_duration = duration;
	m_positive = 0; //we suppose spell will have positive impact on target
	m_temporary = temporary; // Aura saving related
	m_deleted = false;
	m_ignoreunapply = false;
	m_casterGuid = caster->GetGUID();
	Arcemu::Util::ARCEMU_ASSERT( target != NULL );
	m_target = target;

	if( m_target->GetTypeId() == TYPEID_PLAYER )
		p_target = static_cast< Player* >( m_target );
	else
		p_target = NULL;

	if ( i_caster != NULL )
	{
		m_castedItemId = i_caster->GetProto()->ItemId;
		itemCasterGUID = i_caster->GetGUID();
	}
	else
	{
		m_castedItemId = 0;
		itemCasterGUID = 0;
	}

	// Modifies current aura duration based on its mechanic type
	if( p_target && GetDuration() > 0 )
	{
		int32 DurationModifier = p_target->MechanicDurationPctMod[ Spell::GetMechanic(proto) ];
		if( DurationModifier < - 100 )
			DurationModifier = -100; // Can't reduce by more than 100%
		SetDuration( ( GetDuration() * ( 100 + DurationModifier ) ) / 100 );
	}

	if (GetDuration() > 0 && m_spellProto->ChannelInterruptFlags != 0 && caster->IsUnit())
		SetDuration(GetDuration() * float2int32(TO_UNIT(caster)->GetCastSpeedMod()));

	//SetCasterFaction(caster->_getFaction());

	//m_auraSlot = 0;
	m_modcount = 0;
	m_dynamicValue = 0;
	m_areaAura = false;

	if( m_spellProto->c_is_flags & SPELL_FLAG_IS_FORCEDDEBUFF )
		SetNegative( 100 );
	else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_FORCEDBUFF )
		SetPositive( 100 );

	if( caster->IsUnit() )
	{
		if( m_spellProto->BGR_one_buff_from_caster_on_self != 0 )
			static_cast<Unit*>(caster)->RemoveAllAuraFromSelfType2( m_spellProto->BGR_one_buff_from_caster_on_self, m_spellProto->NameHash );

		if( isAttackable( caster, target ) )
		{
			SetNegative();
		}
		else
			SetPositive();

		if( p_target && caster->IsPlayer() )
		{
			if( p_target->DuelingWith == TO_PLAYER(caster) )
			{
				m_castInDuel = true;
			}
		}
	}

	if( !IsPassive() )
	{
		timeleft = ( uint32 )UNIXTIME;
	}

	m_visualSlot = 0xFF;
	pSpellId = 0;
	periodic_target = 0;
	//sLog.outDetail("Aura::Constructor %u (%s) from %u.", m_spellProto->Id, m_spellProto->Name, m_target->GetLowGUID());
	m_auraSlot = 0xffff;
	m_interrupted = -1;
	m_flags = 0;
	//fixed_amount = 0;//used only por percent values to be able to recover value correctly.No need to init this if we are not using it
}

Aura::~Aura()
{
    sEventMgr.RemoveEvents( this );
}

void Aura::Remove()
{
	sEventMgr.RemoveEvents( this );

	//TODO: Check this condition - consider there are 3 aura modifiers and m_deleted can be set to true by first one,
	// other two mods are normally applied, but cant un-apply (?)
	if( m_deleted )
		return;

	sHookInterface.OnAuraRemove(this);

	sLog.outDebug("Removing aura %u from unit %u", m_spellProto->Id, m_target->GetGUID());

	m_deleted = true;

	if( !IsPassive() || m_spellProto->AttributesEx & ATTRIBUTES_ON_NEXT_SWING_2 )
		m_target->ModVisualAuraStackCount( this, -1 );

	ApplyModifiers( false );

	for( uint32 x = 0; x < 3; x++ )
	{
		if( !m_spellProto->Effect[x] )
			continue;

		if( m_spellProto->Effect[x] == SPELL_EFFECT_TRIGGER_SPELL && !m_spellProto->always_apply )
		{
			// I'm not sure about this! FIX ME!!
			if ( dbcSpell.LookupEntryForced( GetSpellProto()->EffectTriggerSpell[x] )->DurationIndex < m_spellProto->DurationIndex )
				m_target->RemoveAura(GetSpellProto()->EffectTriggerSpell[x]);
		}
		else
			if( IsAreaAura() && m_casterGuid == m_target->GetGUID() )
				ClearAATargets();
	}

	if( m_spellProto->procCharges > 0 && m_spellProto->proc_interval == 0 )
	{
		if( m_target->m_chargeSpellsInUse )
		{
			m_target->m_chargeSpellRemoveQueue.push_back( GetSpellId() );
		}
		else
		{
			std::map< uint32, struct SpellCharge >::iterator iter;
			iter = m_target->m_chargeSpells.find( GetSpellId() );
			if( iter != m_target->m_chargeSpells.end() )
			{
				if( iter->second.count > 1 )
					--iter->second.count;
				else
					m_target->m_chargeSpells.erase(iter);
			}
		}
	}

	//maybe we are removing it without even assigning it. Example when we are refreshing an aura
	if( m_auraSlot != 0xFFFF )
		m_target->m_auras[ m_auraSlot ] = NULL;

	// reset diminishing return timer if needed
	::UnapplyDiminishingReturnTimer( m_target, m_spellProto );

	// remove attacker
	Unit* caster = GetUnitCaster();
	if( caster != NULL)
	{
		if( caster != m_target )
		{
			caster->CombatStatus.RemoveAttackTarget( m_target );
			m_target->CombatStatus.RemoveAttacker( caster, caster->GetGUID() );
		}
	}
	else
		m_target->CombatStatus.RemoveAttacker( NULL, m_casterGuid );

	/**********************Cooldown**************************
	* this is only needed for some spells
	* for now only spells that have:
	* (m_spellInfo->Attributes == 0x2050000) && !(m_spellInfo->AttributesEx) ||
	* m_spellProto->Attributes == 0x2040100
	* are handled. Its possible there are more spells like this
	*************************************************************/
	if( caster != NULL && caster->IsPlayer() && caster->IsInWorld() && m_spellProto->c_is_flags & SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE )
	{
        Player *p = static_cast< Player* >( caster );
        
        p->SendSpellCooldownEvent( m_spellProto->Id );
	}

	if( caster != NULL && caster->IsPlayer() && caster->IsInWorld() && caster->GetUInt32Value( PLAYER_FARSIGHT ) != 0 )
	{
		uint8 j;
		for( j = 0; j < 3; ++j )
			if( m_spellProto->Effect[j] == SPELL_EFFECT_ADD_FARSIGHT )
				break;

		if( j != 3 )
		{
			TO_PLAYER(caster)->SetFarsightTarget(0 );
		}
	}

	// If this aura can affect one target at a time, remove this target from the caster map
	if( caster != NULL && GetSpellProto()->AttributesExE & FLAGS6_SINGLE_TARGET_AURA )
		caster->RemoveCurrentUnitForSingleTargetAura( GetSpellProto() );

	/* Remove aurastates */
    uint32 flag = 0;
	if( m_spellProto->MechanicsType == MECHANIC_ENRAGED && !--m_target->asc_enraged )
		flag |= AURASTATE_FLAG_ENRAGED;
	else if( m_spellProto->MechanicsType == MECHANIC_BLEEDING && !--m_target->asc_bleed )
		flag |= AURASTATE_FLAG_BLEED;
	if( m_spellProto->BGR_one_buff_on_target & SPELL_TYPE_SEAL && !--m_target->asc_seal )
		flag |= AURASTATE_FLAG_JUDGEMENT;
	if( flag != 0 )
		m_target->RemoveFlag( UNIT_FIELD_AURASTATE, flag );

    // We will delete this on the next update, eluding some spell crashes :|
    m_target->AddGarbageAura( this );
	m_target->m_auras[ m_auraSlot ] = NULL;

	//only remove channel stuff if caster == target, then it's not removed twice, for example, arcane missiles applies a dummy aura to target
	if (caster != NULL && caster == m_target && m_spellProto->ChannelInterruptFlags != 0)
	{
		caster->SetChannelSpellTargetGUID(0);
		caster->SetChannelSpellId(0);
	}
}

void Aura::AddMod( uint32 t, int32 a, uint32 miscValue, uint32 i )
{
	//this is fix, when u have the same unit in target list several times
	//for(uint32 x= 0;x<m_modcount;x++)
	//	if(m_modList[x].i==i)return;

	/*if(m_modList[0].m_type == t ||
		m_modList[1].m_type == t ||
		m_modList[2].m_type == t)
		return; // don't duplicate mods // some spells apply duplicate mods, like some seals*/

	if( m_modcount >= 3 )
	{
		sLog.outError("Tried to add >3 (%u) mods to spellid %u [%u:%u, %u:%u, %u:%u]", m_modcount+1, this->m_spellProto->Id, m_modList[0].m_type, m_modList[0].m_amount, m_modList[1].m_type, m_modList[1].m_amount, m_modList[2].m_type, m_modList[2].m_amount);
		return;
	}
	m_modList[m_modcount].m_type = t;
	m_modList[m_modcount].m_amount = a;
	m_modList[m_modcount].m_miscValue = miscValue;
	m_modList[m_modcount].i = i;
	m_modcount++;
	//Arcemu::Util::ARCEMU_ASSERT(   m_modcount<=3);
}

void Aura::ApplyModifiers( bool apply )
{
	for( uint32 x = 0; x < m_modcount; x++ )
	{
		if( m_modList[x].m_type < TOTAL_SPELL_AURAS )
		{
			mod = &m_modList[x];
			sLog.outDebug( "WORLD: target=%u, Spell Aura id=%u (%s), SpellId=%u, i=%u, apply=%s, duration=%u, miscValue=%d, damage=%d",
				m_target->GetLowGUID(),mod->m_type, SpellAuraNames[mod->m_type], m_spellProto->Id, mod->i, apply ? "true" : "false",GetDuration(),mod->m_miscValue,mod->m_amount);
			(*this.*SpellAuraHandler[mod->m_type])(apply);

#ifdef GM_Z_DEBUG_DIRECTLY
		  if( m_target->IsPlayer() && m_target->IsInWorld() && x== 0) {
  		  if ( static_cast< Player* >( m_target )->GetSession() && static_cast< Player* >( m_target )->GetSession()->CanUseCommand('z')  )
				sChatHandler.BlueSystemMessage( static_cast< Player* >( m_target )->GetSession(), "[%sSystem%s] |rAura::ApplyModifiers: %s Target = %u, Spell Aura id = %u, SpellId = %u, modi=%u, apply = [%d], duration = %u, damage = %d, GiverGUID: %u.", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE,
				m_target->GetLowGUID(),mod->m_type, m_spellProto->Id, mod->i, apply ,GetDuration(),mod->m_amount,m_casterGuid );
  		}
#endif

		}
		else
			sLog.outError("Unknown Aura id %d", m_modList[x].m_type);
	}
}

void Aura::UpdateModifiers( )
{
	for( uint8 x = 0; x < m_modcount; x++ )
	{
		mod = &m_modList[x];

		if( mod->m_type < TOTAL_SPELL_AURAS )
		{
			sLog.outDebug( "WORLD: Update Aura mods : target = %u , Spell Aura id = %u (%s), SpellId  = %u, i = %u, duration = %u, damage = %d",
				m_target->GetLowGUID(), mod->m_type, SpellAuraNames[mod->m_type], m_spellProto->Id, mod->i, GetDuration(),mod->m_amount);
			switch (mod->m_type)
			{
				case SPELL_AURA_MOD_DECREASE_SPEED:
					UpdateAuraModDecreaseSpeed();
					break;
				case SPELL_AURA_MOD_ATTACK_POWER_BY_STAT_PCT:
				case SPELL_AURA_MOD_RANGED_ATTACK_POWER_BY_STAT_PCT:
					( *this.*SpellAuraHandler[ mod->m_type ] )( false );
					( *this.*SpellAuraHandler[ mod->m_type ] )( true );
					break;
			}
		}
		else
			sLog.outError("Unknown Aura id %d", (uint32)mod->m_type);
	}
}

void Aura::EventUpdateGroupAA( float r ){

	Player *p = NULL;
	
	if( m_target->IsPlayer() )
		p = TO_PLAYER( m_target );
	else
		return;


	if( !p->InGroup() ){
		targets.clear();
		return;
	}	
	
	p->GetGroup()->Lock();
	
	SubGroup *g = p->GetGroup()->GetSubGroup( p->GetSubGroup() );
	
	// Let's check for new targets
	for( GroupMembersSet::iterator itr = g->GetGroupMembersBegin(); itr != g->GetGroupMembersEnd(); ++itr ){
		PlayerInfo *pi = *itr;
		Player *op = pi->m_loggedInPlayer;
		
		if( op == NULL )
			continue;
		
		if( op->GetGUID() == p->GetGUID() )
			continue;
		
		if( op->GetInstanceID() != p->GetInstanceID() )
			continue;
		
		if( p->GetDistanceSq( op ) > r )
			continue;
		
		if( ( p->GetPhase() & op->GetPhase() ) == 0 )
			continue;
		
		if( !op->isAlive() )
			continue;
		
		if( op->HasAura( m_spellProto->Id ) )
			continue;
		
		targets.insert( op->GetGUID() );
	}
	
	// Check for targets that should be no longer affected
	for( AreaAuraList::iterator itr = targets.begin(); itr != targets.end();  ){
		AreaAuraList::iterator itr2 = itr;
		++itr;
		Player *tp = p->GetMapMgr()->GetPlayer( Arcemu::Util::GUID_LOPART( *itr2 ) );
		bool removable = false;
		
		if( tp == NULL ){
			targets.erase( *itr2 );
			continue;
		}
		
		if( p->GetDistanceSq( tp ) > r )
			removable = true;
		
		if( ( p->GetPhase() & tp->GetPhase() ) == 0 )
			removable = true;
		
		if( !g->HasMember( Arcemu::Util::GUID_LOPART( *itr2 ) ) )
			removable = true;
		
		if( removable ){
			targets.erase( itr2 );
			tp->RemoveAura( m_spellProto->Id );
		}
	}
	p->GetGroup()->Unlock();
}

void Aura::EventUpdateRaidAA( float r ){
	Player *p = NULL;
	
	if( m_target->IsPlayer() )
		p = TO_PLAYER( m_target );
	else
		return;

	if( !p->InGroup() ){
		targets.clear();
		return;
	}
	
	Group *g = p->GetGroup();
	
	g->Lock();
	
	uint32 subgroups = g->GetSubGroupCount();
	
	for( uint32 i = 0; i < subgroups; i++ ){
		SubGroup *sg = g->GetSubGroup( i );
		
		for( GroupMembersSet::iterator itr = sg->GetGroupMembersBegin(); itr != sg->GetGroupMembersEnd(); ++itr ){
			PlayerInfo *pi = *itr;
			Player *op = pi->m_loggedInPlayer;
			
			if( op == NULL )
				continue;
			
			if( op->GetGUID() == p->GetGUID() )
				continue;
			
			if( op->GetInstanceID() != p->GetInstanceID() )
				continue;
			
			if( p->GetDistanceSq( op ) > r )
				continue;
			
			if( ( p->GetPhase() & op->GetPhase() ) == 0 )
				continue;
			
			if( !op->isAlive() )
				continue;
			
			if( op->HasAura( m_spellProto->Id ) )
				continue;
			
			targets.insert( op->GetGUID() );
		}
	}
	g->Unlock();

	// Check for targets that should be no longer affected
	for( AreaAuraList::iterator itr = targets.begin(); itr != targets.end();  ){
		AreaAuraList::iterator itr2 = itr;
		++itr;

		Player *tp = p->GetMapMgr()->GetPlayer( Arcemu::Util::GUID_LOPART( *itr2 ) );
		bool removable = false;

		if( tp == NULL ){
			targets.erase( itr2 );
			continue;
		}

		if( p->GetDistanceSq( tp ) > r )
			removable = true;

		if( ( p->GetPhase() & tp->GetPhase() ) == 0 )
			removable = true;

		if( !tp->InGroup() )
			removable = true;

		if( removable ){
			targets.erase( itr2 );
			tp->RemoveAura( m_spellProto->Id );
		}

	}

}

void Aura::EventUpdatePetAA( float r ){
	Player *p = NULL;
	
	if( m_target->IsPlayer() )
		p = TO_PLAYER( m_target );
	else
		return;

	std::list< Pet* > pl = p->GetSummons();
	for( std::list< Pet* >::iterator itr = pl.begin(); itr != pl.end(); ++itr ){
		Pet *pet = *itr;

		if(  p->GetDistanceSq( pet ) > r )
			continue;

		if( !pet->isAlive() )
			continue;

		if( pet->HasAura( m_spellProto->Id ) )
			continue;

		{
			Aura *a = new Aura( m_spellProto, GetDuration(), p, pet, true );
			a->m_areaAura = true;
			a->AddMod( mod->m_type, mod->m_amount, mod->m_miscValue, mod->i );
			pet->AddAura( a );
		}
	}

	for( std::list< Pet* >::iterator itr = pl.begin(); itr != pl.end();  ){
		std::list< Pet* >::iterator itr2 = itr;
		
		Pet *pet = *itr2;
		++itr;

		if(  p->GetDistanceSq( pet ) <= r )
			continue;

		pet->RemoveAura( m_spellProto->Id );
	}
}

void Aura::EventUpdateFriendAA( float r ){
	Unit *u = m_target;
	if( u == NULL )
		return;

	for( std::set< Object* >::iterator itr = u->GetInRangeSetBegin(); itr != u->GetInRangeSetEnd(); ++itr ){
		Object *o = *itr;

		if( !o->IsUnit() )
			continue;

		Unit *ou = TO_UNIT( o );

		if( u->GetDistanceSq( ou ) > r )
			continue;

		if( ( u->GetPhase() & ou->GetPhase() ) == 0 )
			continue;

		if( !ou->isAlive() )
			continue;

		if( isHostile( u, ou ) )
			continue;

		if( isNeutral( u, ou ) )
			continue;

		if( ou->HasAura( m_spellProto->Id ) )
			continue;

		targets.insert( ou->GetGUID() );
	}

	for( AreaAuraList::iterator itr = targets.begin(); itr != targets.end(); ){
		AreaAuraList::iterator itr2 = itr;
		++itr;

		Unit *tu = u->GetMapMgr()->GetUnit( *itr2 );
		bool removable = false;

		if( tu == NULL ){
			targets.erase( itr2 );
			continue;
		}

		if( u->GetDistanceSq( tu ) > r )
			removable = true;

		if( isHostile( u, tu ) )
			removable = true;

		if( isNeutral( u, tu ) )
			removable = true;

		if( ( u->GetPhase() & tu->GetPhase() ) == 0 )
			removable = true;

		if( removable ){
			tu->RemoveAura( m_spellProto->Id );
			targets.erase( itr2 );
		}
	}
}

void Aura::EventUpdateEnemyAA( float r ){
	Unit *u = m_target;
	if( u == NULL )
		return;
	
	for( std::set< Object* >::iterator itr = u->GetInRangeSetBegin(); itr != u->GetInRangeSetEnd(); ++itr ){
		Object *o = *itr;

		if( !o->IsUnit() )
			continue;

		Unit *ou = TO_UNIT( o );

		if( u->GetDistanceSq( ou ) > r )
			continue;

		if( ( u->GetPhase() & ou->GetPhase() ) == 0 )
			continue;

		if( !ou->isAlive() )
			continue;

		if( !isHostile( u, ou ) )
			continue;

		if( ou->HasAura( m_spellProto->Id ) )
			continue;

		targets.insert( ou->GetGUID() );
	}

	for( AreaAuraList::iterator itr = targets.begin(); itr != targets.end(); ){
		AreaAuraList::iterator itr2 = itr;
		++itr;

		Unit *tu = u->GetMapMgr()->GetUnit( *itr2 );
		bool removable = false;

		if( tu == NULL ){
			targets.erase( itr2 );
			continue;
		}

		if( u->GetDistanceSq( tu ) > r )
			removable = true;

		if( !isHostile( u, tu ) )
			removable = true;

		if( isNeutral( u, tu ) )
			removable = true;

		if( ( u->GetPhase() & tu->GetPhase() ) == 0 )
			removable = true;

		if( removable ){
			tu->RemoveAura( m_spellProto->Id );
			targets.erase( itr2 );
		}
	}
}

void Aura::EventUpdateOwnerAA( float r ){
	Object *o = GetTarget();
	if( !o->IsCreature() )
		return;

	Creature *c = TO_CREATURE( o );
	Unit *ou = c->GetOwner();

	if( ou == NULL ){
		ou = c->GetMapMgr()->GetUnit( c->GetCreatedByGUID() );
		if( ou == NULL )
			return;
	}
	
	if( ou->isAlive() &&
		!ou->HasAura( m_spellProto->Id ) &&
		( c->GetDistanceSq( ou ) <= r ) ){

			Aura *a = new Aura( m_spellProto, GetDuration(), c, ou, true );
			a->m_areaAura = true;
			a->AddMod( mod->m_type, mod->m_amount, mod->m_miscValue, mod->i );
			ou->AddAura( a );
	}


	if( !ou->isAlive() || ( c->GetDistanceSq( ou ) > r ) )
		ou->RemoveAura( m_spellProto->Id );		
}

void Aura::EventUpdateAA( float r )
{
	/* burlex: cheap hack to get this to execute in the correct context always */
	if(event_GetCurrentInstanceId() == -1)
	{
		event_Relocate();
		return;
	}

	Unit * u_caster = GetUnitCaster();

	// if the caster is no longer valid->remove the aura
	if( u_caster == NULL  )
	{
		Remove();
		//since we lost the caster we cannot do anything more
		return;
	}

	uint32 AAEffectId = m_spellProto->GetAAEffectId();
	if( AAEffectId == 0 ){
		sLog.outError("Spell %u ( %s ) has tried to update Area Aura targets but Spell has no Area Aura effect.", m_spellProto->Id, m_spellProto->Name );
		Arcemu::Util::ARCEMU_ASSERT( false );
	}

	switch( AAEffectId ){

	case SPELL_EFFECT_APPLY_GROUP_AREA_AURA:
		EventUpdateGroupAA( r );
		break;

	case SPELL_EFFECT_APPLY_RAID_AREA_AURA:
		EventUpdateRaidAA( r );
		break;

	case SPELL_EFFECT_APPLY_PET_AREA_AURA:
		EventUpdatePetAA( r );
		break;

	case SPELL_EFFECT_APPLY_FRIEND_AREA_AURA:
		EventUpdateFriendAA( r );
		break;

	case SPELL_EFFECT_APPLY_ENEMY_AREA_AURA:
		EventUpdateEnemyAA( r );
		break;

	case SPELL_EFFECT_APPLY_OWNER_AREA_AURA:
		EventUpdateOwnerAA( r );
		break;

	default:
		Arcemu::Util::ARCEMU_ASSERT( false );
		break;
	}


	for( AreaAuraList::iterator itr = targets.begin(); itr != targets.end(); ++itr ){
		Unit *u = m_target->GetMapMgr()->GetUnit( *itr );

		if( u->HasAura( m_spellProto->Id ) )
			continue;
		
		Aura *a = new Aura( m_spellProto, GetDuration(), m_target, u, true );
		a->m_areaAura = true;
		a->AddMod( mod->m_type, mod->m_amount , mod->m_miscValue, mod->i );
		u->AddAura( a );
	}
}

void Aura::ClearAATargets(){
	uint32 spellid = m_spellProto->Id;

	for( AreaAuraList::iterator itr = targets.begin(); itr != targets.end(); ++itr ){
		Unit *tu = m_target->GetMapMgr()->GetUnit( *itr );

		tu->RemoveAura( spellid );
	}
	targets.clear();

	if( m_target->IsPlayer() && m_spellProto->HasEffect( SPELL_EFFECT_APPLY_PET_AREA_AURA ) ){
		Player *p = TO_PLAYER( m_target );

		std::list< Pet* > pl = p->GetSummons();
		for( std::list< Pet* >::iterator itr = pl.begin(); itr != pl.end(); ++itr ){
			Pet *pet = *itr;

			pet->RemoveAura( spellid );
		}
	}

	if( m_spellProto->HasEffect( SPELL_EFFECT_APPLY_OWNER_AREA_AURA ) ){
		Unit *u = m_target->GetMapMgr()->GetUnit( m_target->GetCreatedByGUID() );

		u->RemoveAura( spellid );
	}
}

//------------------------- Aura Effects -----------------------------

void Aura::SpellAuraModBaseResistance(bool apply)
{
	SpellAuraModResistance(apply);
	//both add/decrease some resistance difference is unknown
}

void Aura::SpellAuraModBaseResistancePerc(bool apply)
{
	uint32 Flag = mod->m_miscValue;
	int32 amt;
	if(apply)
	{
		amt = mod->m_amount;
		if(amt > 0)
			SetPositive();
		else
			SetNegative();
	}
	else
		amt =- mod->m_amount;

	for(uint32 x= 0;x<7;x++)
	{
		if(Flag & (((uint32)1)<< x))
		{
			if(m_target->GetTypeId() == TYPEID_PLAYER)
			{
				if(mod->m_amount>0)
				{
					static_cast< Player* >( m_target )->BaseResistanceModPctPos[x]+=amt;
				}
				else
				{
					static_cast< Player* >( m_target )->BaseResistanceModPctNeg[x]-=amt;
				}
				static_cast< Player* >( m_target )->CalcResistance(x);

			}
			else if(m_target->GetTypeId() == TYPEID_UNIT)
			{
				static_cast<Creature*>(m_target)->BaseResistanceModPct[x]+=amt;
				static_cast<Creature*>(m_target)->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraNULL(bool apply)
{
	 sLog.outDebug("Unknown Aura id %d", (uint32)mod->m_type);
}

void Aura::SpellAuraBindSight(bool apply)
{
	SetPositive();
	// MindVision
	Player *caster = GetPlayerCaster();
	if( caster == NULL )
		return;

	if( apply )
		caster->SetFarsightTarget(m_target->GetGUID() );
	else
		caster->SetFarsightTarget(0 );
}

void Aura::SpellAuraModPossess(bool apply)
{
	Player *caster = GetPlayerCaster();

	if(apply)
	{
		if( caster != NULL && caster->IsInWorld() )
			static_cast< Player* >(caster)->Possess( m_target );
	}
	else
	{
		bool unpossessfailed = false;
		if( caster != NULL && caster->IsInWorld() )
		{
			caster->UnPossess();
			m_target->RemoveAura(GetSpellId());
			if (caster->GetFarsightTarget() != 0 )
			{
			unpossessfailed = true;
			caster->SetFarsightTarget(0);
			caster->SetCharmedUnitGUID( 0 );
			caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
			}
		}

		// make sure Player::UnPossess() didn't fail, if it did we will just free the target here
		if( m_target->GetCharmedByGUID() != 0 )
		{
			if( m_target->GetTypeId() == TYPEID_UNIT )
			{
				m_target->setAItoUse( true );
				m_target->m_redirectSpellPackets = 0;
			}

			m_target->SetCharmedByGUID(  0 );
			m_target->RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			m_target->SetFaction(m_target->GetCharmTempVal() );
			m_target->UpdateOppFactionSet();
		}
		else
		{
			//mob woke up and realized he was controlled. He will turn to controller and also notify the other mobs he is fighting that they should attack the caster
			//sadly i got only 3 test cases about this so i might be wrong :(
			//zack : disabled until tested
			m_target->GetAIInterface()->EventChangeFaction( caster );
		}
	}
}

void Aura::SpellAuraPeriodicDamage(bool apply)
{
	if( apply )
	{
		if( m_spellProto->MechanicsType == MECHANIC_BLEEDING && m_target->MechanicsDispels[MECHANIC_BLEEDING] )
		{
			m_flags |= 1 << mod->i;
			return;
		}
		int32 dmg	= mod->m_amount;
		Unit *c = GetUnitCaster();
		switch(m_spellProto->Id)
		{
			case 703:
			case 8631:
			case 8632:
			case 8633:
			case 8818:
			case 11289:
			case 11290:
				if( c != NULL )
					c->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);  // remove stealth
				break;
			//mage talent ignite
			case 12654:
			{
				if(!pSpellId) //we need a parent spell and should always have one since it procs on it
					break;
				SpellEntry * parentsp = dbcSpell.LookupEntryForced(pSpellId);
				if(!parentsp)
					return;
				if( c != NULL && c->IsPlayer())
				{
					dmg = float2int32(static_cast< Player* >(c)->m_casted_amount[SCHOOL_FIRE]*parentsp->EffectBasePoints[0]/100.0f);
				}
				else if( c != NULL )
				{
					if (!dmg)
						return;
					Spell *spell = new Spell( c, parentsp, false, NULL );
					SpellCastTargets targets(m_target->GetGUID());

					//this is so not good, maybe parent spell has more then dmg effect and we use it to calc our new dmg :(
					dmg = 0;
					for( uint8 i = 0; i < 3; i++ )
					{
					  //dmg += parentsp->EffectBasePoints[i]*m_spellProto->EffectBasePoints[0];
						dmg += spell->CalculateEffect( i, m_target )* parentsp->EffectBasePoints[0] / 100;
					}
					delete spell;
					spell = NULL;
				}
			}
		};
		//this is warrior : Deep Wounds
		if( c != NULL && c->IsPlayer() && pSpellId)
		{
			uint32 multiplyer= 0;
			if(pSpellId==12834)
					multiplyer = 16; //level 1 of the talent should apply 16 of average melee weapon dmg
			else if(pSpellId==12849)
					multiplyer = 32;
			else if (pSpellId==12867)
					multiplyer = 48;
			if(multiplyer)
			{
				Player *pr=static_cast< Player* >(c);
				Item *it;
				it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if(it)
				{
					dmg = 0;
					for(int i= 0;i<5;i++)
						if(it->GetProto()->Damage[i].Type==SCHOOL_NORMAL)
							dmg += int32((it->GetProto()->Damage[i].Min + it->GetProto()->Damage[i].Max) / 2);
					dmg = multiplyer * dmg /100;
				}
			}
		}
		uint32* gr = GetSpellProto()->SpellGroupType;
		if( gr )
		{
			if( c != NULL )
			{
				SM_FIValue( c->SM_FDOT,(int32*)&dmg, gr );
				SM_PIValue( c->SM_PDOT,(int32*)&dmg, gr );
			}
		}

		if(dmg<= 0)
			return; //who would want a negative dmg here ?

		sLog.outDebug("Adding periodic dmg aura, spellid: %lu",this->GetSpellId() );
		sEventMgr.AddEvent(this, &Aura::EventPeriodicDamage,(uint32)dmg,
			EVENT_AURA_PERIODIC_DAMAGE,GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		/*static_cast< Player* >( c )->GetSession()->SystemMessage("dot will do %u damage every %u seconds (total of %u)", dmg,m_spellProto->EffectAmplitude[mod->i],(GetDuration()/m_spellProto->EffectAmplitude[mod->i])*dmg);
		printf("dot will do %u damage every %u seconds (total of %u)\n", dmg,m_spellProto->EffectAmplitude[mod->i],(GetDuration()/m_spellProto->EffectAmplitude[mod->i])*dmg);*/
		SetNegative();
		if( m_spellProto->BGR_one_buff_on_target & SPELL_TYPE_WARLOCK_IMMOLATE )
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_IMMOLATE);
		}
		//maybe poison aurastate should get triggered on other spells too ?
		else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_POISON )//deadly poison
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_POISON);
		}
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where immunity can cancel only 1 mod and not whole spell
	{
		if( m_spellProto->BGR_one_buff_on_target & SPELL_TYPE_WARLOCK_IMMOLATE )
			m_target->RemoveFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_IMMOLATE );
		//maybe poison aurastate should get triggered on other spells too ?
		else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_POISON )//deadly poison
		{
			m_target->RemoveFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_POISON);
		}
	}
}

void Aura::EventPeriodicDamage(uint32 amount)
{
	//DOT
	if( ! m_target->isAlive() )
		return;

	if( m_target->SchoolImmunityList[GetSpellProto()->School] )
	{
		if( GetUnitCaster() != NULL )
			SendTickImmune( m_target, GetUnitCaster() );
		return;
	}

	float res = float(amount);
	uint32 abs_dmg = 0;
	int bonus = 0;
	uint32 school = GetSpellProto()->School;
	Unit * c = GetUnitCaster();
	uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION;
	uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM;
	bool is_critical = false;

	if( m_target->GetGUID() != m_casterGuid )	//don't use resist when cast on self-- this is some internal stuff
	{
		if( c != NULL )
		{
			int amp = m_spellProto->EffectAmplitude[mod->i];
			if( !amp )
				amp = event_GetEventPeriod( EVENT_AURA_PERIODIC_DAMAGE );

			if( GetDuration() && GetSpellProto()->NameHash != SPELL_HASH_IGNITE )  //static damage for Ignite. Need to be reworked when "static DoTs" will be implemented
				bonus += c->GetSpellDmgBonus(m_target,m_spellProto,amount,true) * amp / GetDuration();

			res += bonus;

			if( res < 0 )
				res = 0;
			else
			{
				float summaryPCTmod = 1.0f;
				if( p_target != NULL ) //resilience
				{
					float dmg_reduction_pct = p_target->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
					if( dmg_reduction_pct > 1.0f )
						dmg_reduction_pct = 1.0f;
					summaryPCTmod -= dmg_reduction_pct;
				}
				res *= summaryPCTmod;
				if( res < 0.0f )
					res = 0.0f;
			}

			if( DotCanCrit() )
			{
				is_critical = c->IsCriticalDamageForSpell( m_target, GetSpellProto() );

				if( is_critical )
				{
					res = c->GetCriticalDamageBonusForSpell( m_target, GetSpellProto(), res);

					aproc |= PROC_ON_SPELL_CRIT_HIT;
					vproc |= PROC_ON_SPELL_CRIT_HIT_VICTIM;
				}
			}
		}

		uint32 ress=(uint32)res;
		abs_dmg = m_target->AbsorbDamage(school, &ress);
		uint32 ms_abs_dmg= m_target->ManaShieldAbsorb(ress);
		if (ms_abs_dmg)
		{
			if(ms_abs_dmg > ress)
				ress = 0;
			else
				ress-=ms_abs_dmg;

			abs_dmg += ms_abs_dmg;
		}


		if(ress < 0) ress = 0;
		res=(float)ress;
		dealdamage dmg;
		dmg.school_type = school;
		dmg.full_damage = ress;
		dmg.resisted_damage = 0;

		if(res <= 0)
			dmg.resisted_damage = dmg.full_damage;

		if(res > 0 && c && m_spellProto->MechanicsType != MECHANIC_BLEEDING)
		{
			c->CalculateResistanceReduction(m_target,&dmg, m_spellProto, 0);
			if((int32)dmg.resisted_damage > dmg.full_damage)
				res = 0;
			else
				res = float(dmg.full_damage - dmg.resisted_damage);
		}

		m_target->SendPeriodicAuraLog( m_casterGuid, m_target->GetNewGUID(), GetSpellProto()->Id, school, float2int32(res), abs_dmg, dmg.resisted_damage, FLAG_PERIODIC_DAMAGE, is_critical);
	}

	// grep: this is hack.. some auras seem to delete this shit.
	SpellEntry *sp = m_spellProto;

	if( m_target->m_damageSplitTarget )
		res = (float)m_target->DoDamageSplitTarget((uint32)res, GetSpellProto()->School, false);

	if( c != NULL )
		c->DealDamage(m_target, float2int32(res),  2, 0, GetSpellId() );
	else
		m_target->DealDamage(m_target, float2int32(res),  2, 0,  GetSpellId() );

	if( m_target->GetGUID() != m_casterGuid && c != NULL )	//don't use resist when cast on self-- this is some internal stuff
	{
		int32 dmg = float2int32(res);

		if( abs_dmg )
			vproc |= PROC_ON_ABSORB;

		c->HandleProc(aproc, m_target, sp, false, dmg, abs_dmg);
		c->m_procCounter = 0;

		m_target->HandleProc(vproc, c, sp, false, dmg, abs_dmg);
		m_target->m_procCounter = 0;
	}
}

void Aura::SpellAuraDummy(bool apply)
{
	if( sScriptMgr.CallScriptedDummyAura(GetSpellId(), mod->i, this, apply) )
		return;

	sLog.outError("Spell %u ( %s ) has an apply dummy aura effect, but no handler for it. ", m_spellProto->Id, m_spellProto->Name );
}

void Aura::SpellAuraModConfuse(bool apply)
{
	Unit* u_caster = GetUnitCaster();

	if( m_target->GetTypeId() == TYPEID_UNIT && static_cast<Creature*>(m_target)->IsTotem() )
		return;

	if(apply)
	{
		if( u_caster == NULL ) return;

		// Check Mechanic Immunity
		if( m_target->MechanicsDispels[MECHANIC_DISORIENTED]
		|| ( m_spellProto->MechanicsType == MECHANIC_POLYMORPHED && m_target->MechanicsDispels[MECHANIC_POLYMORPHED] )
		)
		{
			m_flags |= 1 << mod->i;
			return;
		}
		SetNegative();

		m_target->m_special_state |= UNIT_STATE_CONFUSE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);

		m_target->setAItoUse(true);
		m_target->GetAIInterface()->HandleEvent(EVENT_WANDER, u_caster, 0);

		if(p_target)
		{
			// this is a hackfix to stop player from moving -> see AIInterface::_UpdateMovement() Wander AI for more info
			WorldPacket data1(9);
			data1.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
			data1 << m_target->GetNewGUID() << uint8(0x00);
			p_target->GetSession()->SendPacket(&data1);
			p_target->SpeedCheatDelay( GetDuration() );
		}
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where immunity can cancel only 1 mod and not whole spell
	{
		m_target->m_special_state &= ~UNIT_STATE_CONFUSE;
		m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
		if( p_target )
			p_target->SpeedCheatReset();

		m_target->GetAIInterface()->HandleEvent( EVENT_UNWANDER, NULL, 0 );

		if(p_target)
		{
			// re-enable movement
			WorldPacket data1(9);
			data1.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
			data1 << m_target->GetNewGUID() << uint8(0x01);
			p_target->GetSession()->SendPacket(&data1);

			m_target->setAItoUse(false);

			if( u_caster != NULL )
				sHookInterface.OnEnterCombat( p_target, u_caster );
		}
		else
			m_target->GetAIInterface()->AttackReaction(u_caster, 1, 0);
	}
}

void Aura::SpellAuraModCharm(bool apply)
{
	Player *caster = GetPlayerCaster();
	if( caster == NULL )
		return;
	if( m_target->GetTypeId() != TYPEID_UNIT )
		return;

	Creature* target = static_cast< Creature* >( m_target );

	if( target->IsTotem() )
		return;

	SetPositive(3); //we ignore the other 2 effect of this spell and force it to be a positive spell

	if( apply )
	{
		if( (int32)m_target->getLevel() > mod->m_amount || m_target->IsPet() )
			return;

		// this should be done properly
		if( target->GetEnslaveCount() >= 10 )
			return;

		if( caster->GetCharmedUnitGUID() != 0 )
			return;

		m_target->m_special_state |= UNIT_STATE_CHARM;
		m_target->SetCharmTempVal( m_target->GetFaction( ) );
		m_target->SetFaction(caster->GetFaction());
		m_target->UpdateOppFactionSet();
		m_target->GetAIInterface()->Init(m_target, AITYPE_PET, MOVEMENTTYPE_NONE, caster);
		m_target->SetCharmedByGUID(  caster->GetGUID());
		caster->SetCharmedUnitGUID( target->GetGUID());
		//damn it, the other effects of enslave demon will agro him on us anyway :S
		m_target->GetAIInterface()->WipeHateList();
		m_target->GetAIInterface()->WipeTargetList();
		m_target->GetAIInterface()->setNextTarget( TO_UNIT(NULL));

		target->SetEnslaveCount(target->GetEnslaveCount() + 1);

		if( caster->GetSession() ) // crashfix
		{
			WorldPacket data(SMSG_PET_SPELLS, 500);
			data << target->GetGUID();
			data << uint16(0);
			data << uint32(0x1000);
			data << uint32(0x100);
			data << uint32(PET_SPELL_ATTACK);
			data << uint32(PET_SPELL_FOLLOW);
			data << uint32(PET_SPELL_STAY);
			for(int i = 0; i < 4; i++)
				data << uint32(0);
			data << uint32(PET_SPELL_AGRESSIVE);
			data << uint32(PET_SPELL_DEFENSIVE);
			data << uint32(PET_SPELL_PASSIVE);
			caster->GetSession()->SendPacket(&data);
			target->SetEnslaveSpell(m_spellProto->Id);
		}
	}
	else
	{
		m_target->m_special_state &= ~UNIT_STATE_CHARM;
		m_target->SetFaction(m_target->GetCharmTempVal() );
		m_target->GetAIInterface()->WipeHateList();
		m_target->GetAIInterface()->WipeTargetList();
		m_target->UpdateOppFactionSet();
		m_target->GetAIInterface()->Init(m_target, AITYPE_AGRO, MOVEMENTTYPE_NONE);
		m_target->SetCharmedByGUID(  0);

		if( caster->GetSession() != NULL ) // crashfix
		{
			caster->SetCharmedUnitGUID( 0 );
			WorldPacket data(SMSG_PET_SPELLS, 8);
			data << uint64(0);
			caster->GetSession()->SendPacket(&data);
			target->SetEnslaveSpell(0);
		}
	}
}

void Aura::SpellAuraModFear(bool apply)
{
	Unit* u_caster = GetUnitCaster();

	if( m_target->GetTypeId() == TYPEID_UNIT && 
        ( static_cast<Creature*>(m_target)->IsTotem() || static_cast< Creature* >( m_target )->isRooted() ) )
		return;

	if(apply)
	{
		if( u_caster == NULL ) return;
		// Check Mechanic Immunity
		if( m_target->MechanicsDispels[MECHANIC_FLEEING] )
		{
			m_flags |= 1 << mod->i;
			return;
		}

		SetNegative();

		m_target->m_special_state |= UNIT_STATE_FEAR;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);

		m_target->setAItoUse(true);
		m_target->GetAIInterface()->HandleEvent(EVENT_FEAR, u_caster, 0);
		m_target->m_fearmodifiers++;
		if(p_target)
		{
			// this is a hackfix to stop player from moving -> see AIInterface::_UpdateMovement() Fear AI for more info
			WorldPacket data1(9);
			data1.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
			data1 << m_target->GetNewGUID() << uint8(0x00);
			p_target->GetSession()->SendPacket(&data1);
			p_target->SpeedCheatDelay( GetDuration() );
		}
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where immunity can cancel only 1 mod and not whole spell
	{
		m_target->m_fearmodifiers--;

		if(m_target->m_fearmodifiers <= 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_FEAR;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FLEEING);

			m_target->GetAIInterface()->HandleEvent( EVENT_UNFEAR, NULL, 0 );

			if(p_target)
			{
				// re-enable movement
				WorldPacket data1(9);
				data1.Initialize( SMSG_CLIENT_CONTROL_UPDATE );
				data1 << m_target->GetNewGUID() << uint8(0x01);
				p_target->GetSession()->SendPacket(&data1);

				m_target->setAItoUse(false);

				if( u_caster != NULL )
					sHookInterface.OnEnterCombat( p_target, u_caster );
				p_target->SpeedCheatReset();
			}
			else
				m_target->GetAIInterface()->AttackReaction(u_caster, 1, 0);
		}
	}
}

void Aura::SpellAuraPeriodicHeal( bool apply )
{
	if( apply )
	{
		SetPositive();

		int32 val = mod->m_amount;
		Unit *c = GetUnitCaster();
		if (c && GetSpellProto()->SpellGroupType) {
			SM_FIValue(c->SM_FMiscEffect,&val,GetSpellProto()->SpellGroupType);
			SM_PIValue(c->SM_PMiscEffect,&val,GetSpellProto()->SpellGroupType);
		}

		if (val > 0) {
			sEventMgr.AddEvent( this, &Aura::EventPeriodicHeal,(uint32)val, EVENT_AURA_PERIODIC_HEAL, GetSpellProto()->EffectAmplitude[mod->i], 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

			if( GetSpellProto()->NameHash == SPELL_HASH_REJUVENATION || GetSpellProto()->NameHash == SPELL_HASH_REGROWTH )
			{
				m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_REJUVENATE);
				if(!sEventMgr.HasEvent( m_target, EVENT_REJUVENATION_FLAG_EXPIRE ) )
				{
					sEventMgr.AddEvent( m_target, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_REJUVENATE, EVENT_REJUVENATION_FLAG_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
				}
				else
				{
					sEventMgr.ModifyEventTimeLeft( m_target,EVENT_REJUVENATION_FLAG_EXPIRE, GetDuration(), 0 );
				}
			}
		}
	}
}

void Aura::EventPeriodicHeal( uint32 amount )
{
	if( !m_target->isAlive() )
		return;

	Unit* c = GetUnitCaster();

	int32 bonus = 0;
	bool is_critical = false;

	if( c != NULL)
	{
		bonus += c->HealDoneMod[m_spellProto->School] + m_target->HealTakenMod[m_spellProto->School];
		if( c->IsPlayer() )
		{
			for(uint32 a = 0; a < 5; a++)
				bonus += float2int32( static_cast< Player* >( c )->SpellHealDoneByAttribute[a][m_spellProto->School] * static_cast< Player* >( c )->GetStat(a) );
		}
		//Spell Coefficient
		if( m_spellProto->OTspell_coef_override >= 0 ) //In case we have forced coefficients
			bonus = float2int32( bonus * m_spellProto->OTspell_coef_override );
		else
		{
			//Bonus to HoT part
			if( m_spellProto->fixed_hotdotcoef >= 0 )
			{
				bonus = float2int32( bonus * m_spellProto->fixed_hotdotcoef );
				//we did most calculations in world.cpp, but talents that increase DoT spells duration
				//must be included now.
				if( c->IsPlayer() )
				{
					int durmod = 0;
					SM_FIValue(c->SM_FDur, &durmod, m_spellProto->SpellGroupType);
					bonus += bonus * durmod / 15000;
				}
			}
		}
	}

	if( c != NULL && m_spellProto->SpellGroupType )
	{
		/*
		int penalty_pct = 0;
		int penalty_flt = 0;
		SM_FIValue( c->SM_PPenalty, &penalty_pct, GetSpellProto()->SpellGroupType );
		bonus += bonus * ( penalty_pct / 100 );
		SM_FIValue( c->SM_FPenalty, &penalty_flt, GetSpellProto()->SpellGroupType );
		bonus += penalty_flt;
		*/
		SM_PIValue( c->SM_PPenalty, &bonus, m_spellProto->SpellGroupType );
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		int spell_flat_modifers= 0;
		int spell_pct_modifers= 0;
		SM_FIValue(c->SM_FPenalty,&spell_flat_modifers,GetSpellProto()->SpellGroupType);
		SM_FIValue(c->SM_PPenalty,&spell_pct_modifers,GetSpellProto()->SpellGroupType);
		if(spell_flat_modifers!= 0 || spell_pct_modifers!= 0)
			sLog.outDebug("!!!!!HEAL : spell dmg bonus(p=24) mod flat %d , spell dmg bonus(p=24) pct %d , spell dmg bonus %d, spell group %u",spell_flat_modifers,spell_pct_modifers,bonus,GetSpellProto()->SpellGroupType);
#endif
	}

	int amp = m_spellProto->EffectAmplitude[mod->i];
	if( !amp )
		amp = static_cast< EventableObject* >( this )->event_GetEventPeriod( EVENT_AURA_PERIODIC_HEAL );
//	if(m_spellProto->NameHash != SPELL_HASH_HEALING_STREAM)// Healing Stream is not a HOT
	{
		int32 dur = GetDuration();
		//example : Citrine Pendant of Golden Healing is in AA aura that does not have duration. In this case he would have full healbonus benefit
		if( (dur== 0 || dur==-1) && GetSpellProto()->DurationIndex)
		{
			SpellDuration *sd=dbcSpellDuration.LookupEntry(GetSpellProto()->DurationIndex);
			dur = ::GetDuration( sd );
		}
		if( dur && dur!=-1)
		{
			int ticks = ( amp > 0 ) ? dur / amp : 0;
			bonus = ( ticks > 0 ) ? bonus / ticks : 0;
		}
		//removed by Zack : Why is this directly setting bonus to 0 ? It's not logical
//		else bonus = 0;
	}
    /*Downranking
    if( c != NULL && c->IsPlayer() )
    {
		if( m_spellProto->baseLevel > 0 && m_spellProto->maxLevel > 0 )
		{
            float downrank1 = 1.0f;
            if( m_spellProto->baseLevel < 20 )
                downrank1 = 1.0f - ( 20.0f - float( m_spellProto->baseLevel ) ) * 0.0375f;

            float downrank2 = ( float(m_spellProto->maxLevel + 5.0f) / float(c->getLevel()) );
            if( downrank2 >= 1 || downrank2 < 0 )
                downrank2 = 1.0f;

            bonus = float2int32( float( bonus ) * downrank1 * downrank2 );
        }
    }*/

	int add = ( bonus + amount > 0 ) ? bonus + amount : 0;
	if( c != NULL )
	{
		add += float2int32( add * ( m_target->HealTakenPctMod[m_spellProto->School]+ c->HealDonePctMod[GetSpellProto()->School] ) );
		if (m_spellProto->SpellGroupType)
			SM_PIValue(c->SM_PDOT,&add,m_spellProto->SpellGroupType);

		if( this->DotCanCrit() )
		{
			is_critical = c->IsCriticalHealForSpell( m_target, GetSpellProto() );
			if( is_critical )
				add = float2int32( c->GetCriticalHealBonusForSpell( m_target, GetSpellProto(), (float) add) );
		}
	}

	uint32 curHealth = m_target->GetHealth();
	uint32 maxHealth = m_target->GetMaxHealth();
	uint32 over_heal = 0;

	if( (curHealth + add) >= maxHealth )
	{
		m_target->SetHealth(maxHealth);
		over_heal = curHealth + add - maxHealth;
	}
	else
		m_target->ModHealth(add);

	m_target->SendPeriodicHealAuraLog( m_casterGuid, m_target->GetNewGUID(), GetSpellId(), add, over_heal, is_critical );

	m_target->RemoveAurasByHeal();

	if( GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP )
	{
		m_target->Emote( EMOTE_ONESHOT_EAT );
	}

	// add threat
	Unit* u_caster = GetUnitCaster();
	if( u_caster != NULL )
	{
		if (GetSpellProto()->NameHash == SPELL_HASH_HEALTH_FUNNEL && add > 0)
		{
			dealdamage sdmg;

			sdmg.full_damage = add;
			sdmg.resisted_damage = 0;
			sdmg.school_type = 0;
			u_caster->DealDamage(u_caster, add, 0, 0, 0);
			u_caster->SendAttackerStateUpdate(u_caster, u_caster, &sdmg, add, 0, 0, 0, ATTACK);
		}

		std::vector<Unit*> target_threat;
		int count = 0;
		for(std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT || !static_cast<Unit *>(*itr)->CombatStatus.IsInCombat() || (static_cast<Unit *>(*itr)->GetAIInterface()->getThreatByPtr(u_caster) == 0 && static_cast<Unit *>(*itr)->GetAIInterface()->getThreatByPtr(m_target) == 0))
				continue;

			if( !(u_caster->GetPhase() & (*itr)->GetPhase()) ) //Can't see, no threat
				continue;

			target_threat.push_back(static_cast<Unit *>(*itr));
			count++;
		}
		if (count == 0)
			return;

		add = add / count;

		for(std::vector<Unit*>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
		{
			static_cast<Unit *>(*itr)->GetAIInterface()->HealReaction(u_caster, m_target, m_spellProto, add);
		}

		if(m_target->IsInWorld() && u_caster->IsInWorld())
			u_caster->CombatStatus.WeHealed(m_target);
	}
}

void Aura::SpellAuraModAttackSpeed( bool apply )
{
	if( mod->m_amount < 0 )
		SetNegative();
	else
		SetPositive();

	if ( m_target->GetTypeId() == TYPEID_PLAYER )
	{
		if(apply)
		{
			static_cast< Player* >( m_target )->ModAttackSpeed( mod->m_amount, MOD_MELEE );
		}
		else
		{
			static_cast< Player* >( m_target )->ModAttackSpeed( -mod->m_amount, MOD_MELEE );
		}
		static_cast< Player* >( m_target )->UpdateStats();
	}
	else
	{
		if(apply)
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME,mod->m_amount);
			mod->fixed_amount[1] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME+1,mod->m_amount);
			mod->fixed_amount[2] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,mod->m_amount);
			m_target->ModBaseAttackTime(MELEE,-mod->fixed_amount[0]);
			m_target->ModBaseAttackTime(OFFHAND,-mod->fixed_amount[1]);
			m_target->ModBaseAttackTime(RANGED,-mod->fixed_amount[2]);
		}
		else
		{
			m_target->ModBaseAttackTime(MELEE,mod->fixed_amount[0]);
			m_target->ModBaseAttackTime(OFFHAND,mod->fixed_amount[1]);
			m_target->ModBaseAttackTime(RANGED,mod->fixed_amount[2]);
		}
	}

}

void Aura::SpellAuraModThreatGenerated(bool apply)
{
	mod->m_amount < 0 ? SetPositive() : SetNegative();
	for( uint32 x = 0; x < 7; x++ )
	{
		if( mod->m_miscValue & ( ( (uint32)1 ) << x ) )
		{
			if ( apply )
				m_target->ModGeneratedThreatModifyer(x, mod->m_amount);
			else
				m_target->ModGeneratedThreatModifyer(x, -(mod->m_amount));
		}
	}
}

void Aura::SpellAuraModTaunt(bool apply)
{
	Unit* m_caster = GetUnitCaster();
	if(!m_caster || !m_caster->isAlive())
		return;

	SetNegative();

	if(apply)
	{
		m_target->GetAIInterface()->AttackReaction(m_caster, 1, 0);
		m_target->GetAIInterface()->taunt(m_caster, true);
	}
	else
	{
		if(m_target->GetAIInterface()->getTauntedBy() == m_caster)
		{
			m_target->GetAIInterface()->taunt(m_caster, false);
		}
	}
}

void Aura::SpellAuraModStun(bool apply)
{
	if(apply)
	{
		// Check Mechanic Immunity
		// Stun is a tricky one... it's used for all different kinds of mechanics as a base Aura
		if( !IsPositive() && m_spellProto->NameHash != SPELL_HASH_ICE_BLOCK )  // ice block stuns you, don't want our own spells to ignore stun effects
		{
			if( ( m_spellProto->MechanicsType == MECHANIC_CHARMED &&  m_target->MechanicsDispels[MECHANIC_CHARMED] )
			|| ( m_spellProto->MechanicsType == MECHANIC_INCAPACIPATED && m_target->MechanicsDispels[MECHANIC_INCAPACIPATED] )

			|| ( m_spellProto->MechanicsType == MECHANIC_SAPPED && m_target->MechanicsDispels[MECHANIC_SAPPED] )
			|| ( m_target->MechanicsDispels[MECHANIC_STUNNED] )
				)
			{
				m_flags |= 1 << mod->i;
				return;
			}
		}
		SetNegative();

		m_target->m_rooted++;

		if( m_target->m_rooted == 1 )
			m_target->Root();

		if( m_target->IsStealth() )
			m_target->RemoveStealth();

		m_target->m_stunned++;
		m_target->m_special_state |= UNIT_STATE_STUN;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

		if(m_target->GetTypeId() == TYPEID_UNIT)
			m_target->GetAIInterface()->setNextTarget( TO_UNIT(NULL) );

		// remove the current spell (for channalers)
		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid &&
			m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
		{
			m_target->m_currentSpell->cancel();
			m_target->m_currentSpell = 0;
		}

		//warrior talent - second wind triggers on stun and immobilize. This is not used as proc to be triggered always !
		Unit *caster = GetUnitCaster();
		if( caster != NULL )
		{
			caster->EventStunOrImmobilize( m_target );
			m_target->EventStunOrImmobilize( caster, true );
		}
		if( m_target->IsCasting() )
			m_target->CancelSpell(NULL); //cancel spells.
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where immunity can cancel only 1 mod and not whole spell
	{
		m_target->m_rooted--;

		if(m_target->m_rooted == 0)
			m_target->Unroot();

		m_target->m_stunned--;

		if(m_target->m_stunned == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_STUN;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
		}

		// attack them back.. we seem to lose this sometimes for some reason
		if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			Unit * target = GetUnitCaster();
			if( m_target->GetAIInterface()->getNextTarget() != NULL )
				target = m_target->GetAIInterface()->getNextTarget();

			if( target == NULL )
				return;
			m_target->GetAIInterface()->AttackReaction( target, 1, 0 );
		}
	}

/*
	if(apply)
	{
		switch(this->m_spellProto->Id)
		{
		case 652:
		case 2070:
		case 6770:
		case 6771:
		case 11297:
		case 11298:
			{
				// sap
				Unit *c = GetUnitCaster();
				if(c)
					c->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);  // remove stealth
			}break;
		case 1776:
		case 1777:
		case 1780:
		case 1781:
		case 8629:
		case 8630:
		case 11285:
		case 11286:
		case 11287:
		case 11288:
		case 12540:
		case 13579:
		case 24698:
		case 28456:
			{
				// gouge
				Unit *c = GetUnitCaster();
				if(c && c->GetTypeId() == TYPEID_PLAYER)
				{
					//static_cast< Player* >( c )->CombatModeDelay = 10;
					static_cast< Player* >( c )->EventAttackStop();
					c->smsg_AttackStop(m_target);
					c->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);  // remove stealth
				}
			}
		}
	}*/
}

void Aura::SpellAuraModDamageDone(bool apply)
{
	int32 val;

	if( m_target->IsPlayer() )
	{
		uint32 index;

		if( mod->m_amount > 0 )
		{
			if( apply )
			{
				SetPositive();
				val = mod->m_amount;
			}
			else
			{
				val = -mod->m_amount;
			}
			index = PLAYER_FIELD_MOD_DAMAGE_DONE_POS;

		}
		else
		{
			if( apply )
			{
				SetNegative();
				val = -mod->m_amount;
			}
			else
			{
				val = mod->m_amount;
			}
			index = PLAYER_FIELD_MOD_DAMAGE_DONE_NEG;
		}

		for( uint32 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & ( ( (uint32)1 ) << x ) )
			{
				m_target->ModUnsigned32Value( index + x, val );
			}
		}
	}
	else if( m_target->GetTypeId() == TYPEID_UNIT )
	{
		if( mod->m_amount > 0 )
		{
			if( apply )
			{
				SetPositive();
				val = mod->m_amount;
			}
			else
			{
				val = -mod->m_amount;
			}

		}
		else
		{
			if( apply )
			{
				SetNegative();
				val = mod->m_amount;
			}
			else
			{
				val = -mod->m_amount;
			}
		}

		for( uint32 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & ( ( (uint32)1 ) << x ) )
			{
				static_cast< Creature* >( m_target )->ModDamageDone[x] += val;
			}
		}
	}

	if( mod->m_miscValue & 1 )
		m_target->CalcDamage();
}

void Aura::SpellAuraModDamageTaken(bool apply)
{
	int32 val = (apply) ? mod->m_amount : -mod->m_amount;
	for(uint32 x= 0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->DamageTakenMod[x]+=val;
		}
	}
}

void Aura::SpellAuraDamageShield(bool apply)
{
	if(apply)
	{
		SetPositive();
		DamageProc ds;// = new DamageShield();
		ds.m_damage = mod->m_amount;
		ds.m_spellId = GetSpellProto()->Id;
		ds.m_school = GetSpellProto()->School;
		ds.m_flags = PROC_ON_MELEE_ATTACK_VICTIM | PROC_MISC; //maybe later we might want to add other flags too here
		ds.owner = (void*)this;
		m_target->m_damageShields.push_back(ds);
	}
	else
	{
		for(std::list<struct DamageProc>::iterator i = m_target->m_damageShields.begin();i != m_target->m_damageShields.end();i++)
		{
			if(i->owner==this)
			{
				 m_target->m_damageShields.erase(i);
				 return;
			}
		}
	}
}

void Aura::SpellAuraModStealth(bool apply)
{
	if(apply)
	{
		//Overkill must proc only if we aren't already stealthed
		if( !m_target->IsStealth() && m_target->HasAurasWithNameHash(SPELL_HASH_OVERKILL) )
		{
			m_target->CastSpell(m_target, 58427, true);
		}

		if(p_target && p_target->m_bgHasFlag)
		{
			if(p_target->m_bg && p_target->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
			{
				((WarsongGulch*)p_target->m_bg)->HookOnFlagDrop(p_target);
			}
			if(p_target->m_bg && p_target->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
			{
				((EyeOfTheStorm*)p_target->m_bg)->HookOnFlagDrop(p_target);
			}
	     }

		SetPositive();
		if( m_spellProto->NameHash != SPELL_HASH_VANISH)
			m_target->SetStealth(GetSpellId());

		if( m_spellProto->NameHash == SPELL_HASH_STEALTH)
			m_target->SetFlag(UNIT_FIELD_BYTES_2,0x1E000000);//sneak anim

		m_target->SetFlag(UNIT_FIELD_BYTES_1, 0x020000);
		if( m_target->IsPlayer() )
			m_target->SetFlag(PLAYER_FIELD_BYTES2, 0x2000);

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_STEALTH | AURA_INTERRUPT_ON_INVINCIBLE);
		m_target->m_stealthLevel += mod->m_amount;

		// hack fix for vanish stuff
		if( m_spellProto->NameHash == SPELL_HASH_VANISH && m_target->GetTypeId() == TYPEID_PLAYER )	 // Vanish
		{

			for( Object::InRangeSet::iterator iter = m_target->GetInRangeSetBegin();iter != m_target->GetInRangeSetEnd(); ++iter )
			{
				if ((*iter) == NULL || !(*iter)->IsUnit())
					continue;

				Unit* _unit = static_cast< Unit* >( *iter );

				if( !_unit->isAlive() )
					continue;

				if( _unit->GetCurrentSpell() && _unit->GetCurrentSpell()->GetUnitTarget() == m_target)
					_unit->GetCurrentSpell()->cancel();

				if( _unit->GetAIInterface() != NULL )
					_unit->GetAIInterface()->RemoveThreatByPtr( m_target );
			}

			for( uint32 x = MAX_POSITIVE_AURAS_EXTEDED_START; x < MAX_POSITIVE_AURAS_EXTEDED_END; x++ )
			{
				if( m_target->m_auras[x] != NULL )
				{
					if( m_target->m_auras[x]->GetSpellProto()->MechanicsType == MECHANIC_ROOTED || m_target->m_auras[x]->GetSpellProto()->MechanicsType == MECHANIC_ENSNARED ) // Remove roots and slow spells
					{
						m_target->m_auras[x]->Remove();
					}
					else // if got immunity for slow, remove some that are not in the mechanics
					{
						for( int i = 0; i < 3; i++ )
						{
							uint32 AuraEntry = m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[i];
							if( AuraEntry == SPELL_AURA_MOD_DECREASE_SPEED || AuraEntry == SPELL_AURA_MOD_ROOT || AuraEntry == SPELL_AURA_MOD_STALKED)
							{
								m_target->m_auras[x]->Remove();
								break;
							}
						}
					}
				}
			}

			// check for stealth spells
			if( p_target != NULL )
			{
				uint32 stealth_id = 0;
				SpellSet::iterator itr = p_target->mSpells.begin();
				SpellSet::iterator end = p_target->mSpells.end();
				for(; itr != end; ++itr)
				{
					if( ( (*itr) == 1787 || (*itr) == 1786 || (*itr) == 1785 || (*itr) == 1784) && stealth_id < (*itr) )
					{
						stealth_id = *itr;
					}
				}
				if( stealth_id != 0 )
					p_target->CastSpell( p_target, dbcSpell.LookupEntry( stealth_id ), true );

				p_target->Dismount();

				if( p_target->m_bg && p_target->m_bgHasFlag )
				{
					if( p_target->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH )
					{
						((WarsongGulch*)p_target->m_bg)->HookOnFlagDrop( p_target );
					}
					if( p_target->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
					{
						((EyeOfTheStorm*)p_target->m_bg)->HookOnFlagDrop( p_target );
					}
				}
			}
		}
	}
	else
	{
		m_target->m_stealthLevel -= mod->m_amount;

		if( m_spellProto->NameHash != SPELL_HASH_VANISH )
		{
			m_target->SetStealth(0);
			m_target->RemoveFlag(UNIT_FIELD_BYTES_2,0x1E000000);

			m_target->RemoveFlag(UNIT_FIELD_BYTES_1, 0x020000);

			if( p_target != NULL )
			{
				p_target->RemoveFlag( PLAYER_FIELD_BYTES2, 0x2000 );
                p_target->SendSpellCooldownEvent( m_spellProto->Id );

				if( p_target->m_outStealthDamageBonusPeriod && p_target->m_outStealthDamageBonusPct )
					p_target->m_outStealthDamageBonusTimer = (uint32)UNIXTIME + p_target->m_outStealthDamageBonusPeriod;
			}
		}

		if( (m_target->HasAurasWithNameHash(SPELL_HASH_MASTER_OF_SUBTLETY) || m_target->HasAurasWithNameHash(SPELL_HASH_OVERKILL)) && m_spellProto->NameHash == SPELL_HASH_STEALTH )
		{
			for( uint32 x=MAX_POSITIVE_AURAS_EXTEDED_START ; x<MAX_POSITIVE_AURAS_EXTEDED_END; x++ )
			{
				if( m_target->m_auras[x] && 
				(m_target->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_MASTER_OF_SUBTLETY ||
				 m_target->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_OVERKILL) && 
				 m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[0] != SPELL_AURA_DUMMY )
				{
					uint32 tmp_duration = MSTIME_6SECONDS;
					if( m_target->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_OVERKILL )
						tmp_duration = MSTIME_SECOND * 20;

					m_target->m_auras[x]->SetDuration(tmp_duration);

					sEventMgr.AddEvent(m_target->m_auras[x], &Aura::Remove, EVENT_AURA_REMOVE, tmp_duration, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
				}
			}
		}
	}

	m_target->UpdateVisibility();
}

void Aura::SpellAuraModDetect(bool apply)
{
	if(apply)
	{
		//SetPositive();
		m_target->m_stealthDetectBonus += mod->m_amount;
	}
	else
		m_target->m_stealthDetectBonus -= mod->m_amount;
}

void Aura::SpellAuraModInvisibility(bool apply)
{
	SetPositive();
	if(m_spellProto->Effect[mod->i] == SPELL_EFFECT_APPLY_FRIEND_AREA_AURA ) // WTF is this crap? TODO clean this
		return;

	if(apply)
	{
		m_target->SetInvisibility(GetSpellId());
		m_target->m_invisFlag = static_cast<uint8>( mod->m_miscValue );
		if( m_target->GetTypeId() == TYPEID_PLAYER )
		{
			if( GetSpellId() == 32612 )
				static_cast<Player*>(m_target)->SetFlag( PLAYER_FIELD_BYTES2, 0x4000 ); //Mage Invis self visual
		}

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_INVINCIBLE);
	}
	else
	{
		m_target->m_invisFlag = INVIS_FLAG_NORMAL;
		if( m_target->GetTypeId() == TYPEID_PLAYER )
		{
			if( GetSpellId() == 32612 )
				static_cast<Player*>(m_target)->RemoveFlag( PLAYER_FIELD_BYTES2, 0x4000 );
		}
	}

	m_target->m_invisible = apply;
	m_target->UpdateVisibility();
}

void Aura::SpellAuraModInvisibilityDetection(bool apply)
{
	//Always Positive

	Arcemu::Util::ARCEMU_ASSERT(   mod->m_miscValue < INVIS_FLAG_TOTAL);
	if(apply)
	{
		m_target->m_invisDetect[mod->m_miscValue] += mod->m_amount;
		SetPositive ();
	}
	else
		m_target->m_invisDetect[mod->m_miscValue] -= mod->m_amount;

	if(m_target->IsPlayer())
		static_cast< Player* >( m_target )->UpdateVisibility();
}

void Aura::SpellAuraModTotalHealthRegenPct(bool apply)
{
	if(apply)
	{
		SetPositive();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicHealPct,(float)mod->m_amount,
			EVENT_AURA_PERIODIC_HEALPERC,	GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicHealPct(float RegenPct)
{
	if(!m_target->isAlive())
		return;

	uint32 add = float2int32(m_target->GetMaxHealth() * (RegenPct / 100.0f));

	uint32 newHealth = m_target->GetHealth() + add;

	if(newHealth <= m_target->GetMaxHealth())
		m_target->SetHealth( newHealth);
	else
		m_target->SetHealth( m_target->GetMaxHealth());

	m_target->SendPeriodicAuraLog( m_casterGuid, m_target->GetNewGUID(), m_spellProto->Id, m_spellProto->School, add, 0, 0, FLAG_PERIODIC_HEAL, false);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}

	m_target->RemoveAurasByHeal();
}

void Aura::SpellAuraModTotalManaRegenPct(bool apply)
{
	if(apply)
	{
		SetPositive();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicManaPct,(float)mod->m_amount,
			EVENT_AURA_PERIOCIC_MANA,	GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicManaPct(float RegenPct)
{
	if(!m_target->isAlive())
		return;

	uint32 add = float2int32(m_target->GetMaxPower( POWER_TYPE_MANA ) * (RegenPct / 100.0f));

	uint32 newHealth = m_target->GetPower(POWER_TYPE_MANA) + add;

	if(newHealth <= m_target->GetMaxPower( POWER_TYPE_MANA ))
		m_target->SetPower( POWER_TYPE_MANA, newHealth);
	else
		m_target->SetPower( POWER_TYPE_MANA, m_target->GetMaxPower( POWER_TYPE_MANA ));

	// CAPT
	// TODO: sniff it or disasm wow.exe to find the mana flag

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
}

void Aura::EventPeriodicTriggerDummy(){
	
	if( !sScriptMgr.CallScriptedDummyAura( m_spellProto->Id, mod->i, this, true ) )
		sLog.outError("Spell %u ( %s ) has an apply periodic trigger dummy aura effect, but no handler for it.", m_spellProto->Id, m_spellProto->Name );
}

void Aura::SpellAuraModResistance(bool apply)
{
	uint32 Flag = mod->m_miscValue;
	int32 amt;
	if(apply)
	{
		amt = mod->m_amount;
		if(amt <0 )//don't change it
			SetNegative();
		else
			SetPositive();
	}
	else
		amt = -mod->m_amount;
	Unit* caster = GetUnitCaster();
	if (!IsPositive() && caster != NULL && m_target->GetTypeId() == TYPEID_UNIT)
		m_target->GetAIInterface()->AttackReaction(caster, 1, GetSpellId());

	if( GetSpellProto()->NameHash == SPELL_HASH_FAERIE_FIRE || GetSpellProto()->NameHash == SPELL_HASH_FAERIE_FIRE__FERAL_ )
		m_target->m_can_stealth = !apply;
	
	Player *plr = GetPlayerCaster();
	if( plr != NULL && GetSpellProto()->NameHash == SPELL_HASH_DEVOTION_AURA)
	{
		// Increases the armor bonus of your Devotion Aura by %u - HACKY
		if( plr->HasSpell( 20140 ) ) // Improved Devotion Aura Rank 3
			amt = (int32)(amt * 1.5);
		else if( plr->HasSpell( 20139 ) ) // Improved Devotion Aura Rank 2
			amt = (int32)(amt * 1.34);
		else if( plr->HasSpell( 20138 ) ) // Improved Devotion Aura Rank 1
			amt = (int32)(amt * 1.17);
	}

	if( m_target->GetTypeId() == TYPEID_PLAYER )
	{
		for( uint32 x = 0; x < 7; x++ )
		{
			if(Flag & (((uint32)1)<< x) )
			{
				if(mod->m_amount>0)
						static_cast< Player* >( m_target )->FlatResistanceModifierPos[x]+=amt;
				else
						static_cast< Player* >( m_target )->FlatResistanceModifierNeg[x]-=amt;
				static_cast< Player* >( m_target )->CalcResistance(x);
			}
		}
	}
	else if(m_target->GetTypeId() == TYPEID_UNIT)
	{
		for(uint32 x= 0;x<7;x++)
		{
			if(Flag & (((uint32)1)<<x))
			{
				static_cast<Creature*>(m_target)->FlatResistanceMod[x]+=amt;
				static_cast<Creature*>(m_target)->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraPeriodicTriggerSpellWithValue(bool apply)
{
	if(apply)
	{
		SpellEntry* spe = dbcSpell.LookupEntryForced(m_spellProto->EffectTriggerSpell[mod->i]);
		if(spe == NULL)
			return;

		float amptitude = static_cast<float>( GetSpellProto()->EffectAmplitude[mod->i] );
		Unit* caster = GetUnitCaster();
		uint32 numticks = GetSpellDuration(m_spellProto, caster) / m_spellProto->EffectAmplitude[mod->i];
		if (caster != NULL)
		{	
			SM_FFValue(caster->SM_FAmptitude, &amptitude, m_spellProto->SpellGroupType);
			SM_PFValue(caster->SM_PAmptitude, &amptitude, m_spellProto->SpellGroupType);
			if (m_spellProto->ChannelInterruptFlags != 0)
				amptitude *= caster->GetCastSpeedMod();
		}

 		sEventMgr.AddEvent(this, &Aura::EventPeriodicTriggerSpell, spe, true, mod->m_amount,
			EVENT_AURA_PERIODIC_TRIGGERSPELL, float2int32(amptitude), numticks, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::SpellAuraPeriodicTriggerSpell(bool apply)
{
	if(m_spellProto->EffectTriggerSpell[mod->i] == 0)
		return;

	/*
	// This should be fixed in other way...
	if(IsPassive() &&
		m_spellProto->dummy != 2010 &&
		m_spellProto->dummy != 2020 &&
		m_spellProto->dummy != 2255 &&
		m_spellProto->Id != 8145 &&
		m_spellProto->Id != 8167 &&
		m_spellProto->Id != 8172)
	{
		Unit * target = (m_target != 0) ? m_target : GetUnitCaster();
		if(target == 0 || !target->IsPlayer())
			return; //what about creatures ?

		SpellEntry *proto = dbcSpell.LookupEntry( m_spellProto->EffectTriggerSpell[mod->i] );

		if( apply )
			static_cast< Player* >( target )->AddOnStrikeSpell( proto, m_spellProto->EffectAmplitude[mod->i] );
		else
			static_cast< Player* >( target )->RemoveOnStrikeSpell( proto );

		return;
	}
			*/

	if(apply)
	{
		SpellEntry* trigger = dbcSpell.LookupEntryForced(GetSpellProto()->EffectTriggerSpell[mod->i]);

		if (trigger == NULL)
			return;


		float amptitude = static_cast<float>( GetSpellProto()->EffectAmplitude[mod->i] );
		Unit* caster = GetUnitCaster();
		uint32 numticks = GetSpellDuration(m_spellProto, caster) / m_spellProto->EffectAmplitude[mod->i];
		if (caster != NULL)
		{	
			SM_FFValue(caster->SM_FAmptitude, &amptitude, m_spellProto->SpellGroupType);
			SM_PFValue(caster->SM_PAmptitude, &amptitude, m_spellProto->SpellGroupType);
			if (m_spellProto->ChannelInterruptFlags != 0)
				amptitude *= caster->GetCastSpeedMod();
		}
		
		sEventMgr.AddEvent(this, &Aura::EventPeriodicTriggerSpell, trigger, false, int32(0),
		EVENT_AURA_PERIODIC_TRIGGERSPELL, float2int32(amptitude), numticks, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicTriggerSpell(SpellEntry* spellInfo, bool overridevalues, int32 overridevalue)
{
	Spell* spell = new Spell(m_target, spellInfo, true, this);
	if (overridevalues)
	{
		spell->m_overrideBasePoints = true;
		for (uint32 i = 0; i < 3; ++i)
			spell->m_overridenBasePoints[i] = overridevalue;
	}
	SpellCastTargets targets;
	spell->GenerateTargets(&targets);
	spell->prepare(&targets);
}

void Aura::SpellAuraPeriodicEnergize(bool apply)
{
	if(apply)
	{
		SetPositive();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicEnergize,(uint32)mod->m_amount,(uint32)mod->m_miscValue,
			EVENT_AURA_PERIODIC_ENERGIZE,GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicEnergize( uint32 amount, uint32 type )
{
	uint32 POWER_TYPE = UNIT_FIELD_POWER1 + type;

	Arcemu::Util::ARCEMU_ASSERT(    POWER_TYPE <= UNIT_FIELD_POWER5 );
	
	Unit* ucaster = GetUnitCaster();
	if( ucaster == NULL )
		return;

	ucaster->Energize( m_target, m_spellProto->Id, amount, type );

	if( ( GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP ) && type == POWER_TYPE_MANA )
	{
		m_target->Emote( EMOTE_ONESHOT_EAT );
	}
}

void Aura::SpellAuraModPacify(bool apply)
{
	// Can't Attack
	if( apply )
	{
		if( m_spellProto->Id == 24937 || m_spellProto->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION )
			SetPositive();
		else
			SetNegative();

		m_target->m_pacified++;
		m_target->m_special_state |= UNIT_STATE_PACIFY;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
	}
	else
	{
		m_target->m_pacified--;

		if(m_target->m_pacified == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_PACIFY;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
		}
	}
}

void Aura::SpellAuraModRoot(bool apply)
{
	if(apply)
	{
		// Check Mechanic Immunity
		if( m_target->MechanicsDispels[MECHANIC_ROOTED] )
		{
			m_flags |= 1 << mod->i;
			return;
		}

		SetNegative();

		m_target->m_rooted++;

		if(m_target->m_rooted == 1)
			m_target->Root();

		//warrior talent - second wind triggers on stun and immobilize. This is not used as proc to be triggered always !
		Unit *caster = GetUnitCaster();
		if( caster != NULL )
		{
			caster->EventStunOrImmobilize( m_target );
			m_target->EventStunOrImmobilize( caster, true );
		}
		
		if( GetSpellProto()->School == SCHOOL_FROST && !m_target->asc_frozen++ )
			m_target->SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN );

		/* -Supalosa- TODO: Mobs will attack nearest enemy in range on aggro list when rooted. */
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where immunity can cancel only 1 mod and not whole spell
	{
		m_target->m_rooted--;

		if(m_target->m_rooted == 0)
			m_target->Unroot();

		if(m_target->GetTypeId() == TYPEID_UNIT)
			m_target->GetAIInterface()->AttackReaction(GetUnitCaster(), 1, 0);

		if( GetSpellProto()->School == SCHOOL_FROST && !--m_target->asc_frozen )
			m_target->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN );
	}
}

void Aura::SpellAuraModSilence(bool apply)
{
	if(apply)
	{
		m_target->m_silenced++;
		m_target->m_special_state |= UNIT_STATE_SILENCE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);

		if (m_target->GetCurrentSpell() != NULL)
			m_target->GetCurrentSpell()->cancel();
	}
	else
	{
		m_target->m_silenced--;

		if(m_target->m_silenced == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_SILENCE;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
		}
	}
}

void Aura::SpellAuraReflectSpells(bool apply)
{
	m_target->RemoveReflect( GetSpellId(), apply );

	if( apply )
	{
		ReflectSpellSchool *rss = new ReflectSpellSchool;
		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->school = -1;
		rss->require_aura_hash = 0;
		rss->charges = m_spellProto->procCharges;
		rss->infront = false;

		m_target->m_reflectSpellSchool.push_back(rss);
	}
}

void Aura::SpellAuraModStat( bool apply )
{
	int32 stat = ( int32 )mod->m_miscValue;
	int32 val;

	if( apply )
	{
		val = mod->m_amount;
		if( val < 0 )
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		val = -mod->m_amount;
	}

	if ( stat == -1 )//all stats
	{
		if( m_target->IsPlayer() )
		{
			for( uint8 x = 0; x < 5; x++ )
			{
				if( mod->m_amount > 0 )
					static_cast< Player* >( m_target )->FlatStatModPos[x] += val;
				else
					static_cast< Player* >( m_target )->FlatStatModNeg[x] -= val;

				static_cast< Player* >( m_target )->CalcStat(x);
			}

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			for( uint8 x = 0; x < 5; x++ )
			{
				static_cast< Creature* >( m_target )->FlatStatMod[x] += val;
				static_cast< Creature* >( m_target )->CalcStat(x);
			}
		}
	}
	else if( stat >= 0 )
	{
		Arcemu::Util::ARCEMU_ASSERT(    mod->m_miscValue < 5 );
		if( m_target->IsPlayer() )
		{
			if( mod->m_amount > 0 )
				static_cast< Player* >( m_target )->FlatStatModPos[ mod->m_miscValue ] += val;
			else
				static_cast< Player* >( m_target )->FlatStatModNeg[ mod->m_miscValue ] -= val;

			static_cast< Player* >( m_target )->CalcStat( mod->m_miscValue );

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			static_cast< Creature* >( m_target )->FlatStatMod[ mod->m_miscValue ] += val;
			static_cast< Creature* >( m_target )->CalcStat( mod->m_miscValue );
		}
	}
}

void Aura::SpellAuraModSkill(bool apply)
{
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			SetPositive();
			static_cast< Player* >( m_target )->_ModifySkillBonus(mod->m_miscValue, mod->m_amount);
		}
		else
			static_cast< Player* >( m_target )->_ModifySkillBonus(mod->m_miscValue, -mod->m_amount);

		static_cast< Player* >( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModIncreaseSpeed(bool apply)
{
	if(apply)
		m_target->m_speedModifier += mod->m_amount;
	else
		m_target->m_speedModifier -= mod->m_amount;

	m_target->UpdateSpeed();
}

void Aura::SpellAuraModIncreaseMountedSpeed(bool apply)
{
	if( ( GetSpellId() == 68768 || GetSpellId() == 68769 ) && p_target != NULL )
	{
		int32 newspeed = 0;

		if( p_target->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 150 )
			newspeed = 100;
		else if( p_target->_GetSkillLineCurrent( SKILL_RIDING, true ) >= 75 )
			newspeed = 60;

		mod->m_amount = newspeed; // EffectBasePoints + 1 (59+1 and 99+1)
	}

	if(apply)
	{
		SetPositive();
		m_target->m_mountedspeedModifier += mod->m_amount;
	}
	else
		m_target->m_mountedspeedModifier -= mod->m_amount;
	m_target->UpdateSpeed();
}

void Aura::SpellAuraModCreatureRangedAttackPower(bool apply)
{
	if(apply)
	{
		for(uint32 x = 0; x < 11; x++)
			if (mod->m_miscValue & (((uint32)1)<<x) )
				m_target->CreatureRangedAttackPowerMod[x+1] += mod->m_amount;
		if(mod->m_amount < 0)
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		for(uint32 x = 0; x < 11; x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->CreatureRangedAttackPowerMod[x+1] -= mod->m_amount;
			}
		}
	}
	m_target->CalcDamage();
}

void Aura::SpellAuraModDecreaseSpeed(bool apply)
{
	//there can not be 2 slow downs only most powerful is applied
	if(apply)
	{
		// Check Mechanic Immunity
		if( m_target->MechanicsDispels[MECHANIC_ENSNARED] )
		{
			m_flags |= 1 << mod->i;
			return;
		}
		switch(m_spellProto->NameHash)
		{
			case SPELL_HASH_STEALTH:			// Stealth
				SetPositive();
				break;

			case SPELL_HASH_DAZED:			// Dazed
				SetNegative();
				break;

			default:
				/* burlex: this would be better as a if(caster is hostile to target) then effect = negative) */
				if(m_casterGuid != m_target->GetGUID())
					SetNegative();
				break;
		}

		//let's check Mage talents if we proc anything
		if(m_spellProto->School==SCHOOL_FROST)
		{
			//yes we are freezing the bastard, so can we proc anything on this ?
			Unit *caster = GetUnitCaster();
			if( caster != NULL && caster->IsPlayer() )
				static_cast<Unit*>(caster)->EventChill( m_target );
			if( m_target->IsPlayer() && caster )
				static_cast<Unit*>(m_target)->EventChill( caster, true );
		}
		m_target->speedReductionMap.insert(make_pair(m_spellProto->Id, mod->m_amount));
		//m_target->m_slowdown=this;
		//m_target->m_speedModifier += mod->m_amount;
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where immunity can cancel only 1 mod and not whole spell
	{
		map< uint32, int32 >::iterator itr = m_target->speedReductionMap.find(m_spellProto->Id);
		if(itr != m_target->speedReductionMap.end())
			m_target->speedReductionMap.erase(itr);
		//m_target->m_speedModifier -= mod->m_amount;
		//m_target->m_slowdown= NULL;
	}
	if(m_target->GetSpeedDecrease())
		m_target->UpdateSpeed();
}

void Aura::UpdateAuraModDecreaseSpeed()
{
	if( m_target->MechanicsDispels[MECHANIC_ENSNARED] )
	{
		m_flags |= 1 << mod->i;
		return;
	}

	//let's check Mage talents if we proc anything
	if(m_spellProto->School==SCHOOL_FROST)
	{
		//yes we are freezing the bastard, so can we proc anything on this ?
		Unit *caster = GetUnitCaster();
		if( caster && caster->IsPlayer() )
			static_cast<Unit*>(caster)->EventChill( m_target );
		if( m_target->IsPlayer() && caster )
			static_cast<Unit*>(m_target)->EventChill( caster, true );
	}
}

void Aura::SpellAuraModIncreaseHealth(bool apply)
{
	int32 amt;

	if(apply)
	{
		//threat special cases. We should move these to scripted spells maybe
		switch(m_spellProto->Id)
		{
			case 23782:// Gift of Life
			  mod->m_amount = 1500;
			  break;
			case 12976:// Last Stand
			  mod->m_amount = (uint32)(m_target->GetMaxHealth() * 0.3);
			  break;
		}
		SetPositive();
		amt = mod->m_amount;
	}
	else
		amt =- mod->m_amount;

	if(m_target->IsPlayer())
	{
		//maybe we should not adjust hitpoints too but only maximum health
		static_cast< Player* >( m_target )->SetHealthFromSpell(static_cast< Player* >( m_target )->GetHealthFromSpell() + amt);
		static_cast< Player* >( m_target )->UpdateStats();
		if(apply)
			m_target->ModHealth(amt);
		else
		{
			if((int32)m_target->GetHealth()>-amt)//watch it on remove value is negative
				m_target->ModHealth(amt);
			else m_target->SetHealth(1); //do not kill player but do strip him good
		}
	}
	else
		m_target->ModMaxHealth(amt);
}

void Aura::SpellAuraModIncreaseEnergy(bool apply)
{
	SetPositive();
	//uint32 powerField,maxField ;
	//uint8 powerType = m_target->GetPowerType();

	/*if(powerType == POWER_TYPE_MANA) // Mana
	{
		powerField = UNIT_FIELD_POWER1;
		maxField = UNIT_FIELD_MAXPOWER1;
	}
	else if(powerType == POWER_TYPE_RAGE) // Rage
	{
		powerField = UNIT_FIELD_POWER2;
		maxField = UNIT_FIELD_MAXPOWER2;
	}
	else if(powerType == POWER_TYPE_ENERGY) // Energy
	{
		powerField = UNIT_FIELD_POWER4;
		maxField = UNIT_FIELD_MAXPOWER4;
	}
	else // Capt: if we can not use identify the type: do nothing
		return; */

	int32 amount = apply ? mod->m_amount : -mod->m_amount;
	m_target->ModMaxPower(mod->m_miscValue, amount);
	m_target->ModPower(mod->m_miscValue, amount);

	if(mod->m_miscValue == 0 && m_target->GetTypeId() == TYPEID_PLAYER)
	{
		static_cast< Player* >( m_target )->SetManaFromSpell(static_cast< Player* >( m_target )->GetManaFromSpell() + amount);
	}
}

void Aura::SpellAuraModShapeshift(bool apply)
{
	if( p_target == NULL )
		return;

	if( p_target->m_MountSpellId && p_target->m_MountSpellId != m_spellProto->Id )
		if( !(mod->m_miscValue & ( FORM_BATTLESTANCE | FORM_DEFENSIVESTANCE | FORM_BERSERKERSTANCE ) ) )
			p_target->Dismount(); // these spells are not compatible

	SpellShapeshiftForm * ssf = dbcSpellShapeshiftForm.LookupEntry( mod->m_miscValue );
	if( !ssf )
		return;

	uint32 spellId = 0;
	uint32 spellId2 = 0;
	uint32 modelId = (uint32)(apply ? ssf->modelId : 0);

	bool freeMovements = false;

	switch( ssf->id )
	{
	case FORM_CAT:
		{//druid
			freeMovements = true;
			spellId = 3025;
			if(apply)
			{
                m_target->SetPowerType( POWER_TYPE_ENERGY );
				m_target->SetMaxPower( POWER_TYPE_ENERGY, 100 );//100 Energy
				m_target->SetPower( POWER_TYPE_ENERGY, 0 );//0 Energy
				if(m_target->getRace() != RACE_NIGHTELF)//TAUREN
					modelId = 8571;

			}
			else
			{//turn back to mana
				//m_target->SetBaseAttackTime(MELEE,oldap);
                m_target->SetPowerType( POWER_TYPE_MANA );
				if(m_target->m_stealth)
				{
					uint32 sp = m_target->m_stealth;
					m_target->m_stealth = 0;
					m_target->RemoveAura(sp);//prowl
				}
			}
			static_cast< Player* >( m_target )->UpdateAttackSpeed();

		} break;
	case FORM_TREE:
		{
			freeMovements = true;
			spellId = 34123; // this is area aura
			//spellId2 = 5420;
		} break;
	case FORM_TRAVEL:
		{//druid
			freeMovements = true;
			spellId = 5419;
		} break;
	case FORM_AQUA:
		{//druid aqua
			freeMovements = true;
			spellId = 5421;
		} break;
	case FORM_BEAR:
		{//druid only
			freeMovements = true;
			spellId = 1178;
			if(apply)
			{
				m_target->SetPowerType( POWER_TYPE_RAGE);
				m_target->SetMaxPower( POWER_TYPE_RAGE, 1000);
				m_target->SetPower( POWER_TYPE_RAGE, 0);//0 rage

				if( m_target->getRace() != RACE_NIGHTELF ) //TAUREN
					modelId = 2289;

				//some say there is a second effect
				SpellEntry* spellInfo = dbcSpell.LookupEntry( 21178 );

				Spell *sp = new Spell(m_target, spellInfo, true, NULL);
				SpellCastTargets tgt;
				tgt.m_unitTarget = m_target->GetGUID();
				sp->prepare( &tgt );
			}
			else
			{//reset back to mana
				m_target->SetPowerType( POWER_TYPE_MANA);
				m_target->RemoveAura( 21178 ); // remove Bear Form (Passive2)
			}
		} break;
	case FORM_DIREBEAR:
		{//druid only
			freeMovements = true;
			spellId = 9635;
			if(apply)
			{
				m_target->SetPowerType( POWER_TYPE_RAGE);
				m_target->SetMaxPower( POWER_TYPE_RAGE, 1000);
				m_target->SetPower( POWER_TYPE_RAGE, 0);//0 rage
				if( m_target->getRace() != RACE_NIGHTELF ) //TAUREN
					modelId = 2289;
			}
			else //reset back to mana
				m_target->SetPowerType( POWER_TYPE_MANA);
		} break;
	case FORM_BATTLESTANCE:
		{
			spellId = 21156;
		} break;
	case FORM_DEFENSIVESTANCE:
		{
			spellId = 7376;
		} break;
	case FORM_BERSERKERSTANCE:
		{
			spellId = 7381;
		} break;
	case FORM_SHADOW:
		{
			if(apply)
			{
                static_cast< Player* >( m_target )->SendSpellCooldownEvent( m_spellProto->Id );
			}
			spellId = 49868;
		}break;
	case FORM_FLIGHT:
		{// druid
			freeMovements = true;
			spellId = 33948;
			if(apply)
			{
				if(m_target->getRace() != RACE_NIGHTELF)
					modelId = 20872;
			}
		}break;
	case FORM_STEALTH:
		{// rogue
			if (!m_target->m_can_stealth)
				return;
			//m_target->UpdateVisibility();
		} break;
	case FORM_MOONKIN:
		{//druid
			freeMovements = true;
			spellId = 24905;
			if(apply)
			{
				if(m_target->getRace() != RACE_NIGHTELF)
					modelId = ssf->modelId2; // Lol, why is this the only one that has it in ShapeShift DBC? =/ lameeee...
			}
		}break;
	case FORM_SWIFT: //not tested yet, right now going on trust
		{// druid
			freeMovements = true;
			spellId = 40121; //Swift Form Passive
			if(apply)
			{
				if(m_target->getRace() != RACE_NIGHTELF)//TAUREN
					modelId = 21244;
			}
		}break;
	case FORM_SPIRITOFREDEMPTION:
		{
			spellId = 27795;
			modelId = 12824; // Smaller spirit healer, heehee :3
		}break;
	case FORM_GHOUL:
	case FORM_SKELETON:
	case FORM_ZOMBIE:
		{
			p_target->SendAvailSpells(ssf, apply);
		}break;
	case FORM_METAMORPHOSIS:
		{
			spellId = 59673;
		}break;
	}

	if( apply )
	{
		if( m_target->getClass() == WARRIOR && m_target->GetPower( POWER_TYPE_RAGE ) > p_target->m_retainedrage )
			m_target->SetPower( POWER_TYPE_RAGE, p_target->m_retainedrage );

		if( m_target->getClass() == DRUID )
		{
			if( Rand( p_target->m_furorChance ) )
			{
				uint32 furorSpell;
				if( mod->m_miscValue == FORM_CAT )
					furorSpell = 17099;
				else if( mod->m_miscValue == FORM_BEAR || mod->m_miscValue == FORM_DIREBEAR )
					furorSpell = 17057;
				else
					furorSpell = 0;

				if( furorSpell != 0 )
				{
					SpellEntry *spellInfo = dbcSpell.LookupEntry( furorSpell );

					Spell *sp = new Spell(m_target, spellInfo, true, NULL);
					SpellCastTargets tgt;
					tgt.m_unitTarget = m_target->GetGUID();
					sp->prepare(&tgt);
				}
			}
		}

		if( spellId != GetSpellId() )
		{
			if( p_target->m_ShapeShifted )
				p_target->RemoveAura( p_target->m_ShapeShifted );

			p_target->m_ShapeShifted = GetSpellId();
		}

		if( modelId != 0 )
		{
			m_target->SetDisplayId(modelId );
			m_target->EventModelChange();
		}

		p_target->SetShapeShift( static_cast<uint8>( mod->m_miscValue ));

		// check for spell id
		if( spellId == 0 )
			return;

		SpellEntry* spellInfo = dbcSpell.LookupEntry(spellId );

		Spell *sp = new Spell(m_target, spellInfo, true, NULL);
		SpellCastTargets tgt;
		tgt.m_unitTarget = m_target->GetGUID();
		sp->prepare( &tgt );

		if( spellId2 != 0 )
		{
			spellInfo = dbcSpell.LookupEntry(spellId2);
			sp = new Spell(m_target, spellInfo, true, NULL);
			sp->prepare(&tgt);
		}

		// remove the caster from impairing movements
		if( freeMovements )
		{
			for( uint32 x = MAX_POSITIVE_AURAS_EXTEDED_START; x < MAX_POSITIVE_AURAS_EXTEDED_END; x++ )
			{
				if( m_target->m_auras[x] != NULL )
				{
					if( m_target->m_auras[x]->GetSpellProto()->MechanicsType == MECHANIC_ROOTED || m_target->m_auras[x]->GetSpellProto()->MechanicsType == MECHANIC_ENSNARED ) // Remove roots and slow spells
					{
						m_target->m_auras[x]->Remove();
					}
					else // if got immunity for slow, remove some that are not in the mechanics
					{
						for( int i = 0; i < 3; i++ )
						{
							if( m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED || m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT )
							{
								m_target->m_auras[x]->Remove();
								break;
							}
						}
					}
			   }
			}
		}

		//execute after we changed shape
		p_target->EventTalentHearthOfWildChange( true );
	}
	else
	{
		if( ssf->id != FORM_STEALTH )
			m_target->RemoveAllAurasByRequiredShapeShift( DecimalToMask(mod->m_miscValue) );

		if( m_target->IsCasting() && m_target->m_currentSpell && m_target->m_currentSpell->GetProto() 
			&& ( m_target->m_currentSpell->GetProto()->RequiredShapeShift & DecimalToMask(mod->m_miscValue) ) )
			m_target->InterruptSpell();

		//execute before changing shape back
		p_target->EventTalentHearthOfWildChange( false );
		m_target->SetDisplayId(m_target->GetNativeDisplayId() );
		m_target->EventModelChange();
		if( spellId != GetSpellId() )
		{
			if( spellId )
				m_target->RemoveAura( spellId );
		}
		p_target->m_ShapeShifted = 0;
		p_target->SetShapeShift( 0 );
	}
	p_target->UpdateStats();
}

void Aura::SpellAuraModEffectImmunity(bool apply)
{
	if( m_spellProto->Id == 24937 )
		SetPositive();

	if (!apply)
	{
		if( m_spellProto->Id == 23333 || m_spellProto->Id == 23335 || m_spellProto->Id == 34976 )
		{
			Player* plr = GetPlayerCaster();
			if( plr == NULL || plr->m_bg == NULL)
				return;

			plr->m_bg->HookOnFlagDrop(plr);

		}
	}
}

void Aura::SpellAuraModStateImmunity(bool apply)
{
	//%50 chance to dispel 1 magic effect on target
	//23922
}

void Aura::SpellAuraModSchoolImmunity(bool apply)
{
	if( apply && ( m_spellProto->NameHash == SPELL_HASH_DIVINE_SHIELD || m_spellProto->NameHash == SPELL_HASH_ICE_BLOCK) ) // Paladin - Divine Shield
	{
		if(!m_target->isAlive())
			return;

		Aura * pAura;
		for(uint32 i = MAX_NEGATIVE_AURAS_EXTEDED_START; i < MAX_NEGATIVE_AURAS_EXTEDED_END; ++i)
		{
			pAura = m_target->m_auras[i];
			if( pAura != this && pAura != NULL && !pAura->IsPassive() && !pAura->IsPositive() && !(pAura->GetSpellProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY) )
			{
				pAura->Remove();
			}
		}
	}

	if( apply && ( m_spellProto->NameHash == SPELL_HASH_DIVINE_SHIELD || m_spellProto->NameHash == SPELL_HASH_BLESSING_OF_PROTECTION || m_spellProto->NameHash == SPELL_HASH_ICE_BLOCK) )
	{
		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_INVINCIBLE);
	}

	if(apply)
	{
		//fix me may be negative
		Unit * c = GetUnitCaster();
		if(c)
		{
			if(isAttackable(c,m_target))
				SetNegative();
			else SetPositive();
		}else
			SetPositive();

		sLog.outDebug("%s: value=%x", __FUNCTION__, mod->m_miscValue);
		for( uint8 i = 0; i < SCHOOL_COUNT; i++ )
		{
			if( mod->m_miscValue & ( 1 << i ) )
			{
				m_target->SchoolImmunityList[i]++;
				m_target->RemoveAurasOfSchool( i, false, true );
			}
		}
	}
	else
	{
		for( uint8 i = 0; i < SCHOOL_COUNT; i++ )
		{
			if( mod->m_miscValue & ( 1 << i ) &&
				m_target->SchoolImmunityList[i] > 0 ) 
			{
				m_target->SchoolImmunityList[i]--;
			}
		}
	}
}

void Aura::SpellAuraModDmgImmunity(bool apply)
{

}

void Aura::SpellAuraModDispelImmunity(bool apply)
{
	Arcemu::Util::ARCEMU_ASSERT(   mod->m_miscValue < 10);
	if(apply)
		m_target->dispels[mod->m_miscValue]++;
	else
		m_target->dispels[mod->m_miscValue]--;

	if(apply)
	{
		for(uint32 x=MAX_POSITIVE_AURAS_EXTEDED_START;x<MAX_POSITIVE_AURAS_EXTEDED_END;x++)
		{
            // HACK FIX FOR: 41425 and 25771
			if(m_target->m_auras[x] && m_target->m_auras[x]->GetSpellId() != 41425 && m_target->m_auras[x]->GetSpellId() != 25771)
				if(m_target->m_auras[x]->GetSpellProto()->DispelType==(uint32)mod->m_miscValue)
					m_target->m_auras[x]->Remove();
		}
	}
}

void Aura::SpellAuraProcTriggerSpell(bool apply)
{
	if(apply)
	{
		uint32 groupRelation[3];
		int charges;
		uint32 spellId;

		// Find spell of effect to be triggered
		spellId = GetSpellProto()->EffectTriggerSpell[mod->i];
		if( spellId == 0 )
		{
			sLog.outDebug("Warning! trigger spell is null for spell %u", GetSpellProto()->Id);
			return;
		}

		// Initialize mask
		groupRelation[0] = GetSpellProto()->EffectSpellClassMask[mod->i][0];
		groupRelation[1] = GetSpellProto()->EffectSpellClassMask[mod->i][1];
		groupRelation[2] = GetSpellProto()->EffectSpellClassMask[mod->i][2];

		// Initialize charges
		charges = GetSpellProto()->procCharges;
		Unit* ucaster = GetUnitCaster();
		if( ucaster != NULL && GetSpellProto()->SpellGroupType )
		{
			SM_FIValue( ucaster->SM_FCharges, &charges, GetSpellProto()->SpellGroupType );
			SM_PIValue( ucaster->SM_PCharges, &charges, GetSpellProto()->SpellGroupType );
		}

		m_target->AddProcTriggerSpell(spellId, GetSpellProto()->Id, m_casterGuid, GetSpellProto()->procChance, GetSpellProto()->procFlags, charges, groupRelation, NULL);

		sLog.outDebug("%u is registering %u chance %u flags %u charges %u triggeronself %u interval %u",GetSpellProto()->Id,spellId,GetSpellProto()->procChance,GetSpellProto()->procFlags & ~PROC_TARGET_SELF,charges,GetSpellProto()->procFlags & PROC_TARGET_SELF,GetSpellProto()->proc_interval);
	}
	else
	{
		// Find spell of effect to be triggered
		uint32 spellId = GetSpellProto()->EffectTriggerSpell[mod->i];
		if( spellId == 0 )
		{
			sLog.outDebug("Warning! trigger spell is null for spell %u", GetSpellProto()->Id);
			return;
		}

		m_target->RemoveProcTriggerSpell(spellId, m_casterGuid);
	}
}

void Aura::SpellAuraProcTriggerDamage(bool apply)
{
	if(apply)
	{
		DamageProc ds;
		ds.m_damage = mod->m_amount;
		ds.m_spellId = GetSpellProto()->Id;
		ds.m_school = GetSpellProto()->School;
		ds.m_flags = m_spellProto->procFlags;
		ds.owner = (void*)this;
		m_target->m_damageShields.push_back(ds);
		sLog.outDebug("registering dmg proc %u, school %u, flags %u, charges at least %u",ds.m_spellId,ds.m_school,ds.m_flags,m_spellProto->procCharges);
	}
	else
	{
		for(std::list<struct DamageProc>::iterator i = m_target->m_damageShields.begin();i != m_target->m_damageShields.end();i++)
		{
			if(i->owner == this)
			{
				m_target->m_damageShields.erase(i);
				break;
			}
		}
	}
}

void Aura::SpellAuraTrackCreatures(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			if( p_target->TrackingSpell != 0 )
				p_target->RemoveAura( p_target->TrackingSpell );

			p_target->SetUInt32Value( PLAYER_TRACK_CREATURES,(uint32)1 << ( mod->m_miscValue - 1 ) );
			p_target->TrackingSpell = GetSpellId();
		}
		else
		{
			p_target->TrackingSpell = 0;
			p_target->SetUInt32Value( PLAYER_TRACK_CREATURES, 0 );
		}
	}
}

void Aura::SpellAuraTrackResources(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			if( p_target->TrackingSpell != 0 )
				p_target->RemoveAura( p_target->TrackingSpell );

		p_target->SetUInt32Value( PLAYER_TRACK_RESOURCES, (uint32)1 << ( mod->m_miscValue - 1 ) );
		p_target->TrackingSpell = GetSpellId();
		}
		else
		{
			p_target->TrackingSpell = 0;
			p_target->SetUInt32Value( PLAYER_TRACK_RESOURCES, 0 );
		}
	}
}

void Aura::SpellAuraModParryPerc(bool apply)
{
	//if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt;
		if( apply )
		{
			amt = mod->m_amount;
			if( amt < 0 )
				SetNegative();
			else
				SetPositive();

		}
		else
			amt = -mod->m_amount;

		m_target->SetParryFromSpell( m_target->GetParryFromSpell() + amt );
		if( p_target != NULL )
		{
			p_target->UpdateChances();
		}
	}
}

void Aura::SpellAuraModDodgePerc(bool apply)
{
	//if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt = mod->m_amount;
//		SM_FIValue(m_target->SM_FSPELL_VALUE, &amt, GetSpellProto()->SpellGroupType);
		if( apply )
		{
			if( amt < 0 )
				SetNegative();
			else
				SetPositive();
		}
		else
			amt = -amt;

		m_target->SetDodgeFromSpell( m_target->GetDodgeFromSpell() + amt );
		if( p_target != NULL )
		{
			p_target->UpdateChances();
		}
	}
}

void Aura::SpellAuraModBlockPerc(bool apply)
{
	//if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt;
		if( apply )
		{
			amt = mod->m_amount;
			if( amt < 0 )
				SetNegative();
			else
				SetPositive();
		}
		else
			amt = -mod->m_amount;

		m_target->SetBlockFromSpell( m_target->GetBlockFromSpell() + amt );
		if( p_target != NULL )
		{
			p_target->UpdateStats();
		}
	}
}

void Aura::SpellAuraModCritPerc(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			WeaponModifier md;
			md.value = float( mod->m_amount );
			md.wclass = GetSpellProto()->EquippedItemClass;
			md.subclass = GetSpellProto()->EquippedItemSubClass;
			p_target->tocritchance.insert( make_pair( GetSpellId(), md ) );
		}
		else
		{
			/*std::list<WeaponModifier>::iterator i = static_cast< Player* >( m_target )->tocritchance.begin();

			for(;i!=static_cast< Player* >( m_target )->tocritchance.end();i++)
			{
				if((*i).spellid==GetSpellId())
				{
					static_cast< Player* >( m_target )->tocritchance.erase(i);
					break;
				}
			}*/
			p_target->tocritchance.erase( GetSpellId() );
		}
		p_target->UpdateChances();
	}
}

void Aura::SpellAuraPeriodicLeech(bool apply)
{
	if(apply)
	{
		SetNegative();
		uint32 amt = mod->m_amount;
		sEventMgr.AddEvent(this, &Aura::EventPeriodicLeech,amt,
			EVENT_AURA_PERIODIC_LEECH,	GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicLeech(uint32 amount)
{
	Unit* m_caster = GetUnitCaster();
	
	if( m_caster == NULL )
		return;

	if( ! ( m_target->isAlive() && m_caster->isAlive() ) )
		return;

	SpellEntry* sp = GetSpellProto();

	if( m_target->SchoolImmunityList[sp->School] )
	{
		SendTickImmune(m_target, m_caster);
		return;
	}

	bool is_critical = false;
	uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION;
	uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_SPELL_HIT_VICTIM;

	int amp = sp->EffectAmplitude[mod->i];
	if( !amp )
		amp = event_GetEventPeriod(EVENT_AURA_PERIODIC_LEECH);

	int32 bonus = 0;

	if( GetDuration() )
	{
		float fbonus = m_caster->GetSpellDmgBonus(m_target, sp, amount, true) * 0.5f;
		if( fbonus < 0 ) 
			fbonus = 0.0f;
		bonus = float2int32( fbonus * amp / GetDuration() );
	}

	amount += bonus;

	if( sp->SpellGroupType )
	{
		SM_FIValue( m_caster->SM_FDOT, (int32*)&amount, sp->SpellGroupType );
		SM_PIValue( m_caster->SM_PDOT, (int32*)&amount, sp->SpellGroupType );
	}

	if( DotCanCrit() )
	{
		is_critical = m_caster->IsCriticalDamageForSpell( m_target, sp );

		if( is_critical )
		{
			amount = float2int32( m_caster->GetCriticalDamageBonusForSpell(m_target, sp, (float) amount) );

			aproc |= PROC_ON_SPELL_CRIT_HIT;
			vproc |= PROC_ON_SPELL_CRIT_HIT_VICTIM;
		}
	}

	amount = (uint32) min( amount, m_target->GetUInt32Value(UNIT_FIELD_HEALTH) );

	// Apply bonus from [Warlock] Soul Siphon
	if (m_caster->m_soulSiphon.amt) {
		// Use std::map to prevent counting duplicate auras (stacked ones, from the same unit)
		std::map<uint64, std::set<uint32> *> auras;
		std::map<uint64, std::set<uint32> *>::iterator itx, itx2;
		int32 pct;
		int32 count= 0;

		auras.clear();
		for(uint32 x=MAX_NEGATIVE_AURAS_EXTEDED_START;x<MAX_NEGATIVE_AURAS_EXTEDED_END;x++)
		{
			if( m_target->m_auras[x] == NULL)
				continue;

			Aura *aura = m_target->m_auras[x];
			if( aura->GetSpellProto()->SpellFamilyName != 5 )
				continue;

			skilllinespell *sk = objmgr.GetSpellSkill( aura->GetSpellId() );
			if( sk == NULL || sk->skilline != SKILL_AFFLICTION )
				continue;

			itx = auras.find( aura->GetCasterGUID() );
			if( itx == auras.end() )
			{
				std::set<uint32> *ids = new std::set<uint32>;
				auras.insert( make_pair(aura->GetCasterGUID(), ids) );
				itx = auras.find( aura->GetCasterGUID() );
			}

			std::set<uint32> *ids = itx->second;
			if( ids->find( aura->GetSpellId() ) == ids->end() )
			{
				ids->insert( aura->GetSpellId() );
			}
		}

		if( auras.size() )
		{
			itx = auras.begin();
			while( itx != auras.end() )
			{
				itx2 = itx++;
				count += (int32)itx2->second->size();
				delete itx2->second;
			}
		}

		pct = count * m_caster->m_soulSiphon.amt;
		if ( pct > m_caster->m_soulSiphon.max )
			pct = m_caster->m_soulSiphon.max;
		amount += amount * pct / 100;
	}

	uint32 dmg_amount  = amount;
	uint32 heal_amount = float2int32(amount * sp->EffectMultipleValue[mod->i]);

	uint32 newHealth = m_caster->GetHealth() + heal_amount;

	uint32 mh = m_caster->GetMaxHealth();
	if( newHealth <= mh )
		m_caster->SetHealth(newHealth);
	else
		m_caster->SetHealth(mh);

	m_target->SendPeriodicHealAuraLog( m_caster->GetNewGUID(), m_caster->GetNewGUID(), sp->Id, heal_amount, 0, false );
	m_target->SendPeriodicAuraLog( m_target->GetNewGUID(), m_target->GetNewGUID(), sp->Id, sp->School, heal_amount, 0, 0, FLAG_PERIODIC_LEECH, is_critical);

	//deal damage before we add healing bonus to damage
	m_caster->DealDamage( m_target, dmg_amount, 0, 0, sp->Id, true );
	m_caster->SendSpellNonMeleeDamageLog( m_caster, m_target, sp->Id, dmg_amount, (uint8) sp->School, 0, 0, true, 0, is_critical, true );
 
	m_caster->HandleProc( aproc, m_target, sp, false, dmg_amount );
	m_caster->m_procCounter = 0;

	//some say this prevents some crashes atm
	if( !m_target->isAlive() )
		return;

	m_target->HandleProc( vproc, m_caster, sp, false, dmg_amount);
	m_target->m_procCounter = 0;

	m_target->RemoveAurasByHeal();
}

void Aura::SendTickImmune(Unit * target, Unit *caster)
{
    WorldPacket data(SMSG_SPELLORDAMAGE_IMMUNE, 21);
    data << ( caster ? caster->GetGUID() : target->GetGUID() );
    data << target->GetGUID();
    data << GetSpellProto()->Id;
    data << uint8(1);
    target->SendMessageToSet(&data,true);
}

void Aura::SpellAuraModHitChance(bool apply)
{
	if (!m_target->IsUnit()) return;

	int32 val = mod->m_amount;

	Unit *c = GetUnitCaster();
	if (c && GetSpellProto()->SpellGroupType) {
		SM_FIValue(c->SM_FMiscEffect,&val,GetSpellProto()->SpellGroupType);
		SM_PIValue(c->SM_PMiscEffect,&val,GetSpellProto()->SpellGroupType);
	}

	if (apply)
	{
		static_cast< Unit* >( m_target )->SetHitFromMeleeSpell( static_cast< Unit* >( m_target )->GetHitFromMeleeSpell() + val);
		if(val<0)
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		static_cast< Unit* >( m_target )->SetHitFromMeleeSpell( static_cast< Unit* >( m_target )->GetHitFromMeleeSpell() - val);
		if( static_cast< Unit* >( m_target )->GetHitFromMeleeSpell() < 0 )
		{
			static_cast< Unit* >( m_target )->SetHitFromMeleeSpell( 0 );
		}
	}
}

void Aura::SpellAuraModSpellHitChance(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			p_target->SetHitFromSpell( p_target->GetHitFromSpell() + mod->m_amount );
			if( mod->m_amount < 0 )
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			p_target->SetHitFromSpell( p_target->GetHitFromSpell() - mod->m_amount );
			if( p_target->GetHitFromSpell() < 0 )
			{
				p_target->SetHitFromSpell(0);
			}
		}
	}
}

void Aura::SpellAuraTransform(bool apply)
{
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummyAura(GetSpellId(), mod->i, this, apply))
		return;

	uint32 displayId = 0;
	CreatureInfo* ci = CreatureNameStorage.LookupEntry(mod->m_miscValue);

	if(ci)
		displayId = ci->Male_DisplayID;

	if( p_target != NULL )
		p_target->Dismount();

   // SetPositive();
	switch( GetSpellProto()->Id )
	{
		case 20584://wisp
			m_target->SetDisplayId(apply ? 10045:m_target->GetNativeDisplayId() );
		break;

		case 30167: // Red Ogre Costume
		{
			if( apply )
				m_target->SetDisplayId(11549 );
			else
				m_target->SetDisplayId(m_target->GetNativeDisplayId() );
		}break;

		case 41301: // Time-Lost Figurine
		{
			if( apply )
				m_target->SetDisplayId(18628 );
			else
				m_target->SetDisplayId(m_target->GetNativeDisplayId() );
		}break;

		case 16739: // Orb of Deception
		{
			if( apply )
			{
				if(m_target->getRace() == RACE_ORC)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10139);
					else
						m_target->SetDisplayId(10140);
				}
				if(m_target->getRace() == RACE_TAUREN)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10136);
					else
						m_target->SetDisplayId(10147);
				}
				if(m_target->getRace() == RACE_TROLL)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10135);
					else
						m_target->SetDisplayId(10134);
				}
				if(m_target->getRace() == RACE_UNDEAD)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10146);
					else
						m_target->SetDisplayId(10145);
				}
				if(m_target->getRace() == RACE_BLOODELF)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(17829);
					else
						m_target->SetDisplayId(17830);
				}

				if(m_target->getRace() == RACE_GNOME)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10148);
					else
						m_target->SetDisplayId(10149);
				}
				if(m_target->getRace() == RACE_DWARF)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10141);
					else
						m_target->SetDisplayId(10142);
				}
				if(m_target->getRace() == RACE_HUMAN)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10137);
					else
						m_target->SetDisplayId(10138);
				}
				if(m_target->getRace() == RACE_NIGHTELF)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(10143);
					else
						m_target->SetDisplayId(10144);
				}
				if(m_target->getRace() == RACE_DRAENEI)
				{
					if( m_target->getGender() == 0 )
						m_target->SetDisplayId(17827);
					else
						m_target->SetDisplayId(17828);
				}
			}
			else
				m_target->SetDisplayId(m_target->GetNativeDisplayId() );
		}break;

		case 42365:	// murloc costume
			m_target->SetDisplayId(apply ? 21723 : m_target->GetNativeDisplayId() );
			break;

		case 118://polymorph
		case 851:
		case 5254:
		case 12824:
		case 12825:
		case 12826:
		case 13323:
		case 15534:
		case 22274:
		case 23603:
		case 28270:	 // Polymorph: Cow
		case 28271:	 // Polymorph: Turtle
		case 28272:	 // Polymorph: Pig
		case 61025:  // Polymorph: Serpent
		case 61305:  // Polymorph: Black Cat
		case 61721:  // Polymorph: Rabbit
		case 61780:  // Polymorph: Turkey
			{
				if(!displayId)
				{
					switch(GetSpellProto()->Id)
					{
					case 28270:	 // Cow
						displayId = 1060;
						break;

					case 28272:	 // Pig
						displayId = 16356 + RandomUInt(2);
						break;

					case 28271:	 // Turtle
						displayId = 16359 + RandomUInt(2);
						break;

					default:
						displayId = 856;
						break;

					}
				}

				if(apply)
				{
					Unit* caster = GetUnitCaster();
					if (caster != NULL && m_target->GetTypeId() == TYPEID_UNIT)
						m_target->GetAIInterface()->AttackReaction(caster, 1, GetSpellId());

					m_target->SetDisplayId(displayId);

					// remove the current spell (for channalers)
					if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid &&
						m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
					{
						m_target->m_currentSpell->cancel();
						m_target->m_currentSpell = 0;
					}

					sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1,(uint32)1000,EVENT_AURA_PERIODIC_HEAL,1000,0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
					m_target->polySpell = GetSpellProto()->Id;
				}
				else
				{
					m_target->SetDisplayId(m_target->GetNativeDisplayId());
					m_target->polySpell = 0;
				}
			}break;

		case 19937:
			{
				if (apply)
				{
					// TODO: Sniff the spell / item, we need to know the real displayID
					// guessed this may not be correct
					// human = 7820
					// dwarf = 7819
					// Halfling = 7818
					// maybe 7842 as its from a lesser npc
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, 7842);
				}
				else
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, m_target->GetNativeDisplayId());
				}
			}break;

		default:
		{
			if(!displayId) return;

			if (apply)
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, displayId);
				}
				else
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, m_target->GetNativeDisplayId());
				}
		}break;
	};

	m_target->EventModelChange();
}

void Aura::SpellAuraModSpellCritChance(bool apply)
{
	if( p_target != NULL )
	{
		int32 amt;
		if( apply )
		{
			amt = mod->m_amount;
			if( amt < 0 )
				SetNegative();
			else
				SetPositive();
		}
		else
			amt = -mod->m_amount;

		p_target->spellcritperc += amt;
		p_target->SetSpellCritFromSpell( p_target->GetSpellCritFromSpell() + amt );
		p_target->UpdateChanceFields();
	}
}

void Aura::SpellAuraIncreaseSwimSpeed(bool apply)
{
	if(apply)
	{
		if(m_target->isAlive())  SetPositive();
		m_target->m_swimSpeed = 0.04722222f*(100+mod->m_amount);
	}
	else
		m_target->m_swimSpeed = PLAYER_NORMAL_SWIM_SPEED;
	if( p_target != NULL )
	{
		WorldPacket data( SMSG_FORCE_SWIM_SPEED_CHANGE, 17 );
		data << p_target->GetNewGUID();
		data << (uint32)2;
		data << m_target->m_swimSpeed;
		p_target->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraModCratureDmgDone(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			for( uint8 x = 0; x < 11; x++ )
				if( mod->m_miscValue & ( (uint32)1 << x ) )
					p_target->IncreaseDamageByType[x+1] += mod->m_amount;

			mod->m_amount < 0 ? SetNegative() : SetPositive();
		}
		else
			for( uint8 x = 0; x < 11; x++ )
				if( mod->m_miscValue & (((uint32)1) << x ) )
					p_target->IncreaseDamageByType[x+1] -= mod->m_amount;
	}
}

void Aura::SpellAuraPacifySilence(bool apply)
{
	// Can't Attack or Cast Spells
	if(apply)
	{
		if(m_spellProto->Id == 24937)
			SetPositive();
		else
			SetNegative();

		m_target->m_pacified++;
		m_target->m_silenced++;
		m_target->m_special_state |= UNIT_STATE_PACIFY | UNIT_STATE_SILENCE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);

		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid &&
			m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
			{
				m_target->m_currentSpell->cancel();
				m_target->m_currentSpell = 0;
			}
	}
	else
	{
		m_target->m_pacified--;

		if(m_target->m_pacified == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_PACIFY;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
		}

		m_target->m_silenced--;

		if(m_target->m_silenced == 0)
		{
			m_target->m_special_state &= ~UNIT_STATE_SILENCE;
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
		}
	}
}

void Aura::SpellAuraModScale(bool apply)
{
	float current = m_target->GetScale();
	float delta = mod->m_amount/100.0f;

	m_target->SetScale(  apply ? (current+current*delta) : current/(1.0f+delta));
}

void Aura::SpellAuraPeriodicHealthFunnel(bool apply)
{
	if(apply)
	{
		uint32 amt = mod->m_amount;
		sEventMgr.AddEvent(this, &Aura::EventPeriodicHealthFunnel, amt,
			EVENT_AURA_PERIODIC_HEALTH_FUNNEL, GetSpellProto()->EffectAmplitude[mod->i], 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicHealthFunnel(uint32 amount)
{
	//Blood Siphon
	//Deals 200 damage per second.
	//Feeding Hakkar 1000 health per second.
	Unit* m_caster = GetUnitCaster();
	if(!m_caster)
		return;
	if(m_target->isAlive() && m_caster->isAlive())
	{

		m_caster->DealDamage(m_target, amount, 0, 0, GetSpellId(),true);
		uint32 newHealth = m_caster->GetHealth() + 1000;

		uint32 mh = m_caster->GetMaxHealth();
		if(newHealth <= mh)
			m_caster->SetHealth( newHealth);
		else
			m_caster->SetHealth( mh);

		m_target->SendPeriodicAuraLog( m_target->GetNewGUID(), m_target->GetNewGUID(), m_spellProto->Id, m_spellProto->School, 1000, 0, 0, FLAG_PERIODIC_LEECH, false);

		m_caster->RemoveAurasByHeal();
	}
}

void Aura::SpellAuraPeriodicManaLeech(bool apply)
{
	if( apply )
	{
		uint32 amt = mod->m_amount;
		uint32 mult = amt;

		amt = mult * m_target->GetMaxPower( POWER_TYPE_MANA ) / 100;
		
		Unit* caster = GetUnitCaster();
		if( caster != NULL )
		{
			if( amt > caster->GetMaxPower( POWER_TYPE_MANA ) * ( mult << 1 ) / 100 ) 
				amt = caster->GetMaxPower(POWER_TYPE_MANA) * ( mult << 1 ) / 100;
		}
		sEventMgr.AddEvent( this, &Aura::EventPeriodicManaLeech, amt,
			EVENT_AURA_PERIODIC_LEECH, GetSpellProto()->EffectAmplitude[mod->i], 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	}
}

void Aura::EventPeriodicManaLeech(uint32 amount)
{
	Unit* m_caster = GetUnitCaster();
	if(!m_caster)
		return;
	if(m_target->isAlive() && m_caster->isAlive())
	{

		int32 amt = (int32)min( amount, m_target->GetPower( POWER_TYPE_MANA ) );
		uint32 cm = m_caster->GetPower( POWER_TYPE_MANA ) + amt;
		uint32 mm = m_caster->GetMaxPower( POWER_TYPE_MANA );
		if(cm <= mm)
			m_caster->SetPower( POWER_TYPE_MANA, cm);
		else
			m_caster->SetPower( POWER_TYPE_MANA, mm);
		m_target->ModPower( POWER_TYPE_MANA, -amt);
	}
}

void Aura::SpellAuraModCastingSpeed(bool apply)
{
	float current = m_target->GetCastSpeedMod() ;
	if( apply )
		current -= mod->m_amount / 100.0f;
	else
		current += mod->m_amount / 100.0f;

	m_target->SetCastSpeedMod(current );
}

bool isFeignDeathResisted( uint32 playerlevel, uint32 moblevel ){
	int fMobRes = 0;
	int diff = 0;
	
	if( playerlevel < moblevel ){
		diff = moblevel - playerlevel;

		if( diff <= 2 )
			fMobRes = diff + 4;
		else
			fMobRes = ( diff - 2 ) * 11 + 6;
		
		if( fMobRes > 100 )
			fMobRes = 100;
		
		if( ( rand() % 100 ) < fMobRes )
			return true;		
	}

	return false;
}

void Aura::SpellAuraFeignDeath(bool apply)
{
	if( p_target != NULL )
	{
		WorldPacket data(50);

		if( apply )
		{
			p_target->EventAttackStop();
			p_target->setDeathState(ALIVE);

			//now get rid of mobs agro. pTarget->CombatStatus.AttackersForgetHate() - this works only for already attacking mobs
		    for(std::set<Object*>::iterator itr = p_target->GetInRangeSetBegin(); itr != p_target->GetInRangeSetEnd(); itr++ )
			{
				if( (*itr)->IsUnit() && ( static_cast< Unit* >( *itr ))->isAlive())
				{
					Unit *u = static_cast< Unit* >( *itr );

					if( isFeignDeathResisted( p_target->getLevel(), u->getLevel() ) ){
						sEventMgr.AddEvent( this, &Aura::Remove, EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT );
						return;
					}

					if(u->IsCreature() )
						u->GetAIInterface()->RemoveThreatByPtr( p_target );

					//if this is player and targeting us then we interrupt cast
					if( u->IsPlayer() )
					{
						Player* plr = static_cast< Player* >( *itr );

						if( plr->IsCasting() && plr->GetSelection() == p_target->GetGUID() )
							plr->CancelSpell( NULL ); //cancel current casting spell

						plr->GetSession()->SendPacket( &data );
					}
				}
			}

			p_target->SetFlag( UNIT_FIELD_FLAGS_2, 1 );
			p_target->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH );
			p_target->SetFlag( UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD );

			data.SetOpcode( SMSG_START_MIRROR_TIMER );

			data << uint32( TIMER_FEIGNDEATH );		// type
			data << uint32( GetDuration() );
			data << uint32( GetDuration() );
			data << uint32( 0xFFFFFFFF );
			data << uint8( 0 );
			data << uint32( m_spellProto->Id );		// ???

			p_target->GetSession()->SendPacket( &data );

			data.Initialize( SMSG_CLEAR_TARGET );
			data << p_target->GetGUID();

			p_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);

            if(p_target->hasStateFlag(UF_ATTACKING)) 
                    p_target->clearStateFlag(UF_ATTACKING);

            p_target->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
            p_target->GetSession()->OutPacket(SMSG_CANCEL_AUTO_REPEAT);
		}
		else
		{
			p_target->RemoveFlag(UNIT_FIELD_FLAGS_2, 1);
			p_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH);
			p_target->RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD);
			data.SetOpcode(SMSG_STOP_MIRROR_TIMER);
            data << uint32( TIMER_FEIGNDEATH );
			p_target->GetSession()->SendPacket(&data);
		}
	}
}

void Aura::SpellAuraModDisarm(bool apply)
{
	if(apply)
	{
		if( p_target != NULL && p_target->IsInFeralForm() ) return;

		SetNegative();

		m_target->disarmed = true;
		m_target->m_special_state |= UNIT_STATE_DISARMED;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
	}
	else
	{
		m_target->disarmed = false;
		m_target->m_special_state &= ~UNIT_STATE_DISARMED;
		m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
	}
}

void Aura::SpellAuraModStalked(bool apply)
{
	if(apply)
	{
		m_target->stalkedby = m_casterGuid;
		SetNegative();
	}
	else
	{
		m_target->stalkedby = 0;
	}
}

void Aura::SpellAuraSchoolAbsorb(bool apply)
{
	if(apply)
	{
		SetPositive();

		int32 val = mod->m_amount;

		Unit* caster = GetUnitCaster();

		if( caster != NULL )
		{
			if (GetSpellProto()->SpellGroupType) 
			{
				SM_FIValue(caster->SM_FMiscEffect,&val,GetSpellProto()->SpellGroupType);
				SM_PIValue(caster->SM_PMiscEffect,&val,GetSpellProto()->SpellGroupType);
			}

			//This will fix talents that affects damage absorbed.
			int flat = 0;
			SM_FIValue( caster->SM_FMiscEffect, &flat, GetSpellProto()->SpellGroupType );
			val += val * flat / 100;

			//For spells Affected by Bonus Healing we use Dspell_coef_override.
			if( GetSpellProto()->Dspell_coef_override >= 0 )
				val += float2int32( caster->HealDoneMod[GetSpellProto()->School] * GetSpellProto()->Dspell_coef_override );
			//For spells Affected by Bonus Damage we use OTspell_coef_override.
			else if( GetSpellProto()->OTspell_coef_override >= 0 )
				val += float2int32( caster->GetDamageDoneMod( GetSpellProto()->School ) * GetSpellProto()->OTspell_coef_override );
		}

		Absorb *ab = new Absorb;
		ab->amt = val;
		ab->spellid = GetSpellId();
		ab->caster = m_casterGuid;
		for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
			if( mod->m_miscValue & (((uint32)1) << x ) )
				m_target->Absorbs[x].push_back( ab );
	}
	else
	{
		Absorb *ab = NULL;
        std::set< Absorb* > aset;
        aset.clear();

        //this crap here tends to double delete so we collect the pointers into a set instead of deleting
        //and then when we are done we delete
		for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
		{
			if( mod->m_miscValue & (((uint32)1) << x ) )
			{
				for(SchoolAbsorb::iterator i = m_target->Absorbs[x].begin(); i != m_target->Absorbs[x].end(); i++)
				{
					if((*i)->spellid == GetSpellId() && (*i)->caster==m_casterGuid )
					{
						ab = (*i);
						m_target->Absorbs[x].erase(i);
                        aset.insert( ab );
						break;
					}
				}
			}
		}
        
        for( std::set< Absorb* >::iterator itr = aset.begin(); itr != aset.end(); ++itr )
            delete *itr;
        
        aset.clear();
	}
}

void Aura::SpellAuraModSpellCritChanceSchool(bool apply)
{
	if(apply)
	{
		for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
			if( mod->m_miscValue & (((uint32)1) << x ) )
				m_target->SpellCritChanceSchool[x] += mod->m_amount;
		if( mod->m_amount < 0 )
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		for(uint32 x= 0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->SpellCritChanceSchool[x] -= mod->m_amount;
				/*if(m_target->SpellCritChanceSchool[x] < 0)
					m_target->SpellCritChanceSchool[x] = 0;*/
			}
		}
	}
	if( p_target != NULL )
		p_target->UpdateChanceFields();
}

void Aura::SpellAuraModPowerCost(bool apply)
{
	int32 val = (apply) ? mod->m_amount : -mod->m_amount;
	if (apply)
	{
		if(val > 0)
			SetNegative();
		else
			SetPositive();
	}
	for(uint32 x= 0;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->ModPowerCostMultiplier(x,val/100.0f);
}

void Aura::SpellAuraModPowerCostSchool(bool apply)
{
	if(apply)
	{
		for(uint32 x=1;x<7;x++)
			if (mod->m_miscValue & (((uint32)1)<<x) )
				m_target->PowerCostMod[x] += mod->m_amount;
	}
	else
	{
		for(uint32 x=1;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->PowerCostMod[x] -= mod->m_amount;
			}
		}
	}
}

void Aura::SpellAuraReflectSpellsSchool(bool apply)
{
	m_target->RemoveReflect( GetSpellId(), apply );

	if( apply )
	{
		ReflectSpellSchool *rss = new ReflectSpellSchool;
		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->require_aura_hash = 0;
		rss->infront = false;
		
		if(m_spellProto->Attributes == 0x400D0 && m_spellProto->AttributesEx == 0)
			rss->school = (int)(log10((float)mod->m_miscValue) / log10((float)2));
		else
			rss->school = m_spellProto->School;

		rss->charges = 0;

		m_target->m_reflectSpellSchool.push_back( rss );
	}
}

void Aura::SpellAuraModLanguage(bool apply)
{
	if(apply)
		m_target->m_modlanguage = mod->m_miscValue;
	else
		m_target->m_modlanguage = -1;
}

void Aura::SpellAuraAddFarSight(bool apply)
{
	if(apply)
	{
		if(m_target->GetTypeId() != TYPEID_PLAYER)
			return;

		//FIXME:grep aka Nublex will fix this
		//Make update circle bigger here
	}
	else
	{
		//Destroy new updated objects here if they are still out of update range
		//w/e
	}
}

void Aura::SpellAuraMechanicImmunity(bool apply)
{
	if(apply)
	{
		Arcemu::Util::ARCEMU_ASSERT(   mod->m_miscValue < MECHANIC_END);
		m_target->MechanicsDispels[mod->m_miscValue]++;

		if(mod->m_miscValue != 16 && mod->m_miscValue != 25 && mod->m_miscValue != 19) // don't remove bandages, Power Word and protection effect
		{
			/* Supa's test run of Unit::RemoveAllAurasByMechanic */
			m_target->RemoveAllAurasByMechanic( (uint32)mod->m_miscValue , static_cast<uint32>(-1) , false );

			//Insignia/Medallion of A/H			//Every Man for Himself
			if( m_spellProto->Id == 42292 || m_spellProto->Id == 59752 )
			{
				for(uint32 x= MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; ++x)
				{
					if(m_target->m_auras[x])
					{
						for(uint32 y = 0; y < 3; ++y)
						{
							switch(m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[y])
							{
							case SPELL_AURA_MOD_STUN:
							case SPELL_AURA_MOD_CONFUSE:
							case SPELL_AURA_MOD_ROOT:
							case SPELL_AURA_MOD_FEAR:
							case SPELL_AURA_MOD_DECREASE_SPEED:
								m_target->m_auras[x]->Remove();
								goto out;
								break;
							}
							continue;

out:
							break;
						}
					}
				}
			}
		}
		else
			SetNegative();

		// Demonic Circle hack
		if ( m_spellProto->Id == 48020 && m_target->IsPlayer() && m_target->HasAura( 62388 ) ){
			GameObject* obj = m_target->GetMapMgr()->GetGameObject( m_target->m_ObjectSlots[ 0 ] );

			if( obj != NULL ){
				Player *ptarget = static_cast< Player* >( m_target );

				ptarget->SafeTeleport( obj->GetMapId(), obj->GetInstanceID(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), m_target->GetOrientation() );
			}
		}
	}
	else
		m_target->MechanicsDispels[mod->m_miscValue]--;
}

void Aura::SpellAuraMounted(bool apply)
{
	if(!p_target) return;

	/*Shady: Is it necessary? Stealth should be broken since all spells with Mounted SpellEffect don't have "does not break stealth" flag (except internal Video mount spell).
	So commented, cause we don't need useless checks and hackfixes*/
	/* if(m_target->IsStealth())
	{
		uint32 id = m_target->m_stealth;
		m_target->m_stealth = 0;
		m_target->RemoveAura(id);
	}*/

	if(apply)
	{

		SetPositive();

		//p_target->AdvanceSkillLine(762); // advance riding skill

		if(p_target->m_bg)
			p_target->m_bg->HookOnMount(p_target);

		p_target->Dismount();

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_MOUNT);

		CreatureInfo* ci = CreatureNameStorage.LookupEntry(mod->m_miscValue);
		if(!ci) return;

		uint32 displayId = ci->Male_DisplayID;
		if(!displayId) return;

		p_target->m_MountSpellId = m_spellProto->Id;
		p_target->flying_aura = 0;
		m_target->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , displayId);
		//m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);

		if( p_target->GetShapeShift() && !(p_target->GetShapeShift() & ( FORM_BATTLESTANCE | FORM_DEFENSIVESTANCE | FORM_BERSERKERSTANCE ) ) && p_target->m_ShapeShifted != m_spellProto->Id )
			p_target->RemoveAura( p_target->m_ShapeShifted );

		p_target->DismissActivePets();
	}
	else
	{
		p_target->m_MountSpellId = 0;
		p_target->flying_aura = 0;
		m_target->SetMount(0);
		//m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);

		//if we had pet then respawn
		p_target->SpawnActivePet();
		p_target->RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_DISMOUNT );
	}
}

void Aura::SpellAuraModDamagePercDone(bool apply)
{
	float val = (apply) ? mod->m_amount/100.0f : -mod->m_amount/100.0f;

	switch (GetSpellId()) //dirty or mb not fix bug with wand specializations
	{
	case 14524:
	case 14525:
	case 14526:
	case 14527:
	case 14528:
		return;
	}
	if( p_target != NULL )
	{
		if( GetSpellProto()->EquippedItemClass == -1 )//does not depend on weapon
		{
			for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
			{
				if( mod->m_miscValue & ( (uint32)1 << x ) )
				{
					// display to client (things that are weapon dependant don't get displayed)
					p_target->ModFloatValue( PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + x, val );
				}
			}
		}
		if( mod->m_miscValue & 1 )
		{
			if( apply )
			{
				WeaponModifier md;
				md.value = val;
				md.wclass = GetSpellProto()->EquippedItemClass;
				md.subclass = GetSpellProto()->EquippedItemSubClass;
				p_target->damagedone.insert( make_pair( GetSpellId(), md ) );
			}
			else
			{
				std::map< uint32, WeaponModifier >::iterator i = p_target->damagedone.begin();

				for( ; i != p_target->damagedone.end() ; i++ )
				{
					if(( *i).first == GetSpellId() )
					{
						p_target->damagedone.erase(i);
						break;
					}
				}
				p_target->damagedone.erase( GetSpellId() );
			}
		}
	}
	else
	{
		for( uint8 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & ( (uint32)1 << x ) )
			{
				static_cast< Creature* >( m_target )->ModDamageDonePct[x] += val;
			}
		}
	}
	m_target->CalcDamage();
}

void Aura::SpellAuraModPercStat(bool apply)
{
	int32 val;
	if(apply)
	{
		val = mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();
	}
	else
		val= -mod->m_amount;

	if (mod->m_miscValue == -1)//all stats
	{
		if( p_target != NULL )
		{
			for( uint8 x = 0; x < 5; x++ )
			{
				if( mod->m_amount > 0 )
					p_target->StatModPctPos[x] += val;
				else
					p_target->StatModPctNeg[x] -= val;

				p_target->CalcStat(x);
			}

			p_target->UpdateStats();
			p_target->UpdateChances();
		}
		else
		{
			for( uint8 x = 0; x < 5; x++ )
			{
				static_cast< Creature* >( m_target )->StatModPct[x] += val;
				static_cast< Creature* >( m_target )->CalcStat(x);
			}
		}
	}
	else
	{
		Arcemu::Util::ARCEMU_ASSERT(   mod->m_miscValue < 5);
		if( p_target != NULL )
		{
			if( mod->m_amount > 0 )
				p_target->StatModPctPos[mod->m_miscValue] += val;
			else
				p_target->StatModPctNeg[mod->m_miscValue] -= val;

			p_target->CalcStat(mod->m_miscValue);

			p_target->UpdateStats();
			p_target->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			static_cast< Creature* >( m_target )->StatModPct[ mod->m_miscValue ] += val;
			static_cast< Creature* >( m_target )->CalcStat( mod->m_miscValue );
		}
	}
}

void Aura::SpellAuraSplitDamage(bool apply)
{
	Unit *source = NULL; // This is the Unit whose damage we are splitting
	Unit *destination = NULL; // This is the Unit that shares the beating
	Object *caster = GetCaster();

	// We don't want to split our damage with the owner
	if( ( m_spellProto->Effect[ mod->i ] == SPELL_EFFECT_APPLY_OWNER_AREA_AURA ) &&
		( caster != NULL ) &&
		( m_target != NULL ) &&
		caster->IsPet() &&
		caster->GetGUID() == m_target->GetGUID() )
		return;

	if( m_areaAura ){
		source = GetTarget();
		destination = GetUnitCaster();
	}else{
		source = GetUnitCaster();
		destination = GetTarget();
	}

	if( source == NULL || destination == NULL )
		return;

	if( source->m_damageSplitTarget != NULL ){
		delete source->m_damageSplitTarget;
		source->m_damageSplitTarget = NULL;
	}

	if( apply ){
		DamageSplitTarget *ds = new DamageSplitTarget;
		ds->m_flatDamageSplit = 0;
		ds->m_spellId = GetSpellProto()->Id;
		ds->m_pctDamageSplit = mod->m_miscValue / 100.0f;
		ds->damage_type = static_cast<uint8>( mod->m_type );
		ds->creator = (void*)this;
		ds->m_target = destination->GetGUID();
		source->m_damageSplitTarget = ds;
	}else{
		DamageSplitTarget *ds = source->m_damageSplitTarget;
		source->m_damageSplitTarget = NULL;
		delete ds;
	}
}

void Aura::SpellAuraModRegen(bool apply)
{
	if(apply)//seems like only positive
	{
		SetPositive ();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1,(uint32)((this->GetSpellProto()->EffectBasePoints[mod->i]+1)/5)*3,
			EVENT_AURA_PERIODIC_REGEN,3000,0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::SpellAuraPeriodicTriggerDummy(bool apply)
{
	if( apply ){

		sEventMgr.AddEvent( this, &Aura::EventPeriodicTriggerDummy, EVENT_AURA_PERIODIC_DUMMY, m_spellProto->EffectAmplitude[ mod->i ], 0, 0 );
	
	}else{
		if( !sScriptMgr.CallScriptedDummyAura( m_spellProto->Id, mod->i, this, false ) )
			sLog.outError("Spell %u ( %s ) has an apply periodic trigger dummy aura effect, but no handler for it.", m_spellProto->Id, m_spellProto->Name );

	}
}

void Aura::EventPeriodicTrigger( uint32 amount, uint32 type )
{

}

void Aura::EventPeriodicEnergizeVariable( uint32 amount, uint32 type )
{
	uint32 POWER_TYPE = UNIT_FIELD_POWER1 + type;

	Arcemu::Util::ARCEMU_ASSERT(   POWER_TYPE<=UNIT_FIELD_POWER5);

	Unit* ucaster = GetUnitCaster();
	if( ucaster != NULL )
		ucaster->Energize( m_target, m_spellProto->Id, amount, type );
}

void Aura::EventPeriodicDrink(uint32 amount)
{
	uint32 v = m_target->GetPower( POWER_TYPE_MANA ) + amount;

	if( v > m_target->GetMaxPower( POWER_TYPE_MANA ) )
		v = m_target->GetMaxPower( POWER_TYPE_MANA );

	m_target->SetPower( POWER_TYPE_MANA, v);
}

void Aura::EventPeriodicHeal1(uint32 amount)
{
	if(!m_target->isAlive())
		return;

	uint32 ch = m_target->GetHealth();
	ch+=amount;
	uint32 mh = m_target->GetMaxHealth();

	if(ch>mh)
		m_target->SetHealth(mh);
	else
		m_target->SetHealth(ch);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
	else
	{
		if(!(m_spellProto->BGR_one_buff_on_target & SPELL_TYPE_ARMOR))
			m_target->SendPeriodicHealAuraLog( m_casterGuid, m_target->GetNewGUID(), GetSpellId(), amount, 0, false );
	}

	m_target->RemoveAurasByHeal();
}

void Aura::SpellAuraModPowerRegen(bool apply)
{
	if(!mod->m_amount)
		return;

	if(apply)
	{
		if (mod->m_amount>0)
			SetPositive();
		else
			SetNegative();
	}
	if( p_target != NULL && mod->m_miscValue == POWER_TYPE_MANA )
	{
		int32 val = (apply) ? mod->m_amount: -mod->m_amount;
		p_target->m_ModInterrMRegen +=val;
		p_target->UpdateStats();
	}
}

void Aura::SpellAuraChannelDeathItem(bool apply)
{
	SetNegative(); //this should always be negative as npcs remove negative auras on death

	if(apply)
	{
		//don't need for now
	}
	else
	{
		if( m_target->GetTypeId() == TYPEID_UNIT || m_target->GetTypeId() == TYPEID_PLAYER )
		{
			if ( m_target->GetTypeId() == TYPEID_UNIT && TO_CREATURE(m_target)->GetCreatureInfo()->Type == UNIT_TYPE_CRITTER )
				return;

			if(m_target->IsDead())
			{
				Player *pCaster = m_target->GetMapMgr()->GetPlayer((uint32)m_casterGuid);
				if(!pCaster)
					return;
				/*int32 delta=pCaster->getLevel()-m_target->getLevel();
				if(abs(delta)>5)
					return;*/

				uint32 itemid = GetSpellProto()->EffectItemType[mod->i];

				//Warlocks only get Soul Shards from enemies that grant XP or Honor
				if (itemid == 6265 && (pCaster->getLevel() > m_target->getLevel()) )                            
					if ( (pCaster->getLevel() - m_target->getLevel() ) > 9)
						return;


				ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);
				if(pCaster->GetItemInterface()->CalculateFreeSlots(proto) > 0)
				{
					Item *item = objmgr.CreateItem(itemid,pCaster);
					if(!item) return;


                    item->SetCreatorGUID( pCaster->GetGUID() );
					if(!pCaster->GetItemInterface()->AddItemToFreeSlot(item))
					{
						pCaster->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_INVENTORY_FULL);
						item->DeleteMe();
						return;
					}
					SlotResult * lr = pCaster->GetItemInterface()->LastSearchResult();

                    pCaster->SendItemPushResult( true,false,true,true,lr->ContainerSlot,lr->Slot,1 , item->GetEntry(), item->GetItemRandomSuffixFactor(), item->GetItemRandomPropertyId(), item->GetStackCount()  );
				}
			}
		}
	}
}

void Aura::SpellAuraModDamagePercTaken(bool apply)
{
	float val;
	if(apply)
	{
		val = mod->m_amount/100.0f;
		if(val <= 0)
			SetPositive();
		else
			SetNegative();
	}
	else
	{
		val= -mod->m_amount/100.0f;
	}

	if( m_spellProto->NameHash == SPELL_HASH_ARDENT_DEFENDER ) // Ardent Defender it only applys on 20% hp :/
	{
		m_target->DamageTakenPctModOnHP35 += val;
		return;
	}

	for(uint32 x= 0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->DamageTakenPctMod[x]+=val;
		}
	}
}

void Aura::SpellAuraModRegenPercent(bool apply)
{
	if(apply)
		m_target->PctRegenModifier += mod->m_amount;
	else
		m_target->PctRegenModifier -= mod->m_amount;
}

void Aura::SpellAuraPeriodicDamagePercent(bool apply)
{
	if(apply)
	{
		//uint32 gr = GetSpellProto()->SpellGroupType;
		//if(gr)
		//{
		//	Unit*c=GetUnitCaster();
		//	if(c)
		//	{
		//		SM_FIValue(c->SM_FDOT,(int32*)&dmg,gr);
		//		SM_PIValue(c->SM_PDOT,(int32*)&dmg,gr);
		//	}
		//}

		/*if(m_spellProto->Id == 28347) //Dimensional Siphon
		{
			uint32 dmg = (m_target->GetMaxHealth()*5)/100;
			sEventMgr.AddEvent(this, &Aura::EventPeriodicDamagePercent, dmg,
				EVENT_AURA_PERIODIC_DAMAGE_PERCENT, 1000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		else*/
		{
			uint32 dmg = mod->m_amount;
			sEventMgr.AddEvent(this, &Aura::EventPeriodicDamagePercent, dmg,
				EVENT_AURA_PERIODIC_DAMAGE_PERCENT,GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		SetNegative();
	}
}

void Aura::EventPeriodicDamagePercent(uint32 amount)
{
	//DOT
	if(!m_target->isAlive())
		return;
	if(m_target->SchoolImmunityList[GetSpellProto()->School])
		return;

	uint32 damage = float2int32(amount/100.0f*m_target->GetMaxHealth());

	Unit * c = GetUnitCaster();

	if( m_target->m_damageSplitTarget)
	{
		damage = m_target->DoDamageSplitTarget(damage, GetSpellProto()->School, false);
	}

	if(c)
		c->SpellNonMeleeDamageLog(m_target, GetSpellProto()->Id, damage, pSpellId== 0, true);
	else
		m_target->SpellNonMeleeDamageLog(m_target, GetSpellProto()->Id, damage, pSpellId== 0, true);
}

void Aura::SpellAuraModResistChance(bool apply)
{
	apply ? m_target->m_resistChance = mod->m_amount : m_target->m_resistChance = 0;
}

void Aura::SpellAuraModDetectRange(bool apply)
{
	Unit*m_caster=GetUnitCaster();
	if(!m_caster)return;
	if(apply)
	{
		SetNegative();
		m_caster->setDetectRangeMod(m_target->GetGUID(), mod->m_amount);
	}
	else
	{
		m_caster->unsetDetectRangeMod(m_target->GetGUID());
	}
}

void Aura::SpellAuraPreventsFleeing(bool apply)
{
	// Curse of Recklessness
}

void Aura::SpellAuraModUnattackable(bool apply)
{
/*
		Also known as Apply Aura: Mod Unintractable
		Used by: Spirit of Redemption, Divine Intervention, Phase Shift, Flask of Petrification
		It uses one of the UNIT_FIELD_FLAGS, either UNIT_FLAG_NOT_SELECTABLE or UNIT_FLAG_NOT_ATTACKABLE_2
*/
}

void Aura::SpellAuraInterruptRegen(bool apply)
{
	if(apply)
		m_target->m_interruptRegen++;
	else
	{
		m_target->m_interruptRegen--;
		  if(m_target->m_interruptRegen < 0)
			m_target->m_interruptRegen = 0;
	}
}

void Aura::SpellAuraGhost(bool apply)
{
	if( p_target != NULL )
	{
		m_target->m_invisible = apply;

		if( apply )
		{
			SetNegative();
			p_target->SetMovement( MOVE_WATER_WALK, 4 );
		}
		else
		{
			p_target->SetMovement( MOVE_LAND_WALK, 7 );
		}
	}
}

void Aura::SpellAuraMagnet(bool apply)
{
	if(apply){
		Unit *caster = GetUnitCaster();
		if (!caster)
			return;
		SetPositive();
		m_target->m_magnetcaster = caster->GetGUID();
	}
	else{
		m_target->m_magnetcaster = 0;
	}
}

void Aura::SpellAuraManaShield(bool apply)
{
	if(apply)
	{
		SetPositive();
		m_target->m_manashieldamt = mod->m_amount ;
		m_target->m_manaShieldId = GetSpellId();
	}
	else
	{
		m_target->m_manashieldamt = 0;
		m_target->m_manaShieldId = 0;
	}
}

void Aura::SpellAuraSkillTalent(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			SetPositive();
			p_target->_ModifySkillBonus( mod->m_miscValue, mod->m_amount );
		}
		else
			p_target->_ModifySkillBonus( mod->m_miscValue, -mod->m_amount );

		p_target->UpdateStats();
	}
}

void Aura::SpellAuraModAttackPower(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else
		SetPositive();
	m_target->ModAttackPowerMods(apply? mod->m_amount : -mod->m_amount);
	m_target->CalcDamage();
}

void Aura::SpellAuraVisible(bool apply)
{
	//Show positive spells on target
	if(apply)
	{
		SetNegative();
	}
}

void Aura::SpellAuraModResistancePCT(bool apply)
{
	uint32 Flag = mod->m_miscValue;
	int32 amt;
	if(apply)
	{
		amt=mod->m_amount;
	 //   if(amt>0)SetPositive();
	   // else SetNegative();
	}
	else
		amt= -mod->m_amount;

	for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
	{
		if( Flag & (((uint32)1) << x ) )
		{
			if( p_target != NULL )
			{
				if( mod->m_amount > 0 )
				{
					p_target->ResistanceModPctPos[x] += amt;
				}
				else
				{
					p_target->ResistanceModPctNeg[x] -= amt;
				}
				p_target->CalcResistance(x);

			}
			else if( m_target->GetTypeId() == TYPEID_UNIT )
			{
				static_cast< Creature*>( m_target )->ResistanceModPct[x] += amt;
				static_cast< Creature*>( m_target )->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraModCreatureAttackPower(bool apply)
{
	if( apply )
	{
		for( uint32 x = 0; x < 11; x++ )
			if( mod->m_miscValue & ( ( ( uint32 )1 ) << x ) )
				m_target->CreatureAttackPowerMod[x+1] += mod->m_amount;

		if( mod->m_amount > 0 )
			SetPositive();
		else
			SetNegative();
	}
	else
	{
		for( uint32 x = 0; x < 11; x++ )
		{
			if( mod->m_miscValue & ( ( ( uint32 )1 ) << x ) )
			{
				m_target->CreatureAttackPowerMod[x+1] -= mod->m_amount;
			}
		}
	}
	m_target->CalcDamage();
}

void Aura::SpellAuraModTotalThreat( bool apply )
{
	if( apply )
	{
		if( mod->m_amount < 0 )
			SetPositive();
		else
			SetNegative();

		m_target->ModThreatModifyer( mod->m_amount );
	}
	else
		m_target->ModThreatModifyer(-(mod->m_amount));
}

void Aura::SpellAuraWaterWalk( bool apply )
{
	if( p_target != NULL )
	{
		WorldPacket data( 12 );
		if( apply )
		{
			SetPositive();
			data.SetOpcode( SMSG_MOVE_WATER_WALK );
			data << p_target->GetNewGUID();
			data << uint32( 8 );
		}
		else
		{
			data.SetOpcode( SMSG_MOVE_LAND_WALK );
			data << p_target->GetNewGUID();
			data << uint32( 4 );
		}
		p_target->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraFeatherFall( bool apply )
{
	//TODO: FIX ME: Find true flag for this
	if( p_target == NULL )
		return;

	WorldPacket data( 12 );
	if( apply )
	{
		SetPositive();
		data.SetOpcode( SMSG_MOVE_FEATHER_FALL );
		p_target->m_noFallDamage = true;
	}
	else
	{
		data.SetOpcode(SMSG_MOVE_NORMAL_FALL);
		p_target->m_noFallDamage = false;
	}

	data << m_target->GetNewGUID();
	data << uint32( 0 );
	p_target->SendMessageToSet( &data, true );
}

void Aura::SpellAuraHover( bool apply )
{
	SetPositive();

	WorldPacket data;

	if( apply ){
		data.Initialize( SMSG_MOVE_SET_HOVER );
		m_target->SetFloatValue( UNIT_FIELD_HOVERHEIGHT, ( float( mod->m_amount ) / 2 ) );
	}else{
		data.Initialize( SMSG_MOVE_UNSET_HOVER );
		m_target->SetFloatValue( UNIT_FIELD_HOVERHEIGHT, 0.0f );
	}

	data << WoWGuid( m_target->GetNewGUID() );
	data << uint32( 0 );

	m_target->SendMessageToSet( &data, true );

}

void Aura::SpellAuraAddPctMod( bool apply )
{
	int32 val = apply ? mod->m_amount : -mod->m_amount;
	uint32* AffectedGroups = GetSpellProto()->EffectSpellClassMask[mod->i];

	switch( mod->m_miscValue )//let's generate warnings for unknown types of modifiers
	{
	case SMT_DAMAGE_DONE:
		SendModifierLog( &m_target->SM_PDamageBonus, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_DURATION:
		SendModifierLog( &m_target->SM_PDur, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_THREAT_REDUCED:
		SendModifierLog(&m_target->SM_PThreat, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true);
		break;

	case SMT_EFFECT_1:
		SendModifierLog( &m_target->SM_PEffect1_Bonus, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_CHARGES:
		SendModifierLog(&m_target->SM_PCharges, val, AffectedGroups,static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_RANGE:
		SendModifierLog( &m_target->SM_PRange, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_RADIUS:
		SendModifierLog( &m_target->SM_PRadius, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_CRITICAL:
		SendModifierLog( &m_target->SM_CriticalChance, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_MISC_EFFECT:
		SendModifierLog( &m_target->SM_PMiscEffect, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_NONINTERRUPT:
		SendModifierLog( &m_target->SM_PNonInterrupt, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		//SpellAuraResistPushback(true);
		break;

	case SMT_CAST_TIME:
		SendModifierLog( &m_target->SM_PCastTime, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_COOLDOWN_DECREASE:
		SendModifierLog( &m_target->SM_PCooldownTime, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_EFFECT_2:
		SendModifierLog( &m_target->SM_PEffect2_Bonus, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_COST:
		SendModifierLog( &m_target->SM_PCost, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_CRITICAL_DAMAGE:
		SendModifierLog( &m_target->SM_PCriticalDamage, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	//case SMT_HITCHANCE: - no pct
	//case SMT_ADDITIONAL_TARGET: - no pct
	//case SMT_TRIGGER: - todo

	case SMT_AMPTITUDE:
		SendModifierLog( &m_target->SM_PAmptitude, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_JUMP_REDUCE:
		SendModifierLog( &m_target->SM_PJumpReduce, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_GLOBAL_COOLDOWN:
		SendModifierLog( &m_target->SM_PGlobalCooldown, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_SPELL_VALUE_PCT:
		SendModifierLog( &m_target->SM_PDOT, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_EFFECT_3:
		SendModifierLog( &m_target->SM_PEffect3_Bonus, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_PENALTY:
		SendModifierLog( &m_target->SM_PPenalty, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_EFFECT_BONUS:
		SendModifierLog( &m_target->SM_PEffectBonus, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	case SMT_RESIST_DISPEL:
		SendModifierLog( &m_target->SM_PRezist_dispell, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ), true );
		break;

	default://Unknown modifier type
		sLog.outError( 
			"Unknown spell modifier type %u in spell %u.<<--report this line to the developer", 
			mod->m_miscValue, GetSpellId() );
		break;
	}
}

void Aura::SendModifierLog( int32** m, int32 v, uint32* mask, uint8 type, bool pct )
{
	uint32 intbit = 0, groupnum = 0;

	if( *m == 0 )
	{
		*m = new int32[SPELL_GROUPS];
		for(uint32 bit = 0; bit < SPELL_GROUPS; ++bit, ++intbit)
		{
			if(intbit == 32)
			{
				++groupnum;
				intbit = 0;
			}

			if( ( 1 << intbit ) & mask[groupnum] )
			{
				(*m)[bit] = v;

				if( !m_target->IsPlayer() )
                    continue;
                
                TO_PLAYER(m_target)->SendSpellModifier( static_cast< uint8 >( bit ), type, v, pct );                
			}
			else
				(*m)[bit] = 0;
		}
	}
	else
	{
		for(uint8 bit = 0; bit < SPELL_GROUPS; ++bit,++intbit)
		{
			if(intbit == 32)
			{
				++groupnum;
				intbit = 0;
			}

			if( ( 1 << intbit ) & mask[groupnum] )
			{
				(*m)[bit] += v;

				if( !m_target->IsPlayer() )
					continue;

                TO_PLAYER(m_target)->SendSpellModifier( bit, type, (*m)[ bit ], pct );
			}
		}
	}
}

void Aura::SendDummyModifierLog( std::map< SpellEntry*, uint32 >* m, SpellEntry* spellInfo, uint32 i, bool apply, bool pct )
{
	int32 v = spellInfo->EffectBasePoints[i] + 1;
	uint32* mask = spellInfo->EffectSpellClassMask[i];
	uint8 type = static_cast<uint8>(spellInfo->EffectMiscValue[i]);

	if(apply)
	{
		m->insert(make_pair(spellInfo,i));
	}
	else
	{
		v = -v;
		std::map<SpellEntry*,uint32>::iterator itr = m->find(spellInfo);
		if (itr != m->end())
			m->erase(itr);
	}

	uint32 intbit = 0, groupnum = 0;
	for(uint8 bit = 0; bit < SPELL_GROUPS; ++bit, ++intbit)
	{
		if(intbit == 32)
 		{
			++groupnum;
			intbit = 0;
		}
		if( ( 1 << intbit ) & mask[groupnum] )
		{
			if( p_target == NULL)
				continue;

            p_target->SendSpellModifier( bit, type, v, pct );
		}
	}
}

void Aura::SpellAuraAddClassTargetTrigger(bool apply)
{
	if(apply)
	{
		uint32 groupRelation[3], procClassMask[3];
		int charges;
		SpellEntry *sp;

		// Find spell of effect to be triggered
		sp = dbcSpell.LookupEntryForced(GetSpellProto()->EffectTriggerSpell[mod->i]);
		if( sp == NULL )
		{
			sLog.outDebug("Warning! class trigger spell is null for spell %u", GetSpellProto()->Id);
			return;
		}

		// Initialize proc class mask
		procClassMask[0] = GetSpellProto()->EffectSpellClassMask[mod->i][0];
		procClassMask[1] = GetSpellProto()->EffectSpellClassMask[mod->i][1];
		procClassMask[2] = GetSpellProto()->EffectSpellClassMask[mod->i][2];

		// Initialize mask
		groupRelation[0] = sp->EffectSpellClassMask[mod->i][0];
		groupRelation[1] = sp->EffectSpellClassMask[mod->i][1];
		groupRelation[2] = sp->EffectSpellClassMask[mod->i][2];

		// Initialize charges
		charges = GetSpellProto()->procCharges;
		Unit* ucaster = GetUnitCaster();
		if( ucaster != NULL && GetSpellProto()->SpellGroupType )
		{
			SM_FIValue( ucaster->SM_FCharges, &charges, GetSpellProto()->SpellGroupType );
			SM_PIValue( ucaster->SM_PCharges, &charges, GetSpellProto()->SpellGroupType );
		}

		m_target->AddProcTriggerSpell(sp->Id, GetSpellProto()->Id, m_casterGuid, GetSpellProto()->EffectBasePoints[mod->i] +1, PROC_ON_CAST_SPELL, charges, groupRelation, procClassMask);

		sLog.outDebug("%u is registering %u chance %u flags %u charges %u triggeronself %u interval %u",GetSpellProto()->Id,sp->Id,GetSpellProto()->procChance,PROC_ON_CAST_SPELL,charges,GetSpellProto()->procFlags & PROC_TARGET_SELF,GetSpellProto()->proc_interval);
	}
	else
	{
		// Find spell of effect to be triggered
		uint32 spellId = GetSpellProto()->EffectTriggerSpell[mod->i];
		if( spellId == 0 )
		{
			sLog.outDebug("Warning! trigger spell is null for spell %u", GetSpellProto()->Id);
			return;
		}

		m_target->RemoveProcTriggerSpell(spellId, m_casterGuid);
	}
}

void Aura::SpellAuraModPowerRegPerc(bool apply)
{
	if( apply )
		m_target->PctPowerRegenModifier[mod->m_miscValue] += ((float)(mod->m_amount))/100.0f;
	else
		m_target->PctPowerRegenModifier[mod->m_miscValue] -= ((float)(mod->m_amount))/100.0f;
	if( p_target != NULL )
		p_target->UpdateStats();
}

void Aura::SpellAuraOverrideClassScripts(bool apply)
{
	Player* plr = GetPlayerCaster();
	if( plr == NULL )
		return;

	//misc value is spell to add
	//spell familyname && grouprelation

	//Adding bonus to effect
	switch(mod->m_miscValue)
	{
		//----Shatter---
		// Increases crit chance against rooted targets by (Rank * 10)%.
		case 849:
		case 910:
		case 911:
		case 912:
		case 913:
			if ( p_target != NULL )
			{
				int32 val = (apply) ? (mod->m_miscValue-908)*10 : -(mod->m_miscValue-908)*10;
				if( mod->m_miscValue == 849 )
					val = (apply) ? 10 : -10;
				p_target->m_RootedCritChanceBonus += val;
			}
			break;
// ----?
		case 3736:
		case 4415:
		case 4418:
		case 4554:
		case 4555:
		case 4953:
		case 5142:
		case 5147:
		case 5148:
		case 6953:
			{
			if(apply)
			{
				OverrideIdMap::iterator itermap = objmgr.mOverrideIdMap.find(mod->m_miscValue);
                if(itermap == objmgr.mOverrideIdMap.end())
                 {
                     sLog.outError("Unable to find override with overrideid: %u", mod->m_miscValue);
                     break;
                 }

				std::list<SpellEntry *>::iterator itrSE = itermap->second->begin();

				SpellOverrideMap::iterator itr = plr->mSpellOverrideMap.find((*itrSE)->Id);

				if(itr != plr->mSpellOverrideMap.end())
				{
					ScriptOverrideList::iterator itrSO;
					for(itrSO = itr->second->begin(); itrSO != itr->second->end(); ++itrSO)
					{
						if((*itrSO)->id == (uint32)mod->m_miscValue)
						{
							if((int32)(*itrSO)->damage > mod->m_amount)
							{
								(*itrSO)->damage = mod->m_amount;
							}
							return;
						}
					}
					classScriptOverride *cso = new classScriptOverride;
					cso->aura = 0;
					cso->damage = mod->m_amount;
					cso->effect = 0;
					cso->id = mod->m_miscValue;
					itr->second->push_back(cso);
				}
				else
				{
					classScriptOverride *cso = new classScriptOverride;
					cso->aura = 0;
					cso->damage = mod->m_amount;
					cso->effect = 0;
					cso->id = mod->m_miscValue;
					ScriptOverrideList *lst = new ScriptOverrideList();
					lst->push_back(cso);

					for(;itrSE != itermap->second->end(); ++itrSE)
					{
						plr->mSpellOverrideMap.insert( SpellOverrideMap::value_type( (*itrSE)->Id, lst) );
					}
				}
			}
			else
			{
				OverrideIdMap::iterator itermap = objmgr.mOverrideIdMap.find(mod->m_miscValue);
				SpellOverrideMap::iterator itr = plr->mSpellOverrideMap.begin(), itr2;
				while(itr != plr->mSpellOverrideMap.end())
				{
					std::list<SpellEntry *>::iterator itrSE = itermap->second->begin();
					for(;itrSE != itermap->second->end(); ++itrSE)
					{
						if(itr->first == (*itrSE)->Id)
						{
							itr2 = itr++;
							plr->mSpellOverrideMap.erase(itr2);
							break;
						}
					}
					// Check if the loop above got to the end, if so it means the item wasn't found
					// and the itr wasn't incremented so increment it now.
					if(itrSE == itermap->second->end())      itr++;
				}
			}
		}break;
/*		case 19421: //hunter : Improved Hunter's Mark
		case 19422:
		case 19423:
		case 19424:
		case 19425:
			{
				//this should actually add a new functionality to the spell and not override it. There is a lot to decode and to be done here
			}break;*/
		case 4992: // Warlock: Soul Siphon
		case 4993:
			{
				if( apply )
					m_target->m_soulSiphon.max+= mod->m_amount;
				else
					m_target->m_soulSiphon.max-= mod->m_amount;
			}break;
	default:
		sLog.outError("Unknown override report to devs: %u", mod->m_miscValue);
	};
}

void Aura::SpellAuraModRangedDamageTaken(bool apply)
{
	if(apply)
		m_target->RangedDamageTaken += mod->m_amount;
	else
	{
		m_target->RangedDamageTaken -= mod->m_amount;
		if( m_target->RangedDamageTaken < 0)
			m_target->RangedDamageTaken = 0;
	}
}

void Aura::SpellAuraModHealing(bool apply)
{
	int32 val;
	if(apply)
	{
		 val = mod->m_amount;
		 /*if(val>0)
			 SetPositive();
		 else
			 SetNegative();*/
	}
	else
		val=-mod->m_amount;

	for(uint8 x= 0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->HealTakenMod[x] += val;
		}
	}
}

void Aura::SpellAuraIgnoreRegenInterrupt(bool apply)
{
	if( p_target == NULL )
		return;

	if( apply )
		p_target->PctIgnoreRegenModifier += ((float)(mod->m_amount))/100;
	else
		p_target->PctIgnoreRegenModifier -= ((float)(mod->m_amount))/100;
}

void Aura::SpellAuraModMechanicResistance(bool apply)
{
	//silence=26 ?
	//mecanics=9 ?
	if(apply)
	{
		Arcemu::Util::ARCEMU_ASSERT(    mod->m_miscValue < MECHANIC_END );
		m_target->MechanicsResistancesPCT[mod->m_miscValue]+=mod->m_amount;

		if(mod->m_miscValue != MECHANIC_HEALING && mod->m_miscValue != MECHANIC_INVULNARABLE && mod->m_miscValue != MECHANIC_SHIELDED ) // don't remove bandages, Power Word and protection effect
		{
			SetPositive();
		}
		else
		{
			SetNegative();
		}
	}
	else
		m_target->MechanicsResistancesPCT[mod->m_miscValue]-=mod->m_amount;
}

void Aura::SpellAuraModHealingPCT(bool apply)
{
	int32 val;
	if(apply)
	{
		 val = mod->m_amount;
		 if(val<0)
			SetNegative();
		 else
			SetPositive();
	}
	else
		val=-mod->m_amount;

	for(uint8 x= 0; x<7; x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->HealTakenPctMod[x] += ((float)(val))/100;
		}
	}
}

void Aura::SpellAuraUntrackable(bool apply)
{
    if(apply)
        m_target->SetFlag(UNIT_FIELD_BYTES_1, STANDSTATE_FLAG_UNTRACKABLE);
    else
        m_target->RemoveFlag(UNIT_FIELD_BYTES_1, STANDSTATE_FLAG_UNTRACKABLE);
}

void Aura::SpellAuraModRangedAttackPower(bool apply)
{
	if(apply)
	{
		if(mod->m_amount > 0)
			SetPositive();
		else
			SetNegative();
		m_target->ModRangedAttackPowerMods(mod->m_amount);
	}
	else
		m_target->ModRangedAttackPowerMods(-mod->m_amount);
	m_target->CalcDamage();
}

void Aura::SpellAuraModMeleeDamageTaken(bool apply)
{
	if(apply)
	{
		if(mod->m_amount > 0)//does not exist but let it be
			SetNegative();
		else
			SetPositive();
		m_target->DamageTakenMod[0] += mod->m_amount;
	}
	else
		m_target->DamageTakenMod[0] -= mod->m_amount;
}

void Aura::SpellAuraModMeleeDamageTakenPct(bool apply)
{
	if(apply)
	{
		if(mod->m_amount>0)//does not exist but let it be
			SetNegative();
		else
			SetPositive();
		m_target->DamageTakenPctMod[0]+=mod->m_amount/100.0f;
	}
	else
		m_target->DamageTakenPctMod[0]-=mod->m_amount/100.0f;
}

void Aura::SpellAuraRAPAttackerBonus(bool apply)
{
	if(apply)
	{
		m_target->RAPvModifier += mod->m_amount;
	}
	else
		m_target->RAPvModifier -= mod->m_amount;
}

void Aura::SpellAuraModIncreaseSpeedAlways(bool apply)
{
	if(apply)
	{
		SetPositive();
		m_target->m_speedModifier += mod->m_amount;
	}
	else
		m_target->m_speedModifier -= mod->m_amount;

	m_target->UpdateSpeed();
}

void Aura::SpellAuraModIncreaseEnergyPerc( bool apply )
{
	SetPositive();

	if(apply)
	{
		mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value( UNIT_FIELD_MAXPOWER1+mod->m_miscValue, mod->m_amount );
		m_target->ModMaxPower( mod->m_miscValue, mod->fixed_amount[mod->i] );
		if( p_target != NULL && mod->m_miscValue == POWER_TYPE_MANA )
			p_target->SetManaFromSpell( p_target->GetManaFromSpell() + mod->fixed_amount[mod->i] );
	}
	else
	{
		m_target->ModMaxPower( mod->m_miscValue, -mod->fixed_amount[mod->i] );
		if( p_target != NULL && mod->m_miscValue == POWER_TYPE_MANA )
			p_target->SetManaFromSpell( p_target->GetManaFromSpell() - mod->fixed_amount[mod->i] );
	}
}

void Aura::SpellAuraModIncreaseHealthPerc( bool apply )
{
	SetPositive();
	if( apply )
	{
		mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value( UNIT_FIELD_MAXHEALTH, mod->m_amount );
		m_target->ModMaxHealth(mod->fixed_amount[mod->i] );
		if( p_target != NULL )
			p_target->SetHealthFromSpell( p_target->GetHealthFromSpell() + mod->fixed_amount[mod->i] );
//		else if( m_target->IsPet() )
//			static_cast< Pet* >( m_target )->SetHealthFromSpell( ( ( Pet* )m_target )->GetHealthFromSpell() + mod->fixed_amount[mod->i] );
	}
	else
	{
		m_target->ModMaxHealth(-mod->fixed_amount[mod->i] );
		if( m_target->GetUInt32Value( UNIT_FIELD_HEALTH ) > m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
			m_target->SetHealth(m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		if( p_target != NULL )
			p_target->SetHealthFromSpell( TO_PLAYER(m_target)->GetHealthFromSpell() - mod->fixed_amount[mod->i] );
//		else if( m_target->IsPet() )
//			static_cast< Pet* >( m_target )->SetHealthFromSpell( ( ( Pet* )m_target )->GetHealthFromSpell() - mod->fixed_amount[mod->i] );
	}
}

void Aura::SpellAuraModManaRegInterrupt( bool apply )
{
	if( p_target != NULL )
	{
		if( apply )
			p_target->m_ModInterrMRegenPCT += mod->m_amount;
		else
			p_target->m_ModInterrMRegenPCT -= mod->m_amount;

		p_target->UpdateStats();
	}
}

void Aura::SpellAuraModTotalStatPerc(bool apply)
{
	int32 val;
	if(apply)
	{
	   val= mod->m_amount;
	}
	else
	   val= -mod->m_amount;

	if (mod->m_miscValue == -1)//all stats
	{
		if( p_target != NULL )
		{
			for( uint8 x = 0; x < 5; x++ )
			{
				if( mod->m_amount > 0 )
					p_target->TotalStatModPctPos[x] += val;
				else
					p_target->TotalStatModPctNeg[x] -= val;
				p_target->CalcStat(x);
			}

			p_target->UpdateStats();
			p_target->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			for(uint8 x = 0; x < 5; x++ )
			{
				static_cast< Creature* >( m_target )->TotalStatModPct[x] += val;
				static_cast< Creature* >( m_target )->CalcStat(x);
			}
		}
	}
	else
	{
		Arcemu::Util::ARCEMU_ASSERT(   mod->m_miscValue < 5);
		if( p_target != NULL )
		{
			//druid hearth of the wild should add more features based on form
			if( m_spellProto->NameHash == SPELL_HASH_HEART_OF_THE_WILD )
			{
				//we should remove effect first
				p_target->EventTalentHearthOfWildChange( false );
				//set new value
				if( apply )
					p_target->SetTalentHearthOfWildPCT( val );
				else
					p_target->SetTalentHearthOfWildPCT( 0 ); //this happens on a talent reset
				//reapply
				p_target->EventTalentHearthOfWildChange( true );
			}

			if( mod->m_amount > 0 )
				p_target->TotalStatModPctPos[mod->m_miscValue] += val;
			else
				p_target->TotalStatModPctNeg[mod->m_miscValue] -= val;

			p_target->CalcStat( mod->m_miscValue );
			p_target->UpdateStats();
			p_target->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			static_cast< Creature* >( m_target )->TotalStatModPct[ mod->m_miscValue ] += val;
			static_cast< Creature* >( m_target )->CalcStat( mod->m_miscValue );
		}
	}
}

void Aura::SpellAuraModHaste( bool apply )
{
	//blade flurry - attack a nearby opponent
	if( m_spellProto->NameHash == SPELL_HASH_BLADE_FLURRY )
	{
		if( apply )
			m_target->AddExtraStrikeTarget(GetSpellProto(), 0);
		else
			m_target->RemoveExtraStrikeTarget(GetSpellProto());
	}

	if( mod->m_amount < 0 )
		SetNegative();
	else
		SetPositive();

	if( p_target != NULL )
	{
		if( apply )
		{
			p_target->ModAttackSpeed( mod->m_amount, MOD_MELEE );
		}
		else
		{
			p_target->ModAttackSpeed( -mod->m_amount, MOD_MELEE );
		}

		p_target->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME, mod->m_amount );
			mod->fixed_amount[mod->i*2] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME+1, mod->m_amount );

			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ) <= mod->fixed_amount[mod->i] )
				mod->fixed_amount[mod->i] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ); //watch it, a negative timer might be bad ;)
			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME+1 ) <= mod->fixed_amount[mod->i*2] )
				mod->fixed_amount[mod->i*2] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME+1 );//watch it, a negative timer might be bad ;)

			m_target->ModBaseAttackTime(MELEE,-mod->fixed_amount[mod->i] );
			m_target->ModBaseAttackTime(OFFHAND,-mod->fixed_amount[mod->i*2] );

			if ( m_target->GetTypeId() == TYPEID_UNIT )
				static_cast< Creature* >( m_target )->m_speedFromHaste += mod->fixed_amount[mod->i];
		}
		else
		{
			m_target->ModBaseAttackTime(MELEE,mod->fixed_amount[mod->i] );
			m_target->ModBaseAttackTime(OFFHAND,mod->fixed_amount[mod->i*2] );

			if ( m_target->GetTypeId() == TYPEID_UNIT )
				static_cast< Creature* >( m_target )->m_speedFromHaste -= mod->fixed_amount[mod->i];
		}
	}
}

void Aura::SpellAuraForceReaction( bool apply )
{
	map<uint32,uint32>::iterator itr;
	if( p_target == NULL )
		return;

	if( apply )
	{
		itr = p_target->m_forcedReactions.find( mod->m_miscValue );
		if( itr != p_target->m_forcedReactions.end() )
			itr->second = mod->m_amount;
		else
			p_target->m_forcedReactions.insert( make_pair( mod->m_miscValue, mod->m_amount ) );
	}
	else
		p_target->m_forcedReactions.erase( mod->m_miscValue );

	WorldPacket data( SMSG_SET_FORCED_REACTIONS, ( 8 * p_target->m_forcedReactions.size() ) + 4 );
	data << uint32(p_target->m_forcedReactions.size());
	for( itr = p_target->m_forcedReactions.begin(); itr != p_target->m_forcedReactions.end(); ++itr )
	{
		data << itr->first;
		data << itr->second;
	}

	p_target->GetSession()->SendPacket( &data );
}

void Aura::SpellAuraModRangedHaste( bool apply )
{
	if( mod->m_amount < 0 )
		SetNegative();
	else
		SetPositive();

	if( p_target != NULL )
	{
//		int32 amount = mod->m_amount;
//		if(GetSpellProto()->Id == 6150)// Quick Shots
//		{
//			Unit * pCaster = GetUnitCaster();
//			if(pCaster)
//				SM_FIValue(pCaster->SM_FSPELL_VALUE,&amount,0x100000);
//		}

		if( apply )
			p_target->ModAttackSpeed( mod->m_amount, MOD_RANGED );
		else
			p_target->ModAttackSpeed( -mod->m_amount, MOD_RANGED );

		p_target->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,mod->m_amount);
			m_target->ModBaseAttackTime(RANGED,-mod->fixed_amount[mod->i]);
		}
		else m_target->ModBaseAttackTime(RANGED,mod->fixed_amount[mod->i]);
	}
}

void Aura::SpellAuraModRangedAmmoHaste( bool apply )
{
	SetPositive();
	if( p_target == NULL )
		return;

	if( apply )
		p_target->ModAttackSpeed( mod->m_amount, MOD_RANGED );
	else
		p_target->ModAttackSpeed( -mod->m_amount, MOD_RANGED );

	p_target->UpdateAttackSpeed();
}

void Aura::SpellAuraModResistanceExclusive(bool apply)
{
	SpellAuraModResistance(apply);
}

void Aura::SpellAuraRetainComboPoints(bool apply)
{
	if( p_target != NULL )
		p_target->m_retainComboPoints = apply;
}

void Aura::SpellAuraResistPushback(bool apply)
{
	//DK:This is resist for spell casting delay
	//Only use on players for now

	if( p_target != NULL )
	{
		int32 val = 0;
		if( apply )
		{
			val = mod->m_amount;
			SetPositive();
		}
		else
			val = -mod->m_amount;

		for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
		{
			if (mod->m_miscValue & ( ( (uint32) 1 ) << x ) )
			{
				p_target->SpellDelayResist[x] += val;
			}
		}
	}
}

void Aura::SpellAuraModShieldBlockPCT( bool apply )
{
	if( p_target != NULL )
	{
		if( apply )
		{
			p_target->m_modblockabsorbvalue += ( uint32 )mod->m_amount;
		}
		else
		{
			p_target->m_modblockabsorbvalue -= ( uint32 )mod->m_amount;
		}
		p_target->UpdateStats();
	}
}

void Aura::SpellAuraTrackStealthed(bool apply)
{
	Unit * c = GetUnitCaster();
	if( c == NULL )
		return;

	c->trackStealth = apply;
}

void Aura::SpellAuraModDetectedRange(bool apply)
{
	if( p_target == NULL )
		return;
	if(apply)
	{
		SetPositive();
		p_target->DetectedRange += mod->m_amount;
	}
	else
	{
		p_target->DetectedRange -= mod->m_amount;
	}
}

void Aura::SpellAuraSplitDamageFlat(bool apply)
{
	if( m_target->m_damageSplitTarget )
	{
		delete m_target->m_damageSplitTarget;
		m_target->m_damageSplitTarget = NULL;
	}

	if( apply )
	{
		DamageSplitTarget *ds = new DamageSplitTarget;
		ds->m_flatDamageSplit = mod->m_miscValue;
		ds->m_spellId = GetSpellProto()->Id;
		ds->m_pctDamageSplit = 0;
		ds->damage_type = static_cast<uint8>( mod->m_type );
		ds->creator = (void*)this;
		ds->m_target = m_casterGuid;
		m_target->m_damageSplitTarget = ds;
//		printf("registering dmg split %u, amount= %u \n",ds->m_spellId, mod->m_amount, mod->m_miscValue, mod->m_type);
	}
}

void Aura::SpellAuraModStealthLevel(bool apply)
{
	if(apply)
	{
		SetPositive();
		m_target->m_stealthLevel += mod->m_amount;
	}
	else
		m_target->m_stealthLevel -= mod->m_amount;
}

void Aura::SpellAuraModUnderwaterBreathing(bool apply)
{
	if( p_target != NULL )
	{
		uint32 m_UnderwaterMaxTimeSaved = p_target->m_UnderwaterMaxTime;
		if( apply )
			p_target->m_UnderwaterMaxTime *= (1 + mod->m_amount / 100 );
		else
			p_target->m_UnderwaterMaxTime /= (1 + mod->m_amount / 100 );
		p_target->m_UnderwaterTime *= p_target->m_UnderwaterMaxTime / m_UnderwaterMaxTimeSaved;
	}
}

void Aura::SpellAuraSafeFall(bool apply)
{
	//FIXME:Find true flag
	if( p_target != NULL )
	{
		if( apply )
		{
			p_target->m_safeFall += mod->m_amount;
        }
        else
        {
			p_target->m_safeFall -= mod->m_amount;
        }
	}
}

void Aura::SpellAuraModReputationAdjust(bool apply)
{
	/*SetPositive();
	bool updateclient = true;
	if(IsPassive())
		updateclient = false;	 // don't update client on passive

	if(m_target->GetTypeId()==TYPEID_PLAYER)
	{
		if(apply)
			  static_cast< Player* >( m_target )->modPercAllReputation(mod->m_amount, updateclient);
		  else
			static_cast< Player* >( m_target )->modPercAllReputation(-mod->m_amount, updateclient);
	}*/

	// This is _actually_ "Reputation gains increased by x%."
	// not increase all rep by x%.

	if( p_target != NULL )
	{
		SetPositive();
		if( apply )
			p_target->pctReputationMod += mod->m_amount;
		else
			p_target->pctReputationMod -= mod->m_amount;
	}
}

void Aura::SpellAuraNoPVPCredit(bool apply)
{
	if( p_target == NULL )
		return;

	if( apply )
		p_target->m_honorless++;
	else
		p_target->m_honorless--;
}

void Aura::SpellAuraModHealthRegInCombat(bool apply)
{
	// demon armor etc, they all seem to be 5 sec.
	if(apply)
	{
		sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1, uint32(mod->m_amount), EVENT_AURA_PERIODIC_HEALINCOMB, 5000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicBurn(uint32 amount, uint32 misc)
{
	Unit*m_caster = GetUnitCaster();

	if(!m_caster)
		return;

	if(m_target->isAlive() && m_caster->isAlive())
	{
		if(m_target->SchoolImmunityList[GetSpellProto()->School])
			return;

		uint32 Amount = (uint32)min( amount, m_target->GetPower( misc ) );
		uint32 newHealth = m_target->GetPower(misc) - Amount ;

		m_target->SendPeriodicAuraLog( m_target->GetNewGUID(), m_target->GetNewGUID(), m_spellProto->Id, m_spellProto->School, newHealth, 0, 0, FLAG_PERIODIC_DAMAGE, false);
		m_caster->DealDamage(m_target, Amount, 0, 0, GetSpellProto()->Id);
	}
}

void Aura::SpellAuraPowerBurn(bool apply)
{
	//0 mana,1 rage, 3 energy
	if(apply)
		sEventMgr.AddEvent(this, &Aura::EventPeriodicBurn, uint32(mod->m_amount), (uint32)mod->m_miscValue, EVENT_AURA_PERIODIC_BURN, GetSpellProto()->EffectAmplitude[mod->i], 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Aura::SpellAuraModCritDmgPhysical(bool apply)
{
	if( p_target != NULL )
	{
		if( apply )
		{
			p_target->m_modphyscritdmgPCT += (uint32)mod->m_amount;
		}
		else
		{
			p_target->m_modphyscritdmgPCT -= (uint32)mod->m_amount;
		}
	}
}


void Aura::SpellAuraWaterBreathing( bool apply )
{
   if( p_target != NULL )
   {
	   if( apply )
	   {
			SetPositive();
			WorldPacket data( 4 );
			data.SetOpcode( SMSG_STOP_MIRROR_TIMER );
			data << uint32( 1 );
			p_target->GetSession()->SendPacket( &data );
			p_target->m_UnderwaterState = 0;
	   }

	   p_target->m_bUnlimitedBreath = apply;
   }
}

void Aura::SpellAuraAPAttackerBonus(bool apply)
{
	if(apply)
	{
		m_target->APvModifier += mod->m_amount;
	}
	else
		m_target->APvModifier -= mod->m_amount;
}


void Aura::SpellAuraModPAttackPower(bool apply)
{
	//!!probably there is a flag or something that will signal if randeg or melee attack power !!! (still missing)
	if( p_target != NULL )
	{
		if( apply )
		{
			p_target->ModAttackPowerMultiplier((float)mod->m_amount / 100.0f );
		}
		else
			p_target->ModAttackPowerMultiplier(-(float)mod->m_amount / 100.0f );
		p_target->CalcDamage();
	}
}

void Aura::SpellAuraModRangedAttackPowerPct(bool apply)
{
    if(m_target->IsPlayer())
	{
		m_target->ModRangedAttackPowerMultiplier(((apply)?1:-1)*(float)mod->m_amount/100);
        m_target->CalcDamage();
    }
}

void Aura::SpellAuraIncreaseDamageTypePCT(bool apply)
{
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			for(uint32 x = 0; x < 11; x++)
				if (mod->m_miscValue & (((uint32)1)<<x) )
					static_cast< Player* >( m_target )->IncreaseDamageByTypePCT[x+1] += ((float)(mod->m_amount))/100;
			if(mod->m_amount < 0)
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			for(uint32 x = 0; x < 11; x++)
			{
				if (mod->m_miscValue & (((uint32)1)<<x) )
					static_cast< Player* >( m_target )->IncreaseDamageByTypePCT[x+1] -= ((float)(mod->m_amount))/100;
			}
		}
	}
}

void Aura::SpellAuraIncreaseCricticalTypePCT(bool apply)
{
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			for(uint32 x = 0; x < 11; x++)
				if (mod->m_miscValue & (((uint32)1)<<x) )
					static_cast< Player* >( m_target )->IncreaseCricticalByTypePCT[x+1] += ((float)(mod->m_amount))/100;
			if(mod->m_amount < 0)
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			for(uint32 x = 0; x < 11; x++)
			{
				if (mod->m_miscValue & (((uint32)1)<<x) )
					static_cast< Player* >( m_target )->IncreaseCricticalByTypePCT[x+1] -= ((float)(mod->m_amount))/100;
			}
		}
	}
}

void Aura::SpellAuraIncreasePartySpeed(bool apply)
{
	if(m_target->GetTypeId() == TYPEID_PLAYER && m_target->isAlive() && m_target->GetMount() == 0)
	{
		if(apply)
		{
			m_target->m_speedModifier += mod->m_amount;
		}
		else
		{
			m_target->m_speedModifier -= mod->m_amount;
		}
		m_target->UpdateSpeed();
	}
}

void Aura::SpellAuraIncreaseSpellDamageByAttribute(bool apply)
{
	int32 val;

	if(apply)
	{
		val = mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();

		mod->fixed_amount[mod->i] = val; //we wish to have the same amount when we are removing the spell as when we were applying !
	}
	else
		val = -mod->fixed_amount[mod->i];

	uint32 stat = 3;
	for(uint32 i= 0; i < 3; i++)
	{ //bit hacky but it will work with all currently available spells
		if (m_spellProto->EffectApplyAuraName[i] == SPELL_AURA_INCREASE_SPELL_HEALING_PCT)
		{
			if (m_spellProto->EffectMiscValue[i] < 5)
				stat = m_spellProto->EffectMiscValue[i];
			else
				return;
		}
	}

	if(m_target->IsPlayer())
	{
		for(uint32 x=1;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				if( apply )
				{
					mod->realamount = float2int32(((float)val/100)*m_target->GetStat(stat));
					p_target->ModPosDamageDoneMod( x, mod->realamount );
				}
				else
					p_target->ModPosDamageDoneMod( x, -mod->realamount );
			}
		}
		p_target->UpdateChanceFields();
	}
}

void Aura::SpellAuraModSpellDamageByAP(bool apply)
{
	int32 val;

	if(apply)
	{
		//!! caster may log out before spell expires on target !
		Unit * pCaster = GetUnitCaster();
		if( pCaster == NULL )
			return;

		val = mod->m_amount * pCaster->GetAP() / 100;
		if( val < 0 )
			SetNegative();
		else
			SetPositive();

		mod->fixed_amount[mod->i] = val; //we wish to have the same amount when we are removing the spell as when we were applying !
	}
	else
		val = -mod->fixed_amount[mod->i];

	if(m_target->IsPlayer())
	{
		for(uint32 x=1;x<7;x++) //melee damage != spell damage.
			if (mod->m_miscValue & (((uint32)1)<<x) )
				p_target->ModPosDamageDoneMod( x, val );
		
		p_target->UpdateChanceFields();
	}
}

void Aura::SpellAuraIncreaseHealingByAttribute(bool apply)
{
	int32 val;

	if(apply)
	{
		val = mod->m_amount;

		if(val<0)
			SetNegative();
		else
			SetPositive();

		mod->fixed_amount[mod->i] = val; //we wish to have the same amount when we are removing the spell as when we were applying !
	}
	else
		val = -mod->fixed_amount[mod->i];

	uint32 stat;
	if (mod->m_miscValue < 5)
		stat = mod->m_miscValue;
	else
	{
		sLog.outError(
			"Aura::SpellAuraIncreaseHealingByAttribute::Unknown spell attribute type %u in spell %u.\n",
			mod->m_miscValue,GetSpellId());
		return;
	}

	if( p_target != NULL )
	{
		for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
		{
			p_target->SpellHealDoneByAttribute[stat][x] += (float)val / 100.0f;
		}
		p_target->UpdateChanceFields();
		if( apply )
		{
			mod->realamount = float2int32( ( (float)val / 100.0f ) * p_target->GetStat(stat ) );
			p_target->ModHealingDoneMod(mod->realamount );
		}
		else
			p_target->ModHealingDoneMod(-mod->realamount );
	}
}

void Aura::SpellAuraModHealingByAP(bool apply)
{
	int32 val;

	if(apply)
	{
		//!! caster may log out before spell expires on target !
		Unit * pCaster = GetUnitCaster();
		if( pCaster == NULL )
			return;

		val = mod->m_amount * pCaster->GetAP() / 100;
		if( val < 0 )
			SetNegative();
		else
			SetPositive();

		mod->fixed_amount[mod->i] = val; //we wish to have the same amount when we are removing the spell as when we were applying !
	}
	else
		val = -mod->fixed_amount[mod->i];

	

	for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
	{
		if( mod->m_miscValue  & (((uint32)1) << x ) )
		{
			m_target->HealDoneMod[x] += val;
		}
	}

	if( p_target != NULL )
	{
		p_target->ModHealingDoneMod(val );
		p_target->UpdateChanceFields();
	}
}

void Aura::SpellAuraAddFlatModifier(bool apply)
{
	int32 val = apply ? mod->m_amount : -mod->m_amount;
	uint32* AffectedGroups = GetSpellProto()->EffectSpellClassMask[mod->i];

	switch (mod->m_miscValue)//let's generate warnings for unknown types of modifiers
	{
	case SMT_DAMAGE_DONE:
		SendModifierLog(&m_target->SM_FDamageBonus,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_DURATION:
		SendModifierLog(&m_target->SM_FDur,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_THREAT_REDUCED:
		SendModifierLog(&m_target->SM_FThreat, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_EFFECT_1:
		SendModifierLog(&m_target->SM_FEffect1_Bonus, val, AffectedGroups,  static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_CHARGES:
		SendModifierLog(&m_target->SM_FCharges, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_RANGE:
		SendModifierLog(&m_target->SM_FRange,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_RADIUS:
		SendModifierLog(&m_target->SM_FRadius,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_CRITICAL:
		SendModifierLog(&m_target->SM_CriticalChance,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_MISC_EFFECT:
		SendModifierLog(&m_target->SM_FMiscEffect,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	//case SMT_NONINTERRUPT: - no flat

	case SMT_CAST_TIME:
		SendModifierLog(&m_target->SM_FCastTime,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_COOLDOWN_DECREASE:
		SendModifierLog(&m_target->SM_FCooldownTime, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_EFFECT_2:
		SendModifierLog(&m_target->SM_FEffect2_Bonus, val, AffectedGroups,  static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_COST:
		SendModifierLog(&m_target->SM_FCost,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	//case SMT_CRITICAL_DAMAGE: - no flat

	case SMT_HITCHANCE:
		SendModifierLog(&m_target->SM_FHitchance,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_ADDITIONAL_TARGET:
		SendModifierLog(&m_target->SM_FAdditionalTargets,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_TRIGGER:
		SendModifierLog(&m_target->SM_FChanceOfSuccess,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_AMPTITUDE:
		SendModifierLog(&m_target->SM_FAmptitude,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	//case SMT_JUMP_REDUCE: - no flat

	case SMT_GLOBAL_COOLDOWN:
		SendModifierLog(&m_target->SM_FGlobalCooldown,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	//case SMT_SPELL_VALUE_PCT: - pct only?
		//SendModifierLog(&m_target->SM_FDOT,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		//break;

	case SMT_EFFECT_3:
		SendModifierLog(&m_target->SM_FEffect3_Bonus, val, AffectedGroups,  static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_PENALTY:
		SendModifierLog(&m_target->SM_FPenalty,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	case SMT_EFFECT_BONUS:
		SendModifierLog(&m_target->SM_FEffectBonus,val,AffectedGroups, static_cast<uint8>( mod->m_miscValue ),true);
		break;

	case SMT_RESIST_DISPEL:
		SendModifierLog(&m_target->SM_FRezist_dispell, val, AffectedGroups, static_cast<uint8>( mod->m_miscValue ));
		break;

	default://Unknown modifier type
		sLog.outError(
			"Unknown spell modifier type %u in spell %u.<<--report this line to the developer\n",
			mod->m_miscValue,GetSpellId());
		break;
	}

	//Hunter's BeastMastery talents.
	if( m_target->IsPlayer() )
	{
		Pet * p = static_cast< Player * >(m_target)->GetSummon();
		if( p )
		{
			switch( GetSpellProto()->NameHash )
			{
			case SPELL_HASH_UNLEASHED_FURY:
				p->LoadPetAuras(0);
				break;
			case SPELL_HASH_THICK_HIDE:
				p->LoadPetAuras(1);
				break;
			case SPELL_HASH_ENDURANCE_TRAINING:
				p->LoadPetAuras(2);
				break;
			case SPELL_HASH_FERAL_SWIFTNESS:
				p->LoadPetAuras(3);
				break;
			case SPELL_HASH_BESTIAL_DISCIPLINE:
				p->LoadPetAuras(4);
				break;
			case SPELL_HASH_FEROCITY:
				p->LoadPetAuras(5);
				break;
			case SPELL_HASH_ANIMAL_HANDLER:
				p->LoadPetAuras(6);
				break;
			case SPELL_HASH_CATLIKE_REFLEXES:
				p->LoadPetAuras(7);
				break;
			case SPELL_HASH_SERPENT_S_SWIFTNESS:
				p->LoadPetAuras(8);
				break;
			}
		}
	}
}

void Aura::SpellAuraModHealingDone(bool apply)
{
	int32 val;
	if( apply )
	{
		val = mod->m_amount;
		if( val < 0 )
			SetNegative();
		else
			SetPositive();
	}
	else
		val = -mod->m_amount;
	
	uint32 player_class = m_target->getClass();
	if( player_class == DRUID || player_class == PALADIN || player_class == SHAMAN || player_class == PRIEST )
		val = float2int32( val * 1.88f );

	for( uint8 x = 0; x < SCHOOL_COUNT; x++ )
	{
		if( mod->m_miscValue  & (((uint32)1) << x ) )
		{
			m_target->HealDoneMod[x] += val;
		}
	}
	if( p_target != NULL )
	{
		p_target->UpdateChanceFields();
		p_target->ModHealingDoneMod(val );
	}
}

void Aura::SpellAuraModHealingDonePct(bool apply)
{
	int32 val;
	if(apply)
	{
		val=mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();
	}
	else
		val=-mod->m_amount;

	for(uint32 x= 0;x<7;x++)
	{
		if (mod->m_miscValue  & (((uint32)1)<<x) )
		{
			m_target->HealDonePctMod[x] += ( (float)(val) )/100;
		}
	}
}

void Aura::SpellAuraEmphaty(bool apply)
{
	SetPositive();
	Player * caster = GetPlayerCaster();
	if(caster == NULL)
		return;

	// Show extra info about beast
	uint32 dynflags = m_target->GetUInt32Value(UNIT_DYNAMIC_FLAGS);
	if(apply)
		dynflags |= U_DYN_FLAG_PLAYER_INFO;

	m_target->BuildFieldUpdatePacket(caster, UNIT_DYNAMIC_FLAGS, dynflags);
}

void Aura::SpellAuraModOffhandDamagePCT(bool apply)
{
	//Used only by talents of rogue and warrior;passive,positive
	if( p_target != NULL )
	{
		if( apply )
		{
			SetPositive();
			p_target->offhand_dmg_mod *= ( 100 + mod->m_amount ) / 100.0f;
		}
		else
			p_target->offhand_dmg_mod /= ( 100 + mod->m_amount ) / 100.0f;

		p_target->CalcDamage();
	}
}

void Aura::SpellAuraModPenetration(bool apply) // armor penetration & spell penetration
{
	if( m_spellProto->NameHash == SPELL_HASH_SERRATED_BLADES )
	{
		if( p_target == NULL )
			return;

		if( apply )
		{
			if( m_spellProto->Id == 14171 )
				p_target->PowerCostPctMod[0] += m_target->getLevel() * 2.67f;
			else if( m_spellProto->Id == 14172 )
				p_target->PowerCostPctMod[0] += m_target->getLevel() * 5.43f;
			else if( m_spellProto->Id == 14173 )
				p_target->PowerCostPctMod[0] += m_target->getLevel() * 8.0f;
		}
		else
		{
			if( m_spellProto->Id == 14171 )
				p_target->PowerCostPctMod[0] -= m_target->getLevel() * 2.67f;
			else if( m_spellProto->Id == 14172 )
				p_target->PowerCostPctMod[0] -= m_target->getLevel() * 5.43f;
			else if( m_spellProto->Id == 14173 )
				p_target->PowerCostPctMod[0] -= m_target->getLevel() * 8.0f;
		}
		return;
	}
	if( apply )
	{
		if( mod->m_amount < 0 )
			SetPositive();
		else
			SetNegative();

		for( uint8 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & (((uint32)1) << x ) )
				m_target->PowerCostPctMod[x] -= mod->m_amount;
		}

		if( p_target != NULL )
		{
			if( mod->m_miscValue & 124 )
				m_target->ModSignedInt32Value( PLAYER_FIELD_MOD_TARGET_RESISTANCE, mod->m_amount );
			if( mod->m_miscValue & 1 )
				m_target->ModSignedInt32Value( PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE, mod->m_amount );
		}
	}
	else
	{
		for( uint8 x = 0; x < 7; x++ )
		{
			if( mod->m_miscValue & (((uint32)1)<<x) )
				m_target->PowerCostPctMod[x] += mod->m_amount;
		}
		if( p_target != NULL )
		{
			if( mod->m_miscValue & 124 )
				m_target->ModSignedInt32Value( PLAYER_FIELD_MOD_TARGET_RESISTANCE, -mod->m_amount );
			if( mod->m_miscValue & 1 )
				m_target->ModSignedInt32Value( PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE, -mod->m_amount );
		}
	}
}

void Aura::SpellAuraIncreaseArmorByPctInt(bool apply)
{
	uint32 i_Int = m_target->GetStat(STAT_INTELLECT);

	int32 amt = float2int32(i_Int * ((float)mod->m_amount / 100.0f));
	amt *= (!apply) ? -1 : 1;

	for( uint8 x = 0; x < 7; x++ )
	{
		if(mod->m_miscValue & (((uint32)1)<< x))
		{
			if( p_target != NULL )
			{
				p_target->FlatResistanceModifierPos[x] += amt;
				p_target->CalcResistance(x);
			}
			else if(m_target->GetTypeId() == TYPEID_UNIT)
			{
				static_cast<Creature*>(m_target)->FlatResistanceMod[x] += amt;
				static_cast<Creature*>(m_target)->CalcResistance(x);
			}
		}
	}
}

void Aura::SpellAuraReduceAttackerMHitChance(bool apply)
{
	if( p_target == NULL )
		return;
	if( apply )
		p_target->m_resist_hit[ MOD_MELEE ] += mod->m_amount;
	else
		p_target->m_resist_hit[ MOD_MELEE ] -= mod->m_amount;
}

void Aura::SpellAuraReduceAttackerRHitChance(bool apply)
{
	if( p_target == NULL )
		return;
	if( apply )
		p_target->m_resist_hit[ MOD_RANGED ] += mod->m_amount;
	else
		p_target->m_resist_hit[ MOD_RANGED ] -= mod->m_amount;
}

void Aura::SpellAuraReduceAttackerSHitChance(bool apply)
{
	if( p_target == NULL )
		return;
	for( uint8 i = 0; i < SCHOOL_COUNT; i++ )
	{
		if( mod->m_miscValue & ( 1 << i ) ) // check school
		{
			// signs reversed intentionally
			if( apply )
				p_target->m_resist_hit_spell[ i ] -= mod->m_amount;
			else
				p_target->m_resist_hit_spell[ i ] += mod->m_amount;
		}
	}
}

void Aura::SpellAuraReduceEnemyMCritChance(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		//value is negative percent
		static_cast< Player* >( m_target )->res_M_crit_set(static_cast< Player* >( m_target )->res_M_crit_get()+mod->m_amount);
	}
	else
	{
		static_cast< Player* >( m_target )->res_M_crit_set(static_cast< Player* >( m_target )->res_M_crit_get()-mod->m_amount);
	}
}

void Aura::SpellAuraReduceEnemyRCritChance(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		//value is negative percent
		static_cast< Player* >( m_target )->res_R_crit_set(static_cast< Player* >( m_target )->res_R_crit_get()+mod->m_amount);
	}
	else
	{
		static_cast< Player* >( m_target )->res_R_crit_set(static_cast< Player* >( m_target )->res_R_crit_get()-mod->m_amount);
	}
}

void Aura::SpellAuraLimitSpeed( bool apply )
{
	int32 amount = ( apply ) ? mod->m_amount : -mod->m_amount;
	m_target->m_maxSpeed += (float)amount;
	m_target->UpdateSpeed();
}
void Aura::SpellAuraIncreaseTimeBetweenAttacksPCT(bool apply)
{
	int32 val =  (apply) ? mod->m_amount : -mod->m_amount;
	float pct_value = -val/100.0f;
	m_target->ModCastSpeedMod(pct_value);
}

void Aura::SpellAuraMeleeHaste( bool apply )
{
	if( mod->m_amount < 0 )
		SetNegative();
	else
		SetPositive();

	if( m_target->IsPlayer() )
	{
		if( apply )
			static_cast< Player* >( m_target )->ModAttackSpeed( mod->m_amount, MOD_MELEE );
		else
			static_cast< Player* >( m_target )->ModAttackSpeed( -mod->m_amount, MOD_MELEE );

		static_cast< Player* >(m_target)->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME, mod->m_amount );
			mod->fixed_amount[1] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME+1, mod->m_amount );

			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ) <= mod->fixed_amount[0] )
				mod->fixed_amount[0] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME );
			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME+1 ) <= mod->fixed_amount[1] )
				mod->fixed_amount[1] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME+1 );

			m_target->ModBaseAttackTime(MELEE,-mod->fixed_amount[0] );
			m_target->ModBaseAttackTime(OFFHAND,-mod->fixed_amount[1] );
		}
		else
		{
			m_target->ModBaseAttackTime(MELEE,mod->fixed_amount[0] );
			m_target->ModBaseAttackTime(OFFHAND,mod->fixed_amount[1] );
		}
	}
}

/*
void Aura::SpellAuraIncreaseSpellDamageByInt(bool apply)
{
	 float val;
	 if(apply)
	 {
		 val = mod->m_amount/100.0f;
		 if(mod->m_amount>0)
			 SetPositive();
		 else
			 SetNegative();
	 }
	 else
		val =- mod->m_amount/100.0f;

	if(m_target->IsPlayer())
	{
		for(uint32 x=1;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				static_cast< Player* >( m_target )->SpellDmgDoneByInt[x]+=val;
			}
		}
	}
}

void Aura::SpellAuraIncreaseHealingByInt(bool apply)
{
	 float val;
	 if(apply)
	 {
		 val = mod->m_amount/100.0f;
		 if(val>0)
			 SetPositive();
		 else
			 SetNegative();
	 }
	 else
		val =- mod->m_amount/100.0f;

	if(m_target->IsPlayer())
	{
		for(uint32 x=1;x<7;x++)
		{
   //		 if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				static_cast< Player* >( m_target )->SpellHealDoneByInt[x]+=val;
			}
		}
	}
}
*/
void Aura::SpellAuraModAttackerCritChance(bool apply)
{
	int32 val  = (apply) ? mod->m_amount : -mod->m_amount;
	m_target->AttackerCritChanceMod[0] +=val;
}

void Aura::SpellAuraIncreaseAllWeaponSkill(bool apply)
{
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			SetPositive();
//			static_cast< Player* >( m_target )->ModSkillBonusType(SKILL_TYPE_WEAPON, mod->m_amount);
			//since the frikkin above line does not work we have to do it manually
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_SWORDS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_AXES, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_BOWS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_GUNS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_MACES, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_2H_SWORDS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_STAVES, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_2H_MACES, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_2H_AXES, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_DAGGERS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_CROSSBOWS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_WANDS, mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_POLEARMS, mod->m_amount);
		}
		else
		{
//			static_cast< Player* >( m_target )->ModSkillBonusType(SKILL_TYPE_WEAPON, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_SWORDS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_AXES, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_BOWS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_GUNS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_MACES, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_2H_SWORDS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_STAVES, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_2H_MACES, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_2H_AXES, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_DAGGERS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_CROSSBOWS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_WANDS, -mod->m_amount);
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_POLEARMS, -mod->m_amount);
		}

		static_cast< Player* >( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraIncreaseHitRate( bool apply )
{
	if( !m_target->IsPlayer() )
		return;

	static_cast< Player* >( m_target )->ModifyBonuses( SPELL_HIT_RATING, mod->m_amount, apply );
	static_cast< Player* >( m_target )->UpdateStats();
}

void Aura::SpellAuraIncreaseRageFromDamageDealtPCT(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	static_cast< Player* >( m_target )->rageFromDamageDealt += (apply) ? mod->m_amount : -mod->m_amount;
}

int32 Aura::event_GetInstanceID()
{
	return m_target->event_GetInstanceID();
}

void Aura::RelocateEvents()
{
	event_Relocate();
}

void Aura::SpellAuraReduceCritMeleeAttackDmg(bool apply)
{
	signed int val;
	if(apply)
		val = mod->m_amount;
	else
		val = -mod->m_amount;

	for(uint32 x=1;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->CritMeleeDamageTakenPctMod[x] += val/100.0f;
}

void Aura::SpellAuraReduceCritRangedAttackDmg(bool apply)
{
	signed int val;
	if(apply)
		val = mod->m_amount;
	else
		val = -mod->m_amount;

	for(uint32 x=1;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->CritRangedDamageTakenPctMod[x] += val/100.0f;
}

void Aura::SpellAuraEnableFlight(bool apply)
{
	if(apply)
	{
		m_target->EnableFlight();
		m_target->m_flyspeedModifier += mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			static_cast< Player* >( m_target )->flying_aura = m_spellProto->Id;
		}
	}
	else
	{
		m_target->DisableFlight();
		m_target->m_flyspeedModifier -= mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			static_cast< Player* >( m_target )->flying_aura = 0;
		}
	}
}

void Aura::SpellAuraEnableFlightWithUnmountedSpeed(bool apply)
{
	// Used in flight form (only so far)
	if(apply)
	{
		m_target->EnableFlight();
		m_target->m_flyspeedModifier += mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			static_cast< Player* >( m_target )->flying_aura = m_spellProto->Id;
		}
	}
	else
	{
		m_target->DisableFlight();
		m_target->m_flyspeedModifier -= mod->m_amount;
		m_target->UpdateSpeed();
		if(m_target->IsPlayer())
		{
			static_cast< Player* >( m_target )->flying_aura = 0;
		}
	}
}

void Aura::SpellAuraIncreaseMovementAndMountedSpeed( bool apply )
{
	if( apply )
		m_target->m_mountedspeedModifier += mod->m_amount;
	else
		m_target->m_mountedspeedModifier -= mod->m_amount;
	m_target->UpdateSpeed();
}

void Aura::SpellAuraIncreaseFlightSpeed( bool apply )
{
	if( apply )
		m_target->m_flyspeedModifier += mod->m_amount;
	else
		m_target->m_flyspeedModifier -= mod->m_amount;
	m_target->UpdateSpeed();
}


void Aura::SpellAuraIncreaseRating( bool apply )
{
	int v = apply ? mod->m_amount : -mod->m_amount;

	if( !m_target->IsPlayer() )
		return;

	Player* plr = static_cast< Player* >( m_target );
	for( uint32 x = 1; x < 24; x++ )//skip x= 0
		if( ( ( ( uint32 )1 ) << x ) & mod->m_miscValue )
			plr->ModifyBonuses( 11 + x, mod->m_amount, apply );

	//MELEE_CRITICAL_AVOIDANCE_RATING + RANGED_CRITICAL_AVOIDANCE_RATING + SPELL_CRITICAL_AVOIDANCE_RATING
	//comes only as combination of them  - ModifyBonuses() not adding them individually anyhow
	if( mod->m_miscValue & (0x0004000|0x0008000|0x0010000) )
			plr->ModifyBonuses( RESILIENCE_RATING, mod->m_amount, apply );

	if( mod->m_miscValue & 1 )//weapon skill
	{
		std::map<uint32, uint32>::iterator i;
		for( uint32 y = 0; y < 20; y++ )
			if( m_spellProto->EquippedItemSubClass & ( ( ( uint32 )1 ) << y ) )
			{
					i = static_cast< Player* >( m_target )->m_wratings.find( y );
					if( i == static_cast< Player* >( m_target )->m_wratings.end() )//no prev
					{
						static_cast< Player* >( m_target )->m_wratings[y] = v;
					}else
					{
						i->second += v;
						if( i->second == 0 )
							static_cast< Player* >( m_target )->m_wratings.erase( i );
					}
			}
	}

	plr->UpdateStats();
}

void Aura::EventPeriodicRegenManaStatPct(uint32 perc,uint32 stat)
{
	if(m_target->IsDead())
		return;

	uint32 mana = m_target->GetPower( POWER_TYPE_MANA ) + (m_target->GetStat(stat)*perc)/100;

	if(mana <= m_target->GetMaxPower( POWER_TYPE_MANA ) )
		m_target->SetPower( POWER_TYPE_MANA, mana);
	else
		m_target->SetPower( POWER_TYPE_MANA, m_target->GetMaxPower( POWER_TYPE_MANA ));
}

void Aura::SpellAuraRegenManaStatPCT(bool apply)
{
	if(apply)
	{
		SetPositive();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicRegenManaStatPct,(uint32)mod->m_amount,(uint32)mod->m_miscValue,  EVENT_AURA_REGEN_MANA_STAT_PCT, 5000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::SpellAuraSpellHealingStatPCT(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	if(apply)
	{
		//SetPositive();
		/*mod->realamount = ( mod->m_amount * m_target->GetStat(mod->m_miscValue ) /1 00;

		for( uint32 x = 1; x < 7; x++ )
			m_target->HealDoneMod[x] += mod->realamount;*/

		mod->realamount = ( ( m_target->GetUInt32Value(UNIT_FIELD_SPIRIT) * mod->m_amount ) / 100 );

		TO_PLAYER( m_target )->ModifyBonuses( CRITICAL_STRIKE_RATING, mod->realamount, true );
		TO_PLAYER( m_target )->UpdateChances();
	}
	else
	{
		/*for( uint32 x = 1; x < 7; x++ )
			m_target->HealDoneMod[x] -= mod->realamount;*/

		TO_PLAYER( m_target )->ModifyBonuses( CRITICAL_STRIKE_RATING, mod->realamount, false );
		TO_PLAYER( m_target )->UpdateChances();
	}
}

void Aura::SpellAuraAllowFlight(bool apply)
{
    // allow fly
    WorldPacket data;
    if(apply)
        data.Initialize(SMSG_MOVE_SET_CAN_FLY);
    else
        data.Initialize(SMSG_MOVE_UNSET_CAN_FLY);
    //data.append(m_target->m_PackGUID);
    data << uint32(0);                                      // unk
    m_target->SendMessageToSet(&data, true);
}

void Aura::SpellAuraFinishingMovesCannotBeDodged(bool apply)
{
	if(apply)
	{
		if( !m_target->IsPlayer() )
			return;

		static_cast< Player* >( m_target )->m_finishingmovesdodge = true;
	}
	else
	{
		if( !m_target->IsPlayer() )
			return;

		static_cast< Player* >( m_target )->m_finishingmovesdodge = false;
	}
}

void Aura::SpellAuraModStealthDetection(bool apply)
{
	if(apply)
	{
		m_target->m_stealthDetectBonus += 9999;
	}
	else
		m_target->m_stealthDetectBonus -= 9999;
}

void Aura::SpellAuraReduceAOEDamageTaken(bool apply)
{
	float val = mod->m_amount / 100.0f;
	if( apply )
	{
		mod->fixed_amount[0] = (int)(m_target->AOEDmgMod * val);
		m_target->AOEDmgMod += mod->fixed_amount[0];
	}
	else
		m_target->AOEDmgMod -= mod->fixed_amount[0];
}

void Aura::SpellAuraIncreaseMaxHealth(bool apply)
{
	//should only be used by a player
	//and only ever target players
	if( !m_target->IsPlayer() )
		return;

	int32 amount;
	if( apply )
		amount = mod->m_amount;
	else
		amount = -mod->m_amount;

	static_cast< Player* >( m_target )->SetHealthFromSpell( static_cast< Player* >( m_target )->GetHealthFromSpell() + amount );
	static_cast< Player* >( m_target )->UpdateStats();
}

void Aura::SpellAuraSpiritOfRedemption(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	if(apply)
	{
		m_target->SetScale(  0.5);
		m_target->SetHealth( 1);
		SpellEntry * sorInfo = dbcSpell.LookupEntry(27792);
		Spell * sor = new Spell(m_target, sorInfo, true, NULL);
		SpellCastTargets targets;
		targets.m_unitTarget = m_target->GetGUID();
		sor->prepare(&targets);
	}
	else
	{
		m_target->SetScale(  1);
		m_target->RemoveAura(27792);
		m_target->SetHealth( 0);
	}
}

void Aura::SpellAuraIncreaseAttackerSpellCrit(bool apply)
{
	int32 val = mod->m_amount;

	if (apply)
	{
		if (mod->m_amount>0)
			SetNegative();
		else
			SetPositive();
	}
	else
		val = -val;

	for(uint32 x= 0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x))
			m_target->AttackerCritChanceMod[x] += val;
	}
}

void Aura::SpellAuraIncreaseRepGainPct(bool apply)
{
	if(p_target)
	{
		SetPositive();
		if(apply)
			p_target->pctReputationMod += mod->m_amount;//re use
		else
			p_target->pctReputationMod -= mod->m_amount;//re use
	}
}

void Aura::SpellAuraIncreaseRAPbyStatPct( bool apply )
{
	if( apply )
	{
		if( mod->m_amount > 0 )
			SetPositive();
		else
			SetNegative();

		mod->fixed_amount[mod->i] = m_target->GetStat(mod->m_miscValue ) * mod->m_amount / 100;
		m_target->ModRangedAttackPowerMods(mod->fixed_amount[mod->i] );
	}
	else
		m_target->ModRangedAttackPowerMods(-mod->fixed_amount[mod->i] );

	m_target->CalcDamage();
}

/* not used
void Aura::SpellAuraModRangedDamageTakenPCT(bool apply)
{
	if(apply)
		m_target->RangedDamageTakenPct += mod->m_amount;
	else
		m_target->RangedDamageTakenPct -= mod->m_amount;
}*/

void Aura::SpellAuraModBlockValue(bool apply)
{
	if( p_target != NULL)
 	{
		int32 amt;
 		if( apply )
 		{
			amt = mod->m_amount;
			if( amt < 0 )
				SetNegative();
			else
				SetPositive();
 		}
		else
		{
			amt = -mod->m_amount;
		}
		p_target->m_modblockvaluefromspells += amt;
		p_target->UpdateStats();
	}
}

void Aura::SendInterrupted(uint8 result, Object * m_caster)
{
	if( !m_caster->IsInWorld() )
		return;

	WorldPacket data( SMSG_SPELL_FAILURE, 20 );
	if( m_caster->IsPlayer() )
	{
		data << m_caster->GetNewGUID();
		data << m_spellProto->Id;
		data << uint8( result );
		static_cast< Player* >( m_caster )->GetSession()->SendPacket( &data );
	}

	data.Initialize( SMSG_SPELL_FAILED_OTHER );
	data << m_caster->GetNewGUID();
	data << m_spellProto->Id;
	m_caster->SendMessageToSet( &data, false );

	m_interrupted = (int16)result;
}

void Aura::SendChannelUpdate(uint32 time, Object * m_caster)
{
	WorldPacket data(MSG_CHANNEL_UPDATE, 18);
	data << m_caster->GetNewGUID();
	data << time;

	m_caster->SendMessageToSet(&data, true);
}

void Aura::SpellAuraExpertise(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	static_cast< Player* >( m_target )->CalcExpertise();
}

void Aura::SpellAuraModPossessPet(bool apply)
{
	Player* pCaster = GetPlayerCaster();;
	if( pCaster == NULL || ! pCaster->IsInWorld() )
		return;

	if( !m_target->IsPet() )
		return;

	std::list<Pet*> summons = pCaster->GetSummons();
	for(std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
	{
		if( *itr == m_target )
		{	
			if( apply )
			{
				pCaster->Possess( m_target );
				pCaster->SpeedCheatDelay( GetDuration() );
			}
			else
			{
				pCaster->UnPossess();
			}
			break;
		}
			
	}
}

void Aura::SpellAuraReduceEffectDuration(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	int32 val;
	if(apply)
	{
		SetPositive();
		val = mod->m_amount; // TODO Only maximum effect should be used for Silence or Interrupt effects reduction
	}
	else
	{
		val = -mod->m_amount;
	}
	if(mod->m_miscValue > 0 && mod->m_miscValue < 28){
		static_cast< Player* >( m_target )->MechanicDurationPctMod[mod->m_miscValue] += val;
	}
}

void Aura::HandleAuraControlVehicle(bool apply)
{
}

void Aura::SpellAuraModCombatResultChance(bool apply)
{
	if( apply )
	{
		switch( mod->m_miscValue )
		{
		case 1:
			//m_target->m_CombatResult_Parry += mod->m_amount; // parry?
			break;
		case 2:
			m_target->m_CombatResult_Dodge += mod->m_amount;
			break;
		}
	}
	else
	{
		switch( mod->m_miscValue )
		{
		case 1:
			//m_target->m_CombatResult_Parry += -mod->m_amount; // parry?
			break;
		case 2:
			m_target->m_CombatResult_Dodge += -mod->m_amount;
			break;
		}
	}
}

void Aura::SpellAuraAddHealth(bool apply)
{
	if ( apply )
	{
		SetPositive();
		m_target->ModMaxHealth(mod->m_amount );
		m_target->ModHealth(mod->m_amount );
	}
	else
	{
		m_target->ModMaxHealth(-mod->m_amount );
		uint32 maxHealth = m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH );
		if(m_target->GetUInt32Value( UNIT_FIELD_HEALTH) > maxHealth )
			m_target->SetUInt32Value( UNIT_FIELD_MAXHEALTH, maxHealth );
	}
}

void Aura::SpellAuraRemoveReagentCost(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	if ( apply )
	{
		static_cast<Player*>(m_target)->removeReagentCost = true;
	}
	else
	{
		static_cast<Player*>(m_target)->removeReagentCost = false;
	}
}
void Aura::SpellAuraBlockMultipleDamage(bool apply)
{
	if( !m_target->IsPlayer() )
		return;

	if ( apply )
	{
		static_cast<Player*>(m_target)->m_BlockModPct += mod->m_amount;
	}
	else
	{
		static_cast<Player*>(m_target)->m_BlockModPct += -mod->m_amount;
	}
}

void Aura::SpellAuraModMechanicDmgTakenPct( bool apply )
{
	if( apply )
	{
		m_target->ModDamageTakenByMechPCT[mod->m_miscValue] += (float)mod->m_amount/100;
		
	}
	else
	{
		m_target->ModDamageTakenByMechPCT[mod->m_miscValue] -= (float)mod->m_amount/100;
	}
}

void Aura::SpellAuraIgnoreTargetAuraState( bool apply )
{
	if(!m_target->IsPlayer())
		return;

	if( apply )
	{
		static_cast<Player*>(m_target)->ignoreAuraStateCheck = true;
	}
	else
	{
		static_cast<Player*>(m_target)->ignoreAuraStateCheck = false;
	}
}

void Aura::SpellAuraAllowOnlyAbility(bool apply)
{
	// cannot perform any abilities (other than those in EffectMask), currently only works on players
	if ( !p_target ) 
		return;

	// Generic
	if( apply )
	{
		p_target->m_castFilterEnabled = true;
		for( uint32 x= 0; x<3; x++ )
			p_target->m_castFilter[x] |= m_spellProto->EffectSpellClassMask[mod->i][x];
	}
	else
	{
		p_target->m_castFilterEnabled = false;	// check if we can turn it off
		for( uint32 x= 0; x<3; x++ )
		{
			p_target->m_castFilter[x] &= ~m_spellProto->EffectSpellClassMask[mod->i][x];
			if(p_target->m_castFilter[x])
				p_target->m_castFilterEnabled = true;
		}
	}
}

void Aura::SpellAuraIncreaseAPbyStatPct( bool apply )
{
	if( apply )
	{
		if( mod->m_amount > 0 )
			SetPositive();
		else
			SetNegative();

		mod->fixed_amount[mod->i] = m_target->GetStat(mod->m_miscValue ) * mod->m_amount / 100;
		m_target->ModAttackPowerMods(mod->fixed_amount[mod->i] );
	}
	else
		m_target->ModAttackPowerMods(-mod->fixed_amount[mod->i] );

	m_target->CalcDamage();
}

void Aura::SpellAuraModSpellDamageDOTPct(bool apply)
{
	int32 val = (apply) ? mod->m_amount : -mod->m_amount;

	if( m_spellProto->NameHash == SPELL_HASH_HAUNT )
		m_target->DoTPctIncrease[m_spellProto->School] += val;
	else
		for(uint32 x= 0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->DoTPctIncrease[x] += val;
			}
		}
}

void Aura::SpellAuraConsumeNoAmmo( bool apply ){
	if( p_target == NULL )
		return;

	if( apply ){
		p_target->m_requiresNoAmmo = true;
	}else{
		bool other = false;

		// we have Thori'dal too
		if( m_spellProto->NameHash != SPELL_HASH_REQUIRES_NO_AMMO && p_target->HasAurasWithNameHash( SPELL_HASH_REQUIRES_NO_AMMO ) )
			other = true;

		// We are unequipping Thori'dal but have an aura with no ammo consumption effect
		if( m_spellProto->NameHash == SPELL_HASH_REQUIRES_NO_AMMO && p_target->HasAuraWithName( SPELL_AURA_CONSUMES_NO_AMMO ) )
			other = true;

		// We have more than 1 aura with no ammo consumption effect
		if( p_target->GetAuraCountWithName( SPELL_AURA_CONSUMES_NO_AMMO ) >= 2 )
			other = true;

		p_target->m_requiresNoAmmo = other;
	}
}

void Aura::SpellAuraIgnoreShapeshift( bool apply )
{
	if(!m_target->IsPlayer())
		return;

	if( apply )
	{
		static_cast<Player*>(m_target)->ignoreShapeShiftChecks = true;
	}
	else
	{
		static_cast<Player*>(m_target)->ignoreShapeShiftChecks = false;
	}
}

void Aura::SpellAuraModIgnoreArmorPct(bool apply)
{
	if(apply)
	{
		if( GetSpellProto()->NameHash == SPELL_HASH_MACE_SPECIALIZATION )
		{
			m_target->m_ignoreArmorPctMaceSpec += (mod->m_amount / 100.0f);
		}
		else
			m_target->m_ignoreArmorPct += (mod->m_amount / 100.0f);
	}
	else
	{
		if( GetSpellProto()->NameHash == SPELL_HASH_MACE_SPECIALIZATION )
		{
			m_target->m_ignoreArmorPctMaceSpec -= (mod->m_amount / 100.0f);
		}
		else
			m_target->m_ignoreArmorPct -= (mod->m_amount / 100.0f);
	}
}

void Aura::SpellAuraModBaseHealth(bool apply)
{
	if( !p_target )
		return;

	if( apply )
		mod->fixed_amount[0] = p_target->GetBaseHealth();

	int32 amt = mod->fixed_amount[0] * mod->m_amount / 100;

	if( !apply )
		amt *= -1;

	p_target->SetHealthFromSpell( p_target->GetHealthFromSpell() + amt );
	p_target->UpdateStats();
}

void Aura::SpellAuraModAttackPowerOfArmor( bool apply )
{
	/* Need more info about mods, currently it's only for armor
	uint32 modifier;
	switch( mod->m_miscValue ):
	{
	case 1: //Armor
		modifier = UNIT_FIELD_RESISTANCES;
		break;
	}
	*/
	
	if( apply )
	{
		if( mod->m_amount > 0 )
			SetPositive();
		else
			SetNegative();
		
		mod->fixed_amount[mod->i] = m_target->GetResistance(SCHOOL_NORMAL) / mod->m_amount;
		m_target->ModAttackPowerMods(mod->fixed_amount[mod->i] );
	}
	else
		m_target->ModAttackPowerMods(-mod->fixed_amount[mod->i] );

	m_target->CalcDamage();
}

void Aura::SpellAuraReflectSpellsInfront(bool apply)
{
	m_target->RemoveReflect( GetSpellId(), apply );

	if( apply )
	{
		SpellEntry *sp = dbcSpell.LookupEntry(GetSpellId());
		if (sp == NULL)
			return;

		ReflectSpellSchool *rss = new ReflectSpellSchool;
		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->school = -1;
		rss->require_aura_hash = 0;
		rss->charges = 0;
		rss->infront = true;

		m_target->m_reflectSpellSchool.push_back(rss);
	}
}

void Aura::SpellAuraPhase(bool apply)
{
	if ( m_target->GetAuraStackCount(SPELL_AURA_PHASE) > 1 )
	{
		if ( m_target->IsPlayer() )
			static_cast<Player*>(m_target)->GetSession()->SystemMessage("You can have only one phase aura!");
		Remove();
		return;
	}

	if (apply)
		m_target->Phase( PHASE_SET, m_spellProto->EffectMiscValue[mod->i] );
	else
		m_target->Phase( PHASE_RESET );

	if ( m_target->IsPlayer() )
	{
		WorldPacket data(SMSG_SET_PHASE_SHIFT, 4);
		data << uint32(m_target->m_phase);
		static_cast<Player*>(m_target)->GetSession()->SendPacket(&data);
	}
}

void Aura::SpellAuraCallStabledPet(bool apply)
{
	if(apply)
	{
		Player* pcaster = GetPlayerCaster();
		if( pcaster != NULL && pcaster->getClass() == HUNTER && pcaster->GetSession() != NULL )
			pcaster->GetSession()->SendStabledPetList(0);
	}
}

void Aura::ResetDuration()
{
	timeleft = static_cast<uint32>(UNIXTIME);
	sEventMgr.ModifyEventTimeLeft(this, EVENT_AURA_REMOVE, GetDuration());
}

bool Aura::DotCanCrit()
{
	Unit *caster = this->GetUnitCaster();
	if( caster == NULL )
		return false;

	SpellEntry *sp = this->GetSpellProto();
	uint32 index = MAX_TOTAL_AURAS_START;
	Aura *aura;
	bool found = false;
	
	for (;;)
	{
		aura = caster->FindAuraWithAuraEffect(SPELL_AURA_ALLOW_DOT_TO_CRIT, &index);

		if( aura == NULL )
			break;

		SpellEntry *aura_sp = aura->GetSpellProto();

		uint32 i = 0;
		if( aura_sp->EffectApplyAuraName[1] == SPELL_AURA_ALLOW_DOT_TO_CRIT)
			i = 1;
		else if( aura_sp->EffectApplyAuraName[2] == SPELL_AURA_ALLOW_DOT_TO_CRIT)
			i = 2;

		if( aura_sp->SpellFamilyName == sp->SpellFamilyName && 
			(aura_sp->EffectSpellClassMask[i][0] & sp->SpellGroupType[0] || 
			 aura_sp->EffectSpellClassMask[i][1] & sp->SpellGroupType[1] || 
			 aura_sp->EffectSpellClassMask[i][2] & sp->SpellGroupType[2]) )
		{
			found = true;
			break;
		}

		index++;
	}

	if( found )
		return true;

	if( caster->IsPlayer() )
	{
		switch( caster->getClass() )
		{
			case ROGUE:

				// Rupture can be critical in patch 3.3.3
				if( sp->NameHash == SPELL_HASH_RUPTURE )
					return true;

				break;
		}
	}

	return false;
}


bool Aura::IsCombatStateAffecting(){
	SpellEntry *sp = m_spellProto;

	if( sp->AppliesAura( SPELL_AURA_PERIODIC_DAMAGE ) ||
		sp->AppliesAura( SPELL_AURA_PERIODIC_DAMAGE_PERCENT ) ||
		sp->AppliesAura( SPELL_AURA_PERIODIC_TRIGGER_SPELL ) ||
		sp->AppliesAura( SPELL_AURA_PERIODIC_LEECH ) ||
		sp->AppliesAura( SPELL_AURA_PERIODIC_MANA_LEECH ) )
		return true;
	
	return false;
}

bool Aura::IsAreaAura(){
	SpellEntry *sp = m_spellProto;

	if( sp->HasEffect( SPELL_EFFECT_APPLY_GROUP_AREA_AURA ) ||
		sp->HasEffect( SPELL_EFFECT_APPLY_RAID_AREA_AURA ) ||
		sp->HasEffect( SPELL_EFFECT_APPLY_PET_AREA_AURA ) ||
		sp->HasEffect( SPELL_EFFECT_APPLY_FRIEND_AREA_AURA ) ||
		sp->HasEffect( SPELL_EFFECT_APPLY_ENEMY_AREA_AURA ) ||
		sp->HasEffect( SPELL_EFFECT_APPLY_OWNER_AREA_AURA ) )
		return true;

	return false;
}
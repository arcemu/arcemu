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
		&Aura::SpellAuraNULL,//SPELL_AURA_PERIODIC_MANA_FUNNEL = 63,//obselete?
		&Aura::SpellAuraPeriodicManaLeech,//SPELL_AURA_PERIODIC_MANA_LEECH = 64,
		&Aura::SpellAuraModCastingSpeed,//SPELL_AURA_MOD_CASTING_SPEED = 65,
		&Aura::SpellAuraFeignDeath,//SPELL_AURA_FEIGN_DEATH = 66,
		&Aura::SpellAuraModDisarm,//SPELL_AURA_MOD_DISARM = 67,
		&Aura::SpellAuraModStalked,//SPELL_AURA_MOD_STALKED = 68,
		&Aura::SpellAuraSchoolAbsorb,//SPELL_AURA_SCHOOL_ABSORB = 69,
		&Aura::SpellAuraNULL,//SPELL_AURA_EXTRA_ATTACKS = 70,//obselete?
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
		&Aura::SpellAuraAddTargetTrigger,//SPELL_AURA_ADD_TARGET_TRIGGER = 109,
		&Aura::SpellAuraModPowerRegPerc,//SPELL_AURA_MOD_POWER_REGEN_PERCENT = 110,
		&Aura::SpellAuraNULL,//SPELL_AURA_ADD_CASTER_HIT_TRIGGER = 111,
		&Aura::SpellAuraOverrideClassScripts,//SPELL_AURA_OVERRIDE_CLASS_SCRIPTS = 112,
		&Aura::SpellAuraModRangedDamageTaken,//SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN = 113,
		&Aura::SpellAuraNULL,//SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT = 114,
		&Aura::SpellAuraModHealing,//SPELL_AURA_MOD_HEALING = 115,
		&Aura::SpellAuraIgnoreRegenInterrupt,//SPELL_AURA_IGNORE_REGEN_INTERRUPT = 116,
		&Aura::SpellAuraModMechanicResistance,//SPELL_AURA_MOD_MECHANIC_RESISTANCE = 117,
		&Aura::SpellAuraModHealingPCT,//SPELL_AURA_MOD_HEALING_PCT = 118,
		&Aura::SpellAuraNULL,//SPELL_AURA_SHARE_PET_TRACKING = 119,//obselete
		&Aura::SpellAuraNULL,//SPELL_AURA_UNTRACKABLE = 120,
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
		&Aura::SpellAuraNULL,//SPELL_AURA_CHARISMA = 145,//obselete?
		&Aura::SpellAuraNULL,//SPELL_AURA_PERSUADED = 146,//obselete
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
		&Aura::SpellAuraNoPVPCredit,//missing = 159 //used Apply Aura: No PVP Credit http://www.thottbot.com/?sp=2479
		&Aura::SpellAuraNULL,//missing = 160 //Apply Aura: Mod Side/Rear PBAE Damage Taken %//used http://www.thottbot.com/?sp=23198
		&Aura::SpellAuraModHealthRegInCombat,//SPELL_AURA_MOD_HEALTH_REGEN_IN_COMBAT = 161,
		&Aura::SpellAuraPowerBurn,//missing = 162 //used //Apply Aura: Power Burn (Mana) //http://www.thottbot.com/?sp=19659
		&Aura::SpellAuraModCritDmgPhysical,//missing = 163 //Apply Aura: Mod Crit Damage Bonus (Physical)
		&Aura::SpellAuraNULL,//missing = 164 //used //test spell
		&Aura::SpellAuraAPAttackerBonus,//SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS = 165,	// Melee AP Attacker Bonus
		&Aura::SpellAuraModPAttackPower,//missing = 166 //used //Apply Aura: Mod Attack Power % // http://www.thottbot.com/?sp=30803
		&Aura::SpellAuraModRangedAttackPowerPct,//missing = 167 //http://www.thottbot.com/s34485
		&Aura::SpellAuraIncreaseDamageTypePCT,//missing = 168 //used //Apply Aura: Increase Damage % *type* //http://www.thottbot.com/?sp=24991
		&Aura::SpellAuraIncreaseCricticalTypePCT,//missing = 169 //used //Apply Aura: Increase Critical % *type* //http://www.thottbot.com/?sp=24293
		&Aura::SpellAuraNULL,//missing = 170 //used //Apply Aura: Detect Amore //http://www.thottbot.com/?sp=26802
		&Aura::SpellAuraIncreasePartySpeed,//missing = 171
		&Aura::SpellAuraIncreaseMovementAndMountedSpeed,//missing = 172 //used //Apply Aura: Increase Movement and Mounted Speed (Non-Stacking) //http://www.thottbot.com/?sp=26022 2e effect
		&Aura::SpellAuraNULL,//missing = 173 // Apply Aura: Allow Champion Spells
		&Aura::SpellAuraIncreaseSpellDamageByAttribute,//missing = 174 //used //Apply Aura: Increase Spell Damage by % Spirit (Spells) //http://www.thottbot.com/?sp=15031
		&Aura::SpellAuraIncreaseHealingByAttribute,//missing = 175 //used //Apply Aura: Increase Spell Healing by % Spirit //http://www.thottbot.com/?sp=15031
		&Aura::SpellAuraSpiritOfRedemption,//missing = 176 //used // Apply Aura: Spirit of Redemption
		&Aura::SpellAuraNULL,//missing = 177 //used //Apply Aura: Area Charm // http://www.thottbot.com/?sp=26740
		&Aura::SpellAuraNULL,//missing = 178 //Apply Aura: Increase Debuff Resistance
		&Aura::SpellAuraIncreaseAttackerSpellCrit,//SPELL_AURA_INCREASE_ATTACKER_SPELL_CRIT//Apply Aura: Increase Attacker Spell Crit % *type* //http://www.thottbot.com/?sp=12579
		&Aura::SpellAuraNULL,//missing = 180 //used //Apply Aura: Increase Spell Damage *type* //http://www.thottbot.com/?sp=29113
		&Aura::SpellAuraNULL,//missing = 181
		&Aura::SpellAuraIncreaseArmorByPctInt,//missing = 182 //used //Apply Aura: Increase Armor by % of Intellect //http://www.thottbot.com/?sp=28574  SPELL_AURA_INC_ARMOR_BY_PCT_INT
		&Aura::SpellAuraNULL,//missing = 183 //used //Apply Aura: Decrease Critical Threat by % (Spells) //http://www.thottbot.com/?sp=28746
		&Aura::SpellAuraReduceAttackerMHitChance,//184//Apply Aura: Reduces Attacker Chance to Hit with Melee //http://www.thottbot.com/s31678
		&Aura::SpellAuraReduceAttackerRHitChance,//185//Apply Aura: Reduces Attacker Chance to Hit with Ranged //http://www.thottbot.com/?sp=30895
		&Aura::SpellAuraReduceAttackerSHitChance,//186//Apply Aura: Reduces Attacker Chance to Hit with Spells (Spells) //http://www.thottbot.com/?sp=30895
		&Aura::SpellAuraReduceEnemyMCritChance,//missing = 187 //used //Apply Aura: Reduces Attacker Chance to Crit with Melee (Ranged?) //http://www.thottbot.com/?sp=30893
		&Aura::SpellAuraReduceEnemyRCritChance,//missing = 188 //used //Apply Aura: Reduces Attacker Chance to Crit with Ranged (Melee?) //http://www.thottbot.com/?sp=30893
		&Aura::SpellAuraIncreaseRating,//missing = 189 //Apply Aura: Increases Rating
		&Aura::SpellAuraIncreaseRepGainPct,//SPELL_AURA_MOD_FACTION_REPUTATION_GAIN //used // Apply Aura: Increases Reputation Gained by % //http://www.thottbot.com/?sp=30754
		&Aura::SpellAuraLimitSpeed,//missing = 191 //used // noname //http://www.thottbot.com/?sp=29894
		&Aura::SpellAuraNULL,//192 Apply Aura: Melee Slow %
		&Aura::SpellAuraIncreaseTimeBetweenAttacksPCT,//193 Apply Aura: Increase Time Between Attacks (Melee, Ranged and Spell) by %
		&Aura::SpellAuraNULL,//194 //&Aura::SpellAuraIncreaseSpellDamageByInt,//194 Apply Aura: Increase Spell Damage by % of Intellect (All)
		&Aura::SpellAuraNULL,//195 //&Aura::SpellAuraIncreaseHealingByInt,//195 Apply Aura: Increase Healing by % of Intellect
		&Aura::SpellAuraNULL,//196 Apply Aura: Mod All Weapon Skills (6)
		&Aura::SpellAuraModAttackerCritChance,//197 Apply Aura: Reduce Attacker Critical Hit Chance by %
		&Aura::SpellAuraIncreaseAllWeaponSkill,//198
		&Aura::SpellAuraIncreaseHitRate,//199 Apply Aura: Increases Spell % To Hit (Fire, Nature, Frost)
		&Aura::SpellAuraNULL,//200 // Increases experience earned by $s1%.  Lasts $d.
		&Aura::SpellAuraNULL,//201 Apply Aura: Cannot be Dodged
		&Aura::SpellAuraFinishingMovesCannotBeDodged,//202 // Finishing moves cannot be dodged - 32601, 44452
		&Aura::SpellAuraReduceCritMeleeAttackDmg,//203 Apply Aura: Reduces Attacker Critical Hit Damage with Melee by %
		&Aura::SpellAuraReduceCritRangedAttackDmg,//204 Apply Aura: Reduces Attacker Critical Hit Damage with Ranged by %
		&Aura::SpellAuraNULL,//205 // "School" Vulnerability
		&Aura::SpellAuraEnableFlight,//206 // Take flight on a worn old carpet. - Spell 43343
		&Aura::SpellAuraEnableFlight,//207 set fly
		&Aura::SpellAuraEnableFlightWithUnmountedSpeed,//208
		&Aura::SpellAuraNULL,//209  // mod flight speed?
		&Aura::SpellAuraNULL,//210	// commentator's command - spell 42009
		&Aura::SpellAuraIncreaseFlightSpeed,//211
		&Aura::SpellAuraIncreaseRangedAPStatPCT,//SPELL_AURA_MOD_RANGED_ATTACK_POWER_OF_INTELLECT //212 Apply Aura: Increase Ranged Atk Power by % of Intellect
		&Aura::SpellAuraIncreaseRageFromDamageDealtPCT, //213 Apply Aura: Increase Rage from Damage Dealt by %
		&Aura::SpellAuraNULL,//214 // Tamed Pet Passive (DND)
		&Aura::SpellAuraNULL,//215 // arena preparation buff - cancel soul shard requirement?
		&Aura::SpellAuraModCastingSpeed,//216 Increases casting time %, reuse existing handler...
		&Aura::SpellAuraNULL,//217 // not used
		&Aura::SpellAuraNULL,//218 // increases time between ranged attacks
		&Aura::SpellAuraRegenManaStatPCT,//219 Regenerate mana equal to $s1% of your Intellect every 5 sec, even while casting
		&Aura::SpellAuraSpellHealingStatPCT,//220 Increases your healing spells  by up to $s1% of your Strength
		&Aura::SpellAuraNULL,//221 Detaunt "Ignores an enemy, forcing the caster to not attack it unless there is no other target nearby. When the effect wears off, the creature will attack the most threatening target."
		&Aura::SpellAuraNULL,//222 // not used
		&Aura::SpellAuraNULL,//223 // used in one spell, cold stare 43593
		&Aura::SpellAuraNULL,//224 // not used
		&Aura::SpellAuraNULL,//225 // Prayer of Mending "Places a spell on the target that heals them for $s1 the next time they take damage.  When the heal occurs, Prayer of Mending jumps to a raid member within $a1 yards.  Jumps up to $n times and lasts $d after each jump.  This spell can only be placed on one target at a time."
		&Aura::SpellAuraDrinkNew,//226 // used in brewfest spells, headless hoerseman
		&Aura::SpellAuraNULL,//227 Inflicts [SPELL DAMAGE] damage to enemies in a cone in front of the caster. (based on combat range) http://www.thottbot.com/s40938
		&Aura::SpellAuraNULL,//228 Stealth Detection. http://www.thottbot.com/s34709
		&Aura::SpellAuraNULL,//229 Apply Aura:Reduces the damage your pet takes from area of effect attacks http://www.thottbot.com/s35694
		&Aura::SpellAuraIncreaseMaxHealth,//230 Increase Max Health (commanding shout);
        &Aura::SpellAuraNULL,//231 curse a target http://www.thottbot.com/s40303
        &Aura::SpellAuraNULL,//232 // Reduces duration of Magic effects by $s2%.
        &Aura::SpellAuraNULL,//233 // Beer Goggles
        &Aura::SpellAuraNULL,//234 Apply Aura: Reduces Silence or Interrupt effects, Item spell magic http://www.thottbot.com/s42184
		&Aura::SpellAuraNULL,//235 33206 Instantly reduces a friendly target's threat by $44416s1%, reduces all damage taken by $s1% and increases resistance to Dispel mechanics by $s2% for $d.
		&Aura::SpellAuraNULL,//236
		&Aura::SpellAuraNULL,//237
		&Aura::SpellAuraNULL,//238
		&Aura::SpellAuraNULL,//239
		&Aura::SpellAuraAxeSkillModifier,//240 Increase Axe Skill http://www.wowhead.com/?spell=20574
		&Aura::SpellAuraNULL,//241
		&Aura::SpellAuraNULL,//242
		&Aura::SpellAuraNULL,//243
		&Aura::SpellAuraNULL,//244
		&Aura::SpellAuraNULL,//245
		&Aura::SpellAuraNULL,//246
		&Aura::SpellAuraNULL,//247
		&Aura::SpellAuraNULL,//248
		&Aura::SpellAuraNULL,//249
		&Aura::SpellAuraNULL,//250
		&Aura::SpellAuraNULL,//251
};
/*
ASCENT_INLINE void ApplyFloatSM(float ** m,float v,uint32 mask, float def)
{
	if(*m == 0)
	{
		*m = new float[SPELL_GROUPS];

		for(uint32 x=0;x<SPELL_GROUPS;x++)
		{
			(*m)[x] = def;
			if((1<<x) & mask)
				(*m)[x]+=v;
		}
	}
	else
	{
		for(uint32 x=0;x<SPELL_GROUPS;x++)
		{
			if((1<<x) & mask)
				(*m)[x]+=v;
		}
	}
}*/
/*
ASCENT_INLINE void ApplyFloatPSM(float ** m,int32 v,uint32 mask, float def)
{
	if(*m == 0)
	{
		*m = new float[SPELL_GROUPS];

		for(uint32 x=0;x<SPELL_GROUPS;x++)
		{
			(*m)[x] = def;
			if((1<<x) & mask)
				(*m)[x]+=((float)v)/100.0f;
		}
	}
	else
	{
		for(uint32 x=0;x<SPELL_GROUPS;x++)
		{
			if((1<<x) & mask)
				(*m)[x]+=((float)v)/100.0f;
		}
	}
}*/

Unit* Aura::GetUnitCaster()
{
	if( !m_target )
		return NULL;

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

Aura::Aura( SpellEntry* proto, int32 duration, Object* caster, Unit* target )
{
	m_castInDuel = false;
	m_spellProto = proto;
	m_duration = duration;
	m_positive = 0; //we suppose spell will have positive impact on target
	m_deleted = false;
	m_ignoreunapply = false;
	m_casterGuid = caster->GetGUID();
	m_target = target;

	if( m_target->GetTypeId() == TYPEID_PLAYER )
		p_target = static_cast< Player* >( m_target );
	else
		p_target = NULL;

	/*if( caster->GetTypeId() == TYPEID_PLAYER && target->GetTypeId() == TYPEID_PLAYER )
	{
		if( ( ( Player* )caster )->DuelingWith == ( ( Player* )target ) )
		{
			m_castInDuel = true;
		}
	}*/

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
		if( m_spellProto->buffIndexType > 0 && caster->IsPlayer() )
		{
			( ( Player* )caster )->RemoveSpellTargets( m_spellProto->buffIndexType);
			( ( Player* )caster )->SetSpellTargetType( m_spellProto->buffIndexType, target);
		}

		if( isAttackable( ( Unit* )caster, target ) )
		{
			SetNegative();
			/*if(caster->IsPlayer())
			{
				static_cast< Player* >( caster )->CombatModeDelay = COMBAT_DECAY_TIME;
			}
			else if(caster->IsPet())
			{
				Player* p = static_cast< Pet* >( caster )->GetPetOwner();
				if(p)
					p->CombatModeDelay = COMBAT_DECAY_TIME;
			}*/
		}
		else
			SetPositive();

		if( p_target && caster->IsPlayer() )
		{
			if( p_target->DuelingWith == ((Player*)caster) )
			{
				m_castInDuel = true;
			}
		}
	}

	if( !IsPassive() )
	{
		timeleft = ( uint32 )UNIXTIME;
	}

	m_castedItemId = 0;
	m_visualSlot = 0xFF;
	pSpellId = 0;
	periodic_target = 0;
	//sLog.outDetail("Aura::Constructor %u (%s) from %u.", m_spellProto->Id, m_spellProto->Name, m_target->GetLowGUID());
	m_auraSlot = 0xffffffff;
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
	if( m_deleted )
		return;

	m_deleted = true;
	sEventMgr.RemoveEvents( this );

	if( !IsPassive() || IsPassive() && m_spellProto->AttributesEx & 1024 )
		RemoveAuraVisual();

	ApplyModifiers( false );

	// reset diminishing return timer if needed
	::UnapplyDiminishingReturnTimer( m_target, m_spellProto );

	for( uint32 x = 0; x < 3; x++ )
	{
		if( !m_spellProto->Effect[x] )
			break;

		if( m_spellProto->Effect[x] == SPELL_EFFECT_TRIGGER_SPELL && !m_spellProto->always_apply )
		{
			// Im not sure about this! FIXME!!
			if ( dbcSpell.LookupEntryForced( GetSpellProto()->EffectTriggerSpell[x] )->DurationIndex < m_spellProto->DurationIndex )
			m_target->RemoveAura(GetSpellProto()->EffectTriggerSpell[x]);
		}
		else if( m_spellProto->Effect[x] == SPELL_EFFECT_APPLY_AREA_AURA && m_casterGuid == m_target->GetGUID())
		{
			RemoveAA();
		}
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

	m_target->m_auras[m_auraSlot] = NULL;

	if( GetSpellProto()->SpellGroupType && m_target->GetTypeId() == TYPEID_PLAYER )
	{
		int32 speedmod = 0;
		SM_FIValue( m_target->SM_FSpeedMod, &speedmod, m_spellProto->SpellGroupType );
		if( speedmod )
		{
			m_target->m_speedModifier -= speedmod;
			m_target->UpdateSpeed();
		}
	}

	// remove attacker
	Unit* caster = GetUnitCaster();
	if( caster != NULL)
	{
		if( caster != m_target )
		{
			caster->CombatStatus.RemoveAttackTarget( m_target );
			m_target->CombatStatus.RemoveAttacker( caster, caster->GetGUID() );
		}

		if( m_spellProto->buffIndexType != 0 && m_target->IsPlayer() )
			( ( Player* )m_target )->RemoveSpellIndexReferences( m_spellProto->buffIndexType );
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
		/*WorldPacket data( 12 );
		data.SetOpcode( SMSG_COOLDOWN_EVENT );
		data << m_spellProto->Id << caster->GetGUID();
		caster->SendMessageToSet( &data, true );*/

		packetSMSG_COOLDOWN_EVENT data;
		data.spellid = m_spellProto->Id;
		data.guid = caster->GetGUID();
		caster->OutPacketToSet( SMSG_COOLDOWN_EVENT, sizeof( packetSMSG_COOLDOWN_EVENT ), &data, true );
	}

	if( caster != NULL && caster->IsPlayer() && caster->IsInWorld() )
	{
		int j;
		for( j = 0; j < 3; ++j )
			if( m_spellProto->Effect[j] == SPELL_EFFECT_ADD_FARSIGHT )
				break;

		if(j != 3)
		{
			caster->SetUInt64Value(PLAYER_FARSIGHT, 0);
		}
	}

	delete this; // suicide xD	leaking this shit out
}

void Aura::AddMod( uint32 t, int32 a, uint32 miscValue, uint32 i )
{
	//this is fix, when u have the same unit in target list several times
	//for(uint32 x=0;x<m_modcount;x++)
	//	if(m_modList[x].i==i)return;

	/*if(m_modList[0].m_type == t ||
		m_modList[1].m_type == t ||
		m_modList[2].m_type == t)
		return; // dont duplicate mods // some spells apply duplicate mods, like some seals*/

	if( m_modcount >= 3 )
	{
		sLog.outString("Tried to add >3 (%u) mods to spellid %u [%u:%u, %u:%u, %u:%u]", m_modcount+1, this->m_spellProto->Id, m_modList[0].m_type, m_modList[0].m_amount, m_modList[1].m_type, m_modList[1].m_amount, m_modList[2].m_type, m_modList[2].m_amount);
		return;
	}
	m_modList[m_modcount].m_type = t;
	m_modList[m_modcount].m_amount = a;
	m_modList[m_modcount].m_miscValue = miscValue;
	m_modList[m_modcount].i = i;
	m_modcount++;
	//ASSERT(m_modcount<=3);
}

void Aura::ApplyModifiers( bool apply )
{

	for( uint32 x = 0; x < m_modcount; x++ )
	{
		mod = &m_modList[x];
		sLog.outDebug( "WORLD: target = %u , Spell Aura id = %u, SpellId  = %u, i = %u, apply = %s, duration = %u, damage = %d",
			m_target->GetLowGUID(),mod->m_type, m_spellProto->Id, mod->i, apply ? "true" : "false",GetDuration(),mod->m_amount);

		if(mod->m_type<TOTAL_SPELL_AURAS)
		{
			sLog.outDebug("Known Aura id %d, value %d", (uint32)mod->m_type, (uint32)mod->m_amount );
			(*this.*SpellAuraHandler[mod->m_type])(apply);
		}
		else
			sLog.outError("Unknown Aura id %d", (uint32)mod->m_type);
	}
}

void Aura::AddAuraVisual()
{
	uint8 slot, i;
	slot = 0xFF;

	if (IsPositive())
	{
		for (i = 0; i < MAX_POSITIVE_AURAS; i++)
		{
			//if (m_target->GetUInt32Value((uint16)(UNIT_FIELD_AURA + i)) == 0)
			if(m_target->m_auras[i] == 0)
			{
				slot = i;
				break;
			}
		}
	}
	else
	{
		for (i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
		{
			//if (m_target->GetUInt32Value((uint16)(UNIT_FIELD_AURA + i)) == 0)
			if(m_target->m_auras[i] == 0)
			{
				slot = i;
				break;
			}
		}
	}

	if (slot == 0xFF)
	{
		return;
	}
	m_visualSlot = m_target->AddAuraVisual(m_spellProto->Id, 1, IsPositive());

	/*m_target->SetUInt32Value(UNIT_FIELD_AURA + slot, m_spellProto->Id);

	uint8 flagslot = slot >> 3;

	uint32 value = m_target->GetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot));

	uint8 aurapos = (slot & 7) << 2;
	uint32 value1 = (uint32)AFLAG_SET << aurapos;
	value |= value1;

	m_target->SetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot), value);

	uint32 index = (slot / 8);
	uint32 byte  = (slot % 8);

	uint32 x = 0, y = 0;
	uint32 val = m_target->GetUInt32Value(UNIT_FIELD_AURALEVELS + index);
	if(x != 0)
	{
		val |= (x << (byte * 8));
		m_target->SetUInt32Value(UNIT_FIELD_AURALEVELS + index, val);
	}

	val = m_target->GetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS + index);
	if(y != 0)
	{
		val |= (y << (byte * 8));
		m_target->SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS + index, val);
	}
	sLog.outDebug("Adding Aura Visual - target: %d , slot: %d , flagslot: %d , flagsvalue: 0x%.4X",m_target->GetGUID(),slot,flagslot,value);*/

	//  0000 0000 original
	//  0000 1001 AFLAG_SET
	//  1111 1111 0xFF

	//uint8 appslot = slot >> 1;

	if( m_target->IsPlayer())
	{
		/*WorldPacket data(SMSG_UPDATE_AURA_DURATION, 5);
		data << m_visualSlot << (uint32)m_duration;
		static_cast< Player* >( m_target )->GetSession()->SendPacket(&data);*/

		packetSMSG_SET_AURA_DURATION datasa;
		datasa.slot = m_visualSlot;
		datasa.duration = m_duration;
		static_cast<Player*>(m_target)->GetSession()->OutPacket(SMSG_UPDATE_AURA_DURATION, sizeof(packetSMSG_SET_AURA_DURATION), &datasa);
	}

	WorldPacket data(SMSG_SET_AURA_SINGLE, 22);
	data << m_target->GetNewGUID() << m_visualSlot << uint32(m_spellProto->Id) << uint32(m_duration) << uint32(m_duration);
	m_target->SendMessageToSet(&data,false);

	m_auraSlot = slot;
}

void Aura::RemoveAuraVisual()
{
	m_target->RemoveAuraVisual(m_spellProto->Id, 1);

	//UNIT_FIELD_AURAFLAGS 0-7;UNIT_FIELD_AURAFLAGS+1 8-15;UNIT_FIELD_AURAFLAGS+2 16-23 ... For each Aura 1 Byte

	/*if(m_auraSlot>=MAX_AURAS)
		return;
	uint8 slot = m_auraSlot;

	m_target->SetUInt32Value((uint16)(UNIT_FIELD_AURA + slot), 0);

	uint8 flagslot = slot >> 3;

	uint32 value = m_target->GetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot));

	uint8 aurapos = (slot & 7) << 2;

	uint32 value1 = ~( (uint32)AFLAG_SET << aurapos );

	value &= value1;

	m_target->SetUInt32Value(UNIT_FIELD_AURAFLAGS + flagslot,value);
	sLog.outDebug("Removing Aura Visual - target: %d , slot: %d , flagslot: %d , flagsvalue: 0x%.4X",m_target->GetGUID(),slot,flagslot,value); */
}

void Aura::EventUpdateAA(float r)
{
	uint32 i;
	/* burlex: cheap hack to get this to execute in the correct context always */
	if(event_GetCurrentInstanceId() == -1)
	{
		event_Relocate();
		return;
	}

	Unit * u_caster = GetUnitCaster();

	// if the caster is no longer valid->remove the aura
	if(u_caster == 0)
	{
		Remove();
		//since we lost the caster we cannot do anything more
		return;
	}

	Player * plr = 0;

	if(u_caster->GetTypeId() == TYPEID_PLAYER)
		plr = static_cast< Player* >(u_caster);
	else if(u_caster->GetTypeId() == TYPEID_UNIT)
	{
		if(u_caster->IsPet())
			plr = static_cast<Pet*>(u_caster)->GetPetOwner();
		else
			plr = static_cast< Player* >(static_cast<Creature*>(u_caster)->GetTotemOwner());
	}


	if(plr == 0 || plr->GetTypeId() != TYPEID_PLAYER)	// No player involved...
		return;

	vector<uint32> NewTargets;

	// Add the aura to the caster, if he's in range of course.
	if(plr->GetDistanceSq(u_caster) < r)
	{
		if(!plr->HasActiveAura(m_spellProto->Id))
		{
			Aura * aura = new Aura(m_spellProto, -1, u_caster, plr);
			aura->m_areaAura = true;
			aura->AddMod(mod->m_type, mod->m_amount, mod->m_miscValue, mod->i);
			plr->AddAura(aura);
			NewTargets.push_back(plr->GetLowGUID());
		}
	}

	//report say that aura should also affect pet
	if( plr && plr->GetSummon() &&
		(
			GetSpellProto()->NameHash == SPELL_HASH_TRUESHOT_AURA ||
			GetSpellProto()->NameHash == SPELL_HASH_ASPECT_OF_THE_PACK ||
			GetSpellProto()->NameHash == SPELL_HASH_ASPECT_OF_THE_WILD
			)
		 )
	{
		Unit *summon = plr->GetSummon();
		if( summon && summon->isAlive() && summon->GetDistanceSq(u_caster) <= r && !summon->HasActiveAura( m_spellProto->Id ))
		{
			Aura * aura = new Aura(m_spellProto, -1, u_caster, summon );
			aura->m_areaAura = true;
			aura->AddMod( mod->m_type, mod->m_amount, mod->m_miscValue, mod->i);
			summon->AddAura( aura );
			//make sure we remove this
//			sEventMgr.AddEvent(((Unit*)summon), &Unit::EventRemoveAura, m_spellProto->Id, EVENT_DELETE_TIMER, 10, 1,0);
		}
	}

	SubGroup * group = plr->GetGroup() ?
		plr->GetGroup()->GetSubGroup(plr->GetSubGroup()) : 0;

	if(group && group->GetMemberCount() > 0 )
	{
		plr->GetGroup()->Lock();
		GroupMembersSet::iterator itr = group->GetGroupMembersBegin();
		for(; itr != group->GetGroupMembersEnd(); ++itr)
		{
			if((*itr) && (*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer != plr && (*itr)->m_loggedInPlayer->GetDistanceSq(u_caster) <= r)
			{
				// Add the aura if they don't have it.
				if(!(*itr)->m_loggedInPlayer->HasActiveAura(m_spellProto->Id) &&
					(*itr)->m_loggedInPlayer->GetInstanceID() == plr->GetInstanceID() && (*itr)->m_loggedInPlayer->isAlive())
				{
					Aura * aura = NULL;
					//aura->AddMod(mod->m_type, mod->m_amount, mod->m_miscValue, mod->i);
					for(i = 0; i < m_modcount; ++i)
					{
						/* is this an area aura modifier? */
						if(m_spellProto->Effect[m_modList[i].i] == SPELL_EFFECT_APPLY_AREA_AURA)
						{
							if(!aura)
							{
								aura = new Aura(m_spellProto, -1, u_caster, (*itr)->m_loggedInPlayer);
								aura->m_areaAura = true;
							}
							aura->AddMod(m_modList[i].m_type, m_modList[i].m_amount,
								m_modList[i].m_miscValue, m_modList[i].i);
						}
					}
					if(aura)
					{
						(*itr)->m_loggedInPlayer->AddAura(aura);
						NewTargets.push_back((*itr)->m_loggedInPlayer->GetLowGUID());
					}
				}
			}
		}
		plr->GetGroup()->Unlock();
	}

	// Update the existing players in the target set.
	AreaAuraList::iterator itr, it2;

	for(itr = targets.begin(); itr != targets.end(); )
	{
		it2 = itr;
		++itr;

		// Check if the target is 'valid'.
		Player * iplr = NULL;
		if(m_target->IsInWorld())
			iplr = m_target->GetMapMgr()->GetPlayer(*it2);

		if( iplr == NULL )
			iplr = objmgr.GetPlayer(*it2);

		if(!iplr || iplr->GetDistanceSq(u_caster) > r || iplr->GetInstanceID() != plr->GetInstanceID())
		{
			targets.erase(it2);
			if(iplr)
			{
				// execute in the correct context
                if(iplr->GetInstanceID() != plr->GetInstanceID())
					sEventMgr.AddEvent(((Unit*)iplr), &Unit::EventRemoveAura, m_spellProto->Id, EVENT_DELETE_TIMER, 10, 1,0);
				else
					iplr->RemoveAura(m_spellProto->Id);
			}

			continue;
		}

		// Is the player outside of the group now?
		// (doesn't apply to the caster)
		if(plr == iplr) continue;

		if(!group || !group->HasMember(iplr->GetLowGUID()))
		{
			// execute in the correct context
			if(iplr->GetInstanceID() != plr->GetInstanceID())
				sEventMgr.AddEvent(((Unit*)iplr), &Unit::EventRemoveAura, m_spellProto->Id, EVENT_DELETE_TIMER, 10, 1,0);
			else
				iplr->RemoveAura(m_spellProto->Id);
			targets.erase(it2);
		}
	}

	// Push new targets into the set.
	for(vector<uint32>::iterator vtr = NewTargets.begin(); vtr != NewTargets.end(); ++vtr)
		targets.insert((*vtr));

	NewTargets.clear();
}

void Aura::RemoveAA()
{
	AreaAuraList::iterator itr;
	Unit * caster = GetUnitCaster();

	//report say that aura should also affect pet
	Player *plr = NULL;
	if( GetUnitCaster() && GetUnitCaster()->IsPlayer() )
		plr = static_cast<Player*>(GetUnitCaster());
	if( plr && plr->GetSummon() &&
		(
			GetSpellProto()->NameHash == SPELL_HASH_TRUESHOT_AURA ||
			GetSpellProto()->NameHash == SPELL_HASH_ASPECT_OF_THE_PACK ||
			GetSpellProto()->NameHash == SPELL_HASH_ASPECT_OF_THE_WILD
			)
		 )
	{
		Unit *summon = plr->GetSummon();
		if( summon && summon->isAlive() )
			summon->RemoveAura( m_spellProto->Id );
	}

	for(itr = targets.begin(); itr != targets.end(); ++itr)
	{
		// Check if the target is 'valid'.
		Player * iplr;
		if(m_target->IsInWorld())
			iplr = m_target->GetMapMgr()->GetPlayer((uint32)*itr);
		else
			iplr = objmgr.GetPlayer((uint32)*itr);

		if(!iplr || iplr == caster)
			continue;

		iplr->RemoveAura(m_spellProto->Id);
	}
	targets.clear();
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

	for(uint32 x=0;x<7;x++)
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
	if(apply)
	{
		Unit *caster = GetUnitCaster();
		if(!caster || !caster->IsPlayer())
			return;
		caster->SetUInt64Value(PLAYER_FARSIGHT, m_target->GetGUID());
	}
	else
	{
		Unit *caster = GetUnitCaster();
		if(!caster || !caster->IsPlayer())
			return;
		caster->SetUInt64Value(PLAYER_FARSIGHT, 0 );
	}
}

void Aura::SpellAuraModPossess(bool apply)
{
	Unit *caster = GetUnitCaster();

	if(apply)
	{
		if( caster != NULL && caster->IsInWorld() && caster->GetTypeId() == TYPEID_PLAYER )
		{
			static_cast< Player* >(caster)->Possess( m_target );
			static_cast< Player* >(caster)->SpeedCheatDelay( GetDuration() );
		}
	}
	else
	{
		if( caster != NULL && caster->IsInWorld() && caster->GetTypeId() == TYPEID_PLAYER )
		{
			static_cast< Player* >(caster)->UnPossess();
//			static_cast< Player* >(caster)->SpeedCheatReset();
		}

		// make sure Player::UnPossess() didn't fail, if it did we will just free the target here
		if( m_target->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) != 0 )
		{
			if( m_target->GetTypeId() == TYPEID_UNIT )
			{
				m_target->setAItoUse( true );
				m_target->m_redirectSpellPackets = 0;
			}

			m_target->SetUInt64Value( UNIT_FIELD_CHARMEDBY, 0 );
			m_target->RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE );
			m_target->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, m_target->GetCharmTempVal() );
			m_target->_setFaction();
			m_target->UpdateOppFactionSet();
		}
	}
}

void Aura::SpellAuraPeriodicDamage(bool apply)
{
	if(apply)
	{
		if( m_target )
		{
			if( m_spellProto->MechanicsType == MECHANIC_BLEEDING && m_target->MechanicsDispels[MECHANIC_BLEEDING] )
			{
				m_flags |= 1 << mod->i;
				return;
			}
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
				if(c)
					c->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);  // remove stealth
				break;
			//mage talent ignite
			case 12654:
			{
				if(!pSpellId) //we need a parent spell and should always have one since it procs on it
					break;
				SpellEntry * parentsp = dbcSpell.LookupEntry(pSpellId);
				if(!parentsp)
					return;
				if (c && c->IsPlayer())
				{
					dmg = float2int32(static_cast< Player* >(c)->m_casted_amount[SCHOOL_FIRE]*parentsp->EffectBasePoints[0]/100.0f);
				}
				else
				{
					if (!dmg)
						return;
					Spell *spell = new Spell(GetUnitCaster(), parentsp ,false,NULL);
					SpellCastTargets targets(m_target->GetGUID());
					//this is so not good, maybe parent spell has more then dmg effect and we use it to calc our new dmg :(
					dmg = 0;
					for(int i=0;i<3;i++)
					{
					  //dmg +=parentsp->EffectBasePoints[i]*m_spellProto->EffectBasePoints[0];
						dmg +=spell->CalculateEffect(i,m_target->IsUnit()?(Unit*)m_target:NULL)*parentsp->EffectBasePoints[0]/100;
					}
					delete spell;
				}
			}
		};
		//this is warrior : Deep Wounds
		if(c && c->IsPlayer() && pSpellId)
		{
			uint32 multiplyer=0;
			if(pSpellId==12834)
					multiplyer = 20;//level 1 of the talent should apply 20 of avarege melee weapon dmg
			else if(pSpellId==12849)
					multiplyer = 40;
			else if (pSpellId==12867)
					multiplyer = 60;
			if(multiplyer)
			{
				Player *pr=static_cast< Player* >(c);
				if(pr->GetItemInterface())
				{
					Item *it;
					it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(it && it->GetProto())
					{
						dmg = 0;
						for(int i=0;i<5;i++)
							if(it->GetProto()->Damage[i].Type==SCHOOL_NORMAL)
								dmg += int32((it->GetProto()->Damage[i].Min + it->GetProto()->Damage[i].Max) / 2);
						dmg = multiplyer * dmg /100;
					}
				}
			}
		}
		uint64 gr = GetSpellProto()->SpellGroupType;
		if(gr)
		{
			Unit*c=GetUnitCaster();
			if(c)
			{
				SM_FIValue(c->SM_FDOT,(int32*)&dmg,gr);
				SM_PIValue(c->SM_PDOT,(int32*)&dmg,gr);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
				int spell_flat_modifers=0;
				int spell_pct_modifers=0;
				SM_FIValue(c->SM_FDOT,&spell_flat_modifers,gr);
				SM_FIValue(c->SM_PDOT,&spell_pct_modifers,gr);
				if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
					printf("!!!!!spell periodic dmg mod flat %d , spell dmg bonus pct %d , spell dmg bonus %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,dmg,gr);
#endif
			}
		}

		if(dmg<=0)
			return; //who would want a neagtive dmg here ?

		sEventMgr.AddEvent(this, &Aura::EventPeriodicDamage,(uint32)dmg,
			EVENT_AURA_PERIODIC_DAMAGE,GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		/*static_cast< Player* >( c )->GetSession()->SystemMessage("dot will do %u damage every %u seconds (total of %u)", dmg,m_spellProto->EffectAmplitude[mod->i],(GetDuration()/m_spellProto->EffectAmplitude[mod->i])*dmg);
		printf("dot will do %u damage every %u seconds (total of %u)\n", dmg,m_spellProto->EffectAmplitude[mod->i],(GetDuration()/m_spellProto->EffectAmplitude[mod->i])*dmg);*/
		SetNegative();
		if( m_spellProto->buffType & SPELL_TYPE_WARLOCK_IMMOLATE )
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_IMMOLATE);
		}
		//maybe poison aurastate should get triggered on other spells too ?
		else if( m_spellProto->c_is_flags & SPELL_FLAG_IS_POISON )//deadly poison
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_POISON);
		}
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where imunity can cancel only 1 mod and not whole spell
	{
		if( m_spellProto->buffType & SPELL_TYPE_WARLOCK_IMMOLATE )
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
	if(!m_target || !m_target->isAlive())
		return;
	if(m_target->SchoolImmunityList[GetSpellProto()->School])
		return;
	float res = float(amount);
	int bonus = 0;
	uint32 school = GetSpellProto()->School;
	Unit * c = GetUnitCaster();

	if(m_target->GetGUID()!=m_casterGuid)//don't use resist when cast on self-- this is some internal stuff
	{
		if(c)
		{
			c->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);

			float bonus_damage;

			int amp = m_spellProto->EffectAmplitude[mod->i];
			if( !amp )
				amp = static_cast< EventableObject* >( this )->event_GetEventPeriod( EVENT_AURA_PERIODIC_DAMAGE );

			if(GetDuration())
			{
				if( GetSpellProto() && GetSpellProto()->NameHash == SPELL_HASH_IGNITE )  //static damage for Ignite. Need to be reworked when "static DoTs" will be implemented
					bonus_damage=0;
				else bonus_damage = (float)c->GetSpellDmgBonus(m_target,m_spellProto,amount,true);
				float ticks= float((amp) ? GetDuration()/amp : 0);
				float fbonus = float(bonus);
				fbonus += (ticks) ? bonus_damage/ticks : 0;
				bonus = float2int32(fbonus);
			}
			else bonus = 0;

			res += bonus;

			if(res < 0)
				res = 0;
			else
			{
				float summaryPCTmod = 1.0f;
				if( m_target->IsPlayer() )//resilience
				{
					float dmg_reduction_pct = static_cast< Player* >( m_target )->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
					if( dmg_reduction_pct > 1.0f )
						dmg_reduction_pct = 1.0f;
					summaryPCTmod -= dmg_reduction_pct;
				}
				res *= summaryPCTmod;
				if( res < 0.0f )
					res = 0.0f;
			}
		}

		uint32 ress=(uint32)res;
		uint32 abs_dmg = m_target->AbsorbDamage(school, &ress);
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
			c->CalculateResistanceReduction(m_target,&dmg, m_spellProto);
			if((int32)dmg.resisted_damage > dmg.full_damage)
				res = 0;
			else
				res = float(dmg.full_damage - dmg.resisted_damage);
		}


		SendPeriodicAuraLog(m_casterGuid, m_target, GetSpellProto()->Id, school, float2int32(res), abs_dmg, dmg.resisted_damage, FLAG_PERIODIC_DAMAGE);

		if(school == SHADOW_DAMAGE)
			if( c != NULL && c->isAlive() && c->IsPlayer() && c->getClass() == PRIEST )
				((Player*)c)->VampiricSpell(float2int32(res), m_target);
	}
	// grep: this is hack.. some auras seem to delete this shit.
	SpellEntry * sp = m_spellProto;
	Unit * mtarget = m_target;
	uint64 cguid = m_casterGuid;

	if(mtarget->GetGUID()!=cguid && c)//don't use resist when cast on self-- this is some internal stuff
	{
		uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_SPELL_HIT;
		uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM | PROC_ON_SPELL_HIT_VICTIM;
		c->HandleProc(aproc, mtarget, sp, float2int32(res));
		c->m_procCounter = 0;

		mtarget->HandleProc(vproc,c,sp, float2int32(res));
		mtarget->m_procCounter = 0;
	}

	if(c)
		c->DealDamage(m_target, float2int32(res),  2, 0, GetSpellId ());
	else
		m_target->DealDamage(m_target, float2int32(res),  2, 0,  GetSpellId ());

}

void Aura::SpellAuraDummy(bool apply)
{
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummyAura(GetSpellId(), mod->i, this, apply))
		return;

	uint32 TamingSpellid = 0;

	// for seal -> set judgement crap
	if( GetSpellProto()->buffType & SPELL_TYPE_SEAL && mod->i == 2 )
	{
		Player* c = static_cast< Player* >( GetUnitCaster() );

		if( c == NULL )
			return;

		if( apply )
		{
			c->SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_JUDGEMENT );
			if( !c->judgespell )
				c->judgespell = mod->m_amount;
			if( !c->Seal )
				c->Seal = m_spellProto->Id;
		}
		else
		{
			c->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_JUDGEMENT );

			if( c->judgespell )
				c->judgespell = 0;
			if( c->Seal )
				c->Seal = 0;
		}
	}

	Player * _ptarget = static_cast< Player* >( m_target );

	switch(GetSpellId())
	{
	//Warlock - Demonic Knowledge
	case 35696:
		{
			if ( m_target->IsPet() )
			{
				Unit* PetOwner;
				if ( static_cast< Pet* >( m_target )->GetPetOwner() )
				{
					PetOwner = static_cast< Pet* >( m_target )->GetPetOwner();
					uint32 val1 = m_target->GetUInt32Value( UNIT_FIELD_STAT2 ); // stamina
					uint32 val2 = m_target->GetUInt32Value( UNIT_FIELD_STAT3 ); // intelect
					uint32 val0 = val1+val2;
					float dmginc = (float)(val0*mod->m_amount)/100;
					int32 val;

					if( apply )
						val = (int32)dmginc;
					else
						val = (int32)-dmginc;

					for (uint32 x=0;x<7;x++)
						PetOwner->ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, val);

					PetOwner->CalcDamage();
				}
			}
		}break;
	case 66:
		{
			// mage - invisibility
			if( !apply && !this->IsInterrupted() && !m_ignoreunapply )
				m_target->CastSpell( m_target, 32612, true );
		}break;
	//paladin - Blessing of Light.
/*	case 19977:
	case 19978:
	case 19979:
	case 27144:
	case 32770:
	case 27145:
	case 25890:
		{
			if( mod->i == 0 )
				SMTMod_On_target( apply, false, 0x9B56A8F5, mod->m_amount ); //holy light
			if( mod->i == 1 )
				SMTMod_On_target( apply, false, 0x333C4740, mod->m_amount ); //flash of light
		}break;*/
	//shaman - Healing Way - effect
/*	case 29203:
		{
			SMTMod_On_target( apply, true, 0x08F1A7EF, mod->m_amount ); // Healing Wave
		}break;*/
	//druid - mangle
	case 33878:
	case 33986:
	case 33987:
	case 33876:
	case 33982:
	case 33983:
		{
			int32 val = (apply) ? 30 : -30;
			m_target->ModDamageTakenByMechPCT[MECHANIC_BLEEDING] += float( val ) / 100.0f;
		}break;
	//warrior - sweeping strikes
	case 12328:
		{
	      if(apply)
			 m_target->m_extrastriketargets++;
		}break;
	//taming rod spells
	case 19548:	{                 //invoke damage to trigger attack
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19597; //uses Spelleffect3 #19614
	}break;
	case 19674:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19677;
	}break;
	case 19687:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19676;
	}break;
	case 19688:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19678;
	}break;
	case 19689:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19679;
	}break;
	case 19692:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19680;
	}break;
	case 19693:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19684;
	}break;
	case 19694:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19681;
	}break;
	case 19696:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19682;
	}break;
	case 19697:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19683;
	}break;
	case 19699:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19685;
	}break;
	case 19700:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=19686;
	}break;
	case 30099:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=30100;
	}break;
	case 30102:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=30103;
	}break;
	case 30105:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=30104;
	}break;
	case 30646:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=30647;
	}break;
	case 30653:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=30648;
	}break;
	case 30654:	{
		if (apply)
		{
			m_target->GetAIInterface()->AttackReaction( GetUnitCaster(), 10, 0);
			break;
		}
		else
			TamingSpellid=30648;
	}break;
	case 16972://Predatory Strikes
	case 16974:
	case 16975:
	{
		if(apply)
		{
			SetPositive();
			mod->realamount = (mod->m_amount * m_target->getLevel())/100;
			m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS,mod->realamount);
		}else
			m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, -mod->realamount);
		m_target->CalcDamage();
	}break;
	case 974:
	case 32593:
	case 32594:
		{
			if(apply)
			{
			ProcTriggerSpell pts;
			pts.origId = GetSpellProto()->Id;
			pts.caster = m_casterGuid;
			pts.spellId=GetSpellProto()->Id;
			if(!pts.spellId)
				return;
			pts.procChance = GetSpellProto()->procChance;
			pts.procFlags = GetSpellProto()->procFlags;
			pts.procCharges = GetSpellProto()->procCharges;
			pts.LastTrigger = 0;
			pts.deleted = false;
			m_target->m_procSpells.push_front(pts);
			}
			else
			{
			for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
			{
				if(itr->origId == GetSpellId() && itr->caster == m_casterGuid && !itr->deleted)
				{
					//m_target->m_procSpells.erase(itr);
					itr->deleted = true;
					break;
				}
			}
			}
		}break;
	case 126: //Eye of Killrog
		{
			/*if(m_target->IsInWorld() == false)
				return;

			if(!apply)
			{
				m_target->SetUInt64Value(PLAYER_FARSIGHT,0);
				Creature *summon = m_target->GetMapMgr()->GetCreature(m_target->GetUInt32Value(UNIT_FIELD_SUMMON));
				m_target->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
				m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

				if(summon)
				{
					summon->RemoveFromWorld(false,true);
					delete summon;
				}
				m_target->m_noInterrupt--;
				if(m_target->m_noInterrupt < 0)
					m_target->m_noInterrupt = 0;
			}*/
		}break;
	case 17056://Furor
	case 17058:
	case 17059:
	case 17060:
	case 17061:
		{
			if(apply)
				_ptarget->m_furorChance += mod->m_amount;
			else
				_ptarget->m_furorChance -= mod->m_amount;
		}break;
	case 12295:
	case 12676:
	case 12677:
	case 12678:
		{
			if(apply)
				_ptarget->m_retainedrage += mod->m_amount*10; //don't really know if value is all value or needs to be multiplyed with 10
			else
				_ptarget->m_retainedrage -= mod->m_amount*10;
		}break;
	case 2096://MindVision
		{
		}break;
	case 6196://FarSight
		{
			if(apply)
			{
			}
			else
			{
				// disabled this due to unstableness :S
#if 0
				Creature *summon = m_target->GetMapMgr()->GetCreature(m_target->GetUInt32Value(PLAYER_FARSIGHT));
				if(summon)
				{
					summon->RemoveFromWorld(false,true);
					delete summon;
				}
				m_target->SetUInt64Value(PLAYER_FARSIGHT,0);
#endif
			}
		}break;
	case 15286://Vampiric Embrace
		{
			if(apply)
			{
				SetNegative();
				Unit * caster =this->GetUnitCaster();
				if(caster && caster->IsPlayer())
					((Player*)caster)->m_vampiricEmbrace++;
				++m_target->m_hasVampiricEmbrace;
			}
			else
			{
				Unit * caster =this->GetUnitCaster();
				if(caster && caster->IsPlayer())
					((Player*)caster)->m_vampiricEmbrace--;
				--m_target->m_hasVampiricEmbrace;
			}
		}break;
	case 34914://Vampiric Touch
	case 34916:
	case 34917:
		{
			if(apply)
			{
				SetNegative();
				Unit * caster =this->GetUnitCaster();
				if(caster && caster->IsPlayer())
					++((Player*)caster)->m_vampiricTouch;

				++m_target->m_hasVampiricTouch;
			}
			else
			{
				Unit * caster =this->GetUnitCaster();
				if(caster && caster->IsPlayer())
					--((Player*)caster)->m_vampiricTouch;

				--m_target->m_hasVampiricTouch;
			}
		}break;
	case 18182:
	case 18183:
		{//improved life tap give amt% bonus for convers
			if(apply)
				_ptarget->m_lifetapbonus=mod->m_amount;
			else
				_ptarget->m_lifetapbonus=0;
		}break;
	case 20608://Reincarnation
		{
			if(apply)
				_ptarget->bReincarnation = true;
			else
				_ptarget->bReincarnation = false;
		}break;
	case 20707://Soulstone Resurrecton
		if(apply)
		{
            _ptarget->SetSoulStone(3026);
			_ptarget->SetSoulStoneReceiver((uint32)m_casterGuid);
		}
		else if(m_target->isAlive())
        {
            _ptarget->SetSoulStone(0);
            _ptarget->SetSoulStoneReceiver(0);
        }break;
	case 20762:
		if(apply)
		{
			_ptarget->SoulStone = 20758;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 20763:
		if(apply)
		{
			_ptarget->SoulStone = 20759;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 20764:
		if(apply)
		{
			_ptarget->SoulStone = 20760;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 20765:
		if(apply)
		{
			_ptarget->SoulStone = 20761;
			_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
		}
		else if(m_target->isAlive())
			_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		break;
	case 27239:
		{
			if(apply)
			{
				_ptarget->SoulStone = 27240;
				_ptarget->SoulStoneReceiver = (uint32)m_casterGuid;
			}
			else if(m_target->isAlive())
				_ptarget->SoulStone = _ptarget->SoulStoneReceiver = 0;
		}break;
	//case 20154://Soulstone Resurrecton
	//case 20287:
	//case 20288:
	//case 20289:
	//case 20290:
	//case 20291:
	//case 20292:
	//case 20293:
	/*case 20165:
	case 20347:
	case 20348:
	case 20349:
	case 20166:
	case 20356:
	case 20357:
	case 20164:
	case 20375:
	case 20915:
	case 20918:
	case 20919:
	case 20920:
	case 21082:
	case 20162:
	case 20305:
	case 20306:
	case 20307:
	case 20308:
		{
			if(mod->i == 2 || mod->i == 0)
			{
				Player* c = static_cast< Player* >( GetUnitCaster() );
				if(apply)
				{
					c->RemoveAura(c->Seal);
					c->Seal = GetSpellId();
					c->judgespell = mod->m_amount;
					c->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_JUDGEMENT);
				}
				else
				{
					c->Seal = 0;
					c->RemoveFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_JUDGEMENT);
				}
			}
		}break;*/
	case 570:   // far sight
	case 1345:
	case 6197:
	case 6198:  // eagle eye
	case 24125:
	case 21171:
		{
			/*if(!apply && m_target->GetTypeId() == TYPEID_PLAYER && m_target->IsInWorld())
			{
				// reset players vision
				Player * plr = static_cast< Player* >( m_target );
				plr->GetMapMgr()->ChangeFarsightLocation(plr, NULL);

				Creature * farsight = plr->GetMapMgr()->GetCreature(plr->GetUInt32Value(PLAYER_FARSIGHT));
				plr->SetUInt64Value(PLAYER_FARSIGHT, 0);
				if(farsight)
				{
					farsight->RemoveFromWorld(false,true);
					delete farsight;
				}
			}*/
		}break;

	case 23701://Darkmoon Card: Twisting Nether give 10% chance to self resurrect ->cast 23700
		{
			//if(!apply)

		}break;

	//Second Wind - triggers only on stun and Immobilize
	case 29834:
		{
			Unit *caster = GetUnitCaster();
			if(caster && caster->IsPlayer())
				static_cast< Player* >(caster)->SetTriggerStunOrImmobilize(29841,100, true);//fixed 100% chance
		}break;
	case 29838:
		{
			Unit *caster = GetUnitCaster();
			if(caster && caster->IsPlayer())
				static_cast< Player* >(caster)->SetTriggerStunOrImmobilize(29842,100, true);//fixed 100% chance
		}break;
	//mage Frostbite talent
	case 11071:
	case 12496:
	case 12497:
		{
			Unit *caster = GetUnitCaster();
			if(caster && caster->IsPlayer())
				static_cast< Player* >(caster)->SetTriggerStunOrImmobilize(12494,mod->m_amount, false);
		}break;
	//mage Magic Absorption
	case 29441:
	case 29444:
	case 29445:
	case 29446:
	case 29447:
		{
			if (m_target->IsPlayer())
			{
				static_cast< Player* >( m_target )->m_RegenManaOnSpellResist += ((apply) ? 1:-1)*(float)mod->m_amount/100;
			}
		}break;
/*	//warlock - seed of corruption
	case 27243:
	case 32863:
	case 36123:
	case 38252:
	case 39367:
		{
			//register a cast on death of the player
			if(apply)
			{
				ProcTriggerSpell pts;
				pts.origId = GetSpellProto()->Id;
				pts.caster = m_casterGuid;
//					pts.spellId=GetSpellProto()->Id;
				pts.spellId=32865;
				if(!pts.spellId)
					return;
				pts.procChance = GetSpellProto()->procChance;
//					pts.procFlags = GetSpellProto()->procFlags;
				pts.procFlags = PROC_ON_DIE;
				pts.procCharges = GetSpellProto()->procCharges;
				pts.LastTrigger = 0;
				pts.deleted = false;
				m_target->m_procSpells.push_front(pts);
			}
			else
			{
				for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
				{
					if(itr->origId == GetSpellId() && itr->caster == m_casterGuid)
					{
						//m_target->m_procSpells.erase(itr);
						itr->deleted = true;
						break;
					}
				}
			}
		}break;*/

	case 740:
	case 8918:
	case 9862:
	case 9863:
	case 21791:
	case 25817:
	case 26983:
	case 34550:		// Tranquility
		{
			//uint32 duration = GetDuration();
			//printf("moo\n");
			if(apply)
				sEventMgr.AddEvent(this, &Aura::EventPeriodicHeal1, (uint32)mod->m_amount, EVENT_AURA_PERIODIC_HEAL, 2000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			else
				sEventMgr.RemoveEvents(this, EVENT_AURA_PERIODIC_HEAL);

		}break;

	case 16914:
	case 17401:
	case 17402:
	case 27012:		// hurricane
		{
			if(apply)
				sEventMgr.AddEvent(this, &Aura::EventPeriodicDamage, (uint32)mod->m_amount, EVENT_AURA_PERIODIC_DAMAGE, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			else
				sEventMgr.RemoveEvents(this, EVENT_AURA_PERIODIC_DAMAGE);
		}break;

	case 33763:		// lifebloom
		{
			if(apply)
				return;

			// apply ONCE only.
			if( m_ignoreunapply )
				return;

			Unit * pCaster = GetUnitCaster();
			if( pCaster == NULL )
				pCaster = m_target;

			// this is an ugly hack because i don't want to copy/paste code ;P
			Spell spell(pCaster, m_spellProto, true, NULL);
			spell.SetUnitTarget( m_target );
			spell.Heal( mod->m_amount );
			// Remove other Lifeblooms - but do NOT handle unapply again
			for(uint32 x=0;x<MAX_AURAS;x++)
			{
				if(m_target->m_auras[x])
				{
					if( m_target->m_auras[x]->GetSpellId() == 33763 )
					{
						m_target->m_auras[x]->m_ignoreunapply = true;
						m_target->m_auras[x]->Remove();
					}
				}
			}
			//pCaster->RemoveAllAuras(pSpellId,0);
			//pCaster->Heal( m_target, m_spellProto->Id, mod->m_amount );
		}break;


	case 2584:			// Area spirit healer aura for BG's
		{
			if( !m_target->IsPlayer() || apply )		// already applied in opcode handler
				return;

			Player* pTarget = static_cast< Player* >( m_target );
			uint64 crtguid = pTarget->m_areaSpiritHealer_guid;
			Creature* pCreature = pTarget->IsInWorld() ? pTarget->GetMapMgr()->GetCreature(GET_LOWGUID_PART(crtguid)) : NULL;
			if(pCreature==NULL || pTarget->m_bg==NULL)
				return;

			pTarget->m_bg->RemovePlayerFromResurrect(pTarget,pCreature);
		}break;

	case 17007: //Druid:Leader of the Pack
		{
			if( !m_target->IsPlayer() )
				return;

			Player * pTarget = ((Player*)m_target);
			if( apply )
				pTarget->AddShapeShiftSpell( 24932 );
			else
				pTarget->RemoveShapeShiftSpell( 24932 );
		}break;
	case 26013: // deserter
	case 41425: // Hypothermia(iceblock:D)
		{
			SetNegative();
		}
	case 31223:
	case 31222:
	case 31221:		// Rogue : Master of Subtlety
		{
			if( !m_target->IsPlayer() )
				return;

			Player * pTarget = ((Player*)m_target);
			if( apply )
			{
				pTarget->m_outStealthDamageBonusPct += mod->m_amount;
				pTarget->m_outStealthDamageBonusPeriod = 6;			// 6 seconds
				pTarget->m_outStealthDamageBonusTimer = 0;			// reset it
			}
			else
			{
				pTarget->m_outStealthDamageBonusPct -= mod->m_amount;
				pTarget->m_outStealthDamageBonusPeriod = 6;			// 6 seconds
				pTarget->m_outStealthDamageBonusTimer = 0;			// reset it
			}
		}break;
	}
	if ( TamingSpellid && ! GetTimeLeft() )
	{
		// Creates a 15 minute pet, if player has the quest that goes with the spell and if target corresponds to quest
		Player *p_caster =(Player*)GetUnitCaster();
		SpellEntry *triggerspell = dbcSpell.LookupEntry( TamingSpellid );
		Quest* tamequest = QuestStorage.LookupEntry( triggerspell->EffectMiscValue[1] );
		if ( p_caster->GetQuestLogForEntry(tamequest->id )&& m_target->GetEntry() == tamequest->required_mob[0] )
		{
			if( Rand( 75.0f ) )// 75% chance on success
			{
				Creature *tamed = ( ( m_target->GetTypeId() == TYPEID_UNIT ) ? ( ( Creature* ) m_target ) : 0 );
				QuestLogEntry *qle = p_caster->GetQuestLogForEntry(tamequest->id );

				tamed->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, p_caster, 0 );
				Pet *pPet = objmgr.CreatePet();
				pPet->SetInstanceID( p_caster->GetInstanceID() );
				pPet->CreateAsSummon( tamed->GetEntry(), tamed->GetCreatureName(), tamed, static_cast<Unit*>(p_caster), triggerspell, 2, 900000 );
				pPet->CastSpell( tamed, triggerspell, false );
				tamed->SafeDelete();
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill( 1 );
				qle->UpdatePlayerFields();
				qle->SendQuestComplete();
			}
			else
			{
				p_caster->SendCastResult( triggerspell->Id,SPELL_FAILED_TRY_AGAIN,0,0 );
			}
		}
		else
		{
			p_caster->SendCastResult( triggerspell->Id,SPELL_FAILED_BAD_TARGETS,0,0 );
		}
		TamingSpellid = 0;
	}
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
		if( m_target )
		{
			if( m_target->MechanicsDispels[MECHANIC_DISORIENTED]
			|| ( m_spellProto->MechanicsType == MECHANIC_POLYMORPHED && m_target->MechanicsDispels[MECHANIC_POLYMORPHED] )
			)
			{
				m_flags |= 1 << mod->i;
				return;
			}
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
			data1.Initialize(SMSG_DEATH_NOTIFY_OBSOLETE);
			data1 << m_target->GetNewGUID() << uint8(0x00);
			p_target->GetSession()->SendPacket(&data1);
			p_target->SpeedCheatDelay( GetDuration() );
		}
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where imunity can cancel only 1 mod and not whole spell
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
			data1.Initialize(SMSG_DEATH_NOTIFY_OBSOLETE);
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
	Unit* ucaster = GetUnitCaster();
	Player* caster = static_cast< Player* >( ucaster );
	Creature* target = static_cast< Creature* >( m_target );

	SetPositive(3); //we ignore the other 2 effect of this spell and force it to be a positive spell

	if( m_target == NULL || m_target->GetTypeId() != TYPEID_UNIT )
		return;

	if( static_cast< Creature* >( m_target )->IsTotem() )
		return;

	if( ucaster == NULL || ucaster->GetTypeId() != TYPEID_PLAYER )
		return;

	if( apply )
	{
		if( (int32)m_target->getLevel() > mod->m_amount || m_target->IsPet() )
			return;

		// this should be done properly
		if( target->GetEnslaveCount() >= 10 )
			return;

		if( caster->GetUInt64Value( UNIT_FIELD_CHARM ) != 0 )
			return;

		m_target->m_special_state |= UNIT_STATE_CHARM;
		m_target->SetCharmTempVal( m_target->GetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE ) );
		m_target->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, caster->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		m_target->_setFaction();
		m_target->UpdateOppFactionSet();
		m_target->GetAIInterface()->Init(m_target, AITYPE_PET, MOVEMENTTYPE_NONE, caster);
		m_target->SetUInt64Value(UNIT_FIELD_CHARMEDBY, caster->GetGUID());
		caster->SetUInt64Value(UNIT_FIELD_CHARM, target->GetGUID());
		//damn it, the other effects of enslaive demon will agro him on us anyway :S
		m_target->GetAIInterface()->WipeHateList();
		m_target->GetAIInterface()->WipeTargetList();
		m_target->GetAIInterface()->SetNextTarget( (Unit*)NULL);

		target->SetEnslaveCount(target->GetEnslaveCount() + 1);

		if( caster->GetSession() ) // crashfix
		{
			WorldPacket data(SMSG_PET_SPELLS, 500);
			data << target->GetGUID();
			data << uint32(0) << uint32(0x1000);
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
		m_target->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, m_target->GetCharmTempVal() );
		m_target->_setFaction();
		m_target->GetAIInterface()->WipeHateList();
		m_target->GetAIInterface()->WipeTargetList();
		m_target->UpdateOppFactionSet();
		m_target->GetAIInterface()->Init(m_target, AITYPE_AGRO, MOVEMENTTYPE_NONE);
		m_target->SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

		if( caster != NULL && caster->GetSession() != NULL ) // crashfix
		{
			caster->SetUInt64Value(UNIT_FIELD_CHARM, 0);
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

	if( m_target->GetTypeId() == TYPEID_UNIT && static_cast<Creature*>(m_target)->IsTotem() )
		return;

	if(apply)
	{
		if( u_caster == NULL ) return;
		// Check Mechanic Immunity
		if( m_target )
		{
			if( m_target->MechanicsDispels[MECHANIC_FLEEING] )
			{
				m_flags |= 1 << mod->i;
				return;
			}
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
			data1.Initialize(SMSG_DEATH_NOTIFY_OBSOLETE);
			data1 << m_target->GetNewGUID() << uint8(0x00);
			p_target->GetSession()->SendPacket(&data1);
			p_target->SpeedCheatDelay( GetDuration() );
		}
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where imunity can cancel only 1 mod and not whole spell
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
				data1.Initialize(SMSG_DEATH_NOTIFY_OBSOLETE);
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
		sEventMgr.AddEvent( this, &Aura::EventPeriodicHeal,(uint32)mod->m_amount, EVENT_AURA_PERIODIC_HEAL, GetSpellProto()->EffectAmplitude[mod->i], 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

		if( GetSpellProto()->NameHash == SPELL_HASH_REJUVENATION || GetSpellProto()->NameHash == SPELL_HASH_REGROWTH )
		{
			m_target->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_REJUVENATE);
			if(!sEventMgr.HasEvent( m_target, EVENT_REJUVENATION_FLAG_EXPIRE ) )
			{
				sEventMgr.AddEvent( m_target, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_REJUVENATE, EVENT_REJUVENATION_FLAG_EXPIRE, GetSpellProto()->EffectAmplitude[mod->i], 1, 0 );
			}
			else
			{
				sEventMgr.ModifyEventTimeLeft( m_target,EVENT_REJUVENATION_FLAG_EXPIRE, GetSpellProto()->EffectAmplitude[mod->i], 0 );
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

	if( c != NULL && c->IsPlayer() )
	{
		for(uint32 a = 0; a < 6; a++)
			bonus += float2int32( static_cast< Player* >( c )->SpellHealDoneByAttribute[a][m_spellProto->School] * static_cast< Player* >( c )->GetUInt32Value( UNIT_FIELD_STAT0 + a) );

		bonus += c->HealDoneMod[GetSpellProto()->School] + m_target->HealTakenMod[m_spellProto->School];
		//Druid Tree of Life form. it should work not like this, but it's better then nothing.
		if( static_cast< Player* >( c )->IsInFeralForm() && static_cast< Player* >( c )->GetShapeShift() == FORM_TREE)
			bonus += float2int32( 0.25f * static_cast< Player* >( c )->GetUInt32Value( UNIT_FIELD_STAT4 ) );

		//Spell Coefficient
		if( m_spellProto->OTspell_coef_override >= 0 ) //In case we have forced coefficients
			bonus = float2int32( float( bonus ) * m_spellProto->OTspell_coef_override );
		else
		{
			//Bonus to HoT part
			if( m_spellProto->fixed_hotdotcoef >= 0 )
			{
				bonus = float2int32( float( bonus ) * m_spellProto->fixed_hotdotcoef );
				//we did most calculations in world.cpp, but talents that increase DoT spells duration
				//must be included now.
				if( c->IsPlayer() )
				{
					int durmod = 0;
					SM_FIValue(c->SM_FDur, &durmod, m_spellProto->SpellGroupType);
					bonus += float2int32( float( bonus * durmod ) / 15000.0f );
				}
			}
		}
	}

	if( c != NULL && m_spellProto->SpellGroupType )
	{
		int penalty_pct = 0;
		int penalty_flt = 0;
		SM_FIValue( c->SM_PPenalty, &penalty_pct, GetSpellProto()->SpellGroupType );
		bonus += bonus * ( penalty_pct / 100 );
		SM_FIValue( c->SM_FPenalty, &penalty_flt, GetSpellProto()->SpellGroupType );
		bonus += penalty_flt;
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
		int spell_flat_modifers=0;
		int spell_pct_modifers=0;
		SM_FIValue(c->SM_FPenalty,&spell_flat_modifers,GetSpellProto()->SpellGroupType);
		SM_FIValue(c->SM_PPenalty,&spell_pct_modifers,GetSpellProto()->SpellGroupType);
		if(spell_flat_modifers!=0 || spell_pct_modifers!=0)
			printf("!!!!!HEAL : spell dmg bonus(p=24) mod flat %d , spell dmg bonus(p=24) pct %d , spell dmg bonus %d, spell group %u\n",spell_flat_modifers,spell_pct_modifers,bonus,GetSpellProto()->SpellGroupType);
#endif
	}

	int amp = m_spellProto->EffectAmplitude[mod->i];
	if( !amp )
		amp = static_cast< EventableObject* >( this )->event_GetEventPeriod( EVENT_AURA_PERIODIC_HEAL );

	if( GetDuration() )
	{
		int ticks = ( amp > 0 ) ? GetDuration() / amp : 0;
		bonus = ( ticks > 0 ) ? bonus / ticks : 0;
	}
	else
		bonus = 0;

    //Downranking
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
    }

	int add = ( bonus + amount > 0 ) ? bonus + amount : 0;
	if( c != NULL )
	{
		add += float2int32( add * ( m_target->HealTakenPctMod[m_spellProto->School]+ c->HealDonePctMod[GetSpellProto()->School] / 100.0f));
		if (m_spellProto->SpellGroupType)
			SM_PIValue(c->SM_PDOT,&add,m_spellProto->SpellGroupType);
	}


	uint32 newHealth = m_target->GetUInt32Value( UNIT_FIELD_HEALTH ) + (uint32)add;

	if( newHealth <= m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) )
		m_target->SetUInt32Value( UNIT_FIELD_HEALTH, newHealth );
	else
		m_target->SetUInt32Value( UNIT_FIELD_HEALTH, m_target->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );

	SendPeriodicHealAuraLog( add );

	if( GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP )
	{
		m_target->Emote( EMOTE_ONESHOT_EAT );
	}

	// add threat
	Unit* u_caster = this->GetUnitCaster();
	if( u_caster != NULL )
	{

		uint32 base_threat=Spell::GetBaseThreat(add);
		int count = 0;
		Unit* unit;
		std::vector< Unit* > target_threat;
		if( base_threat > 0 )
		{
			target_threat.reserve(u_caster->GetInRangeCount()); // this helps speed

			for(std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
			{
				if((*itr)->GetTypeId() != TYPEID_UNIT)
					continue;
				unit = static_cast<Unit*>((*itr));
				if(unit->GetAIInterface()->GetNextTarget() == m_target)
				{
					target_threat.push_back(unit);
					++count;
				}
			}
			if(count == 0)
				count = 1;  // division against 0 protection
			/*
			When a tank hold multiple mobs, the threat of a heal on the tank will be split between all the mobs.
			The exact formula is not yet known, but it is more than the Threat/number of mobs.
			So if a tank holds 5 mobs and receives a heal, the threat on each mob will be less than Threat(heal)/5.
			Current speculation is Threat(heal)/(num of mobs *2)
			*/
			uint32 threat = base_threat / (count * 2);

			for(std::vector<Unit*>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
			{
				// for now we'll just use heal amount as threat.. we'll prolly need a formula though
				((Unit*)(*itr))->GetAIInterface()->HealReaction(u_caster, m_target, threat);
			}
		}

		if(m_target->IsInWorld() && u_caster->IsInWorld())
			u_caster->CombatStatus.WeHealed(m_target);
	}
}

void Aura::SpellAuraModAttackSpeed(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else
		SetPositive();

	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{

			static_cast< Player* >( m_target )->m_meleeattackspeedmod += mod->m_amount;
			static_cast< Player* >( m_target )->m_rangedattackspeedmod += mod->m_amount;
		}
		else
		{
			static_cast< Player* >( m_target )->m_meleeattackspeedmod -= mod->m_amount;
			static_cast< Player* >( m_target )->m_rangedattackspeedmod -= mod->m_amount;
		}
		static_cast< Player* >( m_target )->UpdateStats();
	}
	else
	{
		if(apply)
		{
			mod->fixed_amount[0] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME,mod->m_amount);
			mod->fixed_amount[1] = m_target->GetModPUInt32Value(UNIT_FIELD_BASEATTACKTIME_01,mod->m_amount);
			mod->fixed_amount[2] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,mod->m_amount);
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME, -mod->fixed_amount[0]);
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME_01, -mod->fixed_amount[1]);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[2]);
		}
		else
		{
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME, mod->fixed_amount[0]);
			m_target->ModUnsigned32Value(UNIT_FIELD_BASEATTACKTIME_01, mod->fixed_amount[1]);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[2]);
		}
	}

}

void Aura::SpellAuraModThreatGenerated(bool apply)
{
	if(!m_target)
		return;

	//shaman spell 30672 needs to be based on spell schools
//	if(m_target->GetGeneratedThreatModifyer() == mod->m_amount)
	{
		mod->m_amount < 0 ? SetPositive() : SetNegative();
		apply ? m_target->ModGeneratedThreatModifyer(mod->m_amount) : m_target->ModGeneratedThreatModifyer(-(mod->m_amount));
//		return;
	}
/*	else // if we this aura is better then the old one, replace the effect.
	{
		if(apply)
		{
			if(m_target->GetGeneratedThreatModifyer() < mod->m_amount)
			{
//				m_target->ModGeneratedThreatModifyer(0); //wtf ?
				m_target->ModGeneratedThreatModifyer( mod->m_amount );
			}
		}
	}*/
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
	if(!m_target) return;

	if(m_spellProto->Id == 38554) return; // Absorb Eye of Grillok

	if(apply)
	{
		// Check Mechanic Immunity
		// Stun is a tricky one... it's used for all different kinds of mechanics as a base Aura
		if( m_target && !IsPositive() && m_spellProto->NameHash != SPELL_HASH_ICE_BLOCK )  // ice block stuns you, don't want our own spells to ignore stun effects
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

		if(m_target->m_rooted == 1)
			m_target->Root();

		m_target->m_stunned++;
		m_target->m_special_state |= UNIT_STATE_STUN;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

		if(m_target->GetTypeId() == TYPEID_UNIT)
			m_target->GetAIInterface()->SetNextTarget( (Unit*)NULL );

		// remove the current spell (for channelers)
		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid &&
			m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
		{
			m_target->m_currentSpell->cancel();
			m_target->m_currentSpell = 0;
		}

		//warrior talent - second wind triggers on stun and immobilize. This is not used as proc to be triggered always !
		Unit *caster = GetUnitCaster();
		if( caster && caster->IsPlayer() && m_target )
			static_cast<Player*>(caster)->EventStunOrImmobilize( m_target );
		if( m_target && m_target->IsPlayer() && caster )
			static_cast<Player*>(m_target)->EventStunOrImmobilize( caster, true );
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where imunity can cancel only 1 mod and not whole spell
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
			if(m_target->GetAIInterface()->GetNextTarget() != 0)
				target = m_target->GetAIInterface()->GetNextTarget();

			if(!target) return;
			m_target->GetAIInterface()->AttackReaction(target, 1, 0);
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
	for(uint32 x=0;x<7;x++)
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
		SetPositive();
		m_target->SetStealth(GetSpellId());
		if( m_spellProto->NameHash == SPELL_HASH_STEALTH)
			m_target->SetFlag(UNIT_FIELD_BYTES_2,0x1E000000);//sneak anim

		m_target->SetFlag(UNIT_FIELD_BYTES_1, 0x020000);
		if( m_target->IsPlayer() )
			m_target->SetFlag(PLAYER_FIELD_BYTES2, 0x2000);

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_STEALTH);
		m_target->m_stealthLevel += mod->m_amount;

		// hack fix for vanish stuff
		if( m_spellProto->NameHash == SPELL_HASH_VANISH && m_target->GetTypeId() == TYPEID_PLAYER )	 // Vanish
		{

		bool freeMovements = false;
		freeMovements = true;

		// remove the caster from imparing movements
		if( freeMovements )
		{
			for( uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; x++ )
			{
				if( m_target->m_auras[x] != NULL )
				{
					if( m_target->m_auras[x]->GetSpellProto()->MechanicsType == 7 || m_target->m_auras[x]->GetSpellProto()->MechanicsType == 11 ) // Remove roots and slow spells
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

			// check for stealh spells
			Player* p_caster = static_cast< Player* >( m_target );
			uint32 stealth_id = 0;
			SpellSet::iterator itr = p_caster->mSpells.begin();
			SpellSet::iterator end = p_caster->mSpells.end();
			for(; itr != end; ++itr)
			{
				if((*itr) == 1787 || (*itr) == 1786 || (*itr) == 1785 || (*itr) == 1784)
				{
					stealth_id = *itr;
					break;
				}
			}
			if(stealth_id)
				p_caster->CastSpell(p_caster, dbcSpell.LookupEntry(stealth_id), true);

			if(p_caster->IsMounted())
				p_caster->RemoveAura(p_caster->m_MountSpellId);
		}
	}
	else
	{
		m_target->SetStealth(0);
		m_target->m_stealthLevel -= mod->m_amount;
		if( m_spellProto->NameHash == SPELL_HASH_STEALTH)
			m_target->RemoveFlag(UNIT_FIELD_BYTES_2,0x1E000000);

		m_target->RemoveFlag(UNIT_FIELD_BYTES_1, 0x020000);

		if( m_target->GetTypeId() == TYPEID_PLAYER )
		{
			m_target->RemoveFlag(PLAYER_FIELD_BYTES2, 0x2000);
			/*WorldPacket data(12);
			data.SetOpcode(SMSG_COOLDOWN_EVENT);
			data << (uint32)GetSpellProto()->Id << m_target->GetGUID();
			static_cast< Player* >( m_target )->GetSession()->SendPacket (&data);*/
			packetSMSG_COOLDOWN_EVENT cd;
			cd.guid = m_target->GetGUID();
			cd.spellid = m_spellProto->Id;
			static_cast<Player*>(m_target)->GetSession()->OutPacket( SMSG_COOLDOWN_EVENT, sizeof(packetSMSG_COOLDOWN_EVENT), &cd);
			if( ((Player*)m_target)->m_outStealthDamageBonusPeriod && ((Player*)m_target)->m_outStealthDamageBonusPct )
				((Player*)m_target)->m_outStealthDamageBonusTimer = (uint32)UNIXTIME + ((Player*)m_target)->m_outStealthDamageBonusPeriod;
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
	if(m_spellProto->Effect[mod->i] == 128)
		return;

	if(apply)
	{
		m_target->m_invisFlag = mod->m_miscValue;
		if( m_target->GetTypeId() == TYPEID_PLAYER )
		{
			if( GetSpellId() == 32612 ) 
				static_cast<Player*>(m_target)->SetFlag( PLAYER_FIELD_BYTES2, 0x4000 ); //Mage Invis self visual
		}
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

	assert(mod->m_miscValue < INVIS_FLAG_TOTAL);
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

	uint32 add = float2int32(m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * (RegenPct / 100.0f));

	uint32 newHealth = m_target->GetUInt32Value(UNIT_FIELD_HEALTH) + add;

	if(newHealth <= m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
	else
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	SendPeriodicAuraLog(m_casterGuid, m_target, m_spellProto->Id, m_spellProto->School, add, 0, 0, FLAG_PERIODIC_HEAL);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
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

	uint32 add = float2int32(m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1) * (RegenPct / 100.0f));

	uint32 newHealth = m_target->GetUInt32Value(UNIT_FIELD_POWER1) + add;

	if(newHealth <= m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
		m_target->SetUInt32Value(UNIT_FIELD_POWER1, newHealth);
	else
		m_target->SetUInt32Value(UNIT_FIELD_POWER1, m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1));

	// CAPT
	// TODO: sniff it or disasm wow.exe to find the mana flag
	//SendPeriodicAuraLog(m_target, m_casterGuid, GetSpellProto()->Id, FLAG_PERIODIC_HEAL, add,true);
	//SendPeriodicAuraLog(m_target, m_casterGuid, GetSpellProto()->Id, FLAG_PERIODIC_HEAL, add);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
}

void Aura::SpellAuraModResistance(bool apply)
{
	uint32 Flag = mod->m_miscValue;
	int32 amt;
	if(apply)
	{
		amt = mod->m_amount;
		if(amt <0 )//dont' change it
			SetNegative();
		else
			SetPositive();
	}
	else
		amt = -mod->m_amount;

	if( this->GetSpellProto() && ( this->GetSpellProto()->NameHash == SPELL_HASH_FAERIE_FIRE || this->GetSpellProto()->NameHash == SPELL_HASH_FAERIE_FIRE__FERAL_ ) )
		m_target->m_can_stealth = !apply;

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
		for(uint32 x=0;x<7;x++)
		{
			if(Flag & (((uint32)1)<<x))
			{
				static_cast<Creature*>(m_target)->FlatResistanceMod[x]+=amt;
				static_cast<Creature*>(m_target)->CalcResistance(x);
			}
		}
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
		//FIXME: positive or negative?
		uint32 sp = GetSpellProto()->EffectTriggerSpell[mod->i];
		SpellEntry *spe = dbcSpell.LookupEntry(sp);
		if(!sp || !spe)
		{
			//	sp=22845;
			return;//null spell
		}

		Unit *m_caster = GetUnitCaster();
		if(!m_caster)
			return;

		if(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
		{
			sEventMgr.AddEvent(this, &Aura::EventPeriodicTriggerSpell, spe,
			EVENT_AURA_PERIODIC_TRIGGERSPELL,GetSpellProto()->EffectAmplitude[mod->i], 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

            periodic_target = m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT);
		}
		else if(m_target)
		{
			sEventMgr.AddEvent(this, &Aura::EventPeriodicTriggerSpell, spe,
				EVENT_AURA_PERIODIC_TRIGGERSPELL,GetSpellProto()->EffectAmplitude[mod->i], 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			periodic_target = m_target->GetGUID();
		}
	}
}

void Aura::EventPeriodicTriggerSpell(SpellEntry* spellInfo)
{
	// Trigger Spell
	// check for spell id

	Unit *m_caster=GetUnitCaster();
	if(!m_caster || !m_caster->IsInWorld())
		return;

	if( spellInfo->EffectImplicitTargetA[0] == 18 )			// Hellfire, if there are any others insert here
	{
		Spell *spell = new Spell(m_caster, spellInfo, true, this);
		SpellCastTargets targets;
		targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION;
		targets.m_srcX = m_caster->GetPositionX();
		targets.m_srcY = m_caster->GetPositionY();
		targets.m_srcZ = m_caster->GetPositionZ();
		spell->prepare(&targets);
		return;
	}

	Object * oTarget = m_target->GetMapMgr()->_GetObject(periodic_target);
	if(oTarget==NULL)
		return;

	if(oTarget->GetTypeId()==TYPEID_DYNAMICOBJECT)
	{
		Spell *spell = new Spell(m_caster, spellInfo, true, this);
		SpellCastTargets targets;
		targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
		targets.m_destX = oTarget->GetPositionX();
		targets.m_destY = oTarget->GetPositionY();
		targets.m_destZ = oTarget->GetPositionZ();
		spell->prepare(&targets);
		return;
	}

	Unit *pTarget = ((Unit*)oTarget);

	if(!oTarget->IsUnit())
		return;

	if(!pTarget || pTarget->isDead())
	{
		SendInterrupted(SPELL_FAILED_TARGETS_DEAD, m_caster);
		SendChannelUpdate(0, m_caster);
		this->Remove();
		return;
	}

	if(pTarget != m_caster && !isAttackable(m_caster, pTarget))
	{
		SendInterrupted(SPELL_FAILED_BAD_TARGETS, m_caster);
		SendChannelUpdate(0, m_caster);
		this->Remove();
		return;
	}

	if(spellInfo->spellIconID == 225 ) // this is arcane missles to avoid casting on self
		if(m_casterGuid == pTarget->GetGUID())
			return;

	// set up our max Range
	float maxRange = GetMaxRange( dbcSpellRange.LookupEntry( spellInfo->rangeIndex ) );

	if( spellInfo->SpellGroupType )
	{
		SM_FFValue( m_caster->SM_FRange, &maxRange, spellInfo->SpellGroupType );
		SM_PFValue( m_caster->SM_PRange, &maxRange, spellInfo->SpellGroupType );
	}

	if( m_caster->IsStunned() || m_caster->IsFeared() || m_caster->GetDistance2dSq( pTarget ) > ( maxRange*maxRange ) )
	{
		// no longer valid
		SendInterrupted(SPELL_FAILED_INTERRUPTED, m_caster);
		SendChannelUpdate(0, m_caster);
		this->Remove();
		return;
	}

	Spell *spell = new Spell(m_caster, spellInfo, true, this);
	SpellCastTargets targets;
	targets.m_unitTarget = pTarget->GetGUID();
	targets.m_targetMask = TARGET_FLAG_UNIT;
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

void Aura::EventPeriodicEnergize(uint32 amount,uint32 type)
{
	/*Unit*m_caster=GetUnitCaster();
	if(!m_caster) return;

	if(m_target->isAlive() && m_caster->isAlive())
	{
			uint32 powerField;
			uint32 currentPower;
			switch(GetSpellProto()->powerType)
			{
				case POWER_TYPE_HEALTH:{
						powerField = UNIT_FIELD_HEALTH;
									   }break;
				case POWER_TYPE_MANA:{
						powerField = UNIT_FIELD_POWER1;
									 }break;
				case POWER_TYPE_RAGE:{
						powerField = UNIT_FIELD_POWER2;
									 }break;
				case POWER_TYPE_ENERGY:{
						powerField = UNIT_FIELD_POWER4;
									   }break;
				default:{
						sLog.outDebug("unknown power type");
						}break;
			};
			if((GetSpellProto()->powerType != type) && (GetSpellId() != 1539))//Feed Pet
			{

				currentPower = m_target->GetUInt32Value(powerField);

				if(currentPower < (GetSpellProto()->manaCost ))//wtf??
					m_target->SetUInt32Value(powerField, 0);
				else
					m_target->SetUInt32Value(powerField, currentPower - GetSpellProto()->manaCost );
			}*/
			//meaning of this is unknown....reenable it if's needed
	uint32 POWER_TYPE=UNIT_FIELD_POWER1+type;

	ASSERT(POWER_TYPE<=UNIT_FIELD_POWER5);

	uint32 curEnergy = m_target->GetUInt32Value(POWER_TYPE);
	uint32 maxEnergy = m_target->GetUInt32Value(POWER_TYPE+6);
	uint32 totalEnergy = curEnergy+amount;
	if(totalEnergy > maxEnergy)
		m_target->SetUInt32Value(POWER_TYPE,maxEnergy);
	else
		m_target->SetUInt32Value(POWER_TYPE,totalEnergy);

	SendPeriodicAuraLog( m_casterGuid, m_target, m_spellProto->Id, m_spellProto->School, amount, 0, 0, FLAG_PERIODIC_ENERGIZE);

	if((GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP) && type == 0)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
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
		if( m_target )
		{
			if( m_target->MechanicsDispels[MECHANIC_ROOTED] )
			{
				m_flags |= 1 << mod->i;
				return;
			}
		}
		SetNegative();

		m_target->m_rooted++;

		if(m_target->m_rooted == 1)
			m_target->Root();

		/* -Supalosa- TODO: Mobs will attack nearest enemy in range on aggro list when rooted. */
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where imunity can cancel only 1 mod and not whole spell
	{
		m_target->m_rooted--;

		if(m_target->m_rooted == 0)
			m_target->Unroot();

		if(m_target->GetTypeId() == TYPEID_UNIT)
			m_target->GetAIInterface()->AttackReaction(GetUnitCaster(), 1, 0);
	}
}

void Aura::SpellAuraModSilence(bool apply)
{
	if(apply)
	{
		m_target->m_silenced++;
		m_target->m_special_state |= UNIT_STATE_SILENCE;
		m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);

		// remove the current spell (for channelers)
		if(m_target->m_currentSpell && m_target->GetGUID() != m_casterGuid &&
			m_target->m_currentSpell->getState() == SPELL_STATE_CASTING )
		{
			m_target->m_currentSpell->cancel();
			m_target->m_currentSpell = 0;
		}
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
	if(apply)
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
		ReflectSpellSchool *rss = new ReflectSpellSchool;

		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->school = -1;
		m_target->m_reflectSpellSchool.push_back(rss);
	}
	else
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete *i;
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
	}
}

void Aura::SpellAuraModStat(bool apply)
{
	int32 stat = (int32)mod->m_miscValue;
	int32 val;

	if(apply)
	{
		val = mod->m_amount;
		if (val<0)
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		val = -mod->m_amount;
	}

	if (stat == -1)//all stats
	{
		if(m_target->IsPlayer())
		{
			for(uint32 x=0;x<5;x++)
			{
				if(val>0)
					static_cast< Player* >( m_target )->FlatStatModPos[x] += val;
				else
					static_cast< Player* >( m_target )->FlatStatModNeg[x] -= val;

				static_cast< Player* >( m_target )->CalcStat(x);
			}

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			for(uint32 x=0;x<5;x++)
			{
				static_cast<Creature*>(m_target)->FlatStatMod[x] += val;
				static_cast<Creature*>(m_target)->CalcStat(x);
			}
		}
	}
	else if(stat >= 0)
	{
		ASSERT(mod->m_miscValue < 5);
		if(m_target->IsPlayer())
		{
			if(mod->m_amount>0)
				static_cast< Player* >( m_target )->FlatStatModPos[mod->m_miscValue] += val;
			else
				static_cast< Player* >( m_target )->FlatStatModNeg[mod->m_miscValue] -= val;

			static_cast< Player* >( m_target )->CalcStat(mod->m_miscValue);

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			static_cast<Creature*>(m_target)->FlatStatMod[mod->m_miscValue]+=val;
			static_cast<Creature*>(m_target)->CalcStat(mod->m_miscValue);
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
	//there can not be 2 slow downs only most powerfull is applied
	if(apply)
	{
		// Check Mechanic Immunity
		if( m_target )
		{
			if( m_target->MechanicsDispels[MECHANIC_ENSNARED] )
			{
				m_flags |= 1 << mod->i;
				return;
			}
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

		//let's check Mage talents if we proc anythig
		if(m_spellProto->School==SCHOOL_FROST)
		{
			//yes we are freezing the bastard, so can we proc anything on this ?
			Unit *caster = GetUnitCaster();
			if( caster && caster->IsPlayer() && m_target )
				static_cast<Player*>(caster)->EventStunOrImmobilize( m_target );
			if( m_target && m_target->IsPlayer() && caster )
				static_cast<Player*>(m_target)->EventStunOrImmobilize( caster, true );
		}
		m_target->speedReductionMap.insert(make_pair(m_spellProto->Id, mod->m_amount));
		//m_target->m_slowdown=this;
		//m_target->m_speedModifier += mod->m_amount;
	}
	else if( (m_flags & (1 << mod->i)) == 0 ) //add these checks to mods where imunity can cancel only 1 mod and not whole spell
	{
		map< uint32, int32 >::iterator itr = m_target->speedReductionMap.find(m_spellProto->Id);
		if(itr != m_target->speedReductionMap.end())
			m_target->speedReductionMap.erase(itr);
		//m_target->m_speedModifier -= mod->m_amount;
		//m_target->m_slowdown=NULL;
	}
	if(m_target->GetSpeedDecrease())
		m_target->UpdateSpeed();

}

void Aura::SpellAuraModIncreaseHealth(bool apply)
{
	int32 amt;

	if(apply)
	{
		//threet special cases. We should move these to scripted spells maybe
		switch(m_spellProto->Id)
		{
			case 23782:// Gift of Life
			  mod->m_amount = 1500;
			  break;
			case 12976:// Last Stand
			  mod->m_amount = (uint32)(m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.3);
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
			m_target->ModUnsigned32Value(UNIT_FIELD_HEALTH,amt);
		else
		{
			if((int32)m_target->GetUInt32Value(UNIT_FIELD_HEALTH)>-amt)//watch it on remove value is negative
				m_target->ModUnsigned32Value(UNIT_FIELD_HEALTH,amt);
			else m_target->SetUInt32Value(UNIT_FIELD_HEALTH,1); //do not kill player but do strip him good
		}
	}
	else
		m_target->ModUnsigned32Value(UNIT_FIELD_MAXHEALTH, amt);
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
	uint32 powerField = UNIT_FIELD_POWER1 + mod->m_miscValue;

	m_target->ModUnsigned32Value(powerField,apply?mod->m_amount:-mod->m_amount);
	m_target->ModUnsigned32Value(powerField+6,apply?mod->m_amount:-mod->m_amount);

	if(powerField == UNIT_FIELD_POWER1 && m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt = apply ? mod->m_amount : -mod->m_amount;
		static_cast< Player* >( m_target )->SetManaFromSpell(static_cast< Player* >( m_target )->GetManaFromSpell() + amt);
	}
}

void Aura::SpellAuraModShapeshift(bool apply)
{
	if( p_target == NULL )
		return;

	if( p_target->m_MountSpellId && p_target->m_MountSpellId != m_spellProto->Id )
		if( !(mod->m_miscValue & FORM_BATTLESTANCE | FORM_DEFENSIVESTANCE | FORM_BERSERKERSTANCE ) )
			m_target->RemoveAura( p_target->m_MountSpellId ); // these spells are not compatible

	uint32 spellId = 0;
	uint32 modelId = 0;

	bool freeMovements = false;

	switch( mod->m_miscValue )
	{
	case FORM_CAT:
		{//druid
			freeMovements = true;
			spellId = 3025;
			if(apply)
			{
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_ENERGY);
				m_target->SetUInt32Value(UNIT_FIELD_MAXPOWER4,100);//100 Energy
				m_target->SetUInt32Value(UNIT_FIELD_POWER4,0);//0 Energy
				if(m_target->getRace() == RACE_NIGHTELF)//NE
					modelId = 892;
				else //TAUREN
					modelId = 8571;

			}
			else
			{//turn back to mana
				//m_target->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME,oldap);
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_MANA);
				if(m_target->m_stealth)
				{
					uint32 sp = m_target->m_stealth;
					m_target->m_stealth = 0;
					m_target->RemoveAura(sp);//prowl
				}
			}
			static_cast< Player* >( m_target )->UpdateAttackSpeed();

		} break;
	case FORM_TREE:{
		modelId  = 864;
		freeMovements=true;
		spellId = 5420;//3122;
		} break;
	case FORM_TRAVEL:
		{//druid
			freeMovements = true;
			spellId = 5419;
			modelId = 918;

			if( apply )
			{
				if( m_target->IsPlayer() )
					static_cast< Player* >( m_target )->m_MountSpellId = m_spellProto->Id;
			}
			else
			{
				if( m_target->IsPlayer() )
					static_cast< Player* >( m_target )->m_MountSpellId = 0;
			}

		} break;
	case FORM_AQUA:
		{//druid aqua
			freeMovements = true;
			spellId = 5421;
			modelId = 2428;
		} break;
	case FORM_BEAR:
		{//druid only
			freeMovements = true;
			spellId = 1178;
			if(apply)
			{
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_RAGE);
				m_target->SetUInt32Value(UNIT_FIELD_MAXPOWER2, 1000);
				m_target->SetUInt32Value(UNIT_FIELD_POWER2, 0);//0 rage

				if(m_target->getRace() == RACE_NIGHTELF)
					modelId = 2281;
				else //TAUREN
					modelId = 2289;

				//some say there is a second effect
				SpellEntry* spellInfo = dbcSpell.LookupEntry( 21178 );

				Spell *sp = new Spell( m_target, spellInfo, true, NULL );
				SpellCastTargets tgt;
				tgt.m_unitTarget = m_target->GetGUID();
				sp->prepare( &tgt );
			}
			else
			{//reset back to mana
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_MANA);
				m_target->RemoveAura( 21178 ); // remove Bear Form (Passive2)
			}
		} break;
	case FORM_DIREBEAR:
		{//druid only
			freeMovements = true;
			spellId = 9635;
			if(apply)
			{
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_RAGE);
				m_target->SetUInt32Value(UNIT_FIELD_MAXPOWER2, 1000);
				m_target->SetUInt32Value(UNIT_FIELD_POWER2, 0);//0 rage
				if(m_target->getRace() == 4)//NE
					modelId = 2281;
				else //TAUREN
					modelId = 2289;
			}
			else //reset back to mana
				m_target->SetByte(UNIT_FIELD_BYTES_0,3,POWER_TYPE_MANA);
		} break;
	case FORM_GHOSTWOLF:
		{
			modelId = 4613;
			if( apply )
			{
				if( m_target->IsPlayer() )
					static_cast< Player* >( m_target )->m_MountSpellId = m_spellProto->Id;
			}
			else
			{
				if( m_target->IsPlayer() )
					static_cast< Player* >( m_target )->m_MountSpellId = 0;
			}
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
				/*WorldPacket data(12);
				data.SetOpcode(SMSG_COOLDOWN_EVENT);
				data << (uint32)GetSpellProto()->Id << m_target->GetGUID();
				static_cast< Player* >( m_target )->GetSession()->SendPacket(&data);*/
				packetSMSG_COOLDOWN_EVENT cd;
				cd.spellid = m_spellProto->Id;
				cd.guid = m_target->GetGUID();
				((Player*)m_target)->GetSession()->OutPacket(SMSG_COOLDOWN_EVENT, sizeof(packetSMSG_COOLDOWN_EVENT), &cd);
			}
		}break;
	case FORM_FLIGHT:
		{// druid
			freeMovements = true;
			spellId = 33948;
			if(apply)
			{
			    if(m_target->getRace() == RACE_NIGHTELF)
				    modelId = 20857;
			    else
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
				if(m_target->getRace() == RACE_NIGHTELF)
					modelId = 15374;
				else
					modelId = 15375;
			}
		}break;
	case FORM_SWIFT: //not tested yet, right now going on trust
		{// druid
			freeMovements = true;
			spellId = 40121; //Swift Form Passive
			if(apply)
			{
				if(m_target->getRace() == RACE_NIGHTELF)//NE
					modelId = 21243;
				else //TAUREN
					modelId = 21244;
			}
		}break;
	case FORM_SPIRITOFREDEMPTION:
		{
			spellId = 27795;
			modelId = 12824;
		}break;
	}

	if( apply )
	{
		if( m_target->getClass() == WARRIOR && m_target->GetUInt32Value( UNIT_FIELD_POWER2 ) > static_cast< Player* >( m_target )->m_retainedrage )
			m_target->SetUInt32Value(UNIT_FIELD_POWER2, static_cast< Player* >( m_target )->m_retainedrage );

		if( m_target->getClass() == DRUID )
		{
			if( Rand( static_cast< Player* >( m_target )->m_furorChance ) )
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

					Spell *sp = new Spell( m_target, spellInfo, true, NULL );
					SpellCastTargets tgt;
					tgt.m_unitTarget = m_target->GetGUID();
					sp->prepare(&tgt);
				}
			}
		}

		if( spellId != GetSpellId() )
		{
			if( static_cast< Player* >( m_target )->m_ShapeShifted )
				static_cast< Player* >( m_target )->RemoveAura( static_cast< Player* >( m_target )->m_ShapeShifted );

			static_cast< Player* >( m_target )->m_ShapeShifted = GetSpellId();
		}

		if( modelId != 0 )
		{
			m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, modelId );
			m_target->EventModelChange();
		}

		static_cast< Player* >( m_target )->SetShapeShift( mod->m_miscValue );

		// check for spell id
		if( spellId == 0 )
			return;

		SpellEntry* spellInfo = dbcSpell.LookupEntry(spellId );

		Spell *sp = new Spell( m_target, spellInfo, true, NULL );
		SpellCastTargets tgt;
		tgt.m_unitTarget = m_target->GetGUID();
		sp->prepare( &tgt );

		// remove the caster from imparing movements
		if( freeMovements )
		{
			for( uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; x++ )
			{
				if( m_target->m_auras[x] != NULL )
				{
					if( m_target->m_auras[x]->GetSpellProto()->MechanicsType == 7 || m_target->m_auras[x]->GetSpellProto()->MechanicsType == 11 ) // Remove roots and slow spells
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
		static_cast< Player* >( m_target )->EventTalentHearthOfWildChange( true );
	}
	else
	{
		//execute before changing shape back
		static_cast< Player* >( m_target )->EventTalentHearthOfWildChange( false );
		m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		m_target->EventModelChange();
		if( spellId != GetSpellId() )
		{
			if( spellId )
				m_target->RemoveAura( spellId );
		}
		static_cast< Player* >( m_target )->m_ShapeShifted = 0;

		static_cast< Player* >( m_target )->SetShapeShift( 0 );

	}
	static_cast< Player* >( m_target )->UpdateStats();
}

void Aura::SpellAuraModEffectImmunity(bool apply)
{
	if( m_spellProto->Id == 24937 )
		SetPositive();

	if( m_spellProto->Id == 23333 || m_spellProto->Id == 23335 || m_spellProto->Id == 34976 )
	{
		if( !apply )
		{
            Player* plr = static_cast< Player* >( GetUnitCaster() );
			if( plr == NULL || plr->GetTypeId() != TYPEID_PLAYER || plr->m_bg == NULL)
				return;

			if( plr->m_bg->GetType() == BATTLEGROUND_WARSUNG_GULCH )
				((WarsongGulch*)plr->m_bg)->DropFlag(plr);
			else if( plr->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM )
				((EyeOfTheStorm*)plr->m_bg)->DropFlag(plr);
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
		if( !m_target || !m_target->isAlive())
			return;

		Aura * pAura;
		for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
		{
			pAura = m_target->m_auras[i];
			if( pAura != this && pAura != NULL && !pAura->IsPassive() && !pAura->IsPositive() && !(pAura->GetSpellProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY) )
			{
				pAura->Remove();
			}
		}
	}

	if(apply)
	{
		//fixme me may be negative
		Unit * c = GetUnitCaster();
			if(c)
			{
				if(isAttackable(c,m_target))
					SetNegative();
				else SetPositive();
			}else
				SetPositive();
		for(uint32 i = 0; i < 7; i++)
		{
			if(mod->m_miscValue & (1<<i))
			{
				m_target->SchoolImmunityList[i] ++;
				m_target->RemoveAurasOfSchool(i, false, true);
			}
		}
	}
	else
	{
		for(int i = 0; i < 7; i++)
		{
			if(mod->m_miscValue & (1<<i))
				m_target->SchoolImmunityList[i]--;
		}
	}
}

void Aura::SpellAuraModDmgImmunity(bool apply)
{

}

void Aura::SpellAuraModDispelImmunity(bool apply)
{
	assert(mod->m_miscValue < 10);
	if(apply)
		m_target->dispels[mod->m_miscValue]++;
	else
		m_target->dispels[mod->m_miscValue]--;

	if(apply)
	{
		for(uint32 x=0;x<MAX_AURAS;x++)
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
		ProcTriggerSpell pts;
		pts.origId = GetSpellProto()->Id;
		pts.caster = m_casterGuid;
		if(GetSpellProto()->EffectTriggerSpell[mod->i])
			pts.spellId=GetSpellProto()->EffectTriggerSpell[mod->i];
		else
		{
			sLog.outDebug("Warning,trigger spell is null for spell %u",GetSpellProto()->Id);
			return;
		}
		pts.procChance = GetSpellProto()->procChance;
		pts.procFlags = GetSpellProto()->procFlags;
		pts.procCharges = GetSpellProto()->procCharges;
		pts.LastTrigger = 0;
		pts.deleted = false;

		if( m_spellProto->NameHash == SPELL_HASH_THE_TWIN_BLADES_OF_AZZINOTH )
		{
			/* The Twin Blades of Azzinoth.
			 * According to comments on wowhead, this proc has ~0.75ppm (procs-per-minute). */
			Item* mh = static_cast< Player* >( m_target )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			Item* of = static_cast< Player* >( m_target )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			if( mh != NULL && of != NULL )
			{
				float mhs = float( mh->GetProto()->Delay );
				float ohs = float( of->GetProto()->Delay );
				pts.procChance = FL2UINT( float( mhs * ohs / ( 800.0f * ( mhs + ohs ) ) ) ); // 0.75 ppm
			}
		}

		/* We have a custom formula for seal of command. */
		else if( m_spellProto->NameHash == SPELL_HASH_SEAL_OF_COMMAND )
		{
			// default chance of proc
			pts.procChance = 25;

			/* The formula for SoC proc rate is: [ 7 / ( 60 / Weapon Speed ) - from wowwiki */
			if(m_target->IsPlayer())
			{
				float weapspeed = 1.0f;
				Item* itm = static_cast< Player* >( m_target )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if( itm != NULL )
					weapspeed = float( itm->GetProto()->Delay );
				pts.procChance = FL2UINT( float(7.0f / (600.0f / weapspeed)) );
				if( pts.procChance >= 50 )
					pts.procChance = 50;
			}
		}
/*		pts.ospinfo = m_spellProto;
		pts.spinfo = sSpellStore.LookupEntry(pts.spellId);
		if(!pts.spinfo)
		{
			sLog.outDebug("Error, could not register procspell %u\n",pts.spellId);
			return;
		}*/
		m_target->m_procSpells.push_front(pts);
		sLog.outDebug("%u is registering %u chance %u flags %u charges %u triggeronself %u interval %u\n",pts.origId,pts.spellId,pts.procChance,m_spellProto->procFlags & ~PROC_TARGET_SELF,m_spellProto->procCharges,m_spellProto->procFlags & PROC_TARGET_SELF,m_spellProto->proc_interval);
	}
	else
	{
		for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
		{
			if(itr->origId == GetSpellId() && itr->caster == m_casterGuid && !itr->deleted)
			{
				itr->deleted = true;
				break; //only 1 instance of a proc spell per caster ?
			}
		}
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
		sLog.outDebug("registering dmg proc %u, school %u, flags %u, charges %u \n",ds.m_spellId,ds.m_school,ds.m_flags,m_spellProto->procCharges);
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
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			if(static_cast< Player* >( m_target )->TrackingSpell)
				m_target->RemoveAura( static_cast< Player* >( m_target )->TrackingSpell);

			m_target->SetUInt32Value(PLAYER_TRACK_CREATURES,(uint32)1<< (mod->m_miscValue-1));
			static_cast< Player* >( m_target )->TrackingSpell = GetSpellId();
		}
		else
		{
			static_cast< Player* >( m_target )->TrackingSpell = 0;
			m_target->SetUInt32Value(PLAYER_TRACK_CREATURES,0);
		}
	}
}

void Aura::SpellAuraTrackResources(bool apply)
{
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			if(static_cast< Player* >( m_target )->TrackingSpell)
				m_target->RemoveAura(static_cast< Player* >( m_target )->TrackingSpell);

		m_target->SetUInt32Value(PLAYER_TRACK_RESOURCES,(uint32)1<< (mod->m_miscValue-1));
		static_cast< Player* >( m_target )->TrackingSpell = GetSpellId();
		}
		else
		{
			static_cast< Player* >( m_target )->TrackingSpell = 0;
				m_target->SetUInt32Value(PLAYER_TRACK_RESOURCES,0);
		}
	}
}

void Aura::SpellAuraModParryPerc(bool apply)
{
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt;
		if(apply)
		{
			amt = mod->m_amount;
			if(amt<0)
				SetNegative();
			else
				SetPositive();

		}
		else
			amt = -mod->m_amount;

		static_cast< Player* >( m_target )->SetParryFromSpell(static_cast< Player* >( m_target )->GetParryFromSpell() + amt );
		static_cast< Player* >( m_target )->UpdateChances();
	}
}

void Aura::SpellAuraModDodgePerc(bool apply)
{
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt = mod->m_amount;
//		SM_FIValue(m_target->SM_FSPELL_VALUE, &amt, GetSpellProto()->SpellGroupType);
		if(apply)
		{
			if(amt<0)
				SetNegative();
			else
				SetPositive();
		}
		else
			amt = -amt;
		static_cast< Player* >( m_target )->SetDodgeFromSpell(static_cast< Player* >( m_target )->GetDodgeFromSpell() + amt );
		static_cast< Player* >( m_target )->UpdateChances();
	}
}

void Aura::SpellAuraModBlockPerc(bool apply)
{
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt;
		if(apply)
		{
			amt = mod->m_amount;
			if(amt<0)
				SetNegative();
			else
				SetPositive();
		}
		else
			amt = -mod->m_amount;

		static_cast< Player* >( m_target )->SetBlockFromSpell(static_cast< Player* >( m_target )->GetBlockFromSpell() + amt);
		static_cast< Player* >( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModCritPerc(bool apply)
{
	if (m_target->IsPlayer())
	{
		if(apply)
		{
			WeaponModifier md;
			md.value = float(mod->m_amount);
			md.wclass = GetSpellProto()->EquippedItemClass;
			md.subclass = GetSpellProto()->EquippedItemSubClass;
			static_cast< Player* >( m_target )->tocritchance.insert(make_pair(GetSpellId(), md));
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
			static_cast< Player* >( m_target )->tocritchance.erase(GetSpellId());
		}
		static_cast< Player* >( m_target )->UpdateChances();
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
	Unit*m_caster = GetUnitCaster();

	if(!m_caster)
		return;

	if(m_target->isAlive() && m_caster->isAlive())
	{
		if(m_target->SchoolImmunityList[GetSpellProto()->School])
			return;

		int amp = m_spellProto->EffectAmplitude[mod->i];
		if( !amp )
			amp = static_cast< EventableObject* >( this )->event_GetEventPeriod( EVENT_AURA_PERIODIC_LEECH );

		int bonus = 0;

		if(GetDuration())
		{
			float fbonus = float( m_caster->GetSpellDmgBonus(m_target,GetSpellProto(),amount,true) ) * 0.5f;
			if(fbonus < 0) fbonus = 0.0f;
			float ticks= float((amp) ? GetDuration()/amp : 0);
			fbonus = (ticks) ? fbonus/ticks : 0;
			bonus = float2int32(fbonus);
		}

		amount += bonus;

		uint32 Amount = (uint32)min( amount, m_target->GetUInt32Value( UNIT_FIELD_HEALTH ) );
		uint32 newHealth = m_caster->GetUInt32Value(UNIT_FIELD_HEALTH) + Amount ;

		uint32 mh = m_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
		if(newHealth <= mh)
			m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
		else
			m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, mh);

		//SendPeriodicHealAuraLog(Amount);
		WorldPacket data(SMSG_PERIODICAURALOG, 32);
		data << m_caster->GetNewGUID();
		data << m_target->GetNewGUID();
		data << m_spellProto->Id;
		data << uint32(1);
		data << uint32(FLAG_PERIODIC_HEAL);
		data << uint32(Amount);
		m_target->SendMessageToSet(&data,true);

		SendPeriodicAuraLog(m_target, m_target, m_spellProto->Id, m_spellProto->School, Amount, 0, 0, FLAG_PERIODIC_LEECH);

		//deal damage before we add healing bonus to damage
		m_caster->DealDamage(m_target, Amount, 0, 0, GetSpellProto()->Id,true);

		//add here bonus to healing taken. Maybe not all spells should receive it ?
		/*
		//zack : have no idea if we should use downranking here so i'm removing it until confirmed
		float healdoneaffectperc = 1500 / 3500;
		//Downranking
		if(GetSpellProto()->baseLevel > 0 && GetSpellProto()->maxLevel > 0)
		{
			float downrank1 = 1.0f;
			if (GetSpellProto()->baseLevel < 20)
			downrank1 = 1.0f - (20.0f - float (GetSpellProto()->baseLevel) ) * 0.0375f;
			float downrank2 = ( float(GetSpellProto()->maxLevel + 5.0f) / float(m_caster->getLevel()) );
			if (downrank2 >= 1 || downrank2 < 0)
			downrank2 = 1.0f;
			healdoneaffectperc *= downrank1 * downrank2;
		}
		*/
	}
}

void Aura::SpellAuraModHitChance(bool apply)
{
	if (m_target->IsUnit())
	{
		if (apply)
		{
			static_cast< Unit* >( m_target )->SetHitFromMeleeSpell( static_cast< Unit* >( m_target )->GetHitFromMeleeSpell() + mod->m_amount);
			if(mod->m_amount<0)
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			static_cast< Unit* >( m_target )->SetHitFromMeleeSpell( static_cast< Unit* >( m_target )->GetHitFromMeleeSpell() - mod->m_amount);
			if( static_cast< Unit* >( m_target )->GetHitFromMeleeSpell() < 0 )
			{
				static_cast< Unit* >( m_target )->SetHitFromMeleeSpell( 0 );
			}
		}
	}
}

void Aura::SpellAuraModSpellHitChance(bool apply)
{
	if (m_target->IsPlayer())
	{
		if (apply)
		{
			static_cast< Player* >( m_target )->SetHitFromSpell(static_cast< Player* >( m_target )->GetHitFromSpell() + mod->m_amount);
			if(mod->m_amount<0)
				SetNegative();
			else
				SetPositive();
		}
		else
		{
			static_cast< Player* >( m_target )->SetHitFromSpell(static_cast< Player* >( m_target )->GetHitFromSpell() - mod->m_amount);
			if(static_cast< Player* >( m_target )->GetHitFromSpell() < 0)
			{
				static_cast< Player* >( m_target )->SetHitFromSpell(0);
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

	if(p_target && p_target->m_MountSpellId)
		m_target->RemoveAura(p_target->m_MountSpellId);

   // SetPositive();
	switch( GetSpellProto()->Id )
	{
		case 20584://wisp
			m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, apply ? 10045:m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		break;

		case 30167: // Red Ogre Costume
		{
			if( apply )
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, 11549 );
			else
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		}break;

		case 41301: // Time-Lost Figurine
		{
			if( apply )
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, 18628 );
			else
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		}break;

		case 16739: // Orb of Deception
		{
			if( apply )
			{
				if(m_target->getRace() == RACE_ORC)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10139);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10140);
				}
				if(m_target->getRace() == RACE_TAUREN)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10136);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10147);
				}
				if(m_target->getRace() == RACE_TROLL)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10135);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10134);
				}
				if(m_target->getRace() == RACE_UNDEAD)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10146);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10145);
				}
				if(m_target->getRace() == RACE_BLOODELF)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17829);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17830);
				}

				if(m_target->getRace() == RACE_GNOME)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10148);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10149);
				}
				if(m_target->getRace() == RACE_DWARF)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10141);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10142);
				}
				if(m_target->getRace() == RACE_HUMAN)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10137);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10138);
				}
				if(m_target->getRace() == RACE_NIGHTELF)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10143);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 10144);
				}
				if(m_target->getRace() == RACE_DRAENEI)
				{
					if( m_target->getGender() == 0 )
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17827);
					else
						m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, 17828);
				}
			}
			else
				m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
		}break;

		case 42365:	// murloc costume
			m_target->SetUInt32Value( UNIT_FIELD_DISPLAYID, apply ? 21723 : m_target->GetUInt32Value( UNIT_FIELD_NATIVEDISPLAYID ) );
			break;

		case 118://polymorh
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
					m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayId);

					// remove the current spell (for channelers)
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
					m_target->SetUInt32Value(UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
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
					// halfling = 7818
					// maybe 7842 as its from a lesser npc
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, 7842);
				}
				else
				{
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
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
					m_target->SetUInt32Value (UNIT_FIELD_DISPLAYID, m_target->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));
				}
		}break;
	};

	if( m_target )
		m_target->EventModelChange();
}

void Aura::SpellAuraModSpellCritChance(bool apply)
{
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amt;
		if(apply)
		{
			amt = mod->m_amount;
			if(amt<0)
				SetNegative();
			else
				SetPositive();
		}
		else
			amt = -mod->m_amount;

		m_target->spellcritperc += amt;
		static_cast< Player* >( m_target )->SetSpellCritFromSpell( static_cast< Player* >( m_target )->GetSpellCritFromSpell() + amt );
		static_cast< Player* >( m_target )->UpdateChanceFields();
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
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		WorldPacket data(SMSG_FORCE_SWIM_SPEED_CHANGE, 17);
		data << m_target->GetNewGUID();
		data << (uint32)2;
		data << m_target->m_swimSpeed;
		static_cast< Player* >( m_target )->GetSession()->SendPacket(&data);
	}
}

void Aura::SpellAuraModCratureDmgDone(bool apply)
{
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			for(uint32 x = 0; x < 11; x++)
				if(mod->m_miscValue & (((uint32)1)<<x))
					static_cast< Player* >( m_target )->IncreaseDamageByType[x+1] += mod->m_amount;

			mod->m_amount < 0 ? SetNegative() : SetPositive();
		}
		else
			for(uint32 x = 0; x < 11; x++)
				if(mod->m_miscValue & (((uint32)1)<<x) )
					static_cast< Player* >( m_target )->IncreaseDamageByType[x+1] -= mod->m_amount;
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
	float current = m_target->GetFloatValue(OBJECT_FIELD_SCALE_X);
	float delta = mod->m_amount/100.0f;

	m_target->SetFloatValue(OBJECT_FIELD_SCALE_X, apply ? (current+current*delta) : current/(1.0f+delta));
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
		uint32 newHealth = m_caster->GetUInt32Value(UNIT_FIELD_HEALTH) + 1000;

		uint32 mh = m_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
		if(newHealth <= mh)
			m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
		else
			m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, mh);

		SendPeriodicAuraLog(m_target, m_target, m_spellProto->Id, m_spellProto->School, 1000, 0, 0, FLAG_PERIODIC_LEECH);
	}
}

void Aura::SpellAuraPeriodicManaLeech(bool apply)
{
	if(apply)
	{
		uint32 amt=mod->m_amount;
		sEventMgr.AddEvent(this, &Aura::EventPeriodicManaLeech,amt,
			EVENT_AURA_PERIODIC_LEECH,	 GetSpellProto()->EffectAmplitude[mod->i],0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicManaLeech(uint32 amount)
{
	Unit* m_caster = GetUnitCaster();
	if(!m_caster)
		return;
	if(m_target->isAlive() && m_caster->isAlive())
	{

		int32 amt = (int32)min( amount, m_target->GetUInt32Value( UNIT_FIELD_POWER1 ) );
		uint32 cm = m_caster->GetUInt32Value(UNIT_FIELD_POWER1)+amt;
		uint32 mm = m_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
		if(cm <= mm)
			m_caster->SetUInt32Value(UNIT_FIELD_POWER1, cm);
		else
			m_caster->SetUInt32Value(UNIT_FIELD_POWER1, mm);
		m_target->ModUnsigned32Value(UNIT_FIELD_POWER1, -amt);
	}
}

void Aura::SpellAuraModCastingSpeed(bool apply)
{
	float current = m_target->GetFloatValue(UNIT_MOD_CAST_SPEED);
	if(apply)
		current -= float(mod->m_amount / 100.0f);
	else
		current += float(mod->m_amount / 100.0f);
	m_target->SetFloatValue(UNIT_MOD_CAST_SPEED, current );

	// spell haste/slow is limited to 100% fast or 100% slow.
	if( m_target->GetFloatValue( UNIT_MOD_CAST_SPEED ) >= 2.0f )
		m_target->SetFloatValue( UNIT_MOD_CAST_SPEED , 2.0f );
	else if( m_target->GetFloatValue( UNIT_MOD_CAST_SPEED ) <= 0.5f )
		m_target->SetFloatValue( UNIT_MOD_CAST_SPEED , 0.5f );
}

void Aura::SpellAuraFeignDeath(bool apply)
{
	if( m_target->IsPlayer() )
	{
		Player* pTarget = static_cast< Player* >( m_target );
		WorldPacket data(50);
		if( apply )
		{
			pTarget->EventAttackStop();
			pTarget->SetFlag( UNIT_FIELD_FLAGS_2, 1 );
			pTarget->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH );
			//pTarget->SetFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD );
			pTarget->SetFlag( UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD );
			//pTarget->SetUInt32Value( UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD );

			data.SetOpcode( SMSG_START_MIRROR_TIMER );
			data << uint32( 2 );		// type
			data << uint32( GetDuration() );
			data << uint32( GetDuration() );
			data << uint32( 0xFFFFFFFF );
			data << uint8( 0 );
			data << uint32( m_spellProto->Id );		// ???
			pTarget->GetSession()->SendPacket( &data );

			data.Initialize(0x03BE);
			data << pTarget->GetGUID();
//			pTarget->setDeathState(DEAD);

			//now get rid of mobs agro. pTarget->CombatStatus.AttackersForgetHate() - this works only for already attacking mobs
		    for(std::set<Object*>::iterator itr = pTarget->GetInRangeSetBegin(); itr != pTarget->GetInRangeSetEnd(); itr++ )
			{
				if((*itr)->IsUnit() && ((Unit*)(*itr))->isAlive())
				{
					if((*itr)->GetTypeId()==TYPEID_UNIT)
						((Unit*)(*itr))->GetAIInterface()->RemoveThreatByPtr(pTarget);
					//if this is player and targeting us then we interrupt cast
					if( ( (*itr) )->IsPlayer() )
					{
						//if player has selection on us
						if( static_cast< Player* >( *itr )->GetSelection()==pTarget->GetGUID())
						{
							//it seems that all these do not work in 2.3
							//static_cast< Player* >( (*itr) )->SetSelection(0);//loose selection
							//static_cast< Player* >( (*itr) )->SetUInt64Value(UNIT_FIELD_TARGET, 0);
						}
						if( static_cast< Player* >( *itr )->isCasting())
							static_cast< Player* >( *itr )->CancelSpell( NULL ); //cancel current casting spell

						static_cast< Player* >( *itr )->GetSession()->SendPacket( &data );
					}
				}
			}
			pTarget->setDeathState(ALIVE);
		}
		else
		{
			pTarget->RemoveFlag(UNIT_FIELD_FLAGS_2, 1);
			pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH);
			pTarget->RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_DEAD);
			//pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DEAD);
			//pTarget->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			data.SetOpcode(SMSG_STOP_MIRROR_TIMER);
			data << uint32(2);
			pTarget->GetSession()->SendPacket(&data);
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
	Absorb *ab;
	if(apply)
	{
		SetPositive();

		int32 val = mod->m_amount;
		Player * plr = static_cast< Player* >( GetUnitCaster() );
		if( plr )
		{
			//This will fix talents that affects damage absorved.
			int flat = 0;
			SM_FIValue( plr->SM_FSPELL_VALUE, &flat, GetSpellProto()->SpellGroupType );
			val += float2int32( float( val * flat ) / 100.0f );

			//For spells Affected by Bonus Healing we use Dspell_coef_override.
			if( GetSpellProto()->Dspell_coef_override >= 0 )
				val += float2int32( float( plr->HealDoneMod[GetSpellProto()->School] ) * GetSpellProto()->Dspell_coef_override );
			//For spells Affected by Bonus Damage we use OTspell_coef_override.
			else if( GetSpellProto()->OTspell_coef_override >= 0 )
				val += float2int32( float( plr->GetDamageDoneMod( GetSpellProto()->School ) ) * GetSpellProto()->OTspell_coef_override );
		}

		ab = new Absorb;
		ab->amt = val;
		ab->spellid = GetSpellId();
		ab->caster = m_casterGuid;
		for(uint32 x=0;x<7;x++)
			if (mod->m_miscValue & (((uint32)1)<<x) )
				m_target->Absorbs[x].push_back(ab);
	}
	else
	{
		ab = NULL;
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				for(SchoolAbsorb::iterator i = m_target->Absorbs[x].begin(); i != m_target->Absorbs[x].end(); i++)
				{
					if((*i)->spellid == GetSpellId() && (*i)->caster==m_casterGuid )
					{
						ab = (*i);
						m_target->Absorbs[x].erase(i);
						break;
					}
				}

				/*if(ab)
					delete ab;//should not be null, but just in case...*/
			}
		}
	}
}

void Aura::SpellAuraModSpellCritChanceSchool(bool apply)
{
	if(apply)
	{
		for(uint32 x=0;x<7;x++)
			if (mod->m_miscValue & (((uint32)1)<<x))
				m_target->SpellCritChanceSchool[x] += mod->m_amount;
		if(mod->m_amount < 0)
			SetNegative();
		else
			SetPositive();
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				m_target->SpellCritChanceSchool[x] -= mod->m_amount;
				/*if(m_target->SpellCritChanceSchool[x] < 0)
					m_target->SpellCritChanceSchool[x] = 0;*/
			}
		}
	}
	if(m_target->IsPlayer())
		static_cast< Player* >( m_target )->UpdateChanceFields();
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
	for(uint32 x=0;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->ModFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+x,val/100.0f);
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
	if(apply)
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete (*i);
				i = m_target->m_reflectSpellSchool.erase(i);
			}
			else
				++i;
		}
		ReflectSpellSchool *rss = new ReflectSpellSchool;

		rss->chance = mod->m_amount;
		rss->spellId = GetSpellId();
		rss->require_aura_hash = 0;
		if(m_spellProto->Attributes == 0x400D0 && m_spellProto->AttributesEx == 0)
		{
			rss->school = (int)(log10((float)mod->m_miscValue) / log10((float)2));
		}
		else
			rss->school = m_spellProto->School;

		m_target->m_reflectSpellSchool.push_back(rss);
	}
	else
	{
		for(std::list<struct ReflectSpellSchool*>::iterator i = m_target->m_reflectSpellSchool.begin();i != m_target->m_reflectSpellSchool.end();)
		{
			if(GetSpellId() == (*i)->spellId)
			{
				delete *i;
				i = m_target->m_reflectSpellSchool.erase(i);
				break;
			}
			else
				++i;
		}
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
//sLog.outString( "Aura::SpellAuraMechanicImmunity begun." );
	if(apply)
	{
		//sLog.outString( "mod->m_miscValue = %u" , (uint32) mod->m_miscValue );
		//sLog.outString( "Incrementing MechanicsDispels (current value: %u, new val: %u)" , m_target->MechanicsDispels[mod->m_miscValue] , m_target->MechanicsDispels[mod->m_miscValue] + 1 );
		assert(mod->m_miscValue < 31);
		m_target->MechanicsDispels[mod->m_miscValue]++;

		if(mod->m_miscValue != 16 && mod->m_miscValue != 25 && mod->m_miscValue != 19) // dont remove bandages, Power Word and protection effect
		{
			/*
			sLog.outString( "Removing values because we're not a bandage, PW:S or forbearance" );
			for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
				if(m_target->m_auras[x])
				{
					sLog.outString( "Found aura in %u" , x );
					if(m_target->m_auras[x]->GetSpellProto()->MechanicsType == (uint32)mod->m_miscValue)
					{
						sLog.outString( "Removing aura: %u, ID %u" , x , m_target->m_auras[x]->GetSpellId() );
						m_target->m_auras[x]->Remove();
					}
					else if(mod->m_miscValue == 11) // if got immunity for slow, remove some that are not in the mechanics
					{
						sLog.outString( "Removing roots" );
						for(int i=0;i<3;i++)
							if(m_target->m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED)
							{
								sLog.outString( "Removed snare aura in slot %u, spellid %u" , x , m_target->m_auras[x]->GetSpellId() );
								m_target->m_auras[x]->Remove();
								break;
							}
					}
				}
			*/
			/* Supa's test run of Unit::RemoveAllAurasByMechanic */
			if( m_target ) // just to be sure?
			{
				m_target->RemoveAllAurasByMechanic( (uint32)mod->m_miscValue , -1 , false );
			}

			if(m_spellProto->Id==42292)
			{
				// insignia of the A/H
				for(uint32 x= MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
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
	}
	else
		m_target->MechanicsDispels[mod->m_miscValue]--;
}

void Aura::SpellAuraMounted(bool apply)
{
	if(!p_target) return;

	if(m_target->IsStealth())
	{
		uint32 id = m_target->m_stealth;
		m_target->m_stealth = 0;
		m_target->RemoveAura(id);
	}

	if(apply)
	{
		SetPositive();

		//p_target->AdvanceSkillLine(762); // advance riding skill

		if(p_target->m_bg)
			p_target->m_bg->HookOnMount(p_target);

		if(p_target->m_MountSpellId)
			m_target->RemoveAura(p_target->m_MountSpellId);

		m_target->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_MOUNT);

		CreatureInfo* ci = CreatureNameStorage.LookupEntry(mod->m_miscValue);
		if(!ci) return;

		uint32 displayId = ci->Male_DisplayID;
		if(!displayId) return;

		p_target->m_MountSpellId = m_spellProto->Id;
		p_target->flying_aura = 0;
		m_target->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , displayId);
		//m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);

		if( p_target->GetShapeShift() && !(p_target->GetShapeShift() & FORM_BATTLESTANCE | FORM_DEFENSIVESTANCE | FORM_BERSERKERSTANCE ) && p_target->m_ShapeShifted != m_spellProto->Id )
			p_target->RemoveAura( p_target->m_ShapeShifted );

		/*
		//desummon summons
		if( p_target->GetSummon() != NULL )
		{
			if(p_target->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				p_target->GetSummon()->Dismiss(false); // warlock summon -> dismiss
			else
				p_target->GetSummon()->Remove(false, true, true); // hunter pet -> just remove for later re-call
		}
		*/
	}
	else
	{
		p_target->m_MountSpellId = 0;
		p_target->flying_aura = 0;
		m_target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);
		//m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	}
}

void Aura::SpellAuraModDamagePercDone(bool apply)
{
	float val = (apply) ? mod->m_amount/100.0f : -mod->m_amount/100.0f;

	switch (GetSpellId()) //dirty or mb not fix bug with wand specializations
	{
	case 6057:
	case 6085:
	case 14524:
	case 14525:
	case 14526:
	case 14527:
	case 14528:
		return;
	}
	if(m_target->IsPlayer())
	{
		if(GetSpellProto()->EquippedItemClass==-1)//does not depend on weapon
		{
			for(uint32 x=0;x<7;x++)
			{
				if (mod->m_miscValue & (((uint32)1)<<x) )
				{
					m_target->ModFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + x, val); // display to client (things that are weapon dependant dont get displayed)
				}
			}
		}
		if(mod->m_miscValue & 1)
		{
			if(apply)
			{
				WeaponModifier md;
				md.value = val;
				md.wclass = GetSpellProto()->EquippedItemClass;
				md.subclass = GetSpellProto()->EquippedItemSubClass;
				static_cast< Player* >( m_target )->damagedone.insert(make_pair(GetSpellId(), md));
			}
			else
			{
				std::map<uint32,WeaponModifier>::iterator i= static_cast< Player* >( m_target )->damagedone.begin();

				for( ; i != static_cast< Player* >( m_target )->damagedone.end() ; i++ )
				{
					if(( *i).first == GetSpellId() )
					{
						static_cast< Player* >( m_target )->damagedone.erase(i);
						break;
					}
				}
				static_cast< Player* >( m_target )->damagedone.erase(GetSpellId());
			}
		}
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				static_cast<Creature*>(m_target)->ModDamageDonePct[x] += val;
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
		if(m_target->IsPlayer())
		{
			for(uint32 x=0;x<5;x++)
			{
				if(mod->m_amount>0)
					static_cast< Player* >( m_target )->StatModPctPos[x] += val;
				else
					static_cast< Player* >( m_target )->StatModPctNeg[x] -= val;

				static_cast< Player* >( m_target )->CalcStat(x);
			}

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else
		{
			for(uint32 x=0;x<5;x++)
			{
				static_cast<Creature*>(m_target)->StatModPct[x] += val;
				static_cast<Creature*>(m_target)->CalcStat(x);
			}
		}
	}
	else
	{
		ASSERT(mod->m_miscValue < 5);
		if(m_target->IsPlayer())
		{
			if(mod->m_amount>0)
				static_cast< Player* >( m_target )->StatModPctPos[mod->m_miscValue] += val;
			else
				static_cast< Player* >( m_target )->StatModPctNeg[mod->m_miscValue] -= val;

			static_cast< Player* >( m_target )->CalcStat(mod->m_miscValue);

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			static_cast<Creature*>(m_target)->StatModPct[mod->m_miscValue]+=val;
			static_cast<Creature*>(m_target)->CalcStat(mod->m_miscValue);
		}
	}
}

void Aura::SpellAuraSplitDamage(bool apply)
{
	if( !m_target->IsUnit() )
		return;

	//SUPA:FIXU
	// rewrite, copy-paste DamageProc struct.
	if(apply)
	{
		DamageSplitTarget ds;
		ds.m_flatDamageSplit = 0;
		ds.m_spellId = GetSpellProto()->Id;
		ds.m_pctDamageSplit = mod->m_amount / 100.0f;
		ds.damage_type = mod->m_miscValue;
		ds.creator = (void*)this;
		ds.m_target = GetCaster()->GetGUID();
		m_target->m_damageSplitTargets.push_back(ds);
		//sLog.outDebug("registering dmg split %u, school %u, flags %u, charges %u \n",ds.m_spellId,ds.m_school,ds.m_flags,m_spellProto->procCharges);
	}
	else
	{
		for(std::list<struct DamageSplitTarget>::iterator i = m_target->m_damageSplitTargets.begin();i != m_target->m_damageSplitTargets.end();i++)
		{
			if(i->creator == this)
			{
				m_target->m_damageSplitTargets.erase(i);
				break;
			}
		}
	}

	//Unit* uCaster = GetUnitCaster();
	//if( !uCaster )
	//	return;

	//if( m_target == uCaster )
	//	return;

	// Correct (?) implementation of SplitDamage.
	/* COPY: DamageShield
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
*/
	//brrr, temporarty fix, this is used only by soul link atm:P
/*
	if( !m_target->IsPet() )
		return;

	Player* petOwner = static_cast< Pet* >( m_target )->GetPetOwner();

	float val;
	if(apply)
	{
		val = mod->m_miscValue/100.0f;
	}
	else
	{
		val = -mod->m_miscValue/100.0f;
	}

	for(uint32 x=0;x<7;x++)
	{
		petOwner->DamageTakenPctMod[x] -= val;
		m_target->DamageTakenPctMod[x] += val;
	}
	*/
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

void Aura::SpellAuraDrinkNew(bool apply)
{
	if( apply )
	{
		SetPositive();
		sEventMgr.AddEvent(this, &Aura::EventPeriodicDrink, uint32(float2int32(float(mod->m_amount)/5.0f)),
			EVENT_AURA_PERIODIC_REGEN, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Aura::EventPeriodicDrink(uint32 amount)
{
	uint32 v = m_target->GetUInt32Value(UNIT_FIELD_POWER1) + amount;
	if( v > m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1) )
		v = m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1);

	m_target->SetUInt32Value(UNIT_FIELD_POWER1, v);
}

void Aura::EventPeriodicHeal1(uint32 amount)
{
	if(!m_target->isAlive())
		return;

	uint32 ch = m_target->GetUInt32Value(UNIT_FIELD_HEALTH);
	ch+=amount;
	uint32 mh = m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH);

	if(ch>mh)
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH,mh);
	else
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH,ch);

	if(GetSpellProto()->AuraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{
		m_target->Emote(EMOTE_ONESHOT_EAT);
	}
	else
	{
		if(!(m_spellProto->buffType & SPELL_TYPE_ARMOR))
			SendPeriodicHealAuraLog(amount);
	}
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
	if (m_target->IsPlayer() && mod->m_miscValue == POWER_TYPE_MANA)
	{
		int32 val = (apply) ? mod->m_amount: -mod->m_amount;
		static_cast< Player* >( m_target )->m_ModInterrMRegen +=val;
		static_cast< Player* >( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraChannelDeathItem(bool apply)
{
	SetNegative(); //this should always be negative as npcs remove negative auras on death

	if(apply)
	{
		//dont need for now
	}
	else
	{
		if( m_target->GetTypeId() == TYPEID_UNIT || m_target->GetTypeId() == TYPEID_PLAYER )
		{
			if ( m_target->GetTypeId() == TYPEID_UNIT && ((Creature*)m_target)->GetCreatureName() != NULL && ((Creature*)m_target)->GetCreatureName()->Type == CRITTER )
				return;

			if(m_target->isDead())
			{
				Player *pCaster = m_target->GetMapMgr()->GetPlayer((uint32)m_casterGuid);
				if(!pCaster)
					return;
				/*int32 delta=pCaster->getLevel()-m_target->getLevel();
				if(abs(delta)>5)
					return;*/

				uint32 itemid = GetSpellProto()->EffectSpellGroupRelation[mod->i];

				ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);
				if(pCaster->GetItemInterface()->CalculateFreeSlots(proto) > 0)
				{
					Item *item = objmgr.CreateItem(itemid,pCaster);
					if(!item) return;


					item->SetUInt64Value(ITEM_FIELD_CREATOR,pCaster->GetGUID());
					if(!pCaster->GetItemInterface()->AddItemToFreeSlot(item))
					{
						pCaster->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_INVENTORY_FULL);
						delete item;
						return;
					}
					/*WorldPacket data(45);
					pCaster->GetSession()->BuildItemPushResult(&data, pCaster->GetGUID(), 1, 1, itemid ,0,0xFF,1,0xFFFFFFFF);
					pCaster->SendMessageToSet(&data, true);					*/
					SlotResult * lr = pCaster->GetItemInterface()->LastSearchResult();
					pCaster->GetSession()->SendItemPushResult(item,true,false,true,true,lr->ContainerSlot,lr->Slot,1);
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

	for(uint32 x=0;x<7;x++)
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
		m_target->PctRegenModifier += ((float)(mod->m_amount))/100.0f;
	else
		m_target->PctRegenModifier -= ((float)(mod->m_amount))/100.0f;
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
			uint32 dmg = (m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*5)/100;
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

	uint32 damage = float2int32(amount/100.0f*m_target->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	Unit * c = GetUnitCaster();

	if(c)
		c->SpellNonMeleeDamageLog(m_target, GetSpellProto()->Id, damage, pSpellId==0, true);
	else
		m_target->SpellNonMeleeDamageLog(m_target, GetSpellProto()->Id, damage, pSpellId==0, true);
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
		Also known as Apply Aura: Mod Uninteractible
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
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		m_target->m_invisible = apply;

		if(apply)
		{
			SetNegative();
			static_cast< Player* >( m_target )->SetMovement(MOVE_WATER_WALK, 4);
		}
		else
		{
			static_cast< Player* >( m_target )->SetMovement(MOVE_LAND_WALK, 7);
		}
	}
}

void Aura::SpellAuraMagnet(bool apply)
{
// fixed in cast function
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
	if (m_target->IsPlayer())
	{
		if(apply)
		{
			SetPositive();
			static_cast< Player* >( m_target )->_ModifySkillBonus(mod->m_miscValue,mod->m_amount);
		}
		else
			static_cast< Player* >( m_target )->_ModifySkillBonus(mod->m_miscValue,-mod->m_amount);

		static_cast< Player* >( m_target )->UpdateStats();
	}
}

void Aura::SpellAuraModAttackPower(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else
		SetPositive();
	m_target->ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS,apply? mod->m_amount : -mod->m_amount);
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

	for(uint32 x=0;x<7;x++)
	{
		if(Flag & (((uint32)1)<< x))
		{
			if(m_target->GetTypeId() == TYPEID_PLAYER)
			{
				if(mod->m_amount>0)
				{
					static_cast< Player* >( m_target )->ResistanceModPctPos[x] += amt;
				}
				else
				{
					static_cast< Player* >( m_target )->ResistanceModPctNeg[x] -= amt;
				}
				static_cast< Player* >( m_target )->CalcResistance(x);

			}
			else if(m_target->GetTypeId() == TYPEID_UNIT)
			{
				static_cast<Creature*>(m_target)->ResistanceModPct[x] += amt;
				static_cast<Creature*>(m_target)->CalcResistance(x);
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

		if( m_target->GetThreatModifyer() > mod->m_amount ) // replace old mod
		{
			m_target->ModThreatModifyer( 0 );
			m_target->ModThreatModifyer( mod->m_amount );
		}
	}
	else
	{
		if( m_target->GetThreatModifyer() == mod->m_amount ) // only remove it if it hasn't been replaced yet
		{
			m_target->ModThreatModifyer(-(mod->m_amount));
		}
	}
}

void Aura::SpellAuraWaterWalk( bool apply )
{
	if( m_target->IsPlayer() )
	{
		WorldPacket data( 12 );
		if( apply )
		{
			SetPositive();
			data.SetOpcode( SMSG_MOVE_WATER_WALK );
			data << m_target->GetNewGUID();
			data << uint32( 8 );
		}
		else
		{
			data.SetOpcode( SMSG_MOVE_LAND_WALK );
			data << m_target->GetNewGUID();
			data << uint32( 4 );
		}
		static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraFeatherFall( bool apply )
{
	//FIXME:Find true flag
	if( m_target->GetTypeId() == TYPEID_PLAYER )
	{
		WorldPacket data(12);
		if( apply )
		{
			data.SetOpcode(SMSG_MOVE_FEATHER_FALL);
			static_cast< Player* >( m_target )->m_noFallDamage = true;
		}
		else
		{
			data.SetOpcode(SMSG_MOVE_NORMAL_FALL);
			static_cast< Player* >( m_target )->m_noFallDamage = false;
		}
		data << m_target->GetNewGUID() << (uint32)0;
		static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraHover( bool apply )
{
	SetPositive();

	//TODO: FIXME: Find true flag for this
	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		WorldPacket data( apply ? SMSG_MOVE_SET_HOVER : SMSG_MOVE_UNSET_HOVER, 13 );
		data << m_target->GetNewGUID();
		data << uint32( 0 );
		static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
	}
}

void Aura::SpellAuraAddPctMod( bool apply )
{
	int32 val = apply ? mod->m_amount : -mod->m_amount;
	uint64 AffectedGroups = (uint64)GetSpellProto()->EffectSpellGroupRelation[mod->i] + ((uint64)GetSpellProto()->EffectSpellGroupRelation_high[mod->i] << 32);
	//printf("!!! the AffectedGroups %u ,the smt type %u,\n",AffectedGroups,mod->m_miscValue);

	switch( mod->m_miscValue )//let's generate warnings for unknown types of modifiers
	{
	case SMT_CRITICAL:
		SendModifierLog( &m_target->SM_CriticalChance, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_DURATION:
		SendModifierLog( &m_target->SM_PDur, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_RADIUS:
		SendModifierLog( &m_target->SM_PRadius, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_RANGE:
		SendModifierLog( &m_target->SM_PRange, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_CAST_TIME:
		SendModifierLog( &m_target->SM_PCastTime, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_COST:
		SendModifierLog( &m_target->SM_PCost, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_CRITICAL_DAMAGE:
		SendModifierLog( &m_target->SM_PCriticalDamage, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_SPELL_VALUE_PCT:
		SendModifierLog( &m_target->SM_PDOT, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_NONINTERRUPT:
		SendModifierLog( &m_target->SM_PNonInterrupt, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_JUMP_REDUCE:
		SendModifierLog( &m_target->SM_PJumpReduce, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_EFFECT_BONUS:
	case SMT_EFFECT:
		SendModifierLog( &m_target->SM_PEffectBonus, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_DAMAGE_DONE:
		SendModifierLog( &m_target->SM_PDamageBonus, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_SPELL_VALUE:
		SendModifierLog( &m_target->SM_PSPELL_VALUE, val, AffectedGroups, mod->m_miscValue, true );
		break;

	case SMT_PENALTY:
		SendModifierLog( &m_target->SM_PPenalty, val, AffectedGroups, mod->m_miscValue, true );
		break;
	//TODO: disabled until clarified
	/*
	case SMT_ATTACK_POWER_BONUS:
		SendModifierLog(&m_target->SM_PAPBonus,val,AffectedGroups,mod->m_miscValue,true);
		break;
	*/
	case SMT_COOLDOWN_DECREASE:
		SendModifierLog( &m_target->SM_PCooldownTime, val, AffectedGroups, mod->m_miscValue, true );
		break;

	//there are 2 spells in 2.1.1 that will only need attack power bonus
	case SMT_ATTACK_POWER_AND_DMG_BONUS:
		{
			if(GetSpellId()==36563 || GetSpellId()==37186)
			{
				SendModifierLog( &m_target->SM_PDamageBonus, val, AffectedGroups, mod->m_miscValue, true );
			}
			else
			{
				//these are seal of crusader spells
				SendModifierLog( &m_target->SM_PDamageBonus, val, AffectedGroups, mod->m_miscValue, true );
				SendModifierLog( &m_target->SM_PAPBonus, val, AffectedGroups, mod->m_miscValue, true );
			}
		}break;
	//TODO:
	/*
	case SMT_BLOCK:
	case SMT_THREAT_REDUCED:

	case SMT_TRIGGER:
	case SMT_TIME:
		break;
	*/
	//unknown Modifier type
	case SMT_RESIST_DISPEL:
		SendModifierLog( &m_target->SM_PRezist_dispell, val, AffectedGroups, mod->m_miscValue, true );
		break;
	default:
		sLog.outError( "Unknown spell modifier type %u in spell %u.<<--report this line to the developer\n", mod->m_miscValue, GetSpellId() );
		//don't add val, though we could formaly could do,but as we don't know what it is-> no sense
		break;
	}
}


void Aura::SendModifierLog( int32** m, int32 v, uint64 mask, uint8 type, bool pct )
{
	//WorldPacket data( SMSG_SET_FLAT_SPELL_MODIFIER + pct, 6 );
	packetSMSG_SET_FLAT_SPELL_MODIFIER data;

	if( *m == 0 )
	{
		*m = new int32[SPELL_GROUPS];
		for( uint32 x = 0; x < SPELL_GROUPS; x++ )
		{
			if( ( ((uint64)1) << x ) & mask )
			{
				(*m)[x] = v;

				if( !m_target->IsPlayer() )
					continue;

				/*data << uint8( x );		//group
				data << uint8( type );	//type
				data << v;				//value

				static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
				data.clear();*/

				data.group = x;
				data.type = type;
				data.v = v;
				static_cast<Player*>(m_target)->GetSession()->OutPacket( SMSG_SET_FLAT_SPELL_MODIFIER+ pct, sizeof( packetSMSG_SET_FLAT_SPELL_MODIFIER ), &data );
			}
			else
				(*m)[x] = 0;
		}
	}
	else
	{
		for( uint32 x = 0; x < SPELL_GROUPS; x++ )
		{
			if( ( ((uint64)1) << x ) & mask )
			{
				(*m)[x] += v;

				if( !m_target->IsPlayer() )
					continue;

				/*data << uint8( x );		//group
				data << uint8( type );	//type
				data << (*m)[x];		//value

				static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
				data.clear();*/

				data.group = x;
				data.type = type;
				data.v = (*m)[x];
				static_cast<Player*>(m_target)->GetSession()->OutPacket( SMSG_SET_FLAT_SPELL_MODIFIER+ pct, sizeof( packetSMSG_SET_FLAT_SPELL_MODIFIER ), &data );
			}
		}
	}
}

void Aura::SendDummyModifierLog( std::map< SpellEntry*, uint32 >* m, SpellEntry* spellInfo, uint32 i, bool apply, bool pct )
{
	//WorldPacket data( SMSG_SET_FLAT_SPELL_MODIFIER + pct, 6 );
	packetSMSG_SET_FLAT_SPELL_MODIFIER data;

	int32 v = spellInfo->EffectBasePoints[i] + 1;
	uint32 mask = spellInfo->EffectSpellGroupRelation[i];
	uint8 type = spellInfo->EffectMiscValue[i];

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

	for(uint32 x=0;x<SPELL_GROUPS;x++)
	{
		if((1<<x) & mask)
		{
			if(!m_target->IsPlayer())continue;
			/*data << uint8(x);//group
			data << uint8(type);//type
			data << v;//value
			static_cast< Player* >( m_target )->GetSession()->SendPacket(&data);
			data.clear();*/

			data.group = x;
			data.type = type;
			data.v = v;
			static_cast<Player*>(m_target)->GetSession()->OutPacket( SMSG_SET_FLAT_SPELL_MODIFIER+ pct, sizeof( packetSMSG_SET_FLAT_SPELL_MODIFIER ), &data );
		}
	}
}

void Aura::SpellAuraAddTargetTrigger(bool apply)
{
#ifdef NEW_PROCFLAGS
	if (m_target == NULL)
		return;

	if( apply )
	{
		ProcTriggerSpell pts;
		pts.parentId = GetSpellProto()->Id;
		pts.caster = m_casterGuid;
		pts.procCharges = GetSpellProto()->procCharges;
		pts.i = mod->i;
		pts.LastTrigger = 0;

		if(GetSpellProto()->EffectTriggerSpell[mod->i])
			pts.spellId=GetSpellProto()->EffectTriggerSpell[mod->i];
		else
		{
			sLog.outDebug("Warning,trigger spell is null for spell %u",GetSpellProto()->Id);
			return;
		}
		m_target->m_procSpells.push_front(pts);
		sLog.outDebug("%u is registering %u chance %u flags %u charges %u triggeronself %u interval %u\n",pts.origId,pts.spellId,pts.procChance,m_spellProto->procFlags & ~PROC_TARGET_SELF,m_spellProto->procCharges,m_spellProto->procFlags & PROC_TARGET_SELF,m_spellProto->proc_interval);
	}
	else
	{
		for(std::list<struct ProcTriggerSpell>::iterator itr = m_target->m_procSpells.begin();itr != m_target->m_procSpells.end();itr++)
		{
			if(itr->parentId == GetSpellId() && itr->caster == m_casterGuid && !itr->deleted)
			{
				itr->deleted = true;
				break; //only 1 instance of a proc spell per caster ?
			}
		}
	}
#endif
}

void Aura::SpellAuraModPowerRegPerc(bool apply)
{
	if(apply)
		m_target->PctPowerRegenModifier[mod->m_miscValue] += ((float)(mod->m_amount))/100.0f;
	else
		m_target->PctPowerRegenModifier[mod->m_miscValue] -= ((float)(mod->m_amount))/100.0f;
	if (m_target->IsPlayer())
		static_cast< Player* >( m_target )->UpdateStats();
}

void Aura::SpellAuraOverrideClassScripts(bool apply)
{
	if(!GetUnitCaster())
		return;
	if(!GetUnitCaster()->IsPlayer())
		return;
	//misc value is spell to add
	//spell familyname && grouprelation

	Player *plr = static_cast< Player* >(GetUnitCaster());

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
			if (m_target->IsPlayer())
			{
				int32 val = (apply) ? (mod->m_miscValue-908)*10 : -(mod->m_miscValue-908)*10;
				if (mod->m_miscValue==849)
					val = (apply) ? 10 : -10;
				static_cast< Player* >( m_target )->m_RootedCritChanceBonus += val;
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
				//this shoul actually add a new functionality to the spell and not override it. There is a lot to decode and to be done here
			}break;*/
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
		m_target->RangedDamageTaken -= mod->m_amount;;
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

	for(uint32 x=0;x<7;x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->HealTakenMod[x] += val;
		}
	}
}

void Aura::SpellAuraIgnoreRegenInterrupt(bool apply)
{
	if(!m_target->IsPlayer())
		return;

	if(apply)
		static_cast< Player* >( m_target )->PctIgnoreRegenModifier += ((float)(mod->m_amount))/100;
	else
		static_cast< Player* >( m_target )->PctIgnoreRegenModifier -= ((float)(mod->m_amount))/100;
}

void Aura::SpellAuraModMechanicResistance(bool apply)
{
	//silence=26 ?
	//mecanics=9 ?
	if(apply)
	{
		assert(mod->m_miscValue < 27);
		m_target->MechanicsResistancesPCT[mod->m_miscValue]+=mod->m_amount;

		if(mod->m_miscValue != 16 && mod->m_miscValue != 25 && mod->m_miscValue != 19) // dont remove bandages, Power Word and protection effect
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

	for(uint32 x=0; x<7; x++)
	{
		if (mod->m_miscValue & (((uint32)1)<<x) )
		{
			m_target->HealTakenPctMod[x] += ((float)(val))/100;
		}
	}
}

void Aura::SpellAuraModRangedAttackPower(bool apply)
{
	if(apply)
	{
		if(mod->m_amount > 0)
			SetPositive();
		else
			SetNegative();
		m_target->ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS,mod->m_amount);
	}
	else
		m_target->ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS,-mod->m_amount);
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
		m_target->DamageTakenPctMod[0]+=mod->m_amount;
	}
	else
		m_target->DamageTakenPctMod[0]-=mod->m_amount;
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
	uint32 maxField = UNIT_FIELD_MAXPOWER1 + mod->m_miscValue;

	if(apply)
	{
		mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value( maxField, mod->m_amount );
		m_target->ModUnsigned32Value( maxField, mod->fixed_amount[mod->i] );
		if( m_target->IsPlayer() && maxField == UNIT_FIELD_MAXPOWER1 )
			static_cast< Player* >( m_target )->SetManaFromSpell( ( ( Player* )m_target )->GetManaFromSpell() + mod->fixed_amount[mod->i] );
	}
	else
	{
		m_target->ModUnsigned32Value( maxField, -mod->fixed_amount[mod->i] );
		if( m_target->IsPlayer() && maxField == UNIT_FIELD_MAXPOWER1 )
			static_cast< Player* >( m_target )->SetManaFromSpell( ( ( Player* )m_target )->GetManaFromSpell() - mod->fixed_amount[mod->i] );
	}
}

void Aura::SpellAuraModIncreaseHealthPerc( bool apply )
{
	SetPositive();
	if( apply )
	{
		mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value( UNIT_FIELD_MAXHEALTH, mod->m_amount );
		m_target->ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, mod->fixed_amount[mod->i] );
		if( m_target->IsPlayer() )
			( ( Player* )m_target )->SetHealthFromSpell( ( ( Player* )m_target )->GetHealthFromSpell() + mod->fixed_amount[mod->i] );
	}
	else
	{
		m_target->ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, -mod->fixed_amount[mod->i] );
		if( m_target->IsPlayer() )
			( ( Player* )m_target )->SetHealthFromSpell( ( ( Player* )m_target )->GetHealthFromSpell() - mod->fixed_amount[mod->i] );
	}
}

void Aura::SpellAuraModManaRegInterrupt( bool apply )
{
	if( m_target->IsPlayer() )
	{
		if( apply )
			static_cast< Player* >( m_target )->m_ModInterrMRegenPCT += mod->m_amount;
		else
			static_cast< Player* >( m_target )->m_ModInterrMRegenPCT -= mod->m_amount;

		static_cast< Player* >( m_target )->UpdateStats();
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
		if(m_target->IsPlayer())
		{
			for(uint32 x=0;x<5;x++)
			{
				if(mod->m_amount>0)
					static_cast< Player* >( m_target )->TotalStatModPctPos[x] += val;
				else
					static_cast< Player* >( m_target )->TotalStatModPctNeg[x] -= val;
				static_cast< Player* >( m_target )->CalcStat(x);
			}

			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if(m_target->GetTypeId() == TYPEID_UNIT)
		{
			for(uint32 x=0;x<5;x++)
			{
				static_cast<Creature*>(m_target)->TotalStatModPct[x] += val;
				static_cast<Creature*>(m_target)->CalcStat(x);
			}
		}
	}
	else
	{
		ASSERT(mod->m_miscValue < 5);
		if(m_target->IsPlayer())
		{
			//druid hearth of the wild should add more features based on form
			if( m_spellProto->NameHash == SPELL_HASH_HEART_OF_THE_WILD )
			{
				//we should remove effect first
				static_cast< Player* >( m_target )->EventTalentHearthOfWildChange( false );
				//set new value
				if( apply )
					static_cast< Player* >( m_target )->SetTalentHearthOfWildPCT( val );
				else
					static_cast< Player* >( m_target )->SetTalentHearthOfWildPCT( 0 ); //this happens on a talent reset
				//reapply
				static_cast< Player* >( m_target )->EventTalentHearthOfWildChange( true );
			}

			if( mod->m_amount > 0 )
				static_cast< Player* >( m_target )->TotalStatModPctPos[mod->m_miscValue] += val;
			else
				static_cast< Player* >( m_target )->TotalStatModPctNeg[mod->m_miscValue] -= val;

			static_cast< Player* >( m_target )->CalcStat( mod->m_miscValue );
			static_cast< Player* >( m_target )->UpdateStats();
			static_cast< Player* >( m_target )->UpdateChances();
		}
		else if( m_target->GetTypeId() == TYPEID_UNIT )
		{
			static_cast< Creature* >( m_target )->TotalStatModPct[mod->m_miscValue]+=val;
			static_cast< Creature* >( m_target )->CalcStat(mod->m_miscValue);
		}
	}
}

void Aura::SpellAuraModHaste( bool apply )
{
	//blade flurry - attack a nearby opponent
	if( m_spellProto->NameHash == SPELL_HASH_BLADE_FLURRY )
	{
		if( apply )
			m_target->m_extrastriketargets++;
		else
			m_target->m_extrastriketargets--;
	}

	if( mod->m_amount < 0 )
		SetNegative();
	else
		SetPositive();

	// TODO: fix this hacky fix
	if( abs( mod->m_amount ) > 100 )
		return;

	if( m_target->GetTypeId() == TYPEID_PLAYER )
	{
		if( apply )
		{
			static_cast< Player* >( m_target )->m_meleeattackspeedmod += mod->m_amount;
		}
		else
		{
			static_cast< Player* >( m_target )->m_meleeattackspeedmod -= mod->m_amount;
		}
		static_cast< Player* >(m_target)->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME, mod->m_amount );
			mod->fixed_amount[mod->i*2] = m_target->GetModPUInt32Value( UNIT_FIELD_BASEATTACKTIME_01, mod->m_amount );

			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ) <= mod->fixed_amount[mod->i] )
				mod->fixed_amount[mod->i] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME ); //watch it, a negative timer might be bad ;)
			if( (int32)m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME_01 ) <= mod->fixed_amount[mod->i*2] )
				mod->fixed_amount[mod->i*2] = m_target->GetUInt32Value ( UNIT_FIELD_BASEATTACKTIME_01 );//watch it, a negative timer might be bad ;)

			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME, -mod->fixed_amount[mod->i] );
			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME_01, -mod->fixed_amount[mod->i*2] );
		}
		else
		{
			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME, mod->fixed_amount[mod->i] );
			m_target->ModUnsigned32Value( UNIT_FIELD_BASEATTACKTIME_01, mod->fixed_amount[mod->i*2] );
		}
	}
}

void Aura::SpellAuraForceReaction( bool apply )
{
	map<uint32,uint32>::iterator itr;
	Player * p_target = static_cast<Player*>( m_target );
	if( !m_target->IsPlayer() )
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

void Aura::SpellAuraModRangedHaste(bool apply)
{
	if(mod->m_amount<0)
		SetNegative();
	else
		SetPositive();

	if (m_target->GetTypeId() == TYPEID_PLAYER)
	{
		int32 amount = mod->m_amount;
//		if(GetSpellProto()->Id == 6150)// Quick Shots
//		{
//			Unit * pCaster = GetUnitCaster();
//			if(pCaster)
//				SM_FIValue(pCaster->SM_FSPELL_VALUE,&amount,0x100000);
//		}

		if( apply )
			static_cast< Player* >( m_target )->m_rangedattackspeedmod += amount;
		else
			static_cast< Player* >( m_target )->m_rangedattackspeedmod -= amount;
		static_cast< Player* >( m_target )->UpdateAttackSpeed();
	}
	else
	{
		if( apply )
		{
			mod->fixed_amount[mod->i] = m_target->GetModPUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,mod->m_amount);
			m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, -mod->fixed_amount[mod->i]);
		}
		else m_target->ModUnsigned32Value(UNIT_FIELD_RANGEDATTACKTIME, mod->fixed_amount[mod->i]);
	}
}

void Aura::SpellAuraModRangedAmmoHaste(bool apply)
{
	SetPositive();
	if( !m_target->IsPlayer() )
		return;

	Player* p = static_cast< Player* >( m_target );

	if( apply )
	{
		p->m_rangedattackspeedmod+=mod->m_amount;
	}
	else
	{
		p->m_rangedattackspeedmod-=mod->m_amount;
	}

	p->UpdateAttackSpeed();
}

void Aura::SpellAuraModResistanceExclusive(bool apply)
{
	SpellAuraModResistance(apply);
}

void Aura::SpellAuraRetainComboPoints(bool apply)
{
	if( m_target->IsPlayer() )
		static_cast< Player* >( m_target )->m_retainComboPoints = apply;
}

void Aura::SpellAuraResistPushback(bool apply)
{
	//DK:This is resist for spell casting delay
	//Only use on players for now

	if(m_target->IsPlayer())
	{
		int32 val;
		if(apply)
		{
			val = mod->m_amount;
			SetPositive();
		}
		else
			val=-mod->m_amount;

		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				static_cast< Player* >( m_target )->SpellDelayResist[x] += val;
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
	Unit * c;
	if(!(c=GetUnitCaster()))
		return;

	c->trackStealth = apply;
}

void Aura::SpellAuraModDetectedRange(bool apply)
{
	if(!m_target->IsPlayer())
		return;
	if(apply)
	{
		SetPositive();
		static_cast< Player* >( m_target )->DetectedRange += mod->m_amount;
	}
	else
	{
		static_cast< Player* >( m_target )->DetectedRange -= mod->m_amount;
	}
}

void Aura::SpellAuraSplitDamageFlat(bool apply)
{
	//DK:FIXME
	//SUPA:FIXU

	// rewrite, copy-paste DamageProc struct.
	if(apply)
	{
		DamageSplitTarget ds;
		ds.m_flatDamageSplit = mod->m_amount;
		ds.m_spellId = GetSpellProto()->Id;
		ds.m_pctDamageSplit = 0;
		ds.damage_type = mod->m_miscValue;
		ds.creator = (void*)this;
		ds.m_target = GetCaster()->GetGUID();
		m_target->m_damageSplitTargets.push_back(ds);
		//sLog.outDebug(\\\\\\\"registering dmg split %u, school %u, flags %u, charges %u \\\\\\\\n\\\\\\\",ds.m_spellId,ds.m_school,ds.m_flags,m_spellProto->procCharges);
	}
	else
	{
		for(std::list<struct DamageSplitTarget>::iterator i = m_target->m_damageSplitTargets.begin();i != m_target->m_damageSplitTargets.end();i++)
		{
			if(i->creator == this)
			{
				m_target->m_damageSplitTargets.erase(i);
				break;
			}
		}
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
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		uint32 m_UnderwaterMaxTimeSaved = static_cast< Player* >( m_target )->m_UnderwaterMaxTime;
		if( apply )
			static_cast< Player* >( m_target )->m_UnderwaterMaxTime *= (1 + mod->m_amount / 100 );
		else
			static_cast< Player* >( m_target )->m_UnderwaterMaxTime /= (1 + mod->m_amount / 100 );
                static_cast< Player* >( m_target )->m_UnderwaterTime *= static_cast< Player* >( m_target )->m_UnderwaterMaxTime / m_UnderwaterMaxTimeSaved;
	}
}

void Aura::SpellAuraSafeFall(bool apply)
{
	//TODO: FIXME: Find true flag for this
	if( !m_target->IsPlayer() )
		return;

	WorldPacket data( 12 );
	if( apply )
	{
		SetPositive();
		data.SetOpcode( SMSG_MOVE_FEATHER_FALL );
		( ( Player* )m_target )->m_safeFall += mod->m_amount;
	}
	else
	{
		data.SetOpcode(SMSG_MOVE_NORMAL_FALL);
		( ( Player* )m_target )->m_safeFall -= mod->m_amount;
	}

	data << m_target->GetNewGUID();
	data << uint32( 0 );
	static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
}

void Aura::SpellAuraModReputationAdjust(bool apply)
{
	/*SetPositive();
	bool updateclient = true;
	if(IsPassive())
		updateclient = false;	 // dont update client on passive

	if(m_target->GetTypeId()==TYPEID_PLAYER)
	{
		if(apply)
			  static_cast< Player* >( m_target )->modPercAllReputation(mod->m_amount, updateclient);
		  else
			static_cast< Player* >( m_targe t)->modPercAllReputation(-mod->m_amount, updateclient);
	}*/

	// This is _actually_ "Reputation gains increased by x%."
	// not increase all rep by x%.

	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		SetPositive();
		if(apply)
			static_cast< Player* >( m_target )->pctReputationMod += mod->m_amount;
		else
			static_cast< Player* >( m_target )->pctReputationMod -= mod->m_amount;
	}
}

void Aura::SpellAuraNoPVPCredit(bool apply)
{
	if(m_target->GetTypeId() != TYPEID_PLAYER)
		return;

	Player* pPlayer = static_cast< Player* >( m_target );

	if (apply)
		pPlayer->m_honorless++;
	else
		pPlayer->m_honorless--;
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

		uint32 field = UNIT_FIELD_POWER1 + misc;

		uint32 Amount = (uint32)min( amount, m_target->GetUInt32Value( field ) );
		uint32 newHealth = m_target->GetUInt32Value(field) - Amount ;

		SendPeriodicAuraLog(m_target, m_target, m_spellProto->Id, m_spellProto->School, newHealth, 0, 0, FLAG_PERIODIC_DAMAGE);
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
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			static_cast< Player* >( m_target )->m_modphyscritdmgPCT += (uint32)mod->m_amount;
		}
		else
		{
			static_cast< Player* >( m_target )->m_modphyscritdmgPCT -= (uint32)mod->m_amount;
		}
	}
}


void Aura::SpellAuraWaterBreathing( bool apply )
{
   if( m_target->GetTypeId() == TYPEID_PLAYER )
   {
	   if( apply )
	   {
			SetPositive();
			WorldPacket data( 4 );
			data.SetOpcode( SMSG_STOP_MIRROR_TIMER );
			data << uint32( 1 );
			static_cast< Player* >( m_target )->GetSession()->SendPacket( &data );
			static_cast< Player* >( m_target )->m_UnderwaterState = 0;
	   }

	   static_cast< Player* >( m_target )->m_bUnlimitedBreath = apply;
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
	if(m_target->IsPlayer())
	{
		if(apply)
		{
			m_target->ModFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,(float)mod->m_amount/100);
		}
		else
			m_target->ModFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,-(float)mod->m_amount/100);
		m_target->CalcDamage();
	}
}

void Aura::SpellAuraModRangedAttackPowerPct(bool apply)
{
    if(m_target->IsPlayer())
	{
		m_target->ModFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER,((apply)?1:-1)*(float)mod->m_amount/100);
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
					static_cast< Player* >( m_target )->IncreaseDamageByTypePCT[x+1] += ((float)(mod->m_amount))/100;;
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
					static_cast< Player* >( m_target )->IncreaseDamageByTypePCT[x+1] -= ((float)(mod->m_amount))/100;;
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
	if(m_target->GetTypeId() == TYPEID_PLAYER && m_target->isAlive() && m_target->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) == 0)
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
		//!! caster may log out before spell expires on target !
		Unit * pCaster = GetUnitCaster();
		if(!pCaster)
			return;

		val = mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();

		SM_FIValue(pCaster->SM_FEffectBonus,&val,m_spellProto->SpellGroupType);
		mod->fixed_amount[mod->i] = val; //we wish to have the same amount when we are removing the spell as when we were applying !
	}
	else
		val = -mod->fixed_amount[mod->i];

	uint32 stat = 3;
	for(uint32 i=0; i < 3; i++)
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
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x) )
			{
				if( apply )
				{
					mod->realamount = float2int32(((float)val/100)*m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat));
					m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, mod->realamount );
				}
				else
					m_target->ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, -mod->realamount );

				static_cast< Player* >( m_target )->SpellDmgDoneByAttribute[stat][x] += ((float)(val))/100;
			}
		}
		if(m_target->IsPlayer())
			static_cast< Player* >( m_target )->UpdateChanceFields();
	}
}

void Aura::SpellAuraIncreaseHealingByAttribute(bool apply)
{
	int32 val;

	if(apply)
	{
		//!! caster may log out before spell expires on target !
		Unit * pCaster = GetUnitCaster();
		if(!pCaster)
			return;

		val = mod->m_amount;
		if(val<0)
			SetNegative();
		else
			SetPositive();

		SM_FIValue(pCaster->SM_FEffectBonus,&val,m_spellProto->SpellGroupType);
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

	if(m_target->IsPlayer())
	{
		for(uint32 x=1;x<7;x++)
		{
			static_cast< Player* >( m_target )->SpellHealDoneByAttribute[stat][x] += ((float)(val))/100;
		}
		if(m_target->IsPlayer())
		{
			static_cast< Player* >( m_target )->UpdateChanceFields();
			if( apply )
			{
				mod->realamount = float2int32(((float)val/100)*m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat));
				m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, mod->realamount);
			}
			else
				m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, -mod->realamount);
		}
	}
}

void Aura::SpellAuraAddFlatModifier(bool apply)
{
	int32 val = apply?mod->m_amount:-mod->m_amount;
	uint64 AffectedGroups = (uint64)GetSpellProto()->EffectSpellGroupRelation[mod->i] + ((uint64)GetSpellProto()->EffectSpellGroupRelation_high[mod->i] << 32);
//printf("!!! the AffectedGroups %u ,the smt type %u,\n",AffectedGroups,mod->m_miscValue);

	switch (mod->m_miscValue)//let's generate warnings for unknown types of modifiers
	{
	case SMT_CRITICAL:
		SendModifierLog(&m_target->SM_CriticalChance,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_DURATION:
		SendModifierLog(&m_target->SM_FDur,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_RADIUS:
		SendModifierLog(&m_target->SM_FRadius,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_RANGE:
		SendModifierLog(&m_target->SM_FRange,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_CAST_TIME:
		SendModifierLog(&m_target->SM_FCastTime,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_COST:
		SendModifierLog(&m_target->SM_FCost,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_SPELL_VALUE_PCT:
		SendModifierLog(&m_target->SM_FDOT,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_ADDITIONAL_TARGET:
		SendModifierLog(&m_target->SM_FAdditionalTargets,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_DAMAGE_DONE:
		SendModifierLog(&m_target->SM_FDamageBonus,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_EFFECT_BONUS:
	case SMT_EFFECT:
		SendModifierLog(&m_target->SM_FEffectBonus,val,AffectedGroups,mod->m_miscValue,true);
		break;

	case SMT_SPELL_VALUE:
		if(GetSpellProto()->spellIconID==457) AffectedGroups=1;
		if(GetSpellProto()->spellIconID==86) AffectedGroups=256;
		SendModifierLog(&m_target->SM_FSPELL_VALUE,val,AffectedGroups,mod->m_miscValue);
		break;

	case SMT_HITCHANCE:
		SendModifierLog(&m_target->SM_FHitchance,val,AffectedGroups,mod->m_miscValue);
		break;

		// as far as I know its not yet used!!!
	case SMT_PENALTY:
//		SendModifierLog(&m_target->SM_FPenalty,val,AffectedGroups,mod->m_miscValue);
		//all values seem to be pct. Based of 8/8 spells
		SendModifierLog(&m_target->SM_PPenalty,val,AffectedGroups,mod->m_miscValue,true);
		break;

	case SMT_COOLDOWN_DECREASE:
		SendModifierLog(&m_target->SM_FCooldownTime, val, AffectedGroups,mod->m_miscValue);
		break;

	case SMT_TRIGGER:
		SendModifierLog(&m_target->SM_FChanceOfSuccess,val,AffectedGroups,mod->m_miscValue);
		break;

/*	case SMT_THREAT_REDUCED:
	case SMT_BLOCK:
	case SMT_TRIGGER:
	case SMT_TIME:*/
		break;
	case SMT_RESIST_DISPEL:
		SendModifierLog(&m_target->SM_FRezist_dispell, val, AffectedGroups,mod->m_miscValue);
		break;
	default://unknown Modifier type
		sLog.outError(
			"Unknown spell modifier type %u in spell %u.<<--report this line to the developer\n",
			mod->m_miscValue,GetSpellId());
		//don't add val, though we could formaly could do,but as we don't know what it is-> no sense
		break;
	}


}


void Aura::SpellAuraModHealingDone(bool apply)
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
		val = -mod->m_amount;

	for(uint32 x=0;x<7;x++)
	{
		if (mod->m_miscValue  & (((uint32)1)<<x) )
		{
			m_target->HealDoneMod[x] += val;
		}
	}
	if(m_target->IsPlayer())
	{
		static_cast< Player* >( m_target )->UpdateChanceFields();
		m_target->SetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, m_target->GetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS) + val);
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

	for(uint32 x=0;x<7;x++)
	{
		if (mod->m_miscValue  & (((uint32)1)<<x) )
		{
			m_target->HealDonePctMod[x] += val;
		}
	}
}

void Aura::SpellAuraEmphaty(bool apply)
{
	SetPositive();
	Unit * caster = GetUnitCaster();
	if(caster == 0 || !m_target || caster->GetTypeId() != TYPEID_PLAYER)
		return;

	// Show extra info about beast
	uint32 dynflags = m_target->GetUInt32Value(UNIT_DYNAMIC_FLAGS);
	if(apply)
		dynflags |= U_DYN_FLAG_PLAYER_INFO;

	m_target->BuildFieldUpdatePacket(static_cast< Player* >(caster), UNIT_DYNAMIC_FLAGS, dynflags);
}

void Aura::SpellAuraModOffhandDamagePCT(bool apply)
{
	//Used only by talents of rogue and warrior;passive,positive
	if(m_target->GetTypeId() == TYPEID_PLAYER)
	{
		if(apply)
		{
			SetPositive();
			static_cast< Player* >( m_target )->offhand_dmg_mod *= (100+mod->m_amount)/100.0f;
		}
		else
			static_cast< Player* >( m_target )->offhand_dmg_mod /= (100+mod->m_amount)/100.0f;

		m_target->CalcDamage();
	}
}

void Aura::SpellAuraModPenetration(bool apply) // armor penetration & spell penetration
{
	if( m_spellProto->NameHash == SPELL_HASH_SERRATED_BLADES )
	{
		if(!m_target->IsPlayer())
			return;

		Player *plr = static_cast< Player* >(m_target);
		if( apply )
		{
			if( m_spellProto->Id == 14171 )
				plr->PowerCostPctMod[0] += float( m_target->getLevel() * 2.67 );
			else if( m_spellProto->Id == 14172 )
				plr->PowerCostPctMod[0] += float( m_target->getLevel() * 5.43 );
			else if( m_spellProto->Id == 14173 )
				plr->PowerCostPctMod[0] += float( m_target->getLevel() * 8 );
		}
		else
		{
			if( m_spellProto->Id == 14171 )
				plr->PowerCostPctMod[0] -= float( m_target->getLevel() * 2.67 );
			else if( m_spellProto->Id == 14172 )
				plr->PowerCostPctMod[0] -= float( m_target->getLevel() * 5.43 );
			else if( m_spellProto->Id == 14173 )
				plr->PowerCostPctMod[0] -= float( m_target->getLevel() * 8 );
		}
		return;
	}
	if(apply)
	{
		if(mod->m_amount < 0)
			SetPositive();
		else
			SetNegative();

		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x))
				m_target->PowerCostPctMod[x] -= mod->m_amount;
		}

		if(mod->m_miscValue & 124 && m_target->IsPlayer())
			m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, mod->m_amount);
	}
	else
	{
		for(uint32 x=0;x<7;x++)
		{
			if (mod->m_miscValue & (((uint32)1)<<x))
				m_target->PowerCostPctMod[x] += mod->m_amount;
		}
		if(mod->m_miscValue & 124 && m_target->IsPlayer())
			m_target->ModUnsigned32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, -mod->m_amount);
	}
}

void Aura::SpellAuraIncreaseArmorByPctInt(bool apply)
{
	uint32 i_Int = m_target->GetUInt32Value(UNIT_FIELD_STAT3);

	int32 amt = float2int32(i_Int * ((float)mod->m_amount / 100.0f));
	amt *= (!apply) ? -1 : 1;

	for(uint32 x=0;x<7;x++)
	{
		if(mod->m_miscValue & (((uint32)1)<< x))
		{
			if(m_target->GetTypeId() == TYPEID_PLAYER)
			{
				static_cast< Player* >( m_target )->FlatResistanceModifierPos[x] += amt;
				static_cast< Player* >( m_target )->CalcResistance(x);
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
	if (!m_target->IsPlayer())
		return;
	if(apply)
		static_cast< Player* >( m_target )->m_resist_hit[0]+=mod->m_amount;
	else
		static_cast< Player* >( m_target )->m_resist_hit[0]-=mod->m_amount;
}

void Aura::SpellAuraReduceAttackerRHitChance(bool apply)
{
	if (!m_target->IsPlayer())
		return;
	if(apply)
		static_cast< Player* >( m_target )->m_resist_hit[1]+=mod->m_amount;
	else
		static_cast< Player* >( m_target )->m_resist_hit[1]-=mod->m_amount;
}

void Aura::SpellAuraReduceAttackerSHitChance(bool apply)
{
	if (!m_target->IsPlayer())
		return;
	if(apply)
		static_cast< Player* >( m_target )->m_resist_hit[2]-=mod->m_amount;
	else
		static_cast< Player* >( m_target )->m_resist_hit[2]+=mod->m_amount;
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
	m_target->ModFloatValue(UNIT_MOD_CAST_SPEED,pct_value);
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
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_SPEARS, mod->m_amount);
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
			static_cast< Player* >( m_target )->_ModifySkillBonus(SKILL_SPEARS, -mod->m_amount);
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
	if( apply )
		static_cast< Player* >( m_target )->ModifyBonuses( SPELL_HIT_RATING, mod->m_amount );
	else
		static_cast< Player* >( m_target )->ModifyBonuses( SPELL_HIT_RATING, -mod->m_amount );
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
	if(!m_target)
		return;

	signed int val;
	if(apply)
		val = mod->m_amount;
	else
		val = -mod->m_amount;

	for(uint32 x=1;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->CritMeleeDamageTakenPctMod[x] += val;
}

void Aura::SpellAuraReduceCritRangedAttackDmg(bool apply)
{
	if(!m_target)
		return;

	signed int val;
	if(apply)
		val = mod->m_amount;
	else
		val = -mod->m_amount;

	for(uint32 x=1;x<7;x++)
		if (mod->m_miscValue & (((uint32)1)<<x) )
			m_target->CritRangedDamageTakenPctMod[x] += val;
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
	for( uint32 x = 1; x < 24; x++ )//skip x=0
		if( ( ( ( uint32 )1 ) << x ) & mod->m_miscValue )
			plr->ModifyBonuses( 11 + x, v );

	//MELEE_CRITICAL_AVOIDANCE_RATING + RANGED_CRITICAL_AVOIDANCE_RATING + SPELL_CRITICAL_AVOIDANCE_RATING
	//comes only as combination of them  - ModifyBonuses() not adding them individually anyhow
	if( mod->m_miscValue & (0x0004000|0x0008000|0x0010000) )
			plr->ModifyBonuses( RESILIENCE_RATING, v );

	if( mod->m_miscValue & 1 )//weapon skill
	{
		std::map<uint32, uint32>::iterator i;
		for( uint32 y = 0; y < 20; y++ )
			if( m_spellProto->EquippedItemSubClass & ( ( ( uint32 )1 ) << y ) )
			{
					i = static_cast< Player* >( m_target )->m_wratings.find( y );
					if( i == static_cast< Player* >( m_target )->m_wratings.end() )//no prev
					{
						static_cast< Player* >( m_target )->m_wratings[y]=v;
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
	if(m_target->isDead())
		return;

	uint32 mana = m_target->GetUInt32Value(UNIT_FIELD_POWER1) + (m_target->GetUInt32Value(UNIT_FIELD_STAT0 + stat)*perc)/100;

	if(mana <= m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
		m_target->SetUInt32Value(UNIT_FIELD_POWER1, mana);
	else
		m_target->SetUInt32Value(UNIT_FIELD_POWER1, m_target->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
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
	if(apply)
	{
		SetPositive();
		mod->realamount = (mod->m_amount * m_target->GetUInt32Value(UNIT_FIELD_STAT0 + mod->m_miscValue))/100;
		for(uint32 x =1; x<7;x++)
		m_target->HealDoneMod[x]+=mod->realamount;
	}else
	{
		for(uint32 x =1; x<7;x++)
			m_target->HealDoneMod[x]-=mod->realamount;

	}
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
		m_target->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		SpellEntry * sorInfo = dbcSpell.LookupEntry(27792);
		if(!sorInfo) return;
		Spell * sor = new Spell(m_target, sorInfo, true, NULL);
		SpellCastTargets targets;
		targets.m_unitTarget = m_target->GetGUID();
		sor->prepare(&targets);
	}
	else
	{
		m_target->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
		m_target->RemoveAura(27792);
		m_target->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
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

	for(uint32 x=0;x<7;x++)
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

void Aura::SpellAuraIncreaseRangedAPStatPCT(bool apply)
{
	if(p_target)
	{
		if(apply)
		{
			if(mod->m_amount > 0)
				SetPositive();
			else
				SetNegative();

			p_target->m_rap_mod_pct += mod->m_amount;
		}
		else
			p_target->m_rap_mod_pct -= mod->m_amount;

		p_target->UpdateStats();
	}
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

void Aura::SpellAuraAxeSkillModifier(bool apply)
{
	if( p_target != NULL )
	{
		SetPositive();
		if( apply )
		{
			p_target->_ModifySkillBonus( SKILL_AXES, mod->m_amount );
			p_target->_ModifySkillBonus( SKILL_2H_AXES, mod->m_amount );
		}
		else
		{
			p_target->_ModifySkillBonus( SKILL_AXES, -mod->m_amount );
			p_target->_ModifySkillBonus( SKILL_2H_AXES, -mod->m_amount );
		}
		p_target->UpdateStats();
	}
}

void Aura::SpellAuraModPossessPet(bool apply)
{
	Unit *caster = GetUnitCaster();
	Player* pCaster;
	if( caster->IsPlayer() )
		pCaster = static_cast< Player* >( caster );
	else
		return;

	if( !m_target->IsPet() ||
		pCaster->GetSummon() != m_target )
		return;


	if(apply)
	{
		if( caster != NULL && caster->IsInWorld() )
		{
			pCaster->Possess( m_target );
			pCaster->SpeedCheatDelay( GetDuration() );
		}
	}
	else
	{
		if( caster != NULL && caster->IsInWorld() )
		{
			// Thats UGLY
			m_target->setAItoUse(true);
			m_target->m_redirectSpellPackets = 0;
//			pCaster->SpeedCheatReset();

			pCaster->m_noInterrupt--;
			pCaster->SetUInt64Value(PLAYER_FARSIGHT, 0);
			pCaster->SetUInt64Value(UNIT_FIELD_CHARM, 0);

			pCaster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
			m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);
			m_target->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, m_target->GetCharmTempVal());
			m_target->_setFaction();
			m_target->UpdateOppFactionSet();

			/* send "switch mover" packet */
			WorldPacket data(SMSG_DEATH_NOTIFY_OBSOLETE, 10);
			data << pCaster->GetNewGUID() << uint8(1);
			pCaster->GetSession()->SendPacket(&data);
		}
	}
}

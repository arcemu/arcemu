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

#include "StdAfx.h"

pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS] =
{
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_NULL - 0
	&Spell::SpellEffectInstantKill,				//SPELL_EFFECT_INSTAKILL - 1
	&Spell::SpellEffectSchoolDMG,				//SPELL_EFFECT_SCHOOL_DAMAGE - 2
	&Spell::SpellEffectDummy,					//SPELL_EFFECT_DUMMY - 3
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_PORTAL_TELEPORT - 4
	&Spell::SpellEffectTeleportUnits,			//SPELL_EFFECT_TELEPORT_UNITS - 5
	&Spell::SpellEffectApplyAura,				//SPELL_EFFECT_APPLY_AURA - 6
	&Spell::SpellEffectEnvironmentalDamage,		//SPELL_EFFECT_ENVIRONMENTAL_DAMAGE - 7
	&Spell::SpellEffectPowerDrain,				//SPELL_EFFECT_POWER_DRAIN - 8
	&Spell::SpellEffectHealthLeech,				//SPELL_EFFECT_HEALTH_LEECH - 9
	&Spell::SpellEffectHeal,					//SPELL_EFFECT_HEAL - 10
	&Spell::SpellEffectBind,					//SPELL_EFFECT_BIND - 11
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_PORTAL - 12
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_RITUAL_BASE - 13
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_RITUAL_SPECIALIZE - 14
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL - 15
	&Spell::SpellEffectQuestComplete,			//SPELL_EFFECT_QUEST_COMPLETE - 16
	&Spell::SpellEffectWeapondamageNoschool,	//SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL - 17
	&Spell::SpellEffectResurrect,				//SPELL_EFFECT_RESURRECT - 18
	&Spell::SpellEffectAddExtraAttacks,			//SPELL_EFFECT_ADD_EXTRA_ATTACKS - 19
	&Spell::SpellEffectDodge,					//SPELL_EFFECT_DODGE - 20
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_EVADE - 21
	&Spell::SpellEffectParry,					//SPELL_EFFECT_PARRY - 22
	&Spell::SpellEffectBlock,					//SPELL_EFFECT_BLOCK - 23
	&Spell::SpellEffectCreateItem,				//SPELL_EFFECT_CREATE_ITEM - 24
	&Spell::SpellEffectWeapon,					//SPELL_EFFECT_WEAPON - 25
	&Spell::SpellEffectDefense,					//SPELL_EFFECT_DEFENSE - 26
	&Spell::SpellEffectPersistentAA,			//SPELL_EFFECT_PERSISTENT_AREA_AURA - 27
	&Spell::SpellEffectSummon,					//SPELL_EFFECT_SUMMON - 28
	&Spell::SpellEffectLeap,					//SPELL_EFFECT_LEAP - 29
	&Spell::SpellEffectEnergize,				//SPELL_EFFECT_ENERGIZE - 30
	&Spell::SpellEffectWeaponDmgPerc,			//SPELL_EFFECT_WEAPON_PERCENT_DAMAGE - 31
	&Spell::SpellEffectTriggerMissile,			//SPELL_EFFECT_TRIGGER_MISSILE - 32
	&Spell::SpellEffectOpenLock,				//SPELL_EFFECT_OPEN_LOCK - 33
	&Spell::SpellEffectTransformItem,			//SPELL_EFFECT_TRANSFORM_ITEM - 34
	&Spell::SpellEffectApplyGroupAA,			//SPELL_EFFECT_APPLY_GROUP_AREA_AURA - 35
	&Spell::SpellEffectLearnSpell,				//SPELL_EFFECT_LEARN_SPELL - 36
	&Spell::SpellEffectSpellDefense,			//SPELL_EFFECT_SPELL_DEFENSE - 37
	&Spell::SpellEffectDispel,					//SPELL_EFFECT_DISPEL - 38
	&Spell::SpellEffectNULL,                    //SPELL_EFFECT_LANGUAGE - 39
	&Spell::SpellEffectDualWield,				//SPELL_EFFECT_DUAL_WIELD - 40
	&Spell::SpellEffectJumpTarget,				//SPELL_EFFECT_JUMP_TARGET - 41
	&Spell::SpellEffectJumpBehindTarget,		//SPELL_EFFECT_JUMP_BEHIND_TARGET - 42
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER - 43
	&Spell::SpellEffectSkillStep,				//SPELL_EFFECT_SKILL_STEP - 44
	&Spell::SpellEffectAddHonor,				//SPELL_ADD_HONOR - 45
	&Spell::SpellEffectSpawn,					//SPELL_EFFECT_SPAWN - 46
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_TRADE_SKILL - 47
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_STEALTH - 48
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_DETECT - 49
	&Spell::SpellEffectSummonObject,			//SPELL_EFFECT_SUMMON_OBJECT - 50
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_FORCE_CRITICAL_HIT - 51 NA
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_GUARANTEE_HIT - 52 NA
	&Spell::SpellEffectEnchantItem,				//SPELL_EFFECT_ENCHANT_ITEM - 53
	&Spell::SpellEffectEnchantItemTemporary,	//SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY - 54
	&Spell::SpellEffectTameCreature,			//SPELL_EFFECT_TAMECREATURE - 55
	&Spell::SpellEffectSummonPet,				//SPELL_EFFECT_SUMMON_PET	- 56
	&Spell::SpellEffectLearnPetSpell,			//SPELL_EFFECT_LEARN_PET_SPELL - 57
	&Spell::SpellEffectWeapondamage,			//SPELL_EFFECT_WEAPON_DAMAGE - 58
	&Spell::SpellEffectOpenLockItem,			//SPELL_EFFECT_OPEN_LOCK_ITEM - 59
	&Spell::SpellEffectProficiency,				//SPELL_EFFECT_PROFICIENCY - 60
	&Spell::SpellEffectSendEvent,				//SPELL_EFFECT_SEND_EVENT - 61
	&Spell::SpellEffectPowerBurn,				//SPELL_EFFECT_POWER_BURN - 62
	&Spell::SpellEffectThreat,					//SPELL_EFFECT_THREAT - 63
	&Spell::SpellEffectTriggerSpell,			//SPELL_EFFECT_TRIGGER_SPELL - 64
	&Spell::SpellEffectApplyRaidAA,				//SPELL_EFFECT_APPLY_RAID_AREA_AURA - 65
	&Spell::SpellEffectPowerFunnel,				//SPELL_EFFECT_POWER_FUNNEL - 66
	&Spell::SpellEffectHealMaxHealth,			//SPELL_EFFECT_HEAL_MAX_HEALTH - 67
	&Spell::SpellEffectInterruptCast,			//SPELL_EFFECT_INTERRUPT_CAST - 68
	&Spell::SpellEffectDistract,				//SPELL_EFFECT_DISTRACT - 69
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_PULL - 70
	&Spell::SpellEffectPickpocket,				//SPELL_EFFECT_PICKPOCKET - 71
	&Spell::SpellEffectAddFarsight,				//SPELL_EFFECT_ADD_FARSIGHT - 72
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_UNTRAIN_TALENTS? - 73
	&Spell::SpellEffectUseGlyph,				//SPELL_EFFECT_USE_GLYPH - 74
	&Spell::SpellEffectHealMechanical,			//SPELL_EFFECT_HEAL_MECHANICAL - 75
	&Spell::SpellEffectSummonObjectWild,		//SPELL_EFFECT_SUMMON_OBJECT_WILD - 76
	&Spell::SpellEffectScriptEffect,			//SPELL_EFFECT_SCRIPT_EFFECT - 77
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_ATTACK - 78
	&Spell::SpellEffectSanctuary,				//SPELL_EFFECT_SANCTUARY - 79
	&Spell::SpellEffectAddComboPoints,			//SPELL_EFFECT_ADD_COMBO_POINTS - 80
	&Spell::SpellEffectCreateHouse,				//SPELL_EFFECT_CREATE_HOUSE - 81
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_BIND_SIGHT - 82
	&Spell::SpellEffectDuel,					//SPELL_EFFECT_DUEL - 83
	&Spell::SpellEffectStuck,					//SPELL_EFFECT_STUCK - 84
	&Spell::SpellEffectSummonPlayer,			//SPELL_EFFECT_SUMMON_PLAYER - 85
	&Spell::SpellEffectActivateObject,			//SPELL_EFFECT_ACTIVATE_OBJECT - 86
	&Spell::SpellEffectBuildingDamage,				//SPELL_EFFECT_BUILDING_DAMAGE - 87
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_BUILDING_REPAIR - 88
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_BUILDING_SWITCH_STATE - 89
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_KILL_CREDIT_90 - 90
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_THREAT_ALL - 91 UNUSED
	&Spell::SpellEffectEnchantHeldItem,			//SPELL_EFFECT_ENCHANT_HELD_ITEM - 92
	&Spell::SpellEffectSetMirrorName,			//SPELL_EFFECT_SUMMON_PHANTASM - 93 OLD
	&Spell::SpellEffectSelfResurrect,			//SPELL_EFFECT_SELF_RESURRECT - 94
	&Spell::SpellEffectSkinning,				//SPELL_EFFECT_SKINNING - 95
	&Spell::SpellEffectCharge,					//SPELL_EFFECT_CHARGE - 96
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_SUMMON_MULTIPLE_TOTEMS - 97
	&Spell::SpellEffectKnockBack,				//SPELL_EFFECT_KNOCK_BACK - 98
	&Spell::SpellEffectDisenchant,				//SPELL_EFFECT_DISENCHANT - 99
	&Spell::SpellEffectInebriate,				//SPELL_EFFECT_INEBRIATE - 100
	&Spell::SpellEffectFeedPet,					//SPELL_EFFECT_FEED_PET - 101
	&Spell::SpellEffectDismissPet,				//SPELL_EFFECT_DISMISS_PET - 102
	&Spell::SpellEffectReputation,				//SPELL_EFFECT_REPUTATION - 103
	&Spell::SpellEffectSummonObjectSlot,		//SPELL_EFFECT_SUMMON_OBJECT_SLOT1 - 104
	&Spell::SpellEffectSummonObjectSlot,		//SPELL_EFFECT_SUMMON_OBJECT_SLOT2 - 105
	&Spell::SpellEffectSummonObjectSlot,		//SPELL_EFFECT_SUMMON_OBJECT_SLOT3 - 106
	&Spell::SpellEffectSummonObjectSlot,		//SPELL_EFFECT_SUMMON_OBJECT_SLOT4 - 107
	&Spell::SpellEffectDispelMechanic,			//SPELL_EFFECT_DISPEL_MECHANIC - 108
	&Spell::SpellEffectSummonDeadPet,			//SPELL_EFFECT_SUMMON_DEAD_PET - 109
	&Spell::SpellEffectDestroyAllTotems,		//SPELL_EFFECT_DESTROY_ALL_TOTEMS - 110
	&Spell::SpellEffectDurabilityDamage,        //SPELL_EFFECT_DURABILITY_DAMAGE - 111
	&Spell::SpellEffectNULL,					//not in 3.3.5a client - 112
	&Spell::SpellEffectResurrectNew,			//SPELL_EFFECT_RESURRECT_NEW - 113
	&Spell::SpellEffectAttackMe,				//SPELL_EFFECT_ATTACK_ME - 114
	&Spell::SpellEffectDurabilityDamagePCT,     //SPELL_EFFECT_DURABILITY_DAMAGE_PCT - 115
	&Spell::SpellEffectSkinPlayerCorpse,		//SPELL_EFFECT_SKIN_PLAYER_CORPSE - 116
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_SPIRIT_HEAL - 117//Not used
	&Spell::SpellEffectSkill,					//SPELL_EFFECT_SKILL - 118
	&Spell::SpellEffectApplyPetAA,				//SPELL_EFFECT_APPLY_PET_AURA - 119
	&Spell::SpellEffectNULL,					//SPELL_EFFECT_TELEPORT_GRAVEYARD - 120//Not used
	&Spell::SpellEffectDummyMelee,				//SPELL_EFFECT_DUMMYMELEE	- 121
	&Spell::SpellEffectNULL,					// unknown - 122 //not used
	&Spell::SpellEffectStartTaxi,				//SPELL_EFFECT_START_TAXI - 123 // http://www.wowhead.com/?spell=54575
	&Spell::SpellEffectPlayerPull,				//SPELL_EFFECT_PLAYER_PULL - 124 - http://thottbot.com/e2312
	&Spell::SpellEffectReduceThreatPercent,     //SPELL_EFFECT_REDUCE_THREAT_PERCENT - 125 // Reduce Threat by % //http://www.thottbot.com/?sp=32835
	&Spell::SpellEffectSpellSteal,				//SPELL_EFFECT_SPELL_STEAL - 126 // Steal Beneficial Buff (Magic) //http://www.thottbot.com/?sp=30449
	&Spell::SpellEffectProspecting,				// unknown - 127 // Search 5 ore of a base metal for precious gems.  This will destroy the ore in the process.
	&Spell::SpellEffectApplyFriendAA,			// Apply Aura friendly
	&Spell::SpellEffectApplyEnemyAA,			// Apply Aura enemy
	&Spell::SpellEffectRedirectThreat,			// unknown - 130 // http://www.thottbot.com/s34477
	&Spell::SpellEffectNULL,					// unknown - 131 // test spell
	&Spell::SpellEffectPlayMusic,				// Play Music - 132 // http://www.thottbot.com/s46852
	&Spell::SpellEffectForgetSpecialization,	//SPELL_EFFECT_FORGET_SPECIALIZATION - 133 // http://www.thottbot.com/s36441 // I think this is a gm/npc spell
	&Spell::SpellEffectKillCredit,				// Quest Credit (Player only, not party) - 134 // related to summoning objects and removing them, http://www.thottbot.com/s39161
	&Spell::SpellEffectNULL,					// Summon Pet: http://www.thottbot.com/s23498 - 135
	&Spell::SpellEffectRestoreHealthPct,		// Restore Health % - 136 // http://www.thottbot.com/s41542 and http://www.thottbot.com/s39703
	&Spell::SpellEffectRestorePowerPct,			// Restore Power % - 137 // http://www.thottbot.com/s41542
	&Spell::SpellEffectKnockBack2,				// knockback2 - 138 // related to superjump or even "*jump" spells http://www.thottbot.com/?e=Unknown%20138
	&Spell::SpellEffectClearQuest,				// Remove Quest - 139
	&Spell::SpellEffectTriggerSpell,			// triggers a spell from target back to caster - used at Malacrass f.e.
	&Spell::SpellEffectNULL,					// unknown - 141 // triggers spell, magic one,  (Mother spell) http://www.thottbot.com/s41065
	&Spell::SpellEffectTriggerSpellWithValue,	//SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE - 142 // triggers some kind of "Put spell on target" thing... (dono for sure) http://www.thottbot.com/s40872 and http://www.thottbot.com/s33076
	&Spell::SpellEffectApplyOwnerAA,			// Apply Aura on summon owner - 143 // Master -> demon effecting spell, http://www.thottbot.com/s25228 and http://www.thottbot.com/s35696
	&Spell::SpellEffectNULL,					// unknown - 144
	&Spell::SpellEffectNULL,					// unknown - 145
	&Spell::SpellEffectActivateRunes,			// Activate Rune - 146
	&Spell::SpellEffectNULL,					// Quest Fail - 147
	&Spell::SpellEffectNULL,					// unknown - 148
	&Spell::SpellEffectNULL,					// unknown - 149
	&Spell::SpellEffectNULL,					// unknown - 150
	&Spell::SpellEffectTriggerSpell,			// SPELL_EFFECT_TRIGGER_SPELL_2 - 151
	&Spell::SpellEffectNULL,					// Summon Refer-a-Friend - 152
	&Spell::SpellEffectCreatePet,				// Create tamed pet - 153
	&Spell::SpellEffectTeachTaxiPath,			// "Teach" a taxi path - 154
	&Spell::SpellEffectDualWield2H,				// DualWield2H (ex: Titan's Grip) - 155
	&Spell::SpellEffectEnchantItemPrismatic,	// Add a socket to an armor/a weapon - 156
	&Spell::SpellEffectCreateItem2,				//157 SPELL_EFFECT_CREATE_ITEM_2
	&Spell::SpellEffectMilling,					// Milling - 158
	&Spell::SpellEffectRenamePet,				// Allow Pet Rename - 159
	&Spell::SpellEffectNULL,					// unknown - 160
	&Spell::SpellEffectLearnSpec,				// Learn or unlearn a spec - 161
	&Spell::SpellEffectActivateSpec,			// Activate a spec - 162
};

const char* SpellEffectNames[TOTAL_SPELL_EFFECTS] =
{
	"NULL",
	"INSTANT_KILL",              //    1
	"SCHOOL_DAMAGE",             //    2
	"DUMMY",                     //    3
	"PORTAL_TELEPORT",           //    4
	"TELEPORT_UNITS",            //    5
	"APPLY_AURA",                //    6
	"ENVIRONMENTAL_DAMAGE",      //    7
	"POWER_DRAIN",               //    8
	"HEALTH_LEECH",              //    9
	"HEAL",                      //    10
	"BIND",                      //    11
	"PORTAL",                    //    12
	"RITUAL_BASE",               //    13
	"RITUAL_SPECIALIZE",         //    14
	"RITUAL_ACTIVATE_PORTAL",    //    15
	"QUEST_COMPLETE",            //    16
	"WEAPON_DAMAGE_NOSCHOOL",    //    17
	"RESURRECT",                 //    18
	"ADD_EXTRA_ATTACKS",         //    19
	"DODGE",                     //    20
	"EVADE",                     //    21
	"PARRY",                     //    22
	"BLOCK",                     //    23
	"CREATE_ITEM",               //    24
	"WEAPON",                    //    25
	"DEFENSE",                   //    26
	"PERSISTENT_AREA_AURA_GROUP",//    27
	"SUMMON",                    //    28
	"LEAP",                      //    29
	"ENERGIZE",                  //    30
	"WEAPON_PERCENT_DAMAGE",     //    31
	"TRIGGER_MISSILE",           //    32
	"OPEN_LOCK",                 //    33
	"TRANSFORM_ITEM",            //    34
	"APPLY_GROUP_AREA_AURA",     //    35
	"LEARN_SPELL",               //    36
	"SPELL_DEFENSE",             //    37
	"DISPEL",                    //    38
	"LANGUAGE",                  //    39
	"DUAL_WIELD",                //    40
	"LEAP_41",                   //    41
	"JUMP_BEHIND_TARGET",        //    42
	"TELEPORT_UNITS_FACE_CASTER",//    43
	"SKILL_STEP",                //    44
	"UNDEFINED_45",              //    45
	"SPAWN",                     //    46
	"TRADE_SKILL",               //    47
	"STEALTH",                   //    48
	"DETECT",                    //    49
	"SUMMON_OBJECT",             //    50
	"FORCE_CRITICAL_HIT",        //    51
	"GUARANTEE_HIT",             //    52
	"ENCHANT_ITEM",              //    53
	"ENCHANT_ITEM_TEMPORARY",    //    54
	"TAMECREATURE",              //    55
	"SUMMON_PET",                //    56
	"LEARN_PET_SPELL",           //    57
	"WEAPON_DAMAGE",             //    58
	"OPEN_LOCK_ITEM",            //    59
	"PROFICIENCY",               //    60
	"SEND_EVENT",                //    61
	"POWER_BURN",                //    62
	"THREAT",                    //    63
	"TRIGGER_SPELL",             //    64
	"APPLY_RAID_AREA_AURA",      //    65
	"POWER_FUNNEL",              //    66
	"HEAL_MAX_HEALTH",           //    67
	"INTERRUPT_CAST",            //    68
	"DISTRACT",                  //    69
	"PULL",                      //    70
	"PICKPOCKET",                //    71
	"ADD_FARSIGHT",              //    72
	"UNTRAIN_TALENTS",           //    73
	"USE_GLYPH",                 //    74
	"HEAL_MECHANICAL",           //    75
	"SUMMON_OBJECT_WILD",        //    76
	"SCRIPT_EFFECT",             //    77
	"ATTACK",                    //    78
	"SANCTUARY",                 //    79
	"ADD_COMBO_POINTS",          //    80
	"CREATE_HOUSE",              //    81
	"BIND_SIGHT",                //    82
	"DUEL",                      //    83
	"STUCK",                     //    84
	"SUMMON_PLAYER",             //    85
	"ACTIVATE_OBJECT",           //    86
	"BUILDING_DAMAGE",           //    87
	"BUILDING_REPAIR",           //    88
	"BUILDING_SWITCH_STATE",     //    89
	"KILL_CREDIT_90",               //    90
	"THREAT_ALL",                //    91
	"ENCHANT_HELD_ITEM",         //    92
	"SET_MIRROR_NAME",           //    93
	"SELF_RESURRECT",            //    94
	"SKINNING",                  //    95
	"CHARGE",                    //    96
	"SUMMON_MULTIPLE_TOTEMS",    //    97
	"KNOCK_BACK",                //    98
	"DISENCHANT",                //    99
	"INEBRIATE",                 //    100
	"FEED_PET",                  //    101
	"DISMISS_PET",               //    102
	"REPUTATION",                //    103
	"SUMMON_OBJECT_SLOT1",       //    104
	"SUMMON_OBJECT_SLOT2",       //    105
	"SUMMON_OBJECT_SLOT3",       //    106
	"SUMMON_OBJECT_SLOT4",       //    107
	"DISPEL_MECHANIC",           //    108
	"SUMMON_DEAD_PET",           //    109
	"DESTROY_ALL_TOTEMS",        //    110
	"DURABILITY_DAMAGE",         //    111
	"NONE",                      //    112
	"RESURRECT_FLAT",            //    113
	"ATTACK_ME",                 //    114
	"DURABILITY_DAMAGE_PCT",     //    115
	"SKIN_PLAYER_CORPSE",        //    116
	"SPIRIT_HEAL",               //    117
	"SKILL",                     //    118
	"APPLY_PET_AREA_AURA",       //    119
	"TELEPORT_GRAVEYARD",        //    120
	"DUMMYMELEE",                //    121
	"UNKNOWN1",                  //    122
	"START_TAXI",                //    123
	"PLAYER_PULL",               //    124
	"UNKNOWN4",                  //    125
	"UNKNOWN5",                  //    126
	"PROSPECTING",               //    127
	"APPLY_FRIEND_AREA_AURA",    //    128
	"APPLY_RAID_AREA_AURA",      //    129
	"UNKNOWN10",                 //    130
	"UNKNOWN11",                 //    131
	"PLAY_MUSIC",                //    132
	"FORGET_SPECIALIZATION",     //    133
	"KILL_CREDIT",               //    134
	"UNKNOWN15",                 //    135
	"UNKNOWN16",                 //    136
	"RESTORE_POWER_PCT",         //    137
	"KNOCKBACK2",                //    138
	"CLEAR_QUEST",               //    139
	"UNKNOWN20",                 //    140
	"UNKNOWN21",                 //    141
	"TRIGGER_SPELL_WITH_VALUE",  //    142
	"APPLY_OWNER_AREA_AURA",     //    143
	"UNKNOWN23",                 //    144
	"UNKNOWN24",                 //    145
	"ACTIVATE_RUNES",            //    146
	"QUEST_FAIL",                //    147
	"UNKNOWN26",                 //    148
	"UNKNOWN27",                 //    149
	"UNKNOWN28",                 //    150
	"SUMMON_TARGET",             //    151
	"UNKNOWN30",                 //    152
	"TAME_CREATURE",             //    153
	"UNKNOWN32",                 //    154
	"UNKNOWN33",                 //    155
	"UNKNOWN34",                 //    156
	"UNKNOWN35",                 //    157
	"MILLING",                   //    158
	"ALLOW_PET_RENAME",          //    159
	"UNKNOWN36",                 //    160
	"UNKNOWN37"                  //    161 //used by spell 63624(dual talents)
};

void Spell::ApplyAA(uint32 i)   // Apply Area Aura
{
	if(!unitTarget || !unitTarget->isAlive()) return;
	if(u_caster != unitTarget) return;

	Aura* pAura = NULL;
	std::map<uint64, Aura*>::iterator itr = m_pendingAuras.find(unitTarget->GetGUID());
	if(itr == m_pendingAuras.end())
	{
		pAura = sSpellFactoryMgr.NewAura(GetProto(), GetDuration(), m_caster, unitTarget);

		float r = GetRadius(i);

		uint32 eventtype = 0;

		switch(m_spellInfo->Effect[ i ])
		{
			case SPELL_EFFECT_APPLY_GROUP_AREA_AURA:
				eventtype = EVENT_GROUP_AREA_AURA_UPDATE;
				break;
			case SPELL_EFFECT_APPLY_RAID_AREA_AURA:
				eventtype = EVENT_RAID_AREA_AURA_UPDATE;
				break;
			case SPELL_EFFECT_APPLY_PET_AREA_AURA:
				eventtype = EVENT_PET_AREA_AURA_UPDATE;
				break;
			case SPELL_EFFECT_APPLY_FRIEND_AREA_AURA:
				eventtype = EVENT_FRIEND_AREA_AURA_UPDATE;
				break;
			case SPELL_EFFECT_APPLY_ENEMY_AREA_AURA:
				eventtype = EVENT_ENEMY_AREA_AURA_UPDATE;
				break;
			case SPELL_EFFECT_APPLY_OWNER_AREA_AURA:
				eventtype = EVENT_ENEMY_AREA_AURA_UPDATE;
				break;
		}

		if(!sEventMgr.HasEvent(pAura, eventtype))		/* only add it once */
			sEventMgr.AddEvent(pAura, &Aura::EventUpdateAA, r * r, eventtype, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		m_pendingAuras.insert(std::make_pair(unitTarget->GetGUID(), pAura));
		AddRef();
		sEventMgr.AddEvent(this, &Spell::HandleAddAura, unitTarget->GetGUID(), EVENT_SPELL_HIT, 100, 1, 0);
	}
	else
	{
		pAura = itr->second;
	}

	pAura->AddMod(GetProto()->EffectApplyAuraName[i], damage, GetProto()->EffectMiscValue[i], i);
}


void Spell::SpellEffectNULL(uint32 i)
{
	LOG_DEBUG("Unhandled spell effect %u in spell %u.", GetProto()->Effect[i], GetProto()->Id);
}

void Spell::SpellEffectInstantKill(uint32 i)
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	//Sacrifice: if spell caster has "void walker" pet, pet dies and spell caster gets a
	/*Sacrifices the Voidwalker, giving its owner a shield that will absorb
	305 damage for 30 sec. While the shield holds, spellcasting will not be \
	interrupted by damage.*/

	/*
	Demonic Sacrifice

	When activated, sacrifices your summoned demon to grant you an effect that lasts
	30 minutes. The effect is canceled if any Demon is summoned.
	Imp: Increases your Fire damage by 15%.
	Voidwalker: Restores 3% of total Health every 4 sec.
	Succubus: Increases your Shadow damage by 15%.
	Felhunter: Restores 2% of total Mana every 4 sec.

	When activated, sacrifices your summoned demon to grant you an effect that lasts $18789d.  The effect is canceled if any Demon is summoned.

	Imp: Increases your Fire damage by $18789s1%.

	Voidwalker: Restores $18790s1% of total Health every $18790t1 sec.

	Succubus: Increases your Shadow damage by $18791s1%.

	Felhunter: Restores $18792s1% of total Mana every $18792t1 sec.

	*/
	uint32 spellId = GetProto()->Id;

	switch(spellId)
	{
		case 48743:
			{
				// retarget? some one test this spell.
				return;
			}
			break;
		case 18788: //Demonic Sacrifice (508745)
			uint32 DemonicSacEffectSpellId = 0;
			switch(unitTarget->GetEntry())
			{
				case 416:
					DemonicSacEffectSpellId = 18789;
					break; //Imp
				case 417:
					DemonicSacEffectSpellId = 18792;
					break; //Felhunter
				case 1860:
					DemonicSacEffectSpellId = 18790;
					break; //VoidWalker
				case 1863:
					DemonicSacEffectSpellId = 18791;
					break; //Succubus
				case 17252:
					DemonicSacEffectSpellId = 35701;
					break; //felguard
			}
			if(DemonicSacEffectSpellId)
			{
				SpellEntry* se = dbcSpell.LookupEntryForced(DemonicSacEffectSpellId);
				if(se && u_caster)
					u_caster->CastSpell(u_caster, se, true);
			}
	}

	switch(GetProto()->NameHash)
	{
		case SPELL_HASH_SACRIFICE:
			{
				if(!u_caster || !u_caster->IsPet())
					return;

				//TO< Pet* >(u_caster)->Dismiss( true );

				SpellEntry* se = dbcSpell.LookupEntry(5);
				if(TO< Pet* >(u_caster)->GetPetOwner() == NULL)
					return;

				SpellCastTargets targets(u_caster->GetGUID());
				Spell* sp = sSpellFactoryMgr.NewSpell(TO< Pet* >(u_caster)->GetPetOwner(), se, true, 0);
				sp->prepare(&targets);
				return;
			}
			break;
		case SPELL_HASH_DEMONIC_SACRIFICE:
			{
				if(!p_caster || !unitTarget || !unitTarget->IsPet())
					return;

				//TO< Pet* >(unitTarget)->Dismiss( true );

				SpellEntry* se = dbcSpell.LookupEntry(5);

				SpellCastTargets targets(unitTarget->GetGUID());
				Spell* sp = sSpellFactoryMgr.NewSpell(p_caster, se, true, 0);
				sp->prepare(&targets);
				return;
			}
			break;

		default:
			{
				// moar cheaters
				if(!p_caster || (u_caster && u_caster->IsPet()))
					return;

				if(p_caster->GetSession()->GetPermissionCount() == 0)
					return;
			}
	}
	//instant kill effects don't have a log
	//m_caster->SpellNonMeleeDamageLog(unitTarget, GetProto()->Id, unitTarget->GetHealth(), true);
	// cebernic: the value of instant kill must be higher than normal health,cuz anti health regenerated.
	m_caster->DealDamage(unitTarget, unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH) << 1, 0, 0, 0);
}

void Spell::SpellEffectSchoolDMG(uint32 i) // dmg school
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	if(unitTarget->SchoolImmunityList[GetProto()->School])
	{
		SendCastResult(SPELL_FAILED_IMMUNE);
		return;
	}

	uint32 dmg = 0;
	bool static_damage = false;
	bool force_crit = false;

	if(GetProto()->EffectChainTarget[i])//chain
	{
		if(GetProto()->Id == 32445 || GetProto()->Id == 28883)
		{
			int32 reduce = (int32)(GetProto()->dmg_multiplier[i] * 100.0f);
			reduce -= 100;

			if(reduce && chaindamage)
			{
				if(GetProto()->SpellGroupType && u_caster)
				{
					SM_FIValue(u_caster->SM_PJumpReduce, &reduce, GetProto()->SpellGroupType);
				}
				chaindamage += ((GetProto()->EffectBasePoints[i] + 51) * reduce / 100);
			}
			else
			{
				chaindamage = damage;
			}
			dmg = chaindamage;
		}
		else
		{
			int32 reduce = (int32)(GetProto()->dmg_multiplier[i] * 100.0f);

			if(reduce && chaindamage)
			{
				if(GetProto()->SpellGroupType && u_caster)
				{
					SM_FIValue(u_caster->SM_PJumpReduce, &reduce, GetProto()->SpellGroupType);
				}
				chaindamage = chaindamage * reduce / 100;
			}
			else
			{
				chaindamage = damage;
			}
			dmg = chaindamage;
		}
	}
	else
	{
		dmg = damage;
		switch(GetProto()->NameHash)
		{
			case SPELL_HASH_METEOR_SLASH:
				{
					uint32 splitCount = 0;
					for(std::set<Object*>::iterator itr = u_caster->GetInRangeOppFactsSetBegin(); itr != u_caster->GetInRangeOppFactsSetEnd(); ++itr)
					{
						if((*itr)->isInFront(u_caster) && u_caster->CalcDistance((*itr)) <= 65)
							splitCount++;
					};

					if(splitCount > 1)
						dmg = dmg / splitCount;
				}
				break;
			case SPELL_HASH_PULSING_SHOCKWAVE: // loken Pulsing shockwave
				{
					float _distance = u_caster->CalcDistance(unitTarget);
					if(_distance >= 2.0f)
						dmg = static_cast<uint32>(dmg * _distance);
				}
				break;
			case SPELL_HASH_ICE_LANCE: // Ice Lance
				{
					// Deal triple damage to frozen targets or to those in Deep Freeze
					if(unitTarget->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN) || unitTarget->HasAura(44572))
						dmg *= 3;
					//	if (dmg>300)   //dirty bugfix.
					//		dmg = (int32)(damage >> 1);

				}
				break;
			case SPELL_HASH_INCINERATE:	// Incinerate -> Deals x-x extra damage if the target is affected by immolate
				{
					if(unitTarget->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_IMMOLATE))
					{
						// random extra damage
						uint32 extra_dmg = 111 + (GetProto()->RankNumber * 11) + RandomUInt(GetProto()->RankNumber * 11);
						dmg += extra_dmg;
					}
				}
				break;
			case SPELL_HASH_ARCANE_SHOT: //hunter - arcane shot
				{
					if(u_caster)
						dmg += float2int32(u_caster->GetRAP() * 0.15f);
					dmg = float2int32(dmg * (0.9f + RandomFloat(0.2f)));      // randomized damage
				}
				break;
			case SPELL_HASH_GORE: // boar/ravager: Gore (50% chance of double damage)
				{
					dmg *= Rand(50) ? 2 : 1;
				}
				break;
			case SPELL_HASH_THUNDER_CLAP: // Thunderclap
				{
					if(u_caster)
						dmg = (GetProto()->EffectBasePoints[0] + 1) + float2int32(u_caster->GetAP() * 0.12f);
				}
				break;
			case SPELL_HASH_INTERCEPT: // Warrior - Intercept
				{
					if(u_caster)
						dmg = float2int32(u_caster->GetAP() * 0.12f);
				}
				break;
			case SPELL_HASH_SHOCKWAVE:		// Shockwave
				{
					if(u_caster)
						dmg = u_caster->GetAP() * (GetProto()->EffectBasePoints[2] + 1) / 100;
				}
				break;
			case SPELL_HASH_CONCUSSION_BLOW:
				{
					//3.2.2
					//[Concussion Blow]: The damage done by this ability has been reduced by 50%,
					//but its threat generation will remain approximately the same.
					dmg = u_caster->GetAP() * (GetProto()->EffectBasePoints[2] + 1) / 100;
				}
				break;
			case SPELL_HASH_HEROIC_THROW:   // Heroic Throw
				{
					if(u_caster)
						dmg = u_caster->GetAP() / 2 + 12;
					// hardcoded value are faster I guess
					// GetProto()->EffectBasePoints[0]+1 == 12 future reference
				}
				break;
			case SPELL_HASH_BLOODTHIRST:	// Bloodthirst
				{
					dmg = u_caster->GetAP() * (GetProto()->EffectBasePoints[0] + 1) / 100;
				}
				break;
			case SPELL_HASH_SHIELD_OF_RIGHTEOUSNESS: // Shield of Righteousness - a bit like "shield slam", OK for both ranks
				{
					if(p_caster != NULL)
					{
						dmg += float2int32(1.30f * p_caster->GetUInt32Value(PLAYER_RATING_MODIFIER_BLOCK) + GetProto()->EffectBasePoints[0]);
					}
				}
				break;
			case SPELL_HASH_SHIELD_SLAM:	// Shield Slam - damage is increased by block value
				{
					if(p_caster != NULL)
					{
						Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
						if(it && it->GetProto()->InventoryType == INVTYPE_SHIELD)
						{
							float block_multiplier = (100.0f + p_caster->m_modblockabsorbvalue) / 100.0f;
							if(block_multiplier < 1.0f)block_multiplier = 1.0f;

							int32 blockable_damage = float2int32((it->GetProto()->Block + p_caster->m_modblockvaluefromspells + p_caster->GetUInt32Value(PLAYER_RATING_MODIFIER_BLOCK) + ((p_caster->GetStat(STAT_STRENGTH) / 2.0f) - 1.0f)) * block_multiplier);

							/*
								3.2.0:
							    The benefit from additional block value this ability gains is now subject
								to diminishing returns. Diminishing returns occur once block value exceeds
								30 times the player's level and caps the maximum damage benefit from shield
								block value at 34.5 times the player's level.
							*/
							int32 max_blockable_damage = static_cast< int32 >(p_caster->getLevel() * 34.5f);
							if(blockable_damage > max_blockable_damage)
							{
								blockable_damage = max_blockable_damage;
							}

							dmg += blockable_damage;

						}
					}
				}
				break;
			case SPELL_HASH_FIRE_STRIKE:
			case SPELL_HASH_LIGHTNING_STRIKE:
			case SPELL_HASH_MOLTEN_ARMOR:		// fire armor, is static damage
				static_damage = true;
				break;

			case SPELL_HASH_CONFLAGRATE:
				unitTarget->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_IMMOLATE);
				break;

			case SPELL_HASH_JUDGEMENT_OF_COMMAND:
				{
					if(!unitTarget->IsStunned())
						dmg = dmg >> 1;
				}
				break;

			case SPELL_HASH_EXORCISM:
				{
					if(p_caster != NULL)
					{
						uint32 sph = p_caster->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + 1);
						int32 ap = p_caster->GetAP();
						dmg += float2int32((0.15f * sph) + (0.15f * ap));
						if(unitTarget && unitTarget->IsCreature())
						{
							uint32 type = TO_CREATURE(unitTarget)->GetCreatureInfo()->Type;
							if(type == UNIT_TYPE_UNDEAD || type == UNIT_TYPE_DEMON)
								force_crit = true;
						}
					}
				}
				break;

			default:
				break;
		}

		switch(GetProto()->Id)
		{
			case 64422: // Sonic Screech, Auriaya encounter
			case 64688:
				{
					int splitCount = 0;
					for(std::set<Object*>::iterator itr = u_caster->GetInRangeOppFactsSetBegin(); itr != u_caster->GetInRangeOppFactsSetEnd(); ++itr)
					{
						if((*itr)->isInFront(u_caster))
							splitCount++;
					};
					if(splitCount > 1)
						dmg /= splitCount;
				}
				break;

			default:
				break;
		};
	}

	if(p_caster && !static_damage)   //this is wrong but with current spell coef system it has to be here...
	{
		switch(p_caster->getClass())
		{
			case WARRIOR:
			case ROGUE:
			case HUNTER:
			case DEATHKNIGHT:
				static_damage = true; //No spells from these classes benefit from spell damage. Prevents Arc hunters, frost DKs, etc.
				break;
			default:
				break;
		}
	}


	// check for no more damage left (chains)
	if(!dmg)
		dmg = GetProto()->EffectBasePoints[i];

	if(!dmg)
		return;

	if(GetProto()->speed > 0 && m_triggeredSpell == false)
	{
		m_caster->SpellNonMeleeDamageLog(unitTarget, GetProto()->Id, dmg, pSpellId == 0);
	}
	else
	{
		if(GetType() == SPELL_DMG_TYPE_MAGIC)
		{
			m_caster->SpellNonMeleeDamageLog(unitTarget, GetProto()->Id, dmg, !m_triggeredSpell, static_damage);
		}
		else
		{
			if(u_caster != NULL)
			{
				uint32 _type;
				if(GetType() == SPELL_DMG_TYPE_RANGED)
					_type = RANGED;
				else
				{
					if(GetProto()->AttributesExC & FLAGS4_TYPE_OFFHAND)
						_type =  OFFHAND;
					else
						_type = MELEE;
				}

				u_caster->Strike(unitTarget, _type, GetProto(), 0, 0, dmg, !m_triggeredSpell, true, force_crit);
			}
		}
	}
}

void Spell::SpellEffectDummy(uint32 i) // Dummy(Scripted events)
{
	if(sScriptMgr.CallScriptedDummySpell(m_spellInfo->Id, i, this))
		return;

	LOG_ERROR("Spell ID: %u ( %s ) has a dummy effect ( %u ) but no handler for it.", m_spellInfo->Id, m_spellInfo->Name, i);
}

void Spell::SpellEffectTeleportUnits(uint32 i)    // Teleport Units
{
	if(unitTarget == NULL || m_caster == NULL)
		return;

	uint32 spellId = GetProto()->Id;

	// [2010-11-7 18:30] <@dfighter> a.) teleport to bindpoint
	// [2010-11-7 18:30] <@dfighter> b.) teleport to somewhere in world
	// [2010-11-7 18:30] <@dfighter> c.) teleport behind the target
	// [2010-11-7 18:34] <@dfighter> d.) teleport to caster's position

	if(m_spellInfo->EffectCustomFlag == 0)
	{
		LOG_ERROR("Spell %u ( %s ) has a teleport effect, but has no teleport flag.", spellId, m_spellInfo->Name);
		return;
	}

	// Portals
	if(m_spellInfo->HasCustomFlagForEffect(i, TELEPORT_TO_COORDINATES))
	{
		TeleportCoords* TC = ::TeleportCoordStorage.LookupEntry(spellId);

		if(TC == NULL)
		{
			LOG_ERROR("Spell %u ( %s ) has a TELEPORT TO COORDINATES effect, but has no coordinates to teleport to. ", spellId, m_spellInfo->Name);
			return;
		}

		HandleTeleport(TC->x, TC->y, TC->z, TC->mapId, unitTarget);
		return;
	}

	// Hearthstone and co.
	if(m_spellInfo->HasCustomFlagForEffect(i, TELEPORT_TO_BINDPOINT))
	{
		if(unitTarget->IsPlayer())
		{
			Player* p = TO_PLAYER(unitTarget);

			HandleTeleport(p->GetBindPositionX(), p->GetBindPositionY(), p->GetBindPositionZ(), p->GetBindMapId(), p);
		}
		return;
	}

	// Summon
	if(m_spellInfo->HasCustomFlagForEffect(i, TELEPORT_TO_CASTER))
	{
		if(u_caster == NULL)
			return;

		HandleTeleport(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), m_caster->GetMapId(), unitTarget);
		return;
	}

	// Shadowstep for example
	if(m_spellInfo->HasCustomFlagForEffect(i, TELEPORT_BEHIND_TARGET))
	{
		if(p_caster == NULL)
			return;

		///////////////////////////////////////////////// Code taken from the Shadowstep dummy script /////////////////////////////////////////////////////////////////////

		/* this is rather tricky actually. we have to calculate the orientation of the creature/player, and then calculate a little bit of distance behind that. */
		float ang;

		if(unitTarget == m_caster)
		{
			/* try to get a selection */
			unitTarget = m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget);
			if((!unitTarget) || !isAttackable(p_caster, unitTarget, !(GetProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) || (unitTarget->CalcDistance(p_caster) > 28.0f))
			{
				return;
			}
		}

		if(unitTarget->IsCreature())
		{
			if(unitTarget->GetTargetGUID() != 0)
			{
				/* We're chasing a target. We have to calculate the angle to this target, this is our orientation. */
				ang = m_caster->calcAngle(m_caster->GetPositionX(), m_caster->GetPositionY(), unitTarget->GetPositionX(), unitTarget->GetPositionY());
				/* convert degree angle to radians */
				ang = ang * M_PI_FLOAT / 180.0f;
			}
			else
			{
				/* Our orientation has already been set. */
				ang = unitTarget->GetOrientation();
			}
		}
		else
		{
			/* Players orientation is sent in movement packets */
			ang = unitTarget->GetOrientation();
		}
		// avoid teleporting into the model on scaled models
		const static float shadowstep_distance = 1.6f * unitTarget->GetFloatValue(OBJECT_FIELD_SCALE_X);
		float new_x = unitTarget->GetPositionX() - (shadowstep_distance * cosf(ang));
		float new_y = unitTarget->GetPositionY() - (shadowstep_distance * sinf(ang));
		/* Send a movement packet to "charge" at this target. Similar to warrior charge. */
		p_caster->z_axisposition = 0.0f;
		p_caster->SafeTeleport(p_caster->GetMapId(), p_caster->GetInstanceID(), LocationVector(new_x, new_y, (unitTarget->GetPositionZ() + 0.1f), ang));


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		return;
	}

	// For those special teleport spells
	if(sScriptMgr.CallScriptedDummySpell(m_spellInfo->Id, i, this))
		return;

	LOG_ERROR("Unhandled Teleport effect %u for Spell %u ( %s ).", i, m_spellInfo->Id, m_spellInfo->Name);
}

void Spell::SpellEffectApplyAura(uint32 i)  // Apply Aura
{
	if(unitTarget == NULL)
		return;
	// can't apply stuns/fear/polymorph/root etc on boss
	if(unitTarget->IsCreature())
	{
		if(u_caster != NULL && (u_caster != unitTarget))
		{
			Creature* c = TO< Creature* >(unitTarget);
			/*
			Charm (Mind Control, enslave demon): 1
			Confuse (Blind etc): 2
			Fear: 4
			Root: 8
			Silence : 16
			Stun: 32
			Sheep: 64
			Banish: 128
			Sap: 256
			Frozen : 512
			Ensnared 1024
			Sleep 2048
			Taunt (aura): 4096
			Decrease Speed (Hamstring) (aura): 8192
			Spell Haste (Curse of Tongues) (aura): 16384
			Interrupt Cast: 32768
			Mod Healing % (Mortal Strike) (aura): 65536
			Total Stats % (Vindication) (aura): 131072
			*/

			//Spells with Mechanic also add other ugly auras, but if the main aura is the effect --> immune to whole spell
			if(c->GetProto()->modImmunities)
			{
				bool immune = false;
				if(m_spellInfo->MechanicsType)
				{
					switch(m_spellInfo->MechanicsType)
					{
						case MECHANIC_CHARMED:
							if(c->GetProto()->modImmunities & 1)
								immune = true;
							break;
						case MECHANIC_DISORIENTED:
							if(c->GetProto()->modImmunities & 2)
								immune = true;
							break;
						case MECHANIC_FLEEING:
							if(c->GetProto()->modImmunities & 4)
								immune = true;
							break;
						case MECHANIC_ROOTED:
							if(c->GetProto()->modImmunities & 8)
								immune = true;
							break;
						case MECHANIC_SILENCED:
							if(c->GetProto()->modImmunities & 16)
								immune = true;
							break;
						case MECHANIC_STUNNED:
							if(c->GetProto()->modImmunities & 32)
								immune = true;
							break;
						case MECHANIC_POLYMORPHED:
							if(c->GetProto()->modImmunities & 64)
								immune = true;
							break;
						case MECHANIC_BANISHED:
							if(c->GetProto()->modImmunities & 128)
								immune = true;
							break;
						case MECHANIC_SAPPED:
							if(c->GetProto()->modImmunities & 256)
								immune = true;
							break;
						case MECHANIC_FROZEN:
							if(c->GetProto()->modImmunities & 512)
								immune = true;
							break;
						case MECHANIC_ENSNARED:
							if(c->GetProto()->modImmunities & 1024)
								immune = true;
							break;
						case MECHANIC_ASLEEP:
							if(c->GetProto()->modImmunities & 2048)
								immune = true;
							break;
					}
				}
				if(!immune)
				{
					// Spells that do more than just one thing (damage and the effect) don't have a mechanic and we should only cancel the aura to be placed
					switch(m_spellInfo->EffectApplyAuraName[i])
					{
						case SPELL_AURA_MOD_CONFUSE:
							if(c->GetProto()->modImmunities & 2)
								immune = true;
							break;
						case SPELL_AURA_MOD_FEAR:
							if(c->GetProto()->modImmunities & 4)
								immune = true;
							break;
						case SPELL_AURA_MOD_TAUNT:
							if(c->GetProto()->modImmunities & 4096)
								immune = true;
							break;
						case SPELL_AURA_MOD_STUN:
							if(c->GetProto()->modImmunities & 32)
								immune = true;
							break;
						case SPELL_AURA_MOD_SILENCE:
							if((c->GetProto()->modImmunities & 32768) || (c->GetProto()->modImmunities & 16))
								immune = true;
							break;
						case SPELL_AURA_MOD_DECREASE_SPEED:
							if(c->GetProto()->modImmunities & 8192)
								immune = true;
							break;
						case SPELL_AURA_INCREASE_CASTING_TIME_PCT:
							if(c->GetProto()->modImmunities & 16384)
								immune = true;
							break;
						case SPELL_AURA_MOD_LANGUAGE: //hacky way to prefer that the COT icon is set to mob
							if(c->GetProto()->modImmunities & 16384)
								immune = true;
							break;
						case SPELL_AURA_MOD_HEALING_DONE_PERCENT:
							if(c->GetProto()->modImmunities & 65536)
								immune = true;
							break;
						case SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE:
							if(c->GetProto()->modImmunities & 131072)
								immune = true;
							break;
					}
				}
				if(immune)
					return;
			}
		}
	}

#ifdef GM_Z_DEBUG_DIRECTLY
	else
	{
		if(unitTarget->IsPlayer() && unitTarget->IsInWorld() && TO< Player* >(unitTarget)->GetSession() && TO< Player* >(unitTarget)->GetSession()->CanUseCommand('z'))
		{
			sChatHandler.BlueSystemMessage(TO< Player* >(unitTarget)->GetSession(), "[%sSystem%s] |rSpell::SpellEffectApplyAura: %s EffectApplyAuraName [%u] .", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE,
			                               i);
		}
	}
#endif

	// avoid map corruption.
	if(unitTarget->GetInstanceID() != m_caster->GetInstanceID())
		return;

	//check if we already have stronger aura
	Aura* pAura;

	std::map<uint64, Aura*>::iterator itr = m_pendingAuras.find(unitTarget->GetGUID());
	//if we do not make a check to see if the aura owner is the same as the caster then we will stack the 2 auras and they will not be visible client sided
	if(itr == m_pendingAuras.end())
	{
		if(GetProto()->NameHash == SPELL_HASH_BLOOD_FRENZY && ProcedOnSpell)  //Warrior's Blood Frenzy
			GetProto()->DurationIndex = ProcedOnSpell->DurationIndex;

		uint32 Duration = GetDuration();

		// Handle diminishing returns, if it should be resisted, it'll make duration 0 here.
		if(!(GetProto()->Attributes & ATTRIBUTES_PASSIVE)) // Passive
			::ApplyDiminishingReturnTimer(&Duration, unitTarget, GetProto());

		if(!Duration)
		{
			SendCastResult(SPELL_FAILED_IMMUNE);
			return;
		}

		if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
			pAura = sSpellFactoryMgr.NewAura(GetProto(), Duration, g_caster->m_summoner, unitTarget, m_triggeredSpell, i_caster);
		else
			pAura = sSpellFactoryMgr.NewAura(GetProto(), Duration, m_caster, unitTarget, m_triggeredSpell, i_caster);

		pAura->pSpellId = pSpellId; //this is required for triggered spells

		m_pendingAuras.insert(std::make_pair(unitTarget->GetGUID(), pAura));
		AddRef();
		sEventMgr.AddEvent(this, &Spell::HandleAddAura, unitTarget->GetGUID(), EVENT_SPELL_HIT, 100, 1, 0);
	}
	else
	{
		pAura = itr->second;
	}
	pAura->AddMod(GetProto()->EffectApplyAuraName[i], damage, GetProto()->EffectMiscValue[i], i);

}

void Spell::SpellEffectEnvironmentalDamage(uint32 i)
{
	if(!playerTarget) return;

	if(playerTarget->SchoolImmunityList[GetProto()->School])
	{
		SendCastResult(SPELL_FAILED_IMMUNE);
		return;
	}
	//this is GO, not unit
	m_caster->SpellNonMeleeDamageLog(playerTarget, GetProto()->Id, damage, pSpellId == 0);
	playerTarget->SendEnvironmentalDamageLog( playerTarget->GetGUID(), DAMAGE_FIRE, damage );
}

void Spell::SpellEffectPowerDrain(uint32 i)  // Power Drain
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 powerField = UNIT_FIELD_POWER1 + GetProto()->EffectMiscValue[i];
	uint32 curPower = unitTarget->GetUInt32Value(powerField);
	if(powerField == UNIT_FIELD_POWER1 && unitTarget->IsPlayer())
	{
		Player* mPlayer = TO< Player* >(unitTarget);
		if(mPlayer->IsInFeralForm())
			return;

		// Resilience - reduces the effect of mana drains by (CalcRating*2)%.
		damage = float2int32(damage * (1 - ((TO< Player* >(unitTarget)->CalcRating(PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE) * 2) / 100.0f)));
	}
	uint32 amt = damage + ((u_caster->GetDamageDoneMod(GetProto()->School) * 80) / 100);
	if(amt > curPower)
		amt = curPower;
	unitTarget->SetUInt32Value(powerField, curPower - amt);
	u_caster->Energize(u_caster, GetProto()->Id, amt, GetProto()->EffectMiscValue[i]);
}

void Spell::SpellEffectHealthLeech(uint32 i) // Health Leech
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 curHealth = unitTarget->GetHealth();
	uint32 amt = damage;
	if(amt > curHealth)
	{
		amt = curHealth;
	}
	m_caster->DealDamage(unitTarget, damage, 0, 0, GetProto()->Id);

	if(!u_caster)
		return;
	uint32 playerCurHealth = u_caster->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 playerMaxHealth = u_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);

	if(playerCurHealth + amt > playerMaxHealth)
	{
		u_caster->SetHealth(playerMaxHealth);
	}
	else
	{
		u_caster->ModHealth(amt);
	}
}

void Spell::SpellEffectHeal(uint32 i) // Heal
{
	if(p_caster != NULL)
	{
		// HACKY but with SM_FEffect2_bonus it doesnt work

		// Apply this only on targets, which have one of paladins auras
		if(unitTarget && (unitTarget->HasAurasWithNameHash(SPELL_HASH_DEVOTION_AURA) || unitTarget->HasAurasWithNameHash(SPELL_HASH_RETRIBUTION_AURA) ||
		                  unitTarget->HasAurasWithNameHash(SPELL_HASH_CONCENTRATION_AURA) || unitTarget->HasAurasWithNameHash(SPELL_HASH_CRUSADER_AURA) ||  unitTarget->HasAurasWithNameHash(SPELL_HASH_FIRE_RESISTANCE_AURA) ||
		                  unitTarget->HasAurasWithNameHash(SPELL_HASH_FROST_RESISTANCE_AURA) || unitTarget->HasAurasWithNameHash(SPELL_HASH_SHADOW_RESISTANCE_AURA)))
		{
			if(p_caster->HasSpell(20140))     // Improved Devotion Aura Rank 3
				damage = (int32)(damage * 1.06);
			else if(p_caster->HasSpell(20139))     // Improved Devotion Aura Rank 2
				damage = (int32)(damage * 1.04);
			else if(p_caster->HasSpell(20138))     // Improved Devotion Aura Rank 1
				damage = (int32)(damage * 1.02);
		}

		if(p_caster->HasSpell(54943) && p_caster->HasAura(20165))       // Glyph of Seal of Light
			damage = (int32)(damage * 1.05);
	}

	if(GetProto()->EffectChainTarget[i])//chain
	{
		if(!chaindamage)
		{
			chaindamage = damage;
			Heal((int32)chaindamage);
		}
		else
		{
			int32 reduce = GetProto()->EffectDieSides[i] + 1;
			if(GetProto()->SpellGroupType && u_caster)
			{
				SM_FIValue(u_caster->SM_PJumpReduce, &reduce, GetProto()->SpellGroupType);
			}
			chaindamage -= (reduce * chaindamage) / 100;
			Heal((int32)chaindamage);
		}
	}
	else
	{
		//yep, the usual special case. This one is shaman talent : Nature's guardian
		//health is below 30%, we have a mother spell to get value from
		switch(GetProto()->Id)
		{
			case 31616:
				{
					if(unitTarget && unitTarget->IsPlayer() && pSpellId && unitTarget->GetHealthPct() < 30)
					{
						//check for that 10 second cooldown
						SpellEntry* spellInfo = dbcSpell.LookupEntryForced(pSpellId);
						if(spellInfo)
						{
							//heal value is received by the level of current active talent :s
							//maybe we should use CalculateEffect(uint32 i) to gain SM benefits
							int32 value = 0;
							int32 basePoints = spellInfo->EffectBasePoints[i] + 1; //+(m_caster->getLevel()*basePointsPerLevel);
							int32 randomPoints = spellInfo->EffectDieSides[i];
							if(randomPoints <= 1)
								value = basePoints;
							else
								value = basePoints + rand() % randomPoints;
							//the value is in percent. Until now it's a fixed 10%
							Heal(unitTarget->GetMaxHealth()*value / 100);
						}
					}
				}
				break;
				//Bloodthirst
			case 23880:
				{
					if(unitTarget)
					{
						Heal(unitTarget->GetMaxHealth() / 100);
					}
				}
				break;

			case 34299: //Druid: Improved Leader of the PAck
				{
					if(!unitTarget->IsPlayer() || !unitTarget->isAlive())
						break;

					Player* mPlayer = TO< Player* >(unitTarget);
					if(!mPlayer->IsInFeralForm() || (
					            mPlayer->GetShapeShift() != FORM_CAT &&
					            mPlayer->GetShapeShift() != FORM_BEAR &&
					            mPlayer->GetShapeShift() != FORM_DIREBEAR))
						break;
					uint32 max = mPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
					uint32 val = float2int32(((mPlayer->FindAura(34300)) ? 0.04f : 0.02f) * max);
					if(val)
						mPlayer->Heal(mPlayer, 34299, (uint32)(val));
				}
				break;
			case 22845: // Druid: Frenzied Regeneration
				{
					if(!unitTarget->IsPlayer() || !unitTarget->isAlive())
						break;
					Player* mPlayer = TO< Player* >(unitTarget);
					if(!mPlayer->IsInFeralForm() ||
					        (mPlayer->GetShapeShift() != FORM_BEAR &&
					         mPlayer->GetShapeShift() != FORM_DIREBEAR))
						break;
					uint32 val = mPlayer->GetPower(POWER_TYPE_RAGE);
					if(val > 100)
						val = 100;
					uint32 HpPerPoint = float2int32((mPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.003f));   //0.3% of hp per point of rage
					uint32 heal = HpPerPoint * (val / 10); //1 point of rage = 0.3% of max hp
					mPlayer->ModPower(POWER_TYPE_RAGE, -1 * val);

					if(val)
						mPlayer->Heal(mPlayer, 22845, heal);
				}
				break;
			case 18562: //druid - swiftmend
				{
					if(unitTarget)
					{
						uint32 new_dmg = 0;
						//consume rejuvenetaion and regrowth
						Aura* taura = unitTarget->FindAuraByNameHash(SPELL_HASH_REGROWTH);    //Regrowth
						if(taura && taura->GetSpellProto())
						{
							uint32 amplitude = taura->GetSpellProto()->EffectAmplitude[1] / 1000;
							if(!amplitude)
								amplitude = 3;

							//our hapiness is that we did not store the aura mod amount so we have to recalc it
							Spell* spell = sSpellFactoryMgr.NewSpell(m_caster, taura->GetSpellProto(), false, NULL);
							uint32 healamount = spell->CalculateEffect(1, unitTarget);
							delete spell;
							spell = NULL;
							new_dmg = healamount * 18 / amplitude;

							unitTarget->RemoveAura(taura);

							//do not remove flag if we still can cast it again
							if(!unitTarget->HasAurasWithNameHash(SPELL_HASH_REJUVENATION))
							{
								unitTarget->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_REJUVENATE);
								sEventMgr.RemoveEvents(unitTarget, EVENT_REJUVENATION_FLAG_EXPIRE);
							}
						}
						else
						{
							taura = unitTarget->FindAuraByNameHash(SPELL_HASH_REJUVENATION);  //Rejuvenation
							if(taura  && taura->GetSpellProto())
							{
								uint32 amplitude = taura->GetSpellProto()->EffectAmplitude[0] / 1000;
								if(!amplitude) amplitude = 3;

								//our happiness is that we did not store the aura mod amount so we have to recalc it
								Spell* spell = sSpellFactoryMgr.NewSpell(m_caster, taura->GetSpellProto(), false, NULL);
								uint32 healamount = spell->CalculateEffect(0, unitTarget);
								delete spell;
								spell = NULL;
								new_dmg = healamount * 12 / amplitude;

								unitTarget->RemoveAura(taura);

								unitTarget->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_REJUVENATE);
								sEventMgr.RemoveEvents(unitTarget, EVENT_REJUVENATION_FLAG_EXPIRE);
							}
						}

						if(new_dmg > 0)
						{
							SpellEntry* spellInfo = dbcSpell.LookupEntry(18562);
							Spell* spell = sSpellFactoryMgr.NewSpell(unitTarget, spellInfo, true, NULL);
							spell->SetUnitTarget(unitTarget);
							spell->Heal((int32)new_dmg);
							delete spell;
						}
					}
				}
				break;
			default:
				Heal(damage);
				break;
		}
	}
}

void Spell::SpellEffectBind(uint32 i)
{
	if(!playerTarget || !playerTarget->isAlive() || !m_caster)
		return;

	WorldPacket data(45);
	uint32 areaid = playerTarget->GetZoneId();
	uint32 mapid = playerTarget->GetMapId();
	if(GetProto()->EffectMiscValue[i])
	{
		areaid = GetProto()->EffectMiscValue[i];
		AreaTable* at = dbcArea.LookupEntryForced(areaid);
		if(!at)
			return;
		mapid = at->mapId;
	}

	playerTarget->SetBindPoint(playerTarget->GetPositionX(), playerTarget->GetPositionY(), playerTarget->GetPositionZ(), mapid, areaid);

	data.Initialize(SMSG_BINDPOINTUPDATE);
	data << playerTarget->GetBindPositionX() << playerTarget->GetBindPositionY() << playerTarget->GetBindPositionZ() << playerTarget->GetBindMapId() << playerTarget->GetBindZoneId();
	playerTarget->GetSession()->SendPacket(&data);

	data.Initialize(SMSG_PLAYERBOUND);
	data << m_caster->GetGUID() << playerTarget->GetBindZoneId();
	playerTarget->GetSession()->SendPacket(&data);
}

void Spell::SpellEffectQuestComplete(uint32 i) // Quest Complete
{
	if(!p_caster) return;
	QuestLogEntry* en = p_caster->GetQuestLogForEntry(GetProto()->EffectMiscValue[i]);
	if(en)
	{
		en->Complete();
		en->UpdatePlayerFields();
		en->SendQuestComplete();
	}
}

//wand->
void Spell::SpellEffectWeapondamageNoschool(uint32 i) // Weapon damage + (no School)
{
	if(!unitTarget || !u_caster)
		return;

	u_caster->Strike(unitTarget, (GetType() == SPELL_DMG_TYPE_RANGED ? RANGED : MELEE), GetProto(), damage, 0, 0, false, true);
}

void Spell::SpellEffectResurrect(uint32 i) // Resurrect (Flat)
{
	if(!playerTarget)
	{
		if(!corpseTarget)
		{
			// unit resurrection handler
			if(unitTarget)
			{
				if(unitTarget->IsCreature() && unitTarget->IsPet() && unitTarget->IsDead())
				{
					uint32 hlth = ((uint32)GetProto()->EffectBasePoints[i] > unitTarget->GetMaxHealth()) ? unitTarget->GetMaxHealth() : (uint32)GetProto()->EffectBasePoints[i];
					uint32 mana = ((uint32)GetProto()->EffectBasePoints[i] > unitTarget->GetMaxPower(POWER_TYPE_MANA)) ? unitTarget->GetMaxPower(POWER_TYPE_MANA) : (uint32)GetProto()->EffectBasePoints[i];

					if(!unitTarget->IsPet())
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					else
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_PET_DELAYED_REMOVE);
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					unitTarget->SetHealth(hlth);
					unitTarget->SetPower(POWER_TYPE_MANA, mana);
					unitTarget->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
					unitTarget->setDeathState(ALIVE);
					TO< Creature* >(unitTarget)->UnTag();
					TO< Creature* >(unitTarget)->loot.gold = 0;
					TO< Creature* >(unitTarget)->loot.looters.clear();
					TO< Creature* >(unitTarget)->loot.items.clear();
					TO< Creature* >(unitTarget)->SetLimboState(false); // we can regenerate health now
				}
			}

			return;
		}
		playerTarget = objmgr.GetPlayer(GET_LOWGUID_PART(corpseTarget->GetOwner()));
		if(!playerTarget) return;
	}

	if(playerTarget->isAlive() || !playerTarget->IsInWorld())
		return;

	uint32 health = GetProto()->EffectBasePoints[i];
	uint32 mana = GetProto()->EffectMiscValue[i];

	playerTarget->m_resurrectHealth = health;
	playerTarget->m_resurrectMana = mana;

	SendResurrectRequest(playerTarget);
	playerTarget->SetMovement(MOVE_UNROOT, 1);
}

void Spell::SpellEffectAddExtraAttacks(uint32 i) // Add Extra Attacks
{
	if(!u_caster)
		return;
	u_caster->m_extraattacks += damage;
}

void Spell::SpellEffectDodge(uint32 i)
{
	//i think this actually enables the skill to be able to dodge melee+ranged attacks
	//value is static and sets value directly which will be modified by other factors
	//this is only basic value and will be overwritten elsewhere !!!
	//	if(unitTarget->IsPlayer())
	//		unitTarget->SetFloatValue(PLAYER_DODGE_PERCENTAGE,damage);
}

void Spell::SpellEffectParry(uint32 i)
{
	if(unitTarget)
		unitTarget->setcanparry(true);
}

void Spell::SpellEffectBlock(uint32 i)
{
	//i think this actually enables the skill to be able to block melee+ranged attacks
	//value is static and sets value directly which will be modified by other factors
	//	if(unitTarget->IsPlayer())
	//		unitTarget->SetFloatValue(PLAYER_BLOCK_PERCENTAGE,damage);
}

void Spell::SpellEffectCreateItem(uint32 i)
{
	uint32 spellid = m_spellInfo->Id;

	if(playerTarget == NULL)
	{
		LOG_ERROR("Spell %u ( %s ) has a create item effect but no player target!", spellid, m_spellInfo->Name);
		return;
	}


	uint32 itemid = m_spellInfo->EffectItemType[ i ];
	uint32 count = 0;
	uint32 basecount = m_spellInfo->EffectDieSides[ i ];
	uint32 difference = m_spellInfo->EffectBasePoints[ i ];

	if(itemid == 0)
	{
		LOG_ERROR("Spell %u ( %s ) has a create item effect but no itemid to add, Spell needs to be fixed!", spellid, m_spellInfo->Name);
		return;
	}

	ItemPrototype* m_itemProto = ItemPrototypeStorage.LookupEntry(itemid);
	if(m_itemProto == NULL)
	{
		LOG_ERROR("Spell %u ( %s ) has a create item effect but the itemid is invalid!", spellid, m_spellInfo->Name);
		return;
	}

	if(difference > basecount)
	{
		count = basecount + difference;

	}
	else
	{
		uint32 mincount = basecount - difference;
		uint32 maxcount = basecount + difference;
		uint32 variablecount  = maxcount - mincount;
		uint32 randcount = RandomUInt(variablecount);

		count = mincount + randcount;
	}

	uint32 countperlevel = static_cast< uint32 >(Arcemu::round(m_spellInfo->EffectRealPointsPerLevel[ i ]));

	if(countperlevel != 0)
	{
		uint32 leveldiff = m_spellInfo->maxLevel - m_spellInfo->baseLevel;
		uint32 countforlevel = leveldiff * countperlevel;

		count += countforlevel;
	}


	if(count <= 0)
	{
		LOG_ERROR("Spell %u ( %s ) has a create item effect but no item count to add, Spell needs to be fixed! Count overriden to 1.", spellid, m_spellInfo->Name);
		count = 1;
	}

	if(p_caster != NULL)
	{

		skilllinespell* skill = objmgr.GetSpellSkill(spellid);

		// potions learned by discovery variables
		uint32 cast_chance = 5;
		uint32 learn_spell = 0;

		// tailoring specializations get +1 cloth bonus
		switch(spellid)
		{

			case 36686: //Shadowcloth
				if(p_caster->HasSpell(26801)) count++;
				break;

			case 26751: // Primal Mooncloth
				if(p_caster->HasSpell(26798)) count++;
				break;

			case 31373: //Spellcloth
				if(p_caster->HasSpell(26797)) count++;
				break;
		}

		if((skill != NULL) && (skill->skilline == SKILL_ALCHEMY))
		{
			//Potion Master
			if(strstr(m_itemProto->Name1, "Potion"))
			{
				if(p_caster->HasSpell(28675))
					while(Rand(20) && (count < 5))
						count++;

				// Super Rejuvenation Potion
				cast_chance = 2;
				learn_spell = 28586;
			}

			//Elixir Master
			if(strstr(m_itemProto->Name1, "Elixir") || strstr(m_itemProto->Name1, "Flask"))
			{
				if(p_caster->HasSpell(28677))
					while(Rand(20) && (count < 5))
						count++;

				uint32 spList[] = { 28590, 28587, 28588, 28591, 28589 };
				cast_chance = 2;
				learn_spell = spList[ RandomUInt(4) ];
			}

			//Transmutation Master
			if(m_spellInfo->Category == 310)
			{

				//rate for primal might is lower than for anything else
				if(m_spellInfo->Id == 29688)
				{
					if(p_caster->HasSpell(28672))
						while(Rand(40) && (count < 5))
							count++;
				}
				else
				{
					if(p_caster->HasSpell(28672))
						while(Rand(20) && (count < 5))
							count++;
				}

				uint32 spList[] = { 28581, 28585, 28585, 28584, 28582, 28580 };
				cast_chance = 5;
				learn_spell = spList[ RandomUInt(5) ];
			}
		}

		if(!playerTarget->GetItemInterface()->AddItemById(itemid, count, 0))
		{
			SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			return;
		}

		if(p_caster != NULL)
		{

			//random discovery by crafter item id
			switch(itemid)
			{

				case 22845: //Major Arcane Protection Potion
					cast_chance = 20;
					learn_spell = 41458;
					break;

				case 22841: //Major Fire Protection Potion
					cast_chance = 20;
					learn_spell = 41500;
					break;

				case 22842: //Major Frost Protection Potion
					cast_chance = 20;
					learn_spell = 41501;
					break;

				case 22847: //Major Holy Protection Potion
					// there is none
					break;

				case 22844: //Major Nature Protection Potion
					cast_chance = 20;
					learn_spell = 41502;
					break;

				case 22846: //Major Shadow Protection Potion
					cast_chance = 20;
					learn_spell = 41503;
					break;
			}

			if((learn_spell != 0) && (p_caster->getLevel() > 60) && !p_caster->HasSpell(learn_spell) && Rand(cast_chance))
			{
				SpellEntry* dspellproto = dbcSpell.LookupEntryForced(learn_spell);

				if(dspellproto != NULL)
				{
					p_caster->BroadcastMessage("%sDISCOVERY! You discovered the %s !|r", MSG_COLOR_YELLOW, dspellproto->Name);
					p_caster->addSpell(learn_spell);
				}
				else
				{
					LOG_ERROR("Spell %u ( %s ) Effect %u tried to teach a non-existing Spell %u in %s:%u", spellid, m_spellInfo->Name, i, learn_spell, __FILE__, __LINE__);
				}
			}
		}

		if(skill != 0)
		{
			DetermineSkillUp(skill->skilline);

			uint32 discovered_recipe = 0;

			for(std::set<ProfessionDiscovery*>::iterator itr = objmgr.ProfessionDiscoveryTable.begin(); itr != objmgr.ProfessionDiscoveryTable.end(); itr++)
			{
				ProfessionDiscovery* pf = *itr;
				if(spellid == pf->SpellId && p_caster->_GetSkillLineCurrent(skill->skilline) >= pf->SkillValue && !p_caster->HasSpell(pf->SpellToDiscover) && Rand(pf->Chance))
				{
					discovered_recipe = pf->SpellToDiscover;
					break;
				}
			}

			// if something was discovered teach player that recipe and broadcast message
			if(discovered_recipe != 0)
			{
				SpellEntry* se = dbcSpell.LookupEntryForced(discovered_recipe);

				if(se != NULL)
				{
					p_caster->addSpell(discovered_recipe);

					WorldPacket* data;
					char msg[256];
					sprintf(msg, "%sDISCOVERY! %s has discovered how to create %s.|r", MSG_COLOR_GOLD, p_caster->GetName(), se->Name);
					data = sChatHandler.FillMessageData(CHAT_MSG_SYSTEM, LANG_UNIVERSAL,  msg, p_caster->GetGUID(), 0);
					p_caster->GetMapMgr()->SendChatMessageToCellPlayers(p_caster, data, 2, 1, LANG_UNIVERSAL, p_caster->GetSession());
					delete data;
				}
				else
				{
					LOG_ERROR("Spell %u ( %s ) Effect %u tried to teach a non-existing Spell %u in %s:%u", spellid, m_spellInfo->Name, i, learn_spell, __FILE__, __LINE__);
				}
			}
		}
	}
}

void Spell::SpellEffectWeapon(uint32 i)
{
	if(!playerTarget)
		return;

	uint32 skill = 0;
	uint32 spell = 0;

	switch(this->GetProto()->Id)
	{
		case 201:    // one-handed swords
			{
				skill = SKILL_SWORDS;
			}
			break;
		case 202:   // two-handed swords
			{
				skill = SKILL_2H_SWORDS;
			}
			break;
		case 203:   // Unarmed
			{
				skill = SKILL_UNARMED;
			}
			break;
		case 199:   // two-handed maces
			{
				skill = SKILL_2H_MACES;
			}
			break;
		case 198:   // one-handed maces
			{
				skill = SKILL_MACES;
			}
			break;
		case 197:   // two-handed axes
			{
				skill = SKILL_2H_AXES;
			}
			break;
		case 196:   // one-handed axes
			{
				skill = SKILL_AXES;
			}
			break;
		case 5011: // crossbows
			{
				skill = SKILL_CROSSBOWS;
				spell = SPELL_RANGED_GENERAL;
			}
			break;
		case 227:   // staves
			{
				skill = SKILL_STAVES;
			}
			break;
		case 1180:  // daggers
			{
				skill = SKILL_DAGGERS;
			}
			break;
		case 200:   // polearms
			{
				skill = SKILL_POLEARMS;
			}
			break;
		case 15590: // fist weapons
			{
				skill = SKILL_UNARMED;
			}
			break;
		case 264:   // bows
			{
				skill = SKILL_BOWS;
				spell = SPELL_RANGED_GENERAL;
			}
			break;
		case 266: // guns
			{
				skill = SKILL_GUNS;
				spell = SPELL_RANGED_GENERAL;
			}
			break;
		case 2567:  // thrown
			{
				skill = SKILL_THROWN;
			}
			break;
		case 5009:  // wands
			{
				skill = SKILL_WANDS;
			}
			break;
		default:
			{
				skill = 0;
				LOG_DEBUG("WARNING: Could not determine skill for spell id %d (SPELL_EFFECT_WEAPON)", this->GetProto()->Id);
			}
			break;
	}

	if(skill)
	{
		if(spell)
			playerTarget->addSpell(spell);

		// if we do not have the skill line
		if(!playerTarget->_HasSkillLine(skill))
		{
			playerTarget->_AddSkillLine(skill, 1, playerTarget->getLevel() * 5);
		}
		else // unhandled.... if we have the skill line
		{
		}
	}
}

void Spell::SpellEffectDefense(uint32 i)
{
	//i think this actually enables the skill to be able to use defense
	//value is static and sets value directly which will be modified by other factors
	//this is only basic value and will be overwritten elsewhere !!!
	//	if(unitTarget->IsPlayer())
	//		unitTarget->SetFloatValue(UNIT_FIELD_RESISTANCES,damage);
}

void Spell::SpellEffectPersistentAA(uint32 i) // Persistent Area Aura
{
	if(m_AreaAura || !m_caster->IsInWorld())
		return;
	//create only 1 dyn object
	uint32 dur = GetDuration();
	float r = GetRadius(i);

	//Note: this code seems to be useless
	//this must be only source point or dest point
	//this AREA aura it's applied on area
	//it can'be on unit or self or item or object
	//uncomment it if I'm wrong
	//We are thinking in general so it might be useful later DK

	// grep: this is a hack!
	// our shitty dynobj system doesn't support GO casters, so we gotta
	// kinda have 2 summoners for traps that apply AA.
	DynamicObject* dynObj = m_caster->GetMapMgr()->CreateDynamicObject();

	if(g_caster != NULL && g_caster->m_summoner && !unitTarget)
	{
		Unit* caster = g_caster->m_summoner;
		dynObj->Create(caster, this, g_caster->GetPositionX(), g_caster->GetPositionY(),
		               g_caster->GetPositionZ(), dur, r);
		m_AreaAura = true;
		return;
	}

	switch(m_targets.m_targetMask)
	{
		case TARGET_FLAG_SELF:
			{
				dynObj->Create(u_caster, this,	m_caster->GetPositionX(),
				               m_caster->GetPositionY(), m_caster->GetPositionZ(), dur, r);
			}
			break;
		case TARGET_FLAG_UNIT:
			{
				if(!unitTarget || !unitTarget->isAlive())
				{
					dynObj->Remove();
					return;
				}

				dynObj->Create(u_caster, this, unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(),
				               dur, r);
			}
			break;
		case TARGET_FLAG_OBJECT:
			{
				if(!unitTarget || !unitTarget->isAlive())
				{
					dynObj->Remove();
					return;
				}

				dynObj->Create(u_caster, this, unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(),
				               dur, r);
			}
			break;
		case TARGET_FLAG_SOURCE_LOCATION:
			{
				dynObj->Create(u_caster, this, m_targets.m_srcX,
				               m_targets.m_srcY, m_targets.m_srcZ, dur, r);
			}
			break;
		case TARGET_FLAG_DEST_LOCATION:
			{
				dynObj->Create(u_caster ? u_caster : g_caster->m_summoner, this,
				               m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, dur, r);
			}
			break;
		default:
			dynObj->Remove();
			return;
	}

	if(u_caster != NULL)
		if(GetProto()->ChannelInterruptFlags > 0)
		{
			u_caster->SetChannelSpellTargetGUID(dynObj->GetGUID());
			u_caster->SetChannelSpellId(GetProto()->Id);
		}

	m_AreaAura = true;
}

void Spell::SpellEffectSummon(uint32 i)
{
	uint32 summonpropid = m_spellInfo->EffectMiscValueB[ i ];

	SummonPropertiesEntry* spe = dbcSummonProperties.LookupEntry(summonpropid);
	if(spe == NULL)
	{
		LOG_ERROR("No SummonPropertiesEntry for Spell %u ( %s )", m_spellInfo->Id, m_spellInfo->Name);
		return;
	}

	uint32 entry = m_spellInfo->EffectMiscValue[ i ];

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
	CreatureProto* cp = CreatureProtoStorage.LookupEntry(entry);

	if((ci == NULL) || (cp == NULL))
	{
		LOG_ERROR("Spell %u ( %s ) tried to summon creature %u without database data", m_spellInfo->Id, m_spellInfo->Name, entry);
		return;
	}

	LocationVector v(0.0f, 0.0f, 0.0f, 0.0f);

	if((m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION) != 0)
		v = LocationVector(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ);
	else
		v = m_caster->GetPosition();

	// Ugly hack to make sure we always summon at least 1
	if(damage == 0)
		damage = 1;

	// Client adds these spells to the companion window, it's weird but then it happens anyways
	if(spe->Slot == 5)
	{
		SpellEffectSummonCompanion(i, spe, cp, v);
		return;
	}

	switch(spe->ControlType)
	{
		case SUMMON_CONTROL_TYPE_GUARDIAN:
			if(spe->ID == 121)
			{
				SpellEffectSummonTotem(i, spe, cp, v);
				return;
			}
			break;

		case SUMMON_CONTROL_TYPE_PET:
			SpellEffectSummonTemporaryPet(i, spe, cp, v);
			return;

		case SUMMON_CONTROL_TYPE_POSSESSED:
			SpellEffectSummonPossessed(i, spe, cp, v);
			return;

		case SUMMON_CONTROL_TYPE_VEHICLE:
			SpellEffectSummonVehicle( i, spe, cp, v );
			return;
	}

	switch(spe->Type)
	{
		case SUMMON_TYPE_NONE:
		case SUMMON_TYPE_CONSTRUCT:
		case SUMMON_TYPE_OPPONENT:

			if(spe->ControlType == SUMMON_CONTROL_TYPE_GUARDIAN)
				SpellEffectSummonGuardian(i, spe, cp, v);
			else
				SpellEffectSummonWild(i, spe, cp, v);

			return;

		case SUMMON_TYPE_PET:
			SpellEffectSummonTemporaryPet(i, spe, cp, v);
			return;

		case SUMMON_TYPE_GUARDIAN:
		case SUMMON_TYPE_MINION:
		case SUMMON_TYPE_RUNEBLADE:
			SpellEffectSummonGuardian(i, spe, cp, v);
			return;

		case SUMMON_TYPE_TOTEM:
			SpellEffectSummonTotem(i, spe, cp, v);
			return;

		case SUMMON_TYPE_COMPANION:
			// These are used as guardians in some quests
			if(spe->Slot == 6)
				SpellEffectSummonGuardian(i, spe, cp, v);
			else
				SpellEffectSummonCompanion(i, spe, cp, v);
			return;

		case SUMMON_TYPE_VEHICLE:
		case SUMMON_TYPE_MOUNT:
			SpellEffectSummonVehicle( i, spe, cp, v );
			return;

		case SUMMON_TYPE_LIGHTWELL:
			SpellEffectSummonGuardian(i, spe, cp, v);
			return;
	}

	LOG_ERROR("Unknown summon type in summon property %u in spell %u %s", summonpropid, m_spellInfo->Id, m_spellInfo->Name);
}

void Spell::SpellEffectSummonWild(uint32 i, SummonPropertiesEntry* spe, CreatureProto* proto, LocationVector & v)
{
	if(g_caster != NULL)
		u_caster = g_caster->m_summoner;

	if(u_caster == NULL)
		return;

	for(int32 j = 0; j < damage; j++)
	{
		float followangle = -1 * M_PI_FLOAT / 2 * j;
		v.x = v.x + (GetRadius(i) * (cosf(followangle + v.o)));
		v.y = v.y + (GetRadius(i) * (sinf(followangle + v.o)));

		Summon* s = u_caster->GetMapMgr()->CreateSummon(proto->Id, SUMMONTYPE_WILD);
		if(s == NULL)
			return;

		s->Load(proto, u_caster, v, m_spellInfo->Id, spe->Slot - 1);
		s->PushToWorld(m_caster->GetMapMgr());

		if((p_caster != NULL) && (spe->Slot != 0))
			p_caster->SendTotemCreated(spe->Slot - 1, s->GetGUID(), GetDuration(), m_spellInfo->Id);

		int32 duration = static_cast< int32 >(GetDuration());
		if(duration > 0)
			sEventMgr.AddEvent(TO< Object* >(s), &Object::Delete, EVENT_SUMMON_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Spell::SpellEffectSummonGuardian(uint32 i, SummonPropertiesEntry* spe, CreatureProto* proto, LocationVector & v)
{

	if(g_caster != NULL)
		u_caster = g_caster->m_summoner;

	if(u_caster == NULL)
		return;

	float angle_for_each_spawn = -M_PI_FLOAT * 2 / damage;

	for(int j = 0; j < damage; j++)
	{
		float followangle = angle_for_each_spawn * j;

		float x = 3 * (cosf(followangle + u_caster->GetOrientation()));
		float y = 3 * (sinf(followangle + u_caster->GetOrientation()));

		v.x += x;
		v.y += y;

		Summon* s = u_caster->GetMapMgr()->CreateSummon(proto->Id, SUMMONTYPE_GUARDIAN);
		if(s == NULL)
			return;

		s->Load(proto, u_caster, v, m_spellInfo->Id, spe->Slot - 1);
		s->GetAIInterface()->SetUnitToFollowAngle(followangle);
		s->PushToWorld(u_caster->GetMapMgr());

		if((p_caster != NULL) && (spe->Slot != 0))
			p_caster->SendTotemCreated(spe->Slot - 1, s->GetGUID(), GetDuration(), m_spellInfo->Id);

		// Lightwell
		if(spe->Type == SUMMON_TYPE_LIGHTWELL)
		{
			s->Root();
			s->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
		}

		int32 duration = static_cast< int32 >(GetDuration());
		if(duration > 0)
			sEventMgr.AddEvent(TO< Object* >(s), &Object::Delete, EVENT_SUMMON_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	}
}

void Spell::SpellEffectSummonTemporaryPet(uint32 i, SummonPropertiesEntry* spe, CreatureProto* proto, LocationVector & v)
{
	if(p_caster == NULL)
		return;

	p_caster->DismissActivePets();
	p_caster->RemoveFieldSummon();

	int32 count = 0;

	// Only Inferno uses this SummonProperty ID, and somehow it has the wrong count
	if(spe->ID == 711)
		count = 1;
	else
		count = damage;

	// We know for sure that this will suceed because we checked in Spell::SpellEffectSummon
	CreatureInfo* ci = CreatureNameStorage.LookupEntry(proto->Id);

	float angle_for_each_spawn = -M_PI_FLOAT * 2 / damage;

	for(int32 i = 0; i < count; i++)
	{
		float followangle = angle_for_each_spawn * i;

		float x = 3 * (cosf(followangle + u_caster->GetOrientation()));
		float y = 3 * (sinf(followangle + u_caster->GetOrientation()));

		v.x += x;
		v.y += y;

		Pet* pet = objmgr.CreatePet(proto->Id);

		if(!pet->CreateAsSummon(proto->Id, ci, NULL, p_caster, m_spellInfo, 1, GetDuration(), &v, false))
		{
			pet->DeleteMe();
			pet = NULL;
			break;
		}

		pet->GetAIInterface()->SetUnitToFollowAngle(followangle);
	}
}

void Spell::SpellEffectSummonTotem(uint32 i, SummonPropertiesEntry* spe, CreatureProto* proto, LocationVector & v)
{
	if(u_caster == NULL)
		return;

	v.x += (float(spe->Slot) - 1.5f);
	v.y += (float(spe->Slot) - 1.5f);

	float landh = u_caster->GetMapMgr()->GetLandHeight(v.x, v.y, v.z + 2);
	float landdiff = landh - v.z;

	if(fabs(landdiff) <= 15)
		v.z = landh;

	Summon* s = u_caster->GetMapMgr()->CreateSummon(proto->Id, SUMMONTYPE_TOTEM);
	if(s == NULL)
		return;
	s->Load(proto, u_caster, v, m_spellInfo->Id, spe->Slot - 1);
	s->SetMaxHealth(damage);
	s->SetHealth(damage);
	s->PushToWorld(u_caster->GetMapMgr());

	if(p_caster != NULL)
		p_caster->SendTotemCreated(spe->Slot - 1, s->GetGUID(), GetDuration(), m_spellInfo->Id);

	int32 duration = static_cast< int32 >(GetDuration());
	if(duration > 0)
		sEventMgr.AddEvent(TO< Object* >(s), &Object::Delete, EVENT_SUMMON_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		sEventMgr.AddEvent(TO< Object* >(s), &Object::Delete, EVENT_SUMMON_EXPIRE, 60 * 60 * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::SpellEffectSummonPossessed(uint32 i, SummonPropertiesEntry* spe, CreatureProto* proto, LocationVector & v)
{
	if(p_caster == NULL)
		return;

	p_caster->DismissActivePets();
	p_caster->RemoveFieldSummon();

	Summon* s = p_caster->GetMapMgr()->CreateSummon(proto->Id, SUMMONTYPE_POSSESSED);
	if(s == NULL)
		return;

	v.x += (3 * cos(M_PI_FLOAT / 2 + v.o));
	v.y += (3 * cos(M_PI_FLOAT / 2 + v.o));

	s->Load(proto, p_caster, v, m_spellInfo->Id, spe->Slot - 1);
	s->SetCreatedBySpell(m_spellInfo->Id);
	s->PushToWorld(p_caster->GetMapMgr());

	p_caster->Possess(s->GetGUID(), 1000);
}


void Spell::SpellEffectSummonCompanion(uint32 i, SummonPropertiesEntry* spe, CreatureProto* proto, LocationVector & v)
{
	if(u_caster == NULL)
		return;

	if(u_caster->GetSummonedCritterGUID() != 0)
	{
		Unit* critter = u_caster->GetMapMgr()->GetUnit(u_caster->GetSummonedCritterGUID());
		Creature* c = TO< Creature* >(critter);
		uint32 currententry = c->GetCreatureInfo()->Id;

		c->RemoveFromWorld(false, true);
		u_caster->SetSummonedCritterGUID(0);

		// Before WOTLK when you casted the companion summon spell the second time it removed the companion
		// Customized servers or old databases could still use this method
		if(proto->Id == currententry)
			return;
	}

	Summon* s = u_caster->GetMapMgr()->CreateSummon(proto->Id, SUMMONTYPE_COMPANION);
	if(s == NULL)
		return;

	s->Load(proto, u_caster, v, m_spellInfo->Id, spe->Slot - 1);
	s->SetCreatedBySpell(m_spellInfo->Id);
	s->GetAIInterface()->SetFollowDistance(GetRadius(i));
	s->PushToWorld(u_caster->GetMapMgr());
	u_caster->SetSummonedCritterGUID(s->GetGUID());
}

void Spell::SpellEffectSummonVehicle( uint32 i, SummonPropertiesEntry *spe, CreatureProto *proto, LocationVector &v ){
	if( u_caster == NULL )
		return;

	// If it has no vehicle id, then we can't really do anything with it as a vehicle :/
	if( ( proto->vehicleid == 0 ) && ( p_caster == NULL ) && ( !p_caster->GetSession()->HasGMPermissions() ) )
		return;

	Creature *c = u_caster->GetMapMgr()->CreateCreature( proto->Id );
	c->Load( proto,v.x, v.y, v.z, v.o );
	c->Phase( PHASE_SET, u_caster->GetPhase() );
	c->SetCreatedBySpell( m_spellInfo->Id );
	c->SetCreatedByGUID( u_caster->GetGUID() );
	c->SetSummonedByGUID( u_caster->GetGUID() );
	c->RemoveFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK );
	c->PushToWorld( u_caster->GetMapMgr() );

	// Need to delay this a bit since first the client needs to see the vehicle
	u_caster->EnterVehicle( c->GetGUID(), 1 * 1000 );
}

void Spell::SpellEffectLeap(uint32 i) // Leap
{
	if(unitTarget == NULL)
		return;
	float radius = GetRadius(i);
	unitTarget->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);

	NavMeshData* nav = CollideInterface.GetNavMesh(m_caster->GetMapId());

	if(nav != NULL)
	{
		float destx, desty, destz;
		unitTarget->GetPoint(unitTarget->GetOrientation(), radius, destx, desty, destz);
		if(playerTarget != NULL)
			playerTarget->SafeTeleport(playerTarget->GetMapId(), playerTarget->GetInstanceID(), LocationVector(destx, desty, destz, playerTarget->GetOrientation()));
		else if(unitTarget != NULL)
			unitTarget->GetAIInterface()->MoveTeleport(destx, desty, destz, unitTarget->GetOrientation());
	}
	else
	{
		if(playerTarget == NULL)  //let client handle this for players
			return;

		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
		data << playerTarget->GetNewGUID();
		data << getMSTime();
		data << cosf(playerTarget->GetOrientation()) << sinf(playerTarget->GetOrientation());
		data << radius;
		data << float(-10.0f);
		playerTarget->GetSession()->SendPacket(&data);
	}
}

void Spell::SpellEffectEnergize(uint32 i) // Energize
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 modEnergy = 0;
	switch(GetProto()->Id)
	{
		case 30824: // Shamanistic Rage
			modEnergy = damage * GetUnitTarget()->GetAP() / 100;
			break;
		case 31786: // Paladin - Spiritual Attunement
			if(ProcedOnSpell)
			{
				SpellEntry* motherspell = dbcSpell.LookupEntryForced(pSpellId);
				if(motherspell)
				{
					//heal amount from procspell (we only proceed on a heal spell)
					uint32 healamt = 0;
					if(ProcedOnSpell->Effect[0] == SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[0] == SPELL_EFFECT_SCRIPT_EFFECT)
						healamt = ProcedOnSpell->EffectBasePoints[0] + 1;
					else if(ProcedOnSpell->Effect[1] == SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[1] == SPELL_EFFECT_SCRIPT_EFFECT)
						healamt = ProcedOnSpell->EffectBasePoints[1] + 1;
					else if(ProcedOnSpell->Effect[2] == SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[2] == SPELL_EFFECT_SCRIPT_EFFECT)
						healamt = ProcedOnSpell->EffectBasePoints[2] + 1;
					modEnergy = (motherspell->EffectBasePoints[0] + 1) * (healamt) / 100;
				}
			}
			break;
		case 57669:
			{
				modEnergy = float2int32(0.01f * unitTarget->GetUInt32Value(UNIT_FIELD_BASE_MANA));
			}
			break;
		case 31930:
			{
				if(u_caster)
					modEnergy = float2int32(0.25f * unitTarget->GetUInt32Value(UNIT_FIELD_BASE_MANA));
			}
			break;
		case 2687: // Improved Bloodrage, dirty fix
			{
				modEnergy = damage;
				if(p_caster)
				{
					if(p_caster->mSpells.find(12818) != p_caster->mSpells.end())
						modEnergy += 110; //60
					if(p_caster->mSpells.find(12301) != p_caster->mSpells.end())
						modEnergy += 60; //30
				}
			}
			break;
		default:
			modEnergy = damage;
			break;
	}

	if(unitTarget->HasAura(17619))
	{
		modEnergy = uint32(modEnergy * 1.4f);
	}

	u_caster->Energize(unitTarget, GetProto()->Id, modEnergy, GetProto()->EffectMiscValue[i]);
}

void Spell::SpellEffectWeaponDmgPerc(uint32 i) // Weapon Percent damage
{
	if(!unitTarget || !u_caster) return;

	if(GetType() == SPELL_DMG_TYPE_MAGIC)
	{
		uint32 dmg = CalculateDamage(u_caster, unitTarget, MELEE, 0, GetProto()) * damage / 100;
		u_caster->SpellNonMeleeDamageLog(unitTarget, GetProto()->Id, dmg, false, false, false);
	}
	else
	{
		uint32 _type;
		if(GetType() == SPELL_DMG_TYPE_RANGED)
			_type = RANGED;
		else
		{
			if(GetProto()->AttributesExC & 0x1000000)
				_type =  OFFHAND;
			else
				_type = MELEE;
		}

		u_caster->Strike(unitTarget, _type, GetProto(), add_damage, damage, 0, false, true);

		if(GetProto()->NameHash == SPELL_HASH_FAN_OF_KNIVES && p_caster != NULL)   // rogue - fan of knives
		{
			Item* oit = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
			if(oit != NULL)
			{
				if(oit->GetDurability() != 0)
				{
					if(oit->GetProto()->Class == 2 && oit->GetProto()->SubClass == 15)   // daggers
						damage = 105; //causing 105% weapon damage with daggers
					else
						damage = GetProto()->EffectBasePoints[i] + 1;// and 70% weapon damage with all other weapons.

					u_caster->Strike(unitTarget, OFFHAND, GetProto(), add_damage, damage, 0, false, true);
				}
			}
		}
	}
}

void Spell::SpellEffectTriggerMissile(uint32 i) // Trigger Missile
{
	//Used by mortar team
	//Triggers area effect spell at destinatiom

	uint32 spellid = GetProto()->EffectTriggerSpell[i];
	if(spellid == 0)
	{
		LOG_ERROR("Spell %u ( %s ) has a trigger missle effect ( %u ) but no trigger spell ID. Spell needs fixing.", m_spellInfo->Id, m_spellInfo->Name, i);
		return;
	}

	SpellEntry* spInfo = dbcSpell.LookupEntryForced(spellid);
	if(spInfo == NULL)
	{
		LOG_ERROR("Spell %u ( %s ) has a trigger missle effect ( %u ) but has an invalid trigger spell ID. Spell needs fixing.", m_spellInfo->Id, m_spellInfo->Name, i);
		return;
	}

	// Cast the triggered spell on the destination location, spells like Freezing Arrow use it
	if((u_caster != NULL) && (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION))
	{
		u_caster->CastSpellAoF(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, spInfo, true);
		return;
	}

	float spellRadius = GetRadius(i);

	// TODO: Following should be / is probably in SpellTarget code
	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++)
	{
		if(!((*itr)->IsUnit()) || !TO< Unit* >((*itr))->isAlive())
			continue;
		Unit* t = TO< Unit* >((*itr));

		float r;
		float d = m_targets.m_destX - t->GetPositionX();
		r = d * d;
		d = m_targets.m_destY - t->GetPositionY();
		r += d * d;
		d = m_targets.m_destZ - t->GetPositionZ();
		r += d * d;

		if(sqrt(r) > spellRadius) continue;

		if(!isAttackable(m_caster, *itr))//Fix Me: only enemy targets?
			continue;

		Spell* sp = sSpellFactoryMgr.NewSpell(m_caster, spInfo, true, NULL);
		SpellCastTargets tgt;
		tgt.m_unitTarget = (*itr)->GetGUID();
		sp->prepare(&tgt);
	}
}

void Spell::SpellEffectOpenLock(uint32 i) // Open Lock
{
	if(!p_caster) return;

	uint8 loottype = 0;

	uint32 locktype = GetProto()->EffectMiscValue[i];
	switch(locktype)
	{
		case LOCKTYPE_PICKLOCK:
			{
				uint32 v = 0;
				uint32 lockskill = p_caster->_GetSkillLineCurrent(SKILL_LOCKPICKING);

				if(itemTarget)
				{
					if(!itemTarget->locked)
						return;

					Lock* lock = dbcLock.LookupEntryForced(itemTarget->GetProto()->LockId);
					if(!lock) return;
					for(int j = 0; j < LOCK_NUM_CASES; j++)
					{
						if(lock->locktype[j] == 2 && lock->minlockskill[j] && lockskill >= lock->minlockskill[j])
						{
							v = lock->minlockskill[j];
							itemTarget->locked = false;
							itemTarget->UnLock();
							DetermineSkillUp(SKILL_LOCKPICKING, v / 5);
							break;
						}
					}
				}
				else if(gameObjTarget)
				{
					GameObjectInfo* info = gameObjTarget->GetInfo();
					if(gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, 0) == 0)
						return;

					Lock* lock = dbcLock.LookupEntry(info->SpellFocus);
					if(lock == 0)
						return;

					for(int j = 0; j < LOCK_NUM_CASES; j++)
					{
						if(lock->locktype[j] == 2 && lock->minlockskill[j] && lockskill >= lock->minlockskill[j])
						{
							v = lock->minlockskill[j];
							gameObjTarget->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
							gameObjTarget->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
							//Add Fill GO loot here
							if(gameObjTarget->loot.items.size() == 0)
							{
								if(gameObjTarget->GetMapMgr() != NULL)
									lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  gameObjTarget->GetMapMgr()->iInstanceMode);
								else
									lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  0);


								DetermineSkillUp(SKILL_LOCKPICKING, v / 5); //to prevent free skill up
							}
							loottype = LOOT_CORPSE;
							//End of it
							break;
						}
					}
				}
			}
			break;
		case LOCKTYPE_HERBALISM:
			{
				if(!gameObjTarget) return;

				uint32 v = gameObjTarget->GetGOReqSkill();
				bool bAlreadyUsed = false;

				if(Rand(100.0f)) // 3% chance to fail//why?
				{
					if(TO< Player* >(m_caster)->_GetSkillLineCurrent(SKILL_HERBALISM) < v)
					{
						//SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
						return;
					}
					else
					{
						if(gameObjTarget->loot.items.size() == 0)
						{
							if(gameObjTarget->GetMapMgr() != NULL)
								lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  gameObjTarget->GetMapMgr()->iInstanceMode);
							else
								lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  0);
						}
						else
							bAlreadyUsed = true;
					}
					loottype = LOOT_SKINNING;
				}
				else
				{
					SendCastResult(SPELL_FAILED_TRY_AGAIN);
				}
				//Skill up
				if(!bAlreadyUsed) //Avoid cheats with opening/closing without taking the loot
					DetermineSkillUp(SKILL_HERBALISM, v / 5);
			}
			break;
		case LOCKTYPE_MINING:
			{
				if(!gameObjTarget) return;

				uint32 v = gameObjTarget->GetGOReqSkill();
				bool bAlreadyUsed = false;

				if(Rand(100.0f))     // 3% chance to fail//why?
				{
					if(TO< Player* >(m_caster)->_GetSkillLineCurrent(SKILL_MINING) < v)
					{
						//SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
						return;
					}
					else if(gameObjTarget->loot.items.size() == 0)
					{
						if(gameObjTarget->GetMapMgr() != NULL)
							lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  gameObjTarget->GetMapMgr()->iInstanceMode);
						else
							lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  0);
					}
					else
						bAlreadyUsed = true;

					loottype = LOOT_SKINNING;
				}
				else
				{
					SendCastResult(SPELL_FAILED_TRY_AGAIN);
				}
				//Skill up
				if(!bAlreadyUsed) //Avoid cheats with opening/closing without taking the loot
					DetermineSkillUp(SKILL_MINING, v / 5);
			}
			break;
		case LOCKTYPE_SLOW_OPEN: // used for BG go's
			{
				if(!gameObjTarget) return;
				if(p_caster && p_caster->m_bg)
					if(p_caster->m_bg->HookSlowLockOpen(gameObjTarget, p_caster, this))
						return;

				uint32 spellid = !gameObjTarget->GetInfo()->Unknown1 ? 23932 : gameObjTarget->GetInfo()->Unknown1;
				SpellEntry* en = dbcSpell.LookupEntry(spellid);
				Spell* sp = sSpellFactoryMgr.NewSpell(p_caster, en, true, NULL);
				SpellCastTargets tgt;
				tgt.m_unitTarget = gameObjTarget->GetGUID();
				sp->prepare(&tgt);
				return;
			}
			break;
		case LOCKTYPE_QUICK_CLOSE:
			{
				if(!gameObjTarget) return;
				gameObjTarget->EventCloseDoor();
			}
			break;

		case LOCKTYPE_QUICK_OPEN:
			if( gameObjTarget == NULL )
				return;

			if( ( p_caster != NULL ) && ( p_caster->m_bg != NULL ) )
				p_caster->m_bg->HookQuickLockOpen( gameObjTarget, p_caster, this );

			// there is no break here on purpose

		default://not profession
			{
				if(!gameObjTarget)
					return;

				CALL_GO_SCRIPT_EVENT(gameObjTarget, OnActivate)(p_caster);
				CALL_INSTANCE_SCRIPT_EVENT(gameObjTarget->GetMapMgr(), OnGameObjectActivate)(gameObjTarget, p_caster);

				if(sQuestMgr.OnActivateQuestGiver(gameObjTarget, p_caster))
					return;

				if(sQuestMgr.OnGameObjectActivate(p_caster, gameObjTarget))
				{
					p_caster->UpdateNearbyGameObjects();
					return;
				}

				if(gameObjTarget->loot.items.size() == 0)
				{
					if(gameObjTarget->GetMapMgr() != NULL)
						lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  gameObjTarget->GetMapMgr()->iInstanceMode);
					else
						lootmgr.FillGOLoot(&gameObjTarget->loot, gameObjTarget->GetInfo()->sound1,  0);
				}
				loottype = LOOT_CORPSE ;
			}
			break;
	};
	if(gameObjTarget && gameObjTarget->GetType() == GAMEOBJECT_TYPE_CHEST)
		TO< Player* >(m_caster)->SendLoot(gameObjTarget->GetGUID(), loottype, gameObjTarget->GetMapId());
}

void Spell::SpellEffectTransformItem(uint32 i)
{
	bool result;
	AddItemResult result2;

	if(!i_caster) return;
	uint32 itemid = GetProto()->EffectItemType[i];
	if(!itemid) return;

	//Save durability of the old item
	Player* owner = i_caster->GetOwner();
	uint32 dur = i_caster->GetDurability();

	result  = owner->GetItemInterface()->SafeFullRemoveItemByGuid(i_caster->GetGUID());
	if(!result)
	{
		//something went wrong if this happen, item doesn't exist, so it wasn't destroyed.
		return;
	}

	i_caster = NULL;

	Item* it = objmgr.CreateItem(itemid, owner);
	if(!it) return;

	it->SetDurability(dur);
	//additem

	//additem
	result2 = owner->GetItemInterface()->AddItemToFreeSlot(it);
	if(!result2) //should never get here
	{
		owner->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_BAG_FULL);
		it->DeleteMe();
	}
}

void Spell::SpellEffectApplyGroupAA(uint32 i)
{
	ApplyAA(i);
}

void Spell::SpellEffectLearnSpell(uint32 i) // Learn Spell
{
	if(playerTarget == 0 && unitTarget && unitTarget->IsPet()) // something's wrong with this logic here.
	{
		// bug in target map fill?
		//playerTarget = m_caster->GetMapMgr()->GetPlayer((uint32)m_targets.m_unitTarget);
		SpellEffectLearnPetSpell(i);
		return;
	}

	if(GetProto()->Id == 483 || GetProto()->Id == 55884)          // "Learning"
	{
		if(!i_caster || !p_caster) return;

		uint32 spellid = 0;
		for(int j = 0; j < 5; ++j)
		{
			if(i_caster->GetProto()->Spells[j].Trigger == LEARNING && i_caster->GetProto()->Spells[j].Id != 0)
			{
				spellid = i_caster->GetProto()->Spells[j].Id;
				break;
			}
		}

		if(!spellid || !dbcSpell.LookupEntryForced(spellid)) return;

		// learn me!
		p_caster->addSpell(spellid);

		// no normal handler
		return;
	}

	if(playerTarget)
	{
		/*if(u_caster && isHostile(playerTarget, u_caster))
		return;*/

		uint32 spellToLearn = GetProto()->EffectTriggerSpell[i];
		playerTarget->addSpell(spellToLearn);

		if(spellToLearn == 2575)   //hacky fix for mining from creatures
			playerTarget->addSpell(32606);

		if(spellToLearn == 2366)   //hacky fix for herbalism from creatures
			playerTarget->addSpell(32605);

		//smth is wrong here, first we add this spell to player then we may cast it on player...
		SpellEntry* spellinfo = dbcSpell.LookupEntry(spellToLearn);
		//remove specializations
		switch(spellinfo->Id)
		{
			case 26801: //Shadoweave Tailoring
				playerTarget->removeSpell(26798, false, false, 0); //Mooncloth Tailoring
				playerTarget->removeSpell(26797, false, false, 0); //Spellfire Tailoring
				break;
			case 26798: // Mooncloth Tailoring
				playerTarget->removeSpell(26801, false, false, 0); //Shadoweave Tailoring
				playerTarget->removeSpell(26797, false, false, 0); //Spellfire Tailoring
				break;
			case 26797: //Spellfire Tailoring
				playerTarget->removeSpell(26801, false, false, 0); //Shadoweave Tailoring
				playerTarget->removeSpell(26798, false, false, 0); //Mooncloth Tailoring
				break;
			case 10656: //Dragonscale Leatherworking
				playerTarget->removeSpell(10658, false, false, 0); //Elemental Leatherworking
				playerTarget->removeSpell(10660, false, false, 0); //Tribal Leatherworking
				break;
			case 10658: //Elemental Leatherworking
				playerTarget->removeSpell(10656, false, false, 0); //Dragonscale Leatherworking
				playerTarget->removeSpell(10660, false, false, 0); //Tribal Leatherworking
				break;
			case 10660: //Tribal Leatherworking
				playerTarget->removeSpell(10656, false, false, 0); //Dragonscale Leatherworking
				playerTarget->removeSpell(10658, false, false, 0); //Elemental Leatherworking
				break;
			case 28677: //Elixir Master
				playerTarget->removeSpell(28675, false, false, 0); //Potion Master
				playerTarget->removeSpell(28672, false, false, 0); //Transmutation Maste
				break;
			case 28675: //Potion Master
				playerTarget->removeSpell(28677, false, false, 0); //Elixir Master
				playerTarget->removeSpell(28672, false, false, 0); //Transmutation Maste
				break;
			case 28672: //Transmutation Master
				playerTarget->removeSpell(28675, false, false, 0); //Potion Master
				playerTarget->removeSpell(28677, false, false, 0); //Elixir Master
				break;
			case 20219: //Gnomish Engineer
				playerTarget->removeSpell(20222, false, false, 0); //Goblin Engineer
				break;
			case 20222: //Goblin Engineer
				playerTarget->removeSpell(20219, false, false, 0); //Gnomish Engineer
				break;
			case 9788: //Armorsmith
				playerTarget->removeSpell(9787, false, false, 0); //Weaponsmith
				playerTarget->removeSpell(17039, false, false, 0); //Master Swordsmith
				playerTarget->removeSpell(17040, false, false, 0); //Master Hammersmith
				playerTarget->removeSpell(17041, false, false, 0); //Master Axesmith
				break;
			case 9787: //Weaponsmith
				playerTarget->removeSpell(9788, false, false, 0); //Armorsmith
				break;
			case 17041: //Master Axesmith
				playerTarget->removeSpell(9788, false, false, 0); //Armorsmith
				playerTarget->removeSpell(17040, false, false, 0); //Master Hammersmith
				playerTarget->removeSpell(17039, false, false, 0); //Master Swordsmith
				break;
			case 17040: //Master Hammersmith
				playerTarget->removeSpell(9788, false, false, 0); //Armorsmith
				playerTarget->removeSpell(17039, false, false, 0); //Master Swordsmith
				playerTarget->removeSpell(17041, false, false, 0); //Master Axesmith
				break;
			case 17039: //Master Swordsmith
				playerTarget->removeSpell(9788, false, false, 0); //Armorsmith
				playerTarget->removeSpell(17040, false, false, 0); //Master Hammersmith
				playerTarget->removeSpell(17041, false, false, 0); //Master Axesmith
				break;
		}
		for(uint32 j = 0; j < 3; j++)
			if(spellinfo->Effect[j] == SPELL_EFFECT_WEAPON ||
			        spellinfo->Effect[j] == SPELL_EFFECT_PROFICIENCY ||
			        spellinfo->Effect[j] == SPELL_EFFECT_DUAL_WIELD)
			{
				Spell* sp = sSpellFactoryMgr.NewSpell(unitTarget, spellinfo, true, NULL);
				SpellCastTargets targets;
				targets.m_unitTarget = unitTarget->GetGUID();
				targets.m_targetMask = TARGET_FLAG_UNIT;
				sp->prepare(&targets);
				break;
			}
		return;
	}

	// if we got here... try via pet spells..
	SpellEffectLearnPetSpell(i);
}

void Spell::SpellEffectSpellDefense(uint32 i)
{
	//used to enable this ability. We use it all the time ...
}

void Spell::SpellEffectDispel(uint32 i) // Dispel
{
	if(u_caster == NULL || unitTarget == NULL)
		return;

	uint32 start, end;

	if(isAttackable(u_caster, unitTarget) || GetProto()->EffectMiscValue[i] == DISPEL_STEALTH)    // IsAttackable returns false for stealthed
	{
		start = MAX_POSITIVE_AURAS_EXTEDED_START;
		end = MAX_POSITIVE_AURAS_EXTEDED_END;
		if(unitTarget->SchoolImmunityList[GetProto()->School])
			return;
	}
	else
	{
		start = MAX_NEGATIVE_AURAS_EXTEDED_START;
		end = MAX_NEGATIVE_AURAS_EXTEDED_END;
	}

	Aura* aur;
	SpellEntry* aursp;
	std::list< uint32 > dispelledSpells;
	bool finish = false;

	for(uint32 x = start; x < end; x++)
		if(unitTarget->m_auras[x] != NULL)
		{
			bool AuraRemoved = false;
			aur = unitTarget->m_auras[x];
			aursp = aur->GetSpellProto();

			//Nothing can dispel resurrection sickness;
			if(!aur->IsPassive() && !(aursp->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
			{
				if(GetProto()->DispelType == DISPEL_ALL)
				{
					unitTarget->HandleProc(PROC_ON_PRE_DISPELL_AURA_VICTIM, u_caster , GetProto(), m_triggeredSpell, aursp->Id);

					dispelledSpells.push_back(aursp->Id);

					unitTarget->RemoveAura(aur);
					AuraRemoved = true;

					if(--damage <= 0)
						finish = true;
				}
				else if(aursp->DispelType == GetProto()->EffectMiscValue[i])
				{
					unitTarget->HandleProc(PROC_ON_PRE_DISPELL_AURA_VICTIM, u_caster , GetProto(), m_triggeredSpell, aursp->Id);

					dispelledSpells.push_back(aursp->Id);

					unitTarget->RemoveAllAuras(aursp->Id, aur->GetCasterGUID());
					AuraRemoved = true;

					if(--damage <= 0)
						finish = true;
				}

				if(AuraRemoved)
				{
					if(aursp->NameHash == SPELL_HASH_UNSTABLE_AFFLICTION)
					{
						SpellEntry* spellInfo = dbcSpell.LookupEntry(31117);
						Spell* spell = sSpellFactoryMgr.NewSpell(u_caster, spellInfo, true, NULL);
						spell->forced_basepoints[0] = (aursp->EffectBasePoints[0] + 1) * 9;   //damage effect
						spell->ProcedOnSpell = GetProto();
						spell->pSpellId = aursp->Id;
						SpellCastTargets targets;
						targets.m_unitTarget = u_caster->GetGUID();
						spell->prepare(&targets);
					}
					/*else if ( aur->GetSpellProto()->NameHash == SPELL_HASH_LIFEBLOOM )
					{
					Spell* spell= sSpellFactoryMgr.NewSpell(aur->GetCaster(), aur->GetSpellProto(), true, NULL);
					spell->SetUnitTarget( unitTarget );
					spell->Heal( aur->mod->m_amount );
					}*/
				}
			}
			if(finish)
				break;
		}

	// send spell dispell log packet
	if(!dispelledSpells.empty())
	{
		WorldPacket data(SMSG_SPELLDISPELLOG, 25 + dispelledSpells.size() * 5);
		data << m_caster->GetNewGUID();
		data << unitTarget->GetNewGUID();
		data << GetProto()->Id;
		data << uint8(0);				// unused
		data << uint32(dispelledSpells.size());
		for(std::list< uint32 >::iterator itr = dispelledSpells.begin(); itr != dispelledSpells.end(); ++itr)
		{
			data << uint32(*itr);		// dispelled spell id
			data << uint8(0);			// 0 = dispelled, else cleansed
		}

		m_caster->SendMessageToSet(&data, true);
	}
}

void Spell::SpellEffectLanguage(uint32 i)
{
	if(!playerTarget || !GetProto()->EffectMiscValue[i])
		return;

	uint32 skills[15][2] =
	{
		{ 0, 0 },
		{ SKILL_LANG_ORCISH, 669 },
		{ SKILL_LANG_DARNASSIAN, 671 },
		{ SKILL_LANG_TAURAHE, 670 },
		{ SKILL_LANG_DWARVEN, 672 },
		{ SKILL_LANG_COMMON, 668 },
		{ SKILL_LANG_DEMON_TONGUE, 815 },
		{ SKILL_LANG_TITAN, 816 },
		{ SKILL_LANG_THALASSIAN, 813 },
		{ SKILL_LANG_DRACONIC, 814 },
		{ 0, 0 },
		{ SKILL_LANG_GNOMISH, 7340 },
		{ SKILL_LANG_TROLL, 7341 },
		{ SKILL_LANG_GUTTERSPEAK, 17737 },
		{ SKILL_LANG_DRAENEI, 29932 },
	};

	if(skills[GetProto()->EffectMiscValue[i]][0])
	{
		playerTarget->_AddSkillLine(skills[GetProto()->EffectMiscValue[i]][0], 300, 300);
		playerTarget->addSpell(skills[GetProto()->EffectMiscValue[i]][1]);
	}
}

void Spell::SpellEffectDualWield(uint32 i)
{
	if(p_caster == NULL)
		return;

	if(!p_caster->_HasSkillLine(SKILL_DUAL_WIELD))
		p_caster->_AddSkillLine(SKILL_DUAL_WIELD, 1, 1);

	// Increase it by one
	//dual wield is 1/1 , it never increases it's not even displayed in skills tab
}

void Spell::SpellEffectSkillStep(uint32 i) // Skill Step
{
	Player* target;
	if(p_caster == NULL)
	{
		// Check targets
		if(m_targets.m_unitTarget)
		{
			target = objmgr.GetPlayer((uint32)m_targets.m_unitTarget);
			if(!target) return;
		}
		else return;
	}
	else
	{
		target = p_caster;
	}

	uint32 skill = GetProto()->EffectMiscValue[i];
	if(skill == 242)
		skill = SKILL_LOCKPICKING; // somehow for lockpicking misc is different than the skill :s

	skilllineentry* sk = dbcSkillLine.LookupEntryForced(skill);

	if(!sk) return;

	uint32 max = 1;
	switch(sk->type)
	{
		case SKILL_TYPE_PROFESSION:
		case SKILL_TYPE_SECONDARY:
			max = damage * 75;
			break;
		case SKILL_TYPE_WEAPON:
			max = 5 * target->getLevel();
			break;
		case SKILL_TYPE_CLASS:
		case SKILL_TYPE_ARMOR:
			if(skill == SKILL_LOCKPICKING) { max = damage * 75; }
			else { max = 1; }
			break;
		default: //u cant learn other types in game
			return;
	};

	if(target->_HasSkillLine(skill))
	{
		target->_ModifySkillMaximum(skill, max);
	}
	else
	{
		// Don't add skills to players logging in.
		/*if((GetProto()->Attributes & 64) && playerTarget->m_TeleportState == 1)
		return;*/

		if(sk->type == SKILL_TYPE_PROFESSION)
			target->ModPrimaryProfessionPoints(-1);

		if(skill == SKILL_RIDING)
			target->_AddSkillLine(skill, max, max);
		else
			target->_AddSkillLine(skill, 1, max);
	}
}

void Spell::SpellEffectAddHonor(uint32 i)
{
	if(!playerTarget) return;

	uint32 val = GetProto()->EffectBasePoints[i];

	if(GetProto()->AttributesExB & ATTRIBUTESEXB_UNK4) val /= 10;

	val += 1;

	HonorHandler::AddHonorPointsToPlayer(playerTarget, val);

	WorldPacket data(SMSG_PVP_CREDIT, 16);
	data << val;
	data << uint64(0);
	data << uint32(5);
	playerTarget->GetSession()->SendPacket(&data);
}

void Spell::SpellEffectSpawn(uint32 i)
{
	// this effect is mostly for custom teleporting
	switch(GetProto()->Id)
	{
		case 10418: // Arugal spawn-in spell , teleports it to 3 locations randomly sneaking players (bastard ;P)
			{
				//only Creature can cast this spell
				if(u_caster == NULL || p_caster != NULL)
					return;

				static float coord[3][3] = {{ -108.9034f, 2129.5678f, 144.9210f}, { -108.9034f, 2155.5678f, 155.678f}, { -77.9034f, 2155.5678f, 155.678f}};

				int j = rand() % 3;
				//u_caster->GetAIInterface()->SendMoveToPacket(coord[j][0],coord[j][1],coord[j][2],0.0f,0,u_caster->GetAIInterface()->getMoveFlags());
			}
	}
}

void Spell::SpellEffectSummonObject(uint32 i)
{
	if(!u_caster) return;

	uint32 entry = GetProto()->EffectMiscValue[i];

	uint32 mapid = u_caster->GetMapId();
	float px = u_caster->GetPositionX();
	float py = u_caster->GetPositionY();
	float pz = u_caster->GetPositionZ();
	float orient = m_caster->GetOrientation();
	float posx = 0, posy = 0, posz = 0;

	if(entry == GO_FISHING_BOBBER && p_caster)
	{
		float co = cos(orient);
		float si = sin(orient);
		MapMgr* map = m_caster->GetMapMgr();
		Spell* spell = u_caster->GetCurrentSpell();

		float r;
		for(r = 20; r > 10; r--)
		{
			posx = px + r * co;
			posy = py + r * si;
			uint32 liquidtype;
			map->GetLiquidInfo(posx, posy, pz + 2, posz, liquidtype);
			if(!(liquidtype & 1))//water
				continue;
			if(posz > map->GetLandHeight(posx, posy, pz + 2))  //water
				break;
		}

		posx = px + r * co;
		posy = py + r * si;

		// Todo / Fix me: This should be loaded / cached
		uint32 zone = p_caster->GetAreaID();
		if(zone == 0)   // If the player's area ID is 0, use the zone ID instead
			zone = p_caster->GetZoneId();

		uint32 minskill;
		FishingZoneEntry* fishentry = FishingZoneStorage.LookupEntry(zone);
		if(!fishentry)   // Check database if there is fishing area / zone information, if not, return
			return;

		// Todo / Fix me: Add fishskill to the calculations
		minskill = fishentry->MinSkill;
		spell->SendChannelStart(20000);   // 30 seconds
		/*spell->SendSpellStart();
		spell->SendCastResult(SPELL_CANCAST_OK);
		spell->SendSpellGo ();*/

		GameObject* go = u_caster->GetMapMgr()->CreateGameObject(GO_FISHING_BOBBER);

		go->CreateFromProto(GO_FISHING_BOBBER, mapid, posx, posy, posz, orient);
		go->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
		go->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		go->SetUInt64Value(OBJECT_FIELD_CREATED_BY, m_caster->GetGUID());
		u_caster->SetChannelSpellTargetGUID(go->GetGUID());
		go->Phase(PHASE_SET, u_caster->GetPhase());

		go->PushToWorld(m_caster->GetMapMgr());

		if(lootmgr.IsFishable(zone))     // Only set a 'splash' if there is any loot in this area / zone
		{
			uint32 seconds[] = { 0, 4, 10, 14 };
			uint32 rnd = RandomUInt(3);
			sEventMgr.AddEvent(go, &GameObject::FishHooked, TO< Player* >(m_caster), EVENT_GAMEOBJECT_FISH_HOOKED, seconds[ rnd ] * 1000, 1, 0);
		}
		sEventMgr.AddEvent(go, &GameObject::EndFishing, TO< Player* >(m_caster), false, EVENT_GAMEOBJECT_END_FISHING, 17 * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		p_caster->SetSummonedObject(go);
	}
	else
	{
		posx = px;
		posy = py;
		GameObjectInfo* goI = GameObjectNameStorage.LookupEntry(entry);
		if(!goI)
		{
			if(p_caster)
			{
				sChatHandler.BlueSystemMessage(p_caster->GetSession(), "non-existent gameobject %u tried to be created by SpellEffectSummonObject. Report to devs!", entry);
			}
			return;
		}
		if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION && m_targets.m_destX && m_targets.m_destY && m_targets.m_destZ)
		{
			posx = m_targets.m_destX;
			posy = m_targets.m_destY;
			pz = m_targets.m_destZ;
		}
		GameObject* go = m_caster->GetMapMgr()->CreateGameObject(entry);

		go->CreateFromProto(entry, mapid, posx, posy, pz, orient);
		go->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		go->SetUInt64Value(OBJECT_FIELD_CREATED_BY, m_caster->GetGUID());
		go->Phase(PHASE_SET, u_caster->GetPhase());
		go->PushToWorld(m_caster->GetMapMgr());
		sEventMgr.AddEvent(go, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		if(entry == 17032 && p_caster)   // this is a portal
		{
			// enable it for party only
			go->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
			//disable by default
			WorldPacket* pkt = go->BuildFieldUpdatePacket(GAMEOBJECT_BYTES_1, 1 << 24);
			SubGroup* pGroup = p_caster->GetGroup() ? p_caster->GetGroup()->GetSubGroup(p_caster->GetSubGroup()) : NULL;

			if(pGroup)
			{
				p_caster->GetGroup()->Lock();
				for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin(); itr != pGroup->GetGroupMembersEnd(); ++itr)
				{
					if((*itr)->m_loggedInPlayer && m_caster != (*itr)->m_loggedInPlayer)
						(*itr)->m_loggedInPlayer->GetSession()->SendPacket(pkt);
				}
				p_caster->GetGroup()->Unlock();
			}
			delete pkt;
		}
		else if(entry == 36727 || entry == 177193 || entry == 194108)    // Portal of Summoning and portal of doom
		{
			if(!p_caster) return;

			//Player * pTarget = p_caster->GetMapMgr()->GetPlayer( p_caster->GetSelection() );
			Player* pTarget = objmgr.GetPlayer((uint32)p_caster->GetSelection());
			if(!pTarget || !pTarget->IsInWorld())
				return;
			go->m_ritualmembers[0] = p_caster->GetLowGUID();
			go->m_ritualcaster = p_caster->GetLowGUID();
			go->m_ritualtarget = pTarget->GetLowGUID();
			go->m_ritualspell = static_cast<uint16>(GetProto()->Id);
		}
		else if(entry == 186811 || entry == 181622)    // ritual of refreshment, ritual of souls
		{
			if(!p_caster) return;

			go->m_ritualmembers[0] = p_caster->GetLowGUID();
			go->m_ritualcaster = p_caster->GetLowGUID();
			go->m_ritualtarget = 0;
			go->m_ritualspell = static_cast<uint16>(GetProto()->Id);
		}
		else if(entry == 186812 || entry == 181621)    // Refreshment Table, Soulwell
		{
			go->charges = goI->sound1;
		}
		else//Lightwell,if there is some other type -- add it
		{
			go->charges = 5;//Max 5 charges
		}
		if(p_caster)
			p_caster->SetSummonedObject(go);//p_caster
	}
}

void Spell::SpellEffectEnchantItem(uint32 i) // Enchant Item Permanent
{
	if(!itemTarget || !p_caster)
		return;

	// Vellums
	if(GetProto()->EffectItemType[i] && (itemTarget->GetEntry() == 39349 ||
	                                     itemTarget->GetEntry() == 39350 || itemTarget->GetEntry() == 43146 ||
	                                     itemTarget->GetEntry() == 38682 || itemTarget->GetEntry() == 37602 ||
	                                     itemTarget->GetEntry() == 43145))
	{
		uint32 itemid = GetProto()->EffectItemType[i];
		ItemPrototype* it = ItemPrototypeStorage.LookupEntry(itemid);
		if(it == NULL)
		{
			p_caster->GetSession()->SystemMessage("Item is missing, report this to devs. Entry: %u", itemid);
			return;
		}

		Item* pItem = objmgr.CreateItem(itemid, p_caster);
		if(pItem == NULL)
			return;

		p_caster->GetItemInterface()->RemoveItemAmt(itemTarget->GetEntry(), 1);
		p_caster->GetItemInterface()->AddItemToFreeSlot(pItem);

		return;
	}

	EnchantEntry* Enchantment = dbcEnchant.LookupEntryForced(GetProto()->EffectMiscValue[i]);

	if(!Enchantment)
	{
		LOG_ERROR("Invalid enchantment entry %u for Spell %u", GetProto()->EffectMiscValue[ i ], GetProto()->Id);
		return;
	}

	if(p_caster->GetSession()->GetPermissionCount() > 0)
		sGMLog.writefromsession(p_caster->GetSession(), "enchanted item for %s", itemTarget->GetOwner()->GetName());

	//remove other perm enchantment that was enchanted by profession
	itemTarget->RemoveProfessionEnchant();
	int32 Slot = itemTarget->AddEnchantment(Enchantment, 0, true, true, false, 0);
	if(Slot < 0)
		return; // Apply failed

	if(!i_caster)
		DetermineSkillUp();
}

void Spell::SpellEffectEnchantItemTemporary(uint32 i)  // Enchant Item Temporary
{
	if((itemTarget == NULL) || (p_caster == NULL))
		return;

	uint32 Duration = m_spellInfo->EffectBasePoints[ i ];
	uint32 EnchantmentID = m_spellInfo->EffectMiscValue[ i ];

	// don't allow temporary enchants unless we're the owner of the item
	if(itemTarget->GetOwner() != p_caster)
		return;

	if(Duration == 0)
	{
		LOG_ERROR("Spell %u ( %s ) has no enchantment duration. Spell needs to be fixed!", m_spellInfo->Id, m_spellInfo->Name);
		return;
	}

	if(EnchantmentID == 0)
	{
		LOG_ERROR("Spell %u ( %s ) has no enchantment ID. Spell needs to be fixed!", m_spellInfo->Id, m_spellInfo->Name);
		return;
	}

	EnchantEntry* Enchantment = dbcEnchant.LookupEntryForced(EnchantmentID);
	if(Enchantment == NULL)
	{
		LOG_ERROR("Invalid enchantment entry %u for Spell %u", EnchantmentID, GetProto()->Id);
		return;
	}

	itemTarget->RemoveEnchantment(TEMP_ENCHANTMENT_SLOT);

	int32 Slot = itemTarget->AddEnchantment(Enchantment, Duration, false, true, false, TEMP_ENCHANTMENT_SLOT);
	if(Slot < 0)
		return; // Apply failed

	skilllinespell* skill = objmgr.GetSpellSkill(GetProto()->Id);
	if(skill != NULL)
		DetermineSkillUp(skill->skilline, itemTarget->GetProto()->ItemLevel);
}

void Spell::SpellEffectTameCreature(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->IsCreature())
		return;
	Creature* tame = TO_CREATURE(unitTarget);

	// Remove target
	tame->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, p_caster, 0);
	Pet* pPet = objmgr.CreatePet(tame->GetEntry());
	if(! pPet->CreateAsSummon(tame->GetEntry(), tame->GetCreatureInfo(), tame, p_caster, NULL, 2, 0))
	{
		pPet->DeleteMe();//CreateAsSummon() returns false if an error occurred.
		pPet = NULL;
	}
	tame->Despawn(0, tame->GetProto()->RespawnTime);
}

void Spell::SpellEffectSummonPet(uint32 i) //summon - pet
{
	if(!p_caster) return;

	if(GetProto()->Id == 883)  // "Call Pet" spell
	{
		if(p_caster->GetSummon())
		{
			SendCastResult(SPELL_FAILED_ALREADY_HAVE_SUMMON);
			return;
		}

		uint32 petno = p_caster->GetUnstabledPetNumber();
		if(petno)
		{
			if(p_caster->GetPlayerPet(petno)->alive)
			{
				p_caster->SpawnPet(petno);
			}
			else
			{
				SendTameFailure(PETTAME_DEAD);
			}
		}
		else
		{
			SendCastResult(SPELL_FAILED_NO_PET);
		}
		return;
	}

	//uint32 entryId = GetProto()->EffectMiscValue[i];

	//VoidWalker:torment, sacrifice, suffering, consume shadows
	//Succubus:lash of pain, soothing kiss, seduce , lesser invisibility
	//felhunter:	 Devour Magic,Paranoia,Spell Lock,	Tainted Blood

	// remove old pet
	Pet* old = TO< Player* >(m_caster)->GetSummon();
	if(old)
		old->Dismiss();

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(GetProto()->EffectMiscValue[i]);
	if(ci)
	{
		if(p_caster->getClass() == WARLOCK)
		{
			//if demonic sacrifice auras are still active, remove them
			p_caster->RemoveAura(18789);
			p_caster->RemoveAura(18790);
			p_caster->RemoveAura(18791);
			p_caster->RemoveAura(18792);
			p_caster->RemoveAura(35701);
		}

		Pet* summon = objmgr.CreatePet(GetProto()->EffectMiscValue[i]);
		if(! summon->CreateAsSummon(GetProto()->EffectMiscValue[i], ci, NULL, p_caster, GetProto(), 2, 0))
		{
			summon->DeleteMe();//CreateAsSummon() returns false if an error occurred.
			summon = NULL;
		}
	}
}

void Spell::SpellEffectLearnPetSpell(uint32 i)
{
	/*if(unitTarget && m_caster->GetTypeId() == TYPEID_PLAYER)
	{
	if(unitTarget->IsPet() && unitTarget->GetTypeId() == TYPEID_UNIT)
	{
	TO< Player* >(m_caster)->AddPetSpell(GetProto()->EffectTriggerSpell[i], unitTarget->GetEntry());
	}
	}*/

	if(unitTarget && unitTarget->IsPet() && p_caster)
	{
		Pet* pPet = TO< Pet* >(unitTarget);
		if(pPet->IsSummonedPet())
			p_caster->AddSummonSpell(unitTarget->GetEntry(), GetProto()->EffectTriggerSpell[i]);

		pPet->AddSpell(dbcSpell.LookupEntry(GetProto()->EffectTriggerSpell[i]), true);

		// Send Packet
		/*		WorldPacket data(SMSG_SET_EXTRA_AURA_INFO_OBSOLETE, 22);
		data << pPet->GetGUID() << uint8(0) << uint32(GetProto()->EffectTriggerSpell[i]) << uint32(-1) << uint32(0);
		p_caster->GetSession()->SendPacket(&data);*/
	}
}

void Spell::SpellEffectWeapondamage(uint32 i)   // Weapon damage +
{
	if(!unitTarget || !u_caster) return;

	//Hackfix for Mangle
	if(GetProto()->NameHash == SPELL_HASH_MANGLE__CAT_ && p_caster != NULL)
		p_caster->AddComboPoints(unitTarget->GetGUID(), 1);

	// Hacky fix for druid spells where it would "double attack".
	if(GetProto()->Effect[2] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || GetProto()->Effect[1] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)
	{
		add_damage += damage;
		return;
	}

	uint32 _type;
	if(GetType() == SPELL_DMG_TYPE_RANGED)
		_type = RANGED;
	else
	{
		if(hasAttributeExC(FLAGS4_TYPE_OFFHAND))
			_type =  OFFHAND;
		else
			_type = MELEE;
	}
	u_caster->Strike(unitTarget, _type, GetProto(), damage, 0, 0, false, true);
}

void Spell::SpellEffectOpenLockItem(uint32 i)
{
	if(p_caster == NULL || i_caster == NULL)
		return;

	p_caster->HandleSpellLoot(i_caster->GetProto()->ItemId);
}

void Spell::SpellEffectProficiency(uint32 i)
{
	uint32 skill = 0;
	skilllinespell* skillability = objmgr.GetSpellSkill(GetProto()->Id);
	if(skillability)
		skill = skillability->skilline;
	skilllineentry* sk = dbcSkillLine.LookupEntryForced(skill);
	if(skill)
	{
		if(playerTarget)
		{
			if(playerTarget->_HasSkillLine(skill))
			{
				// Increase it by one
				// playerTarget->AdvanceSkillLine(skill);
			}
			else
			{
				// Don't add skills to players logging in.
				/*if((GetProto()->Attributes & 64) && playerTarget->m_TeleportState == 1)
				return;*/

				if(sk && sk->type == SKILL_TYPE_WEAPON)
					playerTarget->_AddSkillLine(skill, 1, 5 * playerTarget->getLevel());
				else
					playerTarget->_AddSkillLine(skill, 1, 1);
			}
		}
	}
}

void Spell::SpellEffectSendEvent(uint32 i) //Send Event
{
	//This is mostly used to trigger events on quests or some places

	if(sScriptMgr.CallScriptedDummySpell(m_spellInfo->Id, i, this))
		return;

	if(sScriptMgr.HandleScriptedSpellEffect(m_spellInfo->Id, i, this))
		return;

	LOG_ERROR("Spell ID: %u ( %s ) has a scripted effect ( %u ) but no handler for it.", m_spellInfo->Id, m_spellInfo->Name, i);

}

void Spell::SpellEffectPowerBurn(uint32 i) // power burn
{
	if(unitTarget == NULL || !unitTarget->isAlive() || unitTarget->GetPowerType() != POWER_TYPE_MANA)
		return;

	if(unitTarget->IsPlayer())
	{
		Player* mPlayer = TO< Player* >(unitTarget);
		if(mPlayer->IsInFeralForm())
			return;

		// Resilience - reduces the effect of mana drains by (CalcRating*2)%.
		damage = float2int32(damage * (1 - ((TO< Player* >(unitTarget)->CalcRating(PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE) * 2) / 100.0f)));
	}
	int32 mult = damage;
	damage = mult * unitTarget->GetMaxPower(POWER_TYPE_MANA) / 100;
	if(m_caster->IsUnit())  //Spell ctor has ASSERT( m_caster != NULL ) so there's no need to add NULL checks, even if static analysis reports them.
	{
		Unit* caster = TO< Unit* >(m_caster);
		if((uint32) damage > caster->GetMaxPower(POWER_TYPE_MANA) * (mult * 2) / 100)
			damage = caster->GetMaxPower(POWER_TYPE_MANA) * (mult * 2) / 100;
	}

	int32 mana = (int32)min((int32)unitTarget->GetPower(POWER_TYPE_MANA), damage);

	unitTarget->ModPower(POWER_TYPE_MANA, -mana);

	m_caster->SpellNonMeleeDamageLog(unitTarget, GetProto()->Id, (uint32)(mana * GetProto()->EffectMultipleValue[i]), pSpellId == 0, true);
}

void Spell::SpellEffectThreat(uint32 i) // Threat
{
	if(!unitTarget || !unitTarget->isAlive() || !unitTarget->IsCreature())
		return;

	int32 amount = GetProto()->EffectBasePoints[i];
	if(GetProto()->SpellGroupType)
	{
		SM_FIValue(u_caster->SM_FMiscEffect, &amount, GetProto()->SpellGroupType);
		SM_PIValue(u_caster->SM_PMiscEffect, &amount, GetProto()->SpellGroupType);
	}

	bool chck = unitTarget->GetAIInterface()->modThreatByPtr(u_caster, amount);
	if(!chck)
		unitTarget->GetAIInterface()->AttackReaction(u_caster, 1, 0);
}

void Spell::SpellEffectClearQuest(uint32 i)
{
	if(playerTarget == NULL)
	{
		LOG_ERROR("Spell %u ( %s ) was not casted on Player, but Spell requires Player to be a target.", m_spellInfo->Id, m_spellInfo->Name);
		return;
	}

	uint32 questid1 = m_spellInfo->EffectBasePoints[ i ];
	uint32 questid2 = m_spellInfo->EffectMiscValue[ i ];

	playerTarget->ClearQuest(questid1);
	playerTarget->ClearQuest(questid2);
}

void Spell::SpellEffectTriggerSpell(uint32 i) // Trigger Spell
{
	SpellEntry* entry = dbcSpell.LookupEntryForced(GetProto()->EffectTriggerSpell[i]);
	if(entry == NULL)
		return;

	SpellCastTargets targets = m_targets;
	Spell* sp = sSpellFactoryMgr.NewSpell(m_caster, entry, m_triggeredSpell, NULL);
	sp->ProcedOnSpell = GetProto();
	sp->prepare(&targets);
}

void Spell::SpellEffectApplyRaidAA(uint32 i)
{
	ApplyAA(i);
}

void Spell::SpellEffectPowerFunnel(uint32 i) // Power Funnel
{
	if(!unitTarget || !unitTarget->isAlive() || !unitTarget->IsPet())
		return;

	//does not exist
}

void Spell::SpellEffectHealMaxHealth(uint32 i)   // Heal Max Health
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 dif = unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) - unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	if(!dif)
	{
		SendCastResult(SPELL_FAILED_ALREADY_AT_FULL_HEALTH);
		return;
	}

	SendHealSpellOnPlayer(m_caster, unitTarget, dif, false, 0, pSpellId ? pSpellId : GetProto()->Id);

	unitTarget->ModHealth(dif);

	if(u_caster != NULL && this->GetProto()->NameHash == SPELL_HASH_LAY_ON_HANDS)
		u_caster->CastSpell(unitTarget, 25771, true);
}

void Spell::SpellEffectInterruptCast(uint32 i) // Interrupt Cast
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	if(!playerTarget)
	{
		if(u_caster && (u_caster != unitTarget))
		{
			unitTarget->GetAIInterface()->AttackReaction(u_caster, 1, m_spellInfo->Id);
			Creature* c = TO< Creature* >(unitTarget);
			if(c->GetProto()->modImmunities & 32768)
				return;
		}
	}

	Spell *TargetSpell = unitTarget->GetCurrentSpell(); // Get target's casting spell
	if(TargetSpell)
	{
		uint32 school = TargetSpell->GetProto()->School; // Get target's casting spell school
		int32 duration = GetDuration(); // Duration of school lockout

		// Check for CastingTime (to prevent interrupting instant casts), PreventionType
		// and InterruptFlags of target's casting spell
		if(school
			&& (TargetSpell->getState() == SPELL_STATE_CASTING
			|| (TargetSpell->getState() == SPELL_STATE_PREPARING && TargetSpell->GetProto()->CastingTimeIndex > 0))
			&& TargetSpell->GetProto()->PreventionType == PREVENTION_TYPE_SILENCE
			&& ((TargetSpell->GetProto()->InterruptFlags & CAST_INTERRUPT_ON_INTERRUPT_SCHOOL)
			|| (TargetSpell->GetProto()->ChannelInterruptFlags & CHANNEL_INTERRUPT_ON_4 )))
		{
			if(unitTarget->IsPlayer())
			{
				// Check for interruption reducing talents
				int32 DurationModifier = TO< Player* >(unitTarget)->MechanicDurationPctMod[MECHANIC_INTERRUPTED];
				if(DurationModifier >= - 100)
					duration = (duration * (100 + DurationModifier)) / 100;

				// Prevent player from casting in that school
				TO_PLAYER(unitTarget)->SendPreventSchoolCast(school, duration);
			}
			else
				// Prevent unit from casting in that school
				unitTarget->SchoolCastPrevent[school] = duration + getMSTime();

			TargetSpell->cancel(); // Interrupt the spell cast
		}
	}
}

void Spell::SpellEffectDistract(uint32 i) // Distract
{
	//spellId 1725 Distract:Throws a distraction attracting the all monsters for ten sec's
	if(!unitTarget || !unitTarget->isAlive())
		return;

	if(m_targets.m_destX != 0.0f || m_targets.m_destY != 0.0f || m_targets.m_destZ != 0.0f)
	{
		//		unitTarget->GetAIInterface()->MoveTo(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, 0);
		uint32 Stare_duration = GetDuration();
		if(Stare_duration > 30 * 60 * 1000)
			Stare_duration = 10000;//if we try to stare for more then a half an hour then better not stare at all :P (bug)
		float newo = unitTarget->calcRadAngle(unitTarget->GetPositionX(), unitTarget->GetPositionY(), m_targets.m_destX, m_targets.m_destY);
		unitTarget->GetAIInterface()->StopMovement(Stare_duration);
		unitTarget->SetFacing(newo);
	}

	//Smoke Emitter 164870
	//Smoke Emitter Big 179066
	//Unit Field Target of
}

void Spell::SpellEffectPickpocket(uint32 i) // pickpocket
{
	//Show random loot based on roll,
	if(!unitTarget || !p_caster || !unitTarget->IsCreature())
		return;

	Creature* target = TO< Creature* >(unitTarget);
	if(target->IsPickPocketed() || (target->GetCreatureInfo()->Type != UNIT_TYPE_HUMANOID))
	{
		SendCastResult(SPELL_FAILED_TARGET_NO_POCKETS);
		return;
	}

	lootmgr.FillPickpocketingLoot(&TO< Creature* >(unitTarget)->loot, unitTarget->GetEntry());

	uint32 _rank = TO< Creature* >(unitTarget)->GetCreatureInfo()->Rank;
	unitTarget->loot.gold = float2int32((_rank + 1) * unitTarget->getLevel() * (RandomUInt(5) + 1) * sWorld.getRate(RATE_MONEY));

	p_caster->SendLoot(unitTarget->GetGUID(), LOOT_PICKPOCKETING, unitTarget->GetMapId());
	target->SetPickPocketed(true);
}

void Spell::SpellEffectAddFarsight(uint32 i) // Add Farsight
{
	if(p_caster == NULL)
		return;

	float x = m_targets.m_destX;
	float y = m_targets.m_destY;
	float z = m_targets.m_destZ;
	if(x == 0) x = m_targets.m_srcX;
	if(y == 0) y = m_targets.m_srcY;
	if(z == 0) z = m_targets.m_srcZ;

	DynamicObject* dynObj = p_caster->GetMapMgr()->CreateDynamicObject();
	dynObj->Create(u_caster, this, x, y, z, GetDuration(), GetRadius(i));

	/*
	if( dynObj == NULL ) //i <3 burlex :P
	{
	delete dynObj;
	return;
	}
	*/
	dynObj->SetUInt32Value(OBJECT_FIELD_TYPE, 65);
	dynObj->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x80000002);
	dynObj->SetInstanceID(p_caster->GetInstanceID());
	p_caster->SetFarsightTarget(dynObj->GetGUID());

	p_caster->GetMapMgr()->ChangeFarsightLocation(p_caster, dynObj);
}

void Spell::SpellEffectUseGlyph(uint32 i)
{
	if(!p_caster) return;

	uint32 g_new = m_spellInfo->EffectMiscValue[i];
	GlyphPropertyEntry* gp_new = dbcGlyphProperty.LookupEntryForced(g_new);
	if(!gp_new)
		return;

	// check if glyph is locked (obviously)
	if(!(p_caster->GetUInt32Value(PLAYER_GLYPHS_ENABLED) & (1 << m_glyphslot)))
	{
		SendCastResult(SPELL_FAILED_GLYPH_SOCKET_LOCKED);
		return;
	}

	uint32 g_old = p_caster->GetGlyph(m_glyphslot);
	if(g_old)
	{
		if(g_old == g_new)
			return;
		else
		{
			GlyphPropertyEntry* gp_old = dbcGlyphProperty.LookupEntryForced(g_old);
			if(gp_old)
				p_caster->RemoveAura(gp_old->SpellID);
		}
	}

	GlyphSlotEntry* gs = dbcGlyphSlot.LookupEntryForced(p_caster->GetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS_1 + m_glyphslot));
	if(gs)
	{
		if(gs->Type != gp_new->Type)
		{
			SendCastResult(SPELL_FAILED_INVALID_GLYPH);
			return;
		}
		p_caster->SetGlyph(m_glyphslot, g_new);
		p_caster->CastSpell(p_caster, gp_new->SpellID, true);
		p_caster->m_specs[p_caster->m_talentActiveSpec].glyphs[m_glyphslot] = static_cast< uint16 >(g_new);
		p_caster->smsg_TalentsInfo(false);
	}

}

void Spell::SpellEffectHealMechanical(uint32 i)
{
	if(!unitTarget || !unitTarget->IsCreature() || TO< Creature* >(unitTarget)->GetCreatureInfo()->Type != UNIT_TYPE_MECHANICAL)
		return;

	Heal(damage);
}

void Spell::SpellEffectSummonObjectWild(uint32 i)
{
	if(!u_caster) return;

	// spawn a new one
	GameObject* GoSummon = u_caster->GetMapMgr()->CreateGameObject(GetProto()->EffectMiscValue[i]);
	if(!GoSummon->CreateFromProto(GetProto()->EffectMiscValue[i],
	                              m_caster->GetMapId(), m_caster->GetPositionX() + 1, m_caster->GetPositionY() + 1, m_caster->GetPositionZ(), m_caster->GetOrientation()))
	{
		delete GoSummon;
		return;
	}

	GoSummon->Phase(PHASE_SET, u_caster->GetPhase());
	GoSummon->PushToWorld(u_caster->GetMapMgr());
	GoSummon->SetSummoned(u_caster);

	sEventMgr.AddEvent(GoSummon, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::SpellEffectScriptEffect(uint32 i) // Script Effect
{
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummySpell(m_spellInfo->Id, i, this))
		return;

	if(sScriptMgr.HandleScriptedSpellEffect(m_spellInfo->Id, i, this))
		return;

	LOG_ERROR("Spell ID: %u ( %s ) has a scripted effect ( %u ) but no handler for it.", m_spellInfo->Id, m_spellInfo->Name, i);
}

void Spell::SpellEffectSanctuary(uint32 i) // Stop all attacks made to you
{
	if(!u_caster)
		return;

	//warning this causes crashes !
	//	Object::InRangeSet::iterator itr = u_caster->GetInRangeOppFactsSetBegin();
	//	Object::InRangeSet::iterator itr_end = u_caster->GetInRangeOppFactsSetEnd();
	//use these instead
	Object::InRangeSet::iterator itr = u_caster->GetInRangeSetBegin();
	Object::InRangeSet::iterator itr_end = u_caster->GetInRangeSetEnd();

	if(p_caster != NULL)
		p_caster->RemoveAllAuraType(SPELL_AURA_MOD_ROOT);

	for(; itr != itr_end; ++itr)
	{
		if((*itr)->IsCreature())
			TO_CREATURE(*itr)->GetAIInterface()->RemoveThreatByPtr(unitTarget);
	}
}

void Spell::SpellEffectAddComboPoints(uint32 i) // Add Combo Points
{
	if(!p_caster)
		return;

	//if this is a procspell Ruthlessness (maybe others later)
	if(pSpellId && (GetProto()->Id == 14157 || GetProto()->Id == 70802 || GetProto()->Id == 14181))
	{
		//it seems this combo adding procspell is going to change combopoint count before they will get reset. We add it after the reset
		/* burlex - this is wrong, and exploitable.. :/ if someone uses this they will have unlimited combo points */
		//re-enabled this by Zack. Explained why it is used + rechecked to make sure initialization is good ...
		// while casting a spell talent will trigger upon the spell prepare faze
		// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
		p_caster->m_spellcomboPoints += static_cast<int8>(damage);
		return;
	}
	p_caster->AddComboPoints(p_caster->GetSelection(), static_cast<uint8>(damage));
}

void Spell::SpellEffectCreateHouse(uint32 i) // Create House
{


}

void Spell::SpellEffectDuel(uint32 i) // Duel
{
	if(!p_caster || !p_caster->isAlive())
		return;

	if(p_caster->IsStealth())
	{
		SendCastResult(SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED);
		return; // Player is stealth
	}
	if(!playerTarget || playerTarget == p_caster)
		return;

	/* not implemented yet
	TODO: dueling zones ? ( SPELL_FAILED_NO_DUELING )
	if (player->IsInvisible())
	{
	SendCastResult(SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE);
	return;
	}
	*/

	//Player *pTarget = sObjHolder.GetObject<Player>(player->GetSelection());	   //  hacky.. and will screw up if plr is deselected..
	if(!playerTarget)
	{
		SendCastResult(SPELL_FAILED_BAD_TARGETS);
		return; // invalid Target
	}
	if(!playerTarget->isAlive())
	{
		SendCastResult(SPELL_FAILED_TARGETS_DEAD);
		return; // Target not alive
	}
	if(playerTarget->hasStateFlag(UF_ATTACKING))
	{
		SendCastResult(SPELL_FAILED_TARGET_IN_COMBAT);
		return; // Target in combat with another unit
	}
	if(playerTarget->DuelingWith)
	{
		SendCastResult(SPELL_FAILED_TARGET_DUELING);
		return; // Already Dueling
	}

	p_caster->RequestDuel(playerTarget);
}

void Spell::SpellEffectStuck(uint32 i)
{
	if(!playerTarget || playerTarget != p_caster)
		return;

	sEventMgr.AddEvent(playerTarget, &Player::EventTeleport, playerTarget->GetBindMapId(), playerTarget->GetBindPositionX(), playerTarget->GetBindPositionY(),
	                   playerTarget->GetBindPositionZ(), EVENT_PLAYER_TELEPORT, 50, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	/*
	playerTarget->SafeTeleport(playerTarget->GetBindMapId(), 0, playerTarget->GetBindPositionX(), playerTarget->GetBindPositionY(), playerTarget->GetBindPositionZ(), 3.14f);*/
}

void Spell::SpellEffectSummonPlayer(uint32 i)
{
	if(!playerTarget) return;

	// vojta: from 2.4 players can be summoned on another map
	//if( m_caster->GetMapMgr()->GetMapInfo() && m_caster->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL && m_caster->GetMapId() != playerTarget->GetMapId())
	//	return;
	if(m_caster->GetMapMgr()->GetMapInfo() && playerTarget->getLevel() < m_caster->GetMapMgr()->GetMapInfo()->minlevel)    // we need some blizzlike message that player needs level xx - feel free to add it ;)
		return;

	playerTarget->SummonRequest(m_caster->GetLowGUID(), m_caster->GetZoneId(), m_caster->GetMapId(), m_caster->GetInstanceID(), m_caster->GetPosition());
}

void Spell::SpellEffectActivateObject(uint32 i) // Activate Object
{
	if(!p_caster)
		return;

	if(!gameObjTarget)
	{
		LOG_ERROR("Spell %u ( %s ) effect %u not handled because no target was found. ", m_spellInfo->Id, m_spellInfo->Name, i);
		return;
	}

	CALL_GO_SCRIPT_EVENT(gameObjTarget, OnActivate)(p_caster);
	gameObjTarget->Activate();

	sEventMgr.AddEvent(gameObjTarget, &GameObject::Deactivate, 0, GetDuration(), 1, 0);
}


void Spell::SpellEffectBuildingDamage(uint32 i){
	if( gameObjTarget == NULL )
		return;

	if( gameObjTarget->GetInfo()->Type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING )
		return;

	if( u_caster == NULL )
		return;

	uint32 damage = m_spellInfo->EffectBasePoints[ i ] + 1;
	Unit *controller = NULL;

	if( u_caster->GetVehicleComponent() != NULL )
		controller = u_caster->GetMapMgr()->GetUnit( u_caster->GetCharmedByGUID() );

	if( controller == NULL )
		controller = u_caster;

	// Baaaam
	gameObjTarget->Damage( damage, u_caster->GetGUID(), controller->GetGUID(), m_spellInfo->Id );
}



void Spell::SpellEffectEnchantHeldItem(uint32 i)
{
	if(!playerTarget) return;

	Item* item = playerTarget->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	if(!item)
		return;

	uint32 Duration = 1800; // Needs to be found in dbc.. I guess?
	switch(GetProto()->NameHash)
	{
		case SPELL_HASH_WINDFURY_WEAPON: // Windfury Weapon Effect
			{
				Duration = 10;
			}
		case SPELL_HASH_FLAMETONGUE_WEAPON: // Flametongue Weapon Effect
			{
				Duration = 10;
			}
	}
	EnchantEntry* Enchantment = dbcEnchant.LookupEntryForced(GetProto()->EffectMiscValue[i]);

	if(!Enchantment)
	{
		LOG_ERROR("Invalid enchantment entry %u for Spell %u", GetProto()->EffectMiscValue[ i ], GetProto()->Id);
		return;
	}

	/* Windfury Totem removed in 3.0.2
	if (m_spellInfo->NameHash == SPELL_HASH_WINDFURY_TOTEM_EFFECT && item->HasEnchantmentOnSlot( 1 ) && item->GetEnchantment( 1 )->Enchantment != Enchantment) //dirty fix for Windfury totem not overwriting existing enchantments
	return;
	*/

	item->RemoveEnchantment(1);
	item->AddEnchantment(Enchantment, Duration, false, true, false, 1);
}

void Spell::SpellEffectSelfResurrect(uint32 i)
{
	if(!p_caster || !unitTarget || playerTarget->isAlive()) return;

	uint32 mana;
	uint32 health;
	uint32 class_ = unitTarget->getClass();

	switch(GetProto()->Id)
	{
		case 3026:
		case 20758:
		case 20759:
		case 20760:
		case 20761:
		case 27240:
		case 47882:
			{
				health = GetProto()->EffectMiscValue[i];
				mana = -damage;
			}
			break;
		case 21169: //Reincarnation. Resurrect with 20% health and mana
			{
				int32 amt = 20;
				if(GetProto()->SpellGroupType)
				{
					SM_FIValue(unitTarget->SM_FMiscEffect, &amt, GetProto()->SpellGroupType);
					SM_PIValue(unitTarget->SM_PMiscEffect, &amt, GetProto()->SpellGroupType);
				}
				health = uint32((unitTarget->GetMaxHealth() * amt) / 100);
				mana = uint32((unitTarget->GetMaxPower(POWER_TYPE_MANA) * amt) / 100);
			}
			break;
		default:
			{
				if(damage < 0) return;
				health = uint32(unitTarget->GetMaxHealth() * damage / 100);
				mana = uint32(unitTarget->GetMaxPower(POWER_TYPE_MANA) * damage / 100);
			}
			break;
	}

	if(class_ == WARRIOR || class_ == ROGUE)
		mana = 0;

	playerTarget->m_resurrectHealth = health;
	playerTarget->m_resurrectMana = mana;

	playerTarget->ResurrectPlayer();
	playerTarget->SetMovement(MOVE_UNROOT, 1);

	playerTarget->SetUInt32Value(PLAYER_SELF_RES_SPELL, 0);

	if(GetProto()->Id == 21169)
		AddCooldown();
}

void Spell::SpellEffectSkinning(uint32 i)
{
	if(!unitTarget || !unitTarget->IsCreature())
		return;

	Creature* cr = TO_CREATURE(unitTarget);
	uint32 skill = cr->GetRequiredLootSkill();
	uint32 sk = TO_PLAYER(m_caster)->_GetSkillLineCurrent(skill);
	uint32 lvl = cr->getLevel();

	if((sk >= lvl * 5) || ((sk + 100) >= lvl * 10))
	{
		//Fill loot for Skinning
		lootmgr.FillSkinningLoot(&cr->loot, unitTarget->GetEntry());
		TO_PLAYER(m_caster)->SendLoot(unitTarget->GetGUID(), LOOT_SKINNING, unitTarget->GetMapId());

		//Not skinable again
		cr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
		cr->Skinned = true;

		if(cr->GetCreatureInfo()->Rank > 0)
			DetermineSkillUp(skill , sk < lvl * 5 ? sk / 5 : lvl, 2);
		else
			DetermineSkillUp(skill , sk < lvl * 5 ? sk / 5 : lvl, 1);
	}
	else
	{
		SendCastResult(SPELL_FAILED_TARGET_UNSKINNABLE);
	}
}

void Spell::SpellEffectCharge(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;
	if(u_caster->IsStunned() || u_caster->m_rooted || u_caster->IsPacified() || u_caster->IsFeared())
		return;

	float x, y, z;
	float rad = unitTarget->GetBoundingRadius() - u_caster->GetBoundingRadius();

	float dx = m_caster->GetPositionX() - unitTarget->GetPositionX();
	float dy = m_caster->GetPositionY() - unitTarget->GetPositionY();
	if(dx == 0.0f || dy == 0.0f)
		return;
	float alpha = atanf(dy / dx);
	if(dx < 0)
		alpha += M_PI_FLOAT;

	x = rad * cosf(alpha) + unitTarget->GetPositionX();
	y = rad * sinf(alpha) + unitTarget->GetPositionY();
	z = unitTarget->GetPositionZ();

	if(!u_caster->GetAIInterface()->MoveCharge(x, y, z))
	{
		//failed
		SendInterrupted(SPELL_FAILED_NOPATH);
		SendCastResult(SPELL_FAILED_NOPATH);
	}
}

void Spell::SpellEffectKnockBack(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	unitTarget->HandleKnockback(m_caster, GetProto()->EffectMiscValue[i] / 10, damage / 10);
}

void Spell::SpellEffectKnockBack2(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	unitTarget->HandleKnockback(m_caster, GetProto()->EffectMiscValue[i] / 10, damage / 10);
}

void Spell::SpellEffectDisenchant(uint32 i)
{
	if(!p_caster)
		return;

	Item* it = p_caster->GetItemInterface()->GetItemByGUID(m_targets.m_itemTarget);
	if(!it)
	{
		SendCastResult(SPELL_FAILED_CANT_BE_DISENCHANTED);
		return;
	}

	//Fill disenchanting loot
	p_caster->SetLootGUID(it->GetGUID());
	if(!it->loot)
	{
		it->loot = new Loot;
		lootmgr.FillItemLoot(it->loot, it->GetEntry());
	}

	Log.Debug("SpellEffect", "Successfully disenchanted item %d", uint32(it->GetEntry()));
	p_caster->SendLoot(it->GetGUID(), LOOT_DISENCHANTING, p_caster->GetMapId());

	//We can increase Enchanting skill up to 60
	uint32 skill = p_caster->_GetSkillLineCurrent(SKILL_ENCHANTING);
	if(skill && skill < 60)
	{
		if(Rand(100.0f - skill * 0.75f))
		{
			uint32 SkillUp = float2int32(1.0f * sWorld.getRate(RATE_SKILLRATE));
			if(skill + SkillUp > 60)
				SkillUp = 60 - skill;

			p_caster->_AdvanceSkillLine(SKILL_ENCHANTING, SkillUp);
		}
	}
	if(it == i_caster)
		i_caster = NULL;
}

void Spell::SpellEffectInebriate(uint32 i) // lets get drunk!
{
	if(playerTarget == NULL)
		return;

	// Drunkee!
	uint16 currentDrunk = playerTarget->GetDrunkValue();
	uint16 drunkMod = static_cast<uint16>(damage) * 256;
	if(currentDrunk + drunkMod > 0xFFFF)
		currentDrunk = 0xFFFF;
	else
		currentDrunk += drunkMod;
	playerTarget->SetDrunkValue(currentDrunk, i_caster ? i_caster->GetEntry() : 0);
}

void Spell::SpellEffectFeedPet(uint32 i)  // Feed Pet
{
	// food flags and food level are checked in Spell::CanCast()
	if(!itemTarget || !p_caster)
		return;

	Pet* pPet = p_caster->GetSummon();
	if(!pPet)
		return;

	/**	Cast feed pet effect
	- effect is item level and pet level dependent, aura ticks are 35, 17, 8 (*1000) happiness*/
	int8 deltaLvl = static_cast<int8>(pPet->getLevel() - itemTarget->GetProto()->ItemLevel);
	damage /= 1000; //damage of Feed pet spell is 35000
	if(deltaLvl > 10) damage = damage >> 1;//divide by 2
	if(deltaLvl > 20) damage = damage >> 1;
	damage *= 1000;

	SpellEntry* spellInfo = dbcSpell.LookupEntry(GetProto()->EffectTriggerSpell[i]);
	Spell* sp = sSpellFactoryMgr.NewSpell(p_caster, spellInfo, true, NULL);
	sp->forced_basepoints[0] = damage;
	SpellCastTargets tgt;
	tgt.m_unitTarget = pPet->GetGUID();
	sp->prepare(&tgt);

	if(itemTarget->GetStackCount() > 1)
	{
		itemTarget->ModStackCount(-1);
		itemTarget->m_isDirty = true;
	}
	else
	{
		p_caster->GetItemInterface()->SafeFullRemoveItemByGuid(itemTarget->GetGUID());
		itemTarget = NULL;
	}
}

void Spell::SpellEffectDismissPet(uint32 i)
{
	// remove pet.. but don't delete so it can be called later
	if(!p_caster) return;

	Pet* pPet = p_caster->GetSummon();
	if(!pPet) return;
	pPet->Remove(true, true);
}

void Spell::SpellEffectReputation(uint32 i)
{
	if(!playerTarget)
		return;

	//playerTarget->modReputation(GetProto()->EffectMiscValue[i], damage, true);
	playerTarget->ModStanding(GetProto()->EffectMiscValue[i], damage);
}

void Spell::SpellEffectSummonObjectSlot(uint32 i)
{
	if(!u_caster || !u_caster->IsInWorld())
		return;

	GameObject* GoSummon = NULL;

	uint32 slot = GetProto()->Effect[i] - SPELL_EFFECT_SUMMON_OBJECT_SLOT1;
	GoSummon = u_caster->m_ObjectSlots[slot] ? u_caster->GetMapMgr()->GetGameObject(u_caster->m_ObjectSlots[slot]) : 0;
	u_caster->m_ObjectSlots[slot] = 0;

	if(GoSummon)
	{
		if(GoSummon->GetInstanceID() != u_caster->GetInstanceID())
			GoSummon->ExpireAndDelete();
		else
		{
			if(GoSummon->IsInWorld())
				GoSummon->RemoveFromWorld(true);
			delete GoSummon;
		}
	}


	// spawn a new one
	GoSummon = u_caster->GetMapMgr()->CreateGameObject(GetProto()->EffectMiscValue[i]);

	float dx = 0.0f;
	float dy = 0.0f;
	float dz = 0.0f;

	if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
	{
		dx = m_targets.m_destX;
		dy = m_targets.m_destY;
		dz = m_targets.m_destZ;
	}
	else
	{
		dx = m_caster->GetPositionX();
		dy = m_caster->GetPositionY();
		dz = m_caster->GetPositionZ();
	}

	if(!GoSummon->CreateFromProto(GetProto()->EffectMiscValue[i], m_caster->GetMapId(), dx, dy, dz, m_caster->GetOrientation()))
	{
		delete GoSummon;
		return;
	}

	GoSummon->SetLevel(u_caster->getLevel());
	GoSummon->SetUInt64Value(OBJECT_FIELD_CREATED_BY, m_caster->GetGUID());
	GoSummon->Phase(PHASE_SET, u_caster->GetPhase());

	GoSummon->PushToWorld(m_caster->GetMapMgr());

	if(GoSummon->GetType() == GAMEOBJECT_TYPE_TRAP)
	{
		GoSummon->invisible = true;
		GoSummon->invisibilityFlag = INVIS_FLAG_TRAP;
		GoSummon->charges = 1;
		GoSummon->checkrate = 1;
		sEventMgr.AddEvent(GoSummon, &GameObject::TrapSearchTarget, EVENT_GAMEOBJECT_TRAP_SEARCH_TARGET, 100, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	sEventMgr.AddEvent(GoSummon, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	GoSummon->SetSummoned(u_caster);
	u_caster->m_ObjectSlots[slot] = GoSummon->GetUIdFromGUID();
}

void Spell::SpellEffectDispelMechanic(uint32 i)
{
	if(!unitTarget || !unitTarget->isAlive())
		return;
	/* this was already working before...
	uint32 sMisc = GetProto()->EffectMiscValue[i];

	for( uint32 x = 0 ; x<MAX_AURAS ; x++ )
	{
	if( unitTarget->m_auras[x] && !unitTarget->m_auras[x]->IsPositive())
	{
	if( unitTarget->m_auras[x]->GetSpellProto()->MechanicsType == sMisc )
	unitTarget->m_auras[x]->Remove();
	}
	}
	*/
	unitTarget->RemoveAllAurasByMechanic(GetProto()->EffectMiscValue[i] , GetProto()->EffectBasePoints[i] , false);

	/*Shady: if it's about Daze spell - dismount should be done by RemoveAllAurasByMechanic.
	We don't need useless code or hackfixes here, so commented.*/
	//if( playerTarget && GetProto()->NameHash == SPELL_HASH_DAZED && playerTarget->IsMounted() )
	//	playerTarget->RemoveAura(playerTarget->m_MountSpellId);
}

void Spell::SpellEffectSummonDeadPet(uint32 i)
{
	//this is pet resurrect
	if(!p_caster)
		return;
	Pet* pPet = p_caster->GetSummon();
	if(pPet)
	{
		if(GetProto()->SpellGroupType)
		{
			SM_FIValue(p_caster->SM_FMiscEffect, &damage, GetProto()->SpellGroupType);
			SM_PIValue(p_caster->SM_PMiscEffect, &damage, GetProto()->SpellGroupType);
		}

		pPet->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
		pPet->SetHealth((uint32)((pPet->GetMaxHealth() * damage) / 100));
		pPet->setDeathState(ALIVE);
		pPet->GetAIInterface()->HandleEvent(EVENT_FOLLOWOWNER, pPet, 0);
		sEventMgr.RemoveEvents(pPet, EVENT_PET_DELAYED_REMOVE);
		pPet->SendSpellsToOwner();
	}
	else
	{

		p_caster->SpawnPet(p_caster->GetUnstabledPetNumber());
		pPet = p_caster->GetSummon();
		if(pPet == NULL)//no pets to Revive
			return;
		if(GetProto()->SpellGroupType)
		{
			SM_FIValue(p_caster->SM_FMiscEffect, &damage, GetProto()->SpellGroupType);
			SM_PIValue(p_caster->SM_PMiscEffect, &damage, GetProto()->SpellGroupType);
		}
		pPet->SetHealth((uint32)((pPet->GetMaxHealth() * damage) / 100));
	}
}

/* This effect has 2 functions
* 1. It delete's all current totems from the player
* 2. It returns a percentage of the mana back to the player
*
* Bur kick my ass if this is not safe:P
*/

void Spell::SpellEffectDestroyAllTotems(uint32 i)
{

	if(p_caster == NULL || !p_caster->IsInWorld())
		return;

	uint32 RetreivedMana = 0;
	uint32 refundpercent = m_spellInfo->EffectBasePoints[ i ] + 1;

	std::vector< uint32 > spellids;

	p_caster->summonhandler.GetSummonSlotSpellIDs(spellids);

	for(std::vector< uint32 >::iterator itr = spellids.begin(); itr != spellids.end(); ++itr)
	{
		uint32 spellid = *itr;
		SpellEntry* sp = dbcSpell.LookupEntry(spellid);

		if(sp != NULL)
		{
			uint32 cost = 0;

			if(sp->ManaCostPercentage != 0)
				cost = (p_caster->GetBaseMana() * sp->ManaCostPercentage) / 100;
			else
				cost = sp->manaCost;

			RetreivedMana += static_cast< uint32 >((cost * refundpercent) / 100.0f);
		}
	}

	p_caster->summonhandler.ExpireSummonsInSlot();

	p_caster->Energize(p_caster, GetProto()->Id, RetreivedMana, POWER_TYPE_MANA);
}

void Spell::SpellEffectResurrectNew(uint32 i)
{
	//base p =hp,misc mana
	if(!playerTarget)
	{
		if(!corpseTarget)
		{
			// unit resurrection handler
			if(unitTarget)
			{
				if(unitTarget->IsCreature() && unitTarget->IsPet() && unitTarget->IsDead())
				{
					uint32 hlth = ((uint32)GetProto()->EffectBasePoints[i] > unitTarget->GetMaxHealth()) ? unitTarget->GetMaxHealth() : (uint32)GetProto()->EffectBasePoints[i];
					uint32 mana = ((uint32)GetProto()->EffectBasePoints[i] > unitTarget->GetMaxPower(POWER_TYPE_MANA)) ? unitTarget->GetMaxPower(POWER_TYPE_MANA) : (uint32)GetProto()->EffectBasePoints[i];

					if(!unitTarget->IsPet())
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					else
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_PET_DELAYED_REMOVE);
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					unitTarget->SetHealth(hlth);
					unitTarget->SetPower(POWER_TYPE_MANA, mana);
					unitTarget->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
					unitTarget->setDeathState(ALIVE);
					TO< Creature* >(unitTarget)->UnTag();
					TO< Creature* >(unitTarget)->loot.gold = 0;
					TO< Creature* >(unitTarget)->loot.looters.clear();
					TO< Creature* >(unitTarget)->loot.items.clear();
				}
			}

			return;
		}
		playerTarget = objmgr.GetPlayer(GET_LOWGUID_PART(corpseTarget->GetOwner()));
		if(!playerTarget) return;
	}

	if(playerTarget->isAlive() || !playerTarget->IsInWorld())
		return;
	//resurrect
	playerTarget->m_resurrectMapId = p_caster->GetMapId();
	playerTarget->m_resurrectInstanceID = p_caster->GetInstanceID();
	playerTarget->m_resurrectPosition = p_caster->GetPosition();
	playerTarget->m_resurrectHealth = damage;
	playerTarget->m_resurrectMana = GetProto()->EffectMiscValue[i];

	SendResurrectRequest(playerTarget);
}

void Spell::SpellEffectAttackMe(uint32 i)
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	unitTarget->GetAIInterface()->AttackReaction(u_caster, 1, 0);
}

void Spell::SpellEffectSkinPlayerCorpse(uint32 i)
{
	Corpse* corpse = 0;
	if(!playerTarget)
	{
		// means we're "skinning" a corpse
		corpse = objmgr.GetCorpse((uint32)m_targets.m_unitTarget);  // hacky
	}
	else if(playerTarget->getDeathState() == CORPSE)	// repopped while we were casting
	{
		corpse = objmgr.GetCorpse(playerTarget->GetLowGUID());
	}

	if(p_caster == NULL)
		return;

	if(playerTarget && !corpse)
	{
		if(!playerTarget->m_bg || !playerTarget->IsDead())
			return;

		// Set all the lootable stuff on the player. If he repops before we've looted, we'll set the flags
		// on corpse then :p

		playerTarget->bShouldHaveLootableOnCorpse = false;
		playerTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
		playerTarget->SetFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);

		// Send the loot.
		p_caster->SendLoot(playerTarget->GetGUID(), LOOT_SKINNING, playerTarget->GetMapId());

		// Send a message to the died player, telling him he has to resurrect at the graveyard.
		// Send an empty corpse location too, :P

		playerTarget->GetSession()->OutPacket(SMSG_PLAYER_SKINNED, 1, "\x00");
		playerTarget->GetSession()->OutPacket(MSG_CORPSE_QUERY, 1, "\x00");

		// don't allow him to spawn a corpse
		playerTarget->bCorpseCreateable = false;

		// and.. force him to the graveyard and repop him.
		playerTarget->RepopRequestedPlayer();

	}
	else if(corpse)
	{
		// find the corpses' owner
		Player* owner = objmgr.GetPlayer(GET_LOWGUID_PART(corpse->GetOwner()));
		if(owner)
		{
			if(!owner->m_bg) return;

			owner->GetSession()->OutPacket(SMSG_PLAYER_SKINNED, 1, "\x00");
			owner->GetSession()->OutPacket(MSG_CORPSE_QUERY, 1, "\x00");
		}

		if(corpse->GetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS) != 1)
			corpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr

		// remove skinnable flag
		corpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 5);

		// remove owner association
		corpse->SetOwner(0);
		corpse->SetCorpseState(CORPSE_STATE_BONES);
		corpse->DeleteFromDB();
		objmgr.CorpseAddEventDespawn(corpse);

		// send loot
		p_caster->SendLoot(corpse->GetGUID(), LOOT_SKINNING, corpse->GetMapId());
	}
}

void Spell::SpellEffectSkill(uint32 i)
{
	// Used by professions only
	// Effect should be renamed in RequireSkill

	if(!p_caster || p_caster->_GetSkillLineMax(GetProto()->EffectMiscValue[i]) >= uint32(damage * 75))
		return;

	if(p_caster->_HasSkillLine(GetProto()->EffectMiscValue[i]))
		p_caster->_ModifySkillMaximum(GetProto()->EffectMiscValue[i], uint32(damage * 75));
	else
		p_caster->_AddSkillLine(GetProto()->EffectMiscValue[i], 1, uint32(damage * 75));
}

void Spell::SpellEffectApplyPetAA(uint32 i)
{
	ApplyAA(i);
}

void Spell::SpellEffectDummyMelee(uint32 i)   // Normalized Weapon damage +
{

	if(!unitTarget || !u_caster)
		return;

	if(GetProto()->NameHash == SPELL_HASH_OVERPOWER && p_caster)   //warrior : overpower - let us clear the event and the combopoint count
	{
		p_caster->NullComboPoints(); //some say that we should only remove 1 point per dodge. Due to cooldown you can't cast it twice anyway..
		sEventMgr.RemoveEvents(p_caster, EVENT_COMBO_POINT_CLEAR_FOR_TARGET);
	}
	else if(GetProto()->NameHash == SPELL_HASH_DEVASTATE)
	{
		//count the number of sunder armors on target
		uint32 sunder_count = 0;
		SpellEntry* spellInfo = dbcSpell.LookupEntry(7386);
		for(uint32 x = MAX_NEGATIVE_AURAS_EXTEDED_START; x < MAX_NEGATIVE_AURAS_EXTEDED_END; ++x)
			if(unitTarget->m_auras[x] && unitTarget->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_SUNDER_ARMOR)
			{
				sunder_count++;
				spellInfo = unitTarget->m_auras[x]->GetSpellProto();
			}
		if(spellInfo == NULL)
			return; //omg how did this happen ?
		//we should also cast sunder armor effect on target with or without dmg
		Spell* spell = sSpellFactoryMgr.NewSpell(u_caster, spellInfo , true, NULL);
		spell->ProcedOnSpell = GetProto();
		spell->pSpellId = GetProto()->Id;
		SpellCastTargets targets(unitTarget->GetGUID());
		spell->prepare(&targets);
		if(!sunder_count)
			return; //no damage = no joy
		damage = damage * sunder_count;
	}
	//Hemorrhage
	if(p_caster && GetProto()->NameHash == SPELL_HASH_HEMORRHAGE)
		p_caster->AddComboPoints(p_caster->GetSelection(), 1);

	switch(GetProto()->Id)
	{
			// AMBUSH
		case 8676:
			add_damage = 77;
			return;
			break;			// r1
		case 8724:
			add_damage = 110;
			return;
			break;			// r2
		case 8725:
			add_damage = 138;
			return;
			break;			// r3
		case 11267:
			add_damage = 204;
			return;
			break;		// r4
		case 11268:
			add_damage = 253;
			return;
			break;		// r5
		case 11269:
			add_damage = 319;
			return;
			break;		// r6
		case 27441:
			add_damage = 369;
			return;
			break;		// r7
		case 48689:
			add_damage = 509;
			return;
			break;		// r8
		case 48690:
			add_damage = 770;
			return;
			break;		// r9
		case 48691:
			add_damage = 908;
			return;
			break;		// r10

			// BACKSTAB
		case 53:
			add_damage = 15;
			return;
			break;			// r1
		case 2589:
			add_damage = 30;
			return;
			break;			// r2
		case 2590:
			add_damage = 48;
			return;
			break;			// r3
		case 2591:
			add_damage = 69;
			return;
			break;			// r4
		case 8721:
			add_damage = 90;
			return;
			break;			// r5
		case 11279:
			add_damage = 135;
			return;
			break;		// r6
		case 11280:
			add_damage = 165;
			return;
			break;		// r7
		case 11281:
			add_damage = 210;
			return;
			break;		// r8
		case 25300:
			add_damage = 225;
			return;
			break;		// r9
		case 26863:
			add_damage = 255;
			return;
			break;		// r10
		case 48656:
			add_damage = 383;
			return;
			break;		// r11
		case 48657:
			add_damage = 465;
			return;
			break;		// r12
	}

	// rogue ambush etc
	for(uint32 x = 0; x < 3; x++)
		if(GetProto()->Effect[x] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)
		{
			add_damage = damage * (GetProto()->EffectBasePoints[x] + 1) / 100;
			return;
		}

	//rogue - mutilate ads dmg if target is poisoned
	uint32 pct_dmg_mod = 100;
	if(GetProto()->NameHash == SPELL_HASH_MUTILATE && unitTarget->IsPoisoned())
		pct_dmg_mod = 120;

	uint32 _type;
	if(GetType() == SPELL_DMG_TYPE_RANGED)
		_type = RANGED;
	else
	{
		if(GetProto()->AttributesExC & 0x1000000)
			_type =  OFFHAND;
		else
			_type = MELEE;
	}
	u_caster->Strike(unitTarget, _type, GetProto(), damage, pct_dmg_mod, 0, false, true);
}

void Spell::SpellEffectStartTaxi(uint32 i)
{
	if(!playerTarget || !playerTarget->isAlive() || !u_caster)
		return;

	if(playerTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER))
		return;

	TaxiPath* taxipath = sTaxiMgr.GetTaxiPath(GetProto()->EffectMiscValue[0]);

	if(!taxipath)
		return;

	TaxiNode* taxinode = sTaxiMgr.GetTaxiNode(taxipath->GetSourceNode());

	if(!taxinode)
		return;

	uint32 modelid = 0;

	if(playerTarget->IsTeamHorde())
	{
		CreatureInfo* ci = CreatureNameStorage.LookupEntry(taxinode->horde_mount);
		if(!ci) return;
		modelid = ci->Male_DisplayID;
		if(!modelid) return;
	}
	else
	{
		CreatureInfo* ci = CreatureNameStorage.LookupEntry(taxinode->alliance_mount);
		if(!ci) return;
		modelid = ci->Male_DisplayID;
		if(!modelid) return;
	}

	playerTarget->TaxiStart(taxipath, modelid, 0);
}

void Spell::SpellEffectPlayerPull(uint32 i)
{
	if(!unitTarget || !unitTarget->isAlive() || !unitTarget->IsPlayer())
		return;

	Player* p_target = TO< Player* >(unitTarget);

	// calculate destination
	float pullD = p_target->CalcDistance(m_caster) - p_target->GetBoundingRadius() - (u_caster ? u_caster->GetBoundingRadius() : 0) - 1.0f;
	float pullO = p_target->calcRadAngle(p_target->GetPositionX(), p_target->GetPositionY(), m_caster->GetPositionX(), m_caster->GetPositionY());
	float pullX = p_target->GetPositionX() + pullD * cosf(pullO);
	float pullY = p_target->GetPositionY() + pullD * sinf(pullO);
	float pullZ = m_caster->GetPositionZ() + 0.3f;
	uint32 time = uint32(pullD * 42.0f);

	p_target->SetOrientation(pullO);

	WorldPacket data(SMSG_MONSTER_MOVE, 60);
	data << p_target->GetNewGUID();
	data << uint8(0);
	data << p_target->GetPositionX() << p_target->GetPositionY() << p_target->GetPositionZ();
	data << getMSTime();
	data << uint8(4);
	data << pullO;
	data << uint32(0x00001000);
	data << time;
	data << uint32(1);
	data << pullX << pullY << pullZ;

	p_target->SendMessageToSet(&data, true);
}

void Spell::SpellEffectReduceThreatPercent(uint32 i)
{
	if(!unitTarget || !unitTarget->IsCreature() || !u_caster || unitTarget->GetAIInterface()->getThreatByPtr(u_caster) == 0)
		return;

	unitTarget->GetAIInterface()->modThreatByPtr(u_caster, (int32)unitTarget->GetAIInterface()->getThreatByPtr(u_caster) * damage / 100);
}

void Spell::SpellEffectSpellSteal(uint32 i)
{
	if(unitTarget == NULL || u_caster == NULL || !unitTarget->isAlive())
		return;

	if(playerTarget != NULL && p_caster != NULL && p_caster != playerTarget)
	{
		if(playerTarget->IsPvPFlagged())
			p_caster->PvPToggle();
	}

	uint32 start, end;
	if(isAttackable(u_caster, unitTarget))
	{
		start = MAX_POSITIVE_AURAS_EXTEDED_START;
		end = MAX_POSITIVE_AURAS_EXTEDED_END;
	}
	else
		return;

	Aura* aur;
	SpellEntry* aursp;
	std::list< uint32 > stealedSpells;

	for(uint32 x = start; x < end; x++)
	{
		if(unitTarget->m_auras[x] != NULL)
		{
			aur = unitTarget->m_auras[x];
			aursp = aur->GetSpellProto();

			if(aursp->Id != 15007 && !aur->IsPassive()
			        //				&& aur->IsPositive()	// Zack : We are only checking positive auras. There is no meaning to check again
			  ) //Nothing can dispel resurrection sickness
			{
				if(aursp->DispelType == DISPEL_MAGIC)
				{
					stealedSpells.push_back(aursp->Id);

					uint32 aurdur = (aur->GetDuration() > 120000 ? 120000 : aur->GetDuration());
					Aura* aura = sSpellFactoryMgr.NewAura(aursp, aurdur, u_caster, u_caster);
					uint32 aur_removed = unitTarget->RemoveAllAuraByNameHash(aursp->NameHash);
					for(uint8 j = 0; j < 3; j++)
					{
						if(aura->GetSpellProto()->Effect[j])
						{
							aura->AddMod(aura->GetSpellProto()->EffectApplyAuraName[j], aura->GetSpellProto()->EffectBasePoints[j] + 1, aura->GetSpellProto()->EffectMiscValue[j], j);
						}
					}
					if(aura->GetSpellProto()->procCharges > 0)
					{
						Aura* aur2;
						for(uint32 j = 0; j < aur_removed - 1; j++)
						{
							aur2 = sSpellFactoryMgr.NewAura(aura->GetSpellProto(), aurdur, u_caster, u_caster);
							u_caster->AddAura(aur2);
						}
						if(!(aura->GetSpellProto()->procFlags & PROC_REMOVEONUSE))
						{
							SpellCharge charge;
							charge.count = aur_removed;
							charge.spellId = aura->GetSpellId();
							charge.ProcFlag = aura->GetSpellProto()->procFlags;
							charge.lastproc = 0;
							charge.procdiff = 0;
							u_caster->m_chargeSpells.insert(make_pair(aura->GetSpellId(), charge));
						}
					}
					u_caster->AddAura(aura);
					break;
				}
			}
		}
	}

	if(!stealedSpells.empty())
	{
		WorldPacket data(SMSG_SPELLSTEALLOG, 25 + stealedSpells.size() * 5);
		data << m_caster->GetNewGUID();
		data << unitTarget->GetNewGUID();
		data << GetProto()->Id;
		data << uint8(0);				// unused
		data << uint32(stealedSpells.size());
		for(std::list< uint32 >::iterator itr = stealedSpells.begin(); itr != stealedSpells.end(); ++itr)
		{
			data << uint32(*itr);		// stealed spell id
			data << uint8(1);			// 0 = dispelled, else cleansed
		}

		m_caster->SendMessageToSet(&data, true);
	}
}

void Spell::SpellEffectProspecting(uint32 i)
{
	if(!p_caster) return;

	if(!itemTarget) // this should never happen
	{
		SendCastResult(SPELL_FAILED_CANT_BE_PROSPECTED);
		return;
	}

	//Fill Prospecting loot
	p_caster->SetLootGUID(itemTarget->GetGUID());
	if(!itemTarget->loot)
	{
		itemTarget->loot = new Loot;
		lootmgr.FillItemLoot(itemTarget->loot , itemTarget->GetEntry());
	}

	if(itemTarget->loot->items.size() > 0)
	{
		Log.Debug("SpellEffect", "Successfully prospected item %d", uint32(itemTarget->GetEntry()));
		p_caster->SendLoot(itemTarget->GetGUID(), LOOT_PROSPECTING, p_caster->GetMapId());
	}
	else // this should never happen either
	{
		Log.Debug("SpellEffect", "Prospecting failed, item %d has no loot", uint32(itemTarget->GetEntry()));
		SendCastResult(SPELL_FAILED_CANT_BE_PROSPECTED);
	}
}

void Spell::SpellEffectApplyFriendAA(uint32 i)
{
	ApplyAA(i);
}

void Spell::SpellEffectApplyEnemyAA(uint32 i)
{
	ApplyAA(i);
}

void Spell::SpellEffectRedirectThreat(uint32 i)
{
	if(!p_caster || !unitTarget)
		return;

	if((unitTarget->IsPlayer() && p_caster->GetGroup() != TO< Player* >(unitTarget)->GetGroup()) || (unitTarget->IsCreature() && !unitTarget->IsPet()))
		return;

	p_caster->SetMisdirectionTarget(unitTarget->GetGUID());
}

void Spell::SpellEffectPlayMusic(uint32 i)
{
	uint32 soundid = m_spellInfo->EffectMiscValue[ i ];

	if(soundid == 0)
	{
		LOG_ERROR("Spell %u ( %s ) has no sound ID to play. Spell needs fixing!", m_spellInfo->Id, m_spellInfo->Name);
		return;
	}

	m_caster->PlaySoundToSet(soundid);
}

void Spell::SpellEffectForgetSpecialization(uint32 i)
{
	if(!playerTarget) return;

	uint32 spellid = GetProto()->EffectTriggerSpell[i];
	playerTarget->removeSpell(spellid, false, false, 0);

	LOG_DEBUG("Player %u have forgot spell %u from spell %u ( caster: %u)", playerTarget->GetLowGUID(), spellid, GetProto()->Id, m_caster->GetLowGUID());
}

void Spell::SpellEffectKillCredit(uint32 i)
{
	CreatureInfo* ci = CreatureNameStorage.LookupEntry(GetProto()->EffectMiscValue[i]);
	if(playerTarget != NULL && ci != NULL)
		sQuestMgr._OnPlayerKill(playerTarget, GetProto()->EffectMiscValue[i], false);
}

void Spell::SpellEffectRestorePowerPct(uint32 i)
{
	if(u_caster == NULL || unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 power_type = GetProto()->EffectMiscValue[i];
	if(power_type > POWER_TYPE_HAPPINESS)
	{
		LOG_ERROR("Unhandled power type %u in %s, report this line to devs.", power_type, __FUNCTION__);
		return;
	}

	uint32 amount = damage * unitTarget->GetMaxPower(power_type) / 100;
	u_caster->Energize(unitTarget, GetProto()->Id, amount, power_type);
}

void Spell::SpellEffectTriggerSpellWithValue(uint32 i)
{
	if(!unitTarget) return;

	SpellEntry* TriggeredSpell = dbcSpell.LookupEntryForced(GetProto()->EffectTriggerSpell[i]);
	if(TriggeredSpell == NULL)
		return;

	Spell* sp = sSpellFactoryMgr.NewSpell(m_caster, TriggeredSpell, true, NULL);

	for(uint32 x = 0; x < 3; x++)
	{
		if(i == x)
			sp->forced_basepoints[x] = damage;	//prayer of mending should inherit heal bonus ?
		else
			sp->forced_basepoints[x] = TriggeredSpell->EffectBasePoints[i];

	}

	SpellCastTargets tgt(unitTarget->GetGUID());
	sp->prepare(&tgt);
}

void Spell::SpellEffectApplyOwnerAA(uint32 i)
{
	ApplyAA(i);
}

void Spell::SpellEffectCreatePet(uint32 i)
{
	if(!playerTarget)
		return;

	if(playerTarget->GetSummon())
		playerTarget->GetSummon()->Remove(true, true);
	CreatureInfo* ci = CreatureNameStorage.LookupEntry(GetProto()->EffectMiscValue[i]);
	if(ci)
	{
		Pet* pPet = objmgr.CreatePet(GetProto()->EffectMiscValue[i]);
		if(! pPet->CreateAsSummon(GetProto()->EffectMiscValue[i], ci, NULL, playerTarget, GetProto(), 1, 0))
		{
			pPet->DeleteMe();//CreateAsSummon() returns false if an error occurred.
			pPet = NULL;
		}
	}
}

void Spell::SpellEffectTeachTaxiPath(uint32 i)
{
	if(!playerTarget || !GetProto()->EffectTriggerSpell[i])
		return;

	uint8 field = (uint8)((GetProto()->EffectTriggerSpell[i] - 1) / 32);
	uint32 submask = 1 << ((GetProto()->EffectTriggerSpell[i] - 1) % 32);

	// Check for known nodes
	if(!(playerTarget->GetTaximask(field) & submask))
	{
		playerTarget->SetTaximask(field, (submask | playerTarget->GetTaximask(field)));

		playerTarget->GetSession()->OutPacket(SMSG_NEW_TAXI_PATH);

		//Send packet
		WorldPacket update(SMSG_TAXINODE_STATUS, 9);
		update << uint64(0) << uint8(1);
		playerTarget->GetSession()->SendPacket(&update);
	}
}

void Spell::SpellEffectDualWield2H(uint32 i)
{
	if(!playerTarget)
		return;

	playerTarget->DualWield2H = true;
}

void Spell::SpellEffectEnchantItemPrismatic(uint32 i)
{
	if(!itemTarget || !p_caster)
		return;

	EnchantEntry* Enchantment = dbcEnchant.LookupEntry(m_spellInfo->EffectMiscValue[i]);

	if(!Enchantment)
	{
		LOG_ERROR("Invalid enchantment entry %u for Spell %u", GetProto()->EffectMiscValue[ i ], GetProto()->Id);
		return;
	}

	if(p_caster->GetSession()->GetPermissionCount() > 0)
		sGMLog.writefromsession(p_caster->GetSession(), "enchanted item for %s", itemTarget->GetOwner()->GetName());

	//remove other socket enchant
	itemTarget->RemoveEnchantment(6);
	int32 Slot = itemTarget->AddEnchantment(Enchantment, 0, true, true, false, 6);

	if(Slot < 6)
		return; // Apply failed

	itemTarget->m_isDirty = true;

}

void Spell::SpellEffectCreateItem2(uint32 i) // Create item
{
	//TODO: This spell effect has also a misc value - meaning is unknown yet
	if(p_caster == NULL)
		return;

	uint32 new_item_id = GetProto()->EffectItemType[i];

	if(new_item_id != 0)
	{
		// create item
		CreateItem(new_item_id);
	}
	else if(i_caster)
	{
		// provide player with item loot (clams)
		// TODO: Finish this
	}
}

void Spell::SpellEffectMilling(uint32 i)
{
	if(!p_caster) return;

	if(!itemTarget) // this should never happen
	{
		SendCastResult(SPELL_FAILED_CANT_BE_PROSPECTED);
		return;
	}

	//Fill Prospecting loot
	p_caster->SetLootGUID(itemTarget->GetGUID());
	if(!itemTarget->loot)
	{
		itemTarget->loot = new Loot;
		lootmgr.FillItemLoot(itemTarget->loot, itemTarget->GetEntry());
	}

	if(itemTarget->loot->items.size() > 0)
	{
		Log.Debug("SpellEffect", "Successfully milled item %d", uint32(itemTarget->GetEntry()));
		p_caster->SendLoot(itemTarget->GetGUID(), LOOT_MILLING, p_caster->GetMapId());
	}
	else // this should never happen either
	{
		Log.Debug("SpellEffect", "Milling failed, item %d has no loot", uint32(itemTarget->GetEntry()));
		SendCastResult(SPELL_FAILED_CANT_BE_PROSPECTED);
	}
}

void Spell::SpellEffectRenamePet(uint32 i)
{
	if(!unitTarget || !unitTarget->IsPet() ||
	        !TO< Pet* >(unitTarget)->GetPetOwner() || TO< Pet* >(unitTarget)->GetPetOwner()->getClass() != HUNTER)
		return;

	unitTarget->SetByte(UNIT_FIELD_BYTES_2, 2, PET_RENAME_ALLOWED);
}

void Spell::SpellEffectRestoreHealthPct(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 currentHealth = unitTarget->GetHealth();
	uint32 maxHealth = unitTarget->GetMaxHealth();
	uint32 modHealth = damage * maxHealth / 100;
	uint32 newHealth = modHealth + currentHealth;

	uint32 overheal = 0;
	if(newHealth >= maxHealth)
	{
		unitTarget->SetHealth(maxHealth);
		overheal = newHealth - maxHealth;
	}
	else
		unitTarget->ModHealth(modHealth);

	SendHealSpellOnPlayer(m_caster, unitTarget, modHealth, false, overheal, pSpellId ? pSpellId : GetProto()->Id);
}

void Spell::SpellEffectLearnSpec(uint32 i)
{
	if(p_caster == NULL)
		return;

	p_caster->m_talentSpecsCount = 2;
	p_caster->smsg_TalentsInfo(false);
}

void Spell::SpellEffectActivateSpec(uint32 i)
{
	if(p_caster == NULL)
		return;

	if(p_caster->CombatStatus.IsInCombat())
	{
		SendCastResult(SPELL_FAILED_AFFECTING_COMBAT);
		return;
	}
	else if(p_caster->m_bg)
	{
		uint32 Type = p_caster->m_bg->GetType();
		if(IS_ARENA(Type))
		{
			SendCastResult(SPELL_FAILED_AFFECTING_COMBAT); // does the job
			return;
		}
		else
		{
			if(p_caster->m_bg->HasStarted())
			{
				SendCastResult(SPELL_FAILED_AFFECTING_COMBAT); // does the job
			}
		}
	}

	uint8 NewSpec = p_caster->m_talentActiveSpec == SPEC_PRIMARY ? SPEC_SECONDARY : SPEC_PRIMARY; // Check if primary spec is on or not
	p_caster->ActivateSpec(NewSpec);

	WorldPacket data(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE + 1);

	data << uint8(1); // Force the client to reset the actionbar and use new values

	for(uint32 j = 0; j < PLAYER_ACTION_BUTTON_COUNT; ++j)
	{
		data << p_caster->m_specs[NewSpec].mActions[j].Action;
		data << p_caster->m_specs[NewSpec].mActions[j].Type;
		data << p_caster->m_specs[NewSpec].mActions[j].Misc;
	}

	p_caster->GetSession()->SendPacket(&data);
	p_caster->SetPower(p_caster->GetPowerType(), 0);
	p_caster->SendPowerUpdate(false);
}

void Spell::SpellEffectDurabilityDamage(uint32 i)
{
	if(!unitTarget || !unitTarget->IsPlayer())
		return;

	int16 slot = int16(GetProto()->EffectMiscValue[i]);

	Item* pItem;
	Container* pContainer;
	uint32 j, k;

	// FIXME: some spells effects have value -1/-2
	// Possibly its mean -1 all player equipped items and -2 all items
	if(slot < 0)
	{
		for(k = 0; k < MAX_INVENTORY_SLOT; k++)
		{
			pItem = p_caster->GetItemInterface()->GetInventoryItem(static_cast<uint16>(k));
			if(pItem != NULL)
			{
				if(pItem->IsContainer())
				{
					pContainer = TO< Container* >(pItem);
					for(j = 0; j < pContainer->GetProto()->ContainerSlots; ++j)
					{
						pItem = pContainer->GetItem(static_cast<uint16>(j));
						if(pItem != NULL)
						{
							uint32 maxdur = pItem->GetDurabilityMax();
							uint32 olddur = pItem->GetDurability();
							uint32 newdur = (olddur) - (damage);

							if(static_cast<int32>(newdur) < 0)
								newdur = 0;

							if(newdur > maxdur)
								newdur = maxdur;

							pItem->SetDurability(newdur);
						}
					}
				}
				else
				{
					uint32 maxdur = pItem->GetDurabilityMax();
					uint32 olddur = pItem->GetDurability();
					uint32 newdur = (olddur) - (damage);

					if(static_cast<int32>(newdur) < 0)
						newdur = 0;

					if(newdur > maxdur)
						newdur = maxdur;

					// Apply / Disapply enchantements from this item
					pItem->SetDurability(newdur);
					if(newdur == 0 && olddur > 0)
						p_caster->ApplyItemMods(pItem, static_cast<uint16>(k), false);
					else if(newdur > 0 && olddur == 0)
						p_caster->ApplyItemMods(pItem, static_cast<uint16>(k), true);
				}
			}
		}
		return;
	}

	// invalid slot value
	if(slot >= INVENTORY_SLOT_BAG_END)
		return;

	pItem = p_caster->GetItemInterface()->GetInventoryItem(slot);
	if(pItem)
	{
		uint32 maxdur = pItem->GetDurabilityMax();
		uint32 olddur = pItem->GetDurability();
		uint32 newdur = (olddur) - (damage);

		if(static_cast<int32>(newdur) < 0)
			newdur = 0;

		if(newdur > maxdur)
			newdur = maxdur;

		pItem->SetDurability(newdur);

		// Apply / Disapply enchantements from this item
		if(newdur == 0 && olddur > 0)
			p_caster->ApplyItemMods(pItem, slot, false);
		else if(newdur > 0 && olddur == 0)
			p_caster->ApplyItemMods(pItem, slot, true);
	}
}

void Spell::SpellEffectDurabilityDamagePCT(uint32 i)
{
	if(!unitTarget || !unitTarget->IsPlayer())
		return;

	int16 slot = int16(GetProto()->EffectMiscValue[i]);

	Item* pItem;
	Container* pContainer;
	uint32 j, k;

	// FIXME: some spells effects have value -1/-2
	// Possibly its mean -1 all player equipped items and -2 all items
	if(slot < 0)
	{
		for(k = 0; k < MAX_INVENTORY_SLOT; k++)
		{
			pItem = p_caster->GetItemInterface()->GetInventoryItem(static_cast<uint16>(k));
			if(pItem != NULL)
			{
				if(pItem->IsContainer())
				{
					pContainer = TO< Container* >(pItem);
					for(j = 0; j < pContainer->GetProto()->ContainerSlots; ++j)
					{
						pItem = pContainer->GetItem(static_cast<uint16>(j));
						if(pItem != NULL)
						{
							uint32 maxdur = pItem->GetDurabilityMax();
							uint32 olddur = pItem->GetDurability();
							uint32 newdur = (olddur - (uint32)(maxdur * (damage / 100.0)));

							if(static_cast<int32>(newdur) < 0)
								newdur = 0;

							if(newdur > maxdur)
								newdur = maxdur;

							pItem->SetDurability(newdur);
						}
					}
				}
				else
				{
					uint32 maxdur = pItem->GetDurabilityMax();
					uint32 olddur = pItem->GetDurability();
					uint32 newdur = (olddur - (uint32)(maxdur * (damage / 100.0)));

					if(static_cast<int32>(newdur) < 0)
						newdur = 0;

					if(newdur > maxdur)
						newdur = maxdur;

					// Apply / Disapply enchantements from this item
					pItem->SetDurability(newdur);
					if(newdur == 0 && olddur > 0)
						p_caster->ApplyItemMods(pItem, static_cast<uint16>(k), false);
					else if(newdur > 0 && olddur == 0)
						p_caster->ApplyItemMods(pItem, static_cast<uint16>(k), true);
				}
			}
		}
		return;
	}

	// invalid slot value
	if(slot >= INVENTORY_SLOT_BAG_END)
		return;

	if(damage <= 0)
		return;

	pItem = p_caster->GetItemInterface()->GetInventoryItem(slot);
	if(pItem)
	{
		uint32 maxdur = pItem->GetDurabilityMax();
		uint32 olddur = pItem->GetDurability();
		uint32 newdur = (olddur - (uint32)(maxdur * (damage / 100.0)));

		if(static_cast<int32>(newdur) < 0)
			newdur = 0;

		if(newdur > maxdur)
			newdur = maxdur;

		pItem->SetDurability(newdur);

		// Apply / Disapply enchantements from this item
		if(newdur == 0 && olddur > 0)
			p_caster->ApplyItemMods(pItem, slot, false);
		else if(newdur > 0 && olddur == 0)
			p_caster->ApplyItemMods(pItem, slot, true);
	}
}

void Spell::SpellEffectActivateRunes(uint32 i)
{
	if(p_caster == NULL || ! p_caster->IsDeathKnight())
		return;

	DeathKnight* dk = TO_DK(p_caster);

	uint32 count = damage;
	if(! count)
		count = 1;

	for(uint8 x = 0; x < MAX_RUNES && count; ++x)
	{
		if(dk->GetRuneType(x) == GetProto()->EffectMiscValue[i] && dk->GetRuneIsUsed(x))
		{
			dk->ResetRune(x);
			--count;
		}
	}
}

void Spell::SpellEffectSetMirrorName(uint32 i)
{
	WorldPacket data(SMSG_CLEAR_TARGET, 8);
	data << uint64(m_caster->GetGUID());
	m_caster->SendMessageToSet(&data, true);
}
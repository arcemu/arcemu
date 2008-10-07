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

#ifndef __SPELLSTORE_H
#define __SPELLSTORE_H

#include "Common.h"
#include "DataStore.h"
#include "Timer.h"

#pragma pack(push,1)

struct BankSlotPrice
{
	uint32 Id;
	uint32 Price;
};

struct ItemSetEntry
{
    uint32 id;                  //1
    char* name;                //2
    //uint32 unused_shit[15];      //3 - 9
    uint32 flag;                //10 constant
    uint32 itemid[8];           //11 - 18
    //uint32 more_unused_shit[9]; //19 - 27
    uint32 SpellID[8];          //28 - 35
    uint32 itemscount[8];       //36 - 43
    uint32 RequiredSkillID;     //44
    uint32 RequiredSkillAmt;    //45
};

struct Lock
{
    uint32 Id;
    uint32 locktype[5]; // 0 - no lock, 1 - item needed for lock, 2 - min lockping skill needed
    //uint32 unk1[3];
    uint32 lockmisc[5]; // if type is 1 here is a item to unlock, else is unknow for now
    //uint32 unk2[3];
    uint32 minlockskill[5]; // min skill in lockpiking to unlock.
    //uint32 unk3[11];

	/* Actually it is:
	uint32 Id;
    uint32 locktype[8];
    uint32 lockmisc[8];
    uint32 minlockskill[8];
    uint32 action[8];
	*/
};

struct emoteentry
{
    uint32 Id;
    //uint32 name;
    uint32 textid;
    uint32 textid2;
    uint32 textid3;
    uint32 textid4;
    //uint32 unk1;
    uint32 textid5;
    //uint32 unk2;
    uint32 textid6;
    //uint32 unk3;
    //uint32 unk4;
    //uint32 unk5;
    //uint32 unk6;
    //uint32 unk7;
    //uint32 unk8;
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
};

struct skilllinespell //SkillLineAbility.dbc
{
    uint32 Id;
    uint32 skilline;
    uint32 spell;
    //uint32 raceMask;
	//uint32 classMask;
    //uint32 excludeRace;
    //uint32 excludeClass;
    //uint32 minSkillLineRank;
    uint32 next;
    uint32 acquireMethod;
    uint32 grey;
    uint32 green;
    //uint32 unk10;
    //uint32 unk11;
    uint32 reqTP;
};

struct EnchantEntry
{
    uint32 Id;
    uint32 type[3];
    int32 min[3];//for compat, in practice min==max
    int32 max[3];
    uint32 spell[3];
    char* Name;
    //uint32 NameAlt1;
    //uint32 NameAlt2;
    //uint32 NameAlt3;
    //uint32 NameAlt4;
    //uint32 NameAlt5;
    //uint32 NameAlt6;
    //uint32 NameAlt7;
    //uint32 NameAlt8;
    //uint32 NameAlt9;
    //uint32 NameAlt10;
    //uint32 NameAlt11;
    //uint32 NameAlt12;
    //uint32 NameAlt13;
    //uint32 NameAlt14;
    //uint32 NameAlt15;
    //uint32 NameFlags;
    uint32 visual;
    uint32 EnchantGroups;
    uint32 GemEntry;
    uint32 unk7;//Gem Related

};

struct GemPropertyEntry{
    uint32 Entry;
    uint32 EnchantmentID;
    uint32 unk1;//bool
    uint32 unk2;//bool
    uint32 SocketMask;
};

struct skilllineentry //SkillLine.dbc
{
    uint32 id;
    uint32 type;
    uint32 skillCostsID;
    char* Name;
    //int32 NameAlt1;
    //uint32 NameAlt2;
    //uint32 NameAlt3;
    //uint32 NameAlt4;
    //uint32 NameAlt5;
    //uint32 NameAlt6;
    //uint32 NameAlt7;
    //uint32 NameAlt8;
    //uint32 NameAlt9;
    //uint32 NameAlt10;
    //uint32 NameAlt11;
    //uint32 NameAlt12;
    //uint32 NameAlt13;
    //uint32 NameAlt14;
    //uint32 NameAlt15;
    //uint32 NameFlags;
    //uint32 Description;
    //uint32 DescriptionAlt1;
    //uint32 DescriptionAlt2;
    //uint32 DescriptionAlt3;
    //uint32 DescriptionAlt4;
    //uint32 DescriptionAlt5;
    //uint32 DescriptionAlt6;
    //uint32 DescriptionAlt7;
    //uint32 DescriptionAlt8;
    //uint32 DescriptionAlt9;
    //uint32 DescriptionAlt10;
    //uint32 DescriptionAlt11;
    //uint32 DescriptionAlt12;
    //uint32 DescriptionAlt13;
    //uint32 DescriptionAlt14;
    //uint32 DescriptionAlt15;
    //uint32 DescriptionFlags;
    //uint32 unk2;

};

// Struct for the entry in Spell.dbc
struct SpellEntry
{
	uint32 Id;                              //1
	uint32 Category;                        //2
	uint32 castUI;                          //3 something like spelltype 0: general spells 1: Pet spells 2: Disguise / transormation spells 3: enchantment spells
	uint32 DispelType;                      //4
	uint32 MechanicsType;                   //5
	uint32 Attributes;                      //6
	uint32 AttributesEx;                    //7
	uint32 AttributesExB;                   //8
	uint32 AttributesExC;                   //9 // Flags to
	uint32 AttributesExD;                   //10 // Flags....
	uint32 AttributesExE;                   //11 // Flags 2.0.1 unknown one
	uint32 AttributesExF;					//12
	uint32 RequiredShapeShift;              //13 // Flags BitMask for shapeshift spells
	uint32 ShapeshiftExclude;               //14-> this is wrong // Flags BitMask for which shapeshift forms this spell can NOT be used in.
	uint32 Targets;                         //15 - N / M
	uint32 TargetCreatureType;              //16
	uint32 RequiresSpellFocus;              //17
	uint32 FacingCasterFlags;				//18
	uint32 CasterAuraState;                 //19
	uint32 TargetAuraState;                 //20
	uint32 ExcludeCasterAuraState;          //21 2.0.1 unknown two
	uint32 ExcludeTargetAuraState;          //22 2.0.1 unknown three
	uint32 CastingTimeIndex;                //23
	uint32 RecoveryTime;                    //24
	uint32 CategoryRecoveryTime;            //25 recoverytime
	uint32 InterruptFlags;                  //26
	uint32 AuraInterruptFlags;              //27
	uint32 ChannelInterruptFlags;           //28
	uint32 procFlags;                       //29
	uint32 procChance;                      //30
	int32 procCharges;                      //31
	uint32 maxLevel;                        //32
	uint32 baseLevel;                       //33
	uint32 spellLevel;                      //34
	uint32 DurationIndex;                   //35
	uint32 powerType;                       //36
	uint32 manaCost;                        //37
	uint32 manaCostPerlevel;                //38
	uint32 manaPerSecond;                   //39
	uint32 manaPerSecondPerLevel;           //40
	uint32 rangeIndex;                      //41
	float  speed;                           //42
	uint32 modalNextSpell;                  //43
	uint32 maxstack;                        //44
	uint32 Totem[2];                        //45 - 46
	uint32 Reagent[8];                      //47 - 54
	uint32 ReagentCount[8];                 //55 - 62
	int32 EquippedItemClass;                //63
	uint32 EquippedItemSubClass;            //64
	uint32 RequiredItemFlags;               //65
	uint32 Effect[3];                       //66 - 68
	uint32 EffectDieSides[3];               //69 - 71
	uint32 EffectBaseDice[3];               //72 - 74
	float  EffectDicePerLevel[3];           //75 - 77
	float  EffectRealPointsPerLevel[3];     //78 - 80
	int32  EffectBasePoints[3];             //81 - 83
	int32  EffectMechanic[3];               //84 - 86       Related to SpellMechanic.dbc
	uint32 EffectImplicitTargetA[3];        //87 - 89
	uint32 EffectImplicitTargetB[3];        //90 - 92
	uint32 EffectRadiusIndex[3];            //93 - 95
	uint32 EffectApplyAuraName[3];          //96 - 98
	uint32 EffectAmplitude[3];              //99 - 101
	float  Effectunknown[3];                //102 - 104     This value is the $ value from description
	uint32 EffectChainTarget[3];            //105 - 107
	uint32 EffectSpellGroupRelation[3];     //108 - 110     Not sure maybe we should rename it. its the relation to field: SpellGroupType
	uint32 EffectMiscValue[3];              //111 - 113
	uint32 EffectMiscValueB[3];             //114 - 116  2.4.3
	uint32 EffectTriggerSpell[3];           //117 - 119
	float  EffectPointsPerComboPoint[3];    //120 - 122
	uint32 SpellVisual;                     //123
	uint32 field114;                        //124
	uint32 spellIconID;                     //125
	uint32 activeIconID;                    //126   activeIconID;
	uint32 spellPriority;                   //127
	char* Name;                             //128
	//uint32 NameAlt1;                        //129
	//uint32 NameAlt2;                        //130
	//uint32 NameAlt3;                        //131
	//uint32 NameAlt4;                        //132
	//uint32 NameAlt5;                        //133
	//uint32 NameAlt6;                        //134
	//uint32 NameAlt7;                        //135
	//uint32 NameAlt8;                        //136
	//uint32 NameAlt9;                        //137
	//uint32 NameAlt10;                       //138
	//uint32 NameAlt11;                       //139
	//uint32 NameAlt12;                       //140
	//uint32 NameAlt13;                       //141
	//uint32 NameAlt14;                       //142
	//uint32 NameAlt15;                       //143
	//uint32 NameFlags;                       //144
	char * Rank;                            //145
	//uint32 RankAlt1;                        //146
	//uint32 RankAlt2;                        //147
	//uint32 RankAlt3;                        //148
	//uint32 RankAlt4;                        //149
	//uint32 RankAlt5;                        //150
	//uint32 RankAlt6;                        //151
	//uint32 RankAlt7;                        //152
	//uint32 RankAlt8;                        //153
	//uint32 RankAlt9;                        //154
	//uint32 RankAlt10;                       //155
	//uint32 RankAlt11;                       //156
	//uint32 RankAlt12;                       //157
	//uint32 RankAlt13;                       //158
	//uint32 RankAlt14;                       //159
	//uint32 RankAlt15;                       //160
	//uint32 RankFlags;                       //161
	char * Description;                     //162
	//uint32 DescriptionAlt1;                 //163
	//uint32 DescriptionAlt2;                 //164
	//uint32 DescriptionAlt3;                 //165
	//uint32 DescriptionAlt4;                 //166
	//uint32 DescriptionAlt5;                 //167
	//uint32 DescriptionAlt6;                 //168
	//uint32 DescriptionAlt7;                 //169
	//uint32 DescriptionAlt8;                 //170
	//uint32 DescriptionAlt9;                 //171
	//uint32 DescriptionAlt10;                //172
	//uint32 DescriptionAlt11;                //173
	//uint32 DescriptionAlt12;                //174
	//uint32 DescriptionAlt13;                //175
	//uint32 DescriptionAlt14;                //176
	//uint32 DescriptionAlt15;                //177
	//uint32 DescriptionFlags;                //178
	char * BuffDescription;                 //179
	//uint32 BuffDescriptionAlt1;             //180
	//uint32 BuffDescriptionAlt2;             //181
	//uint32 BuffDescriptionAlt3;             //182
	//uint32 BuffDescriptionAlt4;             //183
	//uint32 BuffDescriptionAlt5;             //184
	//uint32 BuffDescriptionAlt6;             //185
	//uint32 BuffDescriptionAlt7;             //186
	//uint32 BuffDescriptionAlt8;             //187
	//uint32 BuffDescriptionAlt9;             //188
	//uint32 BuffDescriptionAlt10;            //189
	//uint32 BuffDescriptionAlt11;            //190
	//uint32 BuffDescriptionAlt12;            //191
	//uint32 BuffDescriptionAlt13;            //192
	//uint32 BuffDescriptionAlt14;            //193
	//uint32 BuffDescriptionAlt15;            //194
	//uint32 buffdescflags;                   //195
	uint32 ManaCostPercentage;              //196
	uint32 unkflags;                        //197 
	uint32 StartRecoveryTime;               //198
	uint32 StartRecoveryCategory;           //199
//	uint32 MaxTargetLevel						from 3.0.1
	uint32 SpellFamilyName;                 //200
	uint64 SpellGroupType;					//201+202
	uint32 MaxTargets;                      //203 
	uint32 Spell_Dmg_Type;                  //204   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 PreventionType;                  //205   0,1,2 related to Spell_Dmg_Type I think
	int32 StanceBarOrder;                   //206   related to paladin aura's 
	float dmg_multiplier[3];                //207 - 208   if the name is correct I dono
	uint32 MinFactionID;                    //209   only one spellid:6994 has this value = 369 UNUSED
	uint32 MinReputation;                   //210   only one spellid:6994 has this value = 4 UNUSED
	uint32 RequiredAuraVision;              //211  3 spells 1 or 2   
	uint32 TotemCategory[2];				//212-213
	uint32 RequiresAreaId;					//214 
	uint32 School;							//215
//	uint32 RuneCostID;						//216 from 3.0.1
//	uint32 SpellMissileID;					//217 from 3.0.1
    /// CUSTOM: these fields are used for the modifications made in the world.cpp
    uint32 DiminishStatus;                  //
    uint32 proc_interval;                   //!!! CUSTOM, <Fill description for variable>
	//Buff Groupin Rule -> caster can cast this spell only on 1 target. Value represents the group spell is part of. Can be part of only 1 group
	//caster can only cast on 1 target these spells
    uint32 BGR_one_buff_from_caster_on_1target;		//!!! CUSTOM, <Fill description for variable>
	//target can have only buff of this type on self. Value represents the group spell is part of. Can be part of only 1 group
    uint32 BGR_one_buff_on_target;          //!!! CUSTOM, these are related to creating a item through a spell
	//caster can have only 1 Aura per spell group, ex pal auras
    uint32 BGR_one_buff_from_caster_on_self;        //!!! CUSTOM, these are related to creating a item through a spell
//    uint32 buffIndexType;					//!!! CUSTOM, <Fill description for variable>
    uint32 c_is_flags;						//!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
//    uint32 buffType;                        //!!! CUSTOM, these are related to creating a item through a spell
    uint32 RankNumber;                      //!!! CUSTOM, this protects players from having >1 rank of a spell
    uint32 NameHash;                        //!!! CUSTOM, related to custom spells, summon spell quest related spells
	uint32 talent_tree;						//!!! CUSTOM,
    uint32 EffectSpellGroupRelation_high[3];     //!!! this is not contained in client dbc but server must have it
	uint32 ThreatForSpell;
	float ThreatForSpellCoef;
	uint32 ProcOnNameHash[3];
	uint32 spell_coef_flags;                                //!!! CUSTOM, store flags for spell coefficient calculations

	float base_range_or_radius_sqr;         //!!! CUSTOM, needed for aoe spells most of the time
	// love me or hate me, all "In a cone in front of the caster" spells don't necessarily mean "in front"
	float cone_width;
	//Spell Coefficient
	float casttime_coef;                                    //!!! CUSTOM, faster spell bonus calculation
	float fixed_dddhcoef;                                   //!!! CUSTOM, fixed DD-DH coefficient for some spells
	float fixed_hotdotcoef;                                 //!!! CUSTOM, fixed HOT-DOT coefficient for some spells
	float Dspell_coef_override;                             //!!! CUSTOM, overrides any spell coefficient calculation and use this value in DD&DH
	float OTspell_coef_override;							//!!! CUSTOM, overrides any spell coefficient calculation and use this value in HOT&DOT
	int	ai_target_type;

	bool self_cast_only;
	bool apply_on_shapeshift_change;
	bool always_apply;
	bool is_melee_spell;					//!!! CUSTOM,
	bool is_ranged_spell;					//!!! CUSTOM,
	bool spell_can_crit;					//!!! CUSTOM,
	bool noproc;

	uint32 SchoolMask;						// Custom
/*
//3.0.1 client column namings
m_id
m_category
m_castUI
m_dispelType
m_mechanic
m_attributes
m_attributesEx
m_attributesExB
m_attributesExC
m_attributesExD
m_attributesExE
m_attributesExF
m_shapeshiftMask
m_shapeshiftExclude
m_targets
m_targetCreatureType
m_requiresSpellFocus
m_facingCasterFlags
m_casterAuraState
m_targetAuraState
m_excludeCasterAuraState
m_excludeTargetAuraState
m_castingTimeIndex
m_recoveryTime
m_categoryRecoveryTime
m_interruptFlags
m_auraInterruptFlags
m_channelInterruptFlags
m_procTypeMask
m_procChance
m_procCharges
m_maxLevel
m_baseLevel
m_spellLevel
m_durationIndex
m_powerType
m_manaCost
m_manaCostPerLevel
m_manaPerSecond
m_manaPerSecondPerLevel
m_rangeIndex
m_speed
m_modalNextSpell
m_cumulativeAura
m_totem
m_reagent
m_reagentCount
m_equippedItemClass
m_equippedItemSubclass
m_equippedItemInvTypes
m_effect
m_effectDieSides
m_effectBaseDice
m_effectDicePerLevel
m_effectRealPointsPerLevel
m_effectBasePoints
m_effectMechanic
m_implicitTargetA
m_implicitTargetB
m_effectRadiusIndex
m_effectAura
m_effectAuraPeriod
m_effectAmplitude
m_effectChainTargets
m_effectItemType
m_effectMiscValue
m_effectMiscValueB
m_effectTriggerSpell
m_effectPointsPerCombo
m_spellVisualID
m_spellIconID
m_activeIconID
m_spellPriority
m_name_lang
m_nameSubtext_lang
m_description_lang
m_auraDescription_lang
m_manaCostPct
m_startRecoveryCategory
m_startRecoveryTime
m_maxTargetLevel
m_spellClassSet
m_spellClassMask
m_maxTargets
m_defenseType
m_preventionType
m_stanceBarOrder
m_effectChainAmplitude
m_minFactionID
m_minReputation
m_requiredAuraVision
m_requiredTotemCategoryID
m_requiredAreaID
m_schoolMask
m_runeCostID
m_spellMissileID
*/
};

struct ItemExtendedCostEntry
{
    uint32 costid;
    uint32 honor;
    uint32 arena;
    uint32 item[5];
    uint32 count[5];
	uint32 personalrating;
};

struct TalentEntry
{
    uint32  TalentID;
    uint32  TalentTree;
    uint32  Row;
    uint32  Col;
    uint32  RankID[5];
    //uint32  unk[4];
    uint32  DependsOn;
    //uint32  unk1[2];
    uint32  DependsOnRank;
    //uint32  unk2[4];
};

struct TalentTabEntry
{
    uint32	TalentTabID;
    //char*	Name;
	//unit32  unk3
	//unit32  unk4
	//unit32  unk5
	//unit32  unk6
	//unit32  unk7
	//unit32  unk8
	//unit32  unk9
	//unit32  unk10
	//unit32  unk11
	//unit32  unk12
	//unit32  unk13
	//unit32  unk14
	//unit32  unk15
	//unit32  unk16
	//unit32  unk17
	//unit32  unk18
	//unit32  unk19
	//unit32  unk20
    uint32	ClassMask;
    uint32	TabPage;
    //char*	InternalName;
};

struct Trainerspell
{
    uint32 Id;
    uint32 skilline1;
    uint32 skilline2;
    uint32 skilline3;
    uint32 maxlvl;
    uint32 charclass;
};

struct SpellCastTime
{
    uint32 ID;
    uint32 CastTime;
    //uint32 unk1;
    //uint32 unk2;
};

struct SpellRadius
{
    uint32 ID;
    float Radius;
    //float unk1;
    float Radius2;
};

struct SpellRange
{
    uint32 ID;
    float minRange;
    float maxRange;
    //uint32 unks[35];
};

struct SpellDuration
{
    uint32 ID;
    uint32 Duration1;
    uint32 Duration2;
    uint32 Duration3;
};

struct RandomProps
{
    uint32 ID;
    //uint32 name1;
    uint32 spells[3];
    //uint32 unk1;
    //uint32 unk2;
    //uint32 name2;
    //uint32 RankAlt1;
    //uint32 RankAlt2;
    //uint32 RankAlt3;
    //uint32 RankAlt4;
    //uint32 RankAlt5;
    //uint32 RankAlt6;
    //uint32 RankAlt7;
    //uint32 RankAlt8;
    //uint32 RankAlt9;
    //uint32 RankAlt10;
    //uint32 RankAlt11;
    //uint32 RankAlt12;
    //uint32 RankAlt13;
    //uint32 RankAlt14;
    //uint32 RankAlt15;
    //uint32 RankFlags;

};

struct AreaTable
{
    uint32 AreaId;
    uint32 mapId;
    uint32 ZoneId;
    uint32 explorationFlag;
    uint32 AreaFlags;
    //uint32 unk2;
    //uint32 unk3;
    //uint32 unk4;
    uint32 EXP;//not XP
    //uint32 unk5;
    uint32 level;
    char* name;
    //uint32 nameAlt1;
    //uint32 nameAlt2;
    //uint32 nameAlt3;
    //uint32 nameAlt4;
    //uint32 nameAlt5;
    //uint32 nameAlt6;
    //uint32 nameAlt7;
    //uint32 nameAlt8;
    //uint32 nameAlt9;
    //uint32 nameAlt10;
    //uint32 nameAlt11;
    //uint32 nameAlt12;
    //uint32 nameAlt13;
    //uint32 nameAlt14;
    //uint32 nameAlt15;
    //uint32 nameFlags;
    uint32 category;
    //uint32 unk7;
    //uint32 unk8;
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
    //uint32 unk12;
};

struct FactionTemplateDBC
{
    uint32 ID;
    uint32 Faction;
    uint32 FactionGroup;
    uint32 Mask;
    uint32 FriendlyMask;
    uint32 HostileMask;
    uint32 EnemyFactions[4];
    uint32 FriendlyFactions[4];
};

struct AuctionHouseDBC
{
    uint32 id;
    uint32 unk;
    uint32 fee;
    uint32 tax;
    //char* name;
    //char* nameAlt1;
    //char* nameAlt2;
    //char* nameAlt3;
    //char* nameAlt4;
    //char* nameAlt5;
    //char* nameAlt6;
    //char* nameAlt7;
    //char* nameAlt8;
    //char* nameAlt9;
    //char* nameAlt10;
    //char* nameAlt11;
    //char* nameAlt12;
    //char* nameAlt13;
    //char* nameAlt14;
    //char* nameAlt15;
    //char* nameFlags;
};

struct FactionDBC
{
    uint32 ID;
    int32 RepListId;
    uint32 RaceMask[4];
    uint32 ClassMask[4];
    int32 baseRepValue[4];
    uint32 repFlags[4];
    uint32 parentFaction;
    char* Name;
    //uint32 shit[16];
    //uint32 Description;
    //uint32 shit2[16];
};

struct DBCTaxiNode
{
    uint32 id;
    uint32 mapid;
    float x;
    float y;
    float z;
    //uint32 name;
    //uint32 namealt1;
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    uint32 horde_mount;
    uint32 alliance_mount;
};

struct DBCTaxiPath
{
    uint32 id;
    uint32 from;
    uint32 to;
    uint32 price;
};

struct DBCTaxiPathNode
{
    uint32 id;
    uint32 path;
    uint32 seq;
    uint32 mapid;
    float x;
    float y;
    float z;
    uint32 unk1;
    uint32 waittime;
    //uint32 unk2;
    //uint32 unk3;
};

struct CreatureSpellDataEntry
{
    uint32 id;
    uint32 Spells[3];
    uint32 PHSpell;
    uint32 Cooldowns[3];
    uint32 PH;
};

struct CharRaceEntry
{
	uint32 race_id;
	uint32 team_id;
	uint32 cinematic_id;
	char* name1;
};

struct CharClassEntry
{
    uint32 class_id;
    //uint32 unk1;
    uint32 power_type;
    //uint32 unk2;
    char* name;
    //uint32 namealt1;
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    //uint32 unk3;
    //uint32 unk4;
    //uint32 unk5;
};

struct CreatureFamilyEntry
{
    uint32 ID;
    float minsize;
    uint32 minlevel;
    float maxsize;
    uint32 maxlevel;
    uint32 skilline;
    uint32 tameable;		//second skill line - 270 Generic
    uint32 petdietflags;
    char* name;
    //uint32 namealt1;
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    //uint32 iconFile;
};

struct MapEntry
{
    uint32 id;
    char* name_internal;
    uint32 map_type;
    //uint32 unk;
    char* real_name;
    //uint32 unk_1;
    //uint32 unk1;
    //uint32 unk2;
    //uint32 unk3;
    //uint32 unk4;
    //uint32 unk5;
    //uint32 unk6;
    //uint32 unk7;
    //uint32 unk8;
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
    //uint32 unk12;
    //uint32 unk13;
    //uint32 unk14;
    //uint32 unk15;
    //uint32 unk16;
    //uint32 unk17;
    //uint32 unk18;
    //uint32 unk19;
    //uint32 unk20;
    //uint32 unk21;
    //uint32 unk22;
    //uint32 unk23;
    //uint32 unk24;
    //uint32 unk25;
    //uint32 unk26;
    //uint32 unk27;
    //uint32 unk28;
    //uint32 unk29;
    //uint32 unk30;
    //uint32 unk31;
    //uint32 unk32;
    //uint32 unk33;
    //uint32 unk34;
    //uint32 unk35;
    //uint32 unk36;
    //uint32 unk37;
    //uint32 unk38;
    //uint32 unk39;
    //uint32 unk40;
    //uint32 unk41;
    //uint32 unk42;
    //uint32 unk43;
    //uint32 unk44;
    //uint32 unk45;
    //uint32 unk46;
    //uint32 unk47;
    //uint32 unk48;
    //uint32 unk49;
    //uint32 unk50;
    //uint32 unk51;
    //uint32 unk52;
    //uint32 unk53;
    //uint32 unk54;
    //uint32 unk55;
    //uint32 unk56;
    //uint32 unk57;
    //uint32 unk58;
    //uint32 unk59;
    //uint32 unk60;
    //uint32 unk61;
    //uint32 unk62;
    //uint32 unk63;
    //uint32 unk64;
    //uint32 unk65;
    //uint32 unk66;
    //uint32 unk67;
    //uint32 unk68;
    //uint32 unk69;
    //uint32 aunk1;
    //uint32 aunk2;
    //uint32 aunk3;
    //uint32 aunk4;
    //uint32 aunk5;
    //uint32 aunk6;
    //uint32 aunk7;
    //uint32 aunk8;
    //uint32 aunk9;
    //uint32 aunk10;
    //uint32 aunk11;
    //uint32 aunk12;
    //uint32 aunk13;
    //uint32 aunk14;
    //uint32 aunk15;
    //uint32 aunk16;
    //uint32 aunk17;
    //uint32 aunk18;
    //uint32 aunk19;
    //uint32 aunk20;
    //uint32 aunk21;
    //uint32 aunk22;
    //uint32 aunk23;
    //uint32 aunk24;
    //uint32 bunk1;
    //uint32 bunk2;
    //uint32 bunk3;
    //uint32 bunk4;
    //uint32 bunk5;
    //uint32 bunk6;
    //uint32 bunk7;
    //uint32 bunk8;
    //uint32 bunk9;
    //uint32 bunk10;
    //uint32 bunk11;
    //uint32 bunk12;
    //uint32 bunk13;
    //uint32 bunk14;
    //uint32 bunk15;
    //uint32 bunk16;
    //uint32 bunk17;
    //uint32 bunk18;
    //uint32 bunk19;
    //uint32 bunk20;
    //uint32 bunk21;
    //uint32 bunk22;
    //uint32 bunk23;
    //uint32 bunk24;
};

struct ItemRandomSuffixEntry
{
	uint32 id;
	uint32 enchantments[3];
	uint32 prefixes[3];
};

struct gtFloat
{
	float val;
};

struct CombatRatingDBC
{
	float val;
};

struct ChatChannelDBC
{
	uint32 id;
	uint32 flags;
};

struct DurabilityQualityEntry
{
    uint32 id;
    float quality_modifier;
};

struct DurabilityCostsEntry
{
    uint32 itemlevel;
    uint32 modifier[29];
};

struct AreaTriggerEntry
{
    uint32    id;				// 0
    uint32    mapid;			// 1
    float     x;				// 2
    float     y;				// 3
    float     z;				// 4
    float     o;				// 5
    float     box_x;			// 6 extent x edge
    float     box_y;			// 7 extent y edge
    float     box_z;			// 8 extent z edge
    float     box_o;			// 9 extent rotation by about z axis
};

/*struct CharTitlesEntry
{
	uint32		ID;
	char*		name;										// Field [2], Name
	uint32		bit_index;									// Field [36], used in PLAYER_CHOSEN_TITLE and (uint32(1) << bit_index) in PLAYER_FIELD_KNOWN_TITLES and PLAYER_FIELD_KNOWN_TITLES_X
};*/

#pragma pack(pop)

ARCEMU_INLINE float GetRadius(SpellRadius *radius)
{
    return radius->Radius;
}
ARCEMU_INLINE uint32 GetCastTime(SpellCastTime *time)
{
    return time->CastTime;
}
ARCEMU_INLINE float GetMaxRange(SpellRange *range)
{
    return range->maxRange;
}
ARCEMU_INLINE float GetMinRange(SpellRange *range)
{
    return range->minRange;
}
ARCEMU_INLINE uint32 GetDuration(SpellDuration *dur)
{
    return dur->Duration1;
}

#define SAFE_DBC_CODE_RETURNS			/* undefine this to make out of range/nulls return null. */

template<class T>
class SERVER_DECL DBCStorage
{
	T * m_heapBlock;
	T * m_firstEntry;

	T ** m_entries;
	uint32 m_max;
	uint32 m_numrows;
	uint32 m_stringlength;
	char * m_stringData;

	uint32 rows;
	uint32 cols;
	uint32 useless_shit;
	uint32 header;

public:
	
	DBCStorage()
	{
		m_heapBlock = NULL;
		m_entries = NULL;
		m_firstEntry = NULL;
		m_max = 0;
		m_numrows = 0;
		m_stringlength=0;
		m_stringData = NULL;
	}

	~DBCStorage()
	{
		if(m_heapBlock)
			free(m_heapBlock);
		if(m_entries)
			free(m_entries);
		if( m_stringData != NULL )
			free(m_stringData);
		// points done
		m_heapBlock = NULL;
		m_entries = NULL;
		m_firstEntry = NULL;
		m_stringData = NULL;
	}

	bool Load(const char * filename, const char * format, bool load_indexed, bool load_strings)
	{
		uint32 i;
		uint32 string_length;
		long pos;

		FILE * f = fopen(filename, "rb");
		if(f == NULL)
			return false;

		/* read the number of rows, and allocate our block on the heap */
		fread(&header,4,1,f);
		fread(&rows, 4, 1, f);
		fread(&cols, 4, 1, f);
		fread(&useless_shit, 4, 1, f);
		fread(&string_length, 4, 1, f);
		pos = ftell(f);

#ifdef USING_BIG_ENDIAN
		swap32(&rows); swap32(&cols); swap32(&useless_shit); swap32(&string_length);
#endif

		if( load_strings )
		{
			fseek( f, 20 + ( rows * cols * 4 ), SEEK_SET );
			m_stringData = (char*)malloc(string_length);
			m_stringlength = string_length;
			if (m_stringData)
				fread( m_stringData, string_length, 1, f );
		}

		fseek(f, pos, SEEK_SET);

		m_heapBlock = (T*)malloc(rows * sizeof(T));
		ASSERT(m_heapBlock);

		/* read the data for each row */
		for(i = 0; i < rows; ++i)
		{
			memset(&m_heapBlock[i], 0, sizeof(T));
			ReadEntry(f, &m_heapBlock[i], format, cols, filename);

			if(load_indexed)
			{
				/* all the time the first field in the dbc is our unique entry */
				if(*(uint32*)&m_heapBlock[i] > m_max)
					m_max = *(uint32*)&m_heapBlock[i];
			}
		}

		if(load_indexed)
		{
			m_entries = (T**)malloc(sizeof(T*) * (m_max+1));
			ASSERT(m_entries);

			memset(m_entries, 0, (sizeof(T*) * (m_max+1)));
			for(i = 0; i < rows; ++i)
			{
				if(m_firstEntry == NULL)
					m_firstEntry = &m_heapBlock[i];

				m_entries[*(uint32*)&m_heapBlock[i]] = &m_heapBlock[i];
			}
		}

		m_numrows = rows;

		fclose(f);
		return true;
	}

	void ReadEntry(FILE * f, T * dest, const char * format, uint32 cols, const char * filename)
	{
		const char * t = format;
		uint32 * dest_ptr = (uint32*)dest;
		uint32 c = 0;
		uint32 val;
		size_t len = strlen(format);
		if(len!= cols)
			printf("!!! possible invalid format in file %s (us: %zd, them: %u)\n", filename, len, cols);

		while(*t != 0)
		{
			if((++c) > cols)
			{
				++t;
				printf("!!! Read buffer overflow in DBC reading of file %s\n", filename);
				continue;
			}

			fread(&val, 4, 1, f);
			if(*t == 'x')
			{
				++t;
				continue;		// skip!
			}
#ifdef USING_BIG_ENDIAN
			swap32(&val);
#endif
			if(*t == 's')
			{
				char ** new_ptr = (char**)dest_ptr;
				static const char * null_str = "";
				char * ptr;
				if( val < m_stringlength )
					ptr = m_stringData + val;
				else
					ptr = (char*)null_str;

				*new_ptr = ptr;
				new_ptr++;
				dest_ptr = (uint32*)new_ptr;
			}
			else
			{
				*dest_ptr = val;
				dest_ptr++;
			}

			++t;
		}
	}

	ARCEMU_INLINE uint32 GetNumRows()
	{
		return m_numrows;
	}

	T * LookupEntryForced(uint32 i)
	{
#if 0
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
			{
				printf("LookupEntryForced failed for entry %u\n", i);
				return NULL;
			}
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
#else
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
#endif
	}

	T * CreateCopy(T * obj)
	{
		T * oCopy = (T*)malloc(sizeof(T));
		ASSERT(oCopy);
		memcpy(oCopy,obj,sizeof(T));
		return oCopy;
	}
	void SetRow(uint32 i, T * t)
	{
		if(i < m_max && m_entries)
			m_entries[i] = t;
	}

#ifdef SAFE_DBC_CODE_RETURNS
	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return m_firstEntry;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return &m_heapBlock[0];
			else
				return &m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return &m_heapBlock[0];
		else
			return &m_heapBlock[i];
	}

#else

	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return NULL;
		else
			return m_heapBlock[i];
	}

#endif
};

extern SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
extern SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
extern SERVER_DECL DBCStorage<Lock> dbcLock;
extern SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
extern SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
extern SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
extern SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
extern SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
extern SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
extern SERVER_DECL DBCStorage<AreaTable> dbcArea;
extern SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
extern SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
extern SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
extern SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
extern SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
extern SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
extern SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
extern SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
extern SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
extern SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
extern SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
extern SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
extern SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
extern SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
extern SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
extern SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
extern SERVER_DECL DBCStorage<MapEntry> dbcMap;
extern SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
extern SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
extern SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
extern SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
extern SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
extern SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices; //uses same structure as Bank
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
extern SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
//extern SERVER_DECL DBCStorage<CharTitlesEntry> dbcCharTitle; // not used yet

bool LoadDBCs();

#endif

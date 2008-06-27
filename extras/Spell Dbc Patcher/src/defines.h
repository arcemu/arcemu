//sadly we cannot simply import files from ascent so we copy paste stuff we need here

#ifndef uint32
	typedef unsigned int uint32;
#endif
#ifndef int
	typedef int int32;
#endif

#ifndef ASSERT
	#define ASSERT assert
#endif

#define SERVER_DECL __declspec(dllexport)

const char * spellentrYFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffffffiiiiiiuuuuuuuuuuuuuuufffuuuuuuuuuuuufffuuuuuxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuuuuuuuuuifffuuuuuu";
/*
uuuuuuuuuu
uuuuuuuuuu
uuuuuuuuui
uuuuuuuuuu
fuuuuuuuuu
uuuuuuuuuu
uuuuuuuuuu
uuuffffffi
iiiiiuuuuu
uuuuuuuuuu
fffuuuuuuu
uuuuufffuu
uuuxxxxxxx
xxxxxxxxxx - 140
xxxxxxxxxx
xxxxxxxxxx
xxxxxxxxxx
xxxxxxxxxx
xxxxxxxxxx
xuuuuuuuuu
uifffuuuuu
u */

#define SPELL_DAMAGE_TYPE_MAGIC 1
#define SPELL_CATING_TIME_INSTANT 1

#define RANGEINDEX_10_YARDS 7
#define RANGEINDEX_20_YARDS 3
#define RANGEINDEX_30_YARDS 4
#define RANGEINDEX_40_YARDS 5
#define RANGEINDEX_SELF	1

#define DURATIONINDEX_1SEC	36
#define DURATIONINDEX_2SEC	39
#define DURATIONINDEX_3SEC	27
#define DURATIONINDEX_4SEC	35
#define DURATIONINDEX_5SEC	28
#define DURATIONINDEX_6SEC	32
#define DURATIONINDEX_8SEC	31
#define DURATIONINDEX_10SEC	1
#define DURATIONINDEX_15SEC	8
#define DURATIONINDEX_20SEC	18
#define DURATIONINDEX_25SEC	63
#define DURATIONINDEX_30SEC	9
#define DURATIONINDEX_35SEC	125
#define DURATIONINDEX_40SEC	64
#define DURATIONINDEX_45SEC	22
#define DURATIONINDEX_50SEC	245
#define DURATIONINDEX_60SEC	3
#define DURATIONINDEX_2MIN	4
#define DURATIONINDEX_5MIN	5
#define DURATIONINDEX_30MIN	30
#define DURATIONINDEX_INFINIT	21
#define DURATIONINDEX_NONE 0

#define RADIUSINDEX_10YARDS 13
#define RADIUSINDEX_20YARDS 9
#define RADIUSINDEX_30YARDS 10
#define RADIUSINDEX_40YARDS 23

enum SPELL_CLIENT_TARGETING_TYPE
{
	SPELL_CLIENT_TARGETTING_LOCATION = 64,
};

enum SPELL_ATTRIBUTESEX_FLAGS
{
	SPELL_ATTRIBUTESEX_FLAG_CHANNELED = 4,
};

#define SPELL_DBC_CLIENT_NUMBER "2.3.3"
//this might change from 1 version to another of the DBC
#define SPELL_COLUMN_COUNT 211

#define SQL_INSERTS_PER_QUERY 200

//soundEntry.dbc
struct SoundEntrys
{
	uint32	id;
	uint32	unk1;	//maybe priority or some grouping ? 1 = wav and 28 = mp3 ?
	uint32	internal_name;
	uint32	soundfilename1;
	uint32	soundfilename2;
	uint32	soundfilename3;
	uint32	soundfilename4;
	uint32	soundfilename5;
	uint32	soundfilename6;
	uint32	soundfilename7;
	uint32	soundfilename8;
	uint32	soundfilename9;
	uint32	soundfilename10;
	uint32	sound1repeat;
	uint32	sound2repeat;
	uint32	sound3repeat;
	uint32	sound4repeat;
	uint32	sound5repeat;
	uint32	sound6repeat;
	uint32	sound7repeat;
	uint32	sound8repeat;
	uint32	sound9repeat;
	uint32	sound10repeat;
	uint32	path; // without filename
	float	playspeed; //not sure
	uint32	flags;//not sure
	float	unk2; //seems to be 7 most of the time
	float	unk3; //seems to be 30 most of the time 
	uint32	unk4; //seems to be 2 for non tbc ?
};

//iconNames.dbc
struct IconNames 
{
    uint32 Id;
    uint32 iconpath;		//this is string
};

//SkillLineAbility.dbc
struct skilllinespell 
{
    uint32 Id;
    uint32 skilline;
    uint32 spell;
    uint32 unk1;
    uint32 unk2;
    uint32 unk3;
    uint32 unk4;
    uint32 unk5;
    uint32 next;
    uint32 minrank;
    uint32 grey;
    uint32 green;
    uint32 unk10;
    uint32 unk11;
    uint32 reqTP;
};

// Struct for 1 entry in Spell.dbc
// this must be a direct mapping of the original dbc entry. (no more or less values)
struct SpellEntry
{
	uint32 Id;                              //1
	uint32 Category;                        //2
	uint32 field4;                          //3 something like spelltype 0: general spells 1: Pet spells 2: Disguise / transormation spells 3: enchantment spells
	uint32 DispelType;                      //4
	uint32 MechanicsType;                   //5
	uint32 Attributes;                      //6
	uint32 AttributesEx;                    //7
	uint32 Flags3;                          //8
	uint32 Flags4;                          //9 // Flags to
	uint32 Flags5;                          //10 // Flags....
	uint32 unk201_1;                        //11 // Flags 2.0.1 unknown one
	uint32 unk240_1;						//12
	uint32 RequiredShapeShift;              //13 // Flags BitMask for shapeshift spells
	uint32 UNK14;                           //14-> this is wrong // Flags BitMask for which shapeshift forms this spell can NOT be used in.
	uint32 Targets;                         //15 - N / M
	uint32 TargetCreatureType;              //16
	uint32 RequiresSpellFocus;              //17
	uint32 unk240_2;						//18
	uint32 CasterAuraState;                 //19
	uint32 TargetAuraState;                 //20
	uint32 unk201_2;                        //21 2.0.1 unknown two
	uint32 unk201_3;                        //22 2.0.1 unknown three
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
	int32 EquippedItemClass;               //63
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
	uint32 EffectTriggerSpell[3];           //114 - 116
	float  EffectPointsPerComboPoint[3];    //117 - 117
	uint32 SpellVisual;                     //120
	uint32 field114;                        //121
	uint32 icon;                           //122
	uint32 CoSpell;                         //123   activeIconID;
	uint32 spellPriority;                   //124
	uint32 Name;                             //125
	uint32 NameAlt1;                        //126
	uint32 NameAlt2;                        //127
	uint32 NameAlt3;                        //128
	uint32 NameAlt4;                        //129
	uint32 NameAlt5;                        //130
	uint32 NameAlt6;                        //131
	uint32 NameAlt7;                        //132
	uint32 NameAlt8;                        //133
	uint32 NameAlt9;                        //134
	uint32 NameAlt10;                       //135
	uint32 NameAlt11;                       //136
	uint32 NameAlt12;                       //137
	uint32 NameAlt13;                       //138
	uint32 NameAlt14;                       //139
	uint32 NameAlt15;                       //140
	uint32 NameFlags;                       //141
	uint32 Rank;                            //142
	uint32 RankAlt1;                        //143
	uint32 RankAlt2;                        //144
	uint32 RankAlt3;                        //145
	uint32 RankAlt4;                        //146
	uint32 RankAlt5;                        //147
	uint32 RankAlt6;                        //148
	uint32 RankAlt7;                        //149
	uint32 RankAlt8;                        //150
	uint32 RankAlt9;                        //151
	uint32 RankAlt10;                       //152
	uint32 RankAlt11;                       //153
	uint32 RankAlt12;                       //154
	uint32 RankAlt13;                       //155
	uint32 RankAlt14;                       //156
	uint32 RankAlt15;                       //157
	uint32 RankFlags;                       //158
	uint32 Description;                     //159
	uint32 DescriptionAlt1;                 //160
	uint32 DescriptionAlt2;                 //161
	uint32 DescriptionAlt3;                 //162
	uint32 DescriptionAlt4;                 //163
	uint32 DescriptionAlt5;                 //164
	uint32 DescriptionAlt6;                 //165
	uint32 DescriptionAlt7;                 //166
	uint32 DescriptionAlt8;                 //167
	uint32 DescriptionAlt9;                 //168
	uint32 DescriptionAlt10;                //169
	uint32 DescriptionAlt11;                //170
	uint32 DescriptionAlt12;                //171
	uint32 DescriptionAlt13;                //172
	uint32 DescriptionAlt14;                //173
	uint32 DescriptionAlt15;                //174
	uint32 DescriptionFlags;                //175
	uint32 BuffDescription;                 //176
	uint32 BuffDescriptionAlt1;             //177
	uint32 BuffDescriptionAlt2;             //178
	uint32 BuffDescriptionAlt3;             //179
	uint32 BuffDescriptionAlt4;             //180
	uint32 BuffDescriptionAlt5;             //181
	uint32 BuffDescriptionAlt6;             //182
	uint32 BuffDescriptionAlt7;             //183
	uint32 BuffDescriptionAlt8;             //184
	uint32 BuffDescriptionAlt9;             //185
	uint32 BuffDescriptionAlt10;            //186
	uint32 BuffDescriptionAlt11;            //187
	uint32 BuffDescriptionAlt12;            //188
	uint32 BuffDescriptionAlt13;            //189
	uint32 BuffDescriptionAlt14;            //190
	uint32 BuffDescriptionAlt15;            //191
	uint32 buffdescflags;                   //192
	uint32 ManaCostPercentage;              //193
	uint32 unkflags;                        //194 
	uint32 StartRecoveryTime;               //195
	uint32 StartRecoveryCategory;           //196
	uint32 SpellFamilyName;                 //197
	uint32 SpellGroupType;					//198+199
	uint32 SpellGroupType_high;				//198+199
	uint32 MaxTargets;                      //200 
	uint32 Spell_Dmg_Type;                  //201   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 FG;                              //202   0,1,2 related to Spell_Dmg_Type I think
	int32 FH;                               //203   related to paladin aura's 
	float dmg_multiplier[3];                //204 - 206   if the name is correct I dono
	uint32 FL;                              //207   only one spellid:6994 has this value = 369 UNUSED
	uint32 FM;                              //208   only one spellid:6994 has this value = 4 UNUSED
	uint32 FN;                              //209  3 spells 1 or 2   
	uint32 TotemCategory[2];				//210-211
	uint32 RequiresAreaId;				    //212 
	uint32 School;							// 213
};

// lets make this bitwise for more fun
enum SpellTypes
{
    SPELL_TYPE_NONE                 = 0x00000000,
    SPELL_TYPE_SEAL                 = 0x00000001,
    SPELL_TYPE_ASPECT               = 0x00000002,
    SPELL_TYPE_BLESSING             = 0x00000004,
    SPELL_TYPE_CURSE                = 0x00000008,
    SPELL_TYPE_STING                = 0x00000010,
    SPELL_TYPE_ARMOR                = 0x00000020,
    SPELL_TYPE_AURA                 = 0x00000040,
    //hmm these could be named simply incompatible spells. One active at a time
    SPELL_TYPE_MARK_GIFT            = 0x00000080,
    SPELL_TYPE_TRACK                = 0x00000100,
    SPELL_TYPE_HUNTER_TRAP          = 0x00000200,
    SPELL_TYPE_MAGE_INTEL           = 0x00000400,
    SPELL_TYPE_MAGE_MAGI            = 0x00000800,
    SPELL_TYPE_MAGE_WARDS           = 0x00001000,
    SPELL_TYPE_PRIEST_SH_PPROT      = 0x00002000,
    SPELL_TYPE_SHIELD               = 0x00004000,
    SPELL_TYPE_FORTITUDE            = 0x00008000,
    SPELL_TYPE_SPIRIT               = 0x00010000,
    SPELL_TYPE_MAGE_AMPL_DUMP       = 0x00020000,
    SPELL_TYPE_WARLOCK_IMMOLATE     = 0x00040000, //maybe there is a better way to trigger the aura state for immolate spell
    SPELL_TYPE_FINISHING_MOVE       = 0x00080000, 
    SPELL_TYPE_ELIXIR_BATTLE		= 0x00100000, 
    SPELL_TYPE_ELIXIR_GUARDIAN      = 0x00200000, 
    SPELL_TYPE_ELIXIR_FLASK         = SPELL_TYPE_ELIXIR_BATTLE | SPELL_TYPE_ELIXIR_GUARDIAN, //weee, this contains both battle and guardian elixirs ;)
    SPELL_TYPE_HUNTER_MARK			= 0x00400000,
};

//custom stuff generated for spells that will not change in time
enum SpellIsFlags
{
    SPELL_FLAG_IS_DAMAGING				= 0x00000001,
    SPELL_FLAG_IS_HEALING				= 0x00000002,
    SPELL_FLAG_IS_TARGETINGSTEALTHED	= 0x00000004,
    SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE	= 0x00000008, //it started with rogue cold blood but i'm sure others will come
    SPELL_FLAG_IS_POISON				= 0x00000010, //rogue has a few spells that can stack so can't use the spell_type enum ;)
    SPELL_FLAG_IS_FINISHING_MOVE		= 0x00000020, //rogue has a few spells that can stack so can't use the spell_type enum ;)
    SPELL_FLAG_IS_NOT_USING_DMG_BONUS	= 0x00000040, 
    SPELL_FLAG_IS_CHILD_SPELL			= 0x00000080, //auras proc auras that have same name, these should not remove mother aura when adding to target
    SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET		= 0x00000100, //we should cast these on pet too
    SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER	= 0x00000200, //we should cast these on owner too
    SPELL_FLAG_IS_EXPIREING_WITH_PET	= 0x00000400, //when pet dies, we remove this too
    SPELL_FLAG_IS_EXPIREING_ON_PET		= 0x00000800, //when pet is summoned
};

enum SpellEffects
{
    SPELL_EFFECT_NULL = 0,
    SPELL_EFFECT_INSTANT_KILL,              //    1
    SPELL_EFFECT_SCHOOL_DAMAGE,             //    2    
    SPELL_EFFECT_DUMMY,                     //    3    
    SPELL_EFFECT_PORTAL_TELEPORT,           //    4    
    SPELL_EFFECT_TELEPORT_UNITS,            //    5    
    SPELL_EFFECT_APPLY_AURA,                //    6    
    SPELL_EFFECT_ENVIRONMENTAL_DAMAGE,      //    7    
    SPELL_EFFECT_POWER_DRAIN,               //    8    
    SPELL_EFFECT_HEALTH_LEECH,              //    9    
    SPELL_EFFECT_HEAL,                      //    10    
    SPELL_EFFECT_BIND,                      //    11    
    SPELL_EFFECT_PORTAL,                    //    12
    SPELL_EFFECT_RITUAL_BASE,               //    13
    SPELL_EFFECT_RITUAL_SPECIALIZE,         //    14
    SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL,    //    15
    SPELL_EFFECT_QUEST_COMPLETE,            //    16    
    SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL,    //    17    
    SPELL_EFFECT_RESURRECT,                 //    18    
    SPELL_EFFECT_ADD_EXTRA_ATTACKS,         //    19    
    SPELL_EFFECT_DODGE,                     //    20    
    SPELL_EFFECT_EVADE,                     //    21    
    SPELL_EFFECT_PARRY,                     //    22    
    SPELL_EFFECT_BLOCK,                     //    23    
    SPELL_EFFECT_CREATE_ITEM,               //    24    
    SPELL_EFFECT_WEAPON,                    //    25
    SPELL_EFFECT_DEFENSE,                   //    26
    SPELL_EFFECT_PERSISTENT_AREA_AURA,      //    27    
    SPELL_EFFECT_SUMMON,                    //    28    
    SPELL_EFFECT_LEAP,                      //    29    
    SPELL_EFFECT_ENERGIZE,                  //    30    
    SPELL_EFFECT_WEAPON_PERCENT_DAMAGE,     //    31    
    SPELL_EFFECT_TRIGGER_MISSILE,           //    32    
    SPELL_EFFECT_OPEN_LOCK,                 //    33    
    SPELL_EFFECT_TRANSFORM_ITEM,            //    34    
    SPELL_EFFECT_APPLY_AREA_AURA,           //    35    
    SPELL_EFFECT_LEARN_SPELL,               //    36    
    SPELL_EFFECT_SPELL_DEFENSE,             //    37    
    SPELL_EFFECT_DISPEL,                    //    38    
    SPELL_EFFECT_LANGUAGE,                  //    39
    SPELL_EFFECT_DUAL_WIELD,                //    40    
    SPELL_EFFECT_SUMMON_WILD,               //    41    
    SPELL_EFFECT_SUMMON_GUARDIAN,           //    42    
    SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER,//    43
    SPELL_EFFECT_SKILL_STEP,                //    44    
    SPELL_EFFECT_UNDEFINED_45,              //    45    
    SPELL_EFFECT_SPAWN,                     //    46
    SPELL_EFFECT_TRADE_SKILL,               //    47
    SPELL_EFFECT_STEALTH,                   //    48
    SPELL_EFFECT_DETECT,                    //    49
    SPELL_EFFECT_SUMMON_OBJECT,             //    50    
    //SPELL_EFFECT_TRANS_DOOR,              //    50    
    SPELL_EFFECT_FORCE_CRITICAL_HIT,        //    51
    SPELL_EFFECT_GUARANTEE_HIT,             //    52
    SPELL_EFFECT_ENCHANT_ITEM,              //    53    
    SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY,    //    54    
    SPELL_EFFECT_TAMECREATURE,              //    55    
    SPELL_EFFECT_SUMMON_PET,                //    56    
    SPELL_EFFECT_LEARN_PET_SPELL,           //    57    
    SPELL_EFFECT_WEAPON_DAMAGE,             //    58    
    SPELL_EFFECT_OPEN_LOCK_ITEM,            //    59    
    SPELL_EFFECT_PROFICIENCY,               //    60
    SPELL_EFFECT_SEND_EVENT,                //    61    
    SPELL_EFFECT_POWER_BURN,                //    62
    SPELL_EFFECT_THREAT,                    //    63
    SPELL_EFFECT_TRIGGER_SPELL,             //    64    
    SPELL_EFFECT_HEALTH_FUNNEL,             //    65
    SPELL_EFFECT_POWER_FUNNEL,              //    66
    SPELL_EFFECT_HEAL_MAX_HEALTH,           //    67    
    SPELL_EFFECT_INTERRUPT_CAST,            //    68
    SPELL_EFFECT_DISTRACT,                  //    69
    SPELL_EFFECT_PULL,                      //    70
    SPELL_EFFECT_PICKPOCKET,                //    71
    SPELL_EFFECT_ADD_FARSIGHT,              //    72
    SPELL_EFFECT_SUMMON_POSSESSED,          //    73    
    SPELL_EFFECT_SUMMON_TOTEM,              //    74    
    SPELL_EFFECT_HEAL_MECHANICAL,           //    75
    SPELL_EFFECT_SUMMON_OBJECT_WILD,        //    76
    SPELL_EFFECT_SCRIPT_EFFECT,             //    77    
    SPELL_EFFECT_ATTACK,                    //    78
    SPELL_EFFECT_SANCTUARY,                 //    79
    SPELL_EFFECT_ADD_COMBO_POINTS,          //    80    
    SPELL_EFFECT_CREATE_HOUSE,              //    81
    SPELL_EFFECT_BIND_SIGHT,                //    82
    SPELL_EFFECT_DUEL,                      //    83
    SPELL_EFFECT_STUCK,                     //    84
    SPELL_EFFECT_SUMMON_PLAYER,             //    85
    SPELL_EFFECT_ACTIVATE_OBJECT,           //    86
    SPELL_EFFECT_SUMMON_TOTEM_SLOT1,        //    87    
    SPELL_EFFECT_SUMMON_TOTEM_SLOT2,        //    88    
    SPELL_EFFECT_SUMMON_TOTEM_SLOT3,        //    89    
    SPELL_EFFECT_SUMMON_TOTEM_SLOT4,        //    90    
    SPELL_EFFECT_THREAT_ALL,                //    91
    SPELL_EFFECT_ENCHANT_HELD_ITEM,         //    92
    SPELL_EFFECT_SUMMON_PHANTASM,           //    93
    SPELL_EFFECT_SELF_RESURRECT,            //    94    
    SPELL_EFFECT_SKINNING,                  //    95    
    SPELL_EFFECT_CHARGE,                    //    96    
    SPELL_EFFECT_SUMMON_CRITTER,            //    97    
    SPELL_EFFECT_KNOCK_BACK,                //    98    
    SPELL_EFFECT_DISENCHANT,                //    99    
    SPELL_EFFECT_INEBRIATE,                 //    100    
    SPELL_EFFECT_FEED_PET,                  //    101
    SPELL_EFFECT_DISMISS_PET,               //    102
    SPELL_EFFECT_REPUTATION,                //    103
    SPELL_EFFECT_SUMMON_OBJECT_SLOT1,       //    104
    SPELL_EFFECT_SUMMON_OBJECT_SLOT2,       //    105
    SPELL_EFFECT_SUMMON_OBJECT_SLOT3,       //    106
    SPELL_EFFECT_SUMMON_OBJECT_SLOT4,       //    107
    SPELL_EFFECT_DISPEL_MECHANIC,           //    108    
    SPELL_EFFECT_SUMMON_DEAD_PET,           //    109
    SPELL_EFFECT_DESTROY_ALL_TOTEMS,        //    110
    SPELL_EFFECT_DURABILITY_DAMAGE,         //    111
    SPELL_EFFECT_SUMMON_DEMON,              //    112    
    SPELL_EFFECT_RESURRECT_FLAT,            //    113    
    SPELL_EFFECT_ATTACK_ME,                 //    114
    SPELL_EFFECT_DURABILITY_DAMAGE_PCT,     //    115
    SPELL_EFFECT_SKIN_PLAYER_CORPSE,        //    116
    SPELL_EFFECT_SPIRIT_HEAL,               //    117
    SPELL_EFFECT_SKILL,                     //    118
    SPELL_EFFECT_APPLY_PET_AURA,            //    119    
    SPELL_EFFECT_TELEPORT_GRAVEYARD,        //    120
    SPELL_EFFECT_DUMMYMELEE,                //    121
    SPELL_EFFECT_UNKNOWN1,                  //    122
    SPELL_EFFECT_UNKNOWN2,                  //    123
    SPELL_EFFECT_UNKNOWN3,                  //    124
    SPELL_EFFECT_UNKNOWN4,                  //    125
    SPELL_EFFECT_UNKNOWN5,                  //    126
    SPELL_EFFECT_PROSPECTING,               //    127
    SPELL_EFFECT_UNKNOWN7,                  //    128
    SPELL_EFFECT_UNKNOWN8,                  //    129
    SPELL_EFFECT_UNKNOWN9,                  //    129
    SPELL_EFFECT_UNKNOWN10,                 //    130
    SPELL_EFFECT_UNKNOWN11,                 //    131
    SPELL_EFFECT_UNKNOWN12,                 //    132
    SPELL_EFFECT_FORGET_SPECIALIZATION,     //    133
    SPELL_EFFECT_UNKNOWN14,                 //    134
    SPELL_EFFECT_UNKNOWN15,                 //    135
    SPELL_EFFECT_UNKNOWN16,                 //    136
    SPELL_EFFECT_UNKNOWN17,                 //    137
    SPELL_EFFECT_UNKNOWN18,                 //    138
    SPELL_EFFECT_UNKNOWN19,                 //    139
    SPELL_EFFECT_UNKNOWN20,                 //    140
    SPELL_EFFECT_UNKNOWN21,                 //    141
    SPELL_EFFECT_UNKNOWN22,                 //    142
    SPELL_EFFECT_UNKNOWN23,                 //    143
    TOTAL_SPELL_EFFECTS,                    //    144
};

enum AuraInterruptFlags
{
    AURA_INTERRUPT_NULL                       = 0x0,
    AURA_INTERRUPT_ON_HOSTILE_SPELL_INFLICTED = 0x1,
    AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN        = 0x2,
    AURA_INTERRUPT_ON_UNK1                    = 0x4,
    AURA_INTERRUPT_ON_MOVEMENT                = 0x8, // could be AURA_INTERRUPT_ON_MOVEMENT
    AURA_INTERRUPT_ON_UNK2                    = 0x10,
    AURA_INTERRUPT_ON_UNK3                    = 0x20,
    AURA_INTERRUPT_ON_UNUSED1                 = 0x40,
    AURA_INTERRUPT_ON_SLOWED                  = 0x80,
    AURA_INTERRUPT_ON_LEAVE_WATER             = 0x100, // could be AURA_INTERRUPT_ON_LEAVE_CURRENT_SURFACE
    AURA_INTERRUPT_ON_UNUSED2                 = 0x200,
    AURA_INTERRUPT_ON_UNK4                    = 0x400,
    AURA_INTERRUPT_ON_UNK5                    = 0x800,
    AURA_INTERRUPT_ON_START_ATTACK            = 0x1000,
    AURA_INTERRUPT_ON_UNK6                    = 0x2000,
    AURA_INTERRUPT_ON_UNUSED3                 = 0x4000,
    AURA_INTERRUPT_ON_CAST_SPELL              = 0x8000,
    AURA_INTERRUPT_ON_UNK7                    = 0x10000,
    AURA_INTERRUPT_ON_MOUNT                   = 0x20000,
    AURA_INTERRUPT_ON_STAND_UP                = 0x40000,
    AURA_INTERRUPT_ON_LEAVE_AREA              = 0x80000,
    AURA_INTERRUPT_ON_INVINCIBLE              = 0x100000,
    AURA_INTERRUPT_ON_STEALTH                 = 0x200000,
    AURA_INTERRUPT_ON_UNK8                    = 0x400000,
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
    SPELL_AURA_MOD_UNATTACKABLE = 93,                   // Mod Uninteractible
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
    SPELL_AURA_ADD_TARGET_TRIGGER = 109,                // Add Class Target Trigger    
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
    SPELL_AURA_MOD_POWER_COST_PCT = 123,                // Mod Power Cost %
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
    SPELL_AURA_INCREASE_SPELL_DAMAGE_PCT = 174,         // Increase Spell Damage by % Spirit (Spells
    SPELL_AURA_INCREASE_SPELL_HEALING_PCT = 175,        // Increase Spell Healing by % Spirit
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
    SPELL_AURA_INREASE_SPELL_DAMAGE_PCT_OF_INTELLECT = 194,
    SPELL_AURA_INCREASE_HEALING_PCT_OF_INTELLECT = 195,
    SPELL_AURA_MOD_ALL_WEAPON_SKILLS = 196,
    SPELL_AURA_REDUCE_ATTACKER_CRICTICAL_HIT_CHANCE_PCT = 197,
    SPELL_AURA_INCREASE_SPELL_HIT_PCT = 199,
    SPELL_AURA_CANNOT_BE_DODGED = 201,
    SPELL_AURA_REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_MELEE_PCT = 203,
    SPELL_AURA_REDUCE_ATTACKER_CRICTICAL_HIT_DAMAGE_RANGED_PCT = 204,
    SPELL_AURA_INCREASE_RANGED_ATTACK_POWER_PCT_OF_INTELLECT = 212,
    SPELL_AURA_INCREASE_RAGE_FROM_DAMAGE_DEALT_PCT = 213,
    SPELL_AURA_INCREASE_CASTING_TIME_PCT = 216,
    SPELL_AURA_REGEN_MANA_STAT_PCT=219,
    SPELL_AURA_HEALING_STAT_PCT=220,
    SPELL_AURA_INCREASE_MAX_HEALTH=230,//Used by Commanding Shout
    TOTAL_SPELL_AURAS = 231,
};

enum procFlags
{
    PROC_NULL                       = 0x0,
    PROC_ON_ANY_HOSTILE_ACTION      = 0x1,
    PROC_ON_GAIN_EXPIERIENCE        = 0x2,
    PROC_ON_MELEE_ATTACK            = 0x4,
    PROC_ON_CRIT_HIT_VICTIM         = 0x8,
    PROC_ON_CAST_SPELL              = 0x10,
    PROC_ON_PHYSICAL_ATTACK_VICTIM  = 0x20,
    PROC_ON_RANGED_ATTACK           = 0x40,
    PROC_ON_RANGED_CRIT_ATTACK      = 0x80,
//    PROC_ON_UNK_DAMAGE_VICTIM       = 0x80,//seems to be on ranged dmg victim 99% sure('each melee or ranged attack' -> flag =680 (dec))
    PROC_ON_PHYSICAL_ATTACK         = 0x100,
    PROC_ON_MELEE_ATTACK_VICTIM     = 0x200,
//    PROC_ON_ANY_ACTION              = 0x400,
    PROC_ON_SPELL_LAND              = 0x400,
//    PROC_UNK_DEFILLED               = 0x800,
    PROC_ON_RANGED_CRIT_ATTACK_VICTIM = 0x800,
    PROC_ON_CRIT_ATTACK             = 0x1000,
    PROC_ON_RANGED_ATTACK_VICTIM    = 0x2000,
//    PROC_ANYTIME                    = 0x4000,
    PROC_ON_PRE_DISPELL_AURA_VICTIM = 0x4000,
//    PROC_UNK2_DEFILLED              = 0x8000,
	PROC_ON_SPELL_LAND_VICTIM       = 0x8000,//custom flag. PROC only when spell landed on victim
    PROC_ON_CAST_SPECIFIC_SPELL     = 0x10000,
    PROC_ON_SPELL_HIT_VICTIM        = 0x20000,
    PROC_ON_SPELL_CRIT_HIT_VICTIM   = 0x40000,
//    PROC_ON_UNK2_DAMAGE_VICTIM      = 0x80000,
    PROC_ON_TARGET_DIE		        = 0x80000,
    PROC_ON_ANY_DAMAGE_VICTIM       = 0x100000,
    PROC_ON_TRAP_TRIGGERED          = 0x200000,
    PROC_ON_AUTO_SHOT_HIT           = 0x400000,
    PROC_ON_ABSORB                  = 0x800000,
    PROC_ON_RESIST_VICTIM           = 0x1000000,//added it as custom, maybe already exists in another form ?
    PROC_ON_DODGE_VICTIM            = 0x2000000,//added it as custom, maybe already exists in another form ?
    PROC_ON_DIE                     = 0x4000000,//added it as custom, maybe already exists in another form ?
    PROC_REMOVEONUSE                = 0x8000000,//remove prochcharge only when it is used
    PROC_MISC                       = 0x10000000,//our custom flag to decide if proc dmg or shield
    PROC_ON_BLOCK_VICTIM            = 0x20000000,//added it as custom, maybe already exists in another form ?
    PROC_ON_SPELL_CRIT_HIT          = 0x40000000,//added it as custom, maybe already exists in another form ?
    PROC_TARGET_SELF               = 0x80000000,//our custom flag to decide if proc target is self or victim
};

enum School
{
    SCHOOL_NORMAL = 0,
    SCHOOL_HOLY   = 1,
    SCHOOL_FIRE   = 2,
    SCHOOL_NATURE = 3,
    SCHOOL_FROST  = 4,
    SCHOOL_SHADOW = 5,
    SCHOOL_ARCANE = 6
};

enum School_FLAGS
{
    SCHOOL_FLAG_NORMAL = 1,
    SCHOOL_FLAG_HOLY   = 2,
    SCHOOL_FLAG_FIRE   = 4,
    SCHOOL_FLAG_NATURE = 8,
    SCHOOL_FLAG_FROST  = 16,
    SCHOOL_FLAG_SHADOW = 32,
    SCHOOL_FLAG_ARCANE = 64,
};

enum SPELL_DMG_TYPE // SPELL_ENTRY_Spell_Dmg_Type
{
	SPELL_DMG_TYPE_NONE   = 0,
	SPELL_DMG_TYPE_MAGIC  = 1,
	SPELL_DMG_TYPE_MELEE  = 2,
	SPELL_DMG_TYPE_RANGED = 3
};

enum MECHANICS
{
    MECHANIC_CHARMED = 1,
    MECHANIC_DISORIENTED, // 2
    MECHANIC_DISARMED, // 3
    MECHANIC_DISTRACED, // 4
    MECHANIC_FLEEING, // 5
    MECHANIC_CLUMSY, // 6
    MECHANIC_ROOTED, // 7
    MECHANIC_PACIFIED, // 8
    MECHANIC_SILENCED, // 9
    MECHANIC_ASLEEP, // 10
    MECHANIC_ENSNARED, // 11
    MECHANIC_STUNNED,
    MECHANIC_FROZEN,
    MECHANIC_INCAPACIPATED,
    MECHANIC_BLEEDING,
    MECHANIC_HEALING,
    MECHANIC_POLYMORPHED,
    MECHANIC_BANISHED,
    MECHANIC_SHIELDED,
    MECHANIC_SHACKLED,
    MECHANIC_MOUNTED,
    MECHANIC_SEDUCED,
    MECHANIC_TURNED,
    MECHANIC_HORRIFIED,
    MECHANIC_INVULNARABLE,
    MECHANIC_INTERRUPTED,
    MECHANIC_DAZED
};

enum Attributes
{
    ATTRIBUTES_NULL								= 0x0,
    ATTRIBUTES_UNK2								= 0x1,
    ATTRIBUTES_UNK3								= 0x2, // related to ranged??
    ATTRIBUTE_ON_NEXT_ATTACK					= 0x4,
    ATTRIBUTES_UNK5								= 0x8, // not used.
    ATTRIBUTES_UNK6								= 0x10,
    ATTRIBUTES_UNK7								= 0x20, // Reagents
    ATTRIBUTES_PASSIVE							= 0x40,
    ATTRIBUTES_NO_VISUAL_AURA					= 0x80,
    ATTRIBUTES_UNK10							= 0x100,//seems to be afflicts pet
    ATTRIBUTES_UNK11							= 0x200, // only appears in shaman imbue weapon spells
    ATTRIBUTES_UNK12							= 0x400,
    ATTRIBUTES_UNK13							= 0x800,
    ATTRIBUTES_UNUSED1							= 0x1000,
    ATTRIBUTES_UNUSED2							= 0x2000,
    ATTRIBUTES_UNUSED3							= 0x4000,
    ATTRIBUTES_ONLY_OUTDOORS					= 0x8000,
    ATTRIBUTES_UNK								= 0x10000,
    ATTRIBUTES_REQ_STEALTH						= 0x20000,
    ATTRIBUTES_UNK20							= 0x40000,//it's not : must be behind
    ATTRIBUTES_UNK21							= 0x80000,
    ATTRIBUTES_MUSTFACECASTER					= 0x100000,//may be wrong
    ATTRIBUTES_UNK23							= 0x200000,
    ATTRIBUTES_UNK24							= 0x400000,
    ATTRIBUTES_UNK25							= 0x800000,
    ATTRIBUTES_UNK26							= 0x1000000,
    ATTRIBUTES_TRIGGER_COOLDOWN			        = 0x2000000, //also requires atributes ex = 32 ?
    ATTRIBUTES_UNK28							= 0x4000000,
    ATTRIBUTES_UNK29							= 0x8000000,
    ATTRIBUTES_REQ_OOC							= 0x10000000, //     ATTRIBUTES_REQ_OUT_OF_COMBAT
    ATTRIBUTES_UNK31							= 0x20000000,
    ATTRIBUTES_UNK32							= 0x40000000,
    ATTRIBUTES_UNUSED9							= 0x80000000,
};

enum AttributesEx
{
    ATTRIBUTESEX_NULL                         = 0x0,
    ATTRIBUTESEX_UNK2                         = 0x1,
    ATTRIBUTEEX_DRAIN_WHOLE_MANA              = 0x2,
    ATTRIBUTESEX_UNK4                         = 0x4,
    ATTRIBUTESEX_UNK5                         = 0x8,
    ATTRIBUTESEX_UNK6                         = 0x10,
    ATTRIBUTESEX_DELAY_SOME_TRIGGERS          = 0x20,
    ATTRIBUTESEX_UNK8                         = 0x40,
    ATTRIBUTESEX_UNK9                         = 0x80,
    ATTRIBUTESEX_UNK10                        = 0x100,
    ATTRIBUTESEX_UNK11                        = 0x200,
    ATTRIBUTESEX_UNK12                        = 0x400,
    ATTRIBUTESEX_UNK13                        = 0x800,
    ATTRIBUTESEX_UNK14                        = 0x1000, // related to pickpocket
    ATTRIBUTESEX_UNK15                        = 0x2000, // related to remote control
    ATTRIBUTESEX_UNK16                        = 0x4000,
    ATTRIBUTESEX_UNK17                        = 0x8000,
    ATTRIBUTESEX_UNK18                        = 0x10000, 
    ATTRIBUTESEX_REMAIN_OOC                   = 0x20000,
    ATTRIBUTESEX_UNK20                        = 0x40000,
    ATTRIBUTESEX_UNK21                        = 0x80000,
    ATTRIBUTESEX_UNK22                        = 0x100000, // related to "Finishing move" and "Instantly overpowers"
    ATTRIBUTESEX_UNK23                        = 0x200000,
    ATTRIBUTESEX_UNK24                        = 0x400000, // only related to "Finishing move"
    ATTRIBUTESEX_UNK25                        = 0x800000, // related to spells like "ClearAllBuffs"
    ATTRIBUTESEX_UNK26                        = 0x1000000, // FISHING SPELLS
    ATTRIBUTESEX_UNK27                        = 0x2000000, // related to "Detect" spell
    ATTRIBUTESEX_UNK28                        = 0x4000000,
    ATTRIBUTESEX_UNK29                        = 0x8000000,
    ATTRIBUTESEX_UNK30                        = 0x10000000,
    ATTRIBUTESEX_UNK31                        = 0x20000000,
    ATTRIBUTESEX_UNK32                        = 0x40000000, // Overpower
};

typedef enum {
   EFF_TARGET_NONE										= 0,
   EFF_TARGET_SELF										= 1,
   EFF_TARGET_INVISIBLE_OR_HIDDEN_ENEMIES_AT_LOCATION_RADIUS		= 3,
   EFF_TARGET_PET										= 5,
   EFF_TARGET_SINGLE_ENEMY								= 6,
   EFF_TARGET_SCRIPTED_TARGET							= 7,
   EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS  = 8,
   EFF_TARGET_HEARTSTONE_LOCATION						= 9,
   EFF_TARGET_ALL_ENEMY_IN_AREA							= 15,
   EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT					= 16,
   EFF_TARGET_TELEPORT_LOCATION							= 17,
   EFF_TARGET_LOCATION_TO_SUMMON						= 18,
   EFF_TARGET_ALL_PARTY_AROUND_CASTER					= 20,
   EFF_TARGET_SINGLE_FRIEND								= 21,
   EFF_TARGET_ALL_ENEMIES_AROUND_CASTER					= 22,
   EFF_TARGET_GAMEOBJECT								= 23,
   EFF_TARGET_IN_FRONT_OF_CASTER						= 24,
   EFF_TARGET_SELECTION									= 25,//Dont know the real name!!!
   EFF_TARGET_GAMEOBJECT_ITEM							= 26,
   EFF_TARGET_PET_MASTER								= 27,
   EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED				= 28,
   EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED				= 29,
   EFF_TARGET_ALL_FRIENDLY_IN_AREA						= 30,
   EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME	= 31,
   EFF_TARGET_MINION									= 32,
   EFF_TARGET_ALL_PARTY_IN_AREA							= 33,
   EFF_TARGET_SINGLE_PARTY								= 35,
   EFF_TARGET_PET_SUMMON_LOCATION						= 36,
   EFF_TARGET_ALL_PARTY									= 37,
   EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET					= 38,
   EFF_TARGET_SELF_FISHING								= 39,
   EFF_TARGET_SCRIPTED_GAMEOBJECT						= 40,
   EFF_TARGET_TOTEM_EARTH								= 41,
   EFF_TARGET_TOTEM_WATER								= 42,
   EFF_TARGET_TOTEM_AIR									= 43,
   EFF_TARGET_TOTEM_FIRE								= 44,
   EFF_TARGET_CHAIN										= 45,
   EFF_TARGET_SCIPTED_OBJECT_LOCATION					= 46,
   EFF_TARGET_DYNAMIC_OBJECT							= 47,//not sure exactly where is used
   EFF_TARGET_MULTIPLE_SUMMON_LOCATION					= 48,
   EFF_TARGET_MULTIPLE_SUMMON_PET_LOCATION				= 49,
   EFF_TARGET_SUMMON_LOCATION							= 50,
   EFF_TARGET_CALIRI_EGS								= 51,
   EFF_TARGET_LOCATION_NEAR_CASTER						= 52,
   EFF_TARGET_ALL_ENEMIES_AROUND_TARGET					= 53,
   EFF_TARGET_TARGET_AT_ORIENTATION_TO_CASTER			= 54,
   EFF_TARGET_LOCATION_INFRONT_CASTER					= 55,
   EFF_TARGET_PARTY_MEMBER								= 57,
   EFF_TARGET_TARGET_FOR_VISUAL_EFFECT					= 59,
   EFF_TARGET_SCRIPTED_TARGET2							= 60,
   EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS				= 61,
   EFF_TARGET_PRIEST_CHAMPION							= 62, //wtf ?
   EFF_TARGET_NATURE_SUMMON_LOCATION					= 63, 
   EFF_TARGET_BEHIND_TARGET_LOCATION					= 65, 
   EFF_TARGET_MULTIPLE_GUARDIAN_SUMMON_LOCATION			= 72,
   EFF_TARGET_NETHETDRAKE_SUMMON_LOCATION				= 73,
   EFF_TARGET_SCRIPTED_LOCATION							= 74,
   EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE			= 75,
   EFF_TARGET_ENEMYS_IN_ARE_CHANNELED_WITH_EXCEPTIONS	= 76,
   EFF_TARGET_SELECTED_ENEMY_CHANNELED					= 77,
   EFF_TARGET_SELECTED_ENEMY_DEADLY_POISON				= 86,
} SpellEffectTarget;

enum DISPEL_TYPE
{
    DISPEL_ZGTRINKETS = -1,
    DISPEL_NULL,
    DISPEL_MAGIC,
    DISPEL_CURSE,
    DISPEL_DISEASE,
    DISPEL_POISON,
    DISPEL_STEALTH,
    DISPEL_INVISIBILTY,
    DISPEL_ALL,
    DISPEL_SPECIAL_NPCONLY,
    DISPEL_FRENZY,
};

#define SPELL_RANGED_GENERAL    3018
#define SPELL_RANGED_THROW      2764
#define SPELL_RANGED_WAND       5019

//wooohooo, there are 19 spells that actually require to add a proccounter for these 
//first spell catched is "presence of mind"
//an ugly solution would be to add a proc flag to remove aura on event it should expire (like attack or cast) but that is only if count=1
enum SPELL_MODIFIER_TYPE
{
    SMT_DAMAGE_DONE         =0,// increases the damage done by spell by x% dmg (flat as x dmg)
    SMT_DURATION            =1,// spell    duration increase // GOOD
    //hmm shaman spirit weapon clearly states that this should be parry chance
    SMT_TREAT_REDUCED       =2,// reduces threat generated by this spell by x% // GOOD but need more work
    SMT_ATTACK_POWER_AND_DMG_BONUS=3,// attacker power bonus for ability +x% // Seal of crusader also takes this as dmg bonus + power bonus 
    SMT_BLOCK               =4,// block additional attack // GOOD and need work //Not used in 2.1.1 ?
    SMT_RANGE               =5,// spell range bonus // GOOD
    SMT_RADIUS              =6,// spell radius bonus // GOOD
    SMT_CRITICAL            =7,// critical chance bonus // GOOD //only 1 example in 2.1.1
    SMT_SPELL_VALUE               =8,// dummy effect // needs work is not only pets Hp but effect of spells to
    SMT_NONINTERRUPT        =9,// x% chance not to be interrupted by received damage (no flat)
    SMT_CAST_TIME           =10,// cast time decrease // GOOD
    SMT_COOLDOWN_DECREASE   =11,// cooldown decrease <-probably fully handled by client // GOOD
    SMT_EFFECT              = 12,//used by shaman elemental weapons and another spell
//    SMT_SPEED             =12,// movement speed, while given spell is active(flat is %) // TODO CHECK! ok this is not speed and is used with 23 misc to sometimes so is odd
    // 13 dont exist spells with it
    SMT_COST                =14,// mana/energy/rage cost reduction // GOOD
    SMT_CRITICAL_DAMAGE     =15,// increases critical strike damage bonus (no flat)
    SMT_RESIST              =16,// enemy resist chance decrease (flat as %) // GOOD need work
    SMT_ADDITIONAL_TARGET   =17,// Your Healing Wave will now jump to additional nearby targets. Each jump reduces the effectiveness of the heal by 80% // GOOD
    SMT_TRIGGER             =18,// adds/increases chance to trigger some spell for example increase chance to apply poisons or entaglin // GOOD need work
    SMT_TIME                =19,// delay for nova, redirection time bonus for totem,maybe smth else // GOOD need work
    SMT_JUMP_REDUCE         =20,// Increases the amount healed by Chain Heal to targets beyond the first by x%. (no flat)
    //SMT_CAST_TIME2        =21,// this one looks like cast time...
    SMT_SPELL_VALUE_PCT		=22,// damage done by ability by x% : SELECT id,name,description FROM dbc_spell where (effect_aura_1=108 and effect_misc_1=22) or (effect_aura_2=108 and effect_misc_2=22) or (effect_aura_3=108 and effect_misc_3=22)
    SMT_UNKNOWN23           =23,//increase the target's spell damage and healing by an amount equal to $s1% of their total Spirit." , this one is used with the 12 effect needs some decode
    SMT_PENALTY             =24,// This is a modifer for the amount of +spell damage applied to the spell group, ill be implementing it soon (fishbait) (it should really have a better name ;) ) // GOOD
    // 25 dont exist spells with it
    // 26 is obsolete stuff
    SMT_EFFECT_BONUS        =27,// mana lost cost per point of damage taken for mana shield,Health or Mana gained from Drain Life and Drain Mana increased by x%.
    SMT_RESIST_DISPEL       =28,// TODO NEEDS WORK :D
};

enum ShapeshiftForm
{
    FORM_CAT              = 1,
    FORM_TREE             = 2,
    FORM_TRAVEL           = 3,
    FORM_AQUA             = 4,
    FORM_BEAR             = 5,
    FORM_AMBIENT          = 6,
    FORM_GHOUL            = 7,
    FORM_DIREBEAR         = 8,
    FORM_CREATUREBEAR     = 14,
    FORM_GHOSTWOLF        = 16,
    FORM_BATTLESTANCE     = 17,
    FORM_DEFENSIVESTANCE  = 18,
    FORM_BERSERKERSTANCE  = 19,
    FORM_SWIFT            = 27,
    FORM_SHADOW           = 28,
    FORM_FLIGHT           = 29,
    FORM_STEALTH          = 30,
    FORM_MOONKIN          = 31,
    FORM_SPIRITOFREDEMPTION    = 32,
};

enum SPELL_ENTRY
{
    SPELL_ENTRY_Id,
    SPELL_ENTRY_School,
    SPELL_ENTRY_Category,
    SPELL_ENTRY_field4,
    SPELL_ENTRY_DispelType,
    SPELL_ENTRY_MechanicsType,
    SPELL_ENTRY_Attributes,
    SPELL_ENTRY_AttributesEx,
    SPELL_ENTRY_Flags3,
    SPELL_ENTRY_field10,
    SPELL_ENTRY_field11,
    SPELL_ENTRY_unk201_1,
    SPELL_ENTRY_RequiredShapeShift,
    SPELL_ENTRY_UNK14,
    SPELL_ENTRY_Targets,
    SPELL_ENTRY_TargetCreatureType,
    SPELL_ENTRY_RequiresSpellFocus,
    SPELL_ENTRY_CasterAuraState,
    SPELL_ENTRY_TargetAuraState,
    SPELL_ENTRY_unk201_2,
    SPELL_ENTRY_unk201_3,
    SPELL_ENTRY_CastingTimeIndex,
    SPELL_ENTRY_RecoveryTime,
    SPELL_ENTRY_CategoryRecoveryTime,
    SPELL_ENTRY_InterruptFlags,
    SPELL_ENTRY_AuraInterruptFlags,
    SPELL_ENTRY_ChannelInterruptFlags,
    SPELL_ENTRY_procFlags,
    SPELL_ENTRY_procChance,
    SPELL_ENTRY_procCharges,
    SPELL_ENTRY_maxLevel,
    SPELL_ENTRY_baseLevel,
    SPELL_ENTRY_spellLevel,
    SPELL_ENTRY_DurationIndex,
    SPELL_ENTRY_powerType,
    SPELL_ENTRY_manaCost,
    SPELL_ENTRY_manaCostPerlevel,
    SPELL_ENTRY_manaPerSecond,
    SPELL_ENTRY_manaPerSecondPerLevel,
    SPELL_ENTRY_rangeIndex,
    SPELL_ENTRY_speed,
    SPELL_ENTRY_modalNextSpell,
    SPELL_ENTRY_maxstack,
    SPELL_ENTRY_Totem_1,
    SPELL_ENTRY_Totem_2,
    SPELL_ENTRY_Reagent_1,
    SPELL_ENTRY_Reagent_2,
    SPELL_ENTRY_Reagent_3,
    SPELL_ENTRY_Reagent_4,
    SPELL_ENTRY_Reagent_5,
    SPELL_ENTRY_Reagent_6,
    SPELL_ENTRY_Reagent_7,
    SPELL_ENTRY_Reagent_8,
    SPELL_ENTRY_ReagentCount_1,
    SPELL_ENTRY_ReagentCount_2,
    SPELL_ENTRY_ReagentCount_3,
    SPELL_ENTRY_ReagentCount_4,
    SPELL_ENTRY_ReagentCount_5,
    SPELL_ENTRY_ReagentCount_6,
    SPELL_ENTRY_ReagentCount_7,
    SPELL_ENTRY_ReagentCount_8,
    SPELL_ENTRY_EquippedItemClass,
    SPELL_ENTRY_EquippedItemSubClass,
    SPELL_ENTRY_RequiredItemFlags,
    SPELL_ENTRY_Effect_1,
    SPELL_ENTRY_Effect_2,
    SPELL_ENTRY_Effect_3,
    SPELL_ENTRY_EffectDieSides_1,
    SPELL_ENTRY_EffectDieSides_2,
    SPELL_ENTRY_EffectDieSides_3,
    SPELL_ENTRY_EffectBaseDice_1,
    SPELL_ENTRY_EffectBaseDice_2,
    SPELL_ENTRY_EffectBaseDice_3,
    SPELL_ENTRY_EffectDicePerLevel_1,
    SPELL_ENTRY_EffectDicePerLevel_2,
    SPELL_ENTRY_EffectDicePerLevel_3,
    SPELL_ENTRY_EffectRealPointsPerLevel_1,
    SPELL_ENTRY_EffectRealPointsPerLevel_2,
    SPELL_ENTRY_EffectRealPointsPerLevel_3,
    SPELL_ENTRY_EffectBasePoints_1,
    SPELL_ENTRY_EffectBasePoints_2,
    SPELL_ENTRY_EffectBasePoints_3,
    SPELL_ENTRY_EffectMechanic_1,
    SPELL_ENTRY_EffectMechanic_2,
    SPELL_ENTRY_EffectMechanic_3,
    SPELL_ENTRY_EffectImplicitTargetA_1,
    SPELL_ENTRY_EffectImplicitTargetA_2,
    SPELL_ENTRY_EffectImplicitTargetA_3,
    SPELL_ENTRY_EffectImplicitTargetB_1,
    SPELL_ENTRY_EffectImplicitTargetB_2,
    SPELL_ENTRY_EffectImplicitTargetB_3,
    SPELL_ENTRY_EffectRadiusIndex_1,
    SPELL_ENTRY_EffectRadiusIndex_2, 
    SPELL_ENTRY_EffectRadiusIndex_3, 
    SPELL_ENTRY_EffectApplyAuraName_1,
    SPELL_ENTRY_EffectApplyAuraName_2,
    SPELL_ENTRY_EffectApplyAuraName_3,
    SPELL_ENTRY_EffectAmplitude_1,
    SPELL_ENTRY_EffectAmplitude_2,
    SPELL_ENTRY_EffectAmplitude_3,
    SPELL_ENTRY_Effectunknown_1,
    SPELL_ENTRY_Effectunknown_2,
    SPELL_ENTRY_Effectunknown_3,
    SPELL_ENTRY_EffectChainTarget_1,
    SPELL_ENTRY_EffectChainTarget_2,
    SPELL_ENTRY_EffectChainTarget_3,
    SPELL_ENTRY_EffectSpellGroupRelation_1,
    SPELL_ENTRY_EffectSpellGroupRelation_2,
    SPELL_ENTRY_EffectSpellGroupRelation_3,
    SPELL_ENTRY_EffectMiscValue_1,
    SPELL_ENTRY_EffectMiscValue_2,
    SPELL_ENTRY_EffectMiscValue_3,
    SPELL_ENTRY_EffectTriggerSpell_1,
    SPELL_ENTRY_EffectTriggerSpell_2,
    SPELL_ENTRY_EffectTriggerSpell_3,
    SPELL_ENTRY_EffectPointsPerComboPoint_1,
    SPELL_ENTRY_EffectPointsPerComboPoint_2,
    SPELL_ENTRY_EffectPointsPerComboPoint_3,
    SPELL_ENTRY_SpellVisual,
    SPELL_ENTRY_field114,
    SPELL_ENTRY_dummy,
    SPELL_ENTRY_CoSpell,
    SPELL_ENTRY_spellPriority,
    SPELL_ENTRY_Name,
    SPELL_ENTRY_NameAlt1,
    SPELL_ENTRY_NameAlt2,
    SPELL_ENTRY_NameAlt3,
    SPELL_ENTRY_NameAlt4,
    SPELL_ENTRY_NameAlt5,
    SPELL_ENTRY_NameAlt6,
    SPELL_ENTRY_NameAlt7,
    SPELL_ENTRY_NameAlt8,
    SPELL_ENTRY_NameAlt9,
    SPELL_ENTRY_NameAlt10,
    SPELL_ENTRY_NameAlt11,
    SPELL_ENTRY_NameAlt12,
    SPELL_ENTRY_NameAlt13,
    SPELL_ENTRY_NameAlt14,
    SPELL_ENTRY_NameAlt15,
    SPELL_ENTRY_NameFlags,
    SPELL_ENTRY_Rank,
    SPELL_ENTRY_RankAlt1,
    SPELL_ENTRY_RankAlt2,
    SPELL_ENTRY_RankAlt3,
    SPELL_ENTRY_RankAlt4,
    SPELL_ENTRY_RankAlt5,
    SPELL_ENTRY_RankAlt6,
    SPELL_ENTRY_RankAlt7,
    SPELL_ENTRY_RankAlt8,
    SPELL_ENTRY_RankAlt9,
    SPELL_ENTRY_RankAlt10,
    SPELL_ENTRY_RankAlt11,
    SPELL_ENTRY_RankAlt12,
    SPELL_ENTRY_RankAlt13,
    SPELL_ENTRY_RankAlt14,
    SPELL_ENTRY_RankAlt15,
    SPELL_ENTRY_RankFlags,
    SPELL_ENTRY_Description,
    SPELL_ENTRY_DescriptionAlt1,
    SPELL_ENTRY_DescriptionAlt2,
    SPELL_ENTRY_DescriptionAlt3,
    SPELL_ENTRY_DescriptionAlt4,
    SPELL_ENTRY_DescriptionAlt5,
    SPELL_ENTRY_DescriptionAlt6,
    SPELL_ENTRY_DescriptionAlt7,
    SPELL_ENTRY_DescriptionAlt8,
    SPELL_ENTRY_DescriptionAlt9,
    SPELL_ENTRY_DescriptionAlt10,
    SPELL_ENTRY_DescriptionAlt11,
    SPELL_ENTRY_DescriptionAlt12,
    SPELL_ENTRY_DescriptionAlt13,
    SPELL_ENTRY_DescriptionAlt14,
    SPELL_ENTRY_DescriptionAlt15,
    SPELL_ENTRY_DescriptionFlags,
    SPELL_ENTRY_BuffDescription,
    SPELL_ENTRY_BuffDescriptionAlt1,
    SPELL_ENTRY_BuffDescriptionAlt2,
    SPELL_ENTRY_BuffDescriptionAlt3,
    SPELL_ENTRY_BuffDescriptionAlt4,
    SPELL_ENTRY_BuffDescriptionAlt5,
    SPELL_ENTRY_BuffDescriptionAlt6,
    SPELL_ENTRY_BuffDescriptionAlt7,
    SPELL_ENTRY_BuffDescriptionAlt8,
    SPELL_ENTRY_BuffDescriptionAlt9,
    SPELL_ENTRY_BuffDescriptionAlt10,
    SPELL_ENTRY_BuffDescriptionAlt11,
    SPELL_ENTRY_BuffDescriptionAlt12,
    SPELL_ENTRY_BuffDescriptionAlt13,
    SPELL_ENTRY_BuffDescriptionAlt14,
    SPELL_ENTRY_BuffDescriptionAlt15,
    SPELL_ENTRY_buffdescflags,
    SPELL_ENTRY_ManaCostPercentage,
    SPELL_ENTRY_unkflags,
    SPELL_ENTRY_StartRecoveryTime,
    SPELL_ENTRY_StartRecoveryCategory,
    SPELL_ENTRY_SpellFamilyName,
    SPELL_ENTRY_SpellGroupType,
    SPELL_ENTRY_unkne,
    SPELL_ENTRY_MaxTargets,
    SPELL_ENTRY_Spell_Dmg_Type,
    SPELL_ENTRY_FG,
    SPELL_ENTRY_FH,
    SPELL_ENTRY_dmg_multiplier_1,
    SPELL_ENTRY_dmg_multiplier_2,
    SPELL_ENTRY_dmg_multiplier_3,
    SPELL_ENTRY_FL,
    SPELL_ENTRY_FM,
    SPELL_ENTRY_FN,
    SPELL_ENTRY_unk201_4,
    SPELL_ENTRY_unk201_5,
    SPELL_ENTRY_unk201_6
};

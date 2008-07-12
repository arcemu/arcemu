#include <stdio.h>
#include <string.h>

#define UNUSED_VALUE_AS_FIX 0x00CDCD00	//max 20 bit precision so floats can hit equal sign ?
//#define UNUSED_VALUE_AS_FIX 0	//max 20 bit precision so floats can hit equal sign ?
#define INSERT_BLOCK_SIZE 100

typedef unsigned __int32 uint32;
typedef signed __int32 int32;
typedef signed __int64 int64;
typedef unsigned __int64 uint64;

//const char* spellentryFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuuuffffffiiiiiiuuuuuuuuuuuuuuufffuuuuuuuuuuuufffuuuuuuuuuuuuuuuifffuuuuuuuPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP";
//const char* spellentryFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuuuffffffiiiiiiuuuuuuuuuuuuuuufffUuuuuuuuuuuufffuuuuuuuuuuuuuuuuuuuifffuuuuuuuUuuuuuuuuuuuuuuuufffffffuuuuuu";
const char* spellentryFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuiuuuuuuuuuuuffffffiiiiiiuuuuuuuuuuuuuuufffuuuuuuuuuuuufffuuuuuuuuuuuuuuuuuuuifffuuuuuusUuususssuuuuuuuussssssssssssss";

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
	uint32 dummy;                           //122
	uint32 CoSpell;                         //123   activeIconID;
	uint32 spellPriority;                   //124
	char* Name;                             //125
	//uint32 NameAlt1;                        //126
	//uint32 NameAlt2;                        //127
	//uint32 NameAlt3;                        //128
	//uint32 NameAlt4;                        //129
	//uint32 NameAlt5;                        //130
	//uint32 NameAlt6;                        //131
	//uint32 NameAlt7;                        //132
	//uint32 NameAlt8;                        //133
	//uint32 NameAlt9;                        //134
	//uint32 NameAlt10;                       //135
	//uint32 NameAlt11;                       //136
	//uint32 NameAlt12;                       //137
	//uint32 NameAlt13;                       //138
	//uint32 NameAlt14;                       //139
	//uint32 NameAlt15;                       //140
	//uint32 NameFlags;                       //141
	char * Rank;                            //142
	//uint32 RankAlt1;                        //143
	//uint32 RankAlt2;                        //144
	//uint32 RankAlt3;                        //145
	//uint32 RankAlt4;                        //146
	//uint32 RankAlt5;                        //147
	//uint32 RankAlt6;                        //148
	//uint32 RankAlt7;                        //149
	//uint32 RankAlt8;                        //150
	//uint32 RankAlt9;                        //151
	//uint32 RankAlt10;                       //152
	//uint32 RankAlt11;                       //153
	//uint32 RankAlt12;                       //154
	//uint32 RankAlt13;                       //155
	//uint32 RankAlt14;                       //156
	//uint32 RankAlt15;                       //157
	//uint32 RankFlags;                       //158
	char * Description;                     //159
	//uint32 DescriptionAlt1;                 //160
	//uint32 DescriptionAlt2;                 //161
	//uint32 DescriptionAlt3;                 //162
	//uint32 DescriptionAlt4;                 //163
	//uint32 DescriptionAlt5;                 //164
	//uint32 DescriptionAlt6;                 //165
	//uint32 DescriptionAlt7;                 //166
	//uint32 DescriptionAlt8;                 //167
	//uint32 DescriptionAlt9;                 //168
	//uint32 DescriptionAlt10;                //169
	//uint32 DescriptionAlt11;                //170
	//uint32 DescriptionAlt12;                //171
	//uint32 DescriptionAlt13;                //172
	//uint32 DescriptionAlt14;                //173
	//uint32 DescriptionAlt15;                //174
	//uint32 DescriptionFlags;                //175
	char * BuffDescription;                 //176
	//uint32 BuffDescriptionAlt1;             //177
	//uint32 BuffDescriptionAlt2;             //178
	//uint32 BuffDescriptionAlt3;             //179
	//uint32 BuffDescriptionAlt4;             //180
	//uint32 BuffDescriptionAlt5;             //181
	//uint32 BuffDescriptionAlt6;             //182
	//uint32 BuffDescriptionAlt7;             //183
	//uint32 BuffDescriptionAlt8;             //184
	//uint32 BuffDescriptionAlt9;             //185
	//uint32 BuffDescriptionAlt10;            //186
	//uint32 BuffDescriptionAlt11;            //187
	//uint32 BuffDescriptionAlt12;            //188
	//uint32 BuffDescriptionAlt13;            //189
	//uint32 BuffDescriptionAlt14;            //190
	//uint32 BuffDescriptionAlt15;            //191
	//uint32 buffdescflags;                   //192
	uint32 ManaCostPercentage;              //193
	uint32 unkflags;                        //194 
	uint32 StartRecoveryTime;               //195
	uint32 StartRecoveryCategory;           //196
	uint32 SpellFamilyName;                 //197
	uint64 SpellGroupType;					//198+199
	uint32 MaxTargets;                      //200 
	uint32 Spell_Dmg_Type;                  //201   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 FG;                              //202   0,1,2 related to Spell_Dmg_Type I think
	int32 FH;                               //203   related to paladin aura's 
	float dmg_multiplier[3];                //204 - 206   if the name is correct I dono
	uint32 FL;                              //207   only one spellid:6994 has this value = 369 UNUSED
	uint32 FM;                              //208   only one spellid:6994 has this value = 4 UNUSED
	uint32 FN;                              //209  3 spells 1 or 2   
	uint32 TotemCategory[2];				//210-211
	uint32 RequiresAreaId;				     		//212 
	uint32 School;						// 213

    /// CUSTOM: these fields are used for the modifications made in the world.cpp
    uint32 DiminishStatus;                  //
    uint32 proc_interval;                   //!!! CUSTOM, <Fill description for variable>
    uint32 buffIndexType;					//!!! CUSTOM, <Fill description for variable>
    uint32 c_is_flags;						//!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
    uint32 buffType;                        //!!! CUSTOM, these are related to creating a item through a spell
    uint32 RankNumber;                      //!!! CUSTOM, this protects players from having >1 rank of a spell
    uint32 NameHash;                        //!!! CUSTOM, related to custom spells, summon spell quest related spells
	uint32 talent_tree;						//!!! CUSTOM,
	uint32 in_front_status;					//!!! CUSTOM,
    uint32 EffectSpellGroupRelation_high[3];     //!!! this is not contained in client dbc but server must have it
	uint32 ThreatForSpell;
	uint32 ProcOnNameHash[3];
	uint32 spell_coef_flags;                                //!!! CUSTOM, store flags for spell coefficient calculations
	float base_range_or_radius_sqr;         //!!! CUSTOM, needed for aoe spells most of the time
	float cone_width;
	float casttime_coef;                                    //!!! CUSTOM, faster spell bonus calculation
	float fixed_dddhcoef;                                   //!!! CUSTOM, fixed DD-DH coefficient for some spells
	float fixed_hotdotcoef;                                 //!!! CUSTOM, fixed HOT-DOT coefficient for some spells
	float Dspell_coef_override;                             //!!! CUSTOM, overrides any spell coefficient calculation and use this value in DD&DH
	float OTspell_coef_override;							//!!! CUSTOM, overrides any spell coefficient calculation and use this value in HOT&DOT
	int self_cast_only;
	int apply_on_shapeshift_change;
	int always_apply;
	int is_melee_spell;					//!!! CUSTOM,
	int is_ranged_spell;					//!!! CUSTOM,
	int spell_can_crit;					//!!! CUSTOM,
};


//this is a short term tool that has  purpuse : compare 2 dbc files and dump diff as an sql file
//check output file, last line is 99% fucked up :P
void main()
{
	FILE		*ref_dbc=fopen("pre_spell_fixed.dbc","rb");
	FILE		*res_dbc=fopen("spell_fixed.dbc","rb");
	FILE		*sql_conv=fopen("fixes.sql","w");
	char		row_buff[10000];
	char		header_buff[10000];
	int			header;
	int			rows;
	int			cols;
	int			useless_shit;
	int			m_stringlength;
	SpellEntry  row_buffer_ref;
	SpellEntry  row_buffer_mod;
	int			*buf_ref=(int*)&row_buffer_ref;
	int			*buf_mod=(int*)&row_buffer_mod;
	
	if(!ref_dbc || !res_dbc || !sql_conv)
	{
		printf("Could not open one of the required files\n");
		return;
	}

	fread(&header,4,1,ref_dbc);
	fread(&rows, 4, 1, ref_dbc);
	fread(&cols, 4, 1, ref_dbc);
	fread(&useless_shit, 4, 1, ref_dbc);
	fread(&m_stringlength, 4, 1, ref_dbc);

	fread(&header,4,1,res_dbc);
	fread(&rows, 4, 1, res_dbc);
	fread(&cols, 4, 1, res_dbc);
	fread(&useless_shit, 4, 1, res_dbc);
	fread(&m_stringlength, 4, 1, res_dbc);

	sprintf(header_buff,"");
	sprintf(header_buff,"%s%s",header_buff,"INSERT INTO `spell_fixes` (`Id`, `Category`, `field4`, `DispelType`, `MechanicsType`, `Attributes`, `AttributesEx`,");
	sprintf(header_buff,"%s%s",header_buff,"`Flags3`, `Flags4`, `Flags5`, `unk201_1`, `unk240_1`, `RequiredShapeShift`, `UNK14`, `Targets`, `TargetCreatureType`,");
	sprintf(header_buff,"%s%s",header_buff,"`RequiresSpellFocus`, `unk240_2`, `CasterAuraState`, `TargetAuraState`, `unk201_2`, `unk201_3`, `CastingTimeIndex`,");
	sprintf(header_buff,"%s%s",header_buff,"`RecoveryTime`, `CategoryRecoveryTime`, `InterruptFlags`, `AuraInterruptFlags`, `ChannelInterruptFlags`, `procFlags`,");
	sprintf(header_buff,"%s%s",header_buff,"`procChance`, `procCharges`, `maxLevel`, `baseLevel`, `spellLevel`, `DurationIndex`, `powerType`, `manaCost`,");
	sprintf(header_buff,"%s%s",header_buff,"`manaCostPerlevel`, `manaPerSecond`, `manaPerSecondPerLevel`, `rangeIndex`, `speed`, `modalNextSpell`, `maxstack`,");
	sprintf(header_buff,"%s%s",header_buff,"`Totem_1`, `Totem_2`, `Reagent_1`, `Reagent_2`, `Reagent_3`, `Reagent_4`, `Reagent_5`, `Reagent_6`, `Reagent_7`,");
	sprintf(header_buff,"%s%s",header_buff,"`Reagent_8`, `ReagentCount_1`, `ReagentCount_2`, `ReagentCount_3`, `ReagentCount_4`, `ReagentCount_5`, `ReagentCount_6`,");
	sprintf(header_buff,"%s%s",header_buff,"`ReagentCount_7`, `ReagentCount_8`, `EquippedItemClass`, `EquippedItemSubClass`, `RequiredItemFlags`, `Effect_1`, `Effect_2`,");
	sprintf(header_buff,"%s%s",header_buff,"`Effect_3`, `EffectDieSides_1`, `EffectDieSides_2`, `EffectDieSides_3`, `EffectBaseDice_1`, `EffectBaseDice_2`, `EffectBaseDice_3`,");
	sprintf(header_buff,"%s%s",header_buff,"`EffectDicePerLevel_1`, `EffectDicePerLevel_2`, `EffectDicePerLevel_3`, `EffectRealPointsPerLevel_1`, `EffectRealPointsPerLevel_2`,");
	sprintf(header_buff,"%s%s",header_buff,"`EffectRealPointsPerLevel_3`, `EffectBasePoints_1`, `EffectBasePoints_2`, `EffectBasePoints_3`, `EffectMechanic_1`, `EffectMechanic_2`,");
	sprintf(header_buff,"%s%s",header_buff,"`EffectMechanic_3`, `EffectImplicitTargetA_1`, `EffectImplicitTargetA_2`, `EffectImplicitTargetA_3`, `EffectImplicitTargetB_1`, `EffectImplicitTargetB_2`, `EffectImplicitTargetB_3`,");
	sprintf(header_buff,"%s%s",header_buff,"`EffectRadiusIndex_1`, `EffectRadiusIndex_2`, `EffectRadiusIndex_3`, `EffectApplyAuraName_1`, `EffectApplyAuraName_2`, `EffectApplyAuraName_3`, `EffectAmplitude_1`, `EffectAmplitude_2`, `EffectAmplitude_3`, `Effectunknown_1`, `Effectunknown_2`,");
	sprintf(header_buff,"%s%s",header_buff,"`Effectunknown_3`, `EffectChainTarget_1`, `EffectChainTarget_2`, `EffectChainTarget_3`, `EffectSpellGroupRelation_1`, `EffectSpellGroupRelation_2`, `EffectSpellGroupRelation_3`, `EffectMiscValue_1`, `EffectMiscValue_2`, `EffectMiscValue_3`, `EffectTriggerSpell_1`, `EffectTriggerSpell_2`, ");
	sprintf(header_buff,"%s%s",header_buff,"`EffectTriggerSpell_3`, `EffectPointsPerComboPoint_1`, `EffectPointsPerComboPoint_2`, `EffectPointsPerComboPoint_3`, `SpellVisual`, `field114`, `dummy`, `CoSpell`, `spellPriority`, `Name`, `Rank`, `Description`, `BuffDescription`, `ManaCostPercentage`, `unkflags`,");
	sprintf(header_buff,"%s%s",header_buff,"`StartRecoveryTime`, `StartRecoveryCategory`, `SpellFamilyName`, `SpellGroupType`, `SpellGroupType_high`, `MaxTargets`, `Spell_Dmg_Type`, `FG`, `FH`, `dmg_multiplier_1`, `dmg_multiplier_2`, `dmg_multiplier_3`, `FL`, `FM`, `FN`, `TotemCategory_1`, `TotemCategory_2`,");
	sprintf(header_buff,"%s%s",header_buff,"`RequiresAreaId`, `School`, `DiminishStatus`, `proc_interval`, `buffIndexType`, `c_is_flags`, `buffType`, `RankNumber`, `NameHash`, `talent_tree`, `in_front_status`, `EffectSpellGroupRelation_high_1`, `EffectSpellGroupRelation_high_2`,");
	sprintf(header_buff,"%s%s",header_buff,"`EffectSpellGroupRelation_high_3`, `ThreatForSpell`, `ProcOnNameHash_1`, `ProcOnNameHash_2`, `ProcOnNameHash_3`, `spell_coef_flags`,");
	sprintf(header_buff,"%s%s",header_buff,"`base_range_or_radius_sqr`, `cone_width`, `casttime_coef`, `fixed_dddhcoef`, `fixed_hotdotcoef`, `Dspell_coef_override`,");
	sprintf(header_buff,"%s%s",header_buff,"`OTspell_coef_override`, `self_cast_only`, `apply_on_shapeshift_change`, `always_apply`, `is_melee_spell`, `is_ranged_spell`, `spell_can_crit` ");
//	sprintf(header_buff,"%s%s",header_buff,"`,cast_on_mob_entry_1`, `tp_x`, `tp_y`, `tp_z`, `proc_inherit_from_spell_1`,");
//	sprintf(header_buff,"%s%s",header_buff,"`,proc_inherit_from_spell_2`, `proc_inherit_from_spell_3`, `is_positive`");
	sprintf(header_buff,"%s%s",header_buff,") VALUES \n");

	int col_count = sizeof(SpellEntry)/sizeof(int); //nasty i know :P
	col_count -= 1; // 1 extra column to be safe
	int ins_count=0;
	for(int i=0;i<rows;i++)
	{
		//reset row content
//		sprintf(row_buff,"(",row_buff);
		row_buff[0]=0;

		//read next 2 rows to compares
		fread(&row_buffer_ref,sizeof(SpellEntry),1,ref_dbc);
		fread(&row_buffer_mod,sizeof(SpellEntry),1,res_dbc);

		//compare and build up changes
		int has_change=0;
		for(int j=1;j<col_count;j++)
		{
//			*(float*)&buf_mod[j] = j;
//			*(int*)&buf_mod[j] = j;
			if(
				buf_ref[j]!=buf_mod[j]
				&& spellentryFormat[j]!='s'
				&& !(spellentryFormat[j]=='U' && buf_mod[j]==-1)
				)
			{
				if(spellentryFormat[j]=='f')
					sprintf(row_buff,"%s'%f',",row_buff,*(float*)&buf_mod[j]);
				else if(spellentryFormat[j]=='u')
					sprintf(row_buff,"%s'%u',",row_buff,*(unsigned int*)&buf_mod[j]);
				else 
					sprintf(row_buff,"%s'%d',",row_buff,buf_mod[j]);
				has_change = 1;
			}
			else 
				sprintf(row_buff,"%s'%u',",row_buff,UNUSED_VALUE_AS_FIX);
		}

		//skip inserting to db where no changes have been made
		if( has_change==0 )
			continue;

		ins_count++;

		//cut the last ","
		row_buff[strlen(row_buff)-1]=0;

		//new block ?
		if( ins_count == 1 )
			fprintf(sql_conv,"%s",header_buff);

		//we group sqls so server won't crap out on inserting
		if( ins_count % INSERT_BLOCK_SIZE == 0 || i == rows - 1 )
		{
			sprintf(row_buff,"%s);",row_buff);
			ins_count = 0;
		}
		else
			sprintf(row_buff,"%s),",row_buff);

		//finally add to file the new insert with id 
		fprintf(sql_conv,"(%u,%s\n",buf_mod[0],row_buff);
	}


	fclose(ref_dbc);
	fclose(res_dbc);
	fclose(sql_conv);
}	
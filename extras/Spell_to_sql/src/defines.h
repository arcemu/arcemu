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

#define SPELL_DBC_CLIENT_BUILD 11723 // 3.3.3

#define SQL_INSERTS_PER_QUERY 1000

//this might change from 1 version to another of the DBC
#define SPELL_COLUMN_COUNT 234
//last column is "skip_this_for_sql"

const char sql_translation_table[SPELL_COLUMN_COUNT][3][300] = 
{
	{"uint32",	"Id", "0"},								//1
	{"uint32",	"Category", "0"},						//2
	{"uint32",	"DispelType", "0"},						//3
	{"uint32",	"MechanicsType", "0"},				 	//4
	{"uint32",	"Attributes", "0"},						//5
	{"uint32",	"AttributesEx", "0"},					//6
	{"uint32",	"AttributesExB", "0"},				 	//7
	{"uint32",	"AttributesExC", "0"},				 	//8
	{"uint32",	"AttributesExD", "0"},				 	//9
	{"uint32",	"AttributesExE", "0"},				 	//10
	{"uint32",	"AttributesExF", "0"},				 	//11
	{"uint32",	"UNK_320_1", "0"},			 			//12 added in 3.2
	{"uint32",	"RequiredShapeShift", "0"},				//13 - Flags/BitMask for shapeshifts(stances/shadowform/boomkin/etc)
	{"uint32",	"UNK_320_2", "0"},						//14 added in 3.2
	{"uint32",	"ShapeshiftExclude", "0"},			 	//15 - Flags/BitMask for shapeshifts in which this spell can NOT be used in
	{"uint32",	"UNK_320_3", "0"},						//16 added in 3.2
	{"uint32",	"Targets", "0"},						//17
	{"uint32",	"TargetCreatureType", "0"},				//18
	{"uint32",	"RequiresSpellFocus", "0"},				//19
	{"uint32",	"FacingCasterFlags", "0"},			 	//20
	{"uint32",	"CasterAuraState", "0"},				//21
	{"uint32",	"TargetAuraState", "0"},				//22
	{"uint32",	"CasterAuraStateNot", "0"},				//23
	{"uint32",	"TargetAuraStateNot", "0"},				//24
	{"uint32",	"casterAuraSpell", "0"},				//25
	{"uint32",	"targetAuraSpell", "0"},				//26
	{"uint32",	"casterAuraSpellNot", "0"},				//27
	{"uint32",	"targetAuraSpellNot", "0"},				//28
	{"uint32",	"CastingTimeIndex", "0"},				//29
	{"uint32",	"RecoveryTime", "0"},					//30
	{"uint32",	"CategoryRecoveryTime", "0"},			//31 - recoverytime
	{"uint32",	"InterruptFlags", "0"},					//32
	{"uint32",	"AuraInterruptFlags", "0"},				//33
	{"uint32",	"ChannelInterruptFlags", "0"},		 	//34
	{"uint32",	"procFlags", "0"},					 	//35
	{"uint32",	"procChance", "0"},						//36
	{"int32",	"procCharges", "0"},					//37
	{"uint32",	"maxLevel", "0"},						//38
	{"uint32",	"baseLevel", "0"},					 	//39
	{"uint32",	"spellLevel", "0"},						//40
	{"uint32",	"DurationIndex", "0"},				 	//41
	{"uint32",	"powerType", "0"},					 	//42
	{"uint32",	"manaCost", "0"},						//43
	{"uint32",	"manaCostPerlevel", "0"},				//44
	{"uint32",	"manaPerSecond", "0"},				 	//45
	{"uint32",	"manaPerSecondPerLevel", "0"},		 	//46
	{"uint32",	"rangeIndex", "0"},						//47
	{"float",	"speed", "0"},							//48
	{"uint32",	"modalNextSpell", "0"},					//49
	{"uint32",	"maxstack", "0"},						//50
	{"uint32",	"Totem_1", "0"},						//51
	{"uint32",	"Totem_2", "0"},						//52
	{"uint32",	"Reagent_1","0"},						//53
	{"uint32",	"Reagent_2","0"},						//54
	{"uint32",	"Reagent_3","0"},						//55
	{"uint32",	"Reagent_4","0"},						//56
	{"uint32",	"Reagent_5","0"},						//57
	{"uint32",	"Reagent_6","0"},						//58
	{"uint32",	"Reagent_7","0"},						//59
	{"uint32",	"Reagent_8","0"},						//60
	{"uint32",	"ReagentCount_1","0"},					//61
	{"uint32",	"ReagentCount_2","0"},					//62
	{"uint32",	"ReagentCount_3","0"},					//63
	{"uint32",	"ReagentCount_4","0"},					//64
	{"uint32",	"ReagentCount_5","0"},					//65
	{"uint32",	"ReagentCount_6","0"},					//66
	{"uint32",	"ReagentCount_7","0"},					//67
	{"uint32",	"ReagentCount_8","0"},					//68
	{"int32",	"EquippedItemClass", "0"},				//69
	{"uint32",	"EquippedItemSubClass", "0"},			//70
	{"uint32",	"RequiredItemFlags", "0"},				//71
	{"uint32",	"Effect_1", "0"},						//72
	{"uint32",	"Effect_2", "0"},						//73
	{"uint32",	"Effect_3", "0"},						//74
	{"uint32",	"EffectDieSides_1", "0"},				//75
	{"uint32",	"EffectDieSides_2", "0"},				//76
	{"uint32",	"EffectDieSides_3", "0"},				//77
	{"float",	"EffectRealPointsPerLevel_1", "0"},		//84
	{"float",	"EffectRealPointsPerLevel_2", "0"},		//85
	{"float",	"EffectRealPointsPerLevel_3", "0"},		//86
	{"int32",	"EffectBasePoints_1", "0"},				//87
	{"int32",	"EffectBasePoints_2", "0"},				//88
	{"int32",	"EffectBasePoints_3", "0"},				//89
	{"int32",	"EffectMechanic_1", "0"},				//90 - Related to SpellMechanic.dbc
	{"int32",	"EffectMechanic_2", "0"},				//91 - Related to SpellMechanic.dbc
	{"int32",	"EffectMechanic_3", "0"},				//92 - Related to SpellMechanic.dbc
	{"uint32",	"EffectImplicitTargetA_1", "0"},		//93
	{"uint32",	"EffectImplicitTargetA_2", "0"},		//94
	{"uint32",	"EffectImplicitTargetA_3", "0"},		//95
	{"uint32",	"EffectImplicitTargetB_1", "0"},		//96
	{"uint32",	"EffectImplicitTargetB_2", "0"},		//97
	{"uint32",	"EffectImplicitTargetB_3", "0"},		//98
	{"uint32",	"EffectRadiusIndex_1", "0"},			//99
	{"uint32",	"EffectRadiusIndex_2", "0"},			//100
	{"uint32",	"EffectRadiusIndex_3", "0"},			//101
	{"uint32",	"EffectApplyAuraName_1", "0"},			//102
	{"uint32",	"EffectApplyAuraName_2", "0"},			//103
	{"uint32",	"EffectApplyAuraName_3", "0"},			//104
	{"uint32",	"EffectAmplitude_1", "0"},				//105
	{"uint32",	"EffectAmplitude_2", "0"},				//106
	{"uint32",	"EffectAmplitude_3", "0"},				//107
	{"float",	"EffectMultipleValue_1", "0"},			//108 - This value is the $ value from description
	{"float",	"EffectMultipleValue_2", "0"},			//109 - This value is the $ value from description
	{"float",	"EffectMultipleValue_3", "0"},			//110 - This value is the $ value from description
	{"uint32",	"EffectChainTarget_1", "0"},			//111
	{"uint32",	"EffectChainTarget_2", "0"},			//112
	{"uint32",	"EffectChainTarget_3", "0"},			//113
	{"uint32",	"EffectItemType_1", "0"},			 	//114 - Not sure maybe we should rename it. its the relation to field: SpellGroupType
	{"uint32",	"EffectItemType_2", "0"},			 	//115 - Not sure maybe we should rename it. its the relation to field: SpellGroupType
	{"uint32",	"EffectItemType_3", "0"},			 	//116 - Not sure maybe we should rename it. its the relation to field: SpellGroupType
	{"uint32",	"EffectMiscValue_1", "0"},				//117
	{"uint32",	"EffectMiscValue_2", "0"},				//118
	{"uint32",	"EffectMiscValue_3", "0"},				//119
	{"uint32",	"EffectMiscValueB_1", "0"},				//120
	{"uint32",	"EffectMiscValueB_2", "0"},				//121
	{"uint32",	"EffectMiscValueB_3", "0"},				//122
	{"uint32",	"EffectTriggerSpell_1", "0"},		 	//123
	{"uint32",	"EffectTriggerSpell_2", "0"},		 	//124
	{"uint32",	"EffectTriggerSpell_3", "0"},		 	//125
	{"float",	"EffectPointsPerComboPoint_1", "0"},	//126
	{"float",	"EffectPointsPerComboPoint_2", "0"},	//127
	{"float",	"EffectPointsPerComboPoint_3", "0"},	//128
	{"uint32",	"EffectSpellClassMask_1_1", "0"},		//129
	{"uint32",	"EffectSpellClassMask_1_2", "0"},		//130
	{"uint32",	"EffectSpellClassMask_1_3", "0"},		//131
	{"uint32",	"EffectSpellClassMask_2_1", "0"},		//132
	{"uint32",	"EffectSpellClassMask_2_2", "0"},		//133
	{"uint32",	"EffectSpellClassMask_2_3", "0"},		//134
	{"uint32",	"EffectSpellClassMask_3_1", "0"},		//135
	{"uint32",	"EffectSpellClassMask_3_2", "0"},		//136
	{"uint32",	"EffectSpellClassMask_3_3", "0"},		//137
	{"uint32",	"SpellVisual", "0"},					//138
	{"uint32",	"field114", "0"},						//139
	{"uint32",	"spellIconID", "0"},					//140
	{"uint32",	"activeIconID", "0"},					//141
	{"uint32",	"spellPriority", "0"},				 	//142
	{"str",		"Name", "0"},							//143
	{"uint32",	"NameAlt1","1"},						//144
	{"uint32",	"NameAlt2","1"},						//145
	{"uint32",	"NameAlt3","1"},						//146
	{"uint32",	"NameAlt4","1"},						//147
	{"uint32",	"NameAlt5","1"},						//148
	{"uint32",	"NameAlt6","1"},						//149
	{"uint32",	"NameAlt7","1"},						//150
	{"uint32",	"NameAlt8","1"},						//151
	{"uint32",	"NameAlt9","1"},						//152
	{"uint32",	"NameAlt10","1"},						//153
	{"uint32",	"NameAlt11","1"},						//154
	{"uint32",	"NameAlt12","1"},						//155
	{"uint32",	"NameAlt13","1"},						//156
	{"uint32",	"NameAlt14","1"},						//157
	{"uint32",	"NameAlt15","1"},						//158
	{"uint32",	"NameFlags", "0"},					 	//159
	{"str",		"Rank", "0"},							//160
	{"uint32",	"RankAlt1","1"},						//161
	{"uint32",	"RankAlt2","1"},						//162
	{"uint32",	"RankAlt3","1"},						//163
	{"uint32",	"RankAlt4","1"},						//164
	{"uint32",	"RankAlt5","1"},						//165
	{"uint32",	"RankAlt6","1"},						//166
	{"uint32",	"RankAlt7","1"},						//167
	{"uint32",	"RankAlt8","1"},						//168
	{"uint32",	"RankAlt9","1"},						//169
	{"uint32",	"RankAlt10","1"},						//170
	{"uint32",	"RankAlt11","1"},						//171
	{"uint32",	"RankAlt12","1"},						//172
	{"uint32",	"RankAlt13","1"},						//173
	{"uint32",	"RankAlt14","1"},						//174
	{"uint32",	"RankAlt15","1"},						//175
	{"uint32",	"RankFlags", "0"},					 	//176
	{"str",		"Description", "0"},					//177
	{"uint32",	"DescriptionAlt1","1"},					//178
	{"uint32",	"DescriptionAlt2","1"},					//179
	{"uint32",	"DescriptionAlt3","1"},					//180
	{"uint32",	"DescriptionAlt4","1"},					//181
	{"uint32",	"DescriptionAlt5","1"},					//182
	{"uint32",	"DescriptionAlt6","1"},					//183
	{"uint32",	"DescriptionAlt7","1"},					//184
	{"uint32",	"DescriptionAlt8","1"},					//185
	{"uint32",	"DescriptionAlt9","1"},					//186
	{"uint32",	"DescriptionAlt10","1"},				//187
	{"uint32",	"DescriptionAlt11","1"},				//188
	{"uint32",	"DescriptionAlt12","1"},				//189
	{"uint32",	"DescriptionAlt13","1"},				//190
	{"uint32",	"DescriptionAlt14","1"},				//191
	{"uint32",	"DescriptionAlt15","1"},				//192
	{"uint32",	"DescriptionFlags", "0"},				//193
	{"str",		"BuffDescription", "0"},				//194
	{"uint32",	"BuffDescriptionAlt1","1"},				//195
	{"uint32",	"BuffDescriptionAlt2","1"},				//196
	{"uint32",	"BuffDescriptionAlt3","1"},				//197
	{"uint32",	"BuffDescriptionAlt4","1"},				//198
	{"uint32",	"BuffDescriptionAlt5","1"},				//199
	{"uint32",	"BuffDescriptionAlt6","1"},				//200
	{"uint32",	"BuffDescriptionAlt7","1"},				//201
	{"uint32",	"BuffDescriptionAlt8","1"},				//202
	{"uint32",	"BuffDescriptionAlt9","1"},				//203
	{"uint32",	"BuffDescriptionAlt10","1"},			//204
	{"uint32",	"BuffDescriptionAlt11","1"},			//205
	{"uint32",	"BuffDescriptionAlt12","1"},			//206
	{"uint32",	"BuffDescriptionAlt13","1"},			//207
	{"uint32",	"BuffDescriptionAlt14","1"},			//208
	{"uint32",	"BuffDescriptionAlt15","1"},			//209
	{"uint32",	"buffdescflags", "0"},				 	//210
	{"uint32",	"ManaCostPercentage", "0"},				//211
	{"uint32",	"StartRecoveryTime", "0"},			 	//212
	{"uint32",	"StartRecoveryCategory", "0"},		 	//213
	{"uint32",	"MaxTargetLevel", "0"},					//214
	{"uint32",	"SpellFamilyName", "0"},				//215
	{"uint32",	"SpellGroupType_1", "0"},			 	//216
	{"uint32",	"SpellGroupType_2", "0"},			 	//217
	{"uint32",	"SpellGroupType_3", "0"},			 	//218
	{"uint32",	"MaxTargets", "0"},						//219
	{"uint32",	"Spell_Dmg_Type", "0"},					//220	dmg_class Integer	0=None, 1=Magic, 2=Melee, 3=Ranged
	{"uint32",	"PreventionType", "0"},					//221	0,1,2 related to Spell_Dmg_Type I think
	{"int32",	"StanceBarOrder", "0"},					//222	related to paladin aura's
	{"float",	"dmg_multiplier_1", "0"},				//223
	{"float",	"dmg_multiplier_2", "0"},				//224
	{"float",	"dmg_multiplier_3", "0"},				//225
	{"uint32",	"MinFactionID", "0"},					//226 - only one spellid:6994 has this value = 369 UNUSED
	{"uint32",	"MinReputation", "0"},				 	//227 - only one spellid:6994 has this value = 4 UNUSED
	{"uint32",	"RequiredAuraVision", "0"},				//228 - 3 spells 1 or 2
	{"uint32",	"TotemCategory_1", "0"},				//229
	{"uint32",	"TotemCategory_2", "0"},				//230
	{"int32",	"RequiresAreaId", "0"},					//231
	{"uint32",	"School", "0"},							//232
	{"uint32",	"RuneCostID", "0"},						//233
	{"uint32",	"SpellMissileID", "0"},					//234
	{"uint32",	"PowerDisplayId", "0"},					//235 - Points to PowerDisplay.dbc (related to vehicles?)
	{"uint32",	"UNK4", "0"},							//236
	{"uint32",	"UNK5", "0"},							//237
	{"uint32",	"UNK6", "0"},							//238
	{"uint32",	"UNK7", "0"},							//239
	{"uint32",	"UNK8", "0"},							//240
};

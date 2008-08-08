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

#define SPELL_DBC_CLIENT_NUMBER "2.4.3"

#define SQL_INSERTS_PER_QUERY 1000

//this might change from 1 version to another of the DBC
#define SPELL_COLUMN_COUNT 216
//last column is "skip_this_for_sql"
const char sql_translation_table[220][3][300] = 
{
    {"uint32" ,"Id","0"},                              //1
    {"uint32" ,"Category","0"},                        //2
    {"uint32" ,"castUI","0"},                          //3 // something like spelltype 0: general spells 1: Pet spells 2: Disguise / transormation spells 3: enchantment spells
    {"uint32" ,"DispelType","0"},                      //4
    {"uint32" ,"MechanicsType","0"},                   //5
    {"uint32" ,"Attributes","0"},                      //6
    {"uint32" ,"AttributesEx","0"},                    //7
    {"uint32" ,"Flags3","0"},                          //8
    {"uint32" ,"Flags4","0"},                          //9  // Flags to
    {"uint32" ,"Flags5","0"},                          //10 // Flags....
    {"uint32" ,"Flags6","0"},                          //11 // Flags 2.0.1 unknown one
    {"uint32" ,"Flags7","0"},                          //12 // Flags 2.0.1 unknown one
    {"uint32" ,"RequiredShapeShift","0"},              //13 // Flags BitMask for shapeshift spells
    {"uint32" ,"ShapeshiftExclude","0"},               //14 // -> this is wrong // Flags BitMask for which shapeshift forms this spell can NOT be used in.
    {"uint32" ,"Targets","0"},                         //15 // - N / M
    {"uint32" ,"TargetCreatureType","0"},              //16
    {"uint32" ,"RequiresSpellFocus","0"},              //17
    {"uint32" ,"FacingCasterFlags","0"},               //18
    {"uint32" ,"CasterAuraState","0"},                 //19
    {"uint32" ,"TargetAuraState","0"},                 //20
    {"uint32" ,"ExcludeCasterAuraState","0"},          //21 // 2.0.1 unknown two
    {"uint32" ,"ExcludeTargetAuraState","0"},          //22 // 2.0.1 unknown three
    {"uint32" ,"CastingTimeIndex","0"},                //23
    {"uint32" ,"RecoveryTime","0"},                    //24
    {"uint32" ,"CategoryRecoveryTime","0"},            //25 // recoverytime
    {"uint32" ,"InterruptFlags","0"},                  //26
    {"uint32" ,"AuraInterruptFlags","0"},              //27
    {"uint32" ,"ChannelInterruptFlags","0"},           //28
    {"uint32" ,"procFlags","0"},                       //29
    {"uint32" ,"procChance","0"},                      //30
    {"int32"  ,"procCharges","0"},                     //31
    {"uint32" ,"maxLevel","0"},                        //32
    {"uint32" ,"baseLevel","0"},                       //33
    {"uint32" ,"spellLevel","0"},                      //34
    {"uint32" ,"DurationIndex","0"},                   //35
    {"uint32" ,"powerType","0"},                       //36
    {"uint32" ,"manaCost","0"},                        //37
    {"uint32" ,"manaCostPerlevel","0"},                //38
    {"uint32" ,"manaPerSecond","0"},                   //39
    {"uint32" ,"manaPerSecondPerLevel","0"},           //40
    {"uint32" ,"rangeIndex","0"},                      //41
    {"float"  ,"speed","0"},                           //42
    {"uint32" ,"modalNextSpell","0"},                  //43
    {"uint32" ,"maxstack","0"},                        //44
    {"uint32" ,"Totem_1","0"},                         //45 - 46
    {"uint32" ,"Totem_2","0"},                         //45 - 46
    {"uint32" ,"Reagent_1","0"},                       //47 - 54
    {"uint32" ,"Reagent_2","0"},                       //47 - 54
    {"uint32" ,"Reagent_3","0"},                       //47 - 54
    {"uint32" ,"Reagent_4","0"},                       //47 - 54
    {"uint32" ,"Reagent_5","0"},                       //47 - 54
    {"uint32" ,"Reagent_6","0"},                       //47 - 54
    {"uint32" ,"Reagent_7","0"},                       //47 - 54
    {"uint32" ,"Reagent_8","0"},                       //47 - 54
    {"uint32" ,"ReagentCount_1","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_2","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_3","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_4","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_5","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_6","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_7","0"},                  //55 - 62
    {"uint32" ,"ReagentCount_8","0"},                  //55 - 62
    {"uint32" ,"EquippedItemClass","0"},               //63
    {"uint32" ,"EquippedItemSubClass","0"},            //64
    {"uint32" ,"RequiredItemFlags","0"},               //65
    {"uint32" ,"Effect_1","0"},                        //66 - 68
    {"uint32" ,"Effect_2","0"},                        //66 - 68
    {"uint32" ,"Effect_3","0"},                        //66 - 68
    {"uint32" ,"EffectDieSides_1","0"},                //69 - 71
    {"uint32" ,"EffectDieSides_2","0"},                //69 - 71
    {"uint32" ,"EffectDieSides_3","0"},                //69 - 71
    {"uint32" ,"EffectBaseDice_1","0"},                //72 - 74
    {"uint32" ,"EffectBaseDice_2","0"},                //72 - 74
    {"uint32" ,"EffectBaseDice_3","0"},                //72 - 74
    {"float"  ,"EffectDicePerLevel_1","0"},            //75 - 77
    {"float"  ,"EffectDicePerLevel_2","0"},            //75 - 77
    {"float"  ,"EffectDicePerLevel_3","0"},            //75 - 77
    {"float"  ,"EffectRealPointsPerLevel_1","0"},      //78 - 80
    {"float"  ,"EffectRealPointsPerLevel_2","0"},      //78 - 80
    {"float"  ,"EffectRealPointsPerLevel_3","0"},      //78 - 80
    {"int32"  ,"EffectBasePoints_1","0"},              //81 - 83
    {"int32"  ,"EffectBasePoints_2","0"},              //81 - 83
    {"int32"  ,"EffectBasePoints_3","0"},              //81 - 83
    {"int32"  ,"EffectMechanic_1","0"},                //84 - 86
    {"int32"  ,"EffectMechanic_2","0"},                //84 - 86
    {"int32"  ,"EffectMechanic_3","0"},                //84 - 86
    {"uint32" ,"EffectImplicitTargetA_1","0"},         //87 - 89
    {"uint32" ,"EffectImplicitTargetA_2","0"},         //87 - 89
    {"uint32" ,"EffectImplicitTargetA_3","0"},         //87 - 89
    {"uint32" ,"EffectImplicitTargetB_1","0"},         //90 - 92
    {"uint32" ,"EffectImplicitTargetB_2","0"},         //90 - 92
    {"uint32" ,"EffectImplicitTargetB_3","0"},         //90 - 92
    {"uint32" ,"EffectRadiusIndex_1","0"},             //93 - 95
    {"uint32" ,"EffectRadiusIndex_2","0"},             //93 - 95
    {"uint32" ,"EffectRadiusIndex_3","0"},             //93 - 95
    {"uint32" ,"EffectApplyAuraName_1","0"},           //96 - 98
    {"uint32" ,"EffectApplyAuraName_2","0"},           //96 - 98
    {"uint32" ,"EffectApplyAuraName_3","0"},           //96 - 98
    {"uint32" ,"EffectAmplitude_1","0"},               //99 - 101
    {"uint32" ,"EffectAmplitude_2","0"},               //99 - 101
    {"uint32" ,"EffectAmplitude_3","0"},               //99 - 101
    {"float"  ,"EffectAmplitude_1_blizz","0"},         //102 - 104
    {"float"  ,"EffectAmplitude_2_blizz","0"},         //102 - 104
    {"float"  ,"EffectAmplitude_3_blizz","0"},         //102 - 104 // This value is the $ value from description
    {"uint32" ,"EffectChainTarget_1","0"},             //105 - 107
    {"uint32" ,"EffectChainTarget_2","0"},             //105 - 107
    {"uint32" ,"EffectChainTarget_3","0"},             //105 - 107
    {"uint32" ,"EffectSpellGroupRelation_1","0"},      //108 - 110
    {"uint32" ,"EffectSpellGroupRelation_2","0"},      //108 - 110
    {"uint32" ,"EffectSpellGroupRelation_3","0"},      //108 - 110
    {"uint32" ,"EffectMiscValue_1","0"},               //111 - 113
    {"uint32" ,"EffectMiscValue_2","0"},               //111 - 113
    {"uint32" ,"EffectMiscValue_3","0"},               //111 - 113
	{"uint32" ,"EffectMiscValueB_1","0"},			   //114 - 116 // 2.4.3
	{"uint32" ,"EffectMiscValueB_2","0"},			   //114 - 116 // 2.4.3
	{"uint32" ,"EffectMiscValueB_3","0"},			   //114 - 116 // 2.4.3
    {"uint32" ,"EffectTriggerSpell_1","0"},            //117 - 119
    {"uint32" ,"EffectTriggerSpell_2","0"},            //117 - 119
    {"uint32" ,"EffectTriggerSpell_3","0"},            //117 - 119
    {"float"  ,"EffectPointsPerComboPoint_1","0"},     //120 - 122
    {"float"  ,"EffectPointsPerComboPoint_2","0"},     //120 - 122
    {"float"  ,"EffectPointsPerComboPoint_3","0"},     //120 - 122
    {"uint32" ,"SpellVisual","0"},                     //123
    {"uint32" ,"field114","0"},			               //124
    {"uint32" ,"spellIconID","0"},                     //125
    {"uint32" ,"activeIconID","0"},                    //126
    {"uint32" ,"spellPriority","0"},                   //127
    {"str"	  ,"Name","0"},                            //128
    {"uint32" ,"NameAlt1","1"},                        //129
    {"uint32" ,"NameAlt2","1"},                        //130
    {"uint32" ,"NameAlt3","1"},                        //131
    {"uint32" ,"NameAlt4","1"},                        //132
    {"uint32" ,"NameAlt5","1"},                        //133
    {"uint32" ,"NameAlt6","1"},                        //134
    {"uint32" ,"NameAlt7","1"},                        //135
    {"uint32" ,"NameAlt8","1"},                        //136
    {"uint32" ,"NameAlt9","1"},                        //137
    {"uint32" ,"NameAlt10","1"},                       //138
    {"uint32" ,"NameAlt11","1"},                       //139
    {"uint32" ,"NameAlt12","1"},                       //140
    {"uint32" ,"NameAlt13","1"},                       //141
    {"uint32" ,"NameAlt14","1"},                       //142
    {"uint32" ,"NameAlt15","1"},                       //143
    {"uint32" ,"NameFlags","0"},                       //144
    {"str"	  ,"Rank","0"},                            //145
    {"uint32" ,"RankAlt1","1"},                        //146
    {"uint32" ,"RankAlt2","1"},                        //147
    {"uint32" ,"RankAlt3","1"},                        //148
    {"uint32" ,"RankAlt4","1"},                        //149
    {"uint32" ,"RankAlt5","1"},                        //150
    {"uint32" ,"RankAlt6","1"},                        //151
    {"uint32" ,"RankAlt7","1"},                        //152
    {"uint32" ,"RankAlt8","1"},                        //153
    {"uint32" ,"RankAlt9","1"},                        //154
    {"uint32" ,"RankAlt10","1"},                       //155
    {"uint32" ,"RankAlt11","1"},                       //156
    {"uint32" ,"RankAlt12","1"},                       //157
    {"uint32" ,"RankAlt13","1"},                       //158
    {"uint32" ,"RankAlt14","1"},                       //159
    {"uint32" ,"RankAlt15","1"},                       //160
    {"uint32" ,"RankFlags","0"},                       //161
    {"str"	  ,"Description","0"},                     //162
    {"uint32" ,"DescriptionAlt1","1"},                 //163
    {"uint32" ,"DescriptionAlt2","1"},                 //164
    {"uint32" ,"DescriptionAlt3","1"},                 //165
    {"uint32" ,"DescriptionAlt4","1"},                 //166
    {"uint32" ,"DescriptionAlt5","1"},                 //167
    {"uint32" ,"DescriptionAlt6","1"},                 //168
    {"uint32" ,"DescriptionAlt7","1"},                 //169
    {"uint32" ,"DescriptionAlt8","1"},                 //170
    {"uint32" ,"DescriptionAlt9","1"},                 //171
    {"uint32" ,"DescriptionAlt10","1"},                //172
    {"uint32" ,"DescriptionAlt11","1"},                //173
    {"uint32" ,"DescriptionAlt12","1"},                //174
    {"uint32"  ,"DescriptionAlt13","1"},               //175
    {"uint32"  ,"DescriptionAlt14","1"},               //176
    {"uint32"  ,"DescriptionAlt15","1"},               //177
    {"uint32"  ,"DescriptionFlags","0"},               //178
    {"str"	   ,"BuffDescription","0"},                //179
    {"uint32"  ,"BuffDescriptionAlt1","1"},            //180
    {"uint32"  ,"BuffDescriptionAlt2","1"},            //181
    {"uint32"  ,"BuffDescriptionAlt3","1"},            //182
    {"uint32"  ,"BuffDescriptionAlt4","1"},            //183
    {"uint32"  ,"BuffDescriptionAlt5","1"},            //184
    {"uint32"  ,"BuffDescriptionAlt6","1"},            //185
    {"uint32"  ,"BuffDescriptionAlt7","1"},            //186
    {"uint32"  ,"BuffDescriptionAlt8","1"},            //187
    {"uint32"  ,"BuffDescriptionAlt9","1"},            //188
    {"uint32"  ,"BuffDescriptionAlt10","1"},           //189
    {"uint32"  ,"BuffDescriptionAlt11","1"},           //190
    {"uint32"  ,"BuffDescriptionAlt12","1"},           //191
    {"uint32"  ,"BuffDescriptionAlt13","1"},           //192
    {"uint32"  ,"BuffDescriptionAlt14","1"},           //193
	{"uint32"  ,"BuffDescriptionAlt15","1"},           //194
    {"uint32"  ,"buffdescflags","0"},                  //195
    {"uint32"  ,"ManaCostPercentage","0"},             //196
    {"uint32"  ,"unkflags","0"},                       //197 
    {"uint32"  ,"StartRecoveryTime","0"},              //198
    {"uint32"  ,"StartRecoveryCategory","0"},          //199
    {"uint32"  ,"SpellFamilyName","0"},                //200
    {"uint32"  ,"SpellGroupType","0"},                 //201   // flags 
    {"uint32"  ,"SpellGroupType_high","0"},            //202   // flags
    {"uint32"  ,"MaxTargets","0"},                     //203 
    {"uint32"  ,"Spell_Dmg_Type","0"},                 //204   // dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
    {"uint32"  ,"PreventionType","0"},                 //205   // 0,1,2 related to Spell_Dmg_Type I think
    {"int32"   ,"StanceBarOrder","0"},                 //206   // related to paladin aura's 
    {"float"   ,"dmg_multiplier_1","0"},               //207 - 209   // if the name is correct I dono
    {"float"   ,"dmg_multiplier_2","0"},               //207 - 209   // if the name is correct I dono
    {"float"   ,"dmg_multiplier_3","0"},               //207 - 209   // if the name is correct I dono
    {"uint32"  ,"MinFactionID","0"},                   //210   // only one spellid:6994 has this value = 369
    {"uint32"  ,"MinReputation","0"},                  //211   // only one spellid:6994 has this value = 4
    {"uint32"  ,"RequiredAuraVision","0"},             //212   // only one spellid:26869  has this flag = 1   
    {"uint32"  ,"TotemCategory_1","0"},				   //213
    {"uint32"  ,"TotemCategory_2","0"},                //214
    {"uint32"  ,"RequiresAreaId","0"},				   //215
    {"uint32"  ,"schoolMask","0"},					   //216
};
/**/

/*
#define SPELL_COLUMN_COUNT 218
const char sql_translation_table[220][3][300] = 
{
    {"uint32" ,"Id","0"},                              //1
    {"uint32" ,"Category","0"},                        //3
    {"uint32" ,"castUI","0"},                          //4 something like spelltype 0: general spells 1: Pet spells 2: Disguise / transormation spells 3: enchantment spells
    {"uint32" ,"DispelType","0"},                      //5
    {"uint32" ,"MechanicsType","0"},                   //6
    {"uint32" ,"Attributes","0"},                      //7
    {"uint32" ,"AttributesEx","0"},                    //8
    {"uint32" ,"Flags3","0"},                          //9
    {"uint32" ,"Flags4","0"},                          //10 // Flags to
    {"uint32" ,"Flags5","0"},                          //11 // Flags....
    {"uint32" ,"Flags6","0"},                          //12 // Flags 2.0.1 unknown one
    {"uint32" ,"Flags7","0"},                          //12 + 1// Flags 2.0.1 unknown one
    {"uint32" ,"RequiredShapeShift","0"},              //13 + 1 // Flags BitMask for shapeshift spells
    {"uint32" ,"ShapeshiftExclude","0"},               //14 + 1-> this is wrong // Flags BitMask for which shapeshift forms this spell can NOT be used in.
    {"uint32" ,"Targets","0"},                         //15 + 1 - N / M
    {"uint32" ,"TargetCreatureType","0"},              //16 + 1
    {"uint32" ,"RequiresSpellFocus","0"},              //17 + 1
    {"uint32" ,"FacingCasterFlags","0"},               //18 + 1
    {"uint32" ,"CasterAuraState","0"},                 //19 + 1
    {"uint32" ,"TargetAuraState","0"},                 //20 + 1
    {"uint32" ,"ExcludeCasterAuraState","0"},          //21 + 1 2.0.1 unknown two
    {"uint32" ,"ExcludeTargetAuraState","0"},          //22 + 1 2.0.1 unknown three
    {"uint32" ,"CastingTimeIndex","0"},                //22 + 2
    {"uint32" ,"RecoveryTime","0"},                    //23 + 2
    {"uint32" ,"CategoryRecoveryTime","0"},            //24 + 2 recoverytime
    {"uint32" ,"InterruptFlags","0"},                  //25 + 2
    {"uint32" ,"AuraInterruptFlags","0"},              //26 + 2
    {"uint32" ,"ChannelInterruptFlags","0"},           //27 + 2
    {"uint32" ,"procFlags","0"},                       //28 + 2
    {"uint32" ,"procChance","0"},                      //29 + 2
    {"int32"  ,"procCharges","0"},                     //30 + 2
    {"uint32" ,"maxLevel","0"},                        //31 + 2
    {"uint32" ,"baseLevel","0"},                       //32 + 2
    {"uint32" ,"spellLevel","0"},                      //33 + 2
    {"uint32" ,"DurationIndex","0"},                   //34 + 2
    {"uint32" ,"powerType","0"},                       //35 + 2
    {"uint32" ,"manaCost","0"},                        //36 + 2
    {"uint32" ,"manaCostPerlevel","0"},                //37 + 2
    {"uint32" ,"manaPerSecond","0"},                   //38 + 2
    {"uint32" ,"manaPerSecondPerLevel","0"},           //39 + 2
    {"uint32" ,"rangeIndex","0"},                      //40 + 2
    {"float"  ,"speed","0"},                           //41 + 2
    {"uint32" ,"modalNextSpell","0"},                  //42 + 2
    {"uint32" ,"maxstack","0"},                        //43 + 2
    {"uint32" ,"Totem_1","0"},                        //44 - 45 + 2
    {"uint32" ,"Totem_2","0"},                        //44 - 45 + 2
    {"uint32" ,"Reagent_1","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_2","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_3","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_4","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_5","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_6","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_7","0"},                      //46 - 53 + 2
    {"uint32" ,"Reagent_8","0"},                      //46 - 53 + 2
    {"uint32" ,"ReagentCount_1","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_2","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_3","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_4","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_5","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_6","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_7","0"},                 //54 - 61 + 2
    {"uint32" ,"ReagentCount_8","0"},                 //54 - 61 + 2
    {"uint32" ,"EquippedItemClass","0"},               //62 + 2
    {"uint32" ,"EquippedItemSubClass","0"},            //63 + 2
    {"uint32" ,"RequiredItemFlags","0"},               //64 + 2
    {"uint32" ,"Effect_1","0"},                       //65 - 67 + 2
    {"uint32" ,"Effect_2","0"},                       //65 - 67 + 2
    {"uint32" ,"Effect_3","0"},                       //65 - 67 + 2
    {"uint32" ,"EffectDieSides_1","0"},               //68 - 70 + 2
    {"uint32" ,"EffectDieSides_2","0"},               //68 - 70 + 2
    {"uint32" ,"EffectDieSides_3","0"},               //68 - 70 + 2
    {"uint32" ,"EffectBaseDice_1","0"},               //71 - 73 + 2
    {"uint32" ,"EffectBaseDice_2","0"},               //71 - 73 + 2
    {"uint32" ,"EffectBaseDice_3","0"},               //71 - 73 + 2
    {"float"  ,"EffectDicePerLevel_1","0"},           //74 - 76 + 2
    {"float"  ,"EffectDicePerLevel_2","0"},           //74 - 76 + 2
    {"float"  ,"EffectDicePerLevel_3","0"},           //74 - 76 + 2
    {"float"  ,"EffectRealPointsPerLevel_1","0"},     //77 - 79 + 2
    {"float"  ,"EffectRealPointsPerLevel_2","0"},     //77 - 79 + 2
    {"float"  ,"EffectRealPointsPerLevel_3","0"},     //77 - 79 + 2
    {"int32"  ,"EffectBasePoints_1","0"},             //80 - 82 + 2
    {"int32"  ,"EffectBasePoints_2","0"},             //80 - 82 + 2
    {"int32"  ,"EffectBasePoints_3","0"},             //80 - 82 + 2
    {"int32"  ,"EffectMechanic_1","0"},               //83 - 85 + 2
    {"int32"  ,"EffectMechanic_2","0"},               //83 - 85 + 2
    {"int32"  ,"EffectMechanic_3","0"},               //83 - 85 + 2
    {"uint32" ,"EffectImplicitTargetA_1","0"},        //86 - 88 + 2
    {"uint32" ,"EffectImplicitTargetA_2","0"},        //86 - 88 + 2
    {"uint32" ,"EffectImplicitTargetA_3","0"},        //86 - 88 + 2
    {"uint32" ,"EffectImplicitTargetB_1","0"},        //89 - 91 + 2
    {"uint32" ,"EffectImplicitTargetB_2","0"},        //89 - 91 + 2
    {"uint32" ,"EffectImplicitTargetB_3","0"},        //89 - 91 + 2
    {"uint32" ,"EffectRadiusIndex_1","0"},            //92 - 94 + 2
    {"uint32" ,"EffectRadiusIndex_2","0"},            //92 - 94 + 2
    {"uint32" ,"EffectRadiusIndex_3","0"},            //92 - 94 + 2
    {"uint32" ,"EffectApplyAuraName_1","0"},          //95 - 97 + 2
    {"uint32" ,"EffectApplyAuraName_2","0"},          //95 - 97 + 2
    {"uint32" ,"EffectApplyAuraName_3","0"},          //95 - 97 + 2
    {"uint32" ,"EffectAmplitude_1","0"},              //98 - 100 + 2
    {"uint32" ,"EffectAmplitude_2","0"},              //98 - 100 + 2
    {"uint32" ,"EffectAmplitude_3","0"},              //98 - 100 + 2
    {"float"  ,"EffectAmplitude_1_blizz","0"},        //101 - 103 + 2
    {"float"  ,"EffectAmplitude_2_blizz","0"},        //101 - 103 + 2
    {"float"  ,"EffectAmplitude_3_blizz","0"},        //101 - 103 + 2     This value is the $ value from description
    {"uint32" ,"EffectChainTarget_1","0"},            //104 - 106 + 2
    {"uint32" ,"EffectChainTarget_2","0"},            //104 - 106 + 2
    {"uint32" ,"EffectChainTarget_3","0"},            //104 - 106 + 2
	{"uint32" ,"EffectSpellGroupRelation_1","0"},     //107 - 109 + 2
    {"uint32" ,"EffectSpellGroupRelation_2","0"},     //107 - 109 + 2
    {"uint32" ,"EffectSpellGroupRelation_3","0"},     //107 - 109 + 2
	{"uint32" ,"EffectMiscValue_1","0"},              //110 - 112 + 2
    {"uint32" ,"EffectMiscValue_2","0"},              //110 - 112 + 2
    {"uint32" ,"EffectMiscValue_3","0"},              //110 - 112 + 2
	{"uint32" ,"EffectMiscValueb_1","0"},              //110 - 112 + 2
    {"uint32" ,"EffectMiscValueb_2","0"},              //110 - 112 + 2
    {"uint32" ,"EffectMiscValueb_3","0"},              //110 - 112 + 2
	{"uint32" ,"EffectTriggerSpell_1","0"},           //113 - 115 + 2
    {"uint32" ,"EffectTriggerSpell_2","0"},           //113 - 115 + 2
    {"uint32" ,"EffectTriggerSpell_3","0"},           //113 - 115 + 2
    {"float"  ,"EffectPointsPerComboPoint_1","0"},    //116 - 118 + 2
    {"float"  ,"EffectPointsPerComboPoint_2","0"},    //116 - 118 + 2
    {"float"  ,"EffectPointsPerComboPoint_3","0"},    //116 - 118 + 2
    {"uint32" ,"SpellVisual","0"},                    //119 + 2
    {"uint32" ,"spellIconID","0"},                    //120 + 2
    {"uint32" ,"activeIconID","0"},                   //121 + 2
    {"uint32" ,"spellPriority","0"},                   //123 + 2
    {"uint32" ,"unknow","0"},							    //123 + 2
    {"str"	  ,"Name","1"},                            //124 + 2
    {"uint32" ,"NameAlt1","1"},                        //125 + 2
    {"uint32" ,"NameAlt2","1"},                        //126 + 2
    {"uint32" ,"NameAlt3","1"},                        //127 + 2
    {"uint32" ,"NameAlt4","1"},                        //128 + 2
    {"uint32" ,"NameAlt5","1"},                        //129 + 2
    {"uint32" ,"NameAlt6","1"},                        //130 + 2
    {"uint32" ,"NameAlt7","1"},                        //131 + 2
    {"uint32" ,"NameAlt8","1"},                        //132 + 2
    {"uint32" ,"NameAlt9","1"},                        //133 + 2
    {"uint32" ,"NameAlt10","1"},                       //134 + 2
    {"uint32" ,"NameAlt11","1"},                       //135 + 2
    {"uint32" ,"NameAlt12","1"},                       //136 + 2
    {"uint32" ,"NameAlt13","1"},                       //137 + 2
    {"uint32" ,"NameAlt14","1"},                       //138 + 2
    {"uint32" ,"NameAlt15","1"},                       //139 + 2
    {"uint32" ,"NameFlags","0"},                       //140 + 2
    {"str"	  ,"Rank","1"},                            //141 + 2
    {"uint32" ,"RankAlt1","1"},                        //142 + 2
    {"uint32" ,"RankAlt2","1"},                        //143 + 2
    {"uint32" ,"RankAlt3","1"},                        //144 + 2
    {"uint32" ,"RankAlt4","1"},                        //145 + 2
    {"uint32" ,"RankAlt5","1"},                        //146 + 2
    {"uint32" ,"RankAlt6","1"},                        //147 + 2
    {"uint32" ,"RankAlt7","1"},                        //148 + 2
    {"uint32" ,"RankAlt8","1"},                        //149 + 2
    {"uint32" ,"RankAlt9","1"},                        //150 + 2
    {"uint32" ,"RankAlt10","1"},                       //151 + 2
    {"uint32" ,"RankAlt11","1"},                       //152 + 2
    {"uint32" ,"RankAlt12","1"},                       //153 + 2
    {"uint32" ,"RankAlt13","1"},                       //154 + 2
    {"uint32" ,"RankAlt14","1"},                       //155 + 2
    {"uint32" ,"RankAlt15","1"},                       //156 + 2
    {"uint32" ,"RankFlags","0"},                       //157 + 2
    {"str"	  ,"Description","1"},                     //158 + 2
    {"uint32" ,"DescriptionAlt1","1"},                 //159 + 2
    {"uint32" ,"DescriptionAlt2","1"},                 //160 + 2
    {"uint32" ,"DescriptionAlt3","1"},                 //161 + 2
    {"uint32" ,"DescriptionAlt4","1"},                 //162 + 2
    {"uint32" ,"DescriptionAlt5","1"},                 //163 + 2
    {"uint32" ,"DescriptionAlt6","1"},                 //164 + 2
    {"uint32" ,"DescriptionAlt7","1"},                 //165 + 2
    {"uint32" ,"DescriptionAlt8","1"},                 //166 + 2
    {"uint32" ,"DescriptionAlt9","1"},                 //167 + 2
    {"uint32" ,"DescriptionAlt10","1"},                //168 + 2
    {"uint32" ,"DescriptionAlt11","1"},                //169 + 2
    {"uint32" ,"DescriptionAlt12","1"},                //170 + 2
    {"uint32"  ,"DescriptionAlt13","1"},                //171 + 2
    {"uint32"  ,"DescriptionAlt14","1"},                //172 + 2
    {"uint32"  ,"DescriptionAlt15","1"},                //173 + 2
    {"uint32"  ,"DescriptionFlags","0"},                //174 + 2
    {"str"	   ,"BuffDescription","1"},                 //175 + 2
    {"uint32"  ,"BuffDescriptionAlt1","1"},             //176 + 2
    {"uint32"  ,"BuffDescriptionAlt2","1"},             //177 + 2
    {"uint32"  ,"BuffDescriptionAlt3","1"},             //178 + 2
    {"uint32"  ,"BuffDescriptionAlt4","1"},             //179 + 2
    {"uint32"  ,"BuffDescriptionAlt5","1"},             //180 + 2
    {"uint32"  ,"BuffDescriptionAlt6","1"},             //181 + 2
    {"uint32"  ,"BuffDescriptionAlt7","1"},             //182 + 2
    {"uint32"  ,"BuffDescriptionAlt8","1"},             //183 + 2
    {"uint32"  ,"BuffDescriptionAlt9","1"},             //184 + 2
    {"uint32"  ,"BuffDescriptionAlt10","1"},            //185 + 2
    {"uint32"  ,"BuffDescriptionAlt11","1"},            //186 + 2
    {"uint32"  ,"BuffDescriptionAlt12","1"},            //187 + 2
    {"uint32"  ,"BuffDescriptionAlt13","1"},            //188 + 2
    {"uint32"  ,"BuffDescriptionAlt14","1"},            //189 + 2
	{"uint32"  ,"BuffDescriptionAlt15","1"},            //190 + 2
    {"uint32"  ,"buffdescflags","0"},                   //191 + 2
    {"uint32"  ,"ManaCostPercentage","0"},              //192 + 2
    {"uint32"  ,"StartRecoveryCategory","0"},           //195 + 2
    {"uint32"  ,"StartRecoveryTime","0"},               //194 + 2
    {"uint32"  ,"MaxTargetLvl","0"},                      //199 + 2 
    {"uint32"  ,"SpellFamilyName","0"},                 //196 + 2
    {"uint32"  ,"SpellGroupType","0"},                  //197 + 2   flags 
    {"uint32"  ,"SpellGroupType_high","0"},             //198  + 2  flags
    {"uint32"  ,"MaxTargets","0"},                      //199 + 2 
    {"uint32"  ,"Spell_Dmg_Type","0"},                  //200 + 2   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
    {"uint32"  ,"PreventionType","0"},                  //201 + 2   0,1,2 related to Spell_Dmg_Type I think
    {"int32"   ,"StanceBarOrder","0"},                  //202 + 2   related to paladin aura's 
    {"float"   ,"dmg_multiplier_1","0"},                //203 - 205 + 2   if the name is correct I dono
    {"float"   ,"dmg_multiplier_2","0"},                //203 - 205 + 2   if the name is correct I dono
    {"float"   ,"dmg_multiplier_3","0"},                //203 - 205  + 2  if the name is correct I dono
    {"uint32"  ,"MinFactionID","0"},                    //206 + 2   only one spellid:6994 has this value = 369
    {"uint32"  ,"MinReputation","0"},                   //207 + 2   only one spellid:6994 has this value = 4
    {"uint32"  ,"RequiredAuraVision","0"},              //208 + 2   only one spellid:26869  has this flag = 1   
    {"uint32"  ,"TotemCategory_1","0"},					//209 + 2
    {"uint32"  ,"TotemCategory_2","0"},                 //210 + 2
    {"uint32"  ,"RequiresAreaId","0"},					//211 + 2
    {"uint32"  ,"schoolMask","0"},						//212 + 2
    {"uint32"  ,"m_runeCostID","0"},						//212 + 2
    {"uint32"  ,"m_spellMissileID","0"},						//212 + 2
    {"uint32"  ,"t1","0"},						//212 + 2
    {"uint32"  ,"t2","0"},						//212 + 2
};
/**/

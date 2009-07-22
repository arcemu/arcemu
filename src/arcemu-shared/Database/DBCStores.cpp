/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

#include "DBCStores.h"
#include "DataStore.h"
#include "NGLog.h"
#ifdef ENABLE_ACHIEVEMENTS
SERVER_DECL DBCStorage<AchievementEntry> dbcAchievementStore;
SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchievementCriteriaStore;
SERVER_DECL DBCStorage<AchievementCategoryEntry> dbcAchievementCategoryStore;
#endif
SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
SERVER_DECL DBCStorage<AreaTable> dbcArea;
SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices;
SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyleStore;
SERVER_DECL DBCStorage<BattlemasterListEntry> dbcBattlemasterListStore;
SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
SERVER_DECL DBCStorage<CharTitlesEntry> dbcCharTitlesEntry;
SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
SERVER_DECL DBCStorage<GlyphSlotEntry> dbcGlyphSlot;
SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
SERVER_DECL DBCStorage<ItemLimitCategoryEntry> dbcItemLimitCategory;
SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
SERVER_DECL DBCStorage<Lock> dbcLock;
SERVER_DECL DBCStorage<MapEntry> dbcMap;
SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
SERVER_DECL DBCStorage<ScalingStatDistributionEntry> dbcScalingStatDistribution;
SERVER_DECL DBCStorage<ScalingStatValuesEntry> dbcScalingStatValues;
SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
SERVER_DECL DBCStorage<SpellRuneCostEntry> dbcSpellRuneCost;
SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
SERVER_DECL DBCStorage<WorldMapOverlay> dbcWorldMapOverlayStore;

SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;

const char* WorldMapOverlayStoreFormat="nxiiiixxxxxxxxxxx";
const char* BarberShopStyleEntryFormat="nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxi";
const char* ItemSetFormat = "ulxxxxxxxxxxxxxxxxuuuuuuuuxxxxxxxxxuuuuuuuuuuuuuuuuuu";
const char* LockFormat = "uuuuuuxxxuuuuuxxxuuuuuxxxxxxxxxxx";
const char* EmoteEntryFormat = "uxuuuuxuxuxxxxxxxxx";
const char* skilllinespellFormat = "uuuxxxxuuuuuxx";
const char* EnchantEntrYFormat = "uxuuuuuuuuuuuusxxxxxxxxxxxxxxxxuuuuxxx";
const char* GemPropertyEntryFormat = "uuuuu";
const char* GlyphPropertyEntryFormat = "uuuu";
const char* GlyphSlotEntryFormat = "uuu";
const char* skilllineentrYFormat = "uuulxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

const char* BattlemasterListEntryFormat = "uiiiiiiiiuuuuuiiiiiiiiiiiiiiiiiiuux";

const char* CharTitlesEntryfmt =
	"u" // ID
	"u" // unk1
	"lxxxxxxxxxxxxxxx" // name
	"u" // name_flag
	"lxxxxxxxxxxxxxxx" // name2
	"u" // name2_flag
	"u" // bit_index
;
#ifdef ENABLE_ACHIEVEMENTS
const char* AchievementStoreFormat=
	"n" // ID
	"i" // factionFlag
	"i" // mapID
	"u" // unknown1
	"lxxxxxxxxxxxxxxx" // name
	"u" // name_flags
	"lxxxxxxxxxxxxxxx" // description
	"u" // desc_flags
	"i" // categoryId
	"i" // points
	"u" // orderInCategory
	"i" // flags
	"u" // flags2
	"lxxxxxxxxxxxxxxx" // rewardName
	"u" // rewardName_flags
	"u" // count
	"u" // refAchievement
;

const char* AchievementCategoryStoreFormat=
	"n" // ID
	"u" // parentCategory
	"lxxxxxxxxxxxxxxx" // name
	"u" // name_flags
	"u" // sortOrder
;

const char* AchievementCriteriaStoreFormat=
	"n" // ID
	"i" // referredAchievement
	"i" // requiredType
	"i" // raw.field3
	"i" // raw.field4
	"i" // raw.additionalRequirement1_type
	"i" // raw.additionalRequirement1_value
	"i" // raw.additionalRequirement2_type
	"i" // raw.additionalRequirement2_value
	"lxxxxxxxxxxxxxxx" // name
	"u" // name_flags
	"i" // completionFlag
	"i" // groupFlag
	"u" // unk1
	"i" // timeLimit
	"u" // index
;
#endif

const char* spellentryFormat =
	"u" // Id
	"u" // Category
	"u" // DispelType
	"u" // MechanicsType
	"u" // Attributes
	"u" // AttributesEx
	"u" // AttributesExB
	"u" // AttributesExC
	"u" // AttributesExD
	"u" // AttributesExE
	"u" // AttributesExF
	"u" // RequiredShapeShift
	"u" // ShapeshiftExclude
	"u" // Targets
	"u" // TargetCreatureType
	"u" // RequiresSpellFocus
	"u" // FacingCasterFlags
	"u" // CasterAuraState
	"u" // TargetAuraState
	"u" // ExcludeCasterAuraState
	"u" // ExcludeTargetAuraState
	"u" // casterAuraSpell
	"u" // targetAuraSpell
	"u" // ExcludeCasterAuraState
	"u" // ExcludeTargetAuraState
	"u" // CastingTimeIndex
	"u" // RecoveryTime
	"u" // CategoryRecoveryTime
	"u" // InterruptFlags
	"u" // AuraInterruptFlags
	"u" // ChannelInterruptFlags
	"u" // procFlags
	"u" // procChance
	"u" // procCharges
	"u" // maxLevel
	"u" // baseLevel
	"u" // spellLevel
	"u" // DurationIndex
	"u" // powerType
	"u" // manaCost
	"u" // manaCostPerlevel
	"u" // manaPerSecond
	"u" // manaPerSecondPerLevel
	"u" // rangeIndex
	"f" // speed
	"u" // modalNextSpell
	"u" // maxstack
	"uu" // Totem[2]
	"uuuuuuuu" // Reagent[8]
	"uuuuuuuu" // ReagentCount[8]
	"u" // EquippedItemClass
	"u" // EquippedItemSubClass
	"u" // RequiredItemFlags
	"uuu" // Effect[3]
	"uuu" // EffectDieSides[3]
	"uuu" // EffectBaseDice[3]
	"uuu" // EffectDicePerLevel[3]
	"uuu" // EffectRealPointsPerLevel[3]
	"uuu" // EffectBasePoints[3]
	"uuu" // EffectMechanic[3]
	"uuu" // EffectImplicitTargetA[3]
	"uuu" // EffectImplicitTargetB[3]
	"uuu" // EffectRadiusIndex[3]
	"uuu" // EffectApplyAuraName[3]
	"uuu" // EffectAmplitude[3]
	"uuu" // Effectunknown[3]
	"uuu" // EffectChainTarget[3]
	"uuu" // EffectSpellGroupRelation[3]
	"uuu" // EffectMiscValue[3]
	"uuu" // EffectMiscValueB[3]
	"uuu" // EffectTriggerSpell[3]
	"uuu" // EffectPointsPerComboPoint[3]
	"uuu" // EffectUnk0[3]
	"uuu" // EffectUnk1[3]
	"uuu" // EffectUnk2[3]
	"u" // SpellVisual
	"u" // field114
	"u" // spellIconID
	"u" // activeIconID
	"u" // spellPriority
	"lxxxxxxxxxxxxxxxx" // Name
	"lxxxxxxxxxxxxxxxx" // Rank
	"lxxxxxxxxxxxxxxxx" // Description
	"lxxxxxxxxxxxxxxxx" // BuffDescription
	"u" // ManaCostPercentage
	"u" // unkflags
	"u" // StartRecoveryTime
	"u" // StartRecoveryCategory
	"u" // MaxTargetLevel
	"u" // SpellFamilyName
	"uu" // SpellGroupType
	"u" // MaxTargets
	"u" // Spell_Dmg_Type
	"u" // PreventionType
	"u" // StanceBarOrder
	"fff" // dmg_multiplier[3]
	"u" // MinFactionID
	"u" // MinReputation
	"u" // RequiredAuraVision
	"uu" // TotemCategory[2]
	"i" // RequiresAreaId
	"u" // School
	"ux"
	"x" //Added in 3.1
;


const char* itemextendedcostFormat = "uuuuuuuuuuuuuux";
const char* talententryFormat = "uuuuuuuuuxxxxuxxuxxxxxx";
const char* talenttabentryFormat = "uxxxxxxxxxxxxxxxxxxxuuux";
const char* spellcasttimeFormat = "uuxx";
const char* spellradiusFormat = "ufxf";
const char* spellrangeFormat = "uffffxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* SpellRuneCostFormat="uuuuu";
const char* spelldurationFormat = "uuuu";
const char* randompropsFormat = "uxuuuxxxxxxxxxxxxxxxxxxx";
const char* areagroupFormat = "uuuuuuuu";
const char* areatableFormat = "uuuuuxxxuxulxxxxxxxxxxxxxxxxuxxxxxxx";
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";
const char* auctionhousedbcFormat = "uuuuxxxxxxxxxxxxxxxxx";
const char* factiondbcFormat = "uiuuuuuuuuiiiiuuuuulxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* dbctaxinodeFormat = "uufffxxxxxxxxxxxxxxxxxuu";
const char* dbctaxipathFormat = "uuuu";
const char* dbctaxipathnodeFormat = "uuuufffuuxx";
const char* creaturespelldataFormat = "uuuuuuuuu";
const char* charraceFormat = "uxxxxxxxuxxxxulxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* charclassFormat = "uxuxlxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* creaturefamilyFormat = "ufufuuuuuxlxxxxxxxxxxxxxxxxx";
const char* mapentryFormat =
	"u"					// 0 id
	"s"					// 1 name_internal
	"u"					// 2 map_type
	"u"					// 3 is_pvp_zone
	"lxxxxxxxxxxxxxxxx"	// 4-20 real_name
	"u"					// 21 linked_zone
	"xxxxxxxxxxxxxxxxx" // 22-38 hordeIntro
	"xxxxxxxxxxxxxxxxx" // 39-55 allianceIntro
	"u"					// 56 multimap_id
	"x"					// 57 unk_float (all 1 but arathi 1.25)
	"xxxxxxxxxxxxxxxxx"	// 58-74 normalReqText
	"xxxxxxxxxxxxxxxxx"	// 75-91 heroicReqText
	"xxxxxxxxxxxxxxxxx"	// 92-108 emptyText
	"u"					// 109 parent_map
	"u"					// 110 start_x
	"u"					// 111 start_y
	"u"					// 112 resetTimeRaid
	"u"					// 113 resetTimeHeroic
	"x"					// 114 unk
	"x"					// 115 unk
	"u"					// 116 addon
	"x";				// 117 unk

const char* itemrandomsuffixformat = "uxxxxxxxxxxxxxxxxxxuuuxxuuuxx";//19, 20, 21, 24, 25, 26
const char* chatchannelformat = "iixssssssssssssssslxxxxxxxxxxxxxxxxxx";
const char* durabilityqualityFormat = "uf";
const char* durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char* bankslotpriceformat = "uu";
const char* barbershopstyleFormat="nulxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuu";
const char* gtfloatformat = "f";
const char* areatriggerformat = "uuffffffff";
const char* scalingstatdistributionformat = "uiiiiiiiiiiuuuuuuuuuuu";
const char* scalingstatvaluesformat = "uuuuuuuuuuuuuuuuuuu";
const char* itemlimitcategoryformat = "usxxxxxxxxxxxxxxxxuu";

template<class T>
bool loader_stub(const char * filename, const char * format, bool ind, T& l, bool loadstrs)
{
	Log.Notice("DBC", "Loading %s.", filename);
	return l.Load(filename, format, ind, loadstrs);
}

#define LOAD_DBC(filename, format, ind, stor, strings) if(!loader_stub(filename, format, ind, stor, strings)) { return false; }

bool LoadDBCs()
{
	LOAD_DBC("DBC/WorldMapOverlay.dbc", WorldMapOverlayStoreFormat, true, dbcWorldMapOverlayStore, true);
#ifdef ENABLE_ACHIEVEMENTS
	LOAD_DBC("DBC/Achievement_Category.dbc", AchievementCategoryStoreFormat, true, dbcAchievementCategoryStore, true);
	LOAD_DBC("DBC/Achievement_Criteria.dbc", AchievementCriteriaStoreFormat, true, dbcAchievementCriteriaStore, true);
	LOAD_DBC("DBC/Achievement.dbc", AchievementStoreFormat, true, dbcAchievementStore, true);
#endif
	LOAD_DBC("DBC/BattlemasterList.dbc", BattlemasterListEntryFormat, true, dbcBattlemasterListStore, true);
	LOAD_DBC("DBC/CharTitles.dbc", CharTitlesEntryfmt, true, dbcCharTitlesEntry, true);
	LOAD_DBC("DBC/BarberShopStyle.dbc", BarberShopStyleEntryFormat, true, dbcBarberShopStyleStore, true);
	LOAD_DBC("DBC/ItemSet.dbc", ItemSetFormat, true, dbcItemSet, true);
	LOAD_DBC("DBC/Lock.dbc", LockFormat, true, dbcLock, false);
	LOAD_DBC("DBC/EmotesText.dbc", EmoteEntryFormat, true, dbcEmoteEntry, false);
	LOAD_DBC("DBC/SkillLineAbility.dbc", skilllinespellFormat, false, dbcSkillLineSpell, false);
	LOAD_DBC("DBC/SpellItemEnchantment.dbc", EnchantEntrYFormat, true, dbcEnchant, true);
	LOAD_DBC("DBC/GemProperties.dbc", GemPropertyEntryFormat, true, dbcGemProperty, false);
	LOAD_DBC("DBC/GlyphProperties.dbc", GlyphPropertyEntryFormat, true, dbcGlyphProperty, false);
	LOAD_DBC("DBC/GlyphSlot.dbc", GlyphSlotEntryFormat, true, dbcGlyphSlot, false);
	LOAD_DBC("DBC/SkillLine.dbc", skilllineentrYFormat, true, dbcSkillLine, true);
	LOAD_DBC("DBC/Spell.dbc", spellentryFormat, true, dbcSpell, true);
	LOAD_DBC("DBC/ItemExtendedCost.dbc", itemextendedcostFormat, true, dbcItemExtendedCost, false);
	LOAD_DBC("DBC/Talent.dbc", talententryFormat, true, dbcTalent, false);
	LOAD_DBC("DBC/TalentTab.dbc", talenttabentryFormat, true, dbcTalentTab, false);
	LOAD_DBC("DBC/SpellCastTimes.dbc", spellcasttimeFormat, true, dbcSpellCastTime, false);
	LOAD_DBC("DBC/SpellRadius.dbc", spellradiusFormat, true, dbcSpellRadius, false);
	LOAD_DBC("DBC/SpellRange.dbc", spellrangeFormat, true, dbcSpellRange, false);
	LOAD_DBC("DBC/SpellRuneCost.dbc", SpellRuneCostFormat, true, dbcSpellRuneCost, false);
	LOAD_DBC("DBC/SpellDuration.dbc", spelldurationFormat, true, dbcSpellDuration, false);
	LOAD_DBC("DBC/ItemRandomProperties.dbc", randompropsFormat, true, dbcRandomProps, false);
	LOAD_DBC("DBC/AreaGroup.dbc", areagroupFormat, true, dbcAreaGroup, true);
	LOAD_DBC("DBC/AreaTable.dbc", areatableFormat, true, dbcArea, true);
	LOAD_DBC("DBC/FactionTemplate.dbc", factiontemplatedbcFormat, true, dbcFactionTemplate, false);
	LOAD_DBC("DBC/Faction.dbc", factiondbcFormat, true, dbcFaction, true);
	LOAD_DBC("DBC/TaxiNodes.dbc", dbctaxinodeFormat, false, dbcTaxiNode, false);
	LOAD_DBC("DBC/TaxiPath.dbc", dbctaxipathFormat, false, dbcTaxiPath, false);
	LOAD_DBC("DBC/TaxiPathNode.dbc", dbctaxipathnodeFormat, false, dbcTaxiPathNode, false);
	LOAD_DBC("DBC/CreatureSpellData.dbc", creaturespelldataFormat, true, dbcCreatureSpellData, false);
	LOAD_DBC("DBC/CreatureFamily.dbc", creaturefamilyFormat, true, dbcCreatureFamily, true);
	LOAD_DBC("DBC/ChrRaces.dbc", charraceFormat, true, dbcCharRace, true);
	LOAD_DBC("DBC/ChrClasses.dbc", charclassFormat, true, dbcCharClass, true);
	LOAD_DBC("DBC/Map.dbc", mapentryFormat, true, dbcMap, true);
	LOAD_DBC("DBC/AuctionHouse.dbc", auctionhousedbcFormat, true, dbcAuctionHouse, false);
	LOAD_DBC("DBC/ItemRandomSuffix.dbc", itemrandomsuffixformat, true, dbcItemRandomSuffix, false);
	LOAD_DBC("DBC/gtCombatRatings.dbc", gtfloatformat, false, dbcCombatRating, false);
	LOAD_DBC("DBC/ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, true);
	LOAD_DBC("DBC/DurabilityQuality.dbc", durabilityqualityFormat, true, dbcDurabilityQuality, false);
	LOAD_DBC("DBC/DurabilityCosts.dbc", durabilitycostsFormat, true, dbcDurabilityCosts, false);
	LOAD_DBC("DBC/BankBagSlotPrices.dbc", bankslotpriceformat, true, dbcBankSlotPrices, false);
	LOAD_DBC("DBC/StableSlotPrices.dbc", bankslotpriceformat, true, dbcStableSlotPrices, false);
	LOAD_DBC("DBC/gtBarberShopCostBase.dbc", gtfloatformat, false, dbcBarberShopPrices, false);
	LOAD_DBC("DBC/gtChanceToMeleeCrit.dbc", gtfloatformat, false, dbcMeleeCrit, false);
	LOAD_DBC("DBC/gtChanceToMeleeCritBase.dbc", gtfloatformat, false, dbcMeleeCritBase, false);
	LOAD_DBC("DBC/gtChanceToSpellCrit.dbc", gtfloatformat, false, dbcSpellCrit, false);
	LOAD_DBC("DBC/gtChanceToSpellCritBase.dbc", gtfloatformat, false, dbcSpellCritBase, false);
	LOAD_DBC("DBC/gtRegenMPPerSpt.dbc", gtfloatformat, false, dbcManaRegenBase, false); //it's not a mistake.
	LOAD_DBC("DBC/gtOCTRegenMP.dbc", gtfloatformat, false, dbcManaRegen, false); //it's not a mistake.
	LOAD_DBC("DBC/gtRegenHPPerSpt.dbc", gtfloatformat, false, dbcHPRegenBase, false); //it's not a mistake.
	LOAD_DBC("DBC/gtOCTRegenHP.dbc", gtfloatformat, false, dbcHPRegen, false); //it's not a mistake.
	LOAD_DBC("DBC/AreaTrigger.dbc", areatriggerformat, true, dbcAreaTrigger, true);
	LOAD_DBC("DBC/ScalingStatDistribution.dbc",scalingstatdistributionformat,true,dbcScalingStatDistribution, false);
	LOAD_DBC("DBC/ScalingStatValues.dbc",scalingstatvaluesformat,true,dbcScalingStatValues, false);
	LOAD_DBC("DBC/ItemLimitCategory.dbc", itemlimitcategoryformat, true, dbcItemLimitCategory, true);
	return true;
}
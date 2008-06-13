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

struct RPlayerInfo
{
	uint32 Guid;
	uint32 AccountId;
	string Name;
	uint32 Level;
	uint32 GuildId;
	float PositionX;
	float PositionY;
	uint32 ZoneId;
	uint8 Race;
	uint8 Class;
	uint8 Gender;
	uint32 Latency;
	string GMPermissions;
	uint32 Account_Flags;
	uint32 InstanceId;
	uint32 MapId;

	void Pack(ByteBuffer& buf)
	{
		buf << Guid << AccountId << Name << PositionX << PositionY << ZoneId << Race << Class << Gender << Latency << GMPermissions
			<< Account_Flags << InstanceId << Level << GuildId << MapId;
	}

	void Unpack(ByteBuffer & buf)
	{
		buf >> Guid >> AccountId >> Name >> PositionX >> PositionY >> ZoneId >> Race >> Class >> Gender >> Latency >> GMPermissions
			>> Account_Flags >> InstanceId << Level << GuildId << MapId;
	}

#ifndef _GAME
	/* This stuff is used only by the realm server */
	uint32 RecoveryMapId;
	LocationVector RecoveryPosition;
#endif
};

#ifndef _GAME
/* Copied structures from game */
struct SocketInfo {
	uint32 SocketColor;
	uint32 Unk;
};

struct ItemSpell
{
	uint32 Id;
	uint32 Trigger;
	int32 Charges;
	int32 Cooldown;
	uint32 Category;
	int32 CategoryCooldown;
};

struct ItemDamage
{
	float Min;
	float Max;
	uint32 Type;
};

struct ItemStat
{
	uint32 Type;
	int32 Value;
};

struct ItemPrototype
{
	uint32 ItemId;
	uint32 Class;
	uint32 SubClass;
	uint32 unknown_bc;
	char * Name1;
	char * Name2;
	char * Name3;
	char * Name4;
	uint32 DisplayInfoID;
	uint32 Quality;
	uint32 Flags;
	uint32 BuyPrice;
	uint32 SellPrice;
	uint32 InventoryType;
	uint32 AllowableClass;
	uint32 AllowableRace;
	uint32 ItemLevel;
	uint32 RequiredLevel;
	uint32 RequiredSkill;
	uint32 RequiredSkillRank;
	uint32 RequiredSkillSubRank;
	uint32 RequiredPlayerRank1;
	uint32 RequiredPlayerRank2;
	uint32 RequiredFaction;
	uint32 RequiredFactionStanding;
	uint32 Unique;
	uint32 MaxCount;
	uint32 ContainerSlots;
	ItemStat Stats[10];
	ItemDamage Damage[5];
	uint32 Armor;
	uint32 HolyRes;
	uint32 FireRes;
	uint32 NatureRes;
	uint32 FrostRes;
	uint32 ShadowRes;
	uint32 ArcaneRes;
	uint32 Delay;
	uint32 AmmoType;
	float  Range;
	ItemSpell Spells[5];
	uint32 Bonding;
	char * Description;
	uint32 PageId;
	uint32 PageLanguage;
	uint32 PageMaterial;
	uint32 QuestId;
	uint32 LockId;
	uint32 LockMaterial;
	uint32 Field108;
	uint32 RandomPropId;
	uint32 RandomSuffixId;
	uint32 Block;
	uint32 ItemSet;
	uint32 MaxDurability;
	uint32 ZoneNameID;
	uint32 MapID;
	uint32 BagFamily;
	uint32 ToolCategory;
	SocketInfo Sockets[3];
	uint32 SocketBonus;
	uint32 GemProperties;
	uint32 ItemExtendedCost;
	uint32 DisenchantReqSkill;
	uint32 ArmorDamageModifier;
};

struct CreatureInfo
{
	uint32 Id;
	char * Name;
	char * SubName;
	uint32 Flags1;
	uint32 Type;
	uint32 Family;
	uint32 Rank;
	uint32 Unknown1;
	uint32 SpellDataID;
	uint32 DisplayID;
	float unk2;
	float unk3;
	uint8  Civilian;
	uint8  Leader;
};


struct GameObjectInfo
{
	uint32 ID;
	uint32 Type;
	uint32 DisplayID;
	char * Name;
	uint32 SpellFocus;
	uint32 sound1;
	uint32 sound2;
	uint32 sound3;
	uint32 sound4;
	uint32 sound5;
	uint32 sound6;
	uint32 sound7;
	uint32 sound8;
	uint32 sound9;
	uint32 Unknown1;
	uint32 Unknown2;
	uint32 Unknown3;
	uint32 Unknown4;
	uint32 Unknown5;
	uint32 Unknown6;
	uint32 Unknown7;
	uint32 Unknown8;
	uint32 Unknown9;
	uint32 Unknown10;
	uint32 Unknown11;
	uint32 Unknown12;
	uint32 Unknown13;
	uint32 Unknown14;
};

struct ItemPage
{
	uint32 id;
	char * text;
	uint32 next_page;
};


struct Quest
{
	uint32 id;
	uint32 quest_sort;
	uint32 zone_id;
	uint32 quest_flags;
	uint32 min_level;
	uint32 max_level;
	uint32 type;
	uint32 required_races;
	uint32 required_class;
	uint32 required_tradeskill;
	uint32 required_rep_faction;
	uint32 required_rep_value;

	uint32 time;
	uint32 special_flags;

	uint32 previous_quest_id;
	uint32 next_quest_id;

	uint32 srcitem;
	uint32 srcitemcount;

	char * title;
	char * details;
	char * objectives;
	char * completiontext;
	char * incompletetext;
	char * endtext;

	char * objectivetexts[4];

	uint32 required_item[4];
	uint32 required_itemcount[4];

	uint32 required_mob[4];
	uint32 required_mobcount[4];

	uint32 reward_choiceitem[6];
	uint32 reward_choiceitemcount[6];

	uint32 reward_item[4];
	uint32 reward_itemcount[4];

	uint32 reward_repfaction[2];
	int32 reward_repvalue[2];
	uint32 reward_replimit;

	uint32 reward_money;
	uint32 reward_xp;
	uint32 reward_spell;
	uint32 effect_on_player;

	uint32 point_mapid;
	uint32 point_x;
	uint32 point_y;
	uint32 point_opt;

	uint32 required_money;
	uint32 required_triggers[4];
	uint32 required_quests[4];
	uint32 receive_items[4];
	uint32 receive_itemcount[4];
	int is_repeatable;

	uint32 count_required_mob;
	uint32 count_requiredquests;
	uint32 count_requiredtriggers;
	uint32 count_receiveitems;
	uint32 count_reward_choiceitem;
	uint32 count_required_item;
	uint32 required_mobtype[4];
	uint32 count_reward_item;
	uint32 reward_xp_as_money;
};


struct GossipText_Text
{
	float Prob;
	char * Text[2];
	uint32 Lang;
	uint32 Emote[6];
};

struct GossipText
{
	uint32 ID;
	GossipText_Text Texts[8];
};

struct MapInfo
{
	uint32 mapid;
	uint32 screenid;
	uint32 type;
	uint32 playerlimit;
	uint32 minlevel;
	float repopx;
	float repopy;
	float repopz;
	uint32 repopmapid;
	char * name;
	uint32 flags;
	uint32 cooldown;
	uint32 lvl_mod_a;
	uint32 required_quest;
	uint32 required_item;

	bool HasFlag(uint32 flag)
	{
		return (flags & flag) != 0;
	}
};

#endif

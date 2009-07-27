/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef WOWSERVER_CREATURE_H
#define WOWSERVER_CREATURE_H

class CreatureTemplate;
class GossipScript;

#define MAX_CREATURE_INV_ITEMS 150
#define MAX_CREATURE_LOOT 8
#define MAX_PET_SPELL 4
#define VENDOR_ITEMS_UPDATE_TIME 3600000
#include "Map.h"

struct CreatureItem
{
	uint32 itemid;
	uint32 amount; //!!!!! stack amount.
	uint32 available_amount;
	uint32 max_amount;
	uint32 incrtime;
	ItemExtendedCostEntry * extended_cost;
};

enum CreatureAISpellFlags
{
	CREATURE_AI_FLAG_NONE				= 0x00,
	CREATURE_AI_FLAG_RANDOMCAST			= 0x01,
	CREATURE_AI_FLAG_CASTOUTOFCOMBAT	= 0x02,
	CREATURE_AI_FLAG_PLAYERGCD			= 0x04
};

SERVER_DECL bool Rand(float chance);
SERVER_DECL bool Rand(uint32 chance);
SERVER_DECL bool Rand(int32 chance);
#pragma pack(push,1)
struct CreatureInfo
{
	uint32 Id;
	char * Name;
	char * SubName;
	char * info_str;
	uint32 Flags1;
	uint32 Type;
	uint32 Family;
	uint32 Rank;
	uint32 Unknown1;
	uint32 SpellDataID;
	uint32 Male_DisplayID;
	uint32 Female_DisplayID;
	uint32 Male_DisplayID2;
	uint32 Female_DisplayID2;
	float unkfloat1;
	float unkfloat2;
	uint8  Civilian;
	uint8  Leader;

	std::string lowercase_name;
	GossipScript * gossip_script;
	uint8 GenerateModelId(uint32 * des)
	{
		uint32 models[] = { Male_DisplayID, Male_DisplayID2, Female_DisplayID, Female_DisplayID2 };
		if(!models[0] && !models[1] && !models[2] && !models[3])
		{
			// All models are invalid.
			Log.Notice("CreatureSpawn", "All model IDs are invalid for creature %u", Id);
			return 0;
		}

		while(true)
		{
			uint32 res = RandomUInt(3);
			if(models[res])
			{
				*des = models[res];
				return res < 2 ? 0 : 1;
			}
		}
	}
};

struct CreatureProto
{
	uint32 Id;
	uint32 MinLevel;
	uint32 MaxLevel;
	uint32 Faction;
	uint32 MinHealth;
	uint32 MaxHealth;
	uint32 Mana;
	float  Scale;
	uint32	NPCFLags;
	uint32 AttackTime;
	uint32 AttackType;
	float MinDamage;
	float MaxDamage;
	uint32 CanRanged;
	uint32 RangedAttackTime;
	float RangedMinDamage;
	float RangedMaxDamage;
	uint32 RespawnTime;
	uint32 Resistances[7];
	float CombatReach;
	float BoundingRadius;
	char * aura_string;
	uint32 boss;
	uint32 money;
	uint32 invisibility_type;
	uint32 death_state;
	float	walk_speed;//base movement
	float	run_speed;//most of the time mobs use this
	float fly_speed;
	uint32 extra_a9_flags;
	uint32 AISpells[4];
	uint32 AISpellsFlags;
	uint32 modImmunities;

	/* AI Stuff */
	bool m_canRangedAttack;
	bool m_canFlee;
	float m_fleeHealth;
	uint32 m_fleeDuration;
	bool m_canCallForHelp;
	float m_callForHelpHealth;

	set<uint32> start_auras;
	list<AI_Spell*> spells;
};

struct VendorRestrictionEntry
{
	uint32 entry;
	int32 racemask;
	uint32 reqrepfaction;
	uint32 reqrepvalue;
	uint32 canbuyattextid;
	uint32 cannotbuyattextid;
};

#pragma pack(pop)

struct Formation{
	uint32 fol;
	float ang;
	float dist;
};


enum UNIT_TYPE
{
	UNIT_TYPE_NONE			= 0,
	UNIT_TYPE_BEAST			= 1,
	UNIT_TYPE_DRAGONKIN		= 2,
	UNIT_TYPE_DEMON			= 3,
	UNIT_TYPE_ELEMENTAL		= 4,
	UNIT_TYPE_GIANT			= 5,
	UNIT_TYPE_UNDEAD		= 6,
	UNIT_TYPE_HUMANOID		= 7,
	UNIT_TYPE_CRITTER		= 8,
	UNIT_TYPE_MECHANICAL	= 9,
	UNIT_TYPE_MISC			= 10,
	UNIT_TYPE_TOTEM			= 11,
	UNIT_TYPE_NONCOMBAT_PET	= 12,
	UNIT_TYPE_GAS_CLOUD		= 13,
	UNIT_TYPE_NUM			= 14,
};

enum CreatureFlag1
{
	CREATURE_FLAG1_TAMEABLE   = 0x0001,
	CREATURE_FLAG1_HERBLOOT   = 0x0100,
	CREATURE_FLAG1_MININGLOOT = 0x0200,
};

enum FAMILY
{
	FAMILY_WOLF = 1,
	FAMILY_CAT = 2,
	FAMILY_SPIDER = 3,
	FAMILY_BEAR = 4,
	FAMILY_BOAR = 5,
	FAMILY_CROCOLISK = 6,
	FAMILY_CARRION_BIRD = 7,
	FAMILY_CRAB = 8,
	FAMILY_GORILLA = 9,
	FAMILY_RAPTOR = 11,
	FAMILY_TALLSTRIDER = 12 ,
	FAMILY_FELHUNTER = 15,
	FAMILY_VOIDWALKER = 16,
	FAMILY_SUCCUBUS = 17,
	FAMILY_DOOMGUARD = 19,
	FAMILY_SCORPID = 20,
	FAMILY_TURTLE = 21, 
	FAMILY_IMP = 23,
	FAMILY_BAT = 24,
	FAMILY_HYENA = 25,
	FAMILY_OWL = 26,
	FAMILY_WIND_SERPENT = 27,
	FAMILY_REMOTE_CONTROL = 28,
	FAMILY_FELGUARD = 29,
	FAMILY_DRAGONHAWK = 30,
	FAMILY_RAVAGER = 31,
	FAMILY_WARP_STALKER = 32,
	FAMILY_SPOREBAT = 33,
	FAMILY_NETHER_RAY = 34,
	FAMILY_SERPENT = 35,
	FAMILY_MOTH = 37,
	FAMILY_CHIMAERA = 38,
	FAMILY_DEVILSAUR = 39,
	FAMILY_GHOUL = 40, //DK's minion
	FAMILY_SILITHID = 41,
	FAMILY_WORM = 42,
	FAMILY_RHINO = 43,
	FAMILY_WASP = 44,
	FAMILY_CORE_HOUND = 45,
	FAMILY_SPIRIT_BEAST = 46
};

enum ELITE
{
	ELITE_NORMAL = 0,
	ELITE_ELITE,
	ELITE_RAREELITE,
	ELITE_WORLDBOSS,
	ELITE_RARE
};
enum TIME_REMOVE_CORPSE
{
	TIME_CREATURE_REMOVE_CORPSE = 180000,
	TIME_CREATURE_REMOVE_RARECORPSE = 180000*3,
	TIME_CREATURE_REMOVE_BOSSCORPSE = 180000*5,
};

struct PetSpellCooldown
{
	uint32 spellId;
	int32 cooldown;
};

static uint32 Exotic[7] = { 38, 39, 41, 42, 43, 45, 46 };

class CreatureAIScript;
class GossipScript;
class AuctionHouse;
struct Trainer;
#define CALL_SCRIPT_EVENT(obj, func) if(obj && obj->IsInWorld() && obj->GetTypeId() == TYPEID_UNIT && obj->GetMapMgr() && static_cast<Creature*>(obj)->GetScript() != NULL) static_cast<Creature*>(obj)->GetScript()->func


uint8 get_byte(uint32 buffer, uint32 index);

///////////////////
/// Creature object

class SERVER_DECL Creature : public Unit
{
public:

	Creature(uint64 guid);
	virtual ~Creature();

	// Override superclass method that returns false
	bool IsCreature() { return true; }

	// For derived subclasses of Creature
	virtual bool IsVehicle() { return false; }
    bool IsPet() { return m_isPet; }

	bool Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info);
	void Load(CreatureProto * proto_, float x, float y, float z, float o=0);

	void AddToWorld();
	void AddToWorld(MapMgr * pMapMgr);
	void RemoveFromWorld(bool addrespawnevent, bool free_guid);

	/// Creation
	void Create ( const char* creature_name, uint32 mapid, float x, float y, float z, float ang);
	void CreateWayPoint ( uint32 WayPointID, uint32 mapid, float x, float y, float z, float ang);

	/// Updates
	virtual void Update( uint32 time );

	/// Creature inventory
	ARCEMU_INLINE uint32 GetItemIdBySlot(uint32 slot) { return m_SellItems->at(slot).itemid; }
	ARCEMU_INLINE uint32 GetItemAmountBySlot(uint32 slot) { return m_SellItems->at(slot).amount; }

	ARCEMU_INLINE bool HasItems() { return ((m_SellItems != NULL) ? true : false); }
	ARCEMU_INLINE CreatureProto* GetProto() { return proto; }

	int32 GetSlotByItemId(uint32 itemid)
	{
		uint32 slot = 0;
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
				return slot;
			else
				++slot;
		}
		return -1;
	}

	uint32 GetItemAmountByItemId(uint32 itemid)
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
				return ((itr->amount < 1) ? 1 : itr->amount);
		}
		return 0;
	}

	ARCEMU_INLINE void GetSellItemBySlot(uint32 slot, CreatureItem &ci)
	{
		ci = m_SellItems->at(slot);
	}

	void GetSellItemByItemId(uint32 itemid, CreatureItem &ci)
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
			{
				ci = (*itr);
				return;
			}
		}
		ci.amount = 0;
		ci.max_amount=0;
		ci.available_amount =0;
		ci.incrtime=0;
		ci.itemid = 0;
	}

	ItemExtendedCostEntry * GetItemExtendedCostByItemId( uint32 itemid )
	{
		for( std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr )
		{
			if ( itr->itemid == itemid )
				return itr->extended_cost;
		}
		return NULL;
	}

	ARCEMU_INLINE std::vector<CreatureItem>::iterator GetSellItemBegin() { return m_SellItems->begin(); }
	ARCEMU_INLINE std::vector<CreatureItem>::iterator GetSellItemEnd()   { return m_SellItems->end(); }
	ARCEMU_INLINE size_t GetSellItemCount() { return m_SellItems->size(); }
	void RemoveVendorItem(uint32 itemid)
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); ++itr)
		{
			if(itr->itemid == itemid)
			{
				m_SellItems->erase(itr);
				return;
			}
		}
	}
	void AddVendorItem( uint32 itemid, uint32 amount, ItemExtendedCostEntry * ec );
	void ModAvItemAmount(uint32 itemid, uint32 value);
	void UpdateItemAmount(uint32 itemid);
	/// Quests
	void _LoadQuests();
	bool HasQuests() { return m_quests != NULL; };
	bool HasQuest(uint32 id, uint32 type)
	{
		if(!m_quests) return false;
		for(std::list<QuestRelation*>::iterator itr = m_quests->begin(); itr != m_quests->end(); ++itr)
		{
			if((*itr)->qst->id == id && (*itr)->type & type)
				return true;
		}
		return false;
	}
	void AddQuest(QuestRelation *Q);
	void DeleteQuest(QuestRelation *Q);
	Quest *FindQuest(uint32 quest_id, uint8 quest_relation);
	uint16 GetQuestRelation(uint32 quest_id);
	uint32 NumOfQuests();
	list<QuestRelation *>::iterator QuestsBegin() { return m_quests->begin(); };
	list<QuestRelation *>::iterator QuestsEnd() { return m_quests->end(); };
	void SetQuestList(std::list<QuestRelation *>* qst_lst) { m_quests = qst_lst; };

	ARCEMU_INLINE bool isVendor()		const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR ); }
	ARCEMU_INLINE bool isTrainer()		const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER ); }
	ARCEMU_INLINE bool isClass()		const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER_CLASS ); }
	ARCEMU_INLINE bool isProf()			const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER_PROF ); }
	ARCEMU_INLINE bool isQuestGiver()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER ); }
	ARCEMU_INLINE bool isGossip()		const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP ); }
	ARCEMU_INLINE bool isTaxi()			const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TAXIVENDOR ); }
	ARCEMU_INLINE bool isCharterGiver()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_ARENACHARTER ); }
	ARCEMU_INLINE bool isGuildBank()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GUILD_BANK ); }
	ARCEMU_INLINE bool isBattleMaster()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_BATTLEFIELDPERSON ); }
	ARCEMU_INLINE bool isBanker()		const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_BANKER ); }
	ARCEMU_INLINE bool isInnkeeper()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_INNKEEPER ); }
	ARCEMU_INLINE bool isSpiritHealer()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER ); }
	ARCEMU_INLINE bool isTabardDesigner() const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TABARDCHANGER ); }
	ARCEMU_INLINE bool isAuctioner()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER ); }
	ARCEMU_INLINE bool isStableMaster()	const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_STABLEMASTER ); }
	ARCEMU_INLINE bool isArmorer()		const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_ARMORER ); }

	ARCEMU_INLINE uint32 GetHealthFromSpell() { return m_healthfromspell; }
	void SetHealthFromSpell(uint32 value) { m_healthfromspell = value;}

	int32 m_speedFromHaste;
	int32 FlatResistanceMod[7];
	int32 BaseResistanceModPct[7];
	int32 ResistanceModPct[7];

	int32 FlatStatMod[5];
	int32 StatModPct[5];
	int32 TotalStatModPct[5];

	int32 ModDamageDone[7];
	float ModDamageDonePct[7];
	void CalcResistance(uint32 type);
	void CalcStat(uint32 type);

	bool m_canRegenerateHP;
	void RegenerateHealth();
	void RegenerateMana();
	int BaseAttackType;

	int32 AISpellsCooldown[4];

	bool CanSee(Unit* obj) // * Invisibility & Stealth Detection - Partha *
	{
		if(!obj)
			return false;

		if(obj->m_invisible) // Invisibility - Detection of Players and Units
		{
			if(obj->getDeathState() == CORPSE) // can't see dead players' spirits
				return false;

			if(m_invisDetect[obj->m_invisFlag] < 1) // can't see invisible without proper detection
				return false;
		}

		if(obj->IsStealth()) // Stealth Detection (  I Hate Rogues :P  )
		{
			if(isInFront(obj)) // stealthed player is in front of creature
			{
				// Detection Range = 5yds + (Detection Skill - Stealth Skill)/5
				if(getLevel() < 70)
					detectRange = 5.0f + getLevel() + 0.2f * (float)(GetStealthDetectBonus() - obj->GetStealthLevel());
				else
					detectRange = 75.0f + 0.2f * (float)(GetStealthDetectBonus() - obj->GetStealthLevel());

				if(detectRange < 1.0f) detectRange = 1.0f; // Minimum Detection Range = 1yd
			}
			else // stealthed player is behind creature
			{
				if(GetStealthDetectBonus() > 1000) return true; // immune to stealth
				else detectRange = 0.0f;
			}

			detectRange += GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of creature
			detectRange += obj->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of stealthed player

			if(GetDistance2dSq(obj) > detectRange * detectRange)
				return false;
		}

		return true;
	}

	//Make this unit face another unit
	bool setInFront(Unit* target);

	/// Looting
	void generateLoot();
	bool HasLootForPlayer(Player * plr);

	bool Skinned;
	uint32 GetRequiredLootSkill();

	bool Tagged;
	uint64 TaggerGuid;

	/// Misc
	ARCEMU_INLINE void setEmoteState(uint8 emote) { m_emoteState = emote; };
	ARCEMU_INLINE uint32 GetSQL_id() { return spawnid; };

	virtual void setDeathState(DeathState s);

	uint32 GetOldEmote() { return m_oldEmote; }

	// Serialization
	void SaveToDB();
	void SaveToFile(std::stringstream & name);
	//bool LoadFromDB(uint32 guid);
	//bool LoadFromDB(CreatureTemplate *t);
	void LoadAIAgents(CreatureTemplate * t);
	void LoadAIAgents();
	void DeleteFromDB();

	void OnJustDied();
	void OnRemoveCorpse();
	void OnRespawn(MapMgr * m);
	void SafeDelete();
	//void Despawn();
	void SummonExpire(); // this is used for guardians. They are non respawnable creatures linked to a player


	// In Range
	void AddInRangeObject(Object* pObj);
	void OnRemoveInRangeObject(Object* pObj);
	void ClearInRangeSet();

	// Demon
	void EnslaveExpire();

	// Pet
	void UpdatePet();
	uint32 GetEnslaveCount() { return m_enslaveCount; }
	void SetEnslaveCount(uint32 count) { m_enslaveCount = count; }
	uint32 GetEnslaveSpell() { return m_enslaveSpell; }
	void SetEnslaveSpell(uint32 spellId) { m_enslaveSpell = spellId; }
	bool RemoveEnslave();

	ARCEMU_INLINE Player *GetTotemOwner() { return totemOwner; }
	ARCEMU_INLINE void SetTotemOwner(Player *owner) { totemOwner = owner; }
	ARCEMU_INLINE uint32 GetTotemSlot() { return totemSlot; }
	ARCEMU_INLINE void SetTotemSlot(uint32 slot) { totemSlot = slot; }

	virtual Group *GetGroup();

	ARCEMU_INLINE bool IsPickPocketed() { return m_PickPocketed; }
	ARCEMU_INLINE void SetPickPocketed(bool val = true) { m_PickPocketed = val; }

	ARCEMU_INLINE CreatureAIScript * GetScript() { return _myScriptClass; }
	void LoadScript();

	void CallScriptUpdate();

	uint32 m_TaxiNode;

	ARCEMU_INLINE CreatureInfo *GetCreatureInfo() { return creature_info; }
	ARCEMU_INLINE void SetCreatureInfo(CreatureInfo *ci) { creature_info = ci; }
	ARCEMU_INLINE Trainer* GetTrainer() { return mTrainer; }
	void RegenerateFocus();

	CreatureFamilyEntry * myFamily;
	ARCEMU_INLINE bool IsTotem() { return totemOwner != 0 && totemSlot != -1; }

	ARCEMU_INLINE bool IsExotic()
	{
		for(uint32 i = 0; i < 7; ++i)
      {
			if(GetCreatureInfo()->Family == Exotic[i] && GetCreatureInfo()->Type == UNIT_TYPE_BEAST)
         { return true; }
      }
		return false;
	}

	void TotemExpire();
	void FormationLinkUp(uint32 SqlId);
	void ChannelLinkUpGO(uint32 SqlId);
	void ChannelLinkUpCreature(uint32 SqlId);
	bool haslinkupevent;
	WayPoint * CreateWaypointStruct();
	uint32 spawnid;
	uint32 original_emotestate;

	CreatureSpawn * m_spawn;

	void AISpellUpdate();

	void OnPushToWorld();
	void Despawn(uint32 delay, uint32 respawntime);
	void TriggerScriptEvent(string func);

	AuctionHouse * auctionHouse;
	bool has_waypoint_text;
	bool has_combat_text;
	int8 m_lootMethod;

	void DeleteMe();
	bool CanAddToWorld();

	// scriptdev2
	uint32 GetNpcTextId();

	WayPointMap * m_custom_waypoint_map;
	Player * m_escorter;
	void DestroyCustomWaypointMap();
	bool IsInLimboState() { return m_limbostate; }
	uint32 GetLineByFamily(CreatureFamilyEntry * family){return family->skilline ? family->skilline : 0;};
	void RemoveLimboState(Unit * healer);
	void SetGuardWaypoints();
	bool m_corpseEvent;
	MapCell * m_respawnCell;
	bool m_noRespawn;
	uint32 m_respawnTimeOverride;
	LocationVector * m_transportPosition;
	uint32 m_transportGuid;
	WoWGuid m_transportNewGuid;

	float GetBaseParry();
	bool isattackable(CreatureSpawn *spawn);
protected:
	CreatureAIScript *_myScriptClass;
	bool m_limbostate;
	Trainer* mTrainer;

	void _LoadGoods();
	void _LoadGoods(std::list<CreatureItem*>* lst);
	void _LoadMovement();

	/// Vendor data
	std::vector<CreatureItem>* m_SellItems;

	/// Taxi data
	uint32 mTaxiNode;

	/// Quest data
	std::list<QuestRelation *>* m_quests;

	/// Pet
    bool m_isPet;
	uint32 m_enslaveCount;
	uint32 m_enslaveSpell;

	Player * totemOwner;
	int32 totemSlot;

	bool m_PickPocketed;
	uint32 _fields[UNIT_END];
	uint32 m_healthfromspell;

	CreatureInfo *creature_info;
	CreatureProto * proto;
};

#endif

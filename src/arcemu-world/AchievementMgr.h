#ifndef _ACHIEVEMENTMGR_H
#define _ACHIEVEMENTMGR_H 

// Achievement Reward Types
//! ACHIEVEMENT_REWARDTYPE_NONE (0) : Achievement has no reward
#define ACHIEVEMENT_REWARDTYPE_NONE 0
//! ACHIEVEMENT_REWARDTYPE_ITEM (1) : Achievement has an item reward
#define ACHIEVEMENT_REWARDTYPE_ITEM 1
//! ACHIEVEMENT_REWARDTYPE_TITLE (2) : Achievement has a title reward
#define ACHIEVEMENT_REWARDTYPE_TITLE 2
//! ACHIEVEMENT_REWARDTYPE_SPELL (4) : Achievement has a spell reward
#define ACHIEVEMENT_REWARDTYPE_SPELL 4

// Achievement Flags
//! ACHIEVEMENT_FLAG_NONE (0) : No flags
#define ACHIEVEMENT_FLAG_NONE 0x0000
//! ACHIEVEMENT_FLAG_COUNTER (1) : Counter flag; used for statistics
#define ACHIEVEMENT_FLAG_COUNTER 0x0001
//! ACHIEVEMENT_FLAG_REACH_LEVEL (4) : Reach level? Currently unused in ArcEmu.
#define ACHIEVEMENT_FLAG_REACH_LEVEL 0x0004
//! ACHIEVEMENT_FLAG_AVERAGE (64) : Average? Currently unused in ArcEmu.
#define ACHIEVEMENT_FLAG_AVERAGE 0x0040
//! ACHIEVEMENT_FLAG_REALM_FIRST_REACH (256) : Realm First achievement
#define ACHIEVEMENT_FLAG_REALM_FIRST_REACH 0x0100
//! ACHIEVEMENT_FLAG_REALM_FIRST_KILL (512) : Realm First Kill; Currently only used in Realm First! Obsidian Slayer (456)
#define ACHIEVEMENT_FLAG_REALM_FIRST_KILL 0x0200

//! ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL (2) : All criteria must be completed for the achievement to be complete.
#define ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL 2
//! ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_SOME (4) : Some of the criteria must be completed for the achievement to be complete.
#define ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_SOME 4

//! ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP (2) : You must not be in a group to complete the achievement.
#define ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP 2

//! ACHIEVEMENT_FACTION_FLAG_ALLIANCE (0) : Alliance-only achievement
#define ACHIEVEMENT_FACTION_FLAG_ALLIANCE 0
//! ACHIEVEMENT_FACTION_FLAG_HORDE (1) : Horde-only achievement
#define ACHIEVEMENT_FACTION_FLAG_HORDE 1



/**
	CriteriaProgress structure
*/
struct CriteriaProgress
{
	CriteriaProgress(uint32 iid, uint32 icounter, time_t tdate = time(NULL))
	:
	id(iid),
	counter(icounter),
	date(tdate)
	{ }

	uint32 id;     //! Criteria ID
	int32 counter; //! Completed count: how many times the criteria has been completed
	time_t date;   //! Date/time
};

typedef HM_NAMESPACE::hash_map<uint32, CriteriaProgress*> CriteriaProgressMap;
typedef HM_NAMESPACE::hash_map<uint32, time_t> CompletedAchievementMap;
typedef std::set<uint32> AchievementSet;

class Player;
class WorldPacket;
class ObjectMgr;

/**
	Achievement completion state
*/
enum AchievementCompletionState
{
	ACHIEVEMENT_COMPLETED_NONE,                 //!< #0# Achievement is not completed
	ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED, //!< #1# Achievement is completed, but not stored yet
	ACHIEVEMENT_COMPLETED_COMPLETED_STORED,     //!< #2# Achievement is completed and has been stored
};

/**
	Achievement criteria conditions
	Currently these are not being used at all.
*/
enum AchievementCriteriaCondition
{
	ACHIEVEMENT_CRITERIA_CONDITION_NONE      = 0,  //!< #0# No condition
	ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH  = 1,  //!< #1# Must not die?
	ACHIEVEMENT_CRITERIA_CONDITION_UNK1      = 2,  //!< #2# only used in "Complete a daily quest every day for five consecutive days"
	ACHIEVEMENT_CRITERIA_CONDITION_MAP       = 3,  //!< #3# requires you to be on specific map
	ACHIEVEMENT_CRITERIA_CONDITION_NO_LOOSE  = 4,  //!< #4# only used in "Win 10 arenas without losing"
	ACHIEVEMENT_CRITERIA_CONDITION_UNK2      = 9,  //!< #9# unk
	ACHIEVEMENT_CRITERIA_CONDITION_UNK3      = 13, //!< #13# unk
};

/**
	Achievement Criteria Types
*/
enum AchievementCriteriaTypes
{
	ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0,               //!< #0# Kill creature x
	ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1,                      //!< #1# Win battleground
	ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5,                 //!< #5# Reach level x
	ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7,           //!< #7#  Reach skill level x
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8,        //!< #8#  Complete an achievement
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9,        //!< #9#  Complete x quests
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10, //!< #10# Complete daily quest x days in a row
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11,    //!< #11# Complete quests in zone x
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14,       //!< #14# Complete daily quest
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15,       //!< #15# Complete battleground
	ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16,                //!< #16# Death at map
	ACHIEVEMENT_CRITERIA_TYPE_DEATH= 17,                       //!< #17# Death
	ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON = 18,           //!< #18# Death in dungeon
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19,              //!< #19# Complete raid
	ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20,         //!< #20# Killed by creature
	ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER = 23,           //!< #23# Killed by player
	ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24,         //!< #24# Fall without dying
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27,             //!< #27# Complete quest
	ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28,            //!< #28# Have spell x cast on you
	ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29,                  //!< #29# Cast spell x
	ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE = 30,       //!< #30# Capture a battleground objective
	ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31,     //!< #31# Get an honorable kill at area
	ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32,                  //!< #32# Win arena
	ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33,                 //!< #33# Play arena
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34,                //!< #34# Learn spell x
	ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL = 35,             //!< #35# Honorable kill
	ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36,                   //!< #36# Own item x
	// TODO: the achievements 1162 and 1163 requires a special rating which can't be found in the dbc
	ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38,
	ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING = 39,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40,
	ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM= 42,
	ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43,
	ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK= 44,
	ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT= 45,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION= 46,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47,
	ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP= 48,
	ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49,
	ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT = 50,
	ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52,
	ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53,
	ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54,
	ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55,
	ACHIEVEMENT_CRITERIA_TYPE_KILLING_BLOW = 56,
	ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS = 60,
	ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS = 61,
	ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD = 62,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING = 63,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER = 65,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL = 66,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67,
	ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68,
	ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2= 69,
	ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL= 70,
	ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72,
	// TODO: title id is not mentioned in dbc
	ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE = 74,
	ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS= 75,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76,
	ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL = 77,
	ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE = 78,
	ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS= 80,
	ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION= 82,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID= 83,
	ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS= 84,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD = 85,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED = 86,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION = 87,
	ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION = 88,
	ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS = 89,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM = 90,
	ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM = 91,
	ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED = 93,
	ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED = 94,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALTH = 95,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_ARMOR = 99,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT = 101,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED = 102,
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED = 103,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED = 104,
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED = 105,
	ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED = 106,
	ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED = 107,
	ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN = 108,
	ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109,
	// TODO: target entry is missing
	ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE= 112,
	ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113,
	ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS = 114,
	// 0..114 => 124 criteria types total : Alleycat - We need to get the new Acheivements!
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL = 124,
};

/**
	Achievement Reward: contains type of reward(s) and Id(s).
*/
struct AchievementReward
{
	uint32 type;   //! type(s) of reward(s), ACHIEVEMENT_REWARDTYPE_NONE | ACHIEVEMENT_REWARDTYPE_ITEM | ACHIEVEMENT_REWARDTYPE_TITLE | ACHIEVEMENT_REWARDTYPE_SPELL
	uint32 itemId; //! itemId of item reward
	uint32 rankId; //! rankId of title reward
	uint32 spellId; //! spellId of spell reward
};

/**
AchievementMgr class
Achievement Working List:
- ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL
- ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM
- ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM
- ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA
- ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY
- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT
- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE
- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST
- ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD
- ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION
- ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION
- ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT
- ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL
- ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE
- ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL
- ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL
- ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM
- ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM
- ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS
- ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE (partial)
- ACHIEVEMENT_CRITERIA_TYPE_KILLING_BLOW (some)
- ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE (some)
- ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE
- ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM
- ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT
- ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT
- ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP
- ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER
- ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING
- ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL
- ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA
- ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER
- ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE
- ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS
- ACHIEVEMENT_CRITERIA_TYPE_HK_RACE
- ACHIEVEMENT_CRITERIA_TYPE_DEATH
- ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP
- ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET
- ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2
- Realm-First achievements (most)
- Reward Titles
- Reward Spells
- Reward Items (goes to inventory?)
\todo Several achievement types
\todo Time limits on achievements
\todo Special conditions for achievements (group size, nobody in raid die during fight, etc.)
*/
class SERVER_DECL AchievementMgr
{
public:
	AchievementMgr(Player* pl);
	~AchievementMgr();
	void LoadFromDB(QueryResult *achievementResult, QueryResult *criteriaResult);
	void SaveToDB(QueryBuffer *buf);
	void CheckAllAchievementCriteria();
	void SendAllAchievementData(Player* player);
	void UpdateAchievementCriteria(AchievementCriteriaTypes type, int32 miscvalue1, int32 miscvalue2, uint32 time);
	void UpdateAchievementCriteria(AchievementCriteriaTypes type);
	bool GMCompleteAchievement(WorldSession* gmSession, int32 achievementID);
	bool GMCompleteCriteria(WorldSession* gmSession, int32 criteriaID);
	void GMResetAchievement(int achievementID);
	void GMResetCriteria(int criteriaID);
	bool HasCompleted(uint32 achievementID);
	uint32 GetCompletedAchievementsCount() const;
	uint32 GetCriteriaProgressCount();
	time_t GetCompletedTime(AchievementEntry const* achievement);
	Player* GetPlayer() { return m_player; }

private:
	void GiveAchievementReward(AchievementEntry const* entry);
	void SendAchievementEarned(AchievementEntry const* achievement);
	void SendCriteriaUpdate(CriteriaProgress *progress);
	void SetCriteriaProgress(AchievementCriteriaEntry const* entry, int32 newValue, bool relative = false);
	void UpdateCriteriaProgress(AchievementCriteriaEntry const* entry, int32 updateByValue);
	void CompletedCriteria(AchievementCriteriaEntry const* entry);
	void CompletedAchievement(AchievementEntry const* entry);
	bool IsCompletedCriteria(AchievementCriteriaEntry const* entry);
	AchievementCompletionState GetAchievementCompletionState(AchievementEntry const* entry);

	RWLock m_lock;
	Player* m_player;
	CriteriaProgressMap m_criteriaProgress;
	CompletedAchievementMap m_completedAchievements;
	bool isCharacterLoading;
};

// Function declarations - related to achievements - not in AchievementMgr class - defined in AchievementMgr.cpp
uint32 GetAchievementIDFromLink(const char* achievementlink);
bool SendAchievementProgress(const CriteriaProgress* c);
bool SaveAchievementProgressToDB(const CriteriaProgress* c);


#endif


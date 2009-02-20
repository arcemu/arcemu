#ifndef _ACHIEVEMENTMGR_H
#define _ACHIEVEMENTMGR_H 

struct CriteriaProgress
{
    CriteriaProgress(uint32 iid, uint32 icounter, time_t tdate = time(NULL))
	:
	id(iid),
	counter(icounter),
	date(tdate)
    { }

    uint32 id;
    int32 counter;
    time_t date;
};

typedef HM_NAMESPACE::hash_map<uint32, CriteriaProgress*> CriteriaProgressMap;
typedef HM_NAMESPACE::hash_map<uint32, time_t> CompletedAchievementMap;
typedef std::set<uint32> AchievementSet;

class Player;
class WorldPacket;
class ObjectMgr;

enum AchievementCompletionState
{
    ACHIEVEMENT_COMPLETED_NONE,
    ACHIEVEMENT_COMPLETED_COMPLETED_NOT_STORED,
    ACHIEVEMENT_COMPLETED_COMPLETED_STORED,
};

enum AchievementFactionFlags
{
	ACHIEVEMENT_FACTION_FLAG_HORDE    = 0x00000000,
	ACHIEVEMENT_FACTION_FLAG_ALLIANCE = 0x00000001,
};

enum AchievementFlags
{
	ACHIEVEMENT_FLAG_NONE              = 0x00000000,
	ACHIEVEMENT_FLAG_COUNTER           = 0x00000001,
	ACHIEVEMENT_FLAG_REACH_LEVEL       = 0x00000004,
	ACHIEVEMENT_FLAG_UNKNOWN1          = 0x00000008,
	ACHIEVEMENT_FLAG_UNKNOWN2          = 0x00000010,
	ACHIEVEMENT_FLAG_UNKNOWN3          = 0x00000020, // This is set in one of Sporeggar reputation checks
	ACHIEVEMENT_FLAG_AVERAGE           = 0x00000040,
	ACHIEVEMENT_FLAG_REALM_FIRST_REACH = 0x00000100,
	ACHIEVEMENT_FLAG_REALM_FIRST_KILL  = 0x00000200,
};

enum AchievementCriteriaCondition
{
	ACHIEVEMENT_CRITERIA_CONDITION_NONE      = 0,
	ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH  = 1,
	ACHIEVEMENT_CRITERIA_CONDITION_UNK1      = 2,           // only used in "Complete a daily quest every day for five consecutive days"
	ACHIEVEMENT_CRITERIA_CONDITION_MAP       = 3,           // requires you to be on specific map
	ACHIEVEMENT_CRITERIA_CONDITION_NO_LOOSE  = 4,           // only used in "Win 10 arenas without losing"
	ACHIEVEMENT_CRITERIA_CONDITION_UNK2      = 9,           // unk
	ACHIEVEMENT_CRITERIA_CONDITION_UNK3      = 13,          // unk
};

enum AchievementCriteriaCompletionFlags
{
	// some Achievements (like 698) have several criteria but only one has to be fulfilled. These are identified by this flag.
	ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL = 2,
	ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_SOME = 4,
};

enum AchievementCriteriaGroupFlags
{
	// you mustn't be in a group while fulfilling this achievement
	ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP = 2,
};

enum AchievementCriteriaTypes
{
	ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1,
	ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5,
	ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9,
	// you have to complete a daily quest x times in a row
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15,
	ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16,
	// TODO: this can be both arena and total deaths. Where is this difference in the dbc?
	ACHIEVEMENT_CRITERIA_TYPE_DEATH= 17,
	ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON = 18,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19,
	ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20,
	ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER = 23,
	ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24,
	ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27,
	ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28,
	// for some reason "Slay 15 Turkeys in 3 minutes" uses criteria type 28 (be spell target) - bug or on purpose?
	ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29,
	ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE = 30,
	ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31,
	ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32,
	ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33,
	ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34,
	// TODO: this criteria has additional conditions which can not be found in the dbcs
	ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL = 35,
	ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36,
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
	// noted: rewarded as soon as the player payed, not at taking place at the seat
	ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP= 48,
	ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49,
	// TODO: itemlevel is mentioned in text but not present in dbc
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
	// TODO: creature type (demon, undead etc.) is not stored in dbc
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
	// 0..114 => 115 criteria types total
	ACHIEVEMENT_CRITERIA_TYPE_TOTAL = 115,
};

enum AchievementRewardTypes
{
	ACHIEVEMENT_REWARDTYPE_NONE = 0,
	ACHIEVEMENT_REWARDTYPE_ITEM = 1,
	ACHIEVEMENT_REWARDTYPE_TITLE = 2,
	ACHIEVEMENT_REWARDTYPE_SPELL = 4,
// This is used as a bit field, next type (if any more) will be 8
};

struct AchievementReward
{
	uint32 type;
	uint32 itemId;
	uint32 rankId;
	uint32 spellId;
};

class AchievementMgr
{
public:
	AchievementMgr(Player* pl);
	~AchievementMgr();

	void							LoadFromDB(QueryResult *achievementResult, QueryResult *criteriaResult);
	void							SaveToDB();
	void							CheckAllAchievementCriteria();
	void							SendAllAchievementData(Player* player);
	void							SendRespondInspectAchievements(Player* player);
	void							UpdateAchievementCriteria(AchievementCriteriaTypes type, int32 miscvalue1, int32 miscvalue2, uint32 time);
	void							UpdateAchievementCriteria(AchievementCriteriaTypes type);
	void							GiveAchievementReward(AchievementEntry const* entry);
	uint32							GetCompletedAchievementsCount() const;
	uint32							GetCriteriaProgressCount();
	void							BuildAllDataPacket(WorldPacket *data, bool self=true);
	bool GMCompleteAchievement(WorldSession* gmSession, uint32 achievementID); // completes an achievement for the selected player
	bool GMCompleteCriteria(WorldSession* gmSession, uint32 criteriaID); // completes an achievement criteria for the selected player
	void GMResetAchievement(int achievementID); // erase achievement ID from character (id -1 = erase ALL achievements)
	void GMResetCriteria(int criteriaID); // erase achievement criteria ID from character (id -1 = erase ALL achievement criteria)
	time_t GetCompletedTime(AchievementEntry const* achievement); // returns Date/Time achievement was completed, or 0 if achievement not completed
	bool HasCompleted(uint32 achievementID); // true is player has completed achievement

	Player*							GetPlayer()
									{ return m_player; }

private:
	void							SendAchievementEarned(AchievementEntry const* achievement);
	void							SendCriteriaUpdate(CriteriaProgress *progress);
	void							SetCriteriaProgress(AchievementCriteriaEntry const* entry, int32 newValue, bool relative=false);
	void							UpdateCriteriaProgress(AchievementCriteriaEntry const* entry, int32 updateByValue);
	void							CompletedCriteria(AchievementCriteriaEntry const* entry);
	void							CompletedAchievement(AchievementEntry const* entry);
	bool							IsCompletedCriteria(AchievementCriteriaEntry const* entry);
	AchievementCompletionState		GetAchievementCompletionState(AchievementEntry const* entry);

	Player *						m_player;
	CriteriaProgressMap				m_criteriaProgress;
	CompletedAchievementMap			m_completedAchievements;
};


#endif


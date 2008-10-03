/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef _GUILD_H
#define _GUILD_H

struct PlayerInfo;

#define MAX_GUILD_RANKS 10
enum PETITION_TURNIN_ERRORS
{
	ERR_PETITION_OK,
	ERR_PETITION_ALREADY_SIGNED,
	ERR_PETITION_IN_GUILD,
	ERR_PETITION_CREATOR,
	ERR_PETITION_NOT_ENOUGH_SIGNATURES,

	//PETITION_YOU_ALREADY_IN_A_GUILD = 0x02,
	//PETITION_YOU_NEED_MORE_SIGNS	= 0x04,
	//ERR_PET_SPELL_DEAD
	//ERR_PETITION_DECLINED_S
	//ERR_PETITION_SIGNED_S
	//ERR_PETITION_SIGNED
	//ERR_PETITION_OFFERED

};

enum GUILDEMBLEM_ERRORS
{
	ERR_GUILDEMBLEM_SUCCESS,
	ERR_GUILDEMBLEM_INVALID_TABARD_COLORS,
	ERR_GUILDEMBLEM_NOGUILD,
	ERR_GUILDEMBLEM_NOTGUILDMASTER,
	ERR_GUILDEMBLEM_NOTENOUGHMONEY,
	ERR_GUILDEMBLEM_INVALIDVENDOR,
};

enum GuildMessageTypes
{
	G_MSGTYPE_ALL,
	G_MSGTYPE_ALLBUTONE,
	G_MSGTYPE_PUBLICCHAT,
	G_MSGTYPE_OFFICERCHAT,
};

enum MONEY
{
	MONEY_ONE_COPPER			= 1,
	MONEY_ONE_SILVER			= MONEY_ONE_COPPER * 100,
	MONEY_ONE_GOLD				= MONEY_ONE_SILVER * 100,
};

enum CommandErrors
{
	GUILD_U_HAVE_INVITED = 0x00,
	GUILD_INTERNAL				= 0x01,
	GUILD_ALREADY_IN_GUILD		= 0x02,
	ALREADY_IN_GUILD			  = 0x03,
	INVITED_TO_GUILD			  = 0x04,
	ALREADY_INVITED_TO_GUILD	  = 0x05,
	GUILD_NAME_INVALID			= 0x06,
	GUILD_NAME_EXISTS			 = 0x07,
	GUILD_LEADER_LEAVE			= 0x08,
	GUILD_PERMISSIONS			 = 0x08,
	GUILD_PLAYER_NOT_IN_GUILD	 = 0x09,
	GUILD_PLAYER_NOT_IN_GUILD_S   = 0x0A,
	GUILD_PLAYER_NOT_FOUND		= 0x0B,
	GUILD_NOT_ALLIED			  = 0x0C,
};

//for uint32(0)<<name<<code
enum GUILD_COMMAND_RESULTS
{
	C_R_CREATED							 = 1,
	C_R_ALREADY_IN_GUILD					= 2,
	C_R_TARGET_ALREADY_IN_GUILD			 = 3,
	C_R_ALREADY_INVITED_TO_GUILD			= 4,
	C_R_TARGET_ALREADY_INVITED_TO_GUILD	 = 5,
	C_R_GUILDNAME_HAS_INVALID_CHARACTERS	= 6,
	C_R_GUILD_NAME_EXISTS				   = 7,
	C_R_DONT_HAVE_PERMISSION				= 8,
	C_R_NOT_IN_GUILD						= 9,
	C_R_TARGET_IS_NOT_IN_YOUR_GUILD		 = 10,
	C_R_NAME_NOT_FOUND					  = 11,
	C_R_CANT_INVITE_PLYRS_FROM_OPP_ALLIANCE = 12,
	C_R_NAME_RANK_TOO_HIGH				  = 13,
	C_R_NAME_RANK_AT_LOWEST_RANK			= 14,
	/*
	ERR_NO_GUILD_CHARTER

	ERR_GUILD_NAME_NAME_CONSECUTIVE_SPACES
	ERR_GUILD_NAME_INVALID_SPACE
	ERR_GUILD_NAME_RESERVED
	ERR_GUILD_NAME_PROFANE
	ERR_GUILD_NAME_MIXED_LANGUAGES
	ERR_GUILD_NAME_TOO_SHORT

	ERR_GUILD_ENTER_NAME

	ERR_GUILD_NAME_EXISTS_S
	ERR_GUILD_NAME_INVALID

	ERR_GUILD_RANK_TOO_LOW_S
	ERR_GUILD_RANK_TOO_HIGH_S
	ERR_GUILD_RANK_IN_USE
	ERR_GUILD_RANKS_LOCKED
	ERR_GUILD_LEADER_LEAVE
	ERR_GUILD_NOT_ALLIED
	ERR_GUILD_DISBANDED
	ERR_GUILD_LEADER_CHANGED_SS
	ERR_GUILD_LEADER_IS_S
	ERR_GUILD_INTERNAL
	ERR_GUILD_NOT_IN_A_GUILD
	ERR_GUILD_CANT_DEMOTE_S
	ERR_GUILD_CANT_PROMOTE_S
	ERR_GUILD_PLAYER_NOT_IN_GUILD
	ERR_GUILD_PLAYER_NOT_IN_GUILD_S
	ERR_GUILD_PLAYER_NOT_FOUND_S
	ERR_GUILD_LEADER_SELF
	ERR_GUILD_LEADER_S
	ERR_GUILD_DISBAND_SELF
	ERR_GUILD_DISBAND_S
	ERR_GUILD_REMOVE_SELF
	ERR_GUILD_REMOVE_SS
	ERR_GUILD_LEAVE_S
	ERR_GUILD_QUIT_S
	ERR_GUILD_DEMOTE_SSS
	ERR_GUILD_PROMOTE_SSS
	ERR_GUILD_FOUNDER_S
	ERR_GUILD_JOIN_S
	ERR_GUILD_PERMISSIONS
	ERR_GUILD_DECLINE_S
	ERR_GUILD_ACCEPT
	ERR_ALREADY_IN_GUILD
	ERR_INVITED_TO_GUILD
	ERR_ALREADY_INVITED_TO_GUILD_S
	ERR_ALREADY_IN_GUILD_S
	ERR_INVITED_TO_GUILD_SS
	ERR_GUILD_INVITE_S
	ERR_GUILD_CREATE_S
	*/
};

enum typecommand
{
	GUILD_CREATE_S	= 0x00,
	GUILD_INVITE_S	= 0x01,
	GUILD_QUIT_S	= 0x02,
	GUILD_PROMOTE_S	= 0x03,
	GUILD_FOUNDER_S = 0x0C,
	GUILD_MEMBER_S = 0x0D,
	GUILD_PUBLIC_NOTE_CHANGED_S = 0x13,
	GUILD_OFFICER_NOTE_CHANGED_S = 0x14,

};

enum GuildRankRights
{
	GR_RIGHT_EMPTY			           = 0x00040,
	GR_RIGHT_GCHATLISTEN	           = 0x00001,
	GR_RIGHT_GCHATSPEAK		           = 0x00002,
	GR_RIGHT_OFFCHATLISTEN	           = 0x00004,
	GR_RIGHT_OFFCHATSPEAK	           = 0x00008,
	GR_RIGHT_INVITE			           = 0x00010,
	GR_RIGHT_REMOVE			           = 0x00020,
	// unknown 0x00040
	GR_RIGHT_PROMOTE		           = 0x00080,
	GR_RIGHT_DEMOTE			           = 0x00100,
	// unknown 0x00200
	// unknown 0x00400
	// unknown 0x00800
	GR_RIGHT_SETMOTD		           = 0x01000,
	GR_RIGHT_EPNOTE			           = 0x02000,
	GR_RIGHT_VIEWOFFNOTE	           = 0x04000,
	GR_RIGHT_EOFFNOTE		           = 0x08000,
	GR_RIGHT_EGUILDINFO		           = 0x10000,
	GR_RIGHT_GUILD_BANK_REPAIR         = 0x40000,
	GR_RIGHT_GUILD_BANK_WITHDRAW_MONEY = 0x80000,
	GR_RIGHT_ALL			= 0xDF1FF,
	GR_RIGHT_DEFAULT		= GR_RIGHT_GCHATLISTEN | GR_RIGHT_GCHATSPEAK | GR_RIGHT_EMPTY,

	GR_RIGHT_GUILD_BANK_VIEW_TAB		= 0x01,
	GR_RIGHT_GUILD_BANK_DEPOSIT_ITEMS	= 0x02,
};


//#define GUILDRANK_INVITE GUILDRANK_VETERAN
//#define GUILDRANK_KICK GUILDRANK_OFFICER
//#define GUILDRANK_PROMOTE GUILDRANK_VETERAN
//#define GUILDRANK_DEMOTE GUILDRANK_VETERAN
//#define GUILDRANK_MOTD GUILDRANK_OFFICER

enum GuildEvent
{
	GUILD_EVENT_PROMOTION		   =0x0,
	GUILD_EVENT_DEMOTION			=0x1,
	GUILD_EVENT_MOTD				=0x2,
	GUILD_EVENT_JOINED			  =0x3,
	GUILD_EVENT_LEFT				=0x4,
	GUILD_EVENT_REMOVED			 =0x5,
	GUILD_EVENT_LEADER_IS		   =0x6,
	GUILD_EVENT_LEADER_CHANGED	  =0x7,
	GUILD_EVENT_DISBANDED		   =0x8,
	GUILD_EVENT_TABARDCHANGE		=0x9,
	GUILD_EVENT_UNK1				=0xA,
	GUILD_EVENT_UNK2				=0xB,
	GUILD_EVENT_HASCOMEONLINE	   =0xC,
	GUILD_EVENT_HASGONEOFFLINE	  =0xD,
	GUILD_EVENT_BANKTABBOUGHT		= 0xF,
	GUILD_EVENT_SETNEWBALANCE		= 0x11,
};
enum GuildLogEventE
{
	GUILD_LOG_EVENT_INVITE		= 1,
	GUILD_LOG_EVENT_JOIN		= 2,
	GUILD_LOG_EVENT_PROMOTION	= 3,
	GUILD_LOG_EVENT_DEMOTION	= 4,
	GUILD_LOG_EVENT_REMOVAL		= 5,
	GUILD_LOG_EVENT_LEFT		= 6,
};

enum GuildBankLogEvents
{
	GUILD_BANK_LOG_EVENT_DEPOSIT_ITEM	= 1,
	GUILD_BANK_LOG_EVENT_WITHDRAW_ITEM	= 2,
	GUILD_BANK_LOG_EVENT_DEPOSIT_MONEY	= 4,
	GUILD_BANK_LOG_EVENT_WITHDRAW_MONEY	= 5,
};

#define ITEM_ENTRY_GUILD_CHARTER 5863
#define ARENA_TEAM_CHARTER_2v2      23560
#define ARENA_TEAM_CHARTER_2v2_COST 800000  // 80 G
#define ARENA_TEAM_CHARTER_3v3      23561
#define ARENA_TEAM_CHARTER_3v3_COST 1200000 // 120 G
#define ARENA_TEAM_CHARTER_5v5      23562
#define ARENA_TEAM_CHARTER_5v5_COST 2000000 // 200 G

#define MAX_GUILD_BANK_SLOTS 98
#define MAX_GUILD_BANK_TABS 6

struct SERVER_DECL GuildRankTabPermissions
{
	uint32 iFlags;
	int32 iStacksPerDay;
};

struct SERVER_DECL GuildRank
{
	uint32 iId;
	uint32 iRights;
	int32 iGoldLimitPerDay;
	GuildRankTabPermissions iTabPermissions[MAX_GUILD_BANK_TABS];
	char * szRankName;
	bool CanPerformCommand(uint32 t);
	bool CanPerformBankCommand(uint32 t, uint32 tab);
};

struct SERVER_DECL GuildMember
{
	PlayerInfo * pPlayer;
	const char * szPublicNote;
	const char * szOfficerNote;
	GuildRank * pRank;
	uint32 uLastWithdrawReset;
	uint32 uWithdrawlsSinceLastReset;
	uint32 uLastItemWithdrawReset[MAX_GUILD_BANK_TABS];
	uint32 uItemWithdrawlsSinceLastReset[MAX_GUILD_BANK_TABS];

	uint32 CalculateAllowedItemWithdraws(uint32 tab);
	void OnItemWithdraw(uint32 tabid);
	
	uint32 CalculateAvailableAmount();
	void OnMoneyWithdraw(uint32 amt);
};

struct SERVER_DECL GuildLogEvent
{
	uint32 iLogId;
	uint8 iEvent;
	uint32 iTimeStamp;
	uint32 iEventData[3];
};

struct SERVER_DECL GuildBankEvent
{
	uint32 iLogId;
	uint8 iAction;
	uint32 uPlayer;
	uint32 uEntry;
	uint8 iStack;
	uint32 uTimeStamp;
};

struct SERVER_DECL GuildBankTab
{
	uint8 iTabId;
	char * szTabName;
	char * szTabIcon;
	Item * pSlots[MAX_GUILD_BANK_SLOTS];
	list<GuildBankEvent*> lLog;
};

class Charter;

typedef map<PlayerInfo*, GuildMember*> GuildMemberMap;

class SERVER_DECL Guild
{
public:
	Guild();
	~Guild( );
	static Guild* Create();
	bool LoadFromDB(Field * f);

	/** Log entry processing
	 */
protected:
	uint32 m_hiLogId;
public:
	uint32 GenerateGuildLogEventId();
	
	/* guild bank logging calls
	 */
	void LogGuildBankActionMoney(uint8 iAction, uint32 uGuid, uint32 uAmount);
	void LogGuildBankAction(uint8 iAction, uint32 uGuid, uint32 uEntry, uint8 iStack, GuildBankTab * pTab);
	static void ClearOutOfDateLogEntries();

	/* only call at first create/save */
	void CreateInDB();

	/** Sets new MOTD, and updates in database
	 */
	void SetMOTD(const char * szNewMotd, WorldSession * pClient);

	/** Gets MOTD
	 */
	ARCEMU_INLINE const char * GetMOTD() const { return m_motd; }

	/** Sets guild information, updates in database
	 */
	void SetGuildInformation(const char * szGuildInformation, WorldSession * pClient);

	/** Gets guild information
	 */
	ARCEMU_INLINE const char * GetGuildInformation() const { return m_guildInfo; }

	/** Sends the guild roster to this client.
	 */
	void SendGuildRoster(WorldSession * pClient);

	/** Sends the guild query response to this client.
	 */
	void SendGuildQuery(WorldSession * pClient);

	/** Adds a member to the guild, saves him into the database.
	 * A provided rank of -1 means the lowest rank.
	 */
	void AddGuildMember(PlayerInfo * pMember, WorldSession * pClient, int32 ForcedRank = -1);

	/** Removes a member from the guild.
	 * If this member is the guild master, the guild will be automatically handed down to the next
	 * highest member.
	 */
	void RemoveGuildMember(PlayerInfo * pMember, WorldSession * pClient);

	/** Promotes a member of a guild.
	 * Do not use for changing guild master. Use ChangeGuildMaster() for that instead.
	 */
	void PromoteGuildMember(PlayerInfo * pMember, WorldSession * pClient);

	/** Demotes a member of a guild.
	 * Do not use for changing guild master. Use ChangeGuildMaster() for that instead.
	 */
	void DemoteGuildMember(PlayerInfo * pMember, WorldSession * pClient);

	/** Changes the guild master of the guild.
	 */
	void ChangeGuildMaster(PlayerInfo * pNewMaster, WorldSession * pClient);

	/** Sends a guild command packet to the client.
	 */
	static void SendGuildCommandResult(WorldSession * pClient, uint32 iCmd, const char * szMsg, uint32 iType);

	/** Logs a guild event and sends it to all online players.
	 */
	void LogGuildEvent(uint8 iEvent, uint8 iStringCount, ...);
	
	/** Guild event logging.
	 */
	void AddGuildLogEntry(uint8 iEvent, uint8 iParamCount, ...);

	/** Creates a guild from a charter.
	 */
	void CreateFromCharter(Charter * pCharter, WorldSession * pTurnIn);

	/** Sends a packet to all online players.
	 */
	void SendPacket(WorldPacket * data);

	/** Sends a guild chat message.
	 */
	void GuildChat(const char * szMessage, WorldSession * pClient, uint32 iType);

	/** Sends an officer chat message.
	 */
	void OfficerChat(const char * szMessage, WorldSession * pClient, uint32 iType);

	/** Sends the guild log to a player.
	 */
	void SendGuildLog(WorldSession * pClient);
	void SendGuildBankLog(WorldSession * pClient, uint8 iSlot);

	/** Sets the public note for a player.
	 */
	void SetPublicNote(PlayerInfo * pMember, const char * szNewNote, WorldSession * pClient);

	/** Sets the officer note for a player.
	 */
	void SetOfficerNote(PlayerInfo * pMember, const char * szNewNote, WorldSession * pClient);

	/** Disbands a guild.
	 */
	void Disband();

	/** creation time stuff
	 */
	uint32 creationDay;
	uint32 creationMonth;
	uint32 creationYear;

	/** Getters :P
	 */
	ARCEMU_INLINE const char * GetGuildName() const { return m_guildName; }
	ARCEMU_INLINE const uint32 GetGuildLeader() const { return m_guildLeader; }
	ARCEMU_INLINE const uint32 GetGuildId() const { return m_guildId; }
	ARCEMU_INLINE const uint32 GetBankTabCount() const { return m_bankTabCount; }
	ARCEMU_INLINE const uint32 GetBankBalance() const { return m_bankBalance; }
	ARCEMU_INLINE const size_t GetNumMembers() const { return m_members.size(); }
	/** Creates a guild rank with the specified permissions.
	 */
	GuildRank * CreateGuildRank(const char * szRankName, uint32 iPermissions, bool bFullGuildBankPermissions);

	/** "Pops" or removes the bottom guild rank.
	 */
	void RemoveGuildRank(WorldSession * pClient);

	/** Buys a new guild bank tab, usable only by guild master 
	 */
	void BuyBankTab(WorldSession * pClient);

	/** Deposits money into the guild bank, usable by any member.
	 */
	void DepositMoney(WorldSession * pClient, uint32 uAmount);

	/** Withdraws money from the guild bank, usable by members with that permission.
	 */
	void WithdrawMoney(WorldSession * pClient, uint32 uAmount);

	/** Retrieves a guild rank for editing
	 */
	ARCEMU_INLINE GuildRank * GetGuildRank(uint32 Id)
	{ 
		if(Id >= MAX_GUILD_RANKS)
			return NULL;

		return m_ranks[Id];
	}

	/** Gets a guild bank tab for editing/viewing
	 */

	ARCEMU_INLINE GuildBankTab * GetBankTab(uint32 Id)
	{
		if(Id >= m_bankTabCount)
			return NULL;

		return m_bankTabs[Id];
	}

	/** Gets a guild member struct
	 */
	ARCEMU_INLINE GuildMember * GetGuildMember(PlayerInfo * pInfo)
	{
		GuildMemberMap::iterator itr;
		GuildMember * ret;
		m_lock.Acquire();
		itr = m_members.find(pInfo);
		ret = (itr!=m_members.end()) ? itr->second : NULL;
		m_lock.Release();
		return ret;
	}

	/* Get iterators */
	ARCEMU_INLINE GuildMemberMap::iterator GetGuildMembersBegin() { return m_members.begin(); }
	ARCEMU_INLINE GuildMemberMap::iterator GetGuildMembersEnd() { return m_members.end(); }

	/* Get, Lock, Unlock Mutex */
        ARCEMU_INLINE Mutex& getLock() { return m_lock; }
	ARCEMU_INLINE void Lock() { m_lock.Acquire(); }
	ARCEMU_INLINE void Unlock() { return m_lock.Release(); }

	/** Sends the guild bank to this client.
	 */
	void SendGuildBank(WorldSession * pClient, GuildBankTab * pTab, int8 updated_slot1 = -1, int8 updated_slot2 = -1);
	void SendGuildBankInfo(WorldSession * pClient);

	/** Changes the tabard info.
	 */
	void SetTabardInfo(uint32 EmblemStyle, uint32 EmblemColor, uint32 BorderStyle, uint32 BorderColor, uint32 BackgroundColor);
	
	/** Sends the guild information packet to the specified client.
	 */
	void SendGuildInfo(WorldSession * pClient);

protected:
	
	/** Enables/disables command logging.
	 * Use when performing mass events such as guild creation or destruction.
	 */
	bool m_commandLogging;
	
	/** Internal variables
	 */
	uint32 m_guildId;
	uint32 m_emblemStyle;
	uint32 m_emblemColor;
	uint32 m_borderStyle;
	uint32 m_borderColor;
	uint32 m_backgroundColor;
	uint32 m_guildLeader;
	uint32 m_creationTimeStamp;
	uint32 m_bankTabCount;
	uint32 m_bankBalance;

	typedef vector<GuildBankTab*> GuildBankTabVector;
	GuildBankTabVector m_bankTabs;

	char * m_guildName;
	char * m_guildInfo;
	char * m_motd;
	
	/** Guild Member Map.
	 */
	//typedef map<PlayerInfo*, GuildMember*> GuildMemberMap;
	GuildMemberMap m_members;

	/** Guild Rank Information.
	 */
	GuildRank * m_ranks[MAX_GUILD_RANKS];

	/** Guild log. Ordered in first event -> last event.
	 */
	typedef list<GuildLogEvent*> GuildLogList;
	GuildLogList m_log;
	list<GuildBankEvent*> m_moneyLog;

	/** Guild lock.
	 */
	Mutex m_lock;

	/** finds the lowest rank
	 */
	GuildRank * FindLowestRank();
	GuildRank * FindHighestRank();
};


#endif

/*0,1->guild created
2->you are already in guild
3->selection is already in guild
4->you have been already invited to guild
5->selection is already invited to guild
6->guildname contains invalid characters pls rename
7->there is an already guild named "name"
8->you dont have permission to do that
9->you are not in guild
10->selection isnot in your guild
11->"name" not found
12->you cannot invite players from opposite alliance
13->"name"'s rank is too high
14->"name" is already at lowest rank*/

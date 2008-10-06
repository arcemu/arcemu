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

#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H

ARCEMU_INLINE bool FindXinYString(std::string& x, std::string& y)
{
	return y.find(x) != std::string::npos;
}

typedef std::set<uint32> InstanceBossTrashList;
struct InstanceBossInfo
{
	uint32 mapid;
	uint32 creatureid;
	InstanceBossTrashList trash;
	uint32 trashRespawnOverride;
};

struct GM_Ticket
{
	uint64 guid;
	uint64 playerGuid;
	std::string name;
	uint32 level;
	uint32 map;
	float posX;
	float posY;
	float posZ;
	std::string message;
	uint32 timestamp;
	bool deleted;
	uint64 assignedToPlayer;
	std::string comment;
};

enum
{
	GM_TICKET_CHAT_OPCODE_NEWTICKET     = 1,
	GM_TICKET_CHAT_OPCODE_LISTSTART     = 2,
	GM_TICKET_CHAT_OPCODE_LISTENTRY     = 3,
	GM_TICKET_CHAT_OPCODE_CONTENT       = 4,
	GM_TICKET_CHAT_OPCODE_APPENDCONTENT = 5,
	GM_TICKET_CHAT_OPCODE_REMOVED       = 6,
	GM_TICKET_CHAT_OPCODE_UPDATED       = 7, 
	GM_TICKET_CHAT_OPCODE_ASSIGNED      = 8,
	GM_TICKET_CHAT_OPCODE_RELEASED      = 9, 
	GM_TICKET_CHAT_OPCODE_COMMENT       = 10,
	GM_TICKET_CHAT_OPCODE_ONLINESTATE   = 11
};

#pragma pack(push,1)
struct FishingZoneEntry
{
	uint32 ZoneID;
	uint32 MinSkill;
	uint32 MaxSkill;
};

struct ZoneGuardEntry
{
	uint32 ZoneID;
	uint32 HordeEntry;
	uint32 AllianceEntry;
};

struct UnitModelSizeEntry
{
	uint32	DisplayId;
	float	HalfSize;
};

struct ProfessionDiscovery
{
	uint32 SpellId;
	uint32 SpellToDiscover;
	uint32 SkillValue;
	float Chance;
};

struct ItemPage
{
    uint32 id;
	char * text;
	uint32 next_page;
};

struct WorldStringTable
{
	uint32 id;
	char * text;
};

struct WorldBroadCast
{
	uint32 id;
	char * text;
	uint32 percent;
};

#pragma pack(pop)

struct SpellReplacement
{
	uint32 count;
	uint32 *spells;
};

class Group;

struct GossipMenuItem
{
	uint32		Id;
	uint32		IntId;
	uint8		Icon;
	uint8		Extra;
	string		Text;
	uint32      m_gSender;
	uint32      m_gAction;
	string		m_gBoxMessage;
    uint32      m_gBoxMoney;
};
struct SpellEntry;
struct TrainerSpell
{
	SpellEntry * pCastSpell;
	SpellEntry * pLearnSpell;
	SpellEntry * pCastRealSpell;
	uint32	DeleteSpell;
	uint32	RequiredSpell;
	uint32	RequiredSkillLine;
	uint32	RequiredSkillLineValue;
	bool	IsProfession;
	uint32	Cost;
	uint32	RequiredLevel;
};

struct Trainer
{
	uint32 SpellCount;
	vector<TrainerSpell> Spells;
	char*	UIMessage;
    uint32 RequiredSkill;
	uint32 RequiredSkillLine;
	uint32 RequiredClass;
	uint32 TrainerType;
	uint32 Can_Train_Gossip_TextId;
	uint32 Cannot_Train_GossipTextId;
};

struct LevelInfo
{
	uint32 HP;
	uint32 Mana;
	uint32 Stat[5];
	uint32 XPToNextLevel;
};

struct ReputationMod
{
	uint32 faction[2];
	int32 value;
	uint32 replimit;
};

struct InstanceReputationMod
{
	uint32 mapid;
	uint32 mob_rep_reward;
	uint32 mob_rep_limit;
	uint32 boss_rep_reward;
	uint32 boss_rep_limit;
	uint32 faction[2];
};

struct ReputationModifier
{
	uint32 entry;
	vector<ReputationMod> mods;
};

struct InstanceReputationModifier
{
	uint32 mapid;
	vector<InstanceReputationMod> mods;
};

struct NpcMonsterSay
{
	float Chance;
	uint32 Language;
	uint32 Type;
	const char * MonsterName;

	uint32 TextCount;
	const char ** Texts;
};

enum MONSTER_SAY_EVENTS
{
	MONSTER_SAY_EVENT_ENTER_COMBAT		= 0,
	MONSTER_SAY_EVENT_RANDOM_WAYPOINT	= 1,
	MONSTER_SAY_EVENT_CALL_HELP			= 2,
	MONSTER_SAY_EVENT_ON_COMBAT_STOP	= 3,
	MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN	= 4,
	MONSTER_SAY_EVENT_ON_DIED			= 5,
	NUM_MONSTER_SAY_EVENTS,
};

enum AREATABLE_FLAGS
{
    AREA_CITY_AREA          = 0x20,
    AREA_NEUTRAL_AREA       = 0x40,
    AREA_PVP_ARENA          = 0x80,
    AREA_CITY               = 0x200,
    AREA_SANCTUARY          = 0x800,
    AREA_ISLAND             = 0x1000,
    AREA_PVP_OBJECTIVE_AREA = 0x8000,
};

enum AREATABLE_CATEGORY
{
    AREAC_CONTESTED          = 0,
    AREAC_ALLIANCE_TERRITORY = 2,
    AREAC_HORDE_TERRITORY    = 4,
    AREAC_SANCTUARY          = 6,
};

#define MAX_PREDEFINED_NEXTLEVELXP PLAYER_LEVEL_CAP
static const uint32 NextLevelXp[MAX_PREDEFINED_NEXTLEVELXP]= 
{
	400,	900,	1400,	2100,	2800,	3600,	4500,	5400,	6500,	7600,
	8700,	9800,	11000,	12300,	13600,	15000,	16400,	17800,	19300,	20800,
	22400,	24000,	25500,	27200,	28900,	30500,	32200,	33900,	36300,	38800,
	41600,	44600,	48000,	51400,	55000,	58700,	62400,	66200,	70200,	74300,
	78500,	82800,	87100,	91600,	95300,	101000,	105800,	110700,	115700,	120900,
	126100,	131500,	137000,	142500,	148200,	154000,	159900,	165800,	172000,	494000,
	574700,	614400,	650300,	682300,	710200,	734100,	753700,	768900,	779700,	790500,
#if PLAYER_LEVEL_CAP==80
	801300,	812100,	822900,	833700,	844500,	855300,	866100,	876900,	887700,	898500,
#endif
};

class SERVER_DECL GossipMenu
{
public:
	GossipMenu(uint64 Creature_Guid, uint32 Text_Id);
	void AddItem(GossipMenuItem* GossipItem);
	void AddItem(uint8 Icon, const char* Text, int32 Id = -1, int8 Extra = 0);
	void AddMenuItem(uint8 Icon, std::string Message, uint32 dtSender, uint32 dtAction, std::string BoxMessage, uint32 BoxMoney, bool Coded = false);
	void BuildPacket(WorldPacket& Packet);
	void SendTo(Player* Plr);
	void SendGossipMenu( uint32 TitleTextId, uint64 npcGUID );
	GossipMenuItem GetItem(uint32 Id);
	ARCEMU_INLINE void SetTextID(uint32 TID) { TextId = TID; }

protected:
	uint32 TextId;
	uint64 CreatureGuid;
	std::vector<GossipMenuItem> Menu;
};

class Charter
{
public:
	uint32 GetNumberOfSlotsByType()
	{
		switch(CharterType)
		{
		case CHARTER_TYPE_GUILD:
			return 9;

		case CHARTER_TYPE_ARENA_2V2:
			return 1;

		case CHARTER_TYPE_ARENA_3V3:
			return 2;

		case CHARTER_TYPE_ARENA_5V5:
			return 4;

		default:
			return 9;
		}
	}

	uint32 SignatureCount;
	uint32 * Signatures;
	uint32 CharterType;
	uint32 Slots;
	uint32 LeaderGuid;
	uint64 ItemGuid;
	uint32 CharterId;
	string GuildName;

	Charter(Field * fields);
	Charter(uint32 id, uint32 leader, uint32 type) : CharterType(type), LeaderGuid(leader), CharterId(id)
	{
		SignatureCount = 0;
		ItemGuid = 0;
		Slots = GetNumberOfSlotsByType();
		Signatures = new uint32[Slots];
		memset(Signatures, 0, sizeof(uint32)*Slots);
	}

	~Charter()
	{
		delete [] Signatures;
	}
	
	void SaveToDB();
	void Destroy();		 // When item is deleted.

	void AddSignature(uint32 PlayerGuid);
	void RemoveSignature(uint32 PlayerGuid);

	ARCEMU_INLINE uint32 GetLeader() { return LeaderGuid; }
	ARCEMU_INLINE uint32 GetID() { return CharterId; }

	ARCEMU_INLINE bool IsFull() { return (SignatureCount == Slots); }
};

typedef std::map<uint32, std::list<SpellEntry*>* >                  OverrideIdMap;
typedef HM_NAMESPACE::hash_map<uint32, Player*>                     PlayerStorageMap;
typedef std::list<GM_Ticket*>                                       GmTicketList;
typedef std::map<uint32, InstanceBossInfo*>                         InstanceBossInfoMap;

#ifndef WIN32
#define arcemu_USE_MAP_PLAYER_INDEX
#ifdef arcemu_USE_MAP_PLAYER_INDEX

// you can use the string map (slower)
typedef map<string, PlayerInfo*> PlayerNameStringIndexMap;


#else			// or

// gcc has no default hash for string type,
// so we have to make an explicit hash template here
template<>
struct __gnu_cxx::hash<string>
{
	size_t operator()(string& tbh) const
	{
		// simple crc32 hash for now, we may need to change this later however
		return size_t( crc32( (const unsigned char*)tbh.c_str(), tbh.length() ) );
	}
}

typedef HM_NAMESPACE::hash_map<string, PlayerInfo*> PlayerNameStringIndexMap;

#endif
#else

// vc++ has the type for a string hash already, so we don't need to do anything special
typedef HM_NAMESPACE::hash_map<string, PlayerInfo*> PlayerNameStringIndexMap;

#endif

class SERVER_DECL ObjectMgr : public Singleton < ObjectMgr >, public EventableObject
{
public:
	ObjectMgr();
	~ObjectMgr();
	void LoadCreatureWaypoints();

	void LoadCreatureTimedEmotes();
	TimedEmoteList * GetTimedEmoteList(uint32 spawnid);

	// other objects
    
    // Set typedef's
	typedef HM_NAMESPACE::hash_map<uint32, Group*>						GroupMap;
	
    // HashMap typedef's
    typedef HM_NAMESPACE::hash_map<uint64, Item*>                       ItemMap;
	typedef HM_NAMESPACE::hash_map<uint32, CorpseData*>                 CorpseCollectorMap;
	typedef HM_NAMESPACE::hash_map<uint32, PlayerInfo*>                 PlayerNameMap;
	typedef HM_NAMESPACE::hash_map<uint32, PlayerCreateInfo*>           PlayerCreateInfoMap;
	typedef HM_NAMESPACE::hash_map<uint32, Guild*>                      GuildMap;
	typedef HM_NAMESPACE::hash_map<uint32, skilllinespell*>             SLMap;
	typedef HM_NAMESPACE::hash_map<uint32, std::vector<CreatureItem>*>  VendorMap;
    typedef HM_NAMESPACE::hash_map<uint32, Transporter*>                TransportMap;
	typedef HM_NAMESPACE::hash_map<uint32, Trainer*>                    TrainerMap;
	typedef HM_NAMESPACE::hash_map<uint32, std::vector<TrainerSpell*> > TrainerSpellMap;
    typedef HM_NAMESPACE::hash_map<uint32, ReputationModifier*>         ReputationModMap;
    typedef HM_NAMESPACE::hash_map<uint32, Corpse*>                     CorpseMap;
    
    // Map typedef's
    typedef std::map<uint32, LevelInfo*>                                LevelMap;
	typedef std::map<pair<uint32, uint32>, LevelMap* >                  LevelInfoMap;
    typedef std::map<uint32, std::list<ItemPrototype*>* >               ItemSetContentMap;
	typedef std::map<uint32, uint32>                                    NpcToGossipTextMap;
	typedef std::map<uint32, set<SpellEntry*> >                         PetDefaultSpellMap;
	typedef std::map<uint32, uint32>                                    PetSpellCooldownMap;
	typedef std::map<uint32, SpellEntry*>                               TotemSpellMap;
	typedef std::multimap <uint32,uint32>                               BCEntryStorage;

    // object holders
	GmTicketList         GM_TicketList;
	TotemSpellMap        m_totemSpells;
	OverrideIdMap        mOverrideIdMap;
	InstanceBossInfoMap* m_InstanceBossInfoMap[NUM_MAPS];

	Player* GetPlayer(const char* name, bool caseSensitive = true);
	Player* GetPlayer(uint32 guid);
	
	CorpseMap m_corpses;
	Mutex _corpseslock;
	Mutex m_corpseguidlock;
    Mutex _TransportLock;
	uint32 m_hiCorpseGuid;
	uint32 m_hiGuildId;
	
	Item * CreateItem(uint32 entry,Player * owner);
	Item * LoadItem(uint64 guid);
	Item * LoadExternalItem(uint64 guid);
  
	// Groups
	Group * GetGroupByLeader(Player *pPlayer);
	Group * GetGroupById(uint32 id);
	uint32 GenerateGroupId()
	{
		uint32 r;
		m_guidGenMutex.Acquire();
		r = ++m_hiGroupId;
		m_guidGenMutex.Release();
		return r;
	}

	uint32 GenerateGuildId()
	{
		uint32 r;
		m_guidGenMutex.Acquire();
		r = ++m_hiGuildId;
		m_guidGenMutex.Release();
		return r;
	}

	void AddGroup(Group* group)
	{
		m_groupLock.AcquireWriteLock();
		m_groups.insert(make_pair(group->GetID(), group));
		m_groupLock.ReleaseWriteLock();
	}

	void RemoveGroup(Group* group)
	{
		m_groupLock.AcquireWriteLock();
		m_groups.erase(group->GetID());
		m_groupLock.ReleaseWriteLock();
	}

	void GroupVoiceReconnected();

	void LoadGroups();

	// player names
	void AddPlayerInfo(PlayerInfo *pn);
	PlayerInfo *GetPlayerInfo(uint32 guid );
	PlayerInfo *GetPlayerInfoByName(const char * name);
	void RenamePlayerInfo(PlayerInfo * pn, const char * oldname, const char * newname);
	void DeletePlayerInfo(uint32 guid);
	PlayerCreateInfo* GetPlayerCreateInfo(uint8 race, uint8 class_) const;

	// Guild
	void AddGuild(Guild *pGuild);
	uint32 GetTotalGuildCount();
	bool RemoveGuild(uint32 guildId);
	Guild* GetGuild(uint32 guildId);  
	Guild* GetGuildByLeaderGuid(uint64 leaderGuid);  
	Guild* GetGuildByGuildName(std::string guildName);

	//Corpse Stuff
	Corpse *GetCorpseByOwner(uint32 ownerguid);
	void CorpseCollectorUnload();
	void DespawnCorpse(uint64 Guid);
	void CorpseAddEventDespawn(Corpse *pCorpse);
	void DelinkPlayerCorpses(Player *pOwner);
	Corpse * CreateCorpse();
	void AddCorpse(Corpse*);
	void RemoveCorpse(Corpse*);
	Corpse * GetCorpse(uint32 corpseguid);

	uint32 GetGossipTextForNpc(uint32 ID);

	// Gm Tickets
	void AddGMTicket(GM_Ticket *ticket, bool startup = false);
	void UpdateGMTicket(GM_Ticket *ticket);
	void RemoveGMTicketByPlayer(uint64 playerGuid);
	void RemoveGMTicket(uint64 ticketGuid);
	void DeleteGMTicketPermanently(uint64 ticketGuid);
	void DeleteAllRemovedGMTickets();
	GM_Ticket* GetGMTicket(uint64 ticketGuid);
	GM_Ticket* GetGMTicketByPlayer(uint64 playerGuid);
	//std::list<GM_Ticket*>* GetGMTicketsByPlayer(uint64 playerGuid);

	skilllinespell* GetSpellSkill(uint32 id);

	//Vendors
	std::vector<CreatureItem> *GetVendorList(uint32 entry);
	void SetVendorList(uint32 Entry, std::vector<CreatureItem>* list_);

	//Totem
	SpellEntry* GetTotemSpell(uint32 spellId);

	std::list<ItemPrototype*>* GetListForItemSet(uint32 setid);

	Pet * CreatePet();
	uint32 m_hiPetGuid;
	uint32 m_hiArenaTeamId;
	uint32 GenerateArenaTeamId()
	{
		uint32 ret;
		m_arenaTeamLock.Acquire();
		ret = ++m_hiArenaTeamId;
		m_arenaTeamLock.Release();
		return ret;
	}

	Mutex m_petlock;

	Player * CreatePlayer();
	 Mutex m_playerguidlock;
	PlayerStorageMap _players;
	RWLock _playerslock;
	uint32 m_hiPlayerGuid;
	
	void AddPlayer(Player * p);//add it to global storage
	void RemovePlayer(Player *p);


	// Serialization

	void LoadQuests();
	void LoadPlayersInfo();
	void LoadPlayerCreateInfo();
	void LoadGuilds();
	Corpse* LoadCorpse(uint32 guid);
	void LoadCorpses(MapMgr * mgr);
	void LoadGMTickets();
	void SaveGMTicket(GM_Ticket* ticket, QueryBuffer * buf);
	void LoadInstanceBossInfos();
	void LoadAuctions();
	void LoadAuctionItems();
	void LoadSpellSkills();
	void LoadVendors();
	void ReloadVendors();
	void LoadTotemSpells();
	void LoadAIThreatToSpellId();
	void LoadSpellFixes();
	void LoadSpellProcs();
	void LoadSpellEffectsOverride();
	void LoadReputationModifierTable(const char * tablename, ReputationModMap * dmap);
	void LoadReputationModifiers();
	ReputationModifier * GetReputationModifier(uint32 entry_id, uint32 faction_id);

	void SetHighestGuids();
	uint32 GenerateLowGuid(uint32 guidhigh);
	uint32 GenerateMailID();
	uint64 GenerateTicketID();
	
	void LoadTransporters();
	void ProcessGameobjectQuests();
    void AddTransport(Transporter * pTransporter);
   
	void LoadTrainers();
	Trainer* GetTrainer(uint32 Entry);

	void LoadExtraItemStuff();
	void LoadExtraCreatureProtoStuff();
	void LoadProfessionDiscoveries();

	void StoreBroadCastGroupKey();

	void CreateGossipMenuForPlayer(GossipMenu** Location, uint64 Guid, uint32 TextID, Player* Plr); 

	LevelInfo * GetLevelInfo(uint32 Race, uint32 Class, uint32 Level);
	void GenerateLevelUpInfo();
	void LoadDefaultPetSpells();
	set<SpellEntry*>* GetDefaultPetSpells(uint32 Entry);
	uint32 GetPetSpellCooldown(uint32 SpellId);
	void LoadPetSpellCooldowns();
	WayPointMap * GetWayPointMap(uint32 spawnid);
	void LoadSpellOverride();

	void ResetDailies();

	uint32 GenerateCreatureSpawnID()
	{
		m_CreatureSpawnIdMutex.Acquire();
		uint32 r = ++m_hiCreatureSpawnId;
		m_CreatureSpawnIdMutex.Release();
		return r;
	}

	uint32 GenerateGameObjectSpawnID()
	{
		m_GOSpawnIdMutex.Acquire();
		uint32 r = ++m_hiGameObjectSpawnId;
		m_GOSpawnIdMutex.Release();
		return r;
	}

	Transporter * GetTransporter(uint32 guid);
	Transporter * GetTransporterByEntry(uint32 entry);

	Charter * CreateCharter(uint32 LeaderGuid, CharterTypes Type);
	Charter * GetCharter(uint32 CharterId, CharterTypes Type);
	void RemoveCharter(Charter *);
	void LoadGuildCharters();
	Charter * GetCharterByName(string &charter_name, CharterTypes Type);
	Charter * GetCharterByItemGuid(uint64 guid);
	Charter * GetCharterByGuid(uint64 playerguid, CharterTypes type);

	ArenaTeam * GetArenaTeamByName(string & name, uint32 Type);
	ArenaTeam * GetArenaTeamById(uint32 id);
	ArenaTeam * GetArenaTeamByGuid(uint32 guid, uint32 Type);
	void UpdateArenaTeamRankings();
	void UpdateArenaTeamWeekly();
	void ResetArenaTeamRatings();
	void LoadArenaTeams();
	HM_NAMESPACE::hash_map<uint32, ArenaTeam*> m_arenaTeamMap[3];
	HM_NAMESPACE::hash_map<uint32, ArenaTeam*> m_arenaTeams;
	void RemoveArenaTeam(ArenaTeam * team);
	void AddArenaTeam(ArenaTeam * team);
	Mutex m_arenaTeamLock;

	typedef HM_NAMESPACE::hash_map<uint32, NpcMonsterSay*> MonsterSayMap;
	MonsterSayMap mMonsterSays[NUM_MONSTER_SAY_EVENTS];

	void HandleMonsterSayEvent(Creature * pCreature, MONSTER_SAY_EVENTS Event);
	bool HasMonsterSay(uint32 Entry, MONSTER_SAY_EVENTS Event);
	void LoadMonsterSay();

	bool HandleInstanceReputationModifiers(Player * pPlayer, Unit * pVictim);
	void LoadInstanceReputationModifiers();

	ARCEMU_INLINE bool IsSpellDisabled(uint32 spellid)
	{
		if(m_disabled_spells.find(spellid) != m_disabled_spells.end())
			return true;
		return false;
	}

	void LoadDisabledSpells();
	void ReloadDisabledSpells();
	ARCEMU_INLINE GuildMap::iterator GetGuildsBegin() { return mGuild.begin(); }
	ARCEMU_INLINE GuildMap::iterator GetGuildsEnd() { return mGuild.end(); }

	std::set<ProfessionDiscovery*> ProfessionDiscoveryTable;

	// cebernic: This is an perfect Broadcast system,multi-lang supported also.
	ARCEMU_INLINE uint32 GetBCGroupCountByKey(uint32 Key) { return (uint32)m_BCEntryStorage.count(Key); }
	ARCEMU_INLINE bool IsBCEntryStorageEmpty() { return m_BCEntryStorage.empty(); }
	ARCEMU_INLINE BCEntryStorage::iterator GetBCTotalItemBegin() { return m_BCEntryStorage.begin(); }
	ARCEMU_INLINE BCEntryStorage::iterator GetBCTotalItemEnd() { return m_BCEntryStorage.end(); }
	ARCEMU_INLINE int CalcCurrentBCEntry() 
	// func sync at MAKE_TASK(ObjectMgr, StoreBroadCastGroupKey)[world.cpp]
	{
		if ( m_BCEntryStorage.empty() ) return -1;
	  uint32 RandomCap = (uint32)sWorld.BCTriggerPercentCap;
		
		vector<uint32> Entries;
		BCEntryStorage::iterator it = m_BCEntryStorage.upper_bound( RandomUInt(RandomCap)+1 );
		while( it!=m_BCEntryStorage.end() )
		{
			Entries.push_back(it->second);
			++it;
		}
		if ( Entries.empty() ) return 0;
		uint32 n = (Entries.size()>1 ? RandomUInt(  ((uint32)Entries.size())-1  ) : 0);
		return Entries[n];
	}


protected:
	BCEntryStorage m_BCEntryStorage; // broadcast system.
	RWLock playernamelock;
	uint32 m_mailid;
	uint64 m_ticketid;
	// highest GUIDs, used for creating new objects
	Mutex m_guidGenMutex;
    union
    {
	    uint32 m_hiItemGuid;
	    uint32 m_hiContainerGuid;
    };
	uint32 m_hiGroupId;
	uint32 m_hiCharterId;
	RWLock m_charterLock;

	ReputationModMap m_reputation_faction;
	ReputationModMap m_reputation_creature;
	HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*> m_reputation_instance;

	HM_NAMESPACE::hash_map<uint32, Charter*> m_charters[NUM_CHARTER_TYPES];
	
	set<uint32> m_disabled_spells;

	uint64 TransportersCount;
	HM_NAMESPACE::hash_map<uint32,PlayerInfo*> m_playersinfo;
	PlayerNameStringIndexMap m_playersInfoByName;
	
	HM_NAMESPACE::hash_map<uint32,WayPointMap*> m_waypoints;//stored by spawnid
	HM_NAMESPACE::hash_map<uint32,TimedEmoteList*> m_timedemotes;//stored by spawnid
	uint32 m_hiCreatureSpawnId;
	
	Mutex m_CreatureSpawnIdMutex;
	Mutex m_GOSpawnIdMutex;

	uint32 m_hiGameObjectSpawnId;
	
	///// Object Tables ////
	// These tables are modified as creatures are created and destroyed in the world

	// Group List
	RWLock m_groupLock;
	GroupMap m_groups;

	// Map of all starting infos needed for player creation
	PlayerCreateInfoMap mPlayerCreateInfo;

	// DK: Map of all Guild's
	GuildMap mGuild;

	// Map of all vendor goods
	VendorMap mVendors;

	// Maps for Gossip stuff
	NpcToGossipTextMap  mNpcToGossipText;

	SLMap				mSpellSkills;

	//Corpse Collector
	CorpseCollectorMap mCorpseCollector;

    TransportMap mTransports;

	ItemSetContentMap mItemSets;

	TrainerMap mTrainers;
	LevelInfoMap mLevelInfo;
	PetDefaultSpellMap mDefaultPetSpells;
	PetSpellCooldownMap mPetSpellCooldowns;
};


#define objmgr ObjectMgr::getSingleton()

//void SetProgressBar(int, int, const char*);
//void ClearProgressBar();

#endif

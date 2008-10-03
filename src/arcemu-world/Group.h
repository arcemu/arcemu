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

#ifndef _GROUP_H_
#define _GROUP_H_

enum PartyErrors
{
	ERR_PARTY_NO_ERROR					= 0,
	ERR_PARTY_CANNOT_FIND				= 1,
	ERR_PARTY_IS_NOT_IN_YOUR_PARTY		= 2,
	ERR_PARTY_UNK						= 3,
	ERR_PARTY_IS_FULL					= 4,
	ERR_PARTY_ALREADY_IN_GROUP			= 5,
	ERR_PARTY_YOU_ARENT_IN_A_PARTY		= 6,
	ERR_PARTY_YOU_ARE_NOT_LEADER		= 7,
	ERR_PARTY_WRONG_FACTION				= 8,
	ERR_PARTY_IS_IGNORING_YOU			= 9,
};

enum GroupTypes
{
	GROUP_TYPE_PARTY					= 0,
	GROUP_TYPE_RAID						= 1,
};

enum MaxGroupCount
{
	MAX_GROUP_SIZE_PARTY				= 5,
	MAX_GROUP_SIZE_RAID					= 40,
};

enum QuickGroupUpdateFlags
{
	PARTY_UPDATE_FLAG_POSITION			= 1,
	PARTY_UPDATE_FLAG_ZONEID			= 2,
};

struct PlayerInfo;
typedef struct
{
	PlayerInfo * player_info;
	Player * player;
}GroupMember;

class Group;
class Player;

typedef std::set<PlayerInfo*> GroupMembersSet;

class SERVER_DECL SubGroup	  // Most stuff will be done through here, not through the "Group" class.
{
public:
	friend class Group;

	SubGroup(Group* parent, uint32 id):m_Parent(parent),m_Id(id)
	{
	}

	~SubGroup();

	ARCEMU_INLINE GroupMembersSet::iterator GetGroupMembersBegin(void) { return m_GroupMembers.begin(); }
	ARCEMU_INLINE GroupMembersSet::iterator GetGroupMembersEnd(void)   { return m_GroupMembers.end();   }

	bool AddPlayer(PlayerInfo * info);
	void RemovePlayer(PlayerInfo * info);
	
	ARCEMU_INLINE bool IsFull(void)				{ return m_GroupMembers.size() >= MAX_GROUP_SIZE_PARTY; }
	ARCEMU_INLINE size_t GetMemberCount(void)		{ return m_GroupMembers.size(); }
	
	ARCEMU_INLINE uint32 GetID(void)			   { return m_Id; }
	ARCEMU_INLINE void SetID(uint32 newid)		 { m_Id = newid; }

	ARCEMU_INLINE void   SetParent(Group* parent)  { m_Parent = parent; }
	ARCEMU_INLINE Group* GetParent(void)		   { return m_Parent; }

	void   Disband();
	bool HasMember(uint32 guid);

protected:

	GroupMembersSet	 m_GroupMembers;
	Group*			  m_Parent;
	uint32			  m_Id;

};

class Arena;
class SERVER_DECL Group
{
public:
	friend class SubGroup;

	static Group* Create();

	Group(bool Assign);
	~Group();

	// Adding/Removal Management
	bool AddMember(PlayerInfo * info, int32 subgroupid=-1);
	void RemovePlayer(PlayerInfo * info);

	// Leaders and Looting
	void SetLeader(Player* pPlayer,bool silent);
	void SetLooter(Player *pPlayer, uint8 method, uint16 threshold);

	// Transferring data to clients
	void Update();

	ARCEMU_INLINE void SendPacketToAll(WorldPacket *packet) { SendPacketToAllButOne(packet, NULL); }
	void SendPacketToAllButOne(WorldPacket *packet, Player *pSkipTarget);

	ARCEMU_INLINE void OutPacketToAll(uint16 op, uint16 len, const void* data) { OutPacketToAllButOne(op, len, data, NULL); }
	void OutPacketToAllButOne(uint16 op, uint16 len, const void* data, Player *pSkipTarget);

	void SendNullUpdate(Player *pPlayer);

	// Group Combat
	void SendPartyKillLog(Object * player, Object * Unit);

	// Destroying/Converting
	void Disband();
	Player* FindFirstPlayer();
	
	// Accessing functions
	ARCEMU_INLINE SubGroup* GetSubGroup(uint32 Id)
	{
		if(Id >= 8)
			return 0;

		return m_SubGroups[Id];
	}

	ARCEMU_INLINE uint32 GetSubGroupCount(void) { return m_SubGroupCount; }

	ARCEMU_INLINE uint8 GetMethod(void) { return m_LootMethod; }
	ARCEMU_INLINE uint16 GetThreshold(void) { return m_LootThreshold; }
	ARCEMU_INLINE PlayerInfo* GetLeader(void) { return m_Leader; }
	ARCEMU_INLINE PlayerInfo* GetLooter(void) { return m_Looter; }

	void MovePlayer(PlayerInfo* info, uint8 subgroup);

	bool HasMember(Player *pPlayer);
	bool HasMember(PlayerInfo * info);
	ARCEMU_INLINE uint32 MemberCount(void) { return m_MemberCount; }
	ARCEMU_INLINE bool IsFull() { return ((m_GroupType == GROUP_TYPE_PARTY && m_MemberCount >= MAX_GROUP_SIZE_PARTY) || (m_GroupType == GROUP_TYPE_RAID && m_MemberCount >= MAX_GROUP_SIZE_RAID)); }

	SubGroup* FindFreeSubGroup();

	void ExpandToRaid();

	void SaveToDB();
	void LoadFromDB(Field *fields);

	ARCEMU_INLINE uint8 GetGroupType() { return m_GroupType; }
	ARCEMU_INLINE uint32 GetID() { return m_Id; }

	void UpdateOutOfRangePlayer(Player * pPlayer, uint32 Flags, bool Distribute, WorldPacket * Packet);
	void UpdateAllOutOfRangePlayersFor(Player * pPlayer);
	void HandleUpdateFieldChange(uint32 Index, Player * pPlayer);
	void HandlePartialChange(uint32 Type, Player * pPlayer);

	uint64 m_targetIcons[8];
	bool m_disbandOnNoMembers;
	ARCEMU_INLINE Mutex& getLock() { return m_groupLock; }
	ARCEMU_INLINE void Lock() { m_groupLock.Acquire(); }
	ARCEMU_INLINE void Unlock() { return m_groupLock.Release(); }
	bool m_isqueued;

	void SetAssistantLeader(PlayerInfo * pMember);
	void SetMainTank(PlayerInfo * pMember);
	void SetMainAssist(PlayerInfo * pMember);

	ARCEMU_INLINE PlayerInfo * GetAssistantLeader() { return m_assistantLeader; }
	ARCEMU_INLINE PlayerInfo * GetMainTank() { return m_mainTank; }
	ARCEMU_INLINE PlayerInfo * GetMainAssist() { return m_mainAssist; }

	uint32 m_instanceIds[NUM_MAPS][NUM_INSTANCE_MODES];

	void SetDifficulty(uint8 difficulty);
	
	/************************************************************************/
	/* Voicechat                                                            */
	/************************************************************************/
#ifdef VOICE_CHAT
	void AddVoiceMember(PlayerInfo * pPlayer);
	void RemoveVoiceMember(PlayerInfo * pPlayer);
	void SendVoiceUpdate();
	void CreateVoiceSession();
	void VoiceChannelCreated(uint16 id);
	void VoiceSessionDropped();
	void VoiceSessionReconnected();

public:
	bool m_voiceChannelRequested;
	int16 m_voiceChannelId;
	uint32 m_voiceMemberCount;
protected:
	PlayerInfo* m_voiceMembersList[41];
#endif	// VOICE_CHAT

protected:
	PlayerInfo * m_Leader;
	PlayerInfo * m_Looter;
	PlayerInfo * m_assistantLeader;
	PlayerInfo * m_mainTank;
	PlayerInfo * m_mainAssist;

	uint8 m_LootMethod;
	uint16 m_LootThreshold;
	uint8 m_GroupType;
	uint32 m_Id;

	SubGroup* m_SubGroups[8];
	uint8 m_SubGroupCount;
	uint32 m_MemberCount;
	Mutex m_groupLock;
	bool m_dirty;
	bool m_updateblock;
public:
	uint8 m_difficulty;
};

#endif  // _GROUP_H_

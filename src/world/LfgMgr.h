/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

enum LFGRoles
{
	LFG_ROLE_LEADER = 1,
	LFG_ROLE_TANK = 2,
	LFG_ROLE_HEALER = 4,
	LFG_ROLE_DPS = 8
};

enum LFGUpdateType
{
	LFG_UPDATE_JOINED = 5,
	LFG_UPDATE_REMOVED = 7,
	LFG_UPDATE_PROPOSAL_FAILURE = 8,
	LFG_UPDATE_GROUP_FOUND = 10,
	LFG_UPDATE_PROPOSAL_BEGIN = 13
};

class LFGProposalEntry
{
public:
	uint32 guid;
	uint32 team;
	uint32 selectedRoles;
	uint32 answered;
	uint32 accepted;

	LFGProposalEntry()
	{
		guid = 0;
		team = 0;
		selectedRoles = 0;
		answered = 0;
		accepted = 0;
	}
};

class LFGProposal
{
public:
	enum LFGProposalState
	{
		LFG_PROPOSAL_STATE_DEFAULT = 0,
		LFG_PROPOSAL_STATE_FAIL = 1,
		LFG_PROPOSAL_STATE_SUCCESS = 2
	};

	uint8 state;
	uint32 id;
	uint32 dungeon;
	std::vector< LFGProposalEntry > players;
	uint32 expiryTime;

	LFGProposal()
	{
		state = LFG_PROPOSAL_STATE_DEFAULT;
		id = 0;
		dungeon = 0;
		expiryTime = 0;
	}

	/// Remove a player from the list of players
	void removePlayer( uint32 guid );

	/// Updates the proposal answer for a guid. Returns true on successful update.
	bool updateProposalAnswer( uint32 guid, uint8 answer );

	/// Tells if everyone has accepted
	bool hasAllAccepted() const;
};

/// Generates proposal Id and maintains proposals
class LFGProposalStore
{
public:
	LFGProposalStore();
	~LFGProposalStore();

	/// Generate an Id for and add proposal to the store
	uint32 addProposal( LFGProposal *proposal );

	/// Look up and return proposal, or NULL
	LFGProposal* getProposal( uint32 id );

	/// Look up and return proposal in which this player is in, or NULL
	LFGProposal* getProposalForPlayer( uint32 guid ) const;

	/// Remove proposal from the store
	void removeProposal( uint32 id );

	/// Return expired proposals
	void getExpiredProposals( std::vector< LFGProposal* > &expiredProposals );

private:
	/// The last generated proposal Id
	uint32 lastId;

	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGProposal* > proposals;
};

class LFGQueueGroupRequirements
{
public:
	bool needLeader;
	uint8 tanksNeeded;
	uint8 healersNeeded;
	uint8 dpsNeeded;

	LFGQueueGroupRequirements()
	{
		needLeader = true;
		tanksNeeded = 1;
		healersNeeded = 1;
		dpsNeeded = 3;
	}

	bool met() const
	{
		if( needLeader )
			return false;

		if( tanksNeeded > 0 )
			return false;

		if( healersNeeded > 0 )
			return false;

		if( dpsNeeded > 0 )
			return false;

		return true;
	}
};

class LFGPlayer
{
public:
	uint32 guid;
	uint32 roles;
	uint32 jointime;
};

class LFGQueueEntry
{
public:
	uint32 team;
	std::vector< LFGPlayer > players;
	LFGQueueGroupRequirements requirements;

	/// Try to add the player to this group, return false on failure.
	bool tryAddPlayer( uint32 guid, uint32 roles );

	/// Try to remove player from this group, return false on failure
	bool tryRemovePlayer( uint32 guid );

	/// Is this group ready to be deployed?
	bool groupComplete() const { return requirements.met(); }
};

class LFGQueue
{
public:
	LFGQueue( uint32 dungeon, const LFGQueueGroupRequirements &requirements );
	~LFGQueue();

	/// Add player to queue
	void addPlayer( uint32 guid, uint32 team, uint32 roles );

	/// Readd proposed group to the front of the queue
	void readdGroup( LFGProposal *proposal );

	/// Remove player from queue
	void removePlayer( uint32 guid );

	/// Find next match
	LFGProposal* findMatch( uint32 team, bool force );

	void updateQueueStatus();

private:
	std::deque< LFGQueueEntry > queue;

	const LFGQueueGroupRequirements groupRequirements;

	uint32 dungeon;
};

class LFGInstance
{
public:
	uint32 map;
	uint32 instance;
	LocationVector entrance;
};

class LfgMgr : public Singleton < LfgMgr >, public EventableObject
{
public:
	enum LFGMgrConstants
	{
		LFGMGR_MAX_DUNGEONS = 294,
		LFGMGR_QUEUE_COUNT = LFGMGR_MAX_DUNGEONS + 1
	};

	enum LFGDungeonTypes
	{
		LFG_DUNGEON_TYPE_NORMAL = 1,
		LFG_DUNGEON_TYPE_RAID = 2,
		LFG_DUNGEON_TYPE_HEROIC = 5,
		LFG_DUNGEON_TYPE_RANDOM = 6
	};

public:
	LfgMgr();
	~LfgMgr();
	
	/// Adds the player to the LFG system
	void addPlayer( uint32 guid, uint32 roles, std::vector< uint32 > dungeons );

	/// Removes the player from the LFG system
	void removePlayer( uint32 guid );

	/// Updates the specified proposal with the player's answer
	void updateProposal( uint32 id, uint32 guid, uint8 result );

	/// Teleports a player to their dungeon
	void teleportPlayer( uint32 guid, bool out = false );

	/// Remove the group from the LFG system
	void removeGroup( uint32 groupId );

	/// Remove the group - intance association that belongs to this instance
	void removeInstance( uint32 instanceId );

	/// Updates the LFG system.
	void update( bool force );

	/// EventMgr update method
	void eventUpdate(){ update( false ); }

private:
	/// Teleports the player to their dungeon. Not protected by the lock!
	void teleportPlayerInternal( uint32 guid, bool out = false );

	/// Adds player to queues. Not protected by the lock!
	void addPlayerInternal( uint32 guid, uint32 roles, std::vector< uint32 > dungeons, bool readd );

	/// Removes player from the queues. Not protected by the lock!
	void removePlayerInternal( uint32 guid );

	/// Readd proposed group to the front of the queue
	void readdGroup( LFGProposal *proposal );

	/// Sends the proposal to players
	void sendProposal( LFGProposal *proposal );

	/// Sends the proposal to a specific player
	void sendProposalToPlayer( uint32 guid, LFGProposal *proposal );

	/// We have a valid proposal, handle it!
	void onGroupFound( LFGProposal *proposal );

	/// Propsal failure handler
	void onProposalFailed( LFGProposal *proposal );

	/// Proposal success
	void onProposalSuccess( LFGProposal *proposal );

	/// Pick a random dungeon that is appropriate for the group
	uint32 pickRandomDungeonForProposal( LFGProposal *proposal );

	Mutex lock;

	LFGQueue* queues[ LFGMGR_QUEUE_COUNT ];

	HM_NAMESPACE::HM_HASH_MAP< uint32, std::vector< uint32 > > playerToDungeons;

	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGInstance > groupToInstance;
	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 > instanceToGroup;

	LFGProposalStore proposals;
};

#define sLfgMgr LfgMgr::getSingleton()

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
	uint32 id;
	uint32 dungeon;
	std::vector< LFGProposalEntry > players;
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

class LFGQueueEntry
{
public:
	uint32 guid;
	uint32 roles;
	uint32 team;
};

class LFGQueue
{
public:
	LFGQueue( const LFGQueueGroupRequirements &requirements );
	~LFGQueue();

	/// Add player to queue
	void addPlayer( uint32 guid, uint32 team, uint32 roles );

	/// Remove player from queue
	void removePlayer( uint32 guid );

	/// Find next match
	LFGProposal* findMatch( uint32 team, bool force );

private:
	std::deque< LFGQueueEntry > queue;

	const LFGQueueGroupRequirements groupRequirements;
};

class LfgMgr : public Singleton < LfgMgr >, EventableObject
{
public:
	enum LFGMgrConstants
	{
		LFGMGR_MAX_DUNGEONS = 294,
		LFGMGR_QUEUE_COUNT = LFGMGR_MAX_DUNGEONS + 1
	};

	enum LFGDungeonTypes
	{
		LFG_DUNGEON_TYPE_RAID = 2,
		LFG_DUNGEON_TYPE_RANDOM = 6
	};

public:
	LfgMgr();
	~LfgMgr();
	
	/// Adds the player to the LFG system
	void addPlayer( uint32 guid, uint32 roles, std::vector< uint32 > dungeons );

	/// Removes the player from the LFG system
	void removePlayer( uint32 guid );

	/// Updates the LFG system.
	void update( bool force );

private:
	/// Removes player from the queues, not protected by the lock
	void removePlayerInternal( uint32 guid );

	/// Sends the proposal to players
	void sendProposal( LFGProposal *proposal );

	/// Sends the proposal to a specific player
	void sendProposalToPlayer( uint32 guid, LFGProposal *proposal );

	Mutex lock;

	LFGQueue* queues[ LFGMGR_QUEUE_COUNT ];

	HM_NAMESPACE::HM_HASH_MAP< uint32, std::vector< uint32 > > playerToDungeons;
};

#define sLfgMgr LfgMgr::getSingleton()

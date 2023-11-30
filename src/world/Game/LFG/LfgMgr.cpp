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

#include "StdAfx.h"

#define LFG_PROPOSAL_DURATION_SECONDS 20

void LFGProposal::removePlayer( uint32 guid )
{
	std::vector< LFGProposalEntry >::iterator itr = players.begin();
	while( itr != players.end() )
	{
		LFGProposalEntry &entry = *itr;
		if( entry.guid == guid )
		{
			break;
		}

		++itr;
	}

	if( itr != players.end() )
	{
		players.erase( itr );
	}
}

bool LFGProposal::updateProposalAnswer( uint32 guid, uint8 answer )
{
	std::vector< LFGProposalEntry >::iterator itr = players.begin();
	while( itr != players.end() )
	{
		LFGProposalEntry &entry = *itr;

		if( entry.guid == guid )
		{
			entry.answered = 1;
			entry.accepted = answer;
			return true;
		}

		++itr;
	}

	return false;
}

bool LFGProposal::hasAllAccepted() const
{
	std::vector< LFGProposalEntry >::const_iterator itr = players.begin();
	while( itr != players.end() )
	{
		if( itr->accepted == 0 )
			return false;
		++itr;
	}

	return true;
}

LFGProposalStore::LFGProposalStore()
{
	lastId = 0;
}

LFGProposalStore::~LFGProposalStore()
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGProposal* >::iterator itr = proposals.begin();
	while( itr != proposals.end() )
	{
		LFGProposal *proposal = itr->second;
		delete proposal;
		++itr;
	}

	proposals.clear();
}

uint32 LFGProposalStore::addProposal( LFGProposal *proposal )
{
	uint32 id = ++lastId;
	proposals[ id ] = proposal;
	return id;
}

LFGProposal* LFGProposalStore::getProposal( uint32 id )
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGProposal* >::iterator itr = proposals.find( id );
	if( itr == proposals.end() )
		return NULL;
	else
		return itr->second;
}

LFGProposal* LFGProposalStore::getProposalForPlayer( uint32 guid ) const
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGProposal* >::const_iterator itr = proposals.begin();
	while( itr != proposals.end() )
	{
		LFGProposal *proposal = itr->second;

		std::vector< LFGProposalEntry >::const_iterator playerItr = proposal->players.begin();
		while( playerItr != proposal->players.end() )
		{
			if( playerItr->guid == guid )
			{
				return proposal;
			}

			++playerItr;
		}

		++itr;
	}

	return NULL;
}

void LFGProposalStore::removeProposal( uint32 id )
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGProposal* >::iterator itr = proposals.find( id );
	if( itr == proposals.end() )
		return;
	else
		proposals.erase( itr );
}

void LFGProposalStore::getExpiredProposals( std::vector< LFGProposal* > &expiredProposals )
{
	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGProposal* >::iterator itr = proposals.begin();
	while( itr != proposals.end() )
	{
		if( UNIXTIME >= itr->second->expiryTime )
		{
			expiredProposals.push_back( itr->second );
		}
		++itr;
	}
}

LFGQueue::LFGQueue( uint32 dungeon, const LFGQueueGroupRequirements &requirements ) :
groupRequirements( requirements )
{
	this->dungeon = dungeon;
}

LFGQueue::~LFGQueue()
{
	queue.clear();
}

bool LFGQueueEntry::tryAddPlayer( uint32 guid, uint32 roles )
{
	if( requirements.met() )
		return false;

	LFGQueueGroupRequirements oldRequirements = requirements;

	uint32 selectedRoles = 0;

	/// Can we add the player to the group?
	if( ( requirements.tanksNeeded > 0 ) && ( ( roles & LFG_ROLE_TANK ) != 0 ) )
	{
		selectedRoles |= LFG_ROLE_TANK;
		requirements.tanksNeeded--;
	}
	else
	if( ( requirements.healersNeeded > 0 ) && ( ( roles & LFG_ROLE_HEALER ) != 0 ) )
	{
		selectedRoles |= LFG_ROLE_HEALER;
		requirements.healersNeeded--;
	}
	else
	if( ( requirements.dpsNeeded > 0 ) && ( ( roles & LFG_ROLE_DPS ) != 0 ) )
	{
		selectedRoles |= LFG_ROLE_DPS;
		requirements.dpsNeeded--;
	}

	/// We can add the player!
	if( selectedRoles != 0 )
	{
		/// Do we need a leader?
		if( requirements.needLeader && ( ( roles & LFG_ROLE_LEADER ) != 0 ) )
		{
			selectedRoles |= LFG_ROLE_LEADER;
			requirements.needLeader = false;
		}

		LFGPlayer player;
		player.guid = guid;
		player.roles = selectedRoles;
		player.jointime = UNIXTIME;
		players.push_back( player );
		return true;
	}
	else
	{
		/// Restore requiremenets
		requirements = oldRequirements;
	}

	return false;
}

bool LFGQueueEntry::tryRemovePlayer( uint32 guid )
{
	/// Try to find the guid
	std::vector< LFGPlayer >::iterator itr = players.begin();
	while( itr != players.end() )
	{
		const LFGPlayer &player = *itr;
		if( player.guid == guid )
		{
			break;
		}

		++itr;
	}

	/// Not found :(
	if( itr == players.end() )
	{
		return false;
	}

	/// Found! Remove it!
	uint32 roles = itr->roles;
	players.erase( itr );

	/// Increase the role needs
	if( roles & LFG_ROLE_LEADER )
	{
		requirements.needLeader = true;
	}

	if( ( roles & LFG_ROLE_TANK ) != 0 )
	{
		requirements.tanksNeeded++;
	}
	else
	if( ( roles & LFG_ROLE_HEALER ) != 0 )
	{
		requirements.healersNeeded++;
	}
	else
	if( ( roles & LFG_ROLE_DPS ) != 0 )
	{
		requirements.dpsNeeded++;
	}

	return true;
}

void LFGQueue::addPlayer( uint32 guid, uint32 team, uint32 roles )
{
	/// Try to find a group
	std::deque< LFGQueueEntry >::iterator itr = queue.begin();
	while( itr != queue.end() )
	{
		LFGQueueEntry &entry = *itr;
		if( ( entry.team == team ) && entry.tryAddPlayer( guid, roles ) )
		{
			break;
		}

		++itr;
	}

	/// Couldn't find a group, make a new one!
	if( itr == queue.end() )
	{
		LFGQueueEntry entry;
		entry.team = team;
		entry.tryAddPlayer( guid, roles );
		queue.push_back( entry );
	}
}

void LFGQueue::readdGroup( LFGProposal *proposal )
{
	LFGQueueEntry entry;

	/// When readding, we have at least 1 player
	entry.team = proposal->players[ 0 ].team;
	
	std::vector< LFGProposalEntry >::iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		entry.tryAddPlayer( itr->guid, itr->selectedRoles );
		++itr;
	}

	queue.push_front( entry );
}

void LFGQueue::removePlayer( uint32 guid )
{
	std::deque< LFGQueueEntry >::iterator itr = queue.begin();
	while( itr != queue.end() )
	{
		LFGQueueEntry &entry = *itr;
		if( entry.tryRemovePlayer( guid ) )
		{
			/// Found 'em!
			break;
		}
		++itr;
	}

	/// If the queue entry is now empty, remove it
	if( itr != queue.end() )
	{
		if( itr->players.empty() )
		{
			queue.erase( itr );
		}
	}
}

LFGProposal* LFGQueue::findMatch( uint32 team, bool force )
{
	if( queue.empty() )
		return NULL;

	LFGProposal *proposal = new LFGProposal();

	/// Get the first group with the specified team from the queue
	std::deque< LFGQueueEntry >::iterator itr = queue.begin();
	while( itr != queue.end() && itr->team != team )
	{
		++itr;
	}

	/// No such group :(
	if( itr == queue.end() )
		return NULL;

	const LFGQueueEntry &entry = *itr;

	/// If it is complete, or we're forcing, make a proposal
	if( entry.groupComplete() || force )
	{
		std::vector< LFGPlayer >::const_iterator pitr = entry.players.begin();
		while( pitr != entry.players.end() )
		{
			const LFGPlayer &player = *pitr;
			LFGProposalEntry proposalEntry;
			proposalEntry.guid = player.guid;
			proposalEntry.team = entry.team;
			proposalEntry.selectedRoles = player.roles;
			proposal->players.push_back( proposalEntry );
			pitr++;
		}
	}

	/// We don't have a group to propose
	if( proposal->players.empty() )
	{
		delete proposal;
		proposal = NULL;
	}

	return proposal;
}

void LFGQueue::updateQueueStatus()
{
	Arcemu::GamePackets::LFG::SLFGQueueStatus status;
	status.dungeon = dungeon;
	status.waitTime = 0;
	status.avgWaitTime = 0;
	status.waitTimeTank = 0;
	status.waitTimeDps = 0;
	status.waitTimeHealer = 0;
	status.tanksNeeded = 1;
	status.healersNeeded = 1;
	status.dpsNeeded = 3;

	PacketBuffer buffer;

	std::deque< LFGQueueEntry >::iterator itr = queue.begin();
	while( itr != queue.end() )
	{
		const LFGQueueEntry &entry = *itr;

		status.tanksNeeded = entry.requirements.tanksNeeded;
		status.healersNeeded = entry.requirements.healersNeeded;
		status.dpsNeeded = entry.requirements.dpsNeeded;

		std::vector< LFGPlayer >::const_iterator pitr = entry.players.begin();
		while( pitr != entry.players.end() )
		{
			const LFGPlayer &lfgPlayer = *pitr;

			Player *player = objmgr.GetPlayer( lfgPlayer.guid );
			status.queueTime = UNIXTIME - lfgPlayer.jointime;
			status.serialize( buffer );
			player->SendPacket( &buffer );
			++pitr;
		}

		++itr;
	}
}

initialiseSingleton(LfgMgr);

LfgMgr::LfgMgr()
{
	for( int i = 0; i < LFGMGR_QUEUE_COUNT; i++ )
	{
		queues[ i ] = NULL;
	}

	if( Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		sEventMgr.AddEvent( this, &LfgMgr::eventUpdate, EVENT_LFG_QUEUE_UPDATE, 5 * 1000, 0, 0 );
	}
}

LfgMgr::~LfgMgr()
{
	if( Config.OptionalConfig.GetBoolDefault( "Experimental", "lfg", false ) )
	{
		sEventMgr.RemoveEvents( this );
	}

	playerToDungeons.clear();

	for( int i = 0; i < LFGMGR_QUEUE_COUNT; i++ )
	{
		delete queues[ i ];
		queues[ i ] = NULL;
	}
}

void LfgMgr::addPlayer( uint32 guid, uint32 roles, std::vector< uint32 > dungeons )
{
	Guard guard( lock );

	HM_NAMESPACE::HM_HASH_MAP< uint32, std::vector< uint32 > >::const_iterator itr = playerToDungeons.find( guid );
	if( itr != playerToDungeons.end() )
	{
		LOG_DEBUG( "Tried to add a player who is already in the LFG system" );
		return;
	}

	if( roles == 0 )
	{
		LOG_DEBUG( "Tried to add player without roles." );
		return;
	}

	if( dungeons.empty() )
	{
		LOG_DEBUG( "Tried to add player without dungeons." );
		return;
	}

	addPlayerInternal( guid, roles, dungeons, false );
}

void LfgMgr::readdGroup( LFGProposal *proposal )
{
	uint32 dungeon = proposal->dungeon;
	std::vector< uint32 > dungeons;
	dungeons.push_back( dungeon );

	PacketBuffer buffer;

	Arcemu::GamePackets::LFG::SLFGUpdatePlayer update;
	update.dungeons = dungeons;
	update.queued = 1;
	update.updateType = LFG_UPDATE_PROPOSAL_FAILURE;
	update.unk1 = 0;
	update.unk2 = 0;
	update.comment = "";
	update.serialize( buffer );

	/// Readd the group
	queues[ dungeon ]->readdGroup( proposal );

	/// Notify players
	std::vector< LFGProposalEntry >::iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		Player *player = objmgr.GetPlayer( itr->guid );
		player->SendPacket( &buffer );
		playerToDungeons[ itr->guid ] = dungeons;
		++itr;
	}

	delete proposal;
}

void LfgMgr::addPlayerInternal( uint32 guid, uint32 roles, std::vector< uint32 > dungeons, bool readd )
{
	// Packethandler checks if we're in world, which means player must exist
	Player *player = objmgr.GetPlayer( guid );

	Group *group = player->GetGroup();
	if( group != NULL )
	{
		player->GetSession()->SystemMessage( "LFG group queuing is not supported at this time." );
		return;
	}

	PacketBuffer buffer;
	Arcemu::GamePackets::LFG::SLFGJoinResult result;
	result.state = 0;

	/// Some checks
	for( std::vector< uint32 >::const_iterator itr = dungeons.begin(); itr != dungeons.end(); ++itr )
	{
		uint32 dungeon = *itr;
		const LFGDungeonEntry *entry = dbcLFGDungeon.LookupEntry( dungeon );
		if( entry == NULL )
		{
			LOG_DEBUG( "Tried to add player with dungeon %u that doesn't seem exist", dungeon );
			return;
		}

		const LFGDungeonData *data = objmgr.getLFGDungeonData( dungeon );
		if( data == NULL && ( entry->type != LFG_DUNGEON_TYPE_RANDOM ) )
		{
			LOG_DEBUG( "Tried to add player with dungeon %u that doesn't have teleport data yet.", dungeon );
			player->GetSession()->SystemMessage( "You cannot queue for dungeon %u ( %s ), because there's no teleport data available.", dungeon, entry->name );
			return;
		}

		if( entry->type == LFG_DUNGEON_TYPE_RAID )
		{
			LOG_DEBUG( "Raids are not supported at this time." );
			player->GetSession()->SystemMessage( "Raid Finder is not supported at this time." );
			return;
		}

		if( ( player->getLevel() < entry->minlevel ) || ( player->getLevel() > entry->maxlevel ) )
		{
			LOG_DEBUG( "Player %u tried to join LFG queue %u with incorrect level", guid, dungeon );

			result.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_DO_NOT_MEET_REQUIREMENTS;
			result.serialize( buffer );
			player->SendPacket( &buffer );
			return;
		}
	}

	/// Add player to queues
	for( std::vector< uint32 >::const_iterator itr = dungeons.begin(); itr != dungeons.end(); ++itr )
	{
		uint32 dungeon = *itr;

		if( queues[ dungeon ] == NULL )
		{
			queues[ dungeon ] = new LFGQueue( dungeon, LFGQueueGroupRequirements() );
		}
		
		queues[ dungeon ]->addPlayer( guid, player->GetTeam(), roles );
	}

	/// Add player to player to dungeon map
	playerToDungeons[ guid ] = dungeons;


	/// Notify player that they are in the queue
	if( !readd )
	{
		result.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_OK;
		result.serialize( buffer );
		player->SendPacket( &buffer );
	}

	Arcemu::GamePackets::LFG::SLFGUpdatePlayer update;
	update.dungeons = dungeons;
	update.queued = 1;

	if( readd )
		update.updateType = LFG_UPDATE_PROPOSAL_FAILURE;
	else
		update.updateType = LFG_UPDATE_JOINED;

	update.unk1 = 0;
	update.unk2 = 0;
	update.comment = "";
	update.serialize( buffer );
	player->SendPacket( &buffer );
}

void LfgMgr::removePlayer( uint32 guid )
{
	Guard guard( lock );

	removePlayerInternal( guid );

	LFGProposal *proposal = proposals.getProposalForPlayer( guid );
	if( proposal != NULL )
	{
		proposal->state = LFGProposal::LFG_PROPOSAL_STATE_FAIL;
		proposal->removePlayer( guid );
		sendProposal( proposal );
		onProposalFailed( proposal );
	}
}

void LfgMgr::updateProposal( uint32 id, uint32 guid, uint8 result )
{
	Guard guard( lock );

	LFGProposal *proposal = proposals.getProposal( id );
	if( proposal == NULL )
	{
		LOG_DEBUG( "Tried to update invalid proposal" );
		return;
	}

	if( ! proposal->updateProposalAnswer( guid, result ) )
	{
		LOG_DEBUG( "Player not found in proposal being updated." );
		return;
	}

	if( result == 0 )
	{
		proposal->state = LFGProposal::LFG_PROPOSAL_STATE_FAIL;
	}
	else
	if( proposal->hasAllAccepted() )
	{
		proposal->state = LFGProposal::LFG_PROPOSAL_STATE_SUCCESS;
	}

	/// Update players about the update
	sendProposal( proposal );

	if( proposal->state == LFGProposal::LFG_PROPOSAL_STATE_FAIL )
	{
		onProposalFailed( proposal );
	}
	else
	if( proposal->state == LFGProposal::LFG_PROPOSAL_STATE_SUCCESS )
	{
		onProposalSuccess( proposal );
	}
}

void LfgMgr::teleportPlayer( uint32 guid, bool out )
{
	Guard guard( lock );

	teleportPlayerInternal( guid, out );
}

void LfgMgr::teleportPlayerInternal( uint32 guid, bool out )
{
	Player *player = objmgr.GetPlayer( guid );
	if( player == NULL )
		return;

	Group *group = player->GetGroup();
	if( group == NULL )
		return;

	if( out )
	{
		player->SafeTeleport( player->m_entryPoint.map, player->m_entryPoint.instance, player->m_entryPoint.location );
	}
	else
	{
		uint32 gid = group->GetID();
		
		HM_NAMESPACE::HM_HASH_MAP< uint32, LFGInstance >::const_iterator itr = groupToInstance.find( gid );
		if( itr == groupToInstance.end() )
			return;
		
		const LFGInstance &instance = itr->second;

		player->saveEntryPoint();
		player->SafeTeleport( instance.map, instance.instance, instance.entrance );
	}
}

void LfgMgr::removeGroup( uint32 groupId )
{
	Guard guard( lock );

	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGInstance >::const_iterator itr = groupToInstance.find( groupId );
	if( itr == groupToInstance.end() )
		return;

	groupToInstance.erase( itr );
}

void LfgMgr::removeInstance( uint32 instanceId )
{
	Guard guard( lock );

	HM_NAMESPACE::HM_HASH_MAP< uint32, uint32 >::const_iterator itr = instanceToGroup.find( instanceId );
	if( itr == instanceToGroup.end() )
		return;

	HM_NAMESPACE::HM_HASH_MAP< uint32, LFGInstance >::const_iterator itr2 = groupToInstance.find( itr->second );
	if( itr2 == groupToInstance.end() )
		return;

	instanceToGroup.erase( itr );
	groupToInstance.erase( itr2 );
}

void LfgMgr::removePlayerInternal( uint32 guid )
{
	/// Packethandler checks if we are in world, which means player must exist
	Player *player = objmgr.GetPlayer( guid );

	/// Do we even have this player?
	HM_NAMESPACE::HM_HASH_MAP< uint32, std::vector< uint32 > >::iterator mapItr = playerToDungeons.find( guid );
	if( mapItr == playerToDungeons.end() )
	{
		return;
	}

	/// First remove player from queues
	const std::vector< uint32 > &dungeons = mapItr->second;
	for( std::vector< uint32 >::const_iterator dungeonItr = dungeons.begin(); dungeonItr != dungeons.end(); ++dungeonItr )
	{
		uint32 dungeon = *dungeonItr;
		LFGQueue *queue = queues[ dungeon ];
		queue->removePlayer( guid );
	}

	/// Then remove from the map as well
	playerToDungeons.erase( mapItr );

	/// Notify player about the removal
	PacketBuffer buffer;
	Arcemu::GamePackets::LFG::SLFGUpdatePlayer update;
	update.queued = 0;
	update.updateType = LFG_UPDATE_REMOVED;
	update.unk1 = 0;
	update.unk2 = 0;
	update.comment = "";
	update.serialize( buffer );
	player->SendPacket( &buffer );
}

void LfgMgr::update( bool force )
{
	Guard guard( lock );

	/// Fail expired proposals
	std::vector< LFGProposal* > expiredProposals;
	proposals.getExpiredProposals( expiredProposals );
	std::vector< LFGProposal* >::iterator expiredProposalItr = expiredProposals.begin();
	while( expiredProposalItr != expiredProposals.end() )
	{
		LFGProposal *proposal = *expiredProposalItr;
		proposal->state = LFGProposal::LFG_PROPOSAL_STATE_FAIL;
		sendProposal( proposal );
		onProposalFailed( proposal );

		++expiredProposalItr;
	}
	expiredProposals.clear();

	/// Update queues, create proposals if possible
	for( int dungeon = 1; dungeon < LFGMGR_QUEUE_COUNT; dungeon++ )
	{
		LFGQueue *queue = queues[ dungeon ];
		if( queue == NULL )
		{
			continue;
		}
		
		for( uint32 team = TEAM_ALLIANCE; team <= TEAM_HORDE; team++ )
		{
			/// Let's see if we can find a group to propose for this faction
			LFGProposal *proposal = queue->findMatch( team, force );
			if( proposal != NULL )
			{
				proposal->dungeon = dungeon;
				onGroupFound( proposal );
			}
		}
	}

	/// Send queue status
	for( int dungeon = 1; dungeon < LFGMGR_QUEUE_COUNT; dungeon++ )
	{
		LFGQueue *queue = queues[ dungeon ];
		if( queue == NULL )
		{
			continue;
		}

		queue->updateQueueStatus();
	}
}

void LfgMgr::sendProposal( LFGProposal *proposal )
{
	std::vector< LFGProposalEntry >::iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{		
		sendProposalToPlayer( itr->guid, proposal );
		++itr;
	}
}

void LfgMgr::sendProposalToPlayer( uint32 guid, LFGProposal *proposal )
{
	Player *player = objmgr.GetPlayer( guid );

	Arcemu::GamePackets::LFG::SLFGProposalUpdate update;
	update.dungeonId = proposal->dungeon;
	update.proposalId = proposal->id;
	update.proposalState = proposal->state;
	update.encountersFinishedMask = 0;
	update.silent = 0;

	for( size_t i = 0; i < proposal->players.size(); i++ )
	{
		Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFGProposalEntry e;
		e.roleMask = proposal->players[ i ].selectedRoles;

		if( guid == proposal->players[ i ].guid )
			e.isCurrentPlayer = 1;
		else
			e.isCurrentPlayer = 0;

		e.inDungeon = 0;
		e.inSameGroup = 0;
		e.hasAnswered = proposal->players[ i ].answered;
		e.hasAccepted = proposal->players[ i ].accepted;
		update.entries.push_back( e );
	}

	/// Forced proposed group, need to fill in some dummies, otherwise we get a nice interface Lua error in the client
	if( update.entries.size() < 5 )
	{
		uint32 needed = 5 - update.entries.size();
		while( needed > 0 )
		{
			Arcemu::GamePackets::LFG::SLFGProposalUpdate::LFGProposalEntry e;
			e.roleMask = 8;
			e.isCurrentPlayer = 0;
			e.inDungeon = 0;
			e.inSameGroup = 0;
			e.hasAnswered = 1;
			e.hasAccepted = 1;
			update.entries.push_back( e );
			needed--;
		}
	}

	PacketBuffer buffer;
	update.serialize( buffer );
	player->SendPacket( &buffer );
}

void LfgMgr::onGroupFound( LFGProposal *proposal )
{
	/// We don't want these players to be part of further matchmaking
	std::vector< LFGProposalEntry >::iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		removePlayerInternal( itr->guid );
		++itr;
	}
	
	/// Store the proposal
	proposal->expiryTime = UNIXTIME + LFG_PROPOSAL_DURATION_SECONDS;
	uint32 id = proposals.addProposal( proposal );
	proposal->id = id;
	
	/// Notify players
	sendProposal( proposal );

	PacketBuffer playerUpdateBuffer;
	Arcemu::GamePackets::LFG::SLFGUpdatePlayer playerUpdate;
	playerUpdate.queued = 0;
	playerUpdate.updateType = LFG_UPDATE_PROPOSAL_BEGIN;
	playerUpdate.unk1 = 0;
	playerUpdate.unk2 = 0;
	playerUpdate.comment = "";
	playerUpdate.dungeons.push_back( proposal->dungeon );
	playerUpdate.serialize( playerUpdateBuffer );

	std::vector< LFGProposalEntry >::const_iterator entryItr = proposal->players.begin();
	while( entryItr != proposal->players.end() )
	{
		Player *player = objmgr.GetPlayer( entryItr->guid );
		player->SendPacket( &playerUpdateBuffer );
		++entryItr;
	}
}


void LfgMgr::onProposalFailed( LFGProposal *proposal )
{
	proposals.removeProposal( proposal->id );

	std::vector< uint32 > dungeons;
	dungeons.push_back( proposal->dungeon );

	PacketBuffer playerUpdateBuffer;
	Arcemu::GamePackets::LFG::SLFGUpdatePlayer playerUpdate;
	playerUpdate.queued = 0;
	playerUpdate.updateType = LFG_UPDATE_REMOVED;
	playerUpdate.unk1 = 0;
	playerUpdate.unk2 = 0;
	playerUpdate.comment = "";
	playerUpdate.serialize( playerUpdateBuffer );

	PacketBuffer partyUpdateBuffer;
	Arcemu::GamePackets::LFG::SLFGUpdateParty partyUpdate;
	partyUpdate.updateType = LFG_UPDATE_REMOVED;
	partyUpdate.queued = 0;
	partyUpdate.joined = 0;
	partyUpdate.unk1 = 0;
	partyUpdate.unk2 = 0;
	partyUpdate.comment = "";
	partyUpdate.serialize( partyUpdateBuffer );

	std::vector< uint32 > removables;

	/// Find the player who declined
	std::vector< LFGProposalEntry >::iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		if( ( itr->answered == 1 ) && ( itr->accepted == 0 ) )
		{
			break;
		}
		++itr;
	}

	/// Should always be true
	if( itr != proposal->players.end() )
	{
		/// Tell the player they are out of the queue
		Player *player = objmgr.GetPlayer( itr->guid );
		player->SendPacket( &partyUpdateBuffer );
		player->SendPacket( &playerUpdateBuffer );

		/// And remove them from the proposal
		proposal->players.erase( itr );
	}

	/// Now requeue the rest of the group
	readdGroup( proposal );
}

void LfgMgr::onProposalSuccess( LFGProposal *proposal )
{
	uint32 dungeon = proposal->dungeon;
	LFGDungeonEntry *dungeonEntry = dbcLFGDungeon.LookupEntry( dungeon );
	
	/// Generate a random dungeon
	if( dungeonEntry->type == LFG_DUNGEON_TYPE_RANDOM )
	{
		dungeon = pickRandomDungeonForProposal( proposal );
		/// We couldn't generate a dungeon :(
		if( dungeon == 0 )
		{
			proposal->state = LFGProposal::LFG_PROPOSAL_STATE_FAIL;
			sendProposal( proposal );
			onProposalFailed( proposal );
			return;
		}
	}

	proposals.removeProposal( proposal->id );

	const LFGDungeonData *data = objmgr.getLFGDungeonData( dungeon );

	LocationVector location( data->entrypoint.x, data->entrypoint.y, data->entrypoint.z, data->entrypoint.o  );
	MapMgr *mgr = sInstanceMgr.CreateInstance( INSTANCE_NONRAID, data->map, dungeonEntry->difficulty );
	if( mgr == NULL )
	{
		LOG_DEBUG( "Failed to create instance" );
		delete proposal;
		return;
	}

	PacketBuffer groupFoundBuffer;
	Arcemu::GamePackets::LFG::SLFGUpdatePlayer groupFoundUpdate;
	groupFoundUpdate.queued = 0;
	groupFoundUpdate.updateType = LFG_UPDATE_GROUP_FOUND;
	groupFoundUpdate.unk1 = 0;
	groupFoundUpdate.unk2 = 0;
	groupFoundUpdate.comment = "";
	groupFoundUpdate.serialize( groupFoundBuffer );

	PacketBuffer playerUpdateBuffer;
	Arcemu::GamePackets::LFG::SLFGUpdatePlayer playerUpdate;
	playerUpdate.queued = 0;
	playerUpdate.updateType = LFG_UPDATE_REMOVED;
	playerUpdate.unk1 = 0;
	playerUpdate.unk2 = 0;
	playerUpdate.comment = "";
	playerUpdate.serialize( playerUpdateBuffer );

	PacketBuffer partyUpdateBuffer;
	Arcemu::GamePackets::LFG::SLFGUpdateParty partyUpdate;
	partyUpdate.updateType = LFG_UPDATE_REMOVED;
	partyUpdate.queued = 0;
	partyUpdate.joined = 0;
	partyUpdate.unk1 = 0;
	partyUpdate.unk2 = 0;
	partyUpdate.comment = "";
	partyUpdate.serialize( partyUpdateBuffer );

	std::vector< LFGProposalEntry >::iterator itr;

	/// Send updates to players
	itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		Player *player = objmgr.GetPlayer( itr->guid );
		player->SendPacket( &groupFoundBuffer );
		player->SendPacket( &playerUpdateBuffer );
		player->SendPacket( &partyUpdateBuffer );
		++itr;
	}

	// Create LFD group, and add players
	Group *group = new Group( true );
	group->makeLFDGroup();
	group->setLFGDungeon( proposal->dungeon );
	
	LFGInstance instance;
	instance.map = data->map;
	instance.instance = mgr->GetInstanceID();
	instance.entrance = location;
	
	groupToInstance[ group->GetID() ] = instance;
	instanceToGroup[ mgr->GetInstanceID() ] = group->GetID();
	
	itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		Player *player = objmgr.GetPlayer( itr->guid );
		group->AddMember( player->getPlayerInfo(), -1, itr->selectedRoles );
		if( ( itr->selectedRoles & LFG_ROLE_LEADER ) != 0 )
		{
			group->SetLeader( player, false );
		}
		++itr;
	}
	
	mgr->pInstance->m_creatorGroup = group->GetID();
	group->SetDungeonDifficulty( dungeonEntry->difficulty );

	// Teleport players
	itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		teleportPlayer( itr->guid );
		++itr;
	}

	delete proposal;
}

uint32 LfgMgr::pickRandomDungeonForProposal( LFGProposal *proposal )
{
	/// First find the lowest, and highest level player
	uint32 low = 999;
	uint32 high = 0;

	std::vector< LFGProposalEntry >::const_iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		Player *player = objmgr.GetPlayer( itr->guid );

		if( player->getLevel() > high )
		{
			high = player->getLevel();
		}

		if( player->getLevel() < low )
		{
			low = player->getLevel();
		}

		++itr;
	}

	LFGDungeonEntry *randomDungeonEntry = dbcLFGDungeon.LookupEntry( proposal->dungeon );

	std::vector< uint32 > candidates;

	/// Now find a dungeon that is appropriate based on level
	DBCStorage< LFGDungeonEntry >::iterator lfgDungeonItr = dbcLFGDungeon.begin();
	while( lfgDungeonItr != dbcLFGDungeon.end() )
	{
		const LFGDungeonEntry *lfgDungeonEntry = *lfgDungeonItr;
		LFGDungeonData *data = objmgr.getLFGDungeonData( lfgDungeonEntry->ID );

		/// We need teleport data
		if( data == NULL )
		{
			++lfgDungeonItr;
			continue;
		}

		/// We need the same expansion
		if( lfgDungeonEntry->expansion != randomDungeonEntry->expansion )
		{
			++lfgDungeonItr;
			continue;
		}

		/// If we queued for normal, we need a normal dungeon, same with heroic.
		if( lfgDungeonEntry->difficulty != randomDungeonEntry->difficulty )
		{
			++lfgDungeonItr;
			continue;
		}

		/// Only normal and heroic dungeons
		if( ( lfgDungeonEntry->type != LFG_DUNGEON_TYPE_NORMAL ) && ( lfgDungeonEntry->type != LFG_DUNGEON_TYPE_HEROIC ) )
		{
			++lfgDungeonItr;
			continue;
		}

		/// should be appropriate for both the lowest and highest level
		if( low >= lfgDungeonEntry->minlevel &&
			low <= lfgDungeonEntry->maxlevel &&
			high <= lfgDungeonEntry->maxlevel )
		{
			candidates.push_back( lfgDungeonEntry->ID );
		}

		++lfgDungeonItr;
	}

	/// We haven't found any appropriate :(
	if( candidates.size() == 0 )
	{
		return 0;
	}

	uint32 chosen = 0;
	if( candidates.size() > 1 )
	{
		chosen = RandomUInt( candidates.size() - 1 );
	}

	return candidates[ chosen ];
}


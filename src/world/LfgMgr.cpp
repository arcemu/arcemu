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

LFGQueue::LFGQueue( const LFGQueueGroupRequirements &requirements ) :
groupRequirements( requirements )
{
}

LFGQueue::~LFGQueue()
{
	queue.clear();
}

void LFGQueue::addPlayer( uint32 guid, uint32 team, uint32 roles, bool readd )
{
	LFGQueueEntry entry;
	entry.guid = guid;
	entry.team = team;
	entry.roles = roles;

	if( readd )
		queue.push_front( entry );
	else
		queue.push_back( entry );	
}

void LFGQueue::removePlayer( uint32 guid )
{
	std::deque< LFGQueueEntry >::iterator itr = queue.begin();
	while( itr != queue.end() )
	{
		const LFGQueueEntry &entry = *itr;
		if( entry.guid == guid )
			break;
		++itr;
	}

	queue.erase( itr );
}

LFGProposal* LFGQueue::findMatch( uint32 team, bool force )
{
	if( queue.empty() )
		return NULL;

	LFGQueueGroupRequirements requirements = groupRequirements;
	LFGProposal *proposal = new LFGProposal();

	std::deque< LFGQueueEntry >::iterator itr = queue.begin();
	while( !requirements.met() && ( itr != queue.end() ) )
	{
		const LFGQueueEntry &entry = *itr;
		uint32 selectedRole = 0;

		if( entry.team == team )
		{
			// Select the player for some role if we can
			if( requirements.tanksNeeded > 0 && ( ( entry.roles & LFG_ROLE_TANK ) != 0 ) )
			{
				selectedRole = LFG_ROLE_TANK;
				requirements.tanksNeeded--;
			}
			else
			if( requirements.healersNeeded > 0 && ( ( entry.roles & LFG_ROLE_HEALER ) != 0 ) )
			{
				selectedRole = LFG_ROLE_HEALER;
				requirements.healersNeeded--;
			}
			else
			if( requirements.dpsNeeded > 0 && ( ( entry.roles & LFG_ROLE_DPS ) != 0 ) )
			{
				selectedRole = LFG_ROLE_DPS;
				requirements.dpsNeeded--;
			}

			// We have selected this player
			if( selectedRole != 0 )
			{

				// If they want to be the leader, let them be
				if( requirements.needLeader && ( ( entry.roles & LFG_ROLE_LEADER ) != 0 ) )
				{
					selectedRole |= LFG_ROLE_LEADER;
					requirements.needLeader = false;
				}

				LFGProposalEntry proposalEntry;
				proposalEntry.guid = entry.guid;
				proposalEntry.team = team;
				proposalEntry.selectedRoles = selectedRole;
				proposal->players.push_back( proposalEntry );
			}
		}

		++itr;
	}

	/// There was no leader, so let's make the first player one
	if( requirements.needLeader && proposal->players.size() > 0 )
	{
		proposal->players[ 0 ].selectedRoles |= LFG_ROLE_LEADER;
		requirements.needLeader = false;
	}

	/// If we don't have enough for a group proposal, discard, unless forced
	if( !force && !requirements.met() )
	{
		proposal->players.clear();
		delete proposal;
		proposal = NULL;
	}

	return proposal;
}

initialiseSingleton(LfgMgr);

LfgMgr::LfgMgr()
{
	for( int i = 0; i < LFGMGR_QUEUE_COUNT; i++ )
	{
		queues[ i ] = NULL;
	}
}

LfgMgr::~LfgMgr()
{
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
		if( data == NULL )
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

		if( entry->type == LFG_DUNGEON_TYPE_RANDOM )
		{
			LOG_DEBUG( "Random LFG dungeons are not supported at this time." );
			player->GetSession()->SystemMessage( "Random dungeon finding is not supported at this time." );
			return;
		}
	}

	/// Add player to queues
	for( std::vector< uint32 >::const_iterator itr = dungeons.begin(); itr != dungeons.end(); ++itr )
	{
		uint32 dungeon = *itr;

		if( queues[ dungeon ] == NULL )
		{
			queues[ dungeon ] = new LFGQueue( LFGQueueGroupRequirements() );
		}
		
		queues[ dungeon ]->addPlayer( guid, player->GetTeam(), roles, readd );
	}

	/// Add player to player to dungeon map
	playerToDungeons[ guid ] = dungeons;


	/// Notify player that they are in the queue
	PacketBuffer buffer;

	if( !readd )
	{
		Arcemu::GamePackets::LFG::SLFGJoinResult result;
		result.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_OK;
		result.state = 0;
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
			}
		}
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


void LfgMgr::onProposalFailed( LFGProposal *proposal )
{
	proposals.removeProposal( proposal->id );

	std::vector< uint32 > dungeons;
	dungeons.push_back( proposal->dungeon );

	std::vector< LFGProposalEntry >::iterator itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		/// If the player didn't decline, requeue
		if( ( itr->answered == 0 ) || ( itr->accepted == 1 ) )
		{
			addPlayerInternal( itr->guid, itr->selectedRoles, dungeons, true );
		}
		else
		{
			/// Otherwise tell them they are out of the queue
			Player *player = objmgr.GetPlayer( itr->guid );
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

		++itr;
	}

	delete proposal;
}

void LfgMgr::onProposalSuccess( LFGProposal *proposal )
{
	proposals.removeProposal( proposal->id );

	const LFGDungeonData *data = objmgr.getLFGDungeonData( proposal->dungeon );

	/// Scarlet Monastery, for now
	LocationVector location( data->entrypoint.x, data->entrypoint.y, data->entrypoint.z, data->entrypoint.o  );
	MapMgr *mgr = sInstanceMgr.CreateInstance( INSTANCE_NONRAID, data->map );
	if( mgr == NULL )
	{
		LOG_DEBUG( "Failed to create instance" );
		delete proposal;
		return;
	}

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

	// Create LFD group, and add players, if we have a real group, not a forced proposal
	if( proposal->players.size() > 1 )
	{
		Group *group = new Group( true );
		group->makeLFDGroup();

		itr = proposal->players.begin();		
		while( itr != proposal->players.end() )
		{
			Player *player = objmgr.GetPlayer( itr->guid );
			
			group->AddMember( player->getPlayerInfo() );
			if( ( itr->selectedRoles & LFG_ROLE_LEADER ) != 0 )
			{
				group->SetLeader( player, false );
			}

			player->SendPacket( &partyUpdateBuffer );
			
			++itr;
		}

		mgr->pInstance->m_creatorGroup = group->GetID();
	}
	else
	{
		mgr->pInstance->m_creatorGuid = proposal->players[ 0 ].guid;
	}

	// Teleport players
	itr = proposal->players.begin();
	while( itr != proposal->players.end() )
	{
		Player *player = objmgr.GetPlayer( itr->guid );
		player->SendPacket( &playerUpdateBuffer );
		player->SafeTeleport( mgr, location );
		++itr;
	}

	delete proposal;
}

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

LFGQueue::LFGQueue()
{
}

LFGQueue::~LFGQueue()
{
	queue.clear();
}

void LFGQueue::addPlayer( uint32 guid, uint32 team, uint32 roles )
{
	LFGQueueEntry entry;
	entry.guid = guid;
	entry.team = team;
	entry.roles = roles;

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

initialiseSingleton(LfgMgr);

LfgMgr::LfgMgr()
{
	for( int i = 0; i < LFGMGR_MAX_DUNGEONS; i++ )
	{
		queues[ i ] = NULL;
	}
}

LfgMgr::~LfgMgr()
{
	playerToDungeons.clear();

	for( int i = 0; i < LFGMGR_MAX_DUNGEONS; i++ )
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

	// Packethandler checks if we're in world, which means player must exist
	Player *player = objmgr.GetPlayer( guid );

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
			queues[ dungeon ] = new LFGQueue();
		}
		
		queues[ dungeon ]->addPlayer( guid, player->GetTeam(), roles );
	}

	/// Add player to player to dungeon map
	playerToDungeons[ guid ] = dungeons;


	/// Notify player that they are in the queue
	PacketBuffer buffer;

	Arcemu::GamePackets::LFG::SLFGJoinResult result;
	result.result = Arcemu::GamePackets::LFG::SLFGJoinResult::LFG_JOIN_OK;
	result.state = 0;
	result.serialize( buffer );
	player->SendPacket( &buffer );

	Arcemu::GamePackets::LFG::SLFGUpdatePlayer update;
	update.dungeons = dungeons;
	update.queued = 1;
	update.updateType = 0;
	update.unk1 = 0;
	update.unk2 = 0;
	update.comment = "";
	update.serialize( buffer );
	player->SendPacket( &buffer );
}

void LfgMgr::removePlayer( uint32 guid )
{
	Guard guard( lock );

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
	update.updateType = 0;
	update.unk1 = 0;
	update.unk2 = 0;
	update.comment = "";
	update.serialize( buffer );
	player->SendPacket( &buffer );
}

void LfgMgr::update()
{
	Guard guard( lock );
}



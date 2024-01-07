/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#include "plugin.h"

#define GO_EP_TOWER_BANNER_NORTHPASS    181899
#define GO_EP_TOWER_BANNER_CROWNGUARD   182096
#define GO_EP_TOWER_BANNER_EASTWALL     182097
#define GO_EP_TOWER_BANNER_PLAGUEWOOD   182098

#define EP_TOWER_BANNER_UPDATE_FREQ (2 * 1000)
#define EP_TOWER_BANNER_RANGE 50.0f

#define EP_TOWER_NORTHPASS   0
#define EP_TOWER_CROWNGUARD  1
#define EP_TOWER_EASTWALL    2
#define EP_TOWER_PLAGUEWOOD  3
#define EP_TOWER_COUNT       4

#define TOWER_ALLIANCE_CONTROL 0
#define TOWER_HORDE_CONTROL    1
#define TOWER_NEUTRAL          2
#define TOWER_STATES           3

static const char* towerNames[] = 
{
	"Northpass Tower",
	"Crown Guard Tower",
	"Eastwall Tower",
	"Plaugewood Tower"
};

static int32 towerOwner[] = {
	-1,
	-1,
	-1,
	-1
};

enum EPWorldStates
{
	WORLDSTATE_EPL_TOWERS_ALLIANCE      = 2327,
	WORLDSTATE_EPL_TOWERS_HORDE         = 2328,
	WORLDSTATE_EPL_NORTHPASS_NEUTRAL    = 2352,
	WORLDSTATE_EPL_PLAGUEWOOD_NEUTRAL   = 2353,
	WORLDSTATE_EPL_EASTWALL_ALLIANCE    = 2354,
	WORLDSTATE_EPL_CROWNGUARD_NEUTRAL   = 2355,
	WORLDSTATE_EPL_EASTWALL_HORDE       = 2356,
	WORLDSTATE_EPL_EASTWALL_NEUTRAL     = 2361,
	WORLDSTATE_EPL_PLAGUEWOOD_ALLIANCE  = 2370,
	WORLDSTATE_EPL_PLAGUEWOOD_HORDE     = 2371,
	WORLDSTATE_EPL_NORTHPASS_ALLIANCE   = 2372,
	WORLDSTATE_EPL_NORTHPASS_HORDE      = 2373,
	WORLDSTATE_EPL_CROWNGUARD_ALLIANCE  = 2378,
	WORLDSTATE_EPL_CROWNGUARD_HORDE     = 2379,	
	WORLDSTATE_EPL_TOWER_PROGRESS_UI    = 2426,
	WORLDSTATE_EPL_TOWER_PROGRESS       = 2427
};

static uint32 towerWorldStates[ EP_TOWER_COUNT ][ TOWER_STATES ] = {
	{ WORLDSTATE_EPL_NORTHPASS_ALLIANCE, WORLDSTATE_EPL_NORTHPASS_HORDE, WORLDSTATE_EPL_NORTHPASS_NEUTRAL },
	{ WORLDSTATE_EPL_CROWNGUARD_ALLIANCE, WORLDSTATE_EPL_CROWNGUARD_HORDE, WORLDSTATE_EPL_CROWNGUARD_NEUTRAL },
	{ WORLDSTATE_EPL_EASTWALL_ALLIANCE, WORLDSTATE_EPL_EASTWALL_HORDE, WORLDSTATE_EPL_EASTWALL_NEUTRAL },
	{ WORLDSTATE_EPL_PLAGUEWOOD_ALLIANCE, WORLDSTATE_EPL_PLAGUEWOOD_HORDE, WORLDSTATE_EPL_PLAGUEWOOD_NEUTRAL }
};

#define ZONE_EPL 139

#define TOWER_CAPTURE_PROGRESS_TICK       5
#define TOWER_CAPTURE_PROGRESS_DEFAULT    50

#define TOWER_CAPTURE_TRESHOLD_ALLIANCE   100
#define TOWER_CAPTURE_TRESHOLD_HORDE      0
#define TOWER_CAPTURE_TRESHOLD_NEUTRAL_HI 70
#define TOWER_CAPTURE_TRESHOLD_NEUTRAL_LO 30

class EasternPlaguelandsPvP
{
private:
	MapMgr *mapMgr;

public:
	EasternPlaguelandsPvP()
	{
		mapMgr = NULL;
	}

	~EasternPlaguelandsPvP()
	{
		mapMgr = NULL;
	}

	void setMapMgr( MapMgr *mgr )
	{
		mapMgr = mgr;
	}

	/// Broadcast a message to everyone that says the tower has been captured by the faction
	void broadcastCaptureMessage( uint32 towerId )
	{
		std::string faction;
		if( towerOwner[ towerId ] == TEAM_ALLIANCE )
		{
			faction.assign( "The alliance" );
		}
		else
		{
			faction.assign( "The horde" );
		}
		
		std::stringstream ss;
		ss << faction;
		ss << " has captured";
		ss << " the " << towerNames[ towerId ] << "!";
		
		WorldPacket *packet = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL, ss.str().c_str(), 0, 0 );
		if( packet != NULL )
		{
			mapMgr->SendPacketToPlayersInZone( ZONE_EPL, packet );
			delete packet;
		}
	}

	void onTowerCaptured( uint32 towerId )
	{
		broadcastCaptureMessage( towerId );
	}

	void onTowerOwnershipChange( uint32 towerId, int32 lastOwner )
	{
		uint32 allianceDelta = 0;
		uint32 hordeDelta = 0;

		switch( lastOwner )
		{
			case -1:
				if( towerOwner[ towerId ] == TEAM_ALLIANCE )
				{
					allianceDelta = 1;
				}
				else
				{
					hordeDelta = 1;
				}
				break;

			case TEAM_ALLIANCE:
				allianceDelta = -1;
				break;

			case TEAM_HORDE:
				hordeDelta = -1;
				break;
		}
		
		// Send packets to entire zone if owner changed
		WorldStatesHandler &handler = mapMgr->GetWorldStatesHandler();
		
		uint32 allianceTowers = handler.GetWorldStateForZone( ZONE_EPL, WORLDSTATE_EPL_TOWERS_ALLIANCE );
		uint32 hordeTowers = handler.GetWorldStateForZone( ZONE_EPL, WORLDSTATE_EPL_TOWERS_HORDE );
		
		switch( towerOwner[ towerId ] )
		{
			case -1:
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_ALLIANCE_CONTROL ], 0 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_HORDE_CONTROL ], 0 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_NEUTRAL ], 1 );
				break;
			
			case TEAM_ALLIANCE:
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_ALLIANCE_CONTROL ], 1 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_HORDE_CONTROL ], 0 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_NEUTRAL ], 0 );
				
				onTowerCaptured( towerId );
				break;
			
			case TEAM_HORDE:
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_ALLIANCE_CONTROL ], 0 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_HORDE_CONTROL ], 1 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_NEUTRAL ], 0 );
				
				onTowerCaptured( towerId );
				break;
		}
		
		allianceTowers += allianceDelta;
		hordeTowers += hordeDelta;
		handler.SetWorldStateForZone( ZONE_EPL, WORLDSTATE_EPL_TOWERS_ALLIANCE, allianceTowers );
		handler.SetWorldStateForZone( ZONE_EPL, WORLDSTATE_EPL_TOWERS_HORDE, hordeTowers );
	}
};

static EasternPlaguelandsPvP pvp;

class EPTowerBannerAI : public GameObjectAIScript
{
private:
	uint32 towerId;
	uint32 captureProgress;

public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION( EPTowerBannerAI );

	EPTowerBannerAI( GameObject *go ) : GameObjectAIScript( go )
	{
		towerId = 0;
		captureProgress = TOWER_CAPTURE_PROGRESS_DEFAULT;
	}

	/// Owner change event handler
	void onOwnerChange( int32 lastOwner )
	{
		pvp.onTowerOwnershipChange( towerId, lastOwner );
	}

	/// Calculate the current capture progress based on player counts
	void calculateProgress( uint32 alliancePlayers, uint32 hordePlayers )
	{
		int32 delta = 0;
		if( alliancePlayers > hordePlayers )
		{
			delta = 1;
		}
		else
		if( hordePlayers > alliancePlayers )
		{
			delta = -1;
		}

		delta *= TOWER_CAPTURE_PROGRESS_TICK;

		int32 progress = captureProgress;

		if( ( ( progress < 100 ) && ( delta > 0 ) ) ||
			( ( progress > 0 ) && ( delta < 0 ) ) )
		{
			progress += delta;
			progress = Math::clamp< int32 >( progress, 0, 100 );
			captureProgress = progress;
		}
	}

	/// Calculate the current owner based on the current progress
	/// Returns true on owner change
	bool calculateOwner()
	{
		bool ownerChanged = false;
		if( captureProgress == TOWER_CAPTURE_TRESHOLD_ALLIANCE )
		{
			if( towerOwner[ towerId ] != TEAM_ALLIANCE )
			{
				ownerChanged = true;
				towerOwner[ towerId ] = TEAM_ALLIANCE;
			}
		}
		else
		if( captureProgress <= TOWER_CAPTURE_TRESHOLD_HORDE )
		{
			if( towerOwner[ towerId ] != TEAM_HORDE )
			{
				ownerChanged = true;
				towerOwner[ towerId ] = TEAM_HORDE;
			}
		}
		else
		if( ( captureProgress <= TOWER_CAPTURE_TRESHOLD_NEUTRAL_HI ) && ( captureProgress >= TOWER_CAPTURE_TRESHOLD_NEUTRAL_LO ) )
		{
			if( towerOwner[ towerId ] != -1 )
			{
				ownerChanged = true;
				towerOwner[ towerId ] = -1;
			}
		}

		return ownerChanged;
	}

	void OnSpawn()
	{
		/// If we're not on the right map stop here, so we won't have to deal with multiple threads
		if( _gameobject->GetMapId() != MAP_EASTERN_KINGDOMS )
			return;

		switch( _gameobject->GetInfo()->ID )
		{
			case GO_EP_TOWER_BANNER_NORTHPASS:
				towerId = EP_TOWER_NORTHPASS;
				break;

			case GO_EP_TOWER_BANNER_CROWNGUARD:
				towerId = EP_TOWER_CROWNGUARD;
				break;

			case GO_EP_TOWER_BANNER_EASTWALL:
				towerId = EP_TOWER_EASTWALL;
				break;

			case GO_EP_TOWER_BANNER_PLAGUEWOOD:
				towerId = EP_TOWER_PLAGUEWOOD;
				break;

			default:
				return;
				break;
		}

		RegisterAIUpdateEvent( EP_TOWER_BANNER_UPDATE_FREQ );
	}

	void AIUpdate()
	{
		std::set< Player* > playersInRange;

		uint32 alliancePlayers = 0;
		uint32 hordePlayers = 0;		

		/// Find and count players in range
		std::set< Object* > players = _gameobject->GetInRangePlayers();
		for( std::set< Object* >::const_iterator itr = players.begin(); itr != players.end(); ++itr )
		{
			Player *player = static_cast< Player* >( *itr );
			player->GetDisplayId();

			float d = player->CalcDistance( _gameobject );

			if( d > EP_TOWER_BANNER_RANGE )
			{
				/// No progress bar for players out of range
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_EPL_TOWER_PROGRESS_UI, 0 );
			}
			else
			{
				if( player->GetTeam() == TEAM_ALLIANCE )
				{
					alliancePlayers++;
				}
				else
				{
					hordePlayers++;
				}

				playersInRange.insert( player );
			}
		}

		calculateProgress( alliancePlayers, hordePlayers );
		int32 lastOwner = towerOwner[ towerId ];
		bool ownerChanged = calculateOwner();

		/// Send progress to players fighting for control
		for( std::set< Player* >::const_iterator itr = playersInRange.begin(); itr != playersInRange.end(); ++itr )
		{
			Player *player = *itr;

			Messenger::SendWorldStateUpdate( player, WORLDSTATE_EPL_TOWER_PROGRESS_UI, 1 );
			Messenger::SendWorldStateUpdate( player, WORLDSTATE_EPL_TOWER_PROGRESS, captureProgress );
		}

		if( ownerChanged )
		{
			onOwnerChange( lastOwner );
		}

		playersInRange.clear();
	}
};

void setupEasternPlaguelands( ScriptMgr *mgr )
{
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_EASTERN_KINGDOMS );
	pvp.setMapMgr( mapMgr );

	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_NORTHPASS, &EPTowerBannerAI::Create );
	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_CROWNGUARD, &EPTowerBannerAI::Create );
	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_EASTWALL, &EPTowerBannerAI::Create );
	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_PLAGUEWOOD, &EPTowerBannerAI::Create );
}

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

#define ZONE_TEROKKAR_FOREST 3519

#define TF_TOWER_SCAN_UPDATE_FREQ (2*1000)
#define TF_TOWER_SCAN_RANGE 150.0f
#define TF_TOWER_CAPTURE_RANGE 50.0f
#define TF_TOWER_CAPTURE_PROGRESS_TICK 10

#define TF_TOWER_CAPTURE_THRESHOLD_ALLIANCE   100
#define TF_TOWER_CAPTURE_THRESHOLD_HORDE      0
#define TF_TOWER_CAPTURE_THRESHOLD_NEUTRAL_HI 50
#define TF_TOWER_CAPTURE_THRESHOLD_NEUTRAL_LO 50

enum SpiritTowers
{
	TF_TOWER_NW    = 0,
	TF_TOWER_N     = 1,
	TF_TOWER_NE    = 2,
	TF_TOWER_SE    = 3,
	TF_TOWER_S     = 4,
	TF_TOWER_COUNT = 5
};

enum TerokkarBannerGOs
{
	GO_TEROKKAR_BANNER_NW = 183104,
	GO_TEROKKAR_BANNER_N  = 183411,
	GO_TEROKKAR_BANNER_NE = 183412,
	GO_TEROKKAR_BANNER_SE = 183413,
	GO_TEROKKAR_BANNER_S  = 183414
};

enum SpiritTowerArtkit
{
	ARTKIT_BANNER_HORDE    = 1,
	ARTKIT_BANNER_ALLIANCE = 2,
	ARTKIT_BANNER_NEUTRAL  = 21
};

enum TerokkarWorldStates
{
	WORLDSTATE_TF_TIMER_NEUTRAL              = 2508,
	WORLDSTATE_TF_TIMER_HOURS                = 2509,
	WORLDSTATE_TF_TIMER_MINUTES_2            = 2510,
	WORLDSTATE_TF_TIMER_MINUTES_1            = 2512,

	WORLDSTATE_TF_TOWERS_CONTROLLED_UI       = 2620,
	WORLDSTATE_TF_TOWERS_CONTROOLED_ALLIANCE = 2621,
	WORLDSTATE_TF_TOWERS_CONTROOLED_HORDE    = 2622,
	WORLDSTATE_TF_PROGRESS_BAR_UI            = 2623,
	WORLDSTATE_TF_PROGRESS_BAR_PROGRESS      = 2625,

	WORLDSTATE_TF_TOWER_NW_NEUTRAL           = 2681,
	WORLDSTATE_TF_TOWER_NW_HORDE             = 2682,
	WORLDSTATE_TF_TOWER_NW_ALLIANCE          = 2683,

	WORLDSTATE_TF_TOWER_N_ALLIANCE           = 2684,
	WORLDSTATE_TF_TOWER_N_HORDE              = 2685,
	WORLDSTATE_TF_TOWER_N_NEUTRAL            = 2686,

	WORLDSTATE_TF_TOWER_NE_ALLIANCE          = 2688,
	WORLDSTATE_TF_TOWER_NE_HORDE             = 2689,
	WORLDSTATE_TF_TOWER_NE_NEUTRAL           = 2690,

	WORLDSTATE_TF_TOWER_S_ALLIANCE           = 2691,
	WORLDSTATE_TF_TOWER_S_HORDE              = 2692,
	WORLDSTATE_TF_TOWER_S_NEUTRAL            = 2693,

	WORLDSTATE_TF_TOWER_SE_ALLIANCE          = 2694,
	WORLDSTATE_TF_TOWER_SE_HORDE             = 2695,
	WORLDSTATE_TF_TOWER_SE_NEUTRAL           = 2696,	

	WORLDSTATE_TF_TIMER_ALLIANCE             = 2767,
	WORLDSTATE_TF_TIMER_HORDE                = 2768
};

#define TF_TOWER_OWNER_NEUTRAL 2

static uint32 towerOwners[ TF_TOWER_COUNT ] = 
{
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL
};

static uint32 towerCaptureProgress[ TF_TOWER_COUNT ] = 
{
	50,
	50,
	50,
	50,
	50
};

static uint8 towerArtkits[] = {
	ARTKIT_BANNER_ALLIANCE,
	ARTKIT_BANNER_HORDE,
	ARTKIT_BANNER_NEUTRAL
};

static uint32 towerOwnerWorldStates[ TF_TOWER_COUNT ][ 3 ] =
{
	{ WORLDSTATE_TF_TOWER_NW_ALLIANCE, WORLDSTATE_TF_TOWER_NW_HORDE, WORLDSTATE_TF_TOWER_NW_NEUTRAL },
	{ WORLDSTATE_TF_TOWER_N_ALLIANCE, WORLDSTATE_TF_TOWER_N_HORDE, WORLDSTATE_TF_TOWER_N_NEUTRAL },
	{ WORLDSTATE_TF_TOWER_NE_ALLIANCE, WORLDSTATE_TF_TOWER_NE_HORDE, WORLDSTATE_TF_TOWER_NE_NEUTRAL },
	{ WORLDSTATE_TF_TOWER_SE_ALLIANCE, WORLDSTATE_TF_TOWER_SE_HORDE, WORLDSTATE_TF_TOWER_SE_NEUTRAL },
	{ WORLDSTATE_TF_TOWER_S_ALLIANCE, WORLDSTATE_TF_TOWER_S_HORDE, WORLDSTATE_TF_TOWER_S_NEUTRAL },
};

class TerokkarForestBroadcaster
{
private:
	MapMgr *mgr;

public:
	TerokkarForestBroadcaster( MapMgr *mgr = NULL )
	{
		this->mgr = mgr;
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
	}

	void broadcastMessage( const char *message )
	{
		WorldPacket *packet = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL, message, 0, 0 );
		if( packet != NULL )
		{
			mgr->SendPacketToPlayersInZone( ZONE_TEROKKAR_FOREST, packet );
			delete packet;
		}
	}

	void broadcastFactionTakesControl( uint32 faction )
	{
		std::stringstream ss;

		if( faction == TEAM_ALLIANCE )
		{
			ss << "The alliance ";
		}
		else
		{
			ss << "The horde ";
		}

		ss << "has taken control of a Spirit Tower!";
		
		broadcastMessage( ss.str().c_str() );
	}

	void broadcastFactionLosesControl( uint32 faction )
	{
		std::stringstream ss;

		if( faction == TEAM_ALLIANCE )
		{
			ss << "The alliance ";
		}
		else
		{
			ss << "The horde ";
		}

		ss << "has lost control of a Spirit Tower!";
		
		broadcastMessage( ss.str().c_str() );
	}
};

class TerokkarForestPvP
{
private:
	MapMgr *mgr;

	TerokkarForestBroadcaster broadcaster;

public:
	TerokkarForestPvP( MapMgr *mgr = NULL )
	{
		mgr = NULL;
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
		broadcaster.setMapMgr( mgr );
	}

	void onTowerOwnerChange( uint32 towerId, uint32 lastOwner )
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		int32 allianceDelta = 0;
		int32 hordeDelta = 0;

		switch( towerOwners[ towerId ] )
		{
			case TEAM_ALLIANCE:
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TEAM_ALLIANCE ], 1 );
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TF_TOWER_OWNER_NEUTRAL ], 0 );

				allianceDelta++;
				break;

			case TEAM_HORDE:
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TEAM_HORDE ], 1 );
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TF_TOWER_OWNER_NEUTRAL ], 0 );

				hordeDelta++;
				break;

			case TF_TOWER_OWNER_NEUTRAL:
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ towerId ][ TF_TOWER_OWNER_NEUTRAL ], 1 );

				if( lastOwner == TEAM_ALLIANCE )
				{
					allianceDelta--;
				}
				else
				{
					hordeDelta--;
				}
				break;
		}

		uint32 allianceTowers = handler.GetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TOWERS_CONTROOLED_ALLIANCE );
		uint32 hordeTowers    = handler.GetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TOWERS_CONTROOLED_HORDE );

		allianceTowers += allianceDelta;
		hordeTowers += hordeDelta;

		handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TOWERS_CONTROOLED_ALLIANCE, allianceTowers );
		handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TOWERS_CONTROOLED_HORDE, hordeTowers );

		if( lastOwner == TF_TOWER_OWNER_NEUTRAL )
		{
			broadcaster.broadcastFactionTakesControl( towerOwners[ towerId ] );
		}
		else
		{
			broadcaster.broadcastFactionLosesControl( lastOwner );
		}
	}
};

static TerokkarForestPvP pvp;

class TerokkarSpiritTowerAI : public GameObjectAIScript
{
private:
	uint32 towerId;

public:
	TerokkarSpiritTowerAI( GameObject *go ) : GameObjectAIScript( go )
	{
		towerId = std::numeric_limits< uint32 >::max();
	}

	ADD_GAMEOBJECT_FACTORY_FUNCTION( TerokkarSpiritTowerAI );

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

		delta *= TF_TOWER_CAPTURE_PROGRESS_TICK;

		int32 progress = towerCaptureProgress[ towerId ];

		if( ( ( progress < 100 ) && ( delta > 0 ) ) ||
			( ( progress > 0 ) && ( delta < 0 ) ) )
		{
			progress += delta;
			progress = Math::clamp< int32 >( progress, 0, 100 );
			towerCaptureProgress[ towerId ] = progress;
		}
	}

	/// Calculate the current owner based on the current progress
	/// Returns true on owner change
	bool calculateOwner()
	{
		bool ownerChanged = false;
		if( towerCaptureProgress[ towerId ] == TF_TOWER_CAPTURE_THRESHOLD_ALLIANCE )
		{
			if( towerOwners[ towerId ] != TEAM_ALLIANCE )
			{
				ownerChanged = true;
				towerOwners[ towerId ] = TEAM_ALLIANCE;
			}
		}
		else
		if( towerCaptureProgress[ towerId ] <= TF_TOWER_CAPTURE_THRESHOLD_HORDE )
		{
			if( towerOwners[ towerId ] != TEAM_HORDE )
			{
				ownerChanged = true;
				towerOwners[ towerId ] = TEAM_HORDE;
			}
		}
		else
		if( ( towerCaptureProgress[ towerId ] <= TF_TOWER_CAPTURE_THRESHOLD_NEUTRAL_HI ) && ( towerCaptureProgress[ towerId ] >= TF_TOWER_CAPTURE_THRESHOLD_NEUTRAL_LO ) )
		{
			if( towerOwners[ towerId ] != TF_TOWER_OWNER_NEUTRAL )
			{
				ownerChanged = true;
				towerOwners[ towerId ] = TF_TOWER_OWNER_NEUTRAL;
			}
		}

		return ownerChanged;
	}

	void onOwnerChange( uint32 lastOwner )
	{
		pvp.onTowerOwnerChange( towerId, lastOwner );
	}

	void setArtkit()
	{
		_gameobject->SetArtKit( towerArtkits[ towerOwners[ towerId ] ] );
	}

	void OnSpawn()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}

		switch( _gameobject->GetInfo()->ID )
		{
			case GO_TEROKKAR_BANNER_NW:
				towerId = TF_TOWER_NW;
				break;

			case GO_TEROKKAR_BANNER_N:
				towerId = TF_TOWER_N;
				break;

			case GO_TEROKKAR_BANNER_NE:
				towerId = TF_TOWER_NE;
				break;

			case GO_TEROKKAR_BANNER_SE:
				towerId = TF_TOWER_SE;
				break;

			case GO_TEROKKAR_BANNER_S:
				towerId = TF_TOWER_S;
				break;
		}

		if( towerId >= TF_TOWER_COUNT )
		{
			return;
		}

		setArtkit();

		RegisterAIUpdateEvent( TF_TOWER_SCAN_UPDATE_FREQ );
	}

	void AIUpdate()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}

		std::set< Player* > playersInRange;

		uint32 alliancePlayers = 0;
		uint32 hordePlayers = 0;

		/// Find and count players in range
		std::set< Object* > players = _gameobject->GetInRangePlayers();
		for( std::set< Object* >::const_iterator itr = players.begin(); itr != players.end(); ++itr )
		{
			Player *player = static_cast< Player* >( *itr );

			float d = _gameobject->CalcDistance( player );

			if( d > TF_TOWER_SCAN_RANGE )
			{
				continue;
			}

			if( d > TF_TOWER_CAPTURE_RANGE )
			{
				/// If player is outside the capture range, turn off the capture UI
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_TF_PROGRESS_BAR_UI, 0 );
			}
			else
			{
				if( player->isAlive() && !player->IsInvisible() && !player->IsStealth() && player->IsPvPFlagged() )
				{
					if( player->GetTeam() == TEAM_ALLIANCE )
					{
						alliancePlayers++;
					}
					else
					{
						hordePlayers++;
					}
				}

				playersInRange.insert( player );
			}

			if( playersInRange.empty() )
			{
				/// No player in range, no point in calculating
				return;
			}

			calculateProgress( alliancePlayers, hordePlayers );
			uint32 lastOwner = towerOwners[ towerId ];

			bool ownerChanged = calculateOwner();
			
			/// Send progress to players fighting for control
			for( std::set< Player* >::const_iterator itr = playersInRange.begin(); itr != playersInRange.end(); ++itr )
			{
				Player *player = *itr;
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_TF_PROGRESS_BAR_UI, 1 );
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_TF_PROGRESS_BAR_PROGRESS, towerCaptureProgress[ towerId ] );
			}
			
			if( ownerChanged )
			{
				setArtkit();
				onOwnerChange( lastOwner );
			}
			
			playersInRange.clear();
		}
	}
};

void setupTerokkarForest( ScriptMgr *mgr )
{
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_OUTLAND );
	pvp.setMapMgr( mapMgr );

	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_NW, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_N, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_NE, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_SE, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_S, &TerokkarSpiritTowerAI::Create );
}

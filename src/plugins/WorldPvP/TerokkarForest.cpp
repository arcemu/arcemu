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

/// The capture points are intentionally lower than the tower banner GOs, as the banners are on top of the towers
/// We calculate the players' distance from this lowered point
static float capturePointLocations[ TF_TOWER_COUNT ][ 3 ] =
{
	{ -3081.6499f, 5335.0297f, -19.1525f },
	{ -2939.8999f, 4788.7299f, -17.6051f },
	{ -3174.9399f, 4440.9702f, -20.2437f },
	{ -3603.3100f, 4529.1499f, -15.4779f },
	{ -3812.3701f, 4899.2998f, -18.7724f }
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

static uint32 timerWorldStates[] = {
	WORLDSTATE_TF_TIMER_ALLIANCE,
	WORLDSTATE_TF_TIMER_HORDE,
	WORLDSTATE_TF_TIMER_NEUTRAL
};

#define TF_TIMER_MINUTES (6 * 60)
#define TF_TIMER_UPDATE_FREQUENCY (1 * 1000)

#define SPELL_BLESSING_OF_AUCHINDOUN 33377

/// Multiple threads can access these because of the server hooks that's why they are atomic
Arcemu::Threading::AtomicULong allianceTowersCache;
Arcemu::Threading::AtomicULong hordeTowersCache;


static uint32 isTeamSuperior( uint32 team )
{
	if( team == TEAM_ALLIANCE )
	{
		if( allianceTowersCache.GetVal() == TF_TOWER_COUNT )
		{
			return true;
		}
	}
	else
	if( team == TEAM_HORDE )
	{
		if( hordeTowersCache.GetVal() == TF_TOWER_COUNT )
		{
			return true;
		}
	}

	return false;
}

static uint32 getSuperiorTeam()
{
	if( allianceTowersCache.GetVal() == TF_TOWER_COUNT )
	{
		return TEAM_ALLIANCE;
	}
	else
	if( hordeTowersCache.GetVal() == TF_TOWER_COUNT )
	{
		return TEAM_HORDE;
	}
	else
	{
		return TF_TOWER_OWNER_NEUTRAL;
	}
}

uint32 getOwnedTowersCount( uint32 team )
{
	uint32 count = 0;

	for( int i = 0; i < TF_TOWER_COUNT; i++ )
	{
		if( towerOwners[ i ] == team )
		{
			count++;
		}
	}

	return count;
}

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

class TerokkarPvPTimer
{
private:
	uint32 minutesLeft;

	MapMgr *mgr;

	void updateWorldStates()
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		if( minutesLeft > 0 )
		{
			/// Calculate hours and minutes
			uint32 hours = minutesLeft / 60;
			uint32 minutes = minutesLeft - ( hours * 60 );
			uint32 tens = minutes / 10;
			uint32 ones = minutes - ( tens * 10 );

			uint32 superiorTeam = getSuperiorTeam();

			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, timerWorldStates[ superiorTeam ], 1 );
			
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TIMER_HOURS, hours );
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TIMER_MINUTES_1, tens );
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TIMER_MINUTES_2, ones );
		}
		else
		{
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, timerWorldStates[ getSuperiorTeam() ], 0 );

			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TIMER_HOURS, 0 );
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TIMER_MINUTES_1, 0 );
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TIMER_MINUTES_2, 0 );
		}
	}

public:
	TerokkarPvPTimer()
	{
		minutesLeft = 0;
		mgr = NULL;
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
	}

	uint32 getMinutesLeft() const
	{
		return minutesLeft;
	}

	void reset()
	{
		minutesLeft = TF_TIMER_MINUTES;

		updateWorldStates();
	}

	void countdown()
	{
		minutesLeft--;

		updateWorldStates();
	}
};

class TerokkarForestPvP : public EventableObject
{
private:
	MapMgr *mgr;

	TerokkarForestBroadcaster broadcaster;
	TerokkarPvPTimer pvpTimer;

public:
	TerokkarForestPvP( MapMgr *mgr = NULL )
	{
		mgr = NULL;
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
		broadcaster.setMapMgr( mgr );
		pvpTimer.setMapMgr( mgr );
	}

	int32 event_GetInstanceID()
	{
		return mgr->GetInstanceID();
	}

	void updateTimer()
	{
		if( pvpTimer.getMinutesLeft() > 0 )
		{
			pvpTimer.countdown();
		}
		else
		{
			/// Disable timer ticks
			sEventMgr.RemoveEvents( this, EVENT_ZONE_PVP_TIMER_UPDATE );

			/// Enable capture UI
			switchCaptureUi( true );
		}
	}

	bool isCountdownInProgress()
	{
		if( pvpTimer.getMinutesLeft() > 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void switchCaptureUi( bool on )
	{
		uint32 value;
		if( on )
		{
			value = 1;
		}
		else
		{
			 value = 0;
		}

		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		/// Towers
		for( int i = 0; i < TF_TOWER_COUNT; i++ )
		{
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, towerOwnerWorldStates[ i ][ towerOwners[ i ] ], value );
		}

		// Controlled towers count UI
		handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_TOWERS_CONTROLLED_UI, value );

		if( !on )
		{
			/// Turn off progress bar UI for everyone when switching the capture UI off
			handler.SetWorldStateForZone( ZONE_TEROKKAR_FOREST, WORLDSTATE_TF_PROGRESS_BAR_UI, 0 );
		}
	}

	void onTowerCaptured( uint32 towerId )
	{
		uint32 team =  towerOwners[ towerId ];

		broadcaster.broadcastFactionTakesControl( team );

		if( isTeamSuperior( team ) )
		{
			/// Just gained superiority
			TeamAndZoneMatcher matcher( ZONE_TEROKKAR_FOREST, team );
			CastSpellOnPlayers caster( SPELL_BLESSING_OF_AUCHINDOUN, false );
			mgr->visitPlayers( &caster, &matcher );

			/// Switch off capture UI
			switchCaptureUi( false );

			/// Star the timer, which in turn disables capturing
			pvpTimer.reset();
			sEventMgr.AddEvent( this, &TerokkarForestPvP::updateTimer, EVENT_ZONE_PVP_TIMER_UPDATE, TF_TIMER_UPDATE_FREQUENCY, 0, 0 );
		}
	}

	void onTowerLost( uint32 towerId, uint32 lastOwner )
	{
		broadcaster.broadcastFactionLosesControl( lastOwner );

		uint32 count = getOwnedTowersCount( lastOwner );
		if( count == ( TF_TOWER_COUNT - 1 ) )
		{
			/// Just lost superiority
			TeamAndZoneMatcher matcher( ZONE_TEROKKAR_FOREST, lastOwner );
			RemoveAura remover( SPELL_BLESSING_OF_AUCHINDOUN );
			mgr->visitPlayers( &remover, &matcher );
		}
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

		allianceTowersCache.SetVal( allianceTowers );
		hordeTowersCache.SetVal( hordeTowers );

		if( lastOwner == TF_TOWER_OWNER_NEUTRAL )
		{
			onTowerCaptured( towerId );
		}
		else
		{
			onTowerLost( towerId, lastOwner );
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

		switch( _gameobject->getProto()->ID )
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

		if( pvp.isCountdownInProgress() )
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

			float d = player->CalcDistance( capturePointLocations[ towerId ][ 0 ], capturePointLocations[ towerId ][ 1 ], capturePointLocations[ towerId ][ 2 ] );

			if( d > TF_TOWER_SCAN_RANGE )
			{
				continue;
			}

			if( ( d > TF_TOWER_CAPTURE_RANGE ) || ( player->flying ) )
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
};

bool isTerokkar( uint32 mapId, uint32 zoneId )
{
	if( ( mapId == MAP_OUTLAND ) && ( zoneId == ZONE_TEROKKAR_FOREST ) )
	{
		return true;
	}

	if( ( mapId == MAP_SHADOW_LABYRINTH ) || ( mapId == MAP_SETHEK_HALLS ) || ( mapId == MAP_MANA_TOMBS ) || ( mapId == MAP_AUCHENAI_CRYPTS ) )
	{
		return true;
	}

	return false;
}

void TF_onEnterWorld( Player *player )
{
	if( !isTerokkar( player->GetMapId(), player->GetZoneId() ) )
	{
		return;
	}

	if( isTeamSuperior( player->GetTeam() ) )
	{
		player->CastSpell( player, SPELL_BLESSING_OF_AUCHINDOUN, false );
	}
}

void TF_onLogout( Player *player )
{
	player->RemoveAura( SPELL_BLESSING_OF_AUCHINDOUN );
}

void TF_onZoneChange( Player *player, uint32 newZone, uint32 oldZone )
{
	if( isTerokkar( player->GetMapId(), player->GetZoneId() ) && isTeamSuperior( player->GetTeam() ) )
	{
		player->CastSpell( player, SPELL_BLESSING_OF_AUCHINDOUN, false );
	}
	else
	{
		player->RemoveAura( SPELL_BLESSING_OF_AUCHINDOUN );
	}
}

void setupTerokkarForest( ScriptMgr *mgr )
{
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_OUTLAND );
	pvp.setMapMgr( mapMgr );

	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_NW, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_N, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_NE, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_SE, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_S, &TerokkarSpiritTowerAI::Create );

	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&TF_onEnterWorld );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)&TF_onLogout );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ZONE, (void*)&TF_onZoneChange );
}

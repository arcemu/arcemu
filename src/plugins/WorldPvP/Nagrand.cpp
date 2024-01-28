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

#define ZONE_NAGRAND 3518
#define GO_HALAA_BANNER 182210
#define NAGRAND_PVP_OWNER_NEUTRAL 2
#define NAGRAND_HALAA_SCAN_FREQUENCY (2*1000)
#define HALAA_CAPTURE_RANGE 50.0f
#define HALAA_CAPTURE_MAX_ZDIFF 5.0f
#define HALAA_CAPTURE_PROGRESS_TICK 10

#define HALAA_CAPTURE_THRESHOLD_ALLIANCE 100
#define HALAA_CAPTURE_THRESHOLD_HORDE 0
#define HALAA_CAPTURE_THRESHOLD_NEUTRAL_HI 50
#define HALAA_CAPTURE_THRESHOLD_NEUTRAL_LO 50

enum NagrandWorldStates
{
	WORLDSTATE_HALAA_GUARDS_REMAINING         = 2491,
	WORLDSTATE_HALAA_GUARDS_TOTAL             = 2493,
	WORLDSTATE_HALAA_CAPTURE_PROGRESS         = 2494,
	WORLDSTATE_HALAA_CAPTURE_PROGRESS_UI      = 2495,
	WORLDSTATE_HALAA_CAPTURE_PROGRESS_NEUTRAL = 2497,

	WORLDSTATE_HALAA_GUARDS_UI_ALLIANCE       = 2502,
	WORLDSTATE_HALAA_GUARDS_UI_HORDE          = 2503,

	WORLDSTATE_HALAA_NEUTRAL                  = 2671,
	WORLDSTATE_HALAA_HORDE                    = 2672,
	WORLDSTATE_HALAA_ALLIANCE                 = 2673
};

enum HalaaBannerArtkits
{
	HALAA_BANNER_ARTKIT_HORDE    = 1,
	HALAA_BANNER_ARTKIT_ALLIANCE = 2,
	HALAA_BANNER_ARTKIT_NEUTRAL  = 21
};

static uint8 halaaBannerArtkits[ 3 ] = 
{
	HALAA_BANNER_ARTKIT_ALLIANCE,
	HALAA_BANNER_ARTKIT_HORDE,
	HALAA_BANNER_ARTKIT_NEUTRAL
};

static uint32 halaaOwner = NAGRAND_PVP_OWNER_NEUTRAL;

static uint32 halaaProgress = 50;

static uint32 halaaWorldStates[] =
{
	WORLDSTATE_HALAA_ALLIANCE,
	WORLDSTATE_HALAA_HORDE,
	WORLDSTATE_HALAA_NEUTRAL
};

static uint32 halaaGuardsWorldStates[] = 
{
	WORLDSTATE_HALAA_GUARDS_UI_ALLIANCE,
	WORLDSTATE_HALAA_GUARDS_UI_HORDE
};

class NagrandPvP
{
private:
	MapMgr *mgr;
public:
	NagrandPvP( MapMgr *mgr = NULL )
	{
		setMapMgr( mgr );
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
	}

	void updateHalaaWorldstate()
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		switch( halaaOwner )
		{
			case TEAM_ALLIANCE:
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ TEAM_ALLIANCE ], 1 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );
				
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaGuardsWorldStates[ TEAM_ALLIANCE ], 1 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaGuardsWorldStates[ TEAM_HORDE ], 0 );
				break;

			case TEAM_HORDE:
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ TEAM_HORDE ], 1 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );

				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaGuardsWorldStates[ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaGuardsWorldStates[ TEAM_HORDE ], 1 );
				break;

			case NAGRAND_PVP_OWNER_NEUTRAL:
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaWorldStates[ NAGRAND_PVP_OWNER_NEUTRAL ], 1 );

				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaGuardsWorldStates[ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, halaaGuardsWorldStates[ TEAM_HORDE ], 0 );
				break;
		}
	}

	void onHalaaBannerSpawned()
	{
		updateHalaaWorldstate();
	}

	void onHalaaOwnerChanged( uint32 lastOwner )
	{
		updateHalaaWorldstate();
	}
};

static NagrandPvP pvp;

class HalaaBannerAI : public GameObjectAIScript
{
public:
	HalaaBannerAI( GameObject *go ) : GameObjectAIScript( go )
	{
	}

	ADD_GAMEOBJECT_FACTORY_FUNCTION( HalaaBannerAI );

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

		delta *= HALAA_CAPTURE_PROGRESS_TICK;

		int32 progress = halaaProgress;

		if( ( ( progress < 100 ) && ( delta > 0 ) ) ||
			( ( progress > 0 ) && ( delta < 0 ) ) )
		{
			progress += delta;
			progress = Math::clamp< int32 >( progress, 0, 100 );
			halaaProgress = progress;
		}
	}

	/// Calculate the current owner based on the current progress
	/// Returns true on owner change
	bool calculateOwner()
	{
		bool ownerChanged = false;
		if( halaaProgress == HALAA_CAPTURE_THRESHOLD_ALLIANCE )
		{
			if( halaaOwner != TEAM_ALLIANCE )
			{
				ownerChanged = true;
				halaaOwner = TEAM_ALLIANCE;
			}
		}
		else
		if( halaaProgress <= HALAA_CAPTURE_THRESHOLD_HORDE )
		{
			if( halaaOwner != TEAM_HORDE )
			{
				ownerChanged = true;
				halaaOwner = TEAM_HORDE;
			}
		}
		else
		if( ( halaaProgress <= HALAA_CAPTURE_THRESHOLD_NEUTRAL_HI ) && ( halaaProgress >= HALAA_CAPTURE_THRESHOLD_NEUTRAL_LO ) )
		{
			if( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL )
			{
				ownerChanged = true;
				halaaOwner = NAGRAND_PVP_OWNER_NEUTRAL;
			}
		}

		return ownerChanged;
	}

	void setArtkit()
	{
		_gameobject->SetArtKit( halaaBannerArtkits[ halaaOwner ] );
	}

	void OnSpawn()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}

		setArtkit();

		pvp.onHalaaBannerSpawned();

		RegisterAIUpdateEvent( NAGRAND_HALAA_SCAN_FREQUENCY );
	}

	void onOwnerChange( uint32 lastOwner )
	{
		pvp.onHalaaOwnerChanged( lastOwner );
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
			
			float d = player->CalcDistance( _gameobject );
			float zdiff = fabs( player->GetPositionZ() - _gameobject->GetPositionZ() );

			if( ( d > HALAA_CAPTURE_RANGE ) || ( zdiff > HALAA_CAPTURE_MAX_ZDIFF ) )
			{
				/// If player is outside the capture range, turn off the capture UI
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS_UI, 0 );
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
			uint32 lastOwner = halaaOwner;

			bool ownerChanged = calculateOwner();
			
			/// Send progress to players fighting for control
			for( std::set< Player* >::const_iterator itr = playersInRange.begin(); itr != playersInRange.end(); ++itr )
			{
				Player *player = *itr;
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS_UI, 1 );
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS, halaaProgress );
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

void setupNagrand( ScriptMgr *mgr )
{
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_OUTLAND );
	pvp.setMapMgr( mapMgr );

	mgr->register_gameobject_script( GO_HALAA_BANNER, &HalaaBannerAI::Create );
}

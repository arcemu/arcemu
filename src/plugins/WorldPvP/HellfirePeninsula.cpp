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

#define HP_BANNER_SCAN_UPDATE_FREQ (2 * 1000)
#define HP_BANNER_CAPTURE_RANGE 50.0f
#define HP_BANNER_SCAN_RANGE 80.0f
#define HP_FORT_CAPTURE_PROGRESS_TICK 10

enum HPMainBanners
{
	GO_HP_BANNER_STADIUM          = 182173,
	GO_HP_BANNER_OVERLOOK         = 182174,
	GO_HP_BANNER_BROKENHILL       = 182175,

	GO_HP_BANNER_MISC_OVERLOOK    = 182525,
	GO_HP_BANNER_MISC_BROKENHILL  = 183514,
	GO_HP_BANNER_MISC_STADIUM     = 183515,
};

enum HPWorldStates
{
	WORLDSTATE_HP_STADIUM_HORDE         = 2470,
	WORLDSTATE_HP_STADIUM_ALLIANCE      = 2471,
	WORLDSTATE_HP_STADIUM_NEUTRAL       = 2472,

	WORLDSTATE_HP_PROGRESS_UI           = 2473,
	WORLDSTATE_HP_PROGRESS              = 2474,
	WORLDSTATE_HP_PROGRESS_NEUTRAL      = 2475,

	WORLDSTATE_HP_ALLIANCE_FORTS        = 2476,
	WORLDSTATE_HP_HORDE_FORTS           = 2478,

	WORLDSTATE_HP_OVERLOOK_ALLIANCE     = 2480,
	WORLDSTATE_HP_OVERLOOK_HORDE        = 2481,
	WORLDSTATE_HP_OVERLOOK_NEUTRAL      = 2482,

	WORLDSTATE_HP_BROKENHILL_ALLIANCE   = 2483,
	WORLDSTATE_HP_BROKENHILL_HORDE      = 2484,
	WORLDSTATE_HP_BROKENHILL_NEUTRAL    = 2485,
	
	WORLDSTATE_HP_HORDE_FORTS_SHOW      = 2489,
	WORLDSTATE_HP_ALLIANCE_FORST_SHOW   = 2490
};

enum HPArtkits
{
	ARTKIT_HP_BANNER_NEUTRAL       = 0,
	ARTKIT_HP_BANNER_HORDE         = 1,
	ARTKIT_HP_BANNER_ALLIANCE      = 2,

	ARTKIT_HP_OVERLOOK_HORDE       = 61,
	ARTKIT_HP_OVERLOOK_ALLIANCE    = 62,
	ARTKIT_HP_OVERLOOK_NEUTRAL     = 63,

	ARTKIT_HP_BROKENHILL_HORDE     = 64,
	ARTKIT_HP_BROKENHILL_ALLIANCE  = 65,
	ARTKIT_HP_BROKENHILL_NEUTRAL   = 66,

	ARTKIT_HP_STADIUM_ALLIANCE     = 67,
	ARTKIT_HP_STADIUM_HORDE        = 68,
	ARTKIT_HP_STADIUM_NEUTRAL      = 69
};

enum HPForts
{
	HP_FORT_OVERLOOK   = 0,
	HP_FORT_BROKENHILL = 1,
	HP_FORT_STADIUM    = 2,
	HP_FORT_COUNT      = 3
};

static uint8 fortMiscBannerArtkit[ HP_FORT_COUNT ][ 3 ] =
{
	{ ARTKIT_HP_OVERLOOK_ALLIANCE, ARTKIT_HP_OVERLOOK_HORDE, ARTKIT_HP_OVERLOOK_NEUTRAL },
	{ ARTKIT_HP_BROKENHILL_ALLIANCE, ARTKIT_HP_BROKENHILL_HORDE, ARTKIT_HP_BROKENHILL_NEUTRAL },
	{ ARTKIT_HP_STADIUM_ALLIANCE, ARTKIT_HP_STADIUM_HORDE, ARTKIT_HP_STADIUM_NEUTRAL }
};

static uint8 fortBannerArtkit[ HP_FORT_COUNT ] = 
{
	ARTKIT_HP_BANNER_ALLIANCE,
	ARTKIT_HP_BANNER_HORDE,
	ARTKIT_HP_BANNER_NEUTRAL
};

#define HP_FORT_OWNER_NEUTRAL 2

#define HP_FORT_CAPTURE_THRESHOLD_ALLIANCE    100
#define HP_FORT_CAPTURE_THRESHOLD_HORDE       0
#define HP_FORT_CAPTURE_THRESHOLD_NEUTRAL_HI  70
#define HP_FORT_CAPTURE_THRESHOLD_NEUTRAL_LO  30

static uint32 fortOwner[ HP_FORT_COUNT ] = { HP_FORT_OWNER_NEUTRAL, HP_FORT_OWNER_NEUTRAL, HP_FORT_OWNER_NEUTRAL };

static uint32 fortCaptureProgress[ HP_FORT_COUNT ] = { 50, 50, 50 };

class HPBannerAI : public GameObjectAIScript
{
private:
	uint32 fortId;

public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION( HPBannerAI );

	HPBannerAI( GameObject *go ) :
	GameObjectAIScript( go )
	{
		fortId = std::numeric_limits<uint32>::max();
	}

	void setArtKit( bool distribute )
	{
		_gameobject->SetArtKit( fortBannerArtkit[ fortOwner[ fortId ] ] );

		if( distribute )
		{
			uint32 other = 0;

			switch( fortId )
			{
				case HP_FORT_OVERLOOK:
					other = GO_HP_BANNER_MISC_OVERLOOK;
					break;

				case HP_FORT_BROKENHILL:
					other = GO_HP_BANNER_MISC_BROKENHILL;
					break;

				case HP_FORT_STADIUM:
					other = GO_HP_BANNER_MISC_STADIUM;
					break;
			}

			if( other != 0 )
			{
				std::set< Object* > &objects = _gameobject->GetInRangeObjects();
				for( std::set< Object* >::const_iterator itr = objects.begin(); itr != objects.end(); ++itr )
				{
					Object *obj = *itr;
					if( !obj->IsGameObject() )
					{
						continue;
					}
					
					GameObject *go = TO_GAMEOBJECT( obj );
					if( go->GetInfo()->ID != other )
					{
						continue;
					}
					
					go->SetArtKit( fortMiscBannerArtkit[ fortId ][ fortOwner[ fortId ] ] );
				}
			}
		}
	}

	void onOwnerChange()
	{
		setArtKit( true );
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

		delta *= HP_FORT_CAPTURE_PROGRESS_TICK;

		int32 progress = fortCaptureProgress[ fortId ];

		if( ( ( progress < 100 ) && ( delta > 0 ) ) ||
			( ( progress > 0 ) && ( delta < 0 ) ) )
		{
			progress += delta;
			progress = Math::clamp< int32 >( progress, 0, 100 );
			fortCaptureProgress[ fortId ] = progress;
		}
	}

	/// Calculate the current owner based on the current progress
	/// Returns true on owner change
	bool calculateOwner()
	{
		bool ownerChanged = false;
		if( fortCaptureProgress[ fortId ] == HP_FORT_CAPTURE_THRESHOLD_ALLIANCE )
		{
			if( fortOwner[ fortId ] != TEAM_ALLIANCE )
			{
				ownerChanged = true;
				fortOwner[ fortId ] = TEAM_ALLIANCE;
			}
		}
		else
		if( fortCaptureProgress[ fortId ] <= HP_FORT_CAPTURE_THRESHOLD_HORDE )
		{
			if( fortOwner[ fortId ] != TEAM_HORDE )
			{
				ownerChanged = true;
				fortOwner[ fortId ] = TEAM_HORDE;
			}
		}
		else
		if( ( fortCaptureProgress[ fortId ] <= HP_FORT_CAPTURE_THRESHOLD_NEUTRAL_HI ) && ( fortCaptureProgress[ fortId ] >= HP_FORT_CAPTURE_THRESHOLD_NEUTRAL_LO ) )
		{
			if( fortOwner[ fortId ] != HP_FORT_OWNER_NEUTRAL )
			{
				ownerChanged = true;
				fortOwner[ fortId ] = HP_FORT_OWNER_NEUTRAL;
			}
		}

		return ownerChanged;
	}

	void OnSpawn()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}

		switch( _gameobject->GetInfo()->ID )
		{
			case GO_HP_BANNER_STADIUM:
				fortId = HP_FORT_STADIUM;
				break;

			case GO_HP_BANNER_OVERLOOK:
				fortId = HP_FORT_OVERLOOK;
				break;

			case GO_HP_BANNER_BROKENHILL:
				fortId = HP_FORT_BROKENHILL;
				break;
		}

		if( fortId >= HP_FORT_COUNT )
		{
			return;
		}

		setArtKit( false );

		RegisterAIUpdateEvent( HP_BANNER_SCAN_UPDATE_FREQ );
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

			float d = _gameobject->CalcDistance( player );

			/// If player is outside the "scan range" then we shouldn't even care about him
			/// We need this because the forts are too close to each others, so a player can be in range of more than 1 of them
			if( d > HP_BANNER_SCAN_RANGE )
			{
				continue;
			}

			if( d > HP_BANNER_CAPTURE_RANGE )
			{
				/// If player is outside the capture range, turn off the capture UI
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HP_PROGRESS_UI, 0 );
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
		uint32 lastOwner = fortOwner[ fortId ];
		bool ownerChanged = calculateOwner();

		/// Send progress to players fighting for control
		for( std::set< Player* >::const_iterator itr = playersInRange.begin(); itr != playersInRange.end(); ++itr )
		{
			Player *player = *itr;

			Messenger::SendWorldStateUpdate( player, WORLDSTATE_HP_PROGRESS_UI, 1 );
			Messenger::SendWorldStateUpdate( player, WORLDSTATE_HP_PROGRESS, fortCaptureProgress[ fortId ] );
		}

		if( ownerChanged )
		{
			onOwnerChange();

			if( lastOwner == HP_FORT_OWNER_NEUTRAL )
			{
				//rewardPlayers( playersInRange );
			}
		}

		playersInRange.clear();
	}

};

class HPMiscBannerAI : public GameObjectAIScript
{
private:
	uint32 fortId;

public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION( HPMiscBannerAI );

	HPMiscBannerAI( GameObject *go ) :
	GameObjectAIScript( go )
	{
		fortId = std::numeric_limits<uint32>::max();
	}

	void OnSpawn()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}

		switch( _gameobject->GetInfo()->ID )
		{
			case GO_HP_BANNER_MISC_STADIUM:
				fortId = HP_FORT_STADIUM;
				break;

			case GO_HP_BANNER_MISC_OVERLOOK:
				fortId = HP_FORT_OVERLOOK;
				break;

			case GO_HP_BANNER_MISC_BROKENHILL:
				fortId = HP_FORT_BROKENHILL;
				break;
		}

		if( fortId >= HP_FORT_COUNT )
		{
			return;
		}

		_gameobject->SetArtKit( fortMiscBannerArtkit[ fortId ][ fortOwner[ fortId ] ] );
	}
};

void setupHellfirePeninsula( ScriptMgr *mgr )
{
	static uint32 bannerIds[] = { GO_HP_BANNER_STADIUM, GO_HP_BANNER_OVERLOOK, GO_HP_BANNER_BROKENHILL, 0 };
	static uint32 miscBannerIds[] = { GO_HP_BANNER_MISC_OVERLOOK, GO_HP_BANNER_MISC_STADIUM, GO_HP_BANNER_MISC_BROKENHILL, 0 };
	
	mgr->register_gameobject_script( bannerIds, &HPBannerAI::Create );
	mgr->register_gameobject_script( miscBannerIds, &HPMiscBannerAI::Create );
}

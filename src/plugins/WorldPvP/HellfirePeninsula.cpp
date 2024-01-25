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

#define ZONE_HELLFIRE_PENINSULA 3483

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

static float fortLocations[ HP_FORT_COUNT ][3] = {
	{ -184.89f, 3476.93f, 38.21f },
	{ -471.46f, 3451.09f, 34.63f },
	{ -290.02f, 3702.42f, 56.67f }
};

static const char *fortNames[ HP_FORT_COUNT ] = 
{
	"The Overlook",
	"Broken Hill",
	"The Stadium"
};

static uint32 fortWorldStates[ HP_FORT_COUNT ][ 3 ] = 
{
	{ WORLDSTATE_HP_OVERLOOK_ALLIANCE, WORLDSTATE_HP_OVERLOOK_HORDE, WORLDSTATE_HP_OVERLOOK_NEUTRAL },
	{ WORLDSTATE_HP_BROKENHILL_ALLIANCE, WORLDSTATE_HP_BROKENHILL_HORDE, WORLDSTATE_HP_BROKENHILL_NEUTRAL },
	{ WORLDSTATE_HP_STADIUM_ALLIANCE, WORLDSTATE_HP_STADIUM_HORDE, WORLDSTATE_HP_STADIUM_NEUTRAL }
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
#define HP_FORT_CAPTURE_THRESHOLD_NEUTRAL_HI  65
#define HP_FORT_CAPTURE_THRESHOLD_NEUTRAL_LO  35

static uint32 fortOwner[ HP_FORT_COUNT ] = { HP_FORT_OWNER_NEUTRAL, HP_FORT_OWNER_NEUTRAL, HP_FORT_OWNER_NEUTRAL };

static uint32 fortCaptureProgress[ HP_FORT_COUNT ] = { 50, 50, 50 };

static Arcemu::Threading::AtomicULong allianceFortsCache( 0 );
static Arcemu::Threading::AtomicULong hordeFortsCache( 0 );

enum SuperioritySpells
{
	HELLFIRE_SUPERIORITY_ALLIANCE   = 32071,
	HELLFIRE_SUPERIORITY_HORDE      = 32049
};

static uint32 superioritySpells[] = { HELLFIRE_SUPERIORITY_ALLIANCE, HELLFIRE_SUPERIORITY_HORDE };

enum HellfirePvPQuests
{
	HELLFIRE_PVP_QUEST_ALLIANCE    = 10106,
	HELLFIRE_PVP_QUEST_HORDE       = 13409
};

static uint32 hellfirePvPQuests[] = { HELLFIRE_PVP_QUEST_ALLIANCE, HELLFIRE_PVP_QUEST_HORDE };

static uint32 fortToQuestCredit[] = { 0, 2, 1 };

static uint32 markSpells[] = { 32155, 32158 };

class HellfirePeninsulaPvP
{
private:
	MapMgr *mgr;

public:
	HellfirePeninsulaPvP()
	{
		mgr = NULL;
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
	}

	/// Broadcast a message to everyone that says the tower has been captured by the faction
	void broadcastCaptureMessage( uint32 fortId )
	{
		std::string faction;
		if( fortOwner[ fortId ] == TEAM_ALLIANCE )
		{
			faction.assign( "The alliance" );
		}
		else
		{
			faction.assign( "The horde" );
		}
		
		std::stringstream ss;
		ss << faction;
		ss << " has captured " << fortNames[ fortId ] << "!";
		
		WorldPacket *packet = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL, ss.str().c_str(), 0, 0 );
		if( packet != NULL )
		{
			mgr->SendPacketToPlayersInZone( ZONE_HELLFIRE_PENINSULA, packet );
			delete packet;
		}
	}

	void onFortCaptured( uint32 fortId )
	{
		broadcastCaptureMessage( fortId );
	}

	/// Applies of removes the reward buff for controlling all forts
	void handleBuff()
	{
		uint32 allianceForts = allianceFortsCache.GetVal();
		uint32 hordeForts = hordeFortsCache.GetVal();

		if( allianceForts == HP_FORT_COUNT )
		{
			TeamAndZoneMatcher matcher( ZONE_HELLFIRE_PENINSULA, TEAM_ALLIANCE );
			CastSpellOnPlayers caster( HELLFIRE_SUPERIORITY_ALLIANCE, true );
			mgr->visitPlayers( &caster, &matcher );
		}
		else
		if( hordeForts == HP_FORT_COUNT )
		{
			TeamAndZoneMatcher matcher( ZONE_HELLFIRE_PENINSULA, TEAM_HORDE );
			CastSpellOnPlayers caster( HELLFIRE_SUPERIORITY_HORDE, true );
			mgr->visitPlayers( &caster, &matcher );
		}
		else
		{
			TeamAndZoneMatcher allianceMatcher( ZONE_HELLFIRE_PENINSULA, TEAM_ALLIANCE );
			RemoveAura allianceRemover( HELLFIRE_SUPERIORITY_ALLIANCE );
			mgr->visitPlayers( &allianceRemover, &allianceMatcher );
						
			
			TeamAndZoneMatcher hordeMatcher( ZONE_HELLFIRE_PENINSULA, TEAM_HORDE );
			RemoveAura hordeRemover( HELLFIRE_SUPERIORITY_HORDE );
			mgr->visitPlayers( &hordeRemover, &hordeMatcher );
		}
	}

	void onFortOwnerChange( uint32 fortId, uint32 lastOwner )
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();
		
		/// Set worldstate for forts
		switch( fortOwner[ fortId ] )
		{
			case TEAM_ALLIANCE:
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ TEAM_ALLIANCE ], 1 );
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ HP_FORT_OWNER_NEUTRAL ], 0 );
				break;

			case TEAM_HORDE:
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ TEAM_HORDE ], 1 );
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ HP_FORT_OWNER_NEUTRAL ], 0 );
				break;

			case HP_FORT_OWNER_NEUTRAL:
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, fortWorldStates[ fortId ][ HP_FORT_OWNER_NEUTRAL ], 1 );
				break;
		}

		
		/// Set worldstate for fort counts
		int32 allianceDelta = 0;
		int32 hordeDelta = 0;

		switch( lastOwner )
		{
			case TEAM_ALLIANCE:
				allianceDelta = -1;
				break;

			case TEAM_HORDE:
				hordeDelta = -1;
				break;

			case HP_FORT_OWNER_NEUTRAL:
				if( fortOwner[ fortId ] == TEAM_ALLIANCE )
				{
					allianceDelta = 1;
				}
				else
				{
					hordeDelta = 1;
				}
				break;
		}

		if( ( allianceDelta == 1 ) || ( hordeDelta == 1 ) )
		{
			onFortCaptured( fortId );
		}

		uint32 allianceForts = handler.GetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, WORLDSTATE_HP_ALLIANCE_FORTS );
		uint32 hordeForts = handler.GetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, WORLDSTATE_HP_HORDE_FORTS );

		allianceForts += allianceDelta;
		hordeForts += hordeDelta;

		handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, WORLDSTATE_HP_ALLIANCE_FORTS, allianceForts );
		handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, WORLDSTATE_HP_HORDE_FORTS, hordeForts );

		allianceFortsCache.SetVal( allianceForts );
		hordeFortsCache.SetVal( hordeForts );

		handleBuff();
	}
};

static HellfirePeninsulaPvP pvp;

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

	void onOwnerChange( uint32 lastOwner )
	{
		setArtKit( true );

		pvp.onFortOwnerChange( fortId, lastOwner );
	}

	void rewardPlayers( std::set< Player* > players )
	{
		for( std::set< Player* >::const_iterator itr = players.begin(); itr != players.end(); ++itr )
		{
			Player *player = *itr;
			player->AddQuestKill( hellfirePvPQuests[ player->GetTeam() ], fortToQuestCredit[ fortId ] );
		}
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
			onOwnerChange( lastOwner );

			if( lastOwner == HP_FORT_OWNER_NEUTRAL )
			{
				rewardPlayers( playersInRange );
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

bool isHellfirePeninsula( uint32 mapId, uint32 zone )
{
	if( mapId == MAP_OUTLAND && zone == ZONE_HELLFIRE_PENINSULA )
		return true;

	if( mapId == MAP_SHATTERED_HALLS || mapId == MAP_BLOOD_FURNACE || mapId == MAP_RAMPARTS || mapId == MAP_MAGTHERIDONS_LAIR )
		return true;

	return false;
}

static bool isTeamSuperior( uint32 team )
{
	uint32 fortsControlled = 0;

	if( team == TEAM_ALLIANCE )
	{
		fortsControlled = allianceFortsCache.GetVal();
	}
	else
	{
		fortsControlled = hordeFortsCache.GetVal();
	}

	if( fortsControlled == HP_FORT_COUNT )
	{
		return true;
	}

	return false;
}

void HP_onEnterWorld( Player *player )
{
	if( isHellfirePeninsula( player->GetMapId(), player->GetZoneId() ) && isTeamSuperior( player->GetTeam() ) )
	{
		player->CastSpell( player, superioritySpells[ player->GetTeam() ], true );
	}
}

void HP_onLogout( Player *player )
{
	player->RemoveAura( superioritySpells[ player->GetTeam() ] );
}

void HP_onZoneChange( Player *player, uint32 newZone, uint32 oldZone )
{
	if( isHellfirePeninsula( player->GetMapId(), player->GetZoneId() ) && isTeamSuperior( player->GetTeam() ) )
	{
		player->CastSpell( player, superioritySpells[ player->GetTeam() ], true );
	}
	else
	{
		player->RemoveAura( superioritySpells[ player->GetTeam() ] );
	}
}

void HP_onHonorableKill( Player *killer, Player *victim )
{
	if( ( killer->GetMapId() != MAP_OUTLAND ) && ( killer->GetZoneId() != ZONE_HELLFIRE_PENINSULA ) )
	{
		return;
	}
	
	int32 points = HonorHandler::CalculateHonorPointsForKill( killer->getLevel(), victim->getLevel() );
	if( points <= 0 )
	{
		return;
	}

	/// Are we close enough to one of the forts?
	uint32 i;
	for( i = HP_FORT_OVERLOOK; i < HP_FORT_COUNT; i++ )
	{
		float d = killer->CalcDistance( fortLocations[ i ][ 0 ], fortLocations[ i ][ 1 ], fortLocations[ i ][ 2 ] );
		if( d < HP_BANNER_CAPTURE_RANGE )
		{
			break;
		}
	}

	if( i >= HP_FORT_COUNT )
	{
		return;
	}

	killer->CastSpell( killer, markSpells[ killer->GetTeam() ], false );
}

void setupHellfirePeninsula( ScriptMgr *mgr )
{
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_OUTLAND );
	pvp.setMapMgr( mapMgr );

	WorldStatesHandler &handler = mapMgr->GetWorldStatesHandler();
	handler.SetWorldStateForZone( ZONE_HELLFIRE_PENINSULA, WORLDSTATE_HP_PROGRESS_NEUTRAL, 30 );


	static uint32 bannerIds[] = { GO_HP_BANNER_STADIUM, GO_HP_BANNER_OVERLOOK, GO_HP_BANNER_BROKENHILL, 0 };
	static uint32 miscBannerIds[] = { GO_HP_BANNER_MISC_OVERLOOK, GO_HP_BANNER_MISC_STADIUM, GO_HP_BANNER_MISC_BROKENHILL, 0 };
	
	mgr->register_gameobject_script( bannerIds, &HPBannerAI::Create );
	mgr->register_gameobject_script( miscBannerIds, &HPMiscBannerAI::Create );

	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&HP_onEnterWorld );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)&HP_onLogout );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ZONE, (void*)&HP_onZoneChange );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)&HP_onHonorableKill );
}

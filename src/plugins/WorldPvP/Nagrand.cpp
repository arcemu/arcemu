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

#define HALAA_PVP_DISTANCE 300.0f

#define HALAA_CAPTURE_THRESHOLD_ALLIANCE 100
#define HALAA_CAPTURE_THRESHOLD_HORDE 0
#define HALAA_CAPTURE_THRESHOLD_NEUTRAL_HI 50
#define HALAA_CAPTURE_THRESHOLD_NEUTRAL_LO 50

enum HalaaBannerArtkits
{
	HALAA_BANNER_ARTKIT_HORDE    = 1,
	HALAA_BANNER_ARTKIT_ALLIANCE = 2,
	HALAA_BANNER_ARTKIT_NEUTRAL  = 21
};

#define SPELL_HALAANI_BUFF 33795

static uint8 halaaBannerArtkits[ 3 ] = 
{
	HALAA_BANNER_ARTKIT_ALLIANCE,
	HALAA_BANNER_ARTKIT_HORDE,
	HALAA_BANNER_ARTKIT_NEUTRAL
};

static uint32 halaaOwner = NAGRAND_PVP_OWNER_NEUTRAL;

static Arcemu::Shared::Progress halaaProgress;

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

float halaaBannerLocation[] = { -1572.5699f, 7945.2998f, -22.4800f };

#define SPELL_HALAA_TOKEN_ALLIANCE 33005
#define SPELL_HALAA_TOKEN_HORDE    33004

uint32 halaaTokenSpells[] = {
	SPELL_HALAA_TOKEN_ALLIANCE,
	SPELL_HALAA_TOKEN_HORDE
};

enum HalaaCreatures
{
	NPC_HALAA_GUARD_HORDE             = 18192,
	NPC_HALAA_GUARD_ALLIANCE          = 18256,

	NPC_HALAA_RESEARCHER_HORDE        = 18816,
	NPC_HALAA_RESEARCHER_ALLIANCE     = 18817,

	NPC_HALAA_QUARTERMASTER_HORDE     = 18821,
	NPC_HALAA_QUARTERMASTER_ALLIANCE  = 18822,

	NPC_HALAA_BLADE_MERCHANT_HORDE    = 21474,
	NPC_HALAA_AMMO_VENDOR_HORDE       = 21483,
	NPC_HALAA_FOOD_VENDOR_HORDE       = 21484,
	NPC_HALAA_BLADE_MERCHANT_ALLIANCE = 21485,
	NPC_HALAA_FOOD_VENDOR_ALLIANCE    = 21487,
	NPC_HALAA_AMMO_VENDOR_ALLIANCE    = 21488
};


static uint32 halaaGuardNpcs[] = {
	NPC_HALAA_GUARD_ALLIANCE,
	NPC_HALAA_GUARD_HORDE
};

static uint32 halaaGuardFactions[] = 
{
	1802,
	1801
};

static uint32 halaaGuardEquipments[ 2 ][ 3 ] = 
{
	{ 27407, 24331, 2551 },
	{ 27405, 27406, 5259 }
};

#define HALAA_GUARD_COUNT 15

static float halaaGuardLocations[ HALAA_GUARD_COUNT ][ 4 ] =
{
	{ -1594.1223f, 7861.5952f, -21.3226f, 4.4689f },
	{ -1603.1256f, 7869.5727f, -22.3387f, 4.9331f },
	{ -1590.9753f, 7875.2685f, -22.4529f, 4.3213f },
	{ -1546.7951f, 7998.5595f, -20.6859f, 0.8168f },
	{ -1540.9602f, 7994.7895f, -20.3795f, 0.9173f },
	{ -1544.4096f, 7996.3603f, -20.6171f, 0.7634f },
	{ -1485.6400f, 7899.5898f, -19.7800f, 0.8500f },
	{ -1479.8800f, 7908.3500f, -19.2800f, 4.2100f },
	{ -1652.6800f, 8000.4399f, -26.4000f, 5.2400f },
	{ -1652.0100f, 7988.3300f, -26.5599f, 3.0200f },
	{ -1584.3800f, 7996.0600f, -23.2500f, -0.1631f },
	{ -1605.6300f, 7973.7099f, -24.2400f, -0.4330f },
	{ -1551.8194f, 7964.2597f, -21.5000f, 0.5731f, },
	{ -1579.6700f, 7942.6601f, -23.0800f, -0.5723f },
	{ -1565.7572f, 7947.8593f, -22.6417f, 4.9197f }
};


#define HALAA_NPC_COUNT 5

static uint32 halaaNpcs[ HALAA_NPC_COUNT ][ 2 ] = 
{
	{ NPC_HALAA_RESEARCHER_ALLIANCE, NPC_HALAA_RESEARCHER_HORDE },
	{ NPC_HALAA_QUARTERMASTER_ALLIANCE, NPC_HALAA_QUARTERMASTER_HORDE },
	{ NPC_HALAA_FOOD_VENDOR_ALLIANCE, NPC_HALAA_FOOD_VENDOR_HORDE },
	{ NPC_HALAA_BLADE_MERCHANT_ALLIANCE, NPC_HALAA_BLADE_MERCHANT_HORDE },
	{ NPC_HALAA_AMMO_VENDOR_ALLIANCE, NPC_HALAA_AMMO_VENDOR_HORDE }
};

static float halaaNpcLocations[ HALAA_NPC_COUNT ][ 4 ] = 
{
	{ -1591.1800f, 8020.3901f, -22.20f, 4.59f },
	{ -1588.0000f, 8019.0000f, -22.20f, 4.07f },
	{ -1524.8399f, 7930.3398f, -20.18f, 3.64f },
	{ -1520.1400f, 7927.1098f, -20.25f, 3.39f },
	{ -1570.0100f, 7993.7998f, -22.45f, 5.03f }
};

static uint32 halaaGuards = 0;

static float graveyardLocation[] = { -1654.3730f, 7938.9941f, -46.2350f };

enum WyvernCamps
{
	HALAA_WYVERN_CAMP_NE    = 0,
	HALAA_WYVERN_CAMP_SE    = 1,
	HALAA_WYVERN_CAMP_SW    = 2,
	HALAA_WYVERN_CAMP_NW    = 3,

	HALAA_WYVERN_CAMP_COUNT = 4
};

static uint32 wyvernCampWorldStates[ HALAA_WYVERN_CAMP_COUNT ][ 3 ] =
{
	{ WORLDSTATE_HALAA_WYVERN_CAMP_NE_ALLIANCE, WORLDSTATE_HALAA_WYVERN_CAMP_NE_HORDE, WORLDSTATE_HALAA_WYVERN_CAMP_NE_NEUTRAL },
	{ WORLDSTATE_HALAA_WYVERN_CAMP_SE_ALLIANCE, WORLDSTATE_HALAA_WYVERN_CAMP_SE_HORDE, WORLDSTATE_HALAA_WYVERN_CAMP_SE_NEUTRAL },
	{ WORLDSTATE_HALAA_WYVERN_CAMP_SW_ALLIANCE, WORLDSTATE_HALAA_WYVERN_CAMP_SW_HORDE, WORLDSTATE_HALAA_WYVERN_CAMP_SW_NEUTRAL },
	{ WORLDSTATE_HALAA_WYVERN_CAMP_NW_ALLIANCE, WORLDSTATE_HALAA_WYVERN_CAMP_NW_HORDE, WORLDSTATE_HALAA_WYVERN_CAMP_NW_NEUTRAL }
};

static uint32 wyvernCampOwners[ HALAA_WYVERN_CAMP_COUNT ] = 
{
	NAGRAND_PVP_OWNER_NEUTRAL,
	NAGRAND_PVP_OWNER_NEUTRAL,
	NAGRAND_PVP_OWNER_NEUTRAL,
	NAGRAND_PVP_OWNER_NEUTRAL
};

enum DestroyedRoostGOIds
{
	GO_DESTROYED_WYVERN_ROOST_NE_A     = 182276,
	GO_DESTROYED_WYVERN_ROOST_NE_H     = 182299,

	GO_DESTROYED_WYVERN_ROOST_SE_A     = 182277,
	GO_DESTROYED_WYVERN_ROOST_SE_H     = 182300,

	GO_DESTROYED_WYVERN_ROOST_SW_A     = 182266,
	GO_DESTROYED_WYVERN_ROOST_SW_H     = 182297,

	GO_DESTROYED_WYVERN_ROOST_NW_A     = 182275,
	GO_DESTROYED_WYVERN_ROOST_NW_H     = 182298
};

static uint32 destroyedRoostIds[ HALAA_WYVERN_CAMP_COUNT ][ 2 ] = 
{
	{ GO_DESTROYED_WYVERN_ROOST_NE_A, GO_DESTROYED_WYVERN_ROOST_NE_H },
	{ GO_DESTROYED_WYVERN_ROOST_SE_A, GO_DESTROYED_WYVERN_ROOST_SE_H },
	{ GO_DESTROYED_WYVERN_ROOST_SW_A, GO_DESTROYED_WYVERN_ROOST_SW_H },
	{ GO_DESTROYED_WYVERN_ROOST_NW_A, GO_DESTROYED_WYVERN_ROOST_NW_H },
};

static float destroyedRoostLocations[ HALAA_WYVERN_CAMP_COUNT ][ 4 ] =
{
	{ -1384.53f, 7779.4f, -11.17f, -0.58f },
	{ -1650.28f, 7732.19f, -15.44f, -2.81f },
	{ -1815.8f, 8036.51f, -26.24f, -2.9f },
	{ -1507.9f, 8132.11f, -19.55f, -1.34f }
};

enum WyvernRoostGOIds
{
	GO_WYVERN_ROOST_NE_A  = 182281,
	GO_WYVERN_ROOST_NE_H  = 182303,

	GO_WYVERN_ROOST_SE_A  = 182282,
	GO_WYVERN_ROOST_SE_H  = 182304,

	GO_WYVERN_ROOST_SW_A  = 182267,
	GO_WYVERN_ROOST_SW_H  = 182301,

	GO_WYVERN_ROOST_NW_A  = 182280,
	GO_WYVERN_ROOST_NW_H  = 182302,
};

static uint32 wyvernRoostIds[ HALAA_WYVERN_CAMP_COUNT ][ 2 ] = 
{
	{ GO_WYVERN_ROOST_NE_A, GO_WYVERN_ROOST_NE_H },
	{ GO_WYVERN_ROOST_SE_A, GO_WYVERN_ROOST_SE_H },
	{ GO_WYVERN_ROOST_SW_A, GO_WYVERN_ROOST_SW_H },
	{ GO_WYVERN_ROOST_NW_A, GO_WYVERN_ROOST_NW_H },
};

static float wyvernRoostLocations[ HALAA_WYVERN_CAMP_COUNT ][ 4 ] =
{
	{ -1385.07f, 7779.34f, -11.21f, 0.79f },
	{ -1649.99, 7732.57f, -15.45f, -2.41f },
	{ -1815.65f, 8036.62f, -26.24f, -2.29f },
	{ -1508.3f, 8132.84f, -19.58f, 1.45f }
};

enum BombWagonGOIds
{
	GO_BOMB_WAGON_NE_A  = 182307,
	GO_BOMB_WAGON_NE_H  = 182273,

	GO_BOMB_WAGON_SE_A  = 182308,
	GO_BOMB_WAGON_SE_H  = 182274,

	GO_BOMB_WAGON_SW_A  = 182305,
	GO_BOMB_WAGON_SW_H  = 182222,

	GO_BOMB_WAGON_NW_A  = 182306,
	GO_BOMB_WAGON_NW_H  = 182272,
};

static uint32 bombwagonIds[ HALAA_WYVERN_CAMP_COUNT ][ 2 ] = 
{
	{ GO_BOMB_WAGON_NE_A, GO_BOMB_WAGON_NE_H },
	{ GO_BOMB_WAGON_SE_A, GO_BOMB_WAGON_SE_H },
	{ GO_BOMB_WAGON_SW_A, GO_BOMB_WAGON_SW_H },
	{ GO_BOMB_WAGON_NW_A, GO_BOMB_WAGON_NW_H },	
};

static float bombwagonLocations[ HALAA_WYVERN_CAMP_COUNT ][ 4 ] = 
{
	{ -1389.53f, 7782.51f, -11.63f, -1.52f },
	{ -1662.28f, 7735.0f, -15.97f, 1.88f   },
	{ -1818.48f, 8040.24f, -26.67f, 1.26f  },
	{ -1517.45f, 8140.24f, -20.18f, -2.81f }
};

static uint32 wyvernCampTaxiPaths[ HALAA_WYVERN_CAMP_COUNT ] = 
{
	522,
	524,
	520,
	523
};

#define MODEL_RIDING_WYVERN_PVP 17675
#define ITEM_FIRE_BOMB          24538

class WyvernCampHandler
{
	MapMgr *mgr;

public:
	WyvernCampHandler( MapMgr *mgr = NULL )
	{
		setMapMgr( mgr );
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
	}

	void despawnCamp( uint32 campId )
	{
		MapScriptInterface *inf = mgr->GetInterface();
		LocationVector location;

		for( uint32 team = TEAM_ALLIANCE; team < NAGRAND_PVP_OWNER_NEUTRAL; team++ )
		{
			/// Destroyed Wyvern Roost
			location.ChangeCoords( destroyedRoostLocations[ campId ][ 0 ], destroyedRoostLocations[ campId ][ 1 ], destroyedRoostLocations[ campId ][ 2 ] );
			inf->removePersistentGameObject( destroyedRoostIds[ campId ][ team ], location );

			/// Wyvern Roost
			location.ChangeCoords( wyvernRoostLocations[ campId ][ 0 ], wyvernRoostLocations[ campId ][ 1 ], wyvernRoostLocations[ campId ][ 2 ] );
			inf->removePersistentGameObject( wyvernRoostIds[ campId ][ team ], location );

			/// Bomb wagon
			location.ChangeCoords( bombwagonLocations[ campId ][ 0 ], bombwagonLocations[ campId ][ 1 ], bombwagonLocations[ campId ][ 2 ] );
			inf->removePersistentGameObject( bombwagonIds[ campId ][ team ], location );
		}
	}

	void despawnCamps()
	{
		for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
		{
			despawnCamp( i );
		}
	}

	void spawnCamp( uint32 campId )
	{
		uint32 roostTeam;
		if( halaaOwner == TEAM_ALLIANCE )
		{
			roostTeam = TEAM_HORDE;
		}
		else
		{
			roostTeam = TEAM_ALLIANCE;
		}

		LocationVector location;
		location.ChangeCoords( destroyedRoostLocations[ campId ][ 0 ], destroyedRoostLocations[ campId ][ 1 ], destroyedRoostLocations[ campId ][ 2 ], destroyedRoostLocations[ campId ][ 3 ] );
		mgr->GetInterface()->spawnPersistentGameObject( destroyedRoostIds[ campId ][ roostTeam ], location );
	}

	void spawnCamps()
	{
		for( uint32 camp = 0; camp < HALAA_WYVERN_CAMP_COUNT; camp++ )
		{
			spawnCamp( camp );
		}
	}

	void updateCamps()
	{
		if( halaaOwner == NAGRAND_PVP_OWNER_NEUTRAL )
		{
			despawnCamps();
		}
		else
		{
			spawnCamps();
		}
	}
};

class NagrandBroadcaster
{
private:
	MapMgr *mgr;

public:
	NagrandBroadcaster( MapMgr *mgr = NULL )
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
			mgr->SendPacketToPlayersInZone( ZONE_NAGRAND, packet );
			delete packet;
		}
	}

	void broadcastHalaaDefenseless()
	{
		broadcastMessage( "Halaa is now defenseless!" );
	}

	void broadcastHalaaCaptured( uint32 team )
	{
		std::stringstream ss;
		
		if( team == TEAM_ALLIANCE )
		{
			ss << "The alliance";
		}
		else
		{
			ss << "The horde";
		}

		ss << " has taken control of Halaa!";

		broadcastMessage( ss.str().c_str() );
	}

	void broadcastHalaaLost( uint32 team )
	{
		std::stringstream ss;
		
		if( team == TEAM_ALLIANCE )
		{
			ss << "The alliance";
		}
		else
		{
			ss << "The horde";
		}

		ss << " has lost control of Halaa!";

		broadcastMessage( ss.str().c_str() );
	}
};

class NagrandPvP
{
private:
	MapMgr *mgr;
	NagrandBroadcaster broadcaster;
	WyvernCampHandler campHandler;

public:
	NagrandPvP( MapMgr *mgr = NULL )
	{
		setMapMgr( mgr );
	}

	void setMapMgr( MapMgr *mgr )
	{
		this->mgr = mgr;
		broadcaster.setMapMgr( mgr );
		campHandler.setMapMgr( mgr );
	}

	void updateWyvernCampWorldState( uint32 campId )
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		switch( wyvernCampOwners[ campId ] )
		{
			case TEAM_ALLIANCE:
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ TEAM_ALLIANCE ], 1 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );
				break;

			case TEAM_HORDE:
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ TEAM_HORDE ], 1 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );
				break;

			case NAGRAND_PVP_OWNER_NEUTRAL:
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ campId ][ NAGRAND_PVP_OWNER_NEUTRAL ], 1 );
				break;
		}

	}

	void updateWyvernCampWorldStates()
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		if( halaaOwner == NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
			{
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_ALLIANCE ], 0 );				
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_HORDE ], 0 );
				handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );
			}
		}
		else
		{
			for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
			{
				switch( wyvernCampOwners[ i ] )
				{
					case TEAM_ALLIANCE:
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_ALLIANCE ], 1 );
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_HORDE ], 0 );
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );
						break;

					case TEAM_HORDE:
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_ALLIANCE ], 0 );
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_HORDE ], 1 );
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ NAGRAND_PVP_OWNER_NEUTRAL ], 0 );
						break;

					case NAGRAND_PVP_OWNER_NEUTRAL:
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_ALLIANCE ], 0 );
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ TEAM_HORDE ], 0 );
						handler.SetWorldStateForZone( ZONE_NAGRAND, wyvernCampWorldStates[ i ][ NAGRAND_PVP_OWNER_NEUTRAL ], 1 );
						break;
				}
			}			
		}
	}

	void updateWyvernCamps()
	{
		for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
		{
			wyvernCampOwners[ i ] = NAGRAND_PVP_OWNER_NEUTRAL;
		}

		campHandler.updateCamps();

		updateWyvernCampWorldStates();
	}

	void updateHalaaWorldstates()
	{
		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();

		if( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			handler.SetWorldStateForZone( ZONE_NAGRAND, WORLDSTATE_HALAA_GUARDS_REMAINING, halaaGuards );
			handler.SetWorldStateForZone( ZONE_NAGRAND, WORLDSTATE_HALAA_GUARDS_TOTAL, HALAA_GUARD_COUNT );
		}

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

	void onWyvernCampCaptured( uint32 campId, uint32 team )
	{
		wyvernCampOwners[ campId ] = team;
		
		updateWyvernCampWorldState( campId );
	}

	void onWyvernCampDestroyed( uint32 campId )
	{
		wyvernCampOwners[ campId ] = NAGRAND_PVP_OWNER_NEUTRAL;

		campHandler.despawnCamp( campId );
		
		campHandler.spawnCamp( campId );

		updateWyvernCampWorldState( campId );
	}

	void onGuardDied()
	{
		if( halaaGuards == 0 )
		{
			return;
		}

		halaaGuards--;

		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();
		handler.SetWorldStateForZone( ZONE_NAGRAND, WORLDSTATE_HALAA_GUARDS_REMAINING, halaaGuards );

		if( halaaGuards == 0 )
		{
			broadcaster.broadcastHalaaDefenseless();
		}
	}

	void handleNpcs( uint32 lastOwner )
	{
		MapScriptInterface *inf = mgr->GetInterface();
		LocationVector location;

		/// Remove old NPCs if applicable
		if( lastOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( int i = 0; i < HALAA_NPC_COUNT; i++ )
			{
				location.ChangeCoords( halaaNpcLocations[ i ][ 0 ], halaaNpcLocations[ i ][ 1 ], halaaNpcLocations[ i ][ 2 ] );
				inf->removePersistentCreature( halaaNpcs[ i ][ lastOwner ], location );
			}
		}

		/// Spawn new NPCs if applicable
		if( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( int i = 0; i < HALAA_NPC_COUNT; i++ )
			{
				location.ChangeCoords( halaaNpcLocations[ i ][ 0 ], halaaNpcLocations[ i ][ 1 ], halaaNpcLocations[ i ][ 2 ], halaaNpcLocations[ i ][ 3 ] );
				inf->spawnPersistentCreature( halaaNpcs[ i ][ halaaOwner ], location );
			}
		}
	}

	void handleGuards( uint32 lastOwner )
	{
		MapScriptInterface *inf = mgr->GetInterface();
		LocationVector location;


		/// Remove old guards if applicable
		if( lastOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( int i = 0; i < HALAA_GUARD_COUNT; i++ )
			{
				location.ChangeCoords( halaaGuardLocations[ i ][ 0 ], halaaGuardLocations[ i ][ 1 ], halaaGuardLocations[ i ][ 2 ], halaaGuardLocations[ i ][ 3 ] );
				inf->removePersistentCreature( halaaGuardNpcs[ lastOwner ], location );
			}
		}

		/// Spawn new guards if applicable
		if( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( int i = 0; i < HALAA_GUARD_COUNT; i++ )
			{
				location.ChangeCoords( halaaGuardLocations[ i ][ 0 ], halaaGuardLocations[ i ][ 1 ], halaaGuardLocations[ i ][ 2 ], halaaGuardLocations[ i ][ 3 ] );
				inf->spawnPersistentCreature( halaaGuardNpcs[ halaaOwner ], location );
			}

			halaaGuards = HALAA_GUARD_COUNT;
			WorldStatesHandler &handler = mgr->GetWorldStatesHandler();
			handler.SetWorldStateForZone( ZONE_NAGRAND, WORLDSTATE_HALAA_GUARDS_REMAINING, halaaGuards );
		}
	}

	void onHalaaBannerSpawned()
	{
		updateHalaaWorldstates();
	}

	void onHalaaCaptured()
	{
		uint32 otherTeam;
		if( halaaOwner == TEAM_ALLIANCE )
		{
			otherTeam = TEAM_HORDE;
		}
		else
		{
			otherTeam = TEAM_ALLIANCE;
		}

		/// Remove the buff from the other team
		TeamAndZoneMatcher removeMatcher( ZONE_NAGRAND, otherTeam );
		RemoveAura remove( SPELL_HALAANI_BUFF );
		mgr->visitPlayers( &remove, &removeMatcher );

		/// Add the buff to the halaa owner team
		TeamAndZoneMatcher addMatcher( ZONE_NAGRAND, halaaOwner );
		CastSpellOnPlayers caster( SPELL_HALAANI_BUFF, false );
		mgr->visitPlayers( &caster, &addMatcher );

		broadcaster.broadcastHalaaCaptured( halaaOwner );
	}

	void updateGraveyard()
	{
		uint32 graveyardOwner;
		if( halaaOwner == NAGRAND_PVP_OWNER_NEUTRAL )
		{
			graveyardOwner = std::numeric_limits< uint32 >::max();
		}
		else
		{
			graveyardOwner = halaaOwner;
		}

		LocationVector location( graveyardLocation[ 0 ], graveyardLocation[ 1 ], graveyardLocation[ 2 ] );
		sGraveyardService.setGraveyardOwner( MAP_OUTLAND, location, graveyardOwner );
	}

	void onHalaaOwnerChanged( uint32 lastOwner )
	{
		if( lastOwner == NAGRAND_PVP_OWNER_NEUTRAL )
		{
			onHalaaCaptured();
		}
		else
		{
			broadcaster.broadcastHalaaLost( lastOwner );
		}

		handleGuards( lastOwner );
		handleNpcs( lastOwner );

		updateGraveyard();

		updateHalaaWorldstates();
		updateWyvernCamps();
	}
};

static NagrandPvP pvp;

class HalaaGuardAI : public CreatureAIScript
{
public:
	HalaaGuardAI( Creature *creature ) : CreatureAIScript( creature )
	{
	}

	ADD_CREATURE_FACTORY_FUNCTION( HalaaGuardAI );

	void OnLoad()
	{
	}

	void OnDied( Unit *killer )
	{
		pvp.onGuardDied();
	}
};

class HalaaBannerAI : public GameObjectAIScript
{
private:
	PvPCaptureRateBonusFactorCalculator bonusFactorCalculator;

public:
	HalaaBannerAI( GameObject *go ) :
	GameObjectAIScript( go ),
	bonusFactorCalculator( Config.MainConfig.GetUIntDefault( "WorldPvP", "Halaa_BonusFactor_MaxAdvantage", 5 ) )
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

		delta = delta * NAGRAND_HALAA_SCAN_FREQUENCY * bonusFactorCalculator.calculateBonusFactor( Math::diff< uint32 >( alliancePlayers, hordePlayers ) );

		halaaProgress.advanceBy( delta );
	}

	/// Calculate the current owner based on the current progress
	/// Returns true on owner change
	bool calculateOwner()
	{
		uint32 progress = halaaProgress.getPercent();

		bool ownerChanged = false;
		if( progress == HALAA_CAPTURE_THRESHOLD_ALLIANCE )
		{
			if( halaaOwner != TEAM_ALLIANCE )
			{
				ownerChanged = true;
				halaaOwner = TEAM_ALLIANCE;
			}
		}
		else
		if( progress <= HALAA_CAPTURE_THRESHOLD_HORDE )
		{
			if( halaaOwner != TEAM_HORDE )
			{
				ownerChanged = true;
				halaaOwner = TEAM_HORDE;
			}
		}
		else
		if( ( progress <= HALAA_CAPTURE_THRESHOLD_NEUTRAL_HI ) && ( progress >= HALAA_CAPTURE_THRESHOLD_NEUTRAL_LO ) )
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
		/// Only allow capturing when there are no guards left alive
		if( halaaGuards > 0 )
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
			
			float d = player->CalcDistance( _gameobject );
			float zdiff = fabs( player->GetPositionZ() - _gameobject->GetPositionZ() );

			if( ( d > HALAA_CAPTURE_RANGE ) || ( player->flying ) )
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

		}

		if( playersInRange.empty() )
		{
			/// No player in range, no point in calculating
			return;
		}

		calculateProgress( alliancePlayers, hordePlayers );
		uint32 lastOwner = halaaOwner;

		bool ownerChanged = calculateOwner();
			
		if( ownerChanged )
		{
			setArtkit();
			onOwnerChange( lastOwner );
		}

		if( ownerChanged && ( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL ) )
		{
			/// If we'just captured Halaa, turn off the capture UI
			for( std::set< Player* >::const_iterator itr = playersInRange.begin(); itr != playersInRange.end(); ++itr )
			{
				Player *player = *itr;
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS_UI, 0 );
			}
		}
		else
		{
			/// Send progress to players fighting for control
			for( std::set< Player* >::const_iterator itr = playersInRange.begin(); itr != playersInRange.end(); ++itr )
			{
				Player *player = *itr;
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS_UI, 1 );
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS, halaaProgress.getPercent() );
			}
		}

		playersInRange.clear();
	}
};


class DestroyedWyvernRoostAI : public GameObjectAIScript
{
private:
	uint32 campId;

public:
	DestroyedWyvernRoostAI( GameObject *go ) : GameObjectAIScript( go )
	{
	}

	ADD_GAMEOBJECT_FACTORY_FUNCTION( DestroyedWyvernRoostAI );

	void OnSpawn()
	{
		switch( _gameobject->getProto()->ID )
		{
			case GO_DESTROYED_WYVERN_ROOST_NE_A:
			case GO_DESTROYED_WYVERN_ROOST_NE_H:
				campId = HALAA_WYVERN_CAMP_NE;
				break;

			case GO_DESTROYED_WYVERN_ROOST_SE_A:
			case GO_DESTROYED_WYVERN_ROOST_SE_H:
				campId = HALAA_WYVERN_CAMP_SE;
				break;

			case GO_DESTROYED_WYVERN_ROOST_SW_A:
			case GO_DESTROYED_WYVERN_ROOST_SW_H:
				campId = HALAA_WYVERN_CAMP_SW;
				break;

			case GO_DESTROYED_WYVERN_ROOST_NW_A:
			case GO_DESTROYED_WYVERN_ROOST_NW_H:
				campId = HALAA_WYVERN_CAMP_NW;
				break;
		}
	}

	void OnActivate( Player* player )
	{
		MapMgr *mgr = _gameobject->GetMapMgr();
		uint32 team = player->GetTeam();

		uint32 enemyTeam;
		if( team == TEAM_ALLIANCE )
		{
			enemyTeam = TEAM_HORDE;
		}
		else
		{
			enemyTeam = TEAM_ALLIANCE;
		}

		MapScriptInterface *inf = mgr->GetInterface();
		LocationVector location;

		location.ChangeCoords( wyvernRoostLocations[ campId ][ 0 ], wyvernRoostLocations[ campId ][ 1 ], wyvernRoostLocations[ campId ][ 2 ], wyvernRoostLocations[ campId ][ 3 ] );
		inf->spawnPersistentGameObject( wyvernRoostIds[ campId ][ team ], location );

		location.ChangeCoords( bombwagonLocations[ campId ][ 0 ], bombwagonLocations[ campId ][ 1 ], bombwagonLocations[ campId ][ 2 ], bombwagonLocations[ campId ][ 3 ] );
		inf->spawnPersistentGameObject( bombwagonIds[ campId ][ team ], location );

		location = _gameobject->GetPosition();
		inf->removePersistentGameObject( _gameobject->getProto()->ID, location );

		pvp.onWyvernCampCaptured( campId, team );
	}
};

class WyvernRoostAI : public GameObjectAIScript
{
private:
	uint32 campId;

public:
	WyvernRoostAI( GameObject *go ) : GameObjectAIScript( go )
	{
	}

	ADD_GAMEOBJECT_FACTORY_FUNCTION( WyvernRoostAI );

	void OnSpawn()
	{
		switch( _gameobject->getProto()->ID )
		{
			case GO_WYVERN_ROOST_NE_A:
			case GO_WYVERN_ROOST_NE_H:
				campId = HALAA_WYVERN_CAMP_NE;
				break;

			case GO_WYVERN_ROOST_SE_A:
			case GO_WYVERN_ROOST_SE_H:
				campId = HALAA_WYVERN_CAMP_SE;
				break;

			case GO_WYVERN_ROOST_SW_A:
			case GO_WYVERN_ROOST_SW_H:
				campId = HALAA_WYVERN_CAMP_SW;
				break;

			case GO_WYVERN_ROOST_NW_A:
			case GO_WYVERN_ROOST_NW_H:
				campId = HALAA_WYVERN_CAMP_NW;
				break;
		}
	}

	void OnActivate( Player *player )
	{
		uint32 taxiPathId = wyvernCampTaxiPaths[ campId ];
		
		TaxiPath *path = sTaxiMgr.GetTaxiPath( taxiPathId );
		if( path != NULL )
		{
			if( player->GetItemInterface()->GetItemCount( ITEM_FIRE_BOMB ) == 0 )
			{
				player->GetItemInterface()->AddItemById( ITEM_FIRE_BOMB, 10, 0 );
			}

			player->TaxiStart( path, MODEL_RIDING_WYVERN_PVP, 0 );
		}
	}
};

class BombWagonAI : public GameObjectAIScript
{
private:
	uint32 campId;

public:
	BombWagonAI( GameObject *go ) : GameObjectAIScript( go )
	{
	}

	ADD_GAMEOBJECT_FACTORY_FUNCTION( BombWagonAI );

	void OnSpawn()
	{
		switch( _gameobject->getProto()->ID )
		{
			case GO_BOMB_WAGON_NE_A:
			case GO_BOMB_WAGON_NE_H:
				campId = HALAA_WYVERN_CAMP_NE;
				break;

			case GO_BOMB_WAGON_SE_A:
			case GO_BOMB_WAGON_SE_H:
				campId = HALAA_WYVERN_CAMP_SE;
				break;

			case GO_BOMB_WAGON_SW_A:
			case GO_BOMB_WAGON_SW_H:
				campId = HALAA_WYVERN_CAMP_SW;
				break;

			case GO_BOMB_WAGON_NW_A:
			case GO_BOMB_WAGON_NW_H:
				campId = HALAA_WYVERN_CAMP_NW;
				break;
		}
	}

	void OnActivate( Player *player )
	{
		pvp.onWyvernCampDestroyed( campId );
	}
};

void Nagrand_onEnterWorld( Player *player )
{
	if( player->GetMapId() != MAP_OUTLAND )
	{
		return;
	}

	if( player->GetZoneId() != ZONE_NAGRAND )
	{
		return;
	}

	if( player->GetTeam() == halaaOwner )
	{
		player->CastSpell( player, SPELL_HALAANI_BUFF, false );
	}
}

void Nagrand_onLogout( Player *player )
{
	player->RemoveAura( SPELL_HALAANI_BUFF );
}

void Nagrand_onZoneChange( Player *player, uint32 newZone, uint32 oldZone )
{
	if( ( player->GetMapId() == MAP_OUTLAND ) && ( player->GetZoneId() == ZONE_NAGRAND ) && ( player->GetTeam() == halaaOwner ) )
	{
		player->CastSpell( player, SPELL_HALAANI_BUFF, false );
	}
	else
	{
		player->RemoveAura( SPELL_HALAANI_BUFF );
	}
}

void Nagrand_onHonorableKill( Player *killer, Player *victim )
{
	if( ( killer->GetMapId() != MAP_OUTLAND ) && ( killer->GetZoneId() != ZONE_NAGRAND ) )
	{
		return;
	}

	int32 points = HonorHandler::CalculateHonorPointsForKill( killer->getLevel(), victim->getLevel() );
	if( points <= 0 )
	{
		return;
	}

	// Are we close enough to Halaa
	float d = killer->CalcDistance( halaaBannerLocation[ 0 ], halaaBannerLocation[ 1 ], halaaBannerLocation[ 2 ] );
	if( d > HALAA_PVP_DISTANCE )
	{
		return;
	}

	killer->CastSpell( killer, halaaTokenSpells[ killer->GetTeam() ], false );
}

void setupNagrand( ScriptMgr *mgr )
{
	halaaProgress.setMax( Config.MainConfig.GetUIntDefault( "WorldPvP", "Halaa_CaptureTime", 120 ) * 1000 );
	halaaProgress.setPercent( 50 );

	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_OUTLAND );
	pvp.setMapMgr( mapMgr );

	mgr->register_gameobject_script( GO_HALAA_BANNER, &HalaaBannerAI::Create );

	mgr->register_creature_script( NPC_HALAA_GUARD_ALLIANCE, &HalaaGuardAI::Create );
	mgr->register_creature_script( NPC_HALAA_GUARD_HORDE, &HalaaGuardAI::Create );

	for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
	{
		for( uint32 j = 0; j < NAGRAND_PVP_OWNER_NEUTRAL; j++ )
		{
			mgr->register_gameobject_script( destroyedRoostIds[ i ][ j ], &DestroyedWyvernRoostAI::Create );
			mgr->register_gameobject_script( wyvernRoostIds[ i ][ j ], &WyvernRoostAI::Create );
			mgr->register_gameobject_script( bombwagonIds[ i ][ j ], &BombWagonAI::Create );
		}
	}

	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&Nagrand_onEnterWorld );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)&Nagrand_onLogout );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ZONE, (void*)&Nagrand_onZoneChange );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)&Nagrand_onHonorableKill );

	/// No one should be able to use the graveyard unless the city is captured
	LocationVector location( graveyardLocation[ 0 ], graveyardLocation[ 1 ], graveyardLocation[ 2 ] );
	sGraveyardService.setGraveyardOwner( MAP_OUTLAND, location, std::numeric_limits< uint32 >::max() );
}

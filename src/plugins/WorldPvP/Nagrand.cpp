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

#define HALAA_PVP_DISTANCE 300.0f

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

	WORLDSTATE_HALAA_WYVERN_CAMP_SE_NEUTRAL   = 2659,
	WORLDSTATE_HALAA_WYVERN_CAMP_SE_HORDE     = 2660,
	WORLDSTATE_HALAA_WYVERN_CAMP_SE_ALLIANCE  = 2661,
	WORLDSTATE_HALAA_WYVERN_CAMP_NE_NEUTRAL   = 2662,
	WORLDSTATE_HALAA_WYVERN_CAMP_NE_HORDE     = 2663,
	WORLDSTATE_HALAA_WYVERN_CAMP_NE_ALLIANCE  = 2664,
	WORLDSTATE_HALAA_WYVERN_CAMP_NW_HORDE     = 2665,
	WORLDSTATE_HALAA_WYVERN_CAMP_NW_ALLIANCE  = 2666,
	WORLDSTATE_HALAA_WYVERN_CAMP_NW_NEUTRAL   = 2667,
	WORLDSTATE_HALAA_WYVERN_CAMP_SW_HORDE     = 2668,
	WORLDSTATE_HALAA_WYVERN_CAMP_SW_ALLIANCE  = 2669,
	WORLDSTATE_HALAA_WYVERN_CAMP_SW_NEUTRAL   = 2670,

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

#define SPELL_HALAANI_BUFF 33795

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

enum RoostGOIds
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

static uint32 roostIds[ HALAA_WYVERN_CAMP_COUNT ][ 2 ] = 
{
	{ GO_DESTROYED_WYVERN_ROOST_NE_A, GO_DESTROYED_WYVERN_ROOST_NE_H },
	{ GO_DESTROYED_WYVERN_ROOST_SE_A, GO_DESTROYED_WYVERN_ROOST_SE_H },
	{ GO_DESTROYED_WYVERN_ROOST_SW_A, GO_DESTROYED_WYVERN_ROOST_SW_H },
	{ GO_DESTROYED_WYVERN_ROOST_NW_A, GO_DESTROYED_WYVERN_ROOST_NW_H },
};

static float roostLocations[ HALAA_WYVERN_CAMP_COUNT ][ 4 ] =
{
	{ -1384.53f, 7779.4f, -11.17f, -0.58f },
	{ -1650.28f, 7732.19f, -15.44f, -2.81f },
	{ -1815.8f, 8036.51f, -26.24f, -2.9f },
	{ -1507.9f, 8132.11f, -19.55f, -1.34f }
};

static uint32 roostFactions[ 2 ] = { 84, 83 };

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

	void despawnCamps()
	{
		for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
		{
			GameObject *go;

			for( uint32 team = TEAM_ALLIANCE; team < NAGRAND_PVP_OWNER_NEUTRAL; team++ )
			{
				go = mgr->GetInterface()->GetGameObjectNearestCoords(
					roostLocations[ i ][ 0 ], roostLocations[ i ][ 1 ], roostLocations[ i ][ 2 ],
					roostIds[ i ][ team ] );
				
				if( go != NULL )
				{
					go->Despawn( 1, 0 );
				}
			}
		}
	}

	void spawnCamps()
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

		for( uint32 i = 0; i < HALAA_WYVERN_CAMP_COUNT; i++ )
		{
			GameObject *go = mgr->GetInterface()->SpawnGameObject(
				roostIds[ i ][ roostTeam ],
				roostLocations[ i ][ 0 ], roostLocations[ i ][ 1 ], roostLocations[ i ][ 2 ], roostLocations[ i ][ 3 ],
				false, 0, 0 );

			go->SetFaction( roostFactions[ roostTeam ] );

			go->PushToWorld( mgr );
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

	void onGuardSpawned()
	{
		if( halaaGuards >= HALAA_GUARD_COUNT )
		{
			return;
		}

		halaaGuards++;

		WorldStatesHandler &handler = mgr->GetWorldStatesHandler();
		handler.SetWorldStateForZone( ZONE_NAGRAND, WORLDSTATE_HALAA_GUARDS_REMAINING, halaaGuards );
	}

	void handleNpcs( uint32 lastOwner )
	{
		/// Remove old NPCs if applicable
		if( lastOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( int i = 0; i < HALAA_NPC_COUNT; i++ )
			{
				Creature *creature = mgr->GetInterface()->GetCreatureNearestCoords(
					halaaNpcLocations[ i ][ 0 ], halaaNpcLocations[ i ][ 1 ], halaaNpcLocations[ i ][ 2 ],
					halaaNpcs[ i ][ lastOwner ] );

				if( creature != NULL )
				{
					creature->Despawn( 1, 0 );
				}
			}
		}

		/// Spawn new NPCs if applicable
		if( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			for( int i = 0; i < HALAA_NPC_COUNT; i++ )
			{
				Creature *creature = mgr->GetInterface()->SpawnCreature( 
					halaaNpcs[ i ][ halaaOwner ],
					halaaNpcLocations[ i ][ 0 ], halaaNpcLocations[ i ][ 1 ], halaaNpcLocations[ i ][ 2 ], halaaNpcLocations[ i ][ 3 ],
					false, false, 0, 0 );

				if( creature != NULL )
				{
					creature->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONTMOVEWP );
					creature->PushToWorld( mgr );
				}
			}
		}
	}

	void respawnGuards()
	{
		halaaGuards = HALAA_GUARD_COUNT;

		/// Spawn guards
		for( int i = 0; i < HALAA_GUARD_COUNT; i++ )
		{
			Creature *guard = mgr->GetInterface()->SpawnCreature(
				halaaGuardNpcs[ halaaOwner ],
				halaaGuardLocations[ i ][ 0 ], halaaGuardLocations[ i ][ 1 ], halaaGuardLocations[ i ][ 2 ], halaaGuardLocations[ i ][ 3 ],
				false, false, 0, 0
			);

			if( guard != NULL )
			{
				guard->SetEquippedItem( 0, halaaGuardEquipments[ halaaOwner ][ 0 ] );
				guard->SetEquippedItem( 1, halaaGuardEquipments[ halaaOwner ][ 1 ] );
				guard->SetEquippedItem( 2, halaaGuardEquipments[ halaaOwner ][ 2 ] );
				guard->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONTMOVEWP );
				guard->PushToWorld( mgr );
			}
		}
	}

	void onHalaaBannerSpawned()
	{
		if( halaaOwner != NAGRAND_PVP_OWNER_NEUTRAL )
		{
			respawnGuards();
		}

		handleNpcs( NAGRAND_PVP_OWNER_NEUTRAL );

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

		respawnGuards();

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
		pvp.onGuardSpawned();
	}

	void OnDied( Unit *killer )
	{
		pvp.onGuardDied();
	}
};

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
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_HALAA_CAPTURE_PROGRESS, halaaProgress );
			}
		}

		playersInRange.clear();
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
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_OUTLAND );
	pvp.setMapMgr( mapMgr );

	mgr->register_gameobject_script( GO_HALAA_BANNER, &HalaaBannerAI::Create );

	mgr->register_creature_script( NPC_HALAA_GUARD_ALLIANCE, &HalaaGuardAI::Create );
	mgr->register_creature_script( NPC_HALAA_GUARD_HORDE, &HalaaGuardAI::Create );

	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&Nagrand_onEnterWorld );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)&Nagrand_onLogout );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ZONE, (void*)&Nagrand_onZoneChange );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_HONORABLE_KILL, (void*)&Nagrand_onHonorableKill );

	/// No one should be able to use the graveyard unless the city is captured
	LocationVector location( graveyardLocation[ 0 ], graveyardLocation[ 1 ], graveyardLocation[ 2 ] );
	sGraveyardService.setGraveyardOwner( MAP_OUTLAND, location, std::numeric_limits< uint32 >::max() );
}

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

#define GO_EP_TOWER_BANNER_MISC         182106

#define GO_LORDAERON_SHRINE_ALLIANCE 181682
#define GO_LORDAERON_SHRINE_HORDE    181955

#define EP_TOWER_SCAN_UPDATE_FREQ (2 * 1000)
#define EP_TOWER_SCAN_RANGE 50.0f

#define EP_TOWER_NORTHPASS   0
#define EP_TOWER_CROWNGUARD  1
#define EP_TOWER_EASTWALL    2
#define EP_TOWER_PLAGUEWOOD  3
#define EP_TOWER_COUNT       4

#define EP_TOWER_ARTKIT_NEUTRAL  0
#define EP_TOWER_ARTKIT_HORDE    1
#define EP_TOWER_ARTKIT_ALLIANCE 2

#define TOWER_ALLIANCE_CONTROL 0
#define TOWER_HORDE_CONTROL    1
#define TOWER_NEUTRAL          2
#define TOWER_STATES           3

static float towerCoords[ EP_TOWER_COUNT ][3] = {
	{ 3170.212646f, -4374.623535f, 139.630493f },
	{ 1854.023926f, -3722.084229f, 162.223434f },
	{ 2563.455811f, -4796.467258f, 110.211266f },
	{ 2973.139893f, -3036.209961f, 120.296402f }
};

static const char* towerNames[] = 
{
	"Northpass Tower",
	"Crown Guard Tower",
	"Eastwall Tower",
	"Plaguewood Tower"
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

static uint32 rewardSpellIds[2][5] = 
{
	{ 0, 11413, 11414, 11415, 1386 },
	{ 0, 30880, 30683, 30682, 29520 }
};

#define REWARD_HONOR_POINTS 189

static uint32 questByTeam[] = { 9664, 9665 };
static uint32 towerToQuestCredit[] = { 2, 0, 1, 3 };

#define FACTION_HUMAN 1
#define FACTION_ORC   2

static uint32 entityFactions[] = { FACTION_HUMAN, FACTION_ORC };

static uint32 shrineIds[] = { GO_LORDAERON_SHRINE_ALLIANCE, GO_LORDAERON_SHRINE_HORDE };
static float shrineLocation[] = { 3167.417725f, -4356.077148f, 138.797272f, 4.861288f };

#define NPC_WILLIAM_KIELAR 17209
#define SPELL_SPIRITPARTCILES_BLUE 17327
#define SPELL_SPIRITPARTICLES_RED  31309

static float flightMasterLocation[] = { 2966.970703f, -3037.198730f, 120.299431f, 6.149474f };

static float spectralAura[] = { SPELL_SPIRITPARTCILES_BLUE, SPELL_SPIRITPARTICLES_RED };

#define GO_BANNER_AURAL_ALLIANCE 180421
#define GO_BANNER_AURAL_HORDE    180422

static float graveyardLocation[] = { 1978.47f, -3655.89f, 119.795f };
static float graveyardAuraLocation[] = { 1985.469971f, -3653.879883f, 120.169998f };
static float graveyardAuraObjects[] = { 180421, 180422 };

#define SPELL_LORDAERONS_BLESSING 30238

#define NPC_LORDAERON_COMMANDER 17635
#define NPC_LORDAERON_SOLDIER   17647
#define NPC_LORDAERON_VETERAN   17995
#define NPC_LORDAERON_FIGHTER   17996
#define LORDAERON_SOLDIER_COUNT 4

static uint32 commanderIds[] = { NPC_LORDAERON_COMMANDER, NPC_LORDAERON_VETERAN };
static uint32 soldierIds[] = { NPC_LORDAERON_SOLDIER, NPC_LORDAERON_FIGHTER };

static float commanderLocation[] = { 2547.371582f, -4778.284668f, 108.583450f, 2.341261f };
static float soldierLocation[ LORDAERON_SOLDIER_COUNT ][ 4 ] = {
	{ 2548.851074f, -4778.349609f, 108.879448f, 2.404879f },
	{ 2549.718994f, -4779.288086f, 109.062050f, 2.404879f },
	{ 2547.245361f, -4779.898438f, 108.677017f, 2.404879f },
	{ 2548.307861f, -4780.843750f, 108.893883f, 2.404879f }
};


#define ATTACKWAYPOINT_COUNT 13

static float attackWaypoints[ ATTACKWAYPOINT_COUNT ][ 4 ] = {
	{ 2511.72f, -4749.557f, 97.0630f, 1.9033f },
	{ 2485.5344f, -4702.3164f, 83.0896f, 0.8610f },
	{ 2481.94f, -4665.74f, 75.67f, 0.2422f },
	{ 2570.5356f, -4646.5737f, 79.6280f, 0.2120f },
	{ 2627.7416f, -4611.056f, 84.6658f, 0.549f },
	{ 2756.706f, -4530.61f, 89.08f, 1.1522f },
	{ 2855.3122f, -4424.8042f, 89.8929f, 0.8543f },
	{ 2889.6323f, -4332.642f, 90.689f, 0.2417f },
	{ 2911.6372f, -4166.3696f, 93.9438f, 5.7659f },
	{ 2988.9245f, -4220.51f, 96.9036f, 5.4035f },
	{ 3090.0373f, -4260.4512f, 99.17f, 2.28f },
	{ 3149.7045f, -4319.3281f, 130.9011f, 5.4286f },
	{ 3163.5659f, -4337.4936f, 134.3381f, 4.7336f }
};

static Arcemu::Threading::AtomicULong allianceTowersCache( 0 );
static Arcemu::Threading::AtomicULong hordeTowersCache( 0 );

class TowerEventHandler
{
public:
	TowerEventHandler( MapMgr *mapMgr )
	{
		this->mapMgr = mapMgr;
	}

	virtual void onTowerBecomesNeutral() = 0;
	virtual void onTowerCaptured() = 0;

protected:
	MapMgr *mapMgr;
};

class NorthpassTowerEventHandler : public TowerEventHandler
{
public:
	NorthpassTowerEventHandler( MapMgr *mgr ) : TowerEventHandler( mgr ){}

	void onTowerBecomesNeutral()
	{
		GameObject *go = NULL;

		for( int i = 0; i <= 1; i++ )
		{
			/// Despawn shrine
			go = mapMgr->GetInterface()->GetGameObjectNearestCoords( shrineLocation[ 0 ], shrineLocation[ 1 ], shrineLocation[ 2 ], shrineIds[ i ] );
			if( go != NULL )
			{
				uint32 aura = go->GetInfo()->Unknown3;

				go->Despawn( 1, 0 );
					
				// Despawn aura
				if( aura != 0 )
				{
					go = mapMgr->GetInterface()->GetGameObjectNearestCoords( shrineLocation[ 0 ], shrineLocation[ 1 ], shrineLocation[ 2 ], aura );
					if( go != NULL )
					{
						go->Despawn( 1, 0 );
					}
				}
			}
		}
	}

	void onTowerCaptured()
	{
		/// Spawn the correct shrine
		GameObject *go = mapMgr->GetInterface()->SpawnGameObject(
			shrineIds[ towerOwner[ EP_TOWER_NORTHPASS ] ],
			shrineLocation[ 0 ],
			shrineLocation[ 1 ],
			shrineLocation[ 2 ],
			shrineLocation[ 3 ],
			true,
			0,
			0
		);

		if( go != NULL )
		{
			/// Get the aura id from the go, and then spawn it
			go->SetFaction( entityFactions[ towerOwner[ EP_TOWER_NORTHPASS ] ] );
			uint32 aura = go->GetInfo()->Unknown3;
			if( aura != 0 )
			{
				mapMgr->GetInterface()->SpawnGameObject(
					aura,
					shrineLocation[ 0 ],
					shrineLocation[ 1 ],
					shrineLocation[ 2 ],
					shrineLocation[ 3 ],
					true,
					0,
					0
				);
			}
		}
	}
};

class PlagueWoodTowerEventHandler : public TowerEventHandler
{
public:
	PlagueWoodTowerEventHandler( MapMgr *mgr ) : TowerEventHandler( mgr ){}

	void onTowerBecomesNeutral()
	{
		Creature *flightMaster = mapMgr->GetInterface()->GetCreatureNearestCoords(
			flightMasterLocation[ 0 ],
			flightMasterLocation[ 1 ], 
			flightMasterLocation[ 2 ], 
			NPC_WILLIAM_KIELAR );

		if( flightMaster != NULL )
		{
			flightMaster->Despawn( 1, 0 );
		}
	}

	void onTowerCaptured()
	{
		Creature *flightMaster = mapMgr->GetInterface()->SpawnCreature(
			NPC_WILLIAM_KIELAR,
			flightMasterLocation[ 0 ],
			flightMasterLocation[ 1 ],
			flightMasterLocation[ 2 ],
			flightMasterLocation[ 3 ],
			false,
			false,
			0,
			0 );

		if( flightMaster != NULL )
		{
			flightMaster->SetFaction( entityFactions[ towerOwner[ EP_TOWER_PLAGUEWOOD ] ] );
			flightMaster->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONTMOVEWP );
			flightMaster->PushToWorld( mapMgr );
			flightMaster->CastSpell( flightMaster, spectralAura[ towerOwner[ EP_TOWER_PLAGUEWOOD ] ], true );
		}

	}
};

class EastwallTowerEventHandler : public TowerEventHandler
{
public:
	EastwallTowerEventHandler( MapMgr *mgr ) : TowerEventHandler( mgr ){}

	void onTowerBecomesNeutral()
	{
	}

	void onTowerCaptured()
	{
		uint32 team = towerOwner[ EP_TOWER_EASTWALL ];

		/// Spawn a commander
		Creature *commander = mapMgr->GetInterface()->SpawnCreature(
			commanderIds[ team ],
			commanderLocation[ 0 ], commanderLocation[ 1 ], commanderLocation[ 2 ], commanderLocation[ 3 ],
			true, false, 0, 0
		);
		if( commander == NULL )
		{
			return;
		}

		//commander->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONTMOVEWP );
		commander->CastSpell( commander, spectralAura[ team ], true );

		/// Spawn the 4 soldiers that follow him
		for( int i = 0; i < LORDAERON_SOLDIER_COUNT; i++ )
		{
			Creature *soldier = mapMgr->GetInterface()->SpawnCreature(
				soldierIds[ team ],
				soldierLocation[ i ][ 0 ], soldierLocation[ i ][ 1 ], soldierLocation[ i ][ 2 ], soldierLocation[ i ][ 3 ],
				true, false, 0, 0
			);

			if( soldier == NULL )
			{
				break;
			}

			soldier->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONTMOVEWP );
			soldier->GetAIInterface()->SetUnitToFollow( commander );
			
			float angle = -1.0f * ( M_PI_FLOAT / 5.0f ) * ( i + 1 );
			soldier->GetAIInterface()->SetUnitToFollowAngle( angle );
			
			soldier->GetAIInterface()->SetFollowDistance( 2.0f );
			soldier->CastSpell( soldier, spectralAura[ team ], true );
		}
	}
};

class CrownGuardTowerEventHandler : public TowerEventHandler
{
public:
	CrownGuardTowerEventHandler( MapMgr *mgr ) : TowerEventHandler( mgr ){}

	void onTowerBecomesNeutral()
	{
		/// Crownguard Tower graveyard belongs to no one by default
		LocationVector location( graveyardLocation[ 0 ], graveyardLocation[ 1 ], graveyardLocation[ 2 ] );

		sGraveyardService.setGraveyardOwner(
			MAP_EASTERN_KINGDOMS,
			location,
			std::numeric_limits<uint32>::max()
		);

		/// Remove banner aura from the graveyard
		for( int i = 0; i < 2; i++ )
		{
			GameObject *go = mapMgr->GetInterface()->GetGameObjectNearestCoords(
				graveyardAuraLocation[ 0 ], graveyardAuraLocation[ 1 ], graveyardAuraLocation[ 2 ],
				graveyardAuraObjects[ i ]
			);

			if( go != NULL )
			{
				go->Despawn( 1, 0 );
			}
		}
	}

	void onTowerCaptured()
	{
		/// Crownguard Tower graveyard given to the faction who owns the tower
		LocationVector location( graveyardLocation[ 0 ], graveyardLocation[ 1 ], graveyardLocation[ 2 ] );

		sGraveyardService.setGraveyardOwner(
			MAP_EASTERN_KINGDOMS,
			location,
			towerOwner[ EP_TOWER_CROWNGUARD ]
		);

		/// Spawn the banner aura in the graveyard
		GameObject *go = mapMgr->GetInterface()->SpawnGameObject(
			graveyardAuraObjects[ towerOwner[ EP_TOWER_CROWNGUARD ] ],
			graveyardAuraLocation[ 0 ], graveyardAuraLocation[ 1 ], graveyardAuraLocation[ 2 ], 0.0f,
			true, 0, 0 );
	}
};

class EasternPlaguelandsPvP
{
private:
	MapMgr *mapMgr;
	std::map< uint32, TowerEventHandler* > towerEventHandlers;

public:
	EasternPlaguelandsPvP()
	{
		mapMgr = NULL;
	}

	~EasternPlaguelandsPvP()
	{
		for( std::map< uint32, TowerEventHandler* >::iterator itr = towerEventHandlers.begin(); itr != towerEventHandlers.end(); ++itr )
		{
			delete itr->second;
			itr->second = NULL;
		}

		towerEventHandlers.clear();

		mapMgr = NULL;
	}

	void registerTowerEventHandler( uint32 towerId, TowerEventHandler *handler )
	{
		towerEventHandlers[ towerId ] = handler;
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

	/// Retrieves the right spell id for the reward spell for the specified team
	static uint32 getSpellForTeam( uint32 team )
	{
		uint32 towerCount;

		if( team == TEAM_ALLIANCE )
			towerCount = allianceTowersCache.GetVal();
		else
			towerCount = hordeTowersCache.GetVal();

		uint32 spell = rewardSpellIds[ team ][ towerCount ];
		return spell;
	}

	/// Event handler for when a tower becomes neutral while it is being attacked
	void onTowerBecomesNeutral( uint32 towerId )
	{
		std::map< uint32, TowerEventHandler* >::const_iterator itr = towerEventHandlers.find( towerId );
		if( itr == towerEventHandlers.end() )
			return;

		itr->second->onTowerBecomesNeutral();
	}

	/// Event handler for when a tower becomes owned by either the Alliance or the Horde
	void onTowerCaptured( uint32 towerId )
	{
		broadcastCaptureMessage( towerId );

		std::map< uint32, TowerEventHandler* >::const_iterator itr = towerEventHandlers.find( towerId );
		if( itr != towerEventHandlers.end() )
		{
			itr->second->onTowerCaptured();
		}

		/// Find the right spell for both factions, casting it will upgrade / downgrade appropriately
		uint32 allianceSpell = getSpellForTeam( TEAM_ALLIANCE );
		uint32 hordeSpell = getSpellForTeam( TEAM_HORDE );

		if( allianceSpell != 0 )
		{
			mapMgr->castSpellOnPlayers( TEAM_ALLIANCE, allianceSpell );
		}
		else
		{
			for( int i = 1; i <= EP_TOWER_COUNT; i++ )
			{
				mapMgr->removeAuraFromPlayers( TEAM_ALLIANCE, rewardSpellIds[ TEAM_ALLIANCE ][ i ] );
			}
		}

		if( hordeSpell != 0 )
		{
			mapMgr->castSpellOnPlayers( TEAM_HORDE, hordeSpell );
		}
		else
		{
			for( int i = 1; i <= EP_TOWER_COUNT; i++ )
			{
				mapMgr->removeAuraFromPlayers( TEAM_HORDE, rewardSpellIds[ TEAM_ALLIANCE ][ i ] );
			}
		}
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

		allianceTowers += allianceDelta;
		hordeTowers += hordeDelta;
		
		handler.SetWorldStateForZone( ZONE_EPL, WORLDSTATE_EPL_TOWERS_ALLIANCE, allianceTowers );
		handler.SetWorldStateForZone( ZONE_EPL, WORLDSTATE_EPL_TOWERS_HORDE, hordeTowers );

		allianceTowersCache.SetVal( allianceTowers );
		hordeTowersCache.SetVal( hordeTowers );
		
		switch( towerOwner[ towerId ] )
		{
			case -1:
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_ALLIANCE_CONTROL ], 0 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_HORDE_CONTROL ], 0 );
				handler.SetWorldStateForZone( ZONE_EPL, towerWorldStates[ towerId ][ TOWER_NEUTRAL ], 1 );

				onTowerBecomesNeutral( towerId );
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

	/// Sets the artkit for the current owner
	void setArtkit()
	{
		uint8 artkit;
		switch( towerOwner[ towerId ] )
		{
			case -1:
				artkit = 0;
				break;

			case TEAM_ALLIANCE:
				artkit = EP_TOWER_ARTKIT_ALLIANCE;
				break;

			case TEAM_HORDE:
				artkit = EP_TOWER_ARTKIT_HORDE;
				break;
		}

		_gameobject->SetArtKit( artkit );

		std::set< Object* > &objects = _gameobject->GetInRangeObjects();
		for( std::set< Object* >::const_iterator itr = objects.begin(); itr != objects.end(); ++itr )
		{
			Object *obj = *itr;
			if( !obj->IsGameObject() )
			{
				continue;
			}

			GameObject *go = TO_GAMEOBJECT( obj );
			if( go->GetInfo()->ID != GO_EP_TOWER_BANNER_MISC )
			{
				continue;
			}

			go->SetArtKit( artkit );
		}
	}

	/// Owner change event handler
	void onOwnerChange( int32 lastOwner )
	{
		pvp.onTowerOwnershipChange( towerId, lastOwner );
		setArtkit();
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

	/// Rewards the player for capturing the tower with honor, quest credit, etc
	void rewardPlayers( std::set< Player* > &players )
	{
		for( std::set< Player* >::const_iterator itr = players.begin(); itr != players.end(); ++itr )
		{
			Player *player = *itr;

			HonorHandler::AddHonorPointsToPlayer( player, REWARD_HONOR_POINTS );

			player->AddQuestKill( questByTeam[ player->GetTeam() ], towerToQuestCredit[ towerId ] );
		}
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

		RegisterAIUpdateEvent( EP_TOWER_SCAN_UPDATE_FREQ );
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

			/// Calculate the player's distance from the center of the tower
			float d = player->CalcDistance( towerCoords[ towerId ][ 0 ], towerCoords[ towerId ][ 1 ], towerCoords[ towerId ][ 2 ] );

			if( d > EP_TOWER_SCAN_RANGE )
			{
				/// No progress bar for players out of range
				Messenger::SendWorldStateUpdate( player, WORLDSTATE_EPL_TOWER_PROGRESS_UI, 0 );
			}
			else
			{
				/// Dead, invisible, and stealthed, and not PvP flagged players don't count
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

			if( lastOwner == -1 )
			{
				rewardPlayers( playersInRange );
			}
		}

		playersInRange.clear();
	}
};

enum TaxiPathIds
{
	EP_TAXIPATH_NORTHPASS  = 494,
	EP_TAXIPATH_EASTWALL   = 495,
	EP_TAXIPATH_CROWNGUARD = 496
};

#define MODEL_SPECTRAL_GRYPHON 20070
#define GOSSIP_TEXT_WILLIAM_KIELAR 8834

//  GossipScript for the spectral gryphon master William Kielar
class WilliamKielarGossip : public Arcemu::Gossip::Script
{
	public:
		WilliamKielarGossip()
		{			
		}

		void OnHello( Object *object, Player* player )
		{
			Arcemu::Gossip::Menu menu( object->GetGUID(), GOSSIP_TEXT_WILLIAM_KIELAR );

			menu.AddItem( Arcemu::Gossip::ICON_CHAT, "Take me to Northpass Tower", 1, false );
			menu.AddItem( Arcemu::Gossip::ICON_CHAT, "Take me to Eastwall Tower", 2, false );
			menu.AddItem( Arcemu::Gossip::ICON_CHAT, "Take me to Crown Guard Tower", 3, false );

			menu.Send( player );
		}

		void OnSelectOption( Object *object, Player *player, uint32 selection, const char* code )
		{
			Arcemu::Gossip::Menu::Complete( player );

			uint32 taxiPathId = 0;

			switch( selection )
			{
				case 1:
					taxiPathId = EP_TAXIPATH_NORTHPASS;
					break;

				case 2:
					taxiPathId = EP_TAXIPATH_EASTWALL;
					break;

				case 3:
					taxiPathId = EP_TAXIPATH_CROWNGUARD;
					break;
			}

			if( taxiPathId != 0 )
			{
				TaxiPath *path = sTaxiMgr.GetTaxiPath( taxiPathId );
				if( path != NULL )
				{
					player->TaxiStart( path, MODEL_SPECTRAL_GRYPHON, 0 );
				}
			}
		}

		void Destroy()
		{
			delete this;
		}
};

class TowerCommanderAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TowerCommanderAI);

	TowerCommanderAI( Creature *creature ) : CreatureAIScript( creature )
	{
		_unit->GetAIInterface()->setMoveType( MOVEMENTTYPE_DONTMOVEWP );
		loadWaypoints();
	}

	void OnLoad()
	{
		_unit->GetAIInterface()->setMoveType( MOVEMENTTYPE_FORWARDTHANSTOP );
		_unit->GetAIInterface()->StopMovement( 2 * 1000 );
		_unit->GetAIInterface()->setWaypointToMove( 0 );
	}

	void OnDespawn()
	{
	}

private:

	void loadWaypoints()
	{
		WayPointMap *waypoints = new WayPointMap;

		for( uint32 i = 0; i < ATTACKWAYPOINT_COUNT; i++ )
		{
			waypoints->push_back( createWaypoint( i ) );
		}

		_unit->setCustomWayPoints( waypoints );
		_unit->GetAIInterface()->SetWaypointMap(_unit->getCustomWayPoints());
	}

	WayPoint* createWaypoint( uint32 i )
	{
		WayPoint *wp = _unit->CreateWaypointStruct();
		
		wp->id = i + 1;
		wp->x = attackWaypoints[ i ][ 0 ];
		wp->y = attackWaypoints[ i ][ 1 ];
		wp->z = attackWaypoints[ i ][ 2 ];
		wp->o = attackWaypoints[ i ][ 3 ];
		wp->waittime = 500;
		wp->flags = 256;
		wp->forwardemoteoneshot = false;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = false;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;

		return wp;
	}
};

/// Is the specified map, zone, area triplet considered to be in Eastern Plaguelands?
static bool isEpl( uint32 map, uint32 zone, uint32 area )
{
	if( map == MAP_STRATHOLME )
	{
		return true;
	}

	if( ( map == MAP_EASTERN_KINGDOMS ) && ( ( zone == 139 ) || ( zone == 2017 ) ) )
	{
		return true;
	}

	return false;
}

/// Applies the right reward aura on login
static void Epl_onEnterWorld( Player *player )
{
	if( !isEpl( player->GetMapId(), player->GetZoneId(), player->GetAreaID() ) )
	{
		if( player->GetMapId() != MAP_SCHOLOMANCE )
		{
			player->RemoveAura( SPELL_LORDAERONS_BLESSING );
		}
		return;
	}

	uint32 spell = pvp.getSpellForTeam( player->GetTeam() );
	if( spell != 0 )
	{
		player->CastSpell( player, spell, true );
	}
}

/// Removes the reward aura so that it's not saved in DB
static void Epl_onLogout( Player *player )
{
	uint32 team = player->GetTeam();	
	for( int i = 1; i <= EP_TOWER_COUNT; i++ )
	{
		player->RemoveAura( rewardSpellIds[ team ][ i ] );
	}
}

/// Applies / removes the reward aura when entering / leaving the zone
void Epl_onZoneChange( Player *player, uint32 newZone, uint32 oldZone )
{
	uint32 team = player->GetTeam();	

	if( isEpl( player->GetMapId(), player->GetZoneId(), player->GetAreaID() ) )
	{
		uint32 spell = pvp.getSpellForTeam( team );
		if( spell != 0 )
		{
			player->CastSpell( player, spell, true );
		}
	}
	else
	{
		if( player->GetMapId() != MAP_SCHOLOMANCE )
		{
			player->RemoveAura( SPELL_LORDAERONS_BLESSING );
		}

		for( int i = 1; i <= EP_TOWER_COUNT; i++ )
		{
			player->RemoveAura( rewardSpellIds[ team ][ i ] );
		}
	}
}

void setupEasternPlaguelands( ScriptMgr *mgr )
{
	MapMgr *mapMgr = sInstanceMgr.GetMapMgr( MAP_EASTERN_KINGDOMS );
	pvp.setMapMgr( mapMgr );
	pvp.registerTowerEventHandler( EP_TOWER_NORTHPASS, new NorthpassTowerEventHandler( mapMgr ) );
	pvp.registerTowerEventHandler( EP_TOWER_PLAGUEWOOD, new PlagueWoodTowerEventHandler( mapMgr ) );
	pvp.registerTowerEventHandler( EP_TOWER_CROWNGUARD, new CrownGuardTowerEventHandler( mapMgr ) );
	pvp.registerTowerEventHandler( EP_TOWER_EASTWALL, new EastwallTowerEventHandler( mapMgr ) );

	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_NORTHPASS, &EPTowerBannerAI::Create );
	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_CROWNGUARD, &EPTowerBannerAI::Create );
	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_EASTWALL, &EPTowerBannerAI::Create );
	mgr->register_gameobject_script( GO_EP_TOWER_BANNER_PLAGUEWOOD, &EPTowerBannerAI::Create );

	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&Epl_onEnterWorld );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)&Epl_onLogout );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ZONE, (void*)&Epl_onZoneChange );

	mgr->register_creature_gossip( NPC_WILLIAM_KIELAR, new WilliamKielarGossip() );

	mgr->register_creature_script( NPC_LORDAERON_COMMANDER, &TowerCommanderAI::Create );
	mgr->register_creature_script( NPC_LORDAERON_VETERAN, &TowerCommanderAI::Create );

	/// Haxx! TODO: Improve this
	SpellEntry *spe = dbcSpell.LookupEntryForced( SPELL_LORDAERONS_BLESSING );
	if( spe != NULL )
	{
		spe->EffectApplyAuraName[ 1 ] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
	}

	/// Crownguard Tower graveyard belongs to no one by default
	LocationVector location( graveyardLocation[ 0 ], graveyardLocation[ 1 ], graveyardLocation[ 2 ] );
	sGraveyardService.setGraveyardOwner(
		MAP_EASTERN_KINGDOMS,
		location,
		std::numeric_limits<uint32>::max()
	);
}

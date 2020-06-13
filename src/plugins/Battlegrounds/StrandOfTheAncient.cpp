/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

/***********************************************************************
Strand of the Ancients
======================

* Other Game Objects
	* Defender's Portal, 190763
	* Defender's Portal, 191575
	* Defender's Portal, 192819
		-gps at:
		-gps to:
	* Titan Relic, 194083 (different one, not supported atm)
	* Collision PC Size, 188215
	* The Coffin Carrier, 193184
	* The Blightbringer, 193183
	* The Frostbreaker, 193185
	* The Graceful Maiden (boat?), 193182
	* Doodad_WG_Keep_Door01_collision01, 194162 (Not implemented atm)

* Revive everyone after round one
	* bg->EventResurrectPlayers()

* Setup index 34 in worldstring_tables to equal "Strand of the Ancients"

* Fix level requirements to join the battleground. And fix which npc text is used
  for the battlemaster gossip scripts, displaying the proper of 3 messages
	npc_text
		 13832 = You are not yet strong enough to do battle in the Strand of the Ancients. Return when you have gained more experience.
		 13834 = We cannot allow the Alliance to turn the hidden secrets of the Strand of the Ancients against us. Will you join our brave warriors there?
		+13833 = We cannot allow the Horde to turn the hidden secrets of the Strand of the Ancients against us. Will you join our brave warriors there?

* Increase the view distance on map 607 to 500 or 0 (Unlimited). See the
  transporter patch... Need a way to see the gates from as far away as
  the boats.

* Besure to spawn, platforms, vehicels, and relic so only the proper faction
  can use them.

* Fix it where a BG is instanced as soon as the first player joins, only
  after one faction has field their entire queue for a particular battlefield,
  would a new BG instance be created. It might actually be this way, if so
  just patch so that these pre-loaded instances appear in the battlemaster lists.

* Also change so numbers are reused, once SOTA instance 1 is deleted, there is
  no reason why that instance id can't be reused. Also each BG needs it own
  unique numbering, instead of a shared pool.


************************************************************************/

#include "StdAfx.h"
#include "StrandOfTheAncient.h"


#define GO_RELIC 192834
const float sotaTitanRelic[4] = { 836.5f, -108.8f, 111.59f, 0.0f };

const uint32 GateGOIds[6] =
{
	190722,	// Gate of the Green Emerald
	190727,	// Gate of the Yellow Moon
	190724,	// Gate of the Blue Sapphire
	190726,	// Gate of the Red Sun
	190723,	// Gate of the Purple Amethyst
	192549,	// Chamber of Ancient Relics
};
const float sotaGates[GATE_COUNT][4] =
{
	{ 1411.57f, 108.163f, 28.692f, 5.441f },
	{ 1055.452f, -108.1f, 82.134f, 0.034f },
	{ 1431.3413f, -219.437f, 30.893f, 0.9736f },
	{ 1227.667f, -212.555f, 55.372f, 0.5023f },
	{ 1214.681f, 81.21f, 53.413f, 5.745f },
};
const float sotaChamberGate[4] = { 878.555f, -108.989f, 119.835f, 0.0565f };

// Things radiating out from the gates... same orientation as door.
const uint32 GateSigilGOIds[5] =  { 192687, 192685, 192689, 192690, 192691, };
const float sotaGateSigils[GATE_COUNT][4] =
{
	{ 1414.054f, 106.72f, 41.442f, 5.441f },
	{ 1060.63f, -107.8f, 94.7f, 0.034f },
	{ 1433.383f, -216.4f, 43.642f, 0.9736f },
	{ 1230.75f, -210.724f, 67.611f, 0.5023f },
	{ 1217.8f, 79.532f, 66.58f, 5.745f },
};

// Defender transporter platform locations
const float sotaTransporters[GATE_COUNT][4] =
{
	{ 1394.0444f, 72.586f, 31.0535f, 0.0f },
	{ 1065.0f, -89.7f, 81.08f, 0.0f },
	{ 1467.95f, -225.67f, 30.9f, 0.0f },
	{ 1255.93f, -233.382f, 56.44f, 0.0f },
	{ 1215.679f, 47.47f, 54.281f, 0.0f },
};

// Defender transporter destination locations
const float sotaTransporterDestination[GATE_COUNT][4] =
{
	{ 1388.94f, 103.067f, 34.49f, 5.4571f },
	{ 1043.69f, -87.95f, 87.12f, 0.003f },
	{ 1441.0411f, -240.974f, 35.264f, 0.949f },
	{ 1228.342f, -235.234f, 60.03f, 0.4584f },
	{ 1193.857f, 69.9f, 58.046f, 5.7245f },
};

// Two guns per gate, GUN_LEFT and GUN_RIGHT
static LocationVector CanonLocations[ SOTA_NUM_CANONS ] = {
	LocationVector( 1436.429f, 110.05f, 41.407f, 5.4f ),
	LocationVector( 1404.9023f, 84.758f, 41.183f, 5.46f ),
	LocationVector( 1068.693f, -86.951f, 93.81f, 0.02f ),
	LocationVector( 1068.83f, -127.56f, 96.45f, 0.0912f ),
	LocationVector( 1422.115f, -196.433f, 42.1825f, 1.0222f ),
	LocationVector( 1454.887f, -220.454f, 41.956f, 0.9627f ),
	LocationVector( 1232.345f, -187.517f, 66.945f, 0.45f ),
	LocationVector( 1249.634f, -224.189f, 66.72f, 0.635f ),
	LocationVector( 1236.213f, 92.287f, 64.965f, 5.751f ),
	LocationVector( 1215.11f, 57.772f, 64.739f, 5.78f )
};

static LocationVector DemolisherLocations[ SOTA_NUM_DEMOLISHERS ] = {
	LocationVector( 1620.71f, 64.04f, 7.19f, 3.78f ),
	LocationVector( 1593.59f, 40.8f, 7.52f, 0.86f ),
	LocationVector( 1582.42f, -93.75f, 8.49f, 5.7f ),
	LocationVector( 1611.19f, -117.4f, 8.77f, 2.55f ),
	LocationVector( 1353.34f, 224.01f, 35.24f, 4.236f ),
	LocationVector( 1371.03f, -317.06f, 35.01f, 1.85f )
};

// ---- Verify remaining ----- //

// There should only be two boats. boats three and four here
// are a lazy hack for not wanting to program the boats to move via waypoints
const float sotaBoats[4][4] =
{
	{ 1623.34f, 37.0f, 1.0f, 3.65f },
	{ 2439.4f, 845.38f, 1.0f, 3.35f },
	{ 1623.34f, 37.0f, 1.0f, 3.65f },
	{ 2439.4f, 845.38f, 1.0f, 3.35f },
};

static LocationVector sotaAttackerStartingPosition[ SOTA_NUM_ROUND_STAGES ] = {
	LocationVector( 2445.288f, 849.35f, 10.0f, 3.76f ),
	LocationVector( 1624.7f, 42.93f, 10.0f, 2.63f )
};

static LocationVector sotaDefenderStartingPosition
	= LocationVector( 1209.7f, -65.16f, 70.1f, 0.0f );

static LocationVector FlagPolePositions[ NUM_SOTA_CONTROL_POINTS ] = {
	LocationVector( 1338.863892f, -153.336533f, 30.895121f, -2.530723f ),
	LocationVector( 1309.124268f, 9.410645f, 30.893402f, -1.623156f ),
	LocationVector( 1215.114258f, -65.711861f, 70.084267f, -3.124123f )
};

static LocationVector FlagPositions[ NUM_SOTA_CONTROL_POINTS ] = {
	LocationVector( 1338.859253f, -153.327316f, 30.895077f, -2.530723f ),
	LocationVector( 1309.192017f, 9.416233f, 30.893402f, 1.518436f ),
    LocationVector( 1215.108032f, -65.715767f, 70.084267f, -3.124123f )
};

static const uint32 FlagIDs[ NUM_SOTA_CONTROL_POINTS ][ MAX_PLAYER_TEAMS ] = {
	{ 191306, 191305 },
	{ 191308, 191307 },
	{ 191310, 191309 }
};

static const uint32 CPWorldstates[ NUM_SOTA_CONTROL_POINTS ][ MAX_PLAYER_TEAMS ] = {
	{ WORLDSTATE_SOTA_GY_E_A, WORLDSTATE_SOTA_GY_E_H },
	{ WORLDSTATE_SOTA_GY_W_A, WORLDSTATE_SOTA_GY_W_H },
	{ WORLDSTATE_SOTA_GY_S_A, WORLDSTATE_SOTA_GY_S_H }
};

static const uint32 SOTA_FLAGPOLE_ID = 191311;

const char* ControlPointNames[ NUM_SOTA_CONTROL_POINTS ] = {
	"East Graveyard",
	"West Graveyard",
	"South Graveyard"
};

static LocationVector GraveyardLocations[ NUM_SOTA_GRAVEYARDS ] = {
	LocationVector( 1396.06018066f, -288.036895752f, 32.0815124512f, 0.0f ),
	LocationVector( 1388.80358887f, 203.354873657f, 32.1526679993f, 0.0f ),
	LocationVector( 1122.27844238f, 4.41617822647f, 68.9358291626f, 0.0f ),
	LocationVector( 964.595275879f,	-189.784011841f, 90.6604995728f, 0.0f ),
	LocationVector( 1457.19372559f, -53.7132720947f, 5.18109416962f, 0.0f ),
};

static const uint32 TeamFactions[ MAX_PLAYER_TEAMS ] = {
	1,
	2
};

StrandOfTheAncient::StrandOfTheAncient( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t ) :
CBattleground( mgr, id, lgroup, t ){
	m_zoneid = 4384;
	std::fill( &canon[ 0 ], &canon[ SOTA_NUM_CANONS ], reinterpret_cast< Creature* >( NULL ) );
	std::fill( &demolisher[ 0 ], &demolisher[ SOTA_NUM_DEMOLISHERS ], reinterpret_cast< Creature* >( NULL ) );
}

StrandOfTheAncient::~StrandOfTheAncient(){
	std::fill( &canon[ 0 ], &canon[ SOTA_NUM_CANONS ], reinterpret_cast< Creature* >( NULL ) );
	std::fill( &demolisher[ 0 ], &demolisher[ SOTA_NUM_DEMOLISHERS ], reinterpret_cast< Creature* >( NULL ) );
}


void StrandOfTheAncient::HookOnAreaTrigger(Player* plr, uint32 id)
{

}

void StrandOfTheAncient::HookOnPlayerKill(Player* plr, Player* pVictim)
{
	plr->m_bgScore.KillingBlows++;
	UpdatePvPData();
}

void StrandOfTheAncient::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void StrandOfTheAncient::OnPlatformTeleport(Player* plr)
{
}

void StrandOfTheAncient::OnAddPlayer(Player* plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
}

void StrandOfTheAncient::OnRemovePlayer(Player* plr)
{
	if(!m_started)
		plr->RemoveAura(BG_PREPARATION);
}

LocationVector StrandOfTheAncient::GetStartingCoords( uint32 team ){
	if( team == Attackers )
		return sotaAttackerStartingPosition[ roundprogress ];
	else
		return sotaDefenderStartingPosition;
}

void StrandOfTheAncient::HookOnPlayerDeath(Player* plr)
{
	plr->m_bgScore.Deaths++;
	UpdatePvPData();
}

void StrandOfTheAncient::HookOnMount(Player* plr)
{
	/* Allowed */
}

bool StrandOfTheAncient::HookHandleRepop( Player *plr ){
	float dist = 999999.0f;
	LocationVector dest_pos;
	uint32 id = 0;
	
	// Let's find the closests GY
	for( uint32 i = SOTA_GY_EAST; i < NUM_SOTA_GRAVEYARDS; i++ ){
		if( graveyard[ i ].faction == plr->GetTeam() ){
			if( graveyard[ i ].spiritguide == NULL )
				continue;
			
			float gydist = plr->CalcDistance( graveyard[ i ].spiritguide );
			if( gydist > dist )
				continue;
			
			dist = gydist;
			dest_pos = graveyard[ i ].spiritguide->GetPosition();
			id = i;
		}
	}
	
	if( id >= NUM_SOTA_GRAVEYARDS )
		return false;
	
	// port to it and queue for auto-resurrect
	plr->SafeTeleport( plr->GetMapId(), plr->GetInstanceID(), dest_pos );
	QueuePlayerForResurrect( plr, graveyard[ id ].spiritguide );
	
	return true;
}

void StrandOfTheAncient::OnCreate()
{
	{
		uint32 i;

		BattleRound = 1;
		roundprogress = SOTA_ROUND_PREPARATION;

		for(i = 0; i < 2; i++)
		{
			m_players[i].clear();
			m_pendPlayers[i].clear();
			RoundFinishTime[ i ] = 10 * 60;
		}

		m_pvpData.clear();
		m_resurrectMap.clear();

		// Boats
		for(i = 0; i < 4; i++)
		{
			m_boats[i] = m_mapMgr->CreateAndSpawnGameObject(20808,
			             sotaBoats[i][0], sotaBoats[i][1], sotaBoats[i][2], sotaBoats[i][3], 1.0f);
			m_boats[i]->PushToWorld( m_mapMgr );
		}

		/* Relic */
		m_relic = m_mapMgr->CreateAndSpawnGameObject(GO_RELIC, sotaTitanRelic[0],
		          sotaTitanRelic[1], sotaTitanRelic[2], sotaTitanRelic[3], 1.0f);

		for(i = 0; i < GATE_COUNT; i++)
		{
			m_gates[i] = m_mapMgr->CreateAndSpawnGameObject(GateGOIds[i],
			             sotaGates[i][0], sotaGates[i][1], sotaGates[i][2], sotaGates[i][3], 1.0f);
			m_gateSigils[i] = m_mapMgr->CreateAndSpawnGameObject(GateSigilGOIds[i],
			                  sotaGateSigils[i][0], sotaGateSigils[i][1], sotaGateSigils[i][2],
			                  sotaGateSigils[i][3], 1.0f);
			m_gateTransporters[i] = m_mapMgr->CreateAndSpawnGameObject(192819,
			                        sotaTransporters[i][0], sotaTransporters[i][1], sotaTransporters[i][2],
			                        sotaTransporters[i][3], 1.0f);
		}

		// Spawn door for Chamber of Ancient Relics
		m_endgate = m_mapMgr->CreateAndSpawnGameObject(GateGOIds[i],
		            sotaChamberGate[0], sotaChamberGate[1], sotaChamberGate[2],
		            sotaChamberGate[3], 1.0f);
	}

	PrepareRound();
}

void StrandOfTheAncient::OnStart(){

	m_started = true;

	StartRound();
}

void StrandOfTheAncient::HookGenerateLoot(Player* plr, Object* pOCorpse)
{
	LOG_DEBUG("*** StrandOfTheAncient::HookGenerateLoot");
}

void StrandOfTheAncient::HookOnUnitKill( Player* plr, Unit* pVictim ){
}

void StrandOfTheAncient::HookOnUnitDied( Unit *victim ){
	if( victim->IsCreature() ){

		for( uint32 i = 0; i < SOTA_NUM_DEMOLISHERS; i++ ){
			Creature *c = demolisher[ i ];

			if( c == NULL )
				continue;

			if( victim->GetGUID() != c->GetGUID() )
				continue;

			demolisher[ i ] = SpawnCreature( 28781, DemolisherLocations[ i ], TeamFactions[ Attackers ] );
			c->Despawn( 1, 0 );
		}

		for( uint32 i = 0; i < SOTA_NUM_CANONS; i++ ){
			if( canon[ i ] == NULL )
				continue;

			if( victim->GetGUID() != canon[ i ]->GetGUID() )
				continue;

			canon[ i ]->Despawn( 1, 0 );
			canon[ i ] = NULL;
		}
	}
}

void StrandOfTheAncient::SetIsWeekend(bool isweekend)
{
	LOG_DEBUG("*** StrandOfTheAncient::SetIsWeekend");
	m_isWeekend = isweekend;
}

bool StrandOfTheAncient::HookSlowLockOpen( GameObject *go, Player *player, Spell *spell ){
	uint32 goentry = go->GetEntry();

	switch( goentry ){
		case 191305:
		case 191306:
			CaptureControlPoint( SOTA_CONTROL_POINT_EAST_GY );
			return true;
			break;

		case 191307:
		case 191308:
			CaptureControlPoint( SOTA_CONTROL_POINT_WEST_GY );
			return true;
			break;

		case 191309:
		case 191310:
			CaptureControlPoint( SOTA_CONTROL_POINT_SOUTH_GY );
			return true;
			break;
	}

	return true;
}

bool StrandOfTheAncient::HookQuickLockOpen( GameObject *go, Player *player, Spell *spell ){
	uint32 entry = go->GetEntry();

	if( entry == GO_RELIC )
		FinishRound();

	return true;
}

// For banners
void StrandOfTheAncient::HookFlagStand(Player* plr, GameObject* obj)
{
}

// time in seconds
void StrandOfTheAncient::SetTime( uint32 secs ){
	uint32 minutes = secs / TIME_MINUTE;
	uint32 seconds = secs % TIME_MINUTE;
	uint32 digits[3];
	digits[0] = minutes;
	digits[1] = seconds / 10;
	digits[2] = seconds % 10;

	SetWorldState(WORLDSTATE_SOTA_TIMER_1, digits[0]);
	SetWorldState(WORLDSTATE_SOTA_TIMER_2, digits[1]);
	SetWorldState(WORLDSTATE_SOTA_TIMER_3, digits[2]);
	SetRoundTime(secs);
}

void StrandOfTheAncient::PrepareRound(){
	roundprogress = SOTA_ROUND_PREPARATION;

	if( BattleRound == 1 ){
		Attackers = RandomUInt( 1 );

		if( Attackers == TEAM_ALLIANCE )
			Defenders = TEAM_HORDE;
		else
			Defenders = TEAM_ALLIANCE;
	}else{
		std::swap( Attackers, Defenders );
	}

	for( uint32 i = 0; i < GATE_COUNT; i++ ){
		m_gates[ i ]->Rebuild();
		m_gates[ i ]->SetFaction( TeamFactions[ Defenders ] );
	}

	m_endgate->Rebuild();
	m_endgate->SetFaction( TeamFactions[ Defenders ] );

	m_relic->SetFaction( TeamFactions[ Attackers ] );

	for( uint32 i = 0; i < GATE_COUNT; i++ )
		m_gateTransporters[ i ]->SetFaction( TeamFactions[ Defenders ] );

	for( uint32 i = 0; i < SOTA_NUM_CANONS; i++ ){
		if( canon[ i ] != NULL )
			canon[ i ]->Despawn( 0, 0 );
		canon[ i ] = SpawnCreature( 27894, CanonLocations[ i ], TeamFactions[ Defenders ] );
	}

	for( uint32 i = 0; i < SOTA_NUM_DOCK_DEMOLISHERS; i++ ){
		Creature *c = demolisher[ i ];
		demolisher[ i ] = SpawnCreature( 28781, DemolisherLocations[ i ], TeamFactions[ Attackers ] );
		if( c != NULL )
			c->Despawn( 0, 0 );
	}

	for( uint32 i = SOTA_WEST_WS_DEMOLISHER_INDEX; i < SOTA_NUM_DEMOLISHERS; i++ ){
		if( demolisher[ i ] != NULL ){
			demolisher[ i ]->Despawn( 0, 0 );
			demolisher[ i ] = NULL;
		}
	}
	
	SOTACPStates state;

	if( Attackers == TEAM_ALLIANCE ){
		state = SOTA_CP_STATE_HORDE_CONTROL;
		SetWorldState( WORLDSTATE_SOTA_HORDE_ATTACKER, 0 );
		SetWorldState( WORLDSTATE_SOTA_ALLIANCE_ATTACKER, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_ROUND, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_ROUND, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_DEFENSE, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_DEFENSE, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_BEACHHEAD1, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_BEACHHEAD2, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_BEACHHEAD1, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_BEACHHEAD2, 0 );
	}else{
		state = SOTA_CP_STATE_ALLY_CONTROL;
		SetWorldState( WORLDSTATE_SOTA_HORDE_ATTACKER, 1 );
		SetWorldState( WORLDSTATE_SOTA_ALLIANCE_ATTACKER, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_ROUND, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_ROUND, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_DEFENSE, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_DEFENSE, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_BEACHHEAD1, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_ALLY_BEACHHEAD2, 0 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_BEACHHEAD1, 1 );
		SetWorldState( WORLDSTATE_SOTA_SHOW_HORDE_BEACHHEAD2, 1 );
	}

	SpawnControlPoint( SOTA_CONTROL_POINT_EAST_GY,  state );
	SpawnControlPoint( SOTA_CONTROL_POINT_WEST_GY,  state );
	SpawnControlPoint( SOTA_CONTROL_POINT_SOUTH_GY, state );
	SpawnGraveyard( SOTA_GY_ATTACKER_BEACH, Attackers );
	SpawnGraveyard( SOTA_GY_DEFENDER, Defenders );

	if( BattleRound == 2 ){
		// Teleport players to their place and cast preparation on them
		m_mainLock.Acquire();

		for( std::set< Player* >::iterator itr = m_players[ Attackers ].begin(); itr != m_players[ Attackers ].end(); ++itr ){
			Player *p = *itr;
			p->SafeTeleport( p->GetMapId(), p->GetInstanceID(), sotaAttackerStartingPosition[ 0 ] );
			p->CastSpell( p, BG_PREPARATION, true );
		}

		for( std::set< Player* >::iterator itr = m_players[ Defenders ].begin(); itr != m_players[ Defenders ].end(); ++itr ){
			Player *p = *itr;
			p->SafeTeleport( p->GetMapId(), p->GetInstanceID(), sotaDefenderStartingPosition );
			p->CastSpell( p, BG_PREPARATION, true );
		}

		m_mainLock.Release();

		sEventMgr.AddEvent( this, &StrandOfTheAncient::StartRound, EVENT_SOTA_START_ROUND, 1 * 10 * 1000, 1, 0 );
	}
};

void StrandOfTheAncient::StartRound(){
	roundprogress = SOTA_ROUND_STARTED;

	m_mainLock.Acquire();
	
	for( std::set< Player* >::iterator itr = m_players[ Attackers ].begin(); itr != m_players[ Attackers ].end(); itr++ ){
		Player *p = *itr;
		
		p->SafeTeleport( p->GetMapId(), p->GetInstanceID(), sotaAttackerStartingPosition[ SOTA_ROUND_STARTED ] );
		p->RemoveAura( BG_PREPARATION );
	}

	m_mainLock.Release();

	RemoveAuraFromTeam( Defenders, BG_PREPARATION );

	SetWorldState( WORLDSTATE_SOTA_TIMER_1, 10 );
	SetTime( ROUND_LENGTH );
	sEventMgr.AddEvent( this, &StrandOfTheAncient::TimeTick, EVENT_SOTA_TIMER, MSTIME_SECOND * 5, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	UpdatePvPData();
}

void StrandOfTheAncient::FinishRound(){
	sEventMgr.RemoveEvents( this, EVENT_SOTA_TIMER );
	EventResurrectPlayers();

	RoundFinishTime[ BattleRound - 1 ] = RoundTime;

	if( BattleRound == 1 ){
		BattleRound = 2;
		PrepareRound();
	}else{
		if( RoundFinishTime[ 0 ] < RoundFinishTime[ 1 ] )
			Finish( Attackers );
		else
			Finish( Defenders );
	}
}

void StrandOfTheAncient::Finish( uint32 winningteam ){
    sEventMgr.RemoveEvents( this );
	m_ended = true;
	m_winningteam = winningteam;

	uint32 losingteam;
	if( winningteam == TEAM_ALLIANCE )
		losingteam = TEAM_HORDE;
	else
		losingteam = TEAM_ALLIANCE;

	AddHonorToTeam( winningteam, 3 * 185 );
	AddHonorToTeam( losingteam, 1 * 185 );
	CastSpellOnTeam( m_winningteam, 61213 );

	UpdatePvPData();

	sEventMgr.AddEvent( TO< CBattleground* >( this ), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120 * 1000, 1,0 );
}

void StrandOfTheAncient::TimeTick(){
	SetTime( RoundTime - 5 );

	if( RoundTime == 0){
		sEventMgr.RemoveEvents(this, EVENT_SOTA_TIMER);
		FinishRound();
	}
};

// Not used?
void StrandOfTheAncient::HookOnFlagDrop(Player* plr)
{
}
void StrandOfTheAncient::HookFlagDrop(Player* plr, GameObject* obj)
{
}
void StrandOfTheAncient::HookOnShadowSight()
{
}


void StrandOfTheAncient::SpawnControlPoint( SOTAControlPoints point, SOTACPStates state ){
	if( state >= MAX_SOTA_CP_STATES )
		return;

	SOTAControlPoint &cp = controlpoint[ point ];

	if( cp.worldstate != 0 )
		SetWorldState( cp.worldstate, 0 );

	uint32 team = TEAM_ALLIANCE;
	uint32 faction = 0;

	switch( state ){
		case SOTA_CP_STATE_ALLY_CONTROL:
			team = TEAM_ALLIANCE;
			faction = 2;
			break;

		case SOTA_CP_STATE_HORDE_CONTROL:
			team = TEAM_HORDE;
			faction = 1;
			break;

		default:
			return;
			break;
	}

	// First time spawning
	if( cp.pole == NULL ){
		cp.pole = SpawnGameObject( SOTA_FLAGPOLE_ID, FlagPolePositions[ point ], 0, 35, 1.0f );
		cp.pole->PushToWorld( m_mapMgr );
	}else{
		Arcemu::Util::ArcemuAssert( cp.banner != NULL );
		cp.banner->Despawn( 0, 0 );
	}
	
	cp.banner = SpawnGameObject( FlagIDs[ point ][ team ], FlagPositions[ point ], 0, faction, 1.0f );
	cp.banner->PushToWorld( m_mapMgr );

	cp.state = state;
	cp.worldstate = CPWorldstates[ point ][ team ];
	SetWorldState( cp.worldstate, 1 );

	//Spawn graveyard
	SpawnGraveyard( SOTAGraveyards( uint32( point ) ), team );
}

void StrandOfTheAncient::CaptureControlPoint( SOTAControlPoints point ){
	if( point >= NUM_SOTA_CONTROL_POINTS )
		return;

	SOTAControlPoint &cp = controlpoint[ point ];

	if( cp.banner->GetFaction() == 14 )
		return;

	switch( cp.state ){
		case SOTA_CP_STATE_ALLY_CONTROL:
			SpawnControlPoint( point, SOTA_CP_STATE_HORDE_CONTROL );
			PlaySoundToAll( SOUND_HORDE_CAPTURE );
			SendChatMessage( CHAT_MSG_BG_EVENT_HORDE, 0, "The horde has captured the %s!", ControlPointNames[ point ] );
			break;

		case SOTA_CP_STATE_HORDE_CONTROL:
			SpawnControlPoint( point, SOTA_CP_STATE_ALLY_CONTROL );
			PlaySoundToAll( SOUND_ALLIANCE_CAPTURE );
			SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE, 0, "The alliance has captured the %s!", ControlPointNames[ point ] );
			break;
	}

	cp.banner->SetFaction( 14 ); // So they cannot be recaptured as per SOTA rules

	//Spawn workshop demolisher
	switch( point ){
		case SOTA_CONTROL_POINT_EAST_GY:
			demolisher[ SOTA_EAST_WS_DEMOLISHER_INDEX ] = SpawnCreature( 28781, DemolisherLocations[ SOTA_EAST_WS_DEMOLISHER_INDEX ], TeamFactions[ Attackers ] );
			break;
		case SOTA_CONTROL_POINT_WEST_GY:
			demolisher[ SOTA_WEST_WS_DEMOLISHER_INDEX ] = SpawnCreature( 28781, DemolisherLocations[ SOTA_WEST_WS_DEMOLISHER_INDEX ], TeamFactions[ Attackers ] );
			break;
	}
}

void StrandOfTheAncient::SpawnGraveyard( SOTAGraveyards gyid, uint32 team ){
	if( gyid >= NUM_SOTA_GRAVEYARDS )
		return;

	SOTAGraveyard &gy = graveyard[ gyid ];

	gy.faction = team;

	if( gy.spiritguide != NULL )
		gy.spiritguide->Despawn( 0, 0 );

	gy.spiritguide = SpawnSpiritGuide( GraveyardLocations[ gyid ], team );
	AddSpiritGuide( gy.spiritguide );
}

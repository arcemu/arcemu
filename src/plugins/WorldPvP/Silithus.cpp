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

#define SPELL_SILITHYST 29519
#define SPELL_TRACES_OF_SILITHYST 29534
#define SPELL_CENARION_FAVOR 30754

#define AT_ALLIANCE_STATION 4162
#define AT_HORDE_STATION    4168

#define GO_SILITHYST_MOUND  181597
#define GO_SILITHYST_GEYSER 181598

#define SILITHYST_GEYSER_RESPAWN_TIMER (20 * 60 * 1000)
#define SILITHYST_MOUND_DESPAWN_TIMER (3 * 60 * 1000)

#define QUEST_ALLIANCE 9419
#define QUEST_HORDE    9422

#define REWARD_HONOR_POINTS 19
#define FACTIONID_CENARION_CIRCLE 609
#define REWARD_REPUTATION_POINTS 20

#define SILITHUS_ZONE_ID 1377
#define GATES_ZONE_ID 3478

static uint32 collectedStateForTeam[] = { WORLDSTATE_SILITHUS_ALLIANCE_COLLECTED, WORLDSTATE_SILITHUS_HORDE_COLLECTED };

static double stationCoords[][3] = {
	{ -7141.979492, 1397.992188, 5.156858 },  /// Alliance station coords
	{ -7588.799316, 756.793823, -15.584143 }  /// Horde station coords
};

/// This is a guesstimated value: Areatrigger seems to activate when in a distance of 3.0-4.0. Added some tolerance to be sure
#define STATION_RADIUS 4.5f

static uint32 questByTeam[] = { QUEST_ALLIANCE, QUEST_HORDE };

static Arcemu::Threading::AtomicULong winnerTeam( static_cast< uint32 >( -1 ) );

/// Is the map, zone, area triplet considered to be in Silithus?
static bool isSilithus( uint32 map, uint32 zone, uint32 area )
{
	if( ( ( map == MAP_KALIMDOR ) && ( zone == SILITHUS_ZONE_ID ) ) ||
			( ( map == MAP_KALIMDOR ) && ( zone == GATES_ZONE_ID ) ) ||
			( map == MAP_RUINS_OF_AHNQIRAJ ) ||
			( map == MAP_AHNQIRAJ_TEMPLE )
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// Add Cenarion Favor aura to eligible players when entering the world
void Silithus_onEnterWorld( Player *player )
{
	uint32 winner = static_cast< uint32 >( winnerTeam.GetVal() );
	if( ( player->GetTeam() == winner ) )
	{
		if( isSilithus( player->GetMapId(), player->GetZoneId(), player->GetAreaID() ) )
		{
			player->CastSpell( player, SPELL_CENARION_FAVOR, true );
		}
	}
}

/// Remove Cenarion Favor aura when logging out, so it's not saved
void Silithus_onLogout( Player *player )
{
	player->RemoveAura( SPELL_CENARION_FAVOR );
}

/// Handles adding / removing the zone buff
void Silithus_onZoneChange( Player *player, uint32 newZone, uint32 oldZone )
{
	if( player == NULL )
	{
		return;
	}

	uint32 winner = static_cast< uint32 >( winnerTeam.GetVal() );

	if( player->GetTeam() == winner && isSilithus( player->GetMapId(), player->GetZoneId(), player->GetAreaID() ) )
	{
		player->CastSpell( player, SPELL_CENARION_FAVOR, true );
	}
	else
	{
		player->RemoveAura( SPELL_CENARION_FAVOR );
	}
}

/// Broadcast a message to inform players that one of the teams has won
void broadcastWinMessage( MapMgr *mapMgr, uint32 maxCollected )
{
	uint32 winner = static_cast< uint32 >( winnerTeam.GetVal() );

	stringstream ss;
	
	ss << "The ";
	if( winner == TEAM_ALLIANCE )
	{
		ss << "Alliance";
	}
	else
	{
		ss << "Horde";
	}
	
	ss << " has collected ";
	ss << maxCollected;
	ss << " silithyst!";
	
	WorldPacket *packet = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL, ss.str().c_str(), 0, 0 );
	if( packet != NULL )
	{
		mapMgr->SendPacketToPlayersInZone( SILITHUS_ZONE_ID, packet );
		delete packet;
	}
}

/// Handles turning in Silithyst
void Silithus_onAreaTrigger( Player *player, uint32 areaTrigger )
{
	if( player == NULL )
	{
		return;
	}

	if( areaTrigger == AT_ALLIANCE_STATION || areaTrigger == AT_HORDE_STATION )
	{
		if( player->HasAura( SPELL_SILITHYST ) )
		{
			player->RemoveAura( SPELL_SILITHYST );

			player->CastSpell( player, SPELL_TRACES_OF_SILITHYST, true );
			player->ModStanding( FACTIONID_CENARION_CIRCLE, REWARD_REPUTATION_POINTS );
			HonorHandler::AddHonorPointsToPlayer( player, REWARD_HONOR_POINTS );
			player->AddQuestKill( questByTeam[ player->GetTeam() ], 0 );

			uint32 collectedStateId = collectedStateForTeam[ player->GetTeam() ];

			MapMgr *mapMgr = player->GetMapMgr();
			WorldStatesHandler &handler = mapMgr->GetWorldStatesHandler();
			uint32 currentCollected = handler.GetWorldStateForZone( SILITHUS_ZONE_ID, collectedStateId );
			uint32 maxCollected = handler.GetWorldStateForZone( SILITHUS_ZONE_ID, WORLDSTATE_SILITHUS_MAX_COLLECTED );

			++currentCollected;
				
			// We're turning in the 200th, we're winning the race!
			if( currentCollected == maxCollected )
			{
				uint32 winner = player->GetTeam();
				winnerTeam.SetVal( winner );

				/// Add the zone buff to the winner team, and remove it from the losers
				TeamAndZoneMatcher matcher( SILITHUS_ZONE_ID, winner );
				CastSpellOnPlayers caster( SPELL_CENARION_FAVOR, true );
				mapMgr->visitPlayers( &caster, &matcher );

				if( winner == TEAM_ALLIANCE )
				{
					TeamAndZoneMatcher matcher( SILITHUS_ZONE_ID, TEAM_HORDE );
					RemoveAura removeAura( SPELL_CENARION_FAVOR );
					mapMgr->visitPlayers( &removeAura, &matcher );
				}
				else
				{
					TeamAndZoneMatcher matcher( SILITHUS_ZONE_ID, TEAM_ALLIANCE );
					RemoveAura removeAura( SPELL_CENARION_FAVOR );
					mapMgr->visitPlayers( &removeAura, &matcher );
				}

				/// Reset the counters
				handler.SetWorldStateForZone( SILITHUS_ZONE_ID, collectedStateForTeam[ TEAM_ALLIANCE ], 0 );
				handler.SetWorldStateForZone( SILITHUS_ZONE_ID, collectedStateForTeam[ TEAM_HORDE ], 0 );

				broadcastWinMessage( mapMgr, maxCollected );
			}
			else
			{
				handler.SetWorldStateForZone( SILITHUS_ZONE_ID, collectedStateId, currentCollected );
			}
		}
	}
}

/// When we lose the aura we drop a mound
void Silithus_onAuraRemoved( Aura *aura )
{
	if( aura->GetSpellId() != SPELL_SILITHYST )
	{
		return;
	}

	Unit *target = aura->GetTarget();
	if( !target->IsPlayer() )
	{
		return;
	}

	Player *player = TO_PLAYER( target );

	if( player->GetZoneId() != SILITHUS_ZONE_ID )
	{
		return;
	}

	/// Turning in the Silithyst at the station also removes the aura, and we don't want to spawn the mound then.
	/// So we check if we're far enough from it
	if( player->CalcDistance( stationCoords[ player->GetTeam() ][ 0 ], stationCoords[ player->GetTeam() ][ 1 ], stationCoords[ player->GetTeam() ][ 2 ] ) > STATION_RADIUS )
	{
		GameObject *mound = player->GetMapMgr()->GetInterface()->SpawnGameObject( GO_SILITHYST_MOUND, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), true, 0, 0 );
		if( mound != NULL )
		{
			mound->Despawn(SILITHYST_MOUND_DESPAWN_TIMER, 0 );
		}
	}

}

/// Silithyst geyser and mound will give you Silithyst aura on activation, then despawn
class SilithystGOAI : public GameObjectAIScript
{
public:
	SilithystGOAI( GameObject *go ) : GameObjectAIScript( go ){}
	static GameObjectAIScript* Create( GameObject *go ){ return new SilithystGOAI( go ); }

	void OnActivate( Player *player )
	{
		if( player == NULL )
		{
			return;
		}

		if( !player->HasAura( SPELL_SILITHYST ) )
		{
			player->CastSpell( player, SPELL_SILITHYST, true );
			
			if( _gameobject->GetEntry() == GO_SILITHYST_GEYSER )
				_gameobject->Despawn( 1, SILITHYST_GEYSER_RESPAWN_TIMER );
			else
				_gameobject->Despawn( 1, 0 );
		}
	}
};

static void setupSpells()
{
	// 29519 - Silithyst
	// Interrupt aura on damage, so that we can drop the mound
	SpellEntry *spe = dbcSpell.LookupEntryForced( SPELL_SILITHYST );
	if( spe != NULL )
	{
		spe->AuraInterruptFlags |= AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN;
	}
}

void setupSilithus( ScriptMgr *mgr )
{
	mgr->register_gameobject_script( GO_SILITHYST_MOUND, &SilithystGOAI::Create );
	mgr->register_gameobject_script( GO_SILITHYST_GEYSER, &SilithystGOAI::Create );

	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&Silithus_onEnterWorld );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_LOGOUT, (void*)&Silithus_onLogout );

	mgr->register_hook( SERVER_HOOK_EVENT_ON_AURA_REMOVE, (void*)&Silithus_onAuraRemoved );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ZONE, (void*)&Silithus_onZoneChange );
	mgr->register_hook( SERVER_HOOK_EVENT_ON_AREATRIGGER, (void*)&Silithus_onAreaTrigger );

	setupSpells();
}

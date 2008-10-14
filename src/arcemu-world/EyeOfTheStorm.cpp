/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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


#include "StdAfx.h"

static float EOTSBuffCoordinates[4][4] = {
	{ 2050.542236f, 1372.680176f, 1194.561279f, 1.67552f },
	{ 2047.728271f, 1749.736084f, 1190.198608f, -0.872665f },
	{ 2283.300049f, 1748.891235f, 1189.706787f, 1.76278f },
	{ 2302.68994140625f, 1391.27001953125f, 1197.77001953125f, -1.50098f},
};

static float EOTSBuffRotations[4][2] = {
	{ 0.681998f, -0.731354f },
	{ 0.771625f, 0.636078f },
	{ 0.422618f, -0.906308f },
	{ 0.743145f, 0.669131f },
};

uint32 EOTSbuffentrys[4] = {184964,184971,184978,184973};

const float EOTSGraveyardLocations[EOTS_TOWER_COUNT][3] = {
	{ 2012.403442f, 1455.412354f, 1172.201782f },			// BE Tower
	{ 2013.061890f, 1677.238037f, 1182.125732f },			// Fel Reaver Ruins
	{ 2355.297852f, 1683.713989f, 1173.153687f },			// Mage Tower
	{ 2351.785400f, 1455.399048f, 1185.333374f },			// Draenei Ruins
};

const float EOTSCPLocations[EOTS_TOWER_COUNT][3] = {
	{ 2048.290039f, 1393.757690f, 1194.363525f },			// BE Tower
	{ 2043.571533f, 1729.117310f, 1189.911865f },			// Fel Reaver Ruins
	{ 2284.430664f, 1731.128488f, 1189.874512f },			// Mage Tower
	{ 2285.848877f, 1402.939575f, 1197.128540f },			// Draenei Ruins
};

const float EOTSFlagLocation[3] = { 2174.718750f, 1568.766113f, 1159.958740f };
const float EOTSStartLocations[2][3] = {
	{ 2523.686035f, 1596.597290f, 1269.347656f },
	{ 1807.735962f, 1539.415649f, 1267.627319f },
};

const float EOTSBubbleLocations[2][5] = {
	{ 184719, 1803.21f, 1539.49f, 1261.09f, 3.14159f },
	{ 184720, 2527.6f, 1596.91f, 1262.13f, -3.12414f },
};

const float EOTSBubbleRotations[2][4] = {
	{ -0.173642f, -0.001515f, 0.984770f, -0.008594f },
	{ -0.173642f, -0.001515f, 0.984770f, -0.008594f },
};

static int extraHonorDiv = 12;

//===================================================
// 184083 - Draenei Tower Cap Pt, 184082 - Human Tower Cap Pt, 184081 - Fel Reaver Cap Pt, 184080 - BE Tower Cap Pt
#define EOTS_GO_BE_TOWER 184080
#define EOTS_GO_FELREAVER 184081
#define EOTS_GO_MAGE_TOWER 184082
#define EOTS_GO_DRAENEI_TOWER 184083

#define EOTS_TOWER_BE 0
#define EOTS_TOWER_FELREAVER 1
#define EOTS_TOWER_MAGE 2
#define EOTS_TOWER_DRAENEI 3

#define EOTS_BANNER_NEUTRAL 184382
#define EOTS_BANNER_ALLIANCE 184381
#define EOTS_BANNER_HORDE 184380

#define EOTS_CAPTURE_DISTANCE 900 /*30*/
const uint32 EOTSTowerIds[EOTS_TOWER_COUNT] = { EOTS_GO_BE_TOWER, EOTS_GO_FELREAVER, EOTS_GO_MAGE_TOWER, EOTS_GO_DRAENEI_TOWER };

const uint32 m_iconsStates[EOTS_TOWER_COUNT][3] = {
    {2722, 2723, 2724},
    {2725, 2726, 2727},
    {2728, 2730, 2729},
    {2731, 2732, 2733}
     };

/**
 * WorldStates
 */
#define EOTS_WORLDSTATE_DISPLAYON 2718
#define EOTS_WORLDSTATE_DISPLAYVALUE 2719
#define EOTS_WORLDSTATE_ALLIANCE_VICTORYPOINTS 2749
#define EOTS_WORLDSTATE_HORDE_VICTORYPOINTS 2750
#define EOTS_WORLDSTATE_ALLIANCE_BASES 2752
#define EOTS_WORLDSTATE_HORDE_BASES 2753
#define EOTS_NETHERWING_FLAG_SPELL 34976


#define EOTS_CAPTURE_RATE 4

EyeOfTheStorm::EyeOfTheStorm(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	m_worldStates.clear();
	m_pvpData.clear();
	m_resurrectMap.clear();

	m_playerCountPerTeam=15;
	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		EOTSm_buffs[i] = NULL;
		m_CPStatus[i] = 50;
		m_CPBanner[i] = NULL;
		m_CPStatusGO[i] = NULL;

		m_spiritGuides[i] = NULL;
	}

	m_flagHolder = 0;
	m_points[0] = m_points[1] = 0;
}

EyeOfTheStorm::~EyeOfTheStorm()
{
	for(uint32 i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if(EOTSm_buffs[i] != NULL)
		{
			EOTSm_buffs[i]->m_battleground = NULL;
			if( !EOTSm_buffs[i]->IsInWorld() )
			delete EOTSm_buffs[i];
		}
	}
}

void EyeOfTheStorm::RepopPlayersOfTeam(int32 team, Creature * sh)
{
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(sh);
	if( itr != m_resurrectMap.end() )
	{
		for( set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end(); ++it2 )
		{
			Player* r_plr = m_mapMgr->GetPlayer( *it2 );
			if( r_plr != NULL && (team < 0 || (int32)r_plr->GetTeam() == team) && r_plr->isDead() )
				HookHandleRepop( r_plr );
		}
	}
}

bool EyeOfTheStorm::HookHandleRepop(Player * plr)
{
	uint32 i;
	int32 sval;
	uint32 t = plr->GetTeam();
	float dist = 999999.0f;
	float distcur;
	LocationVector dest;

	if(plr->GetTeam() == 0)
		sval = 100;
	else
		sval = 0;

	dest.ChangeCoords( EOTSStartLocations[t][0], EOTSStartLocations[t][1], EOTSStartLocations[t][2], 0 );

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if( m_CPStatus[i] == sval )
		{
			distcur = plr->GetPositionNC().Distance2DSq( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1] );
			if( distcur < dist )
			{
				dist = distcur;
				dest.ChangeCoords( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1], EOTSGraveyardLocations[i][2], 0 );
			}
		}
	}

	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void EyeOfTheStorm::HookOnAreaTrigger(Player * plr, uint32 id)
{
	int32 tid = -1;
	int32 bonusid = -1;
	switch(id)
	{
	case 4476:			// BE Tower
		tid = EOTS_TOWER_BE;
		break;
	case 4568:			// BE Tower bonus
		bonusid = EOTS_TOWER_BE;
		break;
	case 4514:			// Fel Reaver Tower
		tid = EOTS_TOWER_FELREAVER;
		break;
	case 4569:			// Fel Reaver Tower bonus
		bonusid = EOTS_TOWER_FELREAVER;
		break;
	case 4518:			// Draenei Tower
		tid = EOTS_TOWER_DRAENEI;
		break;
	case 4571:			// Draenei Tower bonus
		bonusid = EOTS_TOWER_DRAENEI;
		break;
	case 4516:			// Mage Tower
		tid = EOTS_TOWER_MAGE;
		break;
	case 4570:			// Mage Tower bonus
		bonusid = EOTS_TOWER_MAGE;
		break;
	}

	if(bonusid > -1){
		uint32 spellid=0;
		uint32 x = (uint32)bonusid;
		if(EOTSm_buffs[x] && EOTSm_buffs[x]->IsInWorld())
		{
			spellid = EOTSm_buffs[x]->GetInfo()->sound3;
			SpellEntry * sp = dbcSpell.LookupEntryForced(spellid);
			if(sp)
			{
				Spell * pSpell = SpellPool.PooledNew();
				pSpell->Init(plr, sp, true, NULL);
				SpellCastTargets targets(plr->GetGUID());
				pSpell->prepare(&targets);
			}
			EOTSm_buffs[x]->Despawn(EOTS_BUFF_RESPAWN_TIME);
		}
	}


	if( tid < 0 )
		return;

#ifdef ANTI_CHEAT
	if(!m_started)
	{
		Anticheat_Log->writefromsession(plr->GetSession(), "%s tryed to hook the flag in eye of the storm before battleground (ID %u) started.", plr->GetName(), this->m_id);
		SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, plr->GetGUID(), "%s will be removed from the game for cheating.", plr->GetName());
		// Remove player from battleground.
		this->RemovePlayer(plr, false);
		// Kick	player from server.
		plr->Kick(6000);
		return;
	}
#endif
	uint32 spellid=0;
	uint32 x = (uint32)tid;
	if(EOTSm_buffs[x] && EOTSm_buffs[x]->IsInWorld())
	{
		spellid = EOTSm_buffs[x]->GetInfo()->sound3;
		SpellEntry * sp = dbcSpell.LookupEntryForced(spellid);
		if(sp)
		{
			Spell * pSpell = SpellPool.PooledNew();
			pSpell->Init(plr, sp, true, NULL);
			SpellCastTargets targets(plr->GetGUID());
			pSpell->prepare(&targets);
		}
		EOTSm_buffs[x]->Despawn(EOTS_BUFF_RESPAWN_TIME);
	}

	uint32 team = plr->GetTeam();
	if( plr->GetLowGUID() != m_flagHolder )
		return;

	int32 val;
	uint32 i;
	uint32 towers = 0;
	if( team == 0 )
		val = 100;
	else
		val = 0;

	if( m_CPStatus[tid] != val )
		return;			// not captured by our team

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if(m_CPStatus[i] == val)
			towers++;
	}

	/*
	Points from flag captures

	* 1 towers controlled = 75 points
	* 2 towers controlled = 85 points
	* 3 towers controlled = 100 points
	* 4 towers controlled = 500 points 
	*/

	// 25 is guessed
	const static uint32 points[5] = { 25, 75, 85, 100, 500 };
	const char * msgs[2] = { "The Alliance have captured the flag.", "The Horde have captured the flag." };

	SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE + team, 0, msgs[team] );
	GivePoints( team, points[towers] );

	m_standFlag->PushToWorld( m_mapMgr );
	m_flagHolder = 0;
	SetWorldState( 2757, 1 );

	plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
	plr->m_bgScore.Misc1++;
	UpdatePvPData();
}

void EyeOfTheStorm::HookOnPlayerDeath(Player * plr)
{
	plr->m_bgScore.Deaths++;
	
	if(plr->m_bgHasFlag)// m_flagHolder == plr->GetLowGUID() maybe need this
		plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
	
	UpdatePvPData();
}

void EyeOfTheStorm::HookFlagDrop(Player * plr, GameObject * obj)
{
	if( !m_dropFlag->IsInWorld() )
		return;

	map<uint32,uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if (itr != plr->m_forcedReactions.end()) {
		return;
	}

	m_dropFlag->RemoveFromWorld(false);
	plr->CastSpell( plr->GetGUID(), EOTS_NETHERWING_FLAG_SPELL, true );

	SetWorldState( 2757, 0 );
	PlaySoundToAll( 8212 );
	SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE + plr->GetTeam(), plr->GetGUID(), "$N has taken the flag!" );
	m_flagHolder = plr->GetLowGUID();

	event_RemoveEvents( EVENT_EOTS_RESET_FLAG );
}

void EyeOfTheStorm::HookFlagStand(Player * plr, GameObject * obj)
{
}

bool EyeOfTheStorm::HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell)
{
	if( m_flagHolder != 0 )
		return false;

	m_standFlag->RemoveFromWorld(false);
	pPlayer->CastSpell( pPlayer->GetGUID(), EOTS_NETHERWING_FLAG_SPELL, true );

	SetWorldState( 2757, 0 );
	PlaySoundToAll( 8212 );
	SendChatMessage( CHAT_MSG_BG_EVENT_ALLIANCE + pPlayer->GetTeam(), pPlayer->GetGUID(), "$N has taken the flag!" );
	m_flagHolder = pPlayer->GetLowGUID();
	return true;
}

void EyeOfTheStorm::HookOnMount(Player * plr)
{
	if( m_flagHolder == plr->GetLowGUID() )
	{
		plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
		//DropFlag( plr );
	}
}

void EyeOfTheStorm::OnAddPlayer(Player * plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
}

void EyeOfTheStorm::OnRemovePlayer(Player * plr)
{
	uint32 i;

	for( i = 0; i < EOTS_TOWER_COUNT; ++i )
	{
		m_CPDisplay[i].erase( plr );
	}

	if( m_flagHolder == plr->GetLowGUID() )
	{
		plr->RemoveAura( EOTS_NETHERWING_FLAG_SPELL );
		//DropFlag( plr );
	}

	if(!m_started)
		plr->RemoveAura(BG_PREPARATION);
}

void EyeOfTheStorm::DropFlag(Player * plr)
{
	if( m_flagHolder != plr->GetLowGUID() )
		return;

	plr->CastSpell(plr, 42792, true);

	m_dropFlag->SetPosition( plr->GetPosition() );
	m_dropFlag->PushToWorld( m_mapMgr );
	m_flagHolder = 0;

	sEventMgr.AddEvent( this, &EyeOfTheStorm::EventResetFlag, EVENT_EOTS_RESET_FLAG, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void EyeOfTheStorm::EventResetFlag()
{
	if(!m_dropFlag->IsInWorld())
		return;

	m_dropFlag->RemoveFromWorld(false);
	m_standFlag->PushToWorld(m_mapMgr);

	SetWorldState( 2757, 1 );
	PlaySoundToAll( 8192 );
	SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The flag has been reset." );
	m_flagHolder = 0;
}

void EyeOfTheStorm::OnCreate()
{
	GameObjectInfo* goi;
	uint32 i;


	/* eww worldstates */
	SetWorldState(2565, 142);
	SetWorldState(2720, 0);
	SetWorldState(2719, 0);
	SetWorldState(2718, 0);
	SetWorldState(2260, 0);
	SetWorldState(2264, 0);
	SetWorldState(2263, 0);
	SetWorldState(2262, 0);
	SetWorldState(2261, 0);
	SetWorldState(2259, 0);
	SetWorldState(2742, 0);
	SetWorldState(2741, 0);
	SetWorldState(2740, 0);
	SetWorldState(2739, 0);
	SetWorldState(2738, 0);
	SetWorldState(2737, 0);
	SetWorldState(2736, 0);
	SetWorldState(2735, 0);
	SetWorldState(2733, 0);
	SetWorldState(2732, 0);
	SetWorldState(2731, 1);
	SetWorldState(2730, 0);
	SetWorldState(2729, 0);
	SetWorldState(2728, 1);
	SetWorldState(2727, 0);
	SetWorldState(2726, 0);
	SetWorldState(2725, 1);
	SetWorldState(2724, 0);
	SetWorldState(2723, 0);
	SetWorldState(2722, 1);
	SetWorldState(2757, 1);
	SetWorldState(2753, 0);
	SetWorldState(2752, 0);
	SetWorldState(2770, 1);
	SetWorldState(2769, 1);
	SetWorldState(2750, 0);
	SetWorldState(2749, 0);
	SetWorldState(3191, 2);
	SetWorldState(3218, 0);
	SetWorldState(3217, 0);
	SetWorldState(3085, 379);

	/* create gameobjects */
	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		goi = GameObjectNameStorage.LookupEntry( EOTSTowerIds[i] );
		if( goi == NULL )
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobjects. Terminating.");
			abort();
			return;
		}

		m_CPStatusGO[i] = m_mapMgr->CreateGameObject(goi->ID);
		m_CPStatusGO[i]->CreateFromProto( goi->ID, m_mapMgr->GetMapId(), EOTSCPLocations[i][0], EOTSCPLocations[i][1], EOTSCPLocations[i][2], 0);
		m_CPStatusGO[i]->PushToWorld( m_mapMgr );

		goi = GameObjectNameStorage.LookupEntry( EOTS_BANNER_NEUTRAL );
		if( goi == NULL )
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobjects. Terminating.");
			abort();
			return;
		}

		m_CPBanner[i] = m_mapMgr->CreateGameObject(goi->ID);
		m_CPBanner[i]->CreateFromProto( goi->ID, m_mapMgr->GetMapId(), EOTSCPLocations[i][0], EOTSCPLocations[i][1], EOTSCPLocations[i][2], 0);
		m_CPBanner[i]->PushToWorld( m_mapMgr );
	}

	/* BUBBLES! */
	for( i = 0; i < 2; ++i )
	{
		m_bubbles[i] = m_mapMgr->CreateGameObject((uint32)EOTSBubbleLocations[i][0]);
		if( !m_bubbles[i]->CreateFromProto( (uint32)EOTSBubbleLocations[i][0], m_mapMgr->GetMapId(), EOTSBubbleLocations[i][1], EOTSBubbleLocations[i][2], EOTSBubbleLocations[i][3], EOTSBubbleLocations[i][4] ) )
		{
			Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "EOTS is being created and you are missing gameobjects. Terminating.");
			abort();
			return;
		}

		m_bubbles[i]->SetFloatValue(OBJECT_FIELD_SCALE_X,0.1f);
		m_bubbles[i]->SetUInt32Value(GAMEOBJECT_STATE,1);
		m_bubbles[i]->SetUInt32Value(GAMEOBJECT_FLAGS,32);
		m_bubbles[i]->SetUInt32Value(GAMEOBJECT_FACTION,114);

		m_bubbles[i]->PushToWorld( m_mapMgr );
	}

	SpawnBuff(EOTS_TOWER_DRAENEI);
	SpawnBuff(EOTS_TOWER_MAGE);
	SpawnBuff(EOTS_TOWER_FELREAVER);
	SpawnBuff(EOTS_TOWER_BE);

	/* Flag */
	m_standFlag = m_mapMgr->CreateGameObject(184141);
	m_standFlag->CreateFromProto( 184141, m_mapMgr->GetMapId(), 2174.782227f, 1569.054688f, 1160.361938f, -1.448624f );
	m_standFlag->SetFloatValue( GAMEOBJECT_ROTATION_02, 0.662620f );
	m_standFlag->SetFloatValue( GAMEOBJECT_ROTATION_03, -0.748956f );
	m_standFlag->SetFloatValue( OBJECT_FIELD_SCALE_X, 2.5f );
	m_standFlag->PushToWorld( m_mapMgr );

	m_dropFlag = m_mapMgr->CreateGameObject(184142);
	m_dropFlag->CreateFromProto( 184142, m_mapMgr->GetMapId(), 2174.782227f, 1569.054688f, 1160.361938f, -1.448624f );
	m_dropFlag->SetFloatValue( GAMEOBJECT_ROTATION_02, 0.885448f );
	m_dropFlag->SetFloatValue( GAMEOBJECT_ROTATION_03, -0.464739f );
	m_dropFlag->SetFloatValue( OBJECT_FIELD_SCALE_X, 2.5f );
}

void EyeOfTheStorm::RespawnCPFlag(uint32 i, uint32 id)
{
	m_CPBanner[i]->RemoveFromWorld(false);
	m_CPBanner[i]->SetNewGuid( m_mapMgr->GenerateGameobjectGuid() );
	m_CPBanner[i]->CreateFromProto( id, m_mapMgr->GetMapId(), m_CPBanner[i]->GetPositionX(), m_CPBanner[i]->GetPositionY(), m_CPBanner[i]->GetPositionZ(), m_CPBanner[i]->GetOrientation() );
	m_CPBanner[i]->PushToWorld( m_mapMgr );
}

void EyeOfTheStorm::UpdateCPs()
{
	uint32 i;
	set<Player*>::iterator itr, itrend;
	Player * plr;
	GameObject * go;
	int32 delta = 0;
	uint32 playercounts[2];
	uint32 towers[2] = {0,0};
	EOTSCaptureDisplayList::iterator eitr, eitr2, eitrend;
	EOTSCaptureDisplayList * disp;

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		/* loop players inrange, add any that arent in the set to the set */
		playercounts[0] = playercounts[1] = 0;
		go = m_CPStatusGO[i];
		disp = &m_CPDisplay[i];
		itr = go->GetInRangePlayerSetBegin();
		itrend = go->GetInRangePlayerSetEnd();

		for( ; itr != itrend; ++itr )
		{
			plr = *itr;
			if( plr->isAlive() && !(plr->IsStealth()) && !(plr->m_invisible) && !(plr->SchoolImmunityList[0]) && plr->GetDistance2dSq( go ) <= EOTS_CAPTURE_DISTANCE )
			{
				playercounts[plr->GetTeam()]++;

				if( disp->find( plr ) == disp->end() )
				{
					disp->insert( plr );
					plr->SendWorldStateUpdate(EOTS_WORLDSTATE_DISPLAYON, 1);
				}
			}
		}

		/* score diff calculation */
		//printf("EOTS: Playercounts = %u %u\n", playercounts[0], playercounts[1]);
		if(playercounts[0] != playercounts[1])
		{
			if(playercounts[0] > playercounts[1])
				delta = playercounts[0];
			else if(playercounts[1] > playercounts[0])
				delta = -(int32)playercounts[1];

			delta *= EOTS_CAPTURE_RATE;
			m_CPStatus[i] += delta;
			if( m_CPStatus[i] > 100 )
				m_CPStatus[i] = 100;
			else if( m_CPStatus[i] < 0 )
				m_CPStatus[i] = 0;

			// change the flag depending on cp status
			if( m_CPStatus[i] == 0 )
			{
				if( m_CPBanner[i]->GetEntry() != EOTS_BANNER_HORDE )
				{
					RespawnCPFlag(i, EOTS_BANNER_HORDE);
					if( m_spiritGuides[i] != NULL )
					{
						RepopPlayersOfTeam( 0, m_spiritGuides[i] );
						m_spiritGuides[i]->Despawn( 0, 0 );
						RemoveSpiritGuide( m_spiritGuides[i] );
						m_spiritGuides[i] = NULL;
					}
					m_spiritGuides[i] = SpawnSpiritGuide( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1], EOTSGraveyardLocations[i][2], 0, 1 );
					AddSpiritGuide( m_spiritGuides[i] );

					SetWorldState(m_iconsStates[i][0], 0);
					SetWorldState(m_iconsStates[i][1], 0);
					SetWorldState(m_iconsStates[i][2], 1);
				}

			}
			else if( m_CPStatus[i] == 100 )
			{
				if( m_CPBanner[i]->GetEntry() != EOTS_BANNER_ALLIANCE )
				{
					RespawnCPFlag(i, EOTS_BANNER_ALLIANCE);
					if( m_spiritGuides[i] != NULL )
					{
						RepopPlayersOfTeam( 1, m_spiritGuides[i] );
						m_spiritGuides[i]->Despawn( 0, 0 );
						RemoveSpiritGuide( m_spiritGuides[i] );
						m_spiritGuides[i] = NULL;
					}

					m_spiritGuides[i] = SpawnSpiritGuide( EOTSGraveyardLocations[i][0], EOTSGraveyardLocations[i][1], EOTSGraveyardLocations[i][2], 0, 0 );
					AddSpiritGuide( m_spiritGuides[i] );

					SetWorldState(m_iconsStates[i][0], 0);
					SetWorldState(m_iconsStates[i][1], 1);
					SetWorldState(m_iconsStates[i][2], 0);
				}
			}
			else
			{
				if( m_CPBanner[i]->GetEntry() != EOTS_BANNER_NEUTRAL )
				{
					RespawnCPFlag(i, EOTS_BANNER_NEUTRAL);
					if( m_spiritGuides[i] != NULL )
					{
						RepopPlayersOfTeam( -1, m_spiritGuides[i] );
						m_spiritGuides[i]->Despawn( 0, 0 );
						RemoveSpiritGuide( m_spiritGuides[i] );
						m_spiritGuides[i] = NULL;
					}
					SetWorldState(m_iconsStates[i][0], 1);
					SetWorldState(m_iconsStates[i][1], 0);
					SetWorldState(m_iconsStates[i][2], 0);
				}
			}
					UpdateIcons();
		}

		/* update the players with the new value */
		eitr = disp->begin();
		eitrend = disp->end();

		for( ; eitr != eitrend; )
		{
			plr = *eitr;
			eitr2 = eitr;
			++eitr;

			if( plr->GetDistance2dSq( go ) > EOTS_CAPTURE_DISTANCE )
			{
				disp->erase( eitr2 );
				plr->SendWorldStateUpdate(EOTS_WORLDSTATE_DISPLAYON, 0);			// hide the cp bar
			}
			else
				plr->SendWorldStateUpdate(EOTS_WORLDSTATE_DISPLAYVALUE, m_CPStatus[i]);
		}
	}

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if( m_CPStatus[i] == 100 )
			towers[0]++;
		else if( m_CPStatus[i] == 0 )
			towers[1]++;
	}

	SetWorldState( EOTS_WORLDSTATE_ALLIANCE_BASES, towers[0] );
	SetWorldState( EOTS_WORLDSTATE_HORDE_BASES, towers[1] );
}

void EyeOfTheStorm::GeneratePoints()
{
	uint32 i;
	uint32 towers[2] = {0,0};

	/*
	#  Unlike Arathi Basin, points are always generated in 2 seconds intervals no matter how many towers are controlled by both teams.
	# Each claimed tower generates victory points for the controlling team. The more towers your team owns, the faster your team gains points

	* 1 tower controlled = 1 point/tick (0.5 points per second)
	* 2 towers controlled = 2 points/tick (1 point per second)
	* 3 towers controlled = 5 points/tick (2.5 points per second)
	* 4 towers controlled = 10 points/tick (5 points per second) 

	*/
	uint32 pointspertick[5] = { 0, 1, 2, 5, 10 };

	for(i = 0; i < EOTS_TOWER_COUNT; ++i)
	{
		if(m_CPStatus[i] == 100)
			towers[0]++;
		else if(m_CPStatus[i] == 0)
			towers[1]++;
	}

	for( i = 0; i < 2; ++i )
	{
		if( towers[i] == 0 )
		{
			//printf("EOTS: No points on team %u\n", i);
			continue;
		}

		if( GivePoints( i, pointspertick[towers[i]] ) )
			return;
	}
}


void EyeOfTheStorm::UpdateIcons()
{
 	for(uint32 i = 0; i < 4; i++)
 	{
 		if(m_CPStatus[i] == 0)
		{
 			SetWorldState(m_iconsStates[i][0], 0);
 			SetWorldState(m_iconsStates[i][1], 0);
 			SetWorldState(m_iconsStates[i][2], 1);
 		} 
		else if(m_CPStatus[i] == 100)
 		{
 			SetWorldState(m_iconsStates[i][0], 0);
 			SetWorldState(m_iconsStates[i][1], 1);
 			SetWorldState(m_iconsStates[i][2], 0);
 		} 
		else
		{
 			SetWorldState(m_iconsStates[i][0], 1);
 			SetWorldState(m_iconsStates[i][1], 0);
 			SetWorldState(m_iconsStates[i][2], 0);
 		}
 	}
}

bool EyeOfTheStorm::GivePoints(uint32 team, uint32 points)
{
	uint32 honor;
	//printf("EOTS: Give team %u %u points.\n", team, points);

	m_points[team] += points;
	if( m_points[team] >= 2000 )
	{
		m_points[team] = 2000;

		m_ended = true;
		m_winningteam = team;
		m_nextPvPUpdateTime = 0;

		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(((CBattleground*)this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

		/* add the marks of honor to all players */
		m_mainLock.Acquire();

		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				(*itr)->Root();
				
				if ( (*itr)==NULL )// never happen?
					continue;

				/* Winning team will gain 2000 / 12 = 166.6 extra honor points */
				/* On weekend 2000 / 8 = 250 (guessing) */
				/* Losing team will also gain the honor from already earned points */
				honor = m_points[i] / extraHonorDiv;
				(*itr)->m_bgScore.BonusHonor += honor;
				HonorHandler::AddHonorPointsToPlayer((*itr), honor);

				if(i == m_winningteam)
				{				
					Item *item;
					item = objmgr.CreateItem( 29024 , *itr);
					item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,3);
					item->SoulBind();
					if(!(*itr)->GetItemInterface()->AddItemToFreeSlot(item))
					{
						(*itr)->GetSession()->SendNotification("No free slots were found in your inventory!");
						item->DeleteMe();
					}
					else
					{
						SlotResult *lr = (*itr)->GetItemInterface()->LastSearchResult();
						(*itr)->GetSession()->SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,3);
					}
				}
				else
				{
					Item *item;
					item = objmgr.CreateItem( 29024 , *itr);
					item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,1);
					item->SoulBind();
					if(!(*itr)->GetItemInterface()->AddItemToFreeSlot(item))
					{
						(*itr)->GetSession()->SendNotification("No free slots were found in your inventory!");
						item->DeleteMe();
					}
					else
					{
						SlotResult *lr = (*itr)->GetItemInterface()->LastSearchResult();
						(*itr)->GetSession()->SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,1);
					}
				}
			}
		}
		m_mainLock.Release();
		SetWorldState( EOTS_WORLDSTATE_ALLIANCE_VICTORYPOINTS + team, m_points[team] );
		UpdatePvPData();
		return true;
	}

	SetWorldState( EOTS_WORLDSTATE_ALLIANCE_VICTORYPOINTS + team, m_points[team] );
	return false;
}

void EyeOfTheStorm::HookOnPlayerKill(Player * plr, Unit * pVictim)
{
	if ( pVictim->IsPlayer() )
	{
		plr->m_bgScore.KillingBlows++;
		UpdatePvPData();
	}
}

void EyeOfTheStorm::HookOnHK(Player * plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void EyeOfTheStorm::SpawnBuff(uint32 x)
{
	uint32 chosen_buffid = EOTSbuffentrys[RandomUInt(3)];
	GameObjectInfo * goi = GameObjectNameStorage.LookupEntry(chosen_buffid);
	if(goi == NULL)
		return;

	if(EOTSm_buffs[x] == NULL)
	{
		EOTSm_buffs[x] = SpawnGameObject(chosen_buffid, m_mapMgr->GetMapId(), EOTSBuffCoordinates[x][0], EOTSBuffCoordinates[x][1], EOTSBuffCoordinates[x][2], EOTSBuffCoordinates[x][3], 0, 114, 1);

		EOTSm_buffs[x]->SetFloatValue(GAMEOBJECT_ROTATION_02, EOTSBuffRotations[x][0]);
		EOTSm_buffs[x]->SetFloatValue(GAMEOBJECT_ROTATION_03, EOTSBuffRotations[x][1]);
		EOTSm_buffs[x]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		EOTSm_buffs[x]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		EOTSm_buffs[x]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		EOTSm_buffs[x]->PushToWorld(m_mapMgr);
	}
	else
	{
		if(EOTSm_buffs[x]->IsInWorld())
			EOTSm_buffs[x]->RemoveFromWorld(false);

		if(chosen_buffid != EOTSm_buffs[x]->GetEntry())
		{
			EOTSm_buffs[x]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
			EOTSm_buffs[x]->SetUInt32Value(OBJECT_FIELD_ENTRY, chosen_buffid);
			EOTSm_buffs[x]->SetInfo(goi);
		}

		EOTSm_buffs[x]->PushToWorld(m_mapMgr);
}
}

LocationVector EyeOfTheStorm::GetStartingCoords(uint32 Team)
{
	return LocationVector( EOTSStartLocations[Team][0], 
		EOTSStartLocations[Team][1],
		EOTSStartLocations[Team][2] );
}

void EyeOfTheStorm::OnStart()
{
	for(uint32 i = 0; i < 2; ++i) {
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr) {
			(*itr)->RemoveAura(BG_PREPARATION);
		}
	}

	uint32 i;

	/* start the events */
	sEventMgr.AddEvent(this, &EyeOfTheStorm::GeneratePoints, EVENT_EOTS_GIVE_POINTS, 2000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	sEventMgr.AddEvent(this, &EyeOfTheStorm::UpdateCPs, EVENT_EOTS_CHECK_CAPTURE_POINT_STATUS, 5000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	/* spirit guides */
	AddSpiritGuide(SpawnSpiritGuide( EOTSStartLocations[0][0], EOTSStartLocations[0][1], EOTSStartLocations[0][2], 0, 0 ));
	AddSpiritGuide(SpawnSpiritGuide( EOTSStartLocations[1][0], EOTSStartLocations[1][1], EOTSStartLocations[1][2], 0, 1 ));

	/* remove the bubbles */
	for( i = 0; i < 2; ++i )
	{
		m_bubbles[i]->RemoveFromWorld(false);
		delete m_bubbles[i];
		m_bubbles[i] = NULL;
	}
	m_started = true;
}

void EyeOfTheStorm::HookOnShadowSight() 
{
}

void EyeOfTheStorm::SetIsWeekend(bool isweekend) 
{
	if (isweekend)
	{
		extraHonorDiv = 8;
	}
	else
	{
		extraHonorDiv = 12;
	}
}


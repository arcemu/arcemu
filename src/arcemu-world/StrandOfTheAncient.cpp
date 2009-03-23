/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

* Banners
	* Flagpole, 191311
	* Alliance Banner, 191310
	* Alliance Banner, 191306
	* Alliance Banner, 191308
	* Alliance Banner Aura, 180100
	* Horde Banner, 191307
	* Horde Banner, 191309
	* Horde Banner, 191305
	* Horde Banner Aura, 180101

* Buffs
	* Restoration Buff, 184977
	* Restoration Buff, 184971
	* Restoration Buff, 184965

* Seaforium (Mount Projectiles)
	* Seaforium Barrel, 194086 (Not used atm)
	* Seaforium Barrel, 190753
	* Massive Seaforium Charge, 190752

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

* SQL Patched (Build 2177)
	npc_gossip_textid
			30586	13832	// Jojindi
			29234	13832	// Strand of the Ancients Battlemaster - Wintergrasp
			30578	13832	// Bethany Aldire - Stormwind City
			30581	13832	// Buhurda - Exodar?
			30590	13832	// Godo Cloudcleaver - Thunder Bluff
			30584	13832	// Mabrian Fardawn - Silvermoon?
			30579	13832	// Marga Bearbrawn - Ironforge
			30580	13832	// Nivara Bladedancer - Darnassus
			30583	13832	// Sarah Forthright - Undercity
			30582	13832	// Ufuda Giant-Slayer - Orgrimmar
			30587	13832	// Vinvo Goldgear - Shattrath

* SQL Patched (Build 2172)
	creature_protos (for above creatures)
			npcflags = 1048577

* Fix level requirements to join the battleground. And fix which npc text is used
  for the battlemaster gossip scripts, displaying the proper of 3 messages
	npc_text
		 13832 = You are not yet strong enough to do battle in the Strand of the Ancients. Return when you have gained more experience.
		 13834 = We cannot allow the Alliance to turn the hidden secrets of the Strand of the Ancients against us. Will you join our brave warriors there?
		+13833 = We cannot allow the Horde to turn the hidden secrets of the Strand of the Ancients against us. Will you join our brave warriors there?

* Battles consist of two 10 minute rounds

* Rounds end whe time expires or when the relic is captured.

* Who defends first and attacks first should be random.

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

/*
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 Team = plr->GetTeam();
		if(Team > 1) Team = 1;

		// Check if the player can be entered into the bg or not.
		if(plr->getLevel() < 71) //Levels 71-80 else don't let them into the bg menu.
		{
			// Send "you cannot enter" message.
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13832, plr);
		}
		else
		{
			uint32 FactMessages[2] = { 13833, 13834 }; // NEED TO BE UPDATED TO SOTA

			// Ask to enter battleground
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
			Menu->AddItem( 0, "I would like to enter the battleground.", BATTLEGROUND_STRAND_OF_THE_ANCIENT);
		}

		if (AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
	{
		// Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		plr->GetSession()->SendBattlegroundList(((Creature*)pObject), BATTLEGROUND_STRAND_OF_THE_ANCIENT);
	}
*/

#define GO_RELIC 192834
const float sotaTitanRelic[4] = { 836.5f, -108.8f, 111.59f, 0.0f };

const uint32 GateGOIds[6] = {
	190722,	// Gate of the Green Emerald
	190727,	// Gate of the Yellow Moon
	190724,	// Gate of the Blue Sapphire
	190726,	// Gate of the Red Sun
	190723,	// Gate of the Purple Amethyst
	192549,	// Chamber of Ancient Relics
};
const float sotaGates[GATE_COUNT][4] = {
	{ 1411.57f, 108.163f, 28.692f, 5.441f },
	{ 1055.452f, -108.1f, 82.134f, 0.034f },
	{ 1431.3413f, -219.437f, 30.893f, 0.9736f },
	{ 1227.667f, -212.555f, 55.372f, 0.5023f },
	{ 1214.681f, 81.21f, 53.413f, 5.745f },
};
const float sotaChamberGate[4] = { 878.555f, -108.989f, 119.835f, 0.0565f };

// Things radiating out from the gates... same orientation as door.
const uint32 GateSigilGOIds[5] =  { 192687, 192685, 192689, 192690, 192691, };
const float sotaGateSigils[GATE_COUNT][4] = {
	{ 1414.054f, 106.72f, 41.442f, 5.441f },
	{ 1060.63f, -107.8f, 94.7f, 0.034f },
	{ 1433.383f, -216.4f, 43.642f, 0.9736f },
	{ 1230.75f, -210.724f, 67.611f, 0.5023f },
	{ 1217.8f, 79.532f, 66.58f, 5.745f },
};

// Defender transporter platform locations
const float sotaTransporters[GATE_COUNT][4] = {
	{ 1394.0444f, 72.586f, 31.0535f, 0.0f },
	{ 1065.0f, -89.7f, 81.08f, 0.0f },
	{ 1467.95f, -225.67f, 30.9f, 0.0f },
	{ 1255.93f, -233.382f, 56.44f, 0.0f },
	{ 1215.679f, 47.47f, 54.281f, 0.0f },
};

// Defender transporter destination locations
const float sotaTransporterDestination[GATE_COUNT][4] = {
	{ 1388.94f, 103.067f, 34.49f, 5.4571f },
	{ 1043.69f, -87.95f, 87.12f, 0.003f },
	{ 1441.0411f, -240.974f, 35.264f, 0.949f },
	{ 1228.342f, -235.234f, 60.03f, 0.4584f },
	{ 1193.857f, 69.9f, 58.046f, 5.7245f },
};

// Two guns per gate, GUN_LEFT and GUN_RIGHT
const float sotaGunMounts[GATE_COUNT][2][4] = {
	{ { 1436.429f, 110.05f, 41.407f, 5.4f }, { 1404.9023f, 84.758f, 41.183f, 5.46f } },
	{ { 1068.693f, -86.951f, 93.81f, 0.02f }, { 1068.83f, -127.56f, 96.45f, 0.0912f } },
	{ { 1422.115f, -196.433f, 42.1825f, 1.0222f }, { 1454.887f, -220.454f, 41.956f, 0.9627f } },
	{ { 1232.345f, -187.517f, 66.945f, 0.45f }, { 1249.634f, 224.189f, 66.72f, 0.635f } },
	{ { 1236.213f, 92.287f, 64.965f, 5.751f }, { 1215.11f, 57.772f, 64.739f, 5.78f } },
};

// ---- Verify remaining ----- //

// This change as the game progresses
const float sotaRepop[2][4] = {
	{ 1600.0f, 58.3f, 11.0f, 2.98f },
	{ 1600.0f, 58.3f, 11.0f, 2.98f },
};

// There should only be two boats. boats three and four here
// are a lazy hack for not wanting to program the boats to move via waypoints
const float sotaBoats[4][4] = {
	{ 1623.34f, 37.0f, 1.0f, 3.65f },
	{ 2439.4f, 845.38f, 1.0f, 3.35f },
	{ 1623.34f, 37.0f, 1.0f, 3.65f },
	{ 2439.4f, 845.38f, 1.0f, 3.35f },
};
const float sotaAttackerStartingPosition[2][4] = {
	{ 2445.288f, 849.35f, 10.0f, 3.76f },
	{ 2445.288f, 849.35f, 10.0f, 3.76f },
};
const float sotaStopBoatsPlayer[2][4] = {
	{ 1624.7f, 42.93f, 10.0f, 2.63f },
	{ 1624.7f, 42.93f, 10.0f, 2.63f },
};
const float sotaDefenderStartingPosition[4] = { 1209.7f, -65.16f, 70.1f, 0.0f };

const float sotaStartingPosition[2][4] = {
	{ 2445.288f, 849.35f, 9.25f, 3.76f },
	{ 1209.7f, -65.16f, 70.1f, 0.0f },
};

// We'll need to borrow this from elsewhere
float CalculateDistance(float x1, float y1, float z1, float x2, float y2, float z2);

StrandOfTheAncient::StrandOfTheAncient(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	//m_worldStates.clear();
	m_pvpData.clear();
	m_resurrectMap.clear();

	uint32 mapId = BattlegroundManager.GetMap(BATTLEGROUND_STRAND_OF_THE_ANCIENT);

	// Boats
	for (int i = 0; i < 4; i++)
	{
		m_boats[i] = m_mapMgr->CreateAndSpawnGameObject(20808,
			sotaBoats[i][0], sotaBoats[i][1], sotaBoats[i][2], sotaBoats[i][3], 1.0f);
		m_boats[i]->PushToWorld(mgr);
	}

	/* Relic */
	m_relic = m_mapMgr->CreateAndSpawnGameObject(GO_RELIC, sotaTitanRelic[0],
		sotaTitanRelic[1], sotaTitanRelic[2], sotaTitanRelic[3], 1.0f);

	for (i = 0; i < GATE_COUNT; i++)
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


	/* create the buffs */
	for(i = 0; i < BUFF_COUNT; ++i)
		SpawnBuff(i);

}

StrandOfTheAncient::~StrandOfTheAncient()
{
	if (m_relic && !m_relic->IsInWorld())
	{
		delete m_relic;
		m_relic = 0;
	}

	for (int i = 0; i < GATE_COUNT; i++)
	{
		if (m_gates[i] && !m_gates[i]->IsInWorld())
		{
			delete m_gates[i];
			m_gates[i] = 0;
		}
		if (m_gateSigils[i] && !m_gateSigils[i]->IsInWorld())
		{
			delete m_gateSigils[i];
			m_gateSigils[i] = 0;
		}
		if (m_gateTransporters[i] && !m_gateTransporters[i]->IsInWorld())
		{
			delete m_gateTransporters[i];
			m_gateTransporters[i] = 0;
		}
	}

	if (m_endgate && !m_endgate->IsInWorld())
	{
		delete m_endgate;
		m_endgate = 0;
	}

	for (uint32 i = 0; i < BUFF_COUNT; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && !m_buffs[i]->IsInWorld())
		{
			delete m_buffs[i];
			m_buffs[i] = 0;
		}
	}

}


void StrandOfTheAncient::HookOnAreaTrigger(Player * plr, uint32 id)
{

}

void StrandOfTheAncient::HookOnPlayerKill(Player * plr, Player * pVictim)
{
	plr->m_bgScore.KillingBlows++;
	UpdatePvPData();
}

void StrandOfTheAncient::HookOnHK(Player * plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void StrandOfTheAncient::OnPlatformTeleport(Player *plr)
{
	LocationVector dest;
	uint32 closest_platform = 0;

	for (uint32 i = 0; i < GATE_COUNT; i++)
	{
		float distance = CalculateDistance(plr->GetPositionX(),
			plr->GetPositionY(), plr->GetPositionZ(),
			sotaTransporterDestination[i][0],
			sotaTransporterDestination[i][1],
			sotaTransporterDestination[i][2]);
		if (distance < 75)
		{
			closest_platform = i;
			break;
		}
	}
	dest.ChangeCoords(sotaTransporterDestination[closest_platform][0],
		sotaTransporterDestination[closest_platform][1],
		sotaTransporterDestination[closest_platform][2],
		sotaTransporterDestination[closest_platform][3]);

	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
}

void StrandOfTheAncient::OnAddPlayer(Player * plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
	//plr->m_CurrentTransporter = m_boat[0];
	sota_players.push_back(plr);
}

void StrandOfTheAncient::OnRemovePlayer(Player * plr)
{
	plr->RemoveAura(BG_PREPARATION);
	sota_players.remove(plr);
}

LocationVector StrandOfTheAncient::GetStartingCoords(uint32 team)
{
	/*
	 *	This needs to be flexible, we can't be starting players in the ocean for
	 *	late arrivals. Also Repop locations need to change as the attack progresses.
	 */

	return LocationVector(sotaStartingPosition[team][0],
		sotaStartingPosition[team][1], sotaStartingPosition[team][2],
		sotaStartingPosition[team][3]);
}

void StrandOfTheAncient::HookOnPlayerDeath(Player * plr)
{
	plr->m_bgScore.Deaths++;
	UpdatePvPData();
}

void StrandOfTheAncient::HookOnMount(Player * plr)
{
	/* Allowed */
}

bool StrandOfTheAncient::HookHandleRepop(Player * plr)
{
	LocationVector dest;

	dest.ChangeCoords(sotaRepop[plr->GetTeam()][0], sotaRepop[plr->GetTeam()][1],
		sotaRepop[plr->GetTeam()][2], sotaRepop[plr->GetTeam()][3]);

	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void StrandOfTheAncient::SpawnBuff(uint32 x)
{
	uint32 mapid = BattlegroundManager.GetMap(BATTLEGROUND_STRAND_OF_THE_ANCIENT);
    switch(x)
	{
	case 0:
		m_buffs[x] = SpawnGameObject(184977, mapid, 1449.9296875f, 1470.70971679688f, 342.634552001953f, -1.64060950279236f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.73135370016098f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,-0.681998312473297f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 1:
		m_buffs[x] = SpawnGameObject(184971, mapid, 1005.17071533203f, 1447.94567871094f, 335.903228759766f, 1.64060950279236f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.73135370016098f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,0.681998372077942f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	case 2:
		m_buffs[x] = SpawnGameObject(184965, mapid, 1317.50573730469f, 1550.85070800781f, 313.234375f, -0.26179963350296f, 0, 114, 1);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_02,0.130526319146156f);
		m_buffs[x]->SetFloatValue(GAMEOBJECT_PARENTROTATION_03,-0.991444826126099f);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 1, 6);
		m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
		break;
	}
}

void StrandOfTheAncient::OnCreate()
{
	sLog.outDebug("OnCreate: SOTA Battleground\n");

	SetWorldState(WORLDSTATE_SOTA_CAPTURE_BAR_DISPLAY, 0);
	SetWorldState(WORLDSTATE_SOTA_CAPTURE_BAR_VALUE, 0);
	PrepareRound();
	SetWorldState(WORLDSTATE_SOTA_BONUS_TIME, 0);
	SetWorldState(WORLDSTATE_SOTA_TIMER_1, 0);
	SetWorldState(WORLDSTATE_SOTA_TIMER_2, 0);
	SetWorldState(WORLDSTATE_SOTA_TIMER_3, 0);
}

void StrandOfTheAncient::OnStart()
{
	sLog.outDebug("OnStart: SOTA Battleground\n");

	LocationVector dest;
	m_started = true;

	for(list<Player*>::iterator itr = sota_players.begin(); itr != sota_players.end(); ++itr)
	{
		Player *plr = *itr;
		dest.ChangeCoords(sotaStopBoatsPlayer[plr->GetTeam()][0], sotaStopBoatsPlayer[plr->GetTeam()][1],
			sotaStopBoatsPlayer[plr->GetTeam()][2], sotaStopBoatsPlayer[plr->GetTeam()][3]);

		plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	}

	SetWorldState(WORLDSTATE_SOTA_CAPTURE_BAR_DISPLAY, (uint32)-1);
	SetWorldState(WORLDSTATE_SOTA_CAPTURE_BAR_VALUE, (uint32)-1);
	SetWorldState(WORLDSTATE_SOTA_BONUS_TIME, 1);
	SetWorldState(WORLDSTATE_SOTA_TIMER_1, 10); // 10 Minute Timer

	//SetTime(ROUND_LENGTH, 0);
	sEventMgr.AddEvent( this, &StrandOfTheAncient::TimeTick, EVENT_SOTA_TIMER, MSTIME_SECOND * 5, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

	UpdatePvPData();
}

void StrandOfTheAncient::HookGenerateLoot(Player *plr, Object * pOCorpse)
{
	sLog.outDebug("*** StrandOfTheAncient::HookGenerateLoot");
}

void StrandOfTheAncient::HookOnUnitKill(Player * plr, Unit * pVictim)
{
	sLog.outDebug("*** StrandOfTheAncient::HookOnUnitKill");
}

void StrandOfTheAncient::SetIsWeekend(bool isweekend) 
{
	sLog.outDebug("*** StrandOfTheAncient::SetIsWeekend");
	m_isWeekend = isweekend;
}

// end game, attackers captured flag
bool StrandOfTheAncient::HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell)
{
	sLog.outDebug("*** StrandOfTheAncient::HookSlowLockOpen - Relic Captured");
	PlaySoundToAll( 8212 );

	/*
	// Root everyone, THEN Start next round, and Unroot -- OR -- Shutdown battleground.
	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
	sEventMgr.AddEvent(((CBattleground*)this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

	// increment the score world state
	SetWorldState(pPlayer->GetTeam() ? WSG_CURRENT_HORDE_SCORE : WSG_CURRENT_ALLIANCE_SCORE, m_scores[pPlayer->GetTeam()]);
	this->GetMapMgr()->SetWorldState

	UpdatePvPData();
	*/
	return true;
}

// For banners
void StrandOfTheAncient::HookFlagStand(Player * plr, GameObject * obj)
{
}

// time in seconds
void StrandOfTheAncient::SetTime(uint32 secs, uint32 WorldState)
{
	uint32 minutes = secs / TIME_MINUTE;
	uint32 seconds = secs % TIME_MINUTE;
	uint32 digits[3];
	digits[0] = minutes;
	digits[1] = seconds / 10;
	digits[2] = seconds % 10;

	//m_mapMgr->GetStateManager().UpdateWorldState( WorldState, 1 );
	SetWorldState( WORLDSTATE_SOTA_TIMER_1, digits[0] );
	SetWorldState( WORLDSTATE_SOTA_TIMER_2, digits[1] );
	SetWorldState( WORLDSTATE_SOTA_TIMER_3, digits[2] );
	SetRoundTime( secs );
}

void StrandOfTheAncient::PrepareRound()
{
	//SetWorldState( WORLDSTATE_SOTA_ALLIANCE_DEFENDER, Attackers == HORDE ? 1 : 0 );
	//SetWorldState( WORLDSTATE_SOTA_ALLIANCE_ATTACKER, Attackers == HORDE ? 0 : 1 );
	SetWorldState( WORLDSTATE_SOTA_ALLIANCE_DEFENDER, 1);
	SetWorldState( WORLDSTATE_SOTA_ALLIANCE_ATTACKER, 0);
};

void StrandOfTheAncient::TimeTick()
{
	SetTime(GetRoundTime() - 5,0);
	if(GetRoundTime() == 0)
	{
		sEventMgr.RemoveEvents(this, EVENT_SOTA_TIMER);
	}
};

// Not used?
void StrandOfTheAncient::HookOnFlagDrop(Player * plr)
{
}
void StrandOfTheAncient::HookFlagDrop(Player * plr, GameObject * obj)
{
}
void StrandOfTheAncient::HookOnShadowSight() 
{
}

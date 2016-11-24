/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"
#include "ArathiBasin.h"

#define BASE_RESOURCES_GAIN 10
#define RESOURCES_WARNING_THRESHOLD 1400
#define RESOURCES_WINVAL 1600

uint32 buffentries[3] = {180380, 180362, 180146};

/* AB Battleground Data */

static float GraveyardLocations[AB_NUM_CONTROL_POINTS][3] =
{
	{ 1201.869507f, 1163.130615f, -56.285969f },												// STABLES
	{ 834.726379f, 784.978699f, -57.081944f },													// FARM
	{ 1016.588318f, 955.184692f, -42.828693f },													// BLACKSMITH
	{ 1211.523682f, 781.556946f, -82.709511f },													// MINE
	{ 772.755676f, 1213.113770f, 15.797392f },													// LUMBERMILL
};

static float NoBaseGYLocations[2][3] =
{
	{ 1354.699951f, 1270.270020f, -11.129100f },												// ALLIANCE
	{ 713.710022f, 638.364014f, -10.599900f },													// HORDE
};

static const char* ControlPointNames[AB_NUM_CONTROL_POINTS] =
{
	"Stable",																					// STABLE
	"Farm",																						// FARM
	"Blacksmith",																				// BLACKSMITH
	"Mine",																						// MINE
	"Lumber Mill",																				// LUMBERMILL
};

static uint32 ControlPointGoIds[AB_NUM_CONTROL_POINTS][AB_NUM_SPAWN_TYPES] =
{
	// NEUTRAL    ALLIANCE-ATTACK    HORDE-ATTACK    ALLIANCE-CONTROLLED    HORDE_CONTROLLED
	{ 180087,       180085,            180086,         180076,                180078 },			// STABLE
	{ 180089,       180085,            180086,         180076,                180078 },			// FARM
	{ 180088,       180085,            180086,         180076,                180078 },			// BLACKSMITH
	{ 180091,       180085,            180086,         180076,                180078 },			// MINE
	{ 180090,       180085,            180086,         180076,                180078 },			// LUMBERMILL
};

static float ControlPointCoordinates[AB_NUM_CONTROL_POINTS][4] =
{
	{ 1166.779541f, 1200.147583f, -56.701763f, -2.251474f },									// STABLE
	{ 806.2484741f, 874.2167358f, -55.9936981f, 0.8377580f },									// FARM
	{ 977.0503540f, 1046.5208740f, -44.8276138f, 0.5410520f },									// BLACKSMITH
	{ 1146.9224854f, 848.1899414f, -110.9200210f, 2.4260077f },									// MINE
	{ 856.141907f, 1148.902100f, 11.184692f, -2.303835f },										// LUMBERMILL
};

static float ControlPointRotations[AB_NUM_CONTROL_POINTS][2] =
{
	{ 0.9025853f, -0.4305111f },																// STABLE
	{ 0.4067366f, 0.9135454f },																	// FARM
	{ 0.2672384f, 0.9636304f },																	// BLACKSMITH
	{ 0.9366722f, 0.3502073f },																	// MINE
	{ 0.9135455f, -0.4067366f },																// LUMBERMILL
};

static float BuffCoordinates[AB_NUM_CONTROL_POINTS][4] =
{
	{ 1185.56616210938f, 1184.62854003906f, -56.3632850646973f, 2.30383467674255f },			// STABLE
	{ 990.113098144531f, 1008.73028564453f, -42.6032752990723f, 0.820304811000824f },			// FARM
	{ 816.906799f, 842.339844f, -56.538746f, 3.272740f },										// BLACKSMITH
	{ 808.846252441406f, 1185.41748046875f, 11.9216051101685f, -0.663225054740906f },			// MINE
	{ 1147.09057617188f, 816.836242675781f, -98.3989562988281f, -0.226892784237862f },			// LUMBERMILL
};

static float BuffRotations[AB_NUM_CONTROL_POINTS][2] =
{
	{ 0.913545489311218f, 0.406736612319946f },													// STABLE
	{ 0.39874908328056f, 0.917060077190399f },													// FARM
	{ 0.913545489311218f, 0.406736612319946f },													// BLACKSMITH
	{ 0.325568109750748f, -0.945518612861633f },												// MINE
	{ 0.113203197717667f, -0.993571877479553f },												// LUMBERMILL
};

static uint32 AssaultFields[AB_NUM_CONTROL_POINTS][2] =
{
	{ WORLDSTATE_AB_CAPTURING_STABLES_ALLIANCE, WORLDSTATE_AB_CAPTURING_STABLES_HORDE },								// STABLE
	{ WORLDSTATE_AB_CAPTURING_FARM_ALLIANCE, WORLDSTATE_AB_CAPTURING_FARM_HORDE },									// FARM
	{ WORLDSTATE_AB_CAPTURING_BLACKSMITH_ALLIANCE, WORLDSTATE_AB_CAPTURING_BLACKSMITH_HORDE },						// BLACKSMITH
	{ WORLDSTATE_AB_CAPTURING_GOLDMINE_ALLIANCE, WORLDSTATE_AB_CAPTURING_GOLDMINE_HORDE },							// MINE
	{ WORLDSTATE_AB_CAPTURING_LUMBERMILL_ALLIANCE, WORLDSTATE_AB_CAPTURING_LUMBERMILL_HORDE },						// LUMBERMILL
};

static uint32 OwnedFields[AB_NUM_CONTROL_POINTS][2] =
{
	{ WORLDSTATE_AB_CAPTURED_STABLES_ALLIANCE, WORLDSTATE_AB_CAPTURED_STABLES_HORDE },								// STABLE
	{ WORLDSTATE_AB_CAPTURED_FARM_ALLIANCE, WORLDSTATE_AB_CAPTURED_FARM_HORDE },										// FARM
	{ WORLDSTATE_AB_CAPTURED_BLACKSMITH_ALLIANCE, WORLDSTATE_AB_CAPTURED_BLACKSMITH_HORDE },							// BLACKSMITH
	{ WORLDSTATE_AB_CAPTURED_GOLDMINE_ALLIANCE, WORLDSTATE_AB_CAPTURED_GOLDMINE_HORDE },								// MINE
	{ WORLDSTATE_AB_CAPTURED_LUMBERMILL_ALLIANCE, WORLDSTATE_AB_CAPTURED_LUMBERMILL_HORDE },							// LUMBERMILL
};

static uint32 NeutralFields[AB_NUM_CONTROL_POINTS] =
{
	WORLDSTATE_AB_SHOW_STABLE_ICON,
	WORLDSTATE_AB_SHOW_FARM_ICON,
	WORLDSTATE_AB_SHOW_BACKSMITH_ICON,
	WORLDSTATE_AB_SHOW_GOLDMINE_ICON,
	WORLDSTATE_AB_SHOW_LUMBERMILL_ICON,
};

static uint32 ResourceUpdateIntervals[6] =
{
	0,
	12000, // 12 secnods
	9000, // 9 seconds
	6000, // 6 seconds
	3000, // 3 seconds
	1000, // 1 second
};

static uint32 PointBonusPerUpdate[6] =
{
	0,
	10,
	10,
	10,
	10,
	30,
};

static uint32 resourcesToGainBH = 260;
static uint32 resourcesToGainBR = 160;

/* End BG Data */

void ArathiBasin::SpawnBuff(uint32 x)
{
	uint32 chosen_buffid = buffentries[RandomUInt(2)];
	GameObjectInfo* goi = GameObjectNameStorage.LookupEntry(chosen_buffid);
	if(goi == NULL)
		return;

	if(m_buffs[x] == NULL)
	{
		m_buffs[x] = SpawnGameObject(chosen_buffid, m_mapMgr->GetMapId(), BuffCoordinates[x][0], BuffCoordinates[x][1], BuffCoordinates[x][2],
		                             BuffCoordinates[x][3], 0, 114, 1);

		m_buffs[x]->SetParentRotation(2, BuffRotations[x][0]);
		m_buffs[x]->SetParentRotation(3, BuffRotations[x][1]);
		m_buffs[x]->SetState(GAMEOBJECT_STATE_CLOSED);
		m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
		m_buffs[x]->SetAnimProgress(100);
		m_buffs[x]->PushToWorld(m_mapMgr);
	}
	else
	{
		// only need to reassign guid if the entry changes.
		if(m_buffs[x]->IsInWorld())
			m_buffs[x]->RemoveFromWorld(false);

		if(chosen_buffid != m_buffs[x]->GetEntry())
		{
			m_buffs[x]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
			m_buffs[x]->SetEntry(chosen_buffid);
			m_buffs[x]->SetInfo(goi);
		}

		m_buffs[x]->PushToWorld(m_mapMgr);
	}
}

void ArathiBasin::SpawnControlPoint(uint32 Id, uint32 Type)
{
	GameObjectInfo* gi, * gi_aura;
	gi = GameObjectNameStorage.LookupEntry(ControlPointGoIds[Id][Type]);
	if(gi == NULL)
		return;

	gi_aura = gi->raw.sound3 ? GameObjectNameStorage.LookupEntry(gi->raw.sound3) : NULL;

	if(m_controlPoints[Id] == NULL)
	{
		m_controlPoints[Id] = SpawnGameObject(gi->ID, m_mapMgr->GetMapId(), ControlPointCoordinates[Id][0], ControlPointCoordinates[Id][1],
		                                      ControlPointCoordinates[Id][2], ControlPointCoordinates[Id][3], 0, 35, 1.0f);

		m_controlPoints[Id]->SetParentRotation(2, ControlPointRotations[Id][0]);
		m_controlPoints[Id]->SetParentRotation(3, ControlPointRotations[Id][1]);
		m_controlPoints[Id]->SetState(GAMEOBJECT_STATE_CLOSED);
		m_controlPoints[Id]->SetType(static_cast<uint8>(gi->Type));
		m_controlPoints[Id]->SetAnimProgress(100);
		m_controlPoints[Id]->Activate();
		m_controlPoints[Id]->SetDisplayId(gi->DisplayID);

		switch(Type)
		{
			case AB_SPAWN_TYPE_ALLIANCE_ASSAULT:
			case AB_SPAWN_TYPE_ALLIANCE_CONTROLLED:
				m_controlPoints[Id]->SetFaction(2);
				break;

			case AB_SPAWN_TYPE_HORDE_ASSAULT:
			case AB_SPAWN_TYPE_HORDE_CONTROLLED:
				m_controlPoints[Id]->SetFaction(1);
				break;

			default:
				m_controlPoints[Id]->SetFaction(35);		// neutral
				break;
		}

		m_controlPoints[Id]->bannerslot = static_cast<uint8>(Id);
		m_controlPoints[Id]->PushToWorld(m_mapMgr);
	}
	else
	{
		if(m_controlPoints[Id]->IsInWorld())
			m_controlPoints[Id]->RemoveFromWorld(false);

		// assign it a new guid (client needs this to see the entry change?)
		m_controlPoints[Id]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_controlPoints[Id]->SetEntry(gi->ID);
		m_controlPoints[Id]->SetDisplayId(gi->DisplayID);
		m_controlPoints[Id]->SetType(static_cast<uint8>(gi->Type));

		switch(Type)
		{
			case AB_SPAWN_TYPE_ALLIANCE_ASSAULT:
			case AB_SPAWN_TYPE_ALLIANCE_CONTROLLED:
				m_controlPoints[Id]->SetFaction(2);
				break;

			case AB_SPAWN_TYPE_HORDE_ASSAULT:
			case AB_SPAWN_TYPE_HORDE_CONTROLLED:
				m_controlPoints[Id]->SetFaction(1);
				break;

			default:
				m_controlPoints[Id]->SetFaction(35);		// neutral
				break;
		}

		m_controlPoints[Id]->SetInfo(gi);
		m_controlPoints[Id]->PushToWorld(m_mapMgr);
	}

	if(gi_aura == NULL)
	{
		// remove it if it exists
		if(m_controlPointAuras[Id] != NULL && m_controlPointAuras[Id]->IsInWorld())
			m_controlPointAuras[Id]->RemoveFromWorld(false);

		return;
	}

	if(m_controlPointAuras[Id] == NULL)
	{
		m_controlPointAuras[Id] = SpawnGameObject(gi_aura->ID, m_mapMgr->GetMapId(), ControlPointCoordinates[Id][0], ControlPointCoordinates[Id][1],
		                          ControlPointCoordinates[Id][2], ControlPointCoordinates[Id][3], 0, 35, 1.0f);

		m_controlPointAuras[Id]->SetParentRotation(2, ControlPointRotations[Id][0]);
		m_controlPointAuras[Id]->SetParentRotation(3, ControlPointRotations[Id][1]);
		m_controlPointAuras[Id]->SetState(GAMEOBJECT_STATE_CLOSED);
		m_controlPointAuras[Id]->SetType(GAMEOBJECT_TYPE_TRAP);
		m_controlPointAuras[Id]->SetAnimProgress(100);
		m_controlPointAuras[Id]->bannerauraslot = static_cast<uint8>(Id);
		m_controlPointAuras[Id]->PushToWorld(m_mapMgr);
	}
	else
	{
		if(m_controlPointAuras[Id]->IsInWorld())
			m_controlPointAuras[Id]->RemoveFromWorld(false);

		// re-spawn the aura
		m_controlPointAuras[Id]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_controlPointAuras[Id]->SetEntry(gi_aura->ID);
		m_controlPointAuras[Id]->SetDisplayId(gi_aura->DisplayID);
		m_controlPointAuras[Id]->SetInfo(gi_aura);
		m_controlPointAuras[Id]->PushToWorld(m_mapMgr);
	}
}

void ArathiBasin::OnCreate()
{
	// Alliance Gate
	GameObject* gate = SpawnGameObject(180255, 529, 1284.597290f, 1281.166626f, -15.977916f, 0.76f, 32, 114, 1.5799990f);
	gate->SetAnimProgress(100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// horde gate
	gate = SpawnGameObject(180256, 529, 708.0902710f, 708.4479370f, -17.3898964f, 3.92f, 32, 114, 1.5699990f);
	gate->SetAnimProgress(100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// spawn (default) control points
	SpawnControlPoint(AB_CONTROL_POINT_STABLE,		AB_SPAWN_TYPE_NEUTRAL);
	SpawnControlPoint(AB_CONTROL_POINT_BLACKSMITH,	AB_SPAWN_TYPE_NEUTRAL);
	SpawnControlPoint(AB_CONTROL_POINT_LUMBERMILL,	AB_SPAWN_TYPE_NEUTRAL);
	SpawnControlPoint(AB_CONTROL_POINT_MINE,		AB_SPAWN_TYPE_NEUTRAL);
	SpawnControlPoint(AB_CONTROL_POINT_FARM,		AB_SPAWN_TYPE_NEUTRAL);

	// spawn buffs
	SpawnBuff(AB_BUFF_STABLES);
	SpawnBuff(AB_BUFF_BLACKSMITH);
	SpawnBuff(AB_BUFF_LUMBERMILL);
	SpawnBuff(AB_BUFF_MINE);
	SpawnBuff(AB_BUFF_FARM);

	// spawn the h/a base spirit guides
	AddSpiritGuide(SpawnSpiritGuide(NoBaseGYLocations[0][0], NoBaseGYLocations[0][1], NoBaseGYLocations[0][2], 0.0f, 0));
	AddSpiritGuide(SpawnSpiritGuide(NoBaseGYLocations[1][0], NoBaseGYLocations[1][1], NoBaseGYLocations[1][2], 0.0f, 1));

	// Let's set this from the config
	SetWorldState( WORLDSTATE_AB_MAX_SCORE, RESOURCES_WINVAL );
}

void ArathiBasin::OnStart()
{
	for(uint32 i = 0; i < 2; ++i)
	{
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
		{
			(*itr)->RemoveAura(BG_PREPARATION);
		}
	}

	// open gates
	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetFlags(64);
		(*itr)->SetState(GAMEOBJECT_STATE_OPEN);
	}

	/* correct? - burlex */
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	m_started = true;
}

ArathiBasin::ArathiBasin(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for(i = 0; i < 2; i++)
	{
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}

	m_pvpData.clear();
	m_resurrectMap.clear();

	for(i = 0; i < AB_NUM_CONTROL_POINTS; ++i)
	{
		m_buffs[i] = NULL;
		m_controlPointAuras[i] = NULL;
		m_controlPoints[i] = NULL;
		m_spiritGuides[i] = NULL;
		m_basesAssaultedBy[i] = -1;
		m_basesOwnedBy[i] = -1;
		m_basesLastOwnedBy[i] = -1;
	}

	for(i = 0; i < 2; ++i)
	{
		m_resources[i] = 0;
		m_capturedBases[i] = 0;
		m_lastHonorGainResources[i] = 0;
		m_lastRepGainResources[i] = 0;
		m_nearingVictory[i] = false;
	}

	m_lgroup = lgroup;

	for(i = 0; i < AB_NUM_CONTROL_POINTS; ++i)
	{
		DefFlag[i][0] = false;
		DefFlag[i][1] = true;
	}

	m_zoneid = 3358;
}

ArathiBasin::~ArathiBasin()
{
	// gates are always spawned, so mapmgr will clean them up
	for(uint32 i = 0; i < AB_NUM_CONTROL_POINTS; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] != NULL)
		{
			if(!m_buffs[i]->IsInWorld())
				delete m_buffs[i];
		}

		if(m_controlPoints[i] != NULL)
		{
			if(!m_controlPoints[i]->IsInWorld())
			{
				delete m_controlPoints[i];
				m_controlPoints[i] = NULL;
			}
		}

		if(m_controlPointAuras[i] != NULL)
		{
			if(!m_controlPointAuras[i]->IsInWorld())
			{
				delete m_controlPointAuras[i];
				m_controlPointAuras[i] = NULL;
			}
		}

		if(m_spiritGuides[i])
		{
			if(!m_spiritGuides[i]->IsInWorld())
				delete m_spiritGuides[i];
		}
	}

	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		if((*itr) != NULL)
		{
			if(!(*itr)->IsInWorld())
				delete(*itr);
		}
	}

	m_resurrectMap.clear();

}

void ArathiBasin::EventUpdateResources(uint32 Team)
{
	uint32 resource_fields[2] = { WORLDSTATE_AB_ALLIANCE_RESOURCES, WORLDSTATE_AB_HORDE_RESOURCES };

	uint32 current_resources = m_resources[Team];
	uint32 current_bases = m_capturedBases[Team];

	if(current_bases > 5)
		current_bases = 5;

	// figure out how much resources we have to add to that team based on the number of captured bases.
	current_resources += (PointBonusPerUpdate[current_bases]);

	// did it change?
	if(current_resources == m_resources[Team])
		return;

	// check for overflow
	if(current_resources > RESOURCES_WINVAL)
		current_resources = RESOURCES_WINVAL;

	m_resources[Team] = current_resources;
	if((current_resources - m_lastRepGainResources[Team]) >= resourcesToGainBR)
	{
		m_mainLock.Acquire();
		for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		{
			uint32 fact = (*itr)->IsTeamHorde() ? 510 : 509; //The Defilers : The League of Arathor
			(*itr)->ModStanding(fact, 10);
		}
		m_mainLock.Release();
		m_lastRepGainResources[Team] += resourcesToGainBR;
	}

	if((current_resources - m_lastHonorGainResources[Team]) >= resourcesToGainBH)
	{
		uint32 honorToAdd = m_honorPerKill;
		m_mainLock.Acquire();
		for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		{
			(*itr)->m_bgScore.BonusHonor += honorToAdd;
			HonorHandler::AddHonorPointsToPlayer((*itr), honorToAdd);
		}

		UpdatePvPData();
		m_mainLock.Release();
		m_lastHonorGainResources[Team] += resourcesToGainBH;
	}

	// update the world states
	SetWorldState(resource_fields[Team], current_resources);

	if(current_resources >= RESOURCES_WARNING_THRESHOLD && !m_nearingVictory[Team])
	{
		m_nearingVictory[Team] = true;
		SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, (uint64)0, "The %s has gathered %u resources and is nearing victory!", Team ? "Horde" : "Alliance", current_resources);
		uint32 sound = SOUND_ALLIANCE_BGALMOSTEND - Team;
		PlaySoundToAll(sound);
	}

	// check for winning condition
	if(current_resources == RESOURCES_WINVAL)
	{
		m_ended = true;
		m_winningteam = static_cast<uint8>(Team);
		m_nextPvPUpdateTime = 0;

		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(TO< CBattleground* >(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		m_winningteam = Team;		

		AddHonorToTeam( m_winningteam, 3 * 185 );

		// Winning spells for AB
		CastSpellOnTeam( m_winningteam, 43484 );
		CastSpellOnTeam( m_winningteam, 69153 );
		CastSpellOnTeam( m_winningteam, 69499 );
		CastSpellOnTeam( m_winningteam, 69500 );

		if( m_winningteam == TEAM_ALLIANCE )
			AddHonorToTeam( TEAM_HORDE, 1 * 185 );
		else
			AddHonorToTeam( TEAM_ALLIANCE, 1 * 185 );

		UpdatePvPData();
	}
}

void ArathiBasin::HookOnPlayerDeath(Player* plr)
{
	// nothing in this BG
	plr->m_bgScore.Deaths++;
	UpdatePvPData();
}

void ArathiBasin::HookOnMount(Player* plr)
{
	// nothing in this BG
}

void ArathiBasin::HookOnPlayerKill(Player* plr, Player* pVictim)
{
	plr->m_bgScore.KillingBlows++;
	UpdatePvPData();
}

void ArathiBasin::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void ArathiBasin::OnAddPlayer(Player* plr)
{
	if(!m_started && plr->IsInWorld())
	{
		plr->CastSpell(plr, BG_PREPARATION, true);
		plr->m_bgScore.MiscData[BG_SCORE_AB_BASES_ASSAULTED] = 0;
		plr->m_bgScore.MiscData[BG_SCORE_AB_BASES_CAPTURED] = 0;
	}
	UpdatePvPData();
}

void ArathiBasin::OnRemovePlayer(Player* plr)
{
	plr->RemoveAura(BG_PREPARATION);
}

void ArathiBasin::HookFlagDrop(Player* plr, GameObject* obj)
{
	// nothing?
}

void ArathiBasin::HookFlagStand(Player* plr, GameObject* obj)
{
	// nothing?
}

LocationVector ArathiBasin::GetStartingCoords(uint32 Team)
{
	if(Team)
		return LocationVector(684.75629f, 681.945007f, -12.915456f, 0.881211f);
	else
		return LocationVector(1314.932495f, 1311.246948f, -9.00952f, 3.802896f);
}

void ArathiBasin::HookOnAreaTrigger(Player* plr, uint32 id)
{
	uint32 spellid = 0;
	int32 buffslot = -1;
	switch(id)
	{
		case 3866:			// stables
			buffslot = AB_BUFF_STABLES;
			break;

		case 3867:			// farm
			buffslot = AB_BUFF_FARM;
			break;

		case 3870:			// blacksmith
			buffslot = AB_BUFF_BLACKSMITH;
			break;

		case 3869:			// mine
			buffslot = AB_BUFF_MINE;
			break;

		case 3868:			// lumbermill
			buffslot = AB_BUFF_LUMBERMILL;
			break;

		case 3948:			// alliance/horde exits
		case 3949:
			{
				RemovePlayer(plr, false);
				return;
			}
			break;
		case 4020:			// Trollbane Hall
		case 4021:			// Defiler's Den
			return;
			break;
		default:
			sLog.Error("ArathiBasin", "Encountered unhandled areatrigger id %u", id);
			return;
			break;
	}

	if(plr->IsDead())		// don't apply to dead players... :P
		return;

	uint32 x = (uint32)buffslot;
	if(m_buffs[x] && m_buffs[x]->IsInWorld())
	{
		// apply the spell
		spellid = m_buffs[x]->GetInfo()->raw.sound3;
		m_buffs[x]->RemoveFromWorld(false);

		// respawn it in buffrespawntime
		sEventMgr.AddEvent(this, &ArathiBasin::SpawnBuff, x, EVENT_AB_RESPAWN_BUFF, AB_BUFF_RESPAWN_TIME, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		// cast the spell on the player
		SpellEntry* sp = dbcSpell.LookupEntryForced(spellid);
		if(sp)
		{
			Spell* pSpell = sSpellFactoryMgr.NewSpell(plr, sp, true, NULL);
			SpellCastTargets targets(plr->GetGUID());
			pSpell->prepare(&targets);
		}
	}
}

bool ArathiBasin::HookHandleRepop(Player* plr)
{
	/* our uber leet ab graveyard handler */
	LocationVector dest(NoBaseGYLocations[plr->m_bgTeam][0], NoBaseGYLocations[plr->m_bgTeam][1], NoBaseGYLocations[plr->m_bgTeam][2], 0.0f);
	float current_distance = 999999.0f;
	float dist;

	for(uint32 i = 0; i < AB_NUM_CONTROL_POINTS; ++i)
	{
		if(m_basesOwnedBy[2] == (int32)plr->m_bgTeam)
		{
			dest.ChangeCoords(GraveyardLocations[2][0], GraveyardLocations[2][1], GraveyardLocations[2][2]);
		}
		else if(m_basesOwnedBy[i] == (int32)plr->m_bgTeam)
		{
			dist = plr->GetPositionV()->Distance2DSq(GraveyardLocations[i][0], GraveyardLocations[i][1]);
			if(dist < current_distance)
			{
				current_distance = dist;
				dest.ChangeCoords(GraveyardLocations[i][0], GraveyardLocations[i][1], GraveyardLocations[i][2]);
			}
		}
	}

	// port us there.
	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void ArathiBasin::CaptureControlPoint(uint32 Id, uint32 Team)
{
	if(m_basesOwnedBy[Id] != -1)
	{
		// there is a very slim chance of this happening, 2 teams events could clash..
		// just in case...
		return;
	}

	// anti cheat, not really necessary because this is a server method but anyway
	if(m_basesAssaultedBy[Id] != (int32)Team)
		return;

	m_basesOwnedBy[Id] = Team;
	m_basesAssaultedBy[Id] = -1;
	m_basesLastOwnedBy[Id] = -1;

	// remove the other spirit guide (if it exists) // burlex: shouldn't' happen
	if(m_spiritGuides[Id] != NULL)
	{
		RemoveSpiritGuide(m_spiritGuides[Id]);
		m_spiritGuides[Id]->Despawn(0, 0);
	}

	// spawn the spirit guide for our faction
	m_spiritGuides[Id] = SpawnSpiritGuide(GraveyardLocations[Id][0], GraveyardLocations[Id][1], GraveyardLocations[Id][2], 0.0f, Team);
	AddSpiritGuide(m_spiritGuides[Id]);

	// send the chat message/sounds out
	PlaySoundToAll(Team ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE);
	SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, 0, "The %s has taken the %s!", Team ? "Horde" : "Alliance", ControlPointNames[Id]);
	DefFlag[Id][0] = false;
	DefFlag[Id][1] = false;

	// update the overhead display on the clients (world states)
	m_capturedBases[Team]++;
	SetWorldState(Team ? WORLDSTATE_AB_HORDE_CAPTUREBASE : WORLDSTATE_AB_ALLIANCE_CAPTUREBASE, m_capturedBases[Team]);

	if(m_capturedBases[Team] >= 4)
	{
		m_mainLock.Acquire();
		for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		{
			if(Team)
			{
				if(m_capturedBases[Team] >= 4 && (*itr)->GetQuestLogForEntry(8121))
					(*itr)->GetQuestLogForEntry(8121)->SendQuestComplete();
				if(m_capturedBases[Team] == 5 && (*itr)->GetQuestLogForEntry(8122))
					(*itr)->GetQuestLogForEntry(8122)->SendQuestComplete();
			}
			else
			{
				if(m_capturedBases[Team] >= 4 && (*itr)->GetQuestLogForEntry(8114))
					(*itr)->GetQuestLogForEntry(8114)->SendQuestComplete();
				if(m_capturedBases[Team] == 5 && (*itr)->GetQuestLogForEntry(8115))
					(*itr)->GetQuestLogForEntry(8115)->SendQuestComplete();
			}
		}
		m_mainLock.Release();
	}

	// respawn the control point with the correct aura
	SpawnControlPoint(Id, Team ? AB_SPAWN_TYPE_HORDE_CONTROLLED : AB_SPAWN_TYPE_ALLIANCE_CONTROLLED);

	// update the map
	SetWorldState(AssaultFields[Id][Team], 0);
	SetWorldState(OwnedFields[Id][Team], 1);

	// resource update event. :)
	if(m_capturedBases[Team] == 1)
	{
		// first
		sEventMgr.AddEvent(this, &ArathiBasin::EventUpdateResources, (uint32)Team, EVENT_AB_RESOURCES_UPDATE_TEAM_0 + Team, ResourceUpdateIntervals[1], 0,
		                   EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		// not first
		event_ModifyTime(EVENT_AB_RESOURCES_UPDATE_TEAM_0 + Team, ResourceUpdateIntervals[m_capturedBases[Team]]);
	}
}

void ArathiBasin::AssaultControlPoint(Player* pPlayer, uint32 Id)
{
#ifdef ANTI_CHEAT
	if(!m_started)
	{
		Anticheat_Log->writefromsession(pPlayer->GetSession(), "%s tried to assault control point in arathi basin before battleground (ID %u) started.", pPlayer->GetName(), this->m_id);
		SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, pPlayer->GetGUID(), "%s will be removed from the game for cheating.", pPlayer->GetName());
		// Remove player from battleground.
		RemovePlayer(pPlayer, false);
		// Kick player from server.
		pPlayer->Kick(6000);
		return;
	}
#endif

	uint32 Team = pPlayer->m_bgTeam;
	uint32 Owner;

	pPlayer->m_bgScore.MiscData[BG_SCORE_AB_BASES_ASSAULTED]++;

	if(m_basesOwnedBy[Id] == -1 && m_basesAssaultedBy[Id] == -1)
	{
		// omgwtfbbq our flag is a virgin?
		SetWorldState(NeutralFields[Id], 0);
	}

	if(m_basesOwnedBy[Id] != -1)
	{
		Owner = m_basesOwnedBy[Id];

		// set it to uncontrolled for now
		m_basesOwnedBy[Id] = -1;
		m_basesLastOwnedBy[Id] = Owner;

		// this control point just got taken over by someone! oh noes!
		if(m_spiritGuides[Id] != NULL)
		{
			map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.find(m_spiritGuides[Id]);
			if(itr != m_resurrectMap.end())
			{
				for(set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end(); ++it2)
				{
					Player* r_plr = m_mapMgr->GetPlayer(*it2);
					if(r_plr != NULL && r_plr->IsDead())
						HookHandleRepop(r_plr);
				}
			}
			m_resurrectMap.erase(itr);
			m_spiritGuides[Id]->Despawn(0, 0);
			m_spiritGuides[Id] = NULL;
		}

		// detract one from the teams controlled points
		m_capturedBases[Owner] -= 1;
		SetWorldState(Owner ? WORLDSTATE_AB_HORDE_CAPTUREBASE : WORLDSTATE_AB_ALLIANCE_CAPTUREBASE, m_capturedBases[Owner]);

		// reset the world states
		SetWorldState(OwnedFields[Id][Owner], 0);

		// modify the resource update time period
		if(m_capturedBases[Owner] == 0)
			this->event_RemoveEvents(EVENT_AB_RESOURCES_UPDATE_TEAM_0 + Owner);
		else
			this->event_ModifyTime(EVENT_AB_RESOURCES_UPDATE_TEAM_0 + Owner, ResourceUpdateIntervals[m_capturedBases[Owner]]);
	}

	// nigga stole my flag!
	if(m_basesAssaultedBy[Id] != -1)
	{
		Owner = m_basesAssaultedBy[Id];

		// woah! vehicle hijack!
		m_basesAssaultedBy[Id] = -1;
		SetWorldState(AssaultFields[Id][Owner], 0);

		// make sure the event does not trigger
		sEventMgr.RemoveEvents(this, EVENT_AB_CAPTURE_CP_1 + Id);
		if(m_basesLastOwnedBy[Id] == (int32)Team)
		{
			m_basesAssaultedBy[Id] = (int32)Team;
			CaptureControlPoint(Id, Team);
			return;
		}

		// no need to remove the spawn, SpawnControlPoint will do this.
	}

	m_basesAssaultedBy[Id] = Team;

	// spawn the new control point gameobject
	SpawnControlPoint(Id, Team ? AB_SPAWN_TYPE_HORDE_ASSAULT : AB_SPAWN_TYPE_ALLIANCE_ASSAULT);

	// update the client's map with the new assaulting field
	SetWorldState(AssaultFields[Id][Team], 1);

	// Check Assault/Defense, the time of capture is not the same.
	if(DefFlag[Id][0] && !DefFlag[Id][1])
	{
		DefFlag[Id][0] = false;
		SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, pPlayer->GetGUID(), "$N defend %s", ControlPointNames[Id]);
		sEventMgr.AddEvent(this, &ArathiBasin::CaptureControlPoint, Id, Team, EVENT_AB_CAPTURE_CP_1 + Id, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		pPlayer->m_bgScore.MiscData[BG_SCORE_AB_BASES_CAPTURED]++;
		UpdatePvPData();
	}
	else if(!DefFlag[Id][0] && !DefFlag[Id][1])
	{
		DefFlag[Id][0] = true;
		SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, pPlayer->GetGUID(), "$N assault %s !", ControlPointNames[Id]);
		PlaySoundToAll(Team ? 8212 : 8174);
		if(Team)
		{
			QuestLogEntry* en = pPlayer->GetQuestLogForEntry(8120);
			switch(Id)
			{
				case AB_CONTROL_POINT_MINE:
					{
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							en->SetMobCount(0, en->GetMobCount(0) + 1);
							en->SendUpdateAddKill(0);
							en->UpdatePlayerFields();
						}
					}
					break;
				case AB_CONTROL_POINT_LUMBERMILL:
					{
						if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
						{
							en->SetMobCount(1, en->GetMobCount(1) + 1);
							en->SendUpdateAddKill(1);
							en->UpdatePlayerFields();
						}
					}
					break;
				case AB_CONTROL_POINT_BLACKSMITH:
					{
						if(en && en->GetMobCount(2) < en->GetQuest()->required_mobcount[2])
						{
							en->SetMobCount(2, en->GetMobCount(2) + 1);
							en->SendUpdateAddKill(2);
							en->UpdatePlayerFields();
						}
					}
					break;
				case AB_CONTROL_POINT_STABLE:
					{
						if(en && en->GetMobCount(3) < en->GetQuest()->required_mobcount[3])
						{
							en->SetMobCount(3, en->GetMobCount(3) + 1);
							en->SendUpdateAddKill(3);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		}
		else
		{
			QuestLogEntry* en = pPlayer->GetQuestLogForEntry(8105);
			switch(Id)
			{
				case AB_CONTROL_POINT_MINE:
					{
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							en->SetMobCount(0, en->GetMobCount(0) + 1);
							en->SendUpdateAddKill(0);
							en->UpdatePlayerFields();
						}
					}
					break;
				case AB_CONTROL_POINT_LUMBERMILL:
					{
						if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
						{
							en->SetMobCount(1, en->GetMobCount(1) + 1);
							en->SendUpdateAddKill(1);
							en->UpdatePlayerFields();
						}
					}
					break;
				case AB_CONTROL_POINT_BLACKSMITH:
					{
						if(en && en->GetMobCount(2) < en->GetQuest()->required_mobcount[2])
						{
							en->SetMobCount(2, en->GetMobCount(2) + 1);
							en->SendUpdateAddKill(2);
							en->UpdatePlayerFields();
						}
					}
					break;
				case AB_CONTROL_POINT_FARM:
					{
						if(en && en->GetMobCount(3) < en->GetQuest()->required_mobcount[3])
						{
							en->SetMobCount(3, en->GetMobCount(3) + 1);
							en->SendUpdateAddKill(3);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		}
		sEventMgr.AddEvent(this, &ArathiBasin::CaptureControlPoint, Id, Team, EVENT_AB_CAPTURE_CP_1 + Id, MSTIME_MINUTE, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		pPlayer->m_bgScore.MiscData[BG_SCORE_AB_BASES_ASSAULTED]++;
		UpdatePvPData();
	}
	else
	{
		DefFlag[Id][0] = true;
		SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, pPlayer->GetGUID(), "$N claims the %s! If left unchallenged, the %s will control it in 1 minute!", ControlPointNames[Id],
		                Team ? "Horde" : "Alliance");
		PlaySoundToAll(8192);
		sEventMgr.AddEvent(this, &ArathiBasin::CaptureControlPoint, Id, Team, EVENT_AB_CAPTURE_CP_1 + Id, MSTIME_MINUTE, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

bool ArathiBasin::HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell)
{
	// burlex todo: find a cleaner way to do this that doesn't waste memory.
	if(pGo->bannerslot >= 0 && pGo->bannerslot < AB_NUM_CONTROL_POINTS)
	{
		//Stealthed / invisible players can't cap
		//if(pPlayer->GetStealthLevel() > 0 || pPlayer->HasAurasWithNameHash(SPELL_HASH_PROWL) || pPlayer->HasAurasWithNameHash(SPELL_HASH_SHADOWMELD))
		if(pPlayer->IsStealth() || pPlayer->m_invisible)
			return false;

		AssaultControlPoint(pPlayer, pGo->bannerslot);
		return true;
	}

	return false;
}

void ArathiBasin::HookOnShadowSight()
{
}
void ArathiBasin::HookGenerateLoot(Player* plr, Object* pOCorpse)
{
}

void ArathiBasin::HookOnUnitKill(Player* plr, Unit* pVictim)
{
}

void ArathiBasin::HookOnFlagDrop(Player* plr)
{
}

void ArathiBasin::SetIsWeekend(bool isweekend)
{
	if(isweekend)
	{
		resourcesToGainBH = 160;
		resourcesToGainBR = 150;
	}
	else
	{
		resourcesToGainBH = 260;
		resourcesToGainBR = 160;
	}
}


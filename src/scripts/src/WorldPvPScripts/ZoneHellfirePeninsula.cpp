/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009-2013 ArcEmu Team <http://www.arcemu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"

//Known Bugs:
//Need to handle a save command because after it gets reseted to neutral!

// Some customizable defines.
#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 4

// Tower Counter Value
#define WORLDSTATE_HELLFIRE_ALLIANCE_TOWERS_CONTROLLED			2476
#define WORLDSTATE_HELLFIRE_HORDE_TOWERS_CONTROLLED				2478

// Tower Counter Display
#define WORLDSTATE_HELLFIRE_TOWER_DISPLAY_ALLIANCE				2490
#define WORLDSTATE_HELLFIRE_TOWER_DISPLAY_HORDE					2489

// PvP Capture Bar
#define WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY				2473
#define WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE				2474

// The Stadium Control Point
#define WORLDSTATE_HELLFIRE_STADIUM_NEUTRAL						2472
#define WORLDSTATE_HELLFIRE_STADIUM_ALLIANCE					2471
#define WORLDSTATE_HELLFIRE_STADIUM_HORDE						2470

// The Overlook Control Point
#define WORLDSTATE_HELLFIRE_OVERLOOK_NEUTRAL					2482
#define WORLDSTATE_HELLFIRE_OVERLOOK_ALLIANCE					2480
#define WORLDSTATE_HELLFIRE_OVERLOOK_HORDE						2481

// Broken Hill Control Point
#define WORLDSTATE_HELLFIRE_BROKENHILL_NEUTRAL					2485
#define WORLDSTATE_HELLFIRE_BROKENHILL_ALLIANCE					2483
#define WORLDSTATE_HELLFIRE_BROKENHILL_HORDE					2484

#define ZONE_HELLFIRE_PENINSULA 3483

#define ALLIANCE_QUEST 10106
#define HORDE_QUEST 13409

// Towers
enum Towers
{
    TOWER_STADIUM,
    TOWER_OVERLOOK,
    TOWER_BROKENHILL,
    TOWER_COUNT,
};

// Tower GameObject Ids
#define TOWER_WEST 182173
#define TOWER_NORTH 182174
#define TOWER_SOUTH 182175

// Buff Ids
#define HELLFIRE_SUPERORITY_ALLIANCE 32071
#define HELLFIRE_SUPERORITY_HORDE 32049

// Owners of the towers, used for save/restore
int32 g_towerOwners[TOWER_COUNT] = { -1, -1, -1 };

// global variables
uint32 g_hordeTowers = 0;
uint32 g_allianceTowers = 0;
int32 g_superiorTeam = -1;			// SUPERIORITY

// Fields to update visual view of the client map
static const uint32 g_hordeStateFields[3] = { WORLDSTATE_HELLFIRE_STADIUM_HORDE, WORLDSTATE_HELLFIRE_OVERLOOK_HORDE, WORLDSTATE_HELLFIRE_BROKENHILL_HORDE };
static const uint32 g_allianceStateFields[3] = { WORLDSTATE_HELLFIRE_STADIUM_ALLIANCE, WORLDSTATE_HELLFIRE_OVERLOOK_ALLIANCE, WORLDSTATE_HELLFIRE_BROKENHILL_ALLIANCE };
static const uint32 g_neutralStateFields[3] = { WORLDSTATE_HELLFIRE_STADIUM_NEUTRAL, WORLDSTATE_HELLFIRE_OVERLOOK_NEUTRAL, WORLDSTATE_HELLFIRE_BROKENHILL_NEUTRAL };

// Banner Status
enum BannerStatus
{
    BANNER_STATUS_NEUTRAL = 0,
    BANNER_STATUS_ALLIANCE = 1,
    BANNER_STATUS_HORDE = 2,
};

//////////////////////////////////////////////////////////////////////////
// Zone Hook
//////////////////////////////////////////////////////////////////////////

void ZoneHook(Player* plr, uint32 Zone, uint32 OldZone)
{
	static uint32 spellids[2] = { HELLFIRE_SUPERORITY_ALLIANCE, HELLFIRE_SUPERORITY_HORDE };

	if( Zone == ZONE_HELLFIRE_PENINSULA )
	{
		if( g_superiorTeam == plr->GetTeam() )
			plr->CastSpell(plr, dbcSpell.LookupEntryForced(spellids[plr->GetTeam()]), true);
	}
	else if( OldZone == ZONE_HELLFIRE_PENINSULA )
	{
			if ( plr->GetTeam() == TEAM_ALLIANCE )
			{
				plr->RemoveAura(HELLFIRE_SUPERORITY_ALLIANCE);
			}
			else
			{
				plr->RemoveAura(HELLFIRE_SUPERORITY_HORDE);
			}
	}
}

class HellfirePeninsulaBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	set< Object* >::iterator itr, itrend;
	uint32 m_bannerStatus;

public:
	static GameObjectAIScript* Create(GameObject* go) { return new HellfirePeninsulaBannerAI(go); }
	GameObject*  pBanner;

	HellfirePeninsulaBannerAI(GameObject* go) : GameObjectAIScript(go)
	{
		m_bannerStatus = BANNER_STATUS_NEUTRAL;
		Status = 50;

		switch(go->GetEntry())
		{
		case TOWER_WEST:
			ControlPointName = "The Stadium";
			towerid = TOWER_STADIUM;
			break;

		case TOWER_NORTH:
			ControlPointName = "The Overlook";
			towerid = TOWER_OVERLOOK;
			break;

		case TOWER_SOUTH:
			ControlPointName = "Broken Hill";
			towerid = TOWER_BROKENHILL;
			break;

		default:
			ControlPointName = "Unknown";
			break;
		}
	}

	void AIUpdate()
	{
		uint32 plrcounts[2] = { 0, 0 };

		// details:
		//   loop through inrange players, for new ones, send the enable CP worldstate.
		//   the value of the map is a timestamp of the last update, to avoid cpu time wasted
		//   doing lookups of objects that have already been updated

		itr = _gameobject->GetInRangePlayerSetBegin();		
		itrend = _gameobject->GetInRangePlayerSetEnd();
		uint32 i;
		map<uint32,uint32>::iterator it2, it3;
		uint32 timeptr = (uint32)UNIXTIME;
		bool in_range;
		bool is_valid;
		Player* plr;
		
		for(; itr != itrend; ++itr)
		{
			plr = TO< Player* >(*itr);
			if( !plr->IsPvPFlagged() || !(plr->isAlive()) && !(plr->IsStealth()) && !(plr->m_invisible) && !(plr->SchoolImmunityList[0]))
				is_valid = false;
			else
				is_valid = true;
			
			in_range = (_gameobject->GetDistanceSq((*itr)) <= BANNER_RANGE);

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				// new player :)
				if( in_range )
				{
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 1);
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE, Status);
					StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));

					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
			else
			{
				// oldie
				if( !in_range )
				{
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE, Status);
					it2->second = timeptr;
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
		}

		// handle stuff for the last tick
		if( Status == 100 && m_bannerStatus != BANNER_STATUS_ALLIANCE )
		{
			m_bannerStatus = BANNER_STATUS_ALLIANCE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Alliance
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00%s has been taken by the Alliance!|r", ControlPointName);

			// tower update
			g_allianceTowers++;
			UpdateTowerCount();

			// state update
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields[towerid], 1);

			// woot
			g_towerOwners[towerid] = 1;
		}
		else if( Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			UpdateTowerCount();
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00%s has been taken by the Horde!|r", ControlPointName);
			
			// tower update
			g_hordeTowers++;
			UpdateTowerCount();

			// state update
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields[towerid], 1);
			
			// woot
			g_towerOwners[towerid] = 0;
		}
		else if( m_bannerStatus != BANNER_STATUS_NEUTRAL )
		{
			// if the difference for the faction is >50, change to neutral
			if( m_bannerStatus == BANNER_STATUS_ALLIANCE && Status <= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();
				
				g_allianceTowers--;
				UpdateTowerCount();

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00The Alliance have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 1);

				// woot
				g_towerOwners[towerid] = -1;
			}
			else if( m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();

				g_hordeTowers--;
				UpdateTowerCount();

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00The Horde have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 1);

				// woot
				g_towerOwners[towerid] = -1;
			}
		}

		// send any out of range players the disable flag
		for(it2 = StoredPlayers.begin(); it2 != StoredPlayers.end(); )
		{
			it3 = it2;
			++it2;

			if( it3->second != timeptr )
			{
				plr = _gameobject->GetMapMgr()->GetPlayer(it3->first);
				
				// they WILL be out of range at this point. this is guaranteed. means they left the set rly quickly.
				if( plr != NULL )
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 0);

				StoredPlayers.erase(it3);
			}
		}

		// work out current status for next tick
		uint32 delta;
		if( plrcounts[0] > plrcounts[1] )
		{
			delta = plrcounts[0] - plrcounts[1];
			delta *= CAPTURE_RATE;

			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			Status += delta;
			if( Status >= 100 )
				Status = 100;
		}
		else if( plrcounts[1] > plrcounts[0] )
		{
			delta = plrcounts[1] - plrcounts[0];
			delta *= CAPTURE_RATE;
			
			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			if( delta > Status )
				Status = 0;
			else
				Status -= delta;
		}
	}

	void SetArtKit()
	{
		// big towers
		static const uint32 artkits_towers[3][3] = {
			{ 69, 67, 68 }, { 63, 62, 61 }, { 66, 65, 64 },
		};

		// flag poles
		static const uint32 artkits_flagpole[3] = { 3, 2, 1 };
	}

	void OnSpawn()
	{
		m_bannerStatus = BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( g_towerOwners[towerid] == 1 )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			Status = 0;

			// state update
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);

			// countz
			g_hordeTowers++;
			UpdateTowerCount();
			SetArtKit();
		}
		else if( g_towerOwners[towerid] == 0 )
		{
			m_bannerStatus = BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);

			// countz
			g_allianceTowers++;
			UpdateTowerCount();
			SetArtKit();
		}

		// start the event timer
		RegisterAIUpdateEvent(UPDATE_PERIOD);
	}

void UpdateTowerCount()
{
	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());

	MapMgr* mgr = _gameobject->GetMapMgr();

	mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), WORLDSTATE_HELLFIRE_ALLIANCE_TOWERS_CONTROLLED, g_allianceTowers);
	mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_HELLFIRE_PENINSULA, _gameobject->GetMapMgr()->GetAreaID(_gameobject->GetPositionX(), _gameobject->GetPositionY() ), WORLDSTATE_HELLFIRE_HORDE_TOWERS_CONTROLLED, g_hordeTowers);

	//Reward Handle the Quests for the Captures
	uint32 quest = plr->GetTeam() == TEAM_ALLIANCE ? ALLIANCE_QUEST : HORDE_QUEST;
   if(plr->HasQuest(quest))
   {
      	QuestLogEntry* qle = plr->GetQuestLogForEntry(quest);

		if(qle == NULL)
			return;

	Creature* place1 = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-290.016f, 3702.42f, 56.6729f, 19029); //STADIUM 19029 npc id
	Creature* place2 = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-471.462f, 3451.09f, 34.6432f, 19032);//BROKENHILL 19032 npc id
	Creature* place3 = plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(-184.889f, 3476.93f, 38.205f, 19028);//OVERLOOK 19028 npc d

	if(place1 != NULL)
	{
			if(qle->GetMobCount(1) < qle->GetQuest()->required_mobcount[1])
			{
			uint32 NewCount = qle->GetMobCount(1) + 1;
			qle->SetMobCount(1, NewCount);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
			}
	}
	if(place2 != NULL)
	{
			if(qle->GetMobCount(2) < qle->GetQuest()->required_mobcount[2])
			{
			uint32 NewCount = qle->GetMobCount(2) + 1;
			qle->SetMobCount(2, NewCount);
			qle->SendUpdateAddKill(2);
			qle->UpdatePlayerFields();
			}
	}
	if(place3 != NULL)
	{
			if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
			{
			uint32 NewCount = qle->GetMobCount(0) + 1;
			qle->SetMobCount(0, NewCount);
			qle->SendUpdateAddKill(0);
			qle->UpdatePlayerFields();
			}
	}
}

	if( g_superiorTeam == 0 && g_allianceTowers != 3 )
	{
		plr->RemoveAura(HELLFIRE_SUPERORITY_ALLIANCE);
		g_superiorTeam = -1;
	}

	if( g_superiorTeam == 1 && g_hordeTowers != 3 )
	{
		plr->RemoveAura(HELLFIRE_SUPERORITY_HORDE);
		g_superiorTeam = -1;
	}

	if( g_allianceTowers == 3 && g_superiorTeam != 0 )
	{
		g_superiorTeam = 0;
		plr->CastSpell(plr ,HELLFIRE_SUPERORITY_ALLIANCE , false);
	}

	if( g_hordeTowers == 3 && g_superiorTeam != 1 )
	{
		g_superiorTeam = 1;
		plr->CastSpell(plr, HELLFIRE_SUPERORITY_HORDE, false);
	}
}
};

uint32 BANNERIDS[] =
	{
		182173,
		182174,
		182175,
		0
	};

void SetupPvPHellfirePeninsula(ScriptMgr* mgr)
{
	mgr->register_gameobject_script(BANNERIDS, &HellfirePeninsulaBannerAI::Create);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void*)ZoneHook);
}




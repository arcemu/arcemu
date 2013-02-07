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
 *
 */
#include "StdAfx.h"

//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (183104, 183411, 182301, 183412, 183413, 183414);

#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 20

// Zone ID
#define ZONE_TEROKKAR_FOREST									3519

//Worldstates
#define WORLDSTATE_TEROKKAR_TOWER1_NEUTRAL						2681
#define WORLDSTATE_TEROKKAR_TOWER1_ALLIANCE						2683
#define WORLDSTATE_TEROKKAR_TOWER1_HORDE						2682

#define WORLDSTATE_TEROKKAR_TOWER2_NEUTRAL						2686
#define WORLDSTATE_TEROKKAR_TOWER2_ALLIANCE						2684
#define WORLDSTATE_TEROKKAR_TOWER2_HORDE						2685

#define WORLDSTATE_TEROKKAR_TOWER3_NEUTRAL						2690
#define WORLDSTATE_TEROKKAR_TOWER3_ALLIANCE						2688
#define WORLDSTATE_TEROKKAR_TOWER3_HORDE						2689

#define WORLDSTATE_TEROKKAR_TOWER4_NEUTRAL						2696
#define WORLDSTATE_TEROKKAR_TOWER4_ALLIANCE						2694
#define WORLDSTATE_TEROKKAR_TOWER4_HORDE						2695

#define WORLDSTATE_TEROKKAR_TOWER5_NEUTRAL						2693
#define WORLDSTATE_TEROKKAR_TOWER5_ALLIANCE						2691
#define WORLDSTATE_TEROKKAR_TOWER5_HORDE						2692

#define WORLDSTATE_TEROKKAR_ALLIANCE_TOWERS_CONTROLLED			2621
#define WORLDSTATE_TEROKKAR_HORDE_TOWERS_CONTROLLED				2622

#define WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY				2623
#define WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE				2625


// Towers
enum Towers
{
    TOWER_1, // North-west
    TOWER_2, // North
    TOWER_3, // North-east
    TOWER_4, // South-east
    TOWER_5, // South
    TOWER_COUNT,
};

// Tower GameObject Ids
#define G_TOWER_1 183104
#define G_TOWER_2 183411
#define G_TOWER_3 183412
#define G_TOWER_4 183413
#define G_TOWER_5 183414

#define BLESSING_OF_AUCHINDOUND 33377

// Owners of the towers, used for save/restore
int32 TFg_towerOwners[TOWER_COUNT] = { -1, -1, -1, -1, -1 };

// global variables
uint32 TFg_hordeTowers = 0;
uint32 TFg_allianceTowers = 0;
int32 TFg_superiorTeam = -1;			// SUPERIORITY

// Fields to update visual view of the client map
static const uint32 g_hordeStateFields[5]		= {	WORLDSTATE_TEROKKAR_TOWER1_HORDE,	WORLDSTATE_TEROKKAR_TOWER2_HORDE,	WORLDSTATE_TEROKKAR_TOWER3_HORDE,	WORLDSTATE_TEROKKAR_TOWER4_HORDE,	WORLDSTATE_TEROKKAR_TOWER5_HORDE };
static const uint32 g_allianceStateFields[5]	= {	WORLDSTATE_TEROKKAR_TOWER1_ALLIANCE, WORLDSTATE_TEROKKAR_TOWER2_ALLIANCE, WORLDSTATE_TEROKKAR_TOWER3_ALLIANCE, WORLDSTATE_TEROKKAR_TOWER4_ALLIANCE, WORLDSTATE_TEROKKAR_TOWER5_ALLIANCE };
static const uint32 g_neutralStateFields[5]		= {	WORLDSTATE_TEROKKAR_TOWER1_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER2_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER3_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER4_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER5_NEUTRAL };

// updates clients visual counter, and adds the buffs to players if needed

enum BannerStatus
{
    BANNER_STATUS_NEUTRAL = 0,
    BANNER_STATUS_ALLIANCE = 1,
    BANNER_STATUS_HORDE = 2,
};

//////////////////////////////////////////////////////////////////////////
// Banner AI
//////////////////////////////////////////////////////////////////////////

class TerokkarForestBannerAI : public GameObjectAIScript
{
		map<uint32, uint32> StoredPlayers;
		uint32 Status;
		const char* ControlPointName;
		uint32 towerid;
		uint32 m_bannerStatus;
		set< Object* >::iterator itr, itrend;

	public:
	static GameObjectAIScript* Create(GameObject* go) { return new TerokkarForestBannerAI(go); }
	GameObject*  pBanner;

		TerokkarForestBannerAI(GameObject* go) : GameObjectAIScript(go)
		{
			m_bannerStatus = BANNER_STATUS_NEUTRAL;
			Status = 50;

			switch(go->GetEntry())
			{
				case G_TOWER_1:
					ControlPointName = "Tower 1";
					towerid = TOWER_1;
					break;

				case G_TOWER_2:
					ControlPointName = "Tower 2";
					towerid = TOWER_2;
					break;

				case G_TOWER_3:
					ControlPointName = "Tower 3";
					towerid = TOWER_3;
					break;

				case G_TOWER_4:
					ControlPointName = "Tower 4";
					towerid = TOWER_4;
					break;

				case G_TOWER_5:
					ControlPointName = "Tower 5";
					towerid = TOWER_5;
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
			map<uint32, uint32>::iterator it2, it3;
			uint32 timeptr = (uint32)UNIXTIME;
			bool in_range;
			bool is_valid;
			Player* plr;
			MapMgr* mgr = _gameobject->GetMapMgr();

			for(; itr != itrend; ++itr)
			{
				plr = TO< Player* >(*itr);
				if(!plr->IsPvPFlagged() || !(plr->isAlive()) && !(plr->IsStealth()) && !(plr->m_invisible) && !(plr->SchoolImmunityList[0]))
					is_valid = false;
				else
					is_valid = true;

				in_range = (_gameobject->GetDistance2dSq((*itr)) <= BANNER_RANGE) ? true : false;

				it2 = StoredPlayers.find((*itr)->GetLowGUID());
				if(it2 == StoredPlayers.end())
				{
					// new player :)
					if(in_range)
					{
						plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 1);
						plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE, Status);
						StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));

						if(is_valid)
							plrcounts[(*itr)->GetTeam()]++;
					}
				}
				else
				{
					// oldie
					if(!in_range)
					{
						plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 0);
						StoredPlayers.erase(it2);
					}
					else
					{
						plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE, Status);
						it2->second = timeptr;
						if(is_valid)
							plrcounts[(*itr)->GetTeam()]++;
					}
				}
			}

			// handle stuff for the last tick
			if(Status == 100 && m_bannerStatus != BANNER_STATUS_ALLIANCE)
			{
				m_bannerStatus = BANNER_STATUS_ALLIANCE;
				SetArtKit();

				// send message to everyone in the zone, has been captured by the Alliance
				mgr->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00%s has been taken by the Alliance!|r", ControlPointName);

				// tower update
				TFg_allianceTowers++;
				UpdateTowerCount();

				// state update
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields[towerid], 1);

				// woot
				TFg_towerOwners[towerid] = 1;
			}
			else if(Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE)
			{
				m_bannerStatus = BANNER_STATUS_HORDE;
				SetArtKit();

				// send message to everyone in the zone, has been captured by the Horde
				mgr->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00%s has been taken by the Horde!|r", ControlPointName);

				// tower update
				TFg_hordeTowers++;
				UpdateTowerCount();

				// state update
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields[towerid], 1);

				// woot
				TFg_towerOwners[towerid] = 0;
			}
			else if(m_bannerStatus != BANNER_STATUS_NEUTRAL)
			{
				// if the difference for the faction is >50, change to neutral
				if(m_bannerStatus == BANNER_STATUS_ALLIANCE && Status <= 50)
				{
					// send message: The Alliance has lost control of point xxx
					m_bannerStatus = BANNER_STATUS_NEUTRAL;
					SetArtKit();

					TFg_allianceTowers--;
					UpdateTowerCount();

					mgr->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00The Alliance have lost control of %s!|r", ControlPointName);

					// state update
					mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields[towerid], 0);
					mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 1);

					// woot
					TFg_towerOwners[towerid] = -1;
				}
				else if(m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50)
				{
					// send message: The Alliance has lost control of point xxx
					m_bannerStatus = BANNER_STATUS_NEUTRAL;
					SetArtKit();

					TFg_hordeTowers--;
					UpdateTowerCount();

					mgr->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00The Horde have lost control of %s!|r", ControlPointName);

					// state update
					mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields[towerid], 0);
					mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 1);

					// woot
					TFg_towerOwners[towerid] = -1;
				}
			}

			// send any out of range players the disable flag
			for(it2 = StoredPlayers.begin(); it2 != StoredPlayers.end();)
			{
				it3 = it2;
				++it2;

				if(it3->second != timeptr)
				{
					plr = _gameobject->GetMapMgr()->GetPlayer(it3->first);

					// they WILL be out of range at this point. this is guaranteed. means they left the set rly quickly.
					if(plr)
						plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 0);

					StoredPlayers.erase(it3);
				}
			}

			// work out current status for next tick
			uint32 delta;
			if(plrcounts[0] > plrcounts[1])
			{
				delta = plrcounts[0] - plrcounts[1];
				delta *= CAPTURE_RATE;

				// cap it at 25 so the banner always gets removed.
				if(delta > 25)
					delta = 25;

				Status += delta;
				if(Status >= 100)
					Status = 100;
			}
			else if(plrcounts[1] > plrcounts[0])
			{
				delta = plrcounts[1] - plrcounts[0];
				delta *= CAPTURE_RATE;

				// cap it at 25 so the banner always gets removed.
				if(delta > 25)
					delta = 25;

				if(delta > Status)
					Status = 0;
				else
					Status -= delta;
			}
		}

		void SetArtKit()
		{
			static const uint32 artkits_flagpole[3] = { 3, 2, 1 };
		}

		void OnSpawn()
		{
			MapMgr* mgr = _gameobject->GetMapMgr();
			Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());

			m_bannerStatus = BANNER_STATUS_NEUTRAL;

			// preloaded data, do we have any?
			if(TFg_towerOwners[towerid] == 1)
			{
				m_bannerStatus = BANNER_STATUS_HORDE;
				Status = 0;

				// state update
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields[towerid], 1);
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);

				// countz
				TFg_hordeTowers++;
				UpdateTowerCount();
				SetArtKit();
			}
			else if(TFg_towerOwners[towerid] == 0)
			{
				m_bannerStatus = BANNER_STATUS_ALLIANCE;
				Status = 100;

				// state update
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields[towerid], 1);
				mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields[towerid], 0);

				// countz
				TFg_allianceTowers++;
				UpdateTowerCount();
				SetArtKit();
			}

			// start the event timer
			RegisterAIUpdateEvent(UPDATE_PERIOD);
		}

void UpdateTowerCount()
{
	MapMgr* mgr = _gameobject->GetMapMgr();
	if (!mgr)
		return;

	mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ),WORLDSTATE_TEROKKAR_ALLIANCE_TOWERS_CONTROLLED, TFg_allianceTowers);
	mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_TEROKKAR_FOREST, mgr->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ),WORLDSTATE_TEROKKAR_HORDE_TOWERS_CONTROLLED, TFg_hordeTowers);

	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ() - 34);
	if (!plr)
		return;
		
	if(TFg_superiorTeam == 0 && TFg_allianceTowers != 5)
	{
		plr->RemoveAura(BLESSING_OF_AUCHINDOUND);
		TFg_superiorTeam = -1;
	}

	if(TFg_superiorTeam == 1 && TFg_hordeTowers != 5)
	{
		plr->RemoveAura(BLESSING_OF_AUCHINDOUND);
		TFg_superiorTeam = -1;
	}

	if(TFg_allianceTowers == 5 && TFg_superiorTeam != 0)
	{
		TFg_superiorTeam = 0;
		plr->CastSpell(plr, BLESSING_OF_AUCHINDOUND, false);
	}

	if(TFg_hordeTowers == 5 && TFg_superiorTeam != 1)
	{
		TFg_superiorTeam = 1;
		plr->CastSpell(plr, BLESSING_OF_AUCHINDOUND, false);
	}
}
};

//////////////////////////////////////////////////////////////////////////
// Zone Hook
//////////////////////////////////////////////////////////////////////////

void TFZoneHook(Player* plr, uint32 Zone, uint32 OldZone)
{
	if(!plr)
		return;

	if(Zone == ZONE_TEROKKAR_FOREST)
	{
		if(TFg_superiorTeam == plr->GetTeam())
			plr->CastSpell(plr, dbcSpell.LookupEntry(BLESSING_OF_AUCHINDOUND), true);
	}
	else if(OldZone == ZONE_TEROKKAR_FOREST)
	{
		if(TFg_superiorTeam == plr->GetTeam())
			plr->RemoveAura(BLESSING_OF_AUCHINDOUND);
	}
}

uint32 BANNERS[] =
	{
	183104,
	183411,
	183412,
	183413,
	183414,
	0
	};

void SetupPvPTerokkarForest(ScriptMgr* mgr)
{
	//Banner AI
	mgr->register_gameobject_script(BANNERS, &TerokkarForestBannerAI::Create);

	// register instance hooker
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void*)&TFZoneHook);
}


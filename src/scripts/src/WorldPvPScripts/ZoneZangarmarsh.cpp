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

#define ZONE_ZANGARMARSH										3521

// tower stuff
#define WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_MAP					2652
#define WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_MAP				2650
#define WORLDSTATE_ZANGARMARSH_EAST_HORDE_MAP					2651

#define WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_UI					2560
#define WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_UI					2558
#define WORLDSTATE_ZANGARMARSH_EAST_HORDE_UI					2559

#define WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_MAP					2646
#define WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_MAP				2644
#define WORLDSTATE_ZANGARMARSH_WEST_HORDE_MAP					2645

#define WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_UI					2557
#define WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_UI					2555
#define WORLDSTATE_ZANGARMARSH_WEST_HORDE_UI					2556

// capture bars
#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST			2527
#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST			2528

#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST			2533
#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST			2534

// flag

//ally
#define WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE		2655
#define WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE	2656

//horde
#define WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE		2658
#define WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE	2657

#define WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL				2647
#define WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE			2648
#define WORLDSTATE_ZANGARMARSH_CITY_HORDE				2649

//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (182523, 182522);

#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 4

// Towers
enum Towers
{
    TOWER_EAST,
    TOWER_WEST,
    TOWER_COUNT,
};

// Tower GameObject Ids
#define G_TOWER_EAST 182523
#define G_TOWER_WEST 182522

#define TWIN_SPIRE_BLESSING 33779

// Owners of the towers, used for save/restore
int32 ZMg_towerOwners[TOWER_COUNT] = { -1, -1 };

// global variables
uint32 ZMg_hordeTowers = 0;
uint32 ZMg_allianceTowers = 0;
int32 ZMg_superiorTeam = -1;			// SUPERIORITY
int32 ZMCityOwners = -1;

// Visual view for MAP
static const uint32 g_hordeStateFields_MAP[TOWER_COUNT]		= {	WORLDSTATE_ZANGARMARSH_EAST_HORDE_MAP,		WORLDSTATE_ZANGARMARSH_WEST_HORDE_MAP};
static const uint32 g_allianceStateFields_MAP[TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_MAP,	WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_MAP};
static const uint32 g_neutralStateFields_MAP[TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_MAP,	WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_MAP};

// Visual view for UI
static const uint32 g_hordeStateFields_UI[TOWER_COUNT]		= {	WORLDSTATE_ZANGARMARSH_EAST_HORDE_UI,		WORLDSTATE_ZANGARMARSH_WEST_HORDE_UI};
static const uint32 g_allianceStateFields_UI[TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_UI,	WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_UI};
static const uint32 g_neutralStateFields_UI[TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_UI,		WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_UI};

void ZMSpawnBanners(shared_ptr<MapMgr> bmgr, int32 side);

enum BannerStatus
{
    BANNER_STATUS_NEUTRAL = 0,
    BANNER_STATUS_ALLIANCE = 1,
    BANNER_STATUS_HORDE = 2,
};

//////////////////////////////////////////////////////////////////////////
// Banner AI
//////////////////////////////////////////////////////////////////////////

class ZangarmarshBannerAI : public GameObjectAIScript
{
		map<uint32, uint32> StoredPlayers;
		uint32 Status;
		const char* ControlPointName;
		uint32 towerid;
		uint32 m_bannerStatus;
		set< Object* >::iterator itr, itrend;

	public:

	static GameObjectAIScript* Create(GameObject* go) { return new ZangarmarshBannerAI(go); }
	GameObject*  pBanner;

		ZangarmarshBannerAI(GameObject* go) : GameObjectAIScript(go)
		{
			m_bannerStatus = BANNER_STATUS_NEUTRAL;
			Status = 50;

			switch(go->GetEntry())
			{
				case G_TOWER_EAST:
					ControlPointName = "East Beacon";
					towerid = TOWER_EAST;
					break;

				case G_TOWER_WEST:
					ControlPointName = "West Beacon";
					towerid = TOWER_WEST;
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

			for(; itr != itrend; ++itr)
			{
				plr = TO< Player* >(*itr);
				if( !plr->IsPvPFlagged() || !(plr->isAlive()) && !(plr->IsStealth()) && !(plr->m_invisible) && !(plr->SchoolImmunityList[0]))
					is_valid = false;
				else
					is_valid = true;

				in_range = (_gameobject->GetDistance2dSq((*itr)) <= BANNER_RANGE) ? true : false;

				it2 = StoredPlayers.find((*itr)->GetLowGUID());
				if(it2 == StoredPlayers.end())
				{
					if(in_range)
					{
						if(towerid == TOWER_WEST)
						{
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 1);
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST, Status);
						}
						else
						{
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 1);
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST, Status);
						}
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
						if(towerid == TOWER_WEST)
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 0);
						else
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 0);
						StoredPlayers.erase(it2);
					}
					else
					{
						if(towerid == TOWER_WEST)
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST, Status);
						else
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST, Status);
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

				// send message to everyone in the zone, has been captured by the Alliance
				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00%s has been taken by the Alliance!|r", ControlPointName);

				// tower update
				ZMg_allianceTowers++;
				UpdateTowerCountZM();

				// state update
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_MAP[towerid], 0);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields_MAP[towerid], 1);

				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_UI[towerid], 0);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields_UI[towerid], 1);

				// woot
				ZMg_towerOwners[towerid] = 1;
			}
			else if(Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE)
			{
				m_bannerStatus = BANNER_STATUS_HORDE;

				// send message to everyone in the zone, has been captured by the Horde
				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00%s has been taken by the Horde!|r", ControlPointName);

				// tower update
				ZMg_hordeTowers++;
				UpdateTowerCountZM();

				// state update
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_MAP[towerid], 0);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields_MAP[towerid], 1);

				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_UI[towerid], 0);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields_UI[towerid], 1);

				// woot
				ZMg_towerOwners[towerid] = 0;
			}
			else if(m_bannerStatus != BANNER_STATUS_NEUTRAL)
			{
				// if the difference for the faction is >50, change to neutral
				if(m_bannerStatus == BANNER_STATUS_ALLIANCE && Status <= 50)
				{
					// send message: The Alliance has lost control of point xxx
					m_bannerStatus = BANNER_STATUS_NEUTRAL;

					ZMg_allianceTowers--;
					UpdateTowerCountZM();

					_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00The Alliance have lost control of %s!|r", ControlPointName);

					// state update
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields_MAP[towerid], 0);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_MAP[towerid], 1);

					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields_UI[towerid], 0);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_UI[towerid], 1);

					// woot
					ZMg_towerOwners[towerid] = -1;
				}
				else if(m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50)
				{
					// send message: The Alliance has lost control of point xxx
					m_bannerStatus = BANNER_STATUS_NEUTRAL;

					ZMg_hordeTowers--;
					UpdateTowerCountZM();

					_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00The Horde have lost control of %s!|r", ControlPointName);

					// state update
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields_MAP[towerid], 0);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_MAP[towerid], 1);

					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields_UI[towerid], 0);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_UI[towerid], 1);

					// woot
					ZMg_towerOwners[towerid] = -1;
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
					{
						if(towerid == TOWER_WEST)
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 0);
						else
							plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 0);
					}

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
		void OnSpawn()
		{
			Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());

			m_bannerStatus = BANNER_STATUS_NEUTRAL;

			// preloaded data, do we have any?
			if(ZMg_towerOwners[towerid] == 1)
			{
				m_bannerStatus = BANNER_STATUS_HORDE;
				Status = 0;

				// state update
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields_MAP[towerid], 1);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_MAP[towerid], 0);

				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_hordeStateFields_UI[towerid], 1);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_UI[towerid], 0);

				// countz
				ZMg_hordeTowers++;
				UpdateTowerCountZM();
			}
			else if(ZMg_towerOwners[towerid] == 0)
			{
				m_bannerStatus = BANNER_STATUS_ALLIANCE;
				Status = 100;

				// state update
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields_MAP[towerid], 1);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_MAP[towerid], 0);

				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_allianceStateFields_UI[towerid], 1);
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, _gameobject->GetMapMgr()->GetAreaID( _gameobject->GetPositionX(), _gameobject->GetPositionY() ), g_neutralStateFields_UI[towerid], 0);

				// countz
				ZMg_allianceTowers++;
				UpdateTowerCountZM();
			}

			// start the event timer
			RegisterAIUpdateEvent(UPDATE_PERIOD);
		}

void UpdateTowerCountZM()
{
	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
	if (!plr)
		return;
		
	MapMgr* mgr = plr->GetMapMgr();

	if(!mgr)
		return;

	if(ZMg_superiorTeam == 0 && ZMg_allianceTowers != TOWER_COUNT)
	{
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE, 1);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE, 0);
		ZMg_superiorTeam = -1;
	}

	if(ZMg_superiorTeam == 1 && ZMg_hordeTowers != TOWER_COUNT)
	{
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE, 1);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE, 0);
		ZMg_superiorTeam = -1;
	}

	if(ZMg_allianceTowers == TOWER_COUNT && ZMg_superiorTeam != 0)
	{
		ZMg_superiorTeam = 0;
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE, 0);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE, 1);
	}

	if(ZMg_hordeTowers == TOWER_COUNT && ZMg_superiorTeam != 1)
	{
		ZMg_superiorTeam = 1;
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE, 0);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE, 1);
	}
}
};

//////////////////////////////////////////////////////////////////////////
// Scouts AI
//////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL ZMScouts : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player*  plr)
		{
			uint32 Team = plr->GetTeam();

			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), Team == 0 ? 9433 : 9750, plr);
				Menu->AddItem(0, "I would like to browse your goods.", 3);

			if(ZMg_superiorTeam == Team && ZMCityOwners != Team && !plr->HasAura(Team == 0 ? 32430 : 32431))
			{
				Menu->AddItem(0, Team == 0 ? "Give me the flag, I'll take it to the Central Tower for the glory of the Alliance!" : "Give me the flag, I'll take it to the Central Tower for the glory of the Horde!" , Team + 1);
			}	
			Menu->SendTo(plr);
		};

		void GossipSelectOption(Object* pObject, Player*  plr, uint32 Id, uint32 IntId, const char* Code)
		{
			if(IntId == 3)
			{
				plr->GetSession()->SendInventoryList(TO_CREATURE(pObject));
				return;
			}

			uint32 CastSpellID = IntId == 1 ? 32430 : 32431;
			if(CastSpellID != 0)
				if(!plr->HasAura(CastSpellID))
					plr->CastSpell(plr, CastSpellID, true);
			return;
		}
};

class ZMCityBannerAI : public GameObjectAIScript
{
	public:
		ZMCityBannerAI(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new ZMCityBannerAI(GO); }
	
		void OnActivate(Player* pPlayer)
		{
			if(!pPlayer)
				return;

			//Get Team from Player
			uint32 pTeam = pPlayer->GetTeam();

			if(pTeam > 1) 
				pTeam = 1;

			//if Player has both Towers and is not Owner of the Town then Add Aura.
			if(ZMg_superiorTeam != pTeam || ZMCityOwners == pTeam)
				return;

			uint32 flagaura = (pTeam == 0 ? 32430 : 32431);

			if(pPlayer->HasAura(flagaura))
				pPlayer->RemoveAura(flagaura);
			else
				return; // we must have flag to capture city

			ZMCityOwners = pTeam; // we are now city owners! Yay
			//brrr....
			const char* cOwners = ZMCityOwners == 0 ? "0" : "1";
			if(ZMCityOwners == -1)   // just in case
				cOwners = "-1";

			SetGrave();

			uint32 oppositeTeam = (pTeam == 0 ? 1 : 0);

			pPlayer->CastSpell(pTeam, TWIN_SPIRE_BLESSING, false);
			pPlayer->RemoveAura(oppositeTeam, TWIN_SPIRE_BLESSING);
			
			//Spawn Flag of the Owner and Despawn the other Flag

			_gameobject->Despawn(1000, 0);

			if(pTeam == 0)
			{
			MapMgr* mgr = pPlayer->GetMapMgr();
				if(!mgr)
					return;

				GameObject* bGo = NULL;
				bGo = mgr->GetInterface()->SpawnGameObject(182527, 253.54f, 7083.81f, 36.7728f, 0.008727f, false, 0, 0);
					if(!bGo)
						return;

				bGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, 1);
				bGo->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
				bGo->SetUInt32Value(GAMEOBJECT_FACTION, FACTION_HORDE);

				bGo->PushToWorld(mgr);
			}

			if(pTeam == 1)
			{
			MapMgr* mgr = pPlayer->GetMapMgr();
				if(!mgr)
					return;

				GameObject* bGo = NULL;
				bGo = mgr->GetInterface()->SpawnGameObject(182528, 253.54f, 7083.81f, 36.7728f, 0.008727f, false, 0, 0);
					if(!bGo)
						return;

				bGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, 1);
				bGo->SetUInt32Value(GAMEOBJECT_FLAGS, 0);
				bGo->SetUInt32Value(GAMEOBJECT_FACTION, FACTION_ALLIANCE);

				bGo->PushToWorld(mgr);
			}
		}

void SetGrave()
{
	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
	
	MapMgr* mgr = plr->GetMapMgr();

	if(!mgr || mgr->GetMapId() != 530)
		return;

	// any better solution?
	uint32 gOwners = ZMCityOwners == 0 ? 0 : 1;
	if(ZMCityOwners == -1)
		gOwners = 3;

	StorageContainerIterator<GraveyardTeleport> * itr;
	itr = GraveyardStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		GraveyardTeleport* pGrave = itr->Get();
		if(pGrave->ID == 142)
		{
			pGrave->FactionID = gOwners;
			break;
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	if(ZMCityOwners == 0)
	{
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 1);
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else if(ZMCityOwners == 1)
	{
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_HORDE, 1);
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else
	{
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);
		plr->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
	}
}
};

void SetGrave(Player* plr)
{
	MapMgr* mgr = plr->GetMapMgr();

	if(!mgr || mgr->GetMapId() != 530)
		return;

	// any better solution?
	uint32 gOwners = ZMCityOwners == 0 ? 0 : 1;
	if(ZMCityOwners == -1)
		gOwners = 3;

	StorageContainerIterator<GraveyardTeleport> * itr;
	itr = GraveyardStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		GraveyardTeleport* pGrave = itr->Get();
		if(pGrave->ID == 142)
		{
			pGrave->FactionID = gOwners;
			break;
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	if(ZMCityOwners == 0)
	{
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 1);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else if(ZMCityOwners == 1)
	{
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_HORDE, 1);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else
	{
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);
		mgr->GetWorldStatesHandler().SetWorldStateForZone(ZONE_ZANGARMARSH, plr->GetAreaID(), WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
// Zone Hook
//////////////////////////////////////////////////////////////////////////

void ZMZoneHook(Player* plr, uint32 Zone, uint32 OldZone)
{
	if(!plr)
		return;

	if(Zone == ZONE_ZANGARMARSH)
	{
		if(ZMCityOwners == plr->GetTeam())
			plr->CastSpell(plr, dbcSpell.LookupEntry(TWIN_SPIRE_BLESSING), true);
	}
	else if(OldZone == ZONE_ZANGARMARSH)
	{
		if(ZMCityOwners == plr->GetTeam())
			plr->RemoveAura(TWIN_SPIRE_BLESSING);
	}
}

//////////////////////////////////////////////////////////////////////////
// Object Spawn Data
//////////////////////////////////////////////////////////////////////////

struct sgodata
{
	uint32 entry;
	float posx;
	float posy;
	float posz;
	float facing;
	float orientation[4];
	uint32 state;
	uint32 flags;
	uint32 faction;
	float scale;
	uint32 is_banner;
};

void ZMSpawnBanners(Player* plr)
{
	MapMgr* mgr = plr->GetMapMgr();
	if(!mgr)
		return;
	// -1 = neutral
	//  0 = alliance
	//  1 = horde

	//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (182529, 182527, 182528);
	const static sgodata gobdata[] =
	{
		{ 182529, 253.54f, 7083.81f, 36.7728f, -0.017453f, 0, 0, 0.008727f, -0.999962f, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	uint32 i = ZMCityOwners + 1;
	if(i > 2)   // how does that happen? oO
		i = 2;
	const sgodata* b;
	b = &gobdata[i];

		GameObject* GetGo = mgr->GetInterface()->GetGameObjectNearestCoords(b->posx, b->posy, b->posz, b->entry);
		if(!GetGo == NULL)
			return;

	GameObject* bGo = NULL;
	bGo = mgr->GetInterface()->SpawnGameObject(b->entry, b->posx, b->posy, b->posz, b->facing, false, 0, 0);
	if(!bGo)
		return;

	bGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, b->state);
	bGo->SetUInt32Value(GAMEOBJECT_FLAGS, b->flags);
	bGo->SetUInt32Value(GAMEOBJECT_FACTION, b->faction);

	bGo->PushToWorld(mgr);
}

void ZMSpawnObjects(Player* plr)
{
	MapMgr* mgr = plr->GetMapMgr();

	if(!mgr || mgr->GetMapId() != 530)
		return;

	ZMSpawnBanners(plr);
}

//////////////////////////////////////////////////////////////////////////
// Kill Hook
//////////////////////////////////////////////////////////////////////////

void Tokens(Player* pPlayer, Player* pVictim)
{
	if(!pPlayer || !pVictim)
		return;
	if(!pPlayer->HasAura(TWIN_SPIRE_BLESSING) || pPlayer->GetTeam() == pVictim->GetTeam())
		return;

	uint32 TokenSpell = pPlayer->GetTeam() == 0 ? 32155 : 32158;

	pPlayer->CastSpell(pPlayer, TokenSpell, true);
}

uint32 BANNER[] =
	{
		G_TOWER_EAST,
		G_TOWER_WEST,
		0
	};

void SetupPvPZangarmarsh(ScriptMgr* mgr)
{
	//Control Points
	mgr->register_gameobject_script(BANNER, &ZangarmarshBannerAI::Create);

	// City banners AI
	mgr->register_gameobject_script(182529, &ZMCityBannerAI::Create);
	mgr->register_gameobject_script(182527, &ZMCityBannerAI::Create);
	mgr->register_gameobject_script(182528, &ZMCityBannerAI::Create);

	// scouts AI
	GossipScript* zms = (GossipScript*) new ZMScouts();
	mgr->register_gossip_script(18564, zms);
	mgr->register_gossip_script(18581, zms);

	//Hooks
	mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)Tokens);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void*)&ZMZoneHook);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&ZMSpawnObjects);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ENTER_WORLD, (void*)&SetGrave);
}


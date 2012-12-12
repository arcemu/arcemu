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

// Some customizable defines.
#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 20

// Guard Display
#define WORLDSTATE_HALAA_GUARD_DISPLAY_ALLIANCE					2502
#define WORLDSTATE_HALAA_GUARD_DISPLAY_HORDE					2503

// Guard Fields
#define WORLDSTATE_HALAA_GUARDS_REMAINING						2491
#define WORLDSTATE_HALAA_GUARDS_REMAINING_MAX					3493

// Zone ID
#define ZONE_NAGRAND											3518


enum OutdoorPvPNagrandSpells
{
    NA_KILL_TOKEN_ALLIANCE = 33005,
    NA_KILL_TOKEN_HORDE = 33004,
    NA_CAPTURE_BUFF = 33795  // strength of the halaani
};

// global variables
int32 NGg_superiorTeam = -1;			// SUPERIORITY
int32 CityOwners = -1;
int32 Halaa = -1;
uint32 g_guards = 15;
int32 g_capturable = 1;

enum OutdoorPvPNAWorldStates
{
    NA_UI_HORDE_GUARDS_SHOW = 2503,
    NA_UI_ALLIANCE_GUARDS_SHOW = 2502,
    NA_UI_GUARDS_MAX = 2493,
    NA_UI_GUARDS_LEFT = 2491,

    NA_UI_TOWER_SLIDER_DISPLAY = 2495,
    NA_UI_TOWER_SLIDER_POS = 2494,
    NA_UI_TOWER_SLIDER_N = 2497,

    NA_MAP_WYVERN_NORTH_NEU_H = 2762,
    NA_MAP_WYVERN_NORTH_NEU_A = 2662,
    NA_MAP_WYVERN_NORTH_H = 2663,
    NA_MAP_WYVERN_NORTH_A = 2664,

    NA_MAP_WYVERN_SOUTH_NEU_H = 2760,
    NA_MAP_WYVERN_SOUTH_NEU_A = 2670,
    NA_MAP_WYVERN_SOUTH_H = 2668,
    NA_MAP_WYVERN_SOUTH_A = 2669,

    NA_MAP_WYVERN_WEST_NEU_H = 2761,
    NA_MAP_WYVERN_WEST_NEU_A = 2667,
    NA_MAP_WYVERN_WEST_H = 2665,
    NA_MAP_WYVERN_WEST_A = 2666,

    NA_MAP_WYVERN_EAST_NEU_H = 2763,
    NA_MAP_WYVERN_EAST_NEU_A = 2659,
    NA_MAP_WYVERN_EAST_H = 2660,
    NA_MAP_WYVERN_EAST_A = 2661,

    NA_MAP_HALAA_NEUTRAL = 2671,
    NA_MAP_HALAA_NEU_A = 2676,
    NA_MAP_HALAA_NEU_H = 2677,
    NA_MAP_HALAA_HORDE = 2672,
    NA_MAP_HALAA_ALLIANCE = 2673
};

// Banner Status
enum BannerStatus
{
    BANNER_STATUS_NEUTRAL = 0,
    BANNER_STATUS_ALLIANCE = 1,
    BANNER_STATUS_HORDE = 2,
};

//////////////////////////////////////////////////////////////////////////
// Gameobjects
//////////////////////////////////////////////////////////////////////////

class GryphonHorstDestroyedAI : public GameObjectAIScript
{
	public:
		GryphonHorstDestroyedAI(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new GryphonHorstDestroyedAI(GO); }
	
		void OnActivate(Player* plr)
		{
			if(!plr)
				return;

			uint32 Team = plr->GetTeam();

			if (CityOwners == Team)
				return;
			
			uint32 FACTION = Team == TEAM_ALLIANCE ? 1 : 2;

			_gameobject->Despawn(500, 0);

			GameObject* Spawn;
			Spawn = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182267, _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ(), 0, false, 0, 0);
			if(!Spawn)
				return;
			Spawn->SetUInt32Value(GAMEOBJECT_FACTION, FACTION);

			Spawn->PushToWorld(_gameobject->GetMapMgr());

			switch(_gameobject->GetEntry())
			{
			case 182275:
				{
					if(plr->GetTeam() == TEAM_ALLIANCE)
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_A, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_NEU_A, 0);
					}
					else
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_H, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_NEU_H, 0);
					}
				}	
				break;

			case 182276:
				{
					if(plr->GetTeam() == TEAM_ALLIANCE)
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_A, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_NEU_A, 0);
					}
					else
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_H, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_NEU_H, 0);
					}
				}
				break;

			case 182277:
				{
					if(plr->GetTeam() == TEAM_ALLIANCE)
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_A, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_NEU_A, 0);
					}
					else
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_H, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_NEU_H, 0);
					}
				}
				break;

			case 182297:
				{
					if(plr->GetTeam() == TEAM_ALLIANCE)
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_A, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_NEU_A, 0);
					}
					else
					{
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_H, 1);
					_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_NEU_H, 0);
					}
				}
				break;
			}
		}
};

class GryphonHorstAI : public GameObjectAIScript
{
	public:
		GryphonHorstAI(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new GryphonHorstAI(GO); }
		
		void OnSpawn()
		{
			RegisterAIUpdateEvent(500);
		}

		void AIUpdate()
		{
			if(g_guards == 0)
			_gameobject->Despawn(0, 0);
		}

		void OnActivate(Player* plr)
		{
			if(!plr)
				return;

			uint32 Team = plr->GetTeam();

			if (CityOwners == Team)
				return;

			//West
			uint32 WWX = _gameobject->GetPositionX() ==  -1507.94f;
			uint32 WWY = _gameobject->GetPositionY() ==  8132.10f;
			uint32 WWZ = _gameobject->GetPositionZ() ==  -19.55f;
			//North
			uint32 NNX = _gameobject->GetPositionX() ==  -1384.53f;
			uint32 NNY = _gameobject->GetPositionY() ==  7779.39f;
			uint32 NNZ = _gameobject->GetPositionZ() ==  -11.17f;
			//East
			uint32 EEX = _gameobject->GetPositionX() ==  -1650.28f;
			uint32 EEY = _gameobject->GetPositionY() ==  7732.18f;
			uint32 EEZ = _gameobject->GetPositionZ() ==  -15.44f;
			//South
			uint32 SSX = _gameobject->GetPositionX() ==  -1815.80f;
			uint32 SSY = _gameobject->GetPositionY() ==  8036.50f;
			uint32 SSZ = _gameobject->GetPositionZ() ==  -26.23f;

			//West
			if( ( WWX ) & ( WWY ) & ( WWZ ) )
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(523), 295, 0);
			//East
			if( ( EEX ) & ( EEY ) & ( EEZ ) )
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(524), 295, 0);
			//North
			if( ( NNX ) & ( NNY ) & ( NNZ ) )
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(522), 295, 0);
			//South
			if( ( SSX ) & ( SSY ) & ( SSZ ) )
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(520), 295, 0);
		}
};

class HalaaFlagAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	set< Object* >::iterator itr, itrend;
	uint32 m_bannerStatus;

public:
	static GameObjectAIScript* Create(GameObject* go) { return new HalaaFlagAI(go); }
	GameObject*  pBanner;

	HalaaFlagAI(GameObject* go) : GameObjectAIScript(go)
	{
		m_bannerStatus = BANNER_STATUS_NEUTRAL;
		Status = 50;
	}

	void AIUpdate()
	{
		if(g_guards == 0)
		{
		g_capturable = 1;
		}

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
		bool capturable;
		Player* plr;

		for(; itr != itrend; ++itr)
		{
			plr = TO< Player* >(*itr);
			if( !plr->IsPvPFlagged() || !(plr->isAlive()) && !(plr->IsStealth()) && !(plr->m_invisible) && !(plr->SchoolImmunityList[0]))
				is_valid = false;
			else
				is_valid = true;
			
			in_range = (_gameobject->GetDistanceSq((*itr)) <= BANNER_RANGE) ? true : false;
			capturable = (g_capturable == 1? true : false);

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				// new player :)
				if( in_range & capturable)
				{
					plr->SendWorldStateUpdate(NA_UI_TOWER_SLIDER_DISPLAY, 1);
					plr->SendWorldStateUpdate(NA_UI_TOWER_SLIDER_POS, Status);
					StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));

					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
			else
			{
				// oldie
				if( !in_range & !capturable )
				{
					plr->SendWorldStateUpdate(NA_UI_TOWER_SLIDER_DISPLAY, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					plr->SendWorldStateUpdate(NA_UI_TOWER_SLIDER_POS, Status);
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
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_NAGRAND, ZONE_NAGRAND, "|cffffff00%s has been taken by the Alliance!|r", "Halaa");

			// tower update
			UpdateTowerCount();

			//owner
			CityOwners = 0;
			g_capturable = 0;
			g_guards = 15;

			//state
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_NEUTRAL, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_ALLIANCE, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_HORDE, 0);

			//Bats
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_NEU_H, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_NEU_H, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_NEU_H, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_NEU_H, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_H, 0);
			
			//Disable Alliance Bats
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_NEU_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_NEU_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_NEU_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_NEU_A, 0);

			//Guards
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_HORDE_GUARDS_SHOW, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_ALLIANCE_GUARDS_SHOW, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_GUARDS_MAX, 15);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_GUARDS_LEFT, 15);
			
			//Spawn Npcs / Gos
			SpawnA();
		}
		else if( Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			UpdateTowerCount();
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_NAGRAND, ZONE_NAGRAND, "|cffffff00%s has been taken by the Horde!|r", "Halaa");
			
			// tower update
			UpdateTowerCount();

			//owner
			CityOwners = 1;
			g_capturable = 0;
			g_guards = 15;

			//state
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_NEUTRAL, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_HORDE, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_ALLIANCE, 0);
			
			//Bats
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_NEU_A, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_NEU_A, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_NEU_A, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_NEU_A, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_A, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_A, 0);
			
			//Disable Horde Bats
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_WEST_NEU_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_SOUTH_NEU_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_EAST_NEU_H, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_WYVERN_NORTH_NEU_H, 0);

			//Guards
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_ALLIANCE_GUARDS_SHOW, 0);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_HORDE_GUARDS_SHOW, 1);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_GUARDS_MAX, 15);
			_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_UI_GUARDS_LEFT, 15);

			//Spawn Npcs / Gos
			SpawnH();
		}
		else if( m_bannerStatus != BANNER_STATUS_NEUTRAL )
		{
			// if the difference for the faction is >50, change to neutral
			if( m_bannerStatus == BANNER_STATUS_ALLIANCE && Status <= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();
				
				UpdateTowerCount();

				//owner
				CityOwners = -1;

				//status
				_gameobject->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, plr->GetAreaID(), NA_MAP_HALAA_NEUTRAL, 1);

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_NAGRAND, ZONE_NAGRAND, "|cffffff00The Alliance have lost control of %s!|r", "Halaa");
			}
			else if( m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();

				UpdateTowerCount();

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_NAGRAND, ZONE_NAGRAND, "|cffffff00The Horde have lost control of %s!|r", "Halaa");
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
					plr->SendWorldStateUpdate(NA_UI_TOWER_SLIDER_DISPLAY, 0);

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
		Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
		m_bannerStatus = BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( CityOwners == 1 )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			Status = 0;

			// countz
			UpdateTowerCount();
			SetArtKit();
		}
		else if( CityOwners == 0 )
		{
			m_bannerStatus = BANNER_STATUS_ALLIANCE;
			Status = 100;

			// countz
			UpdateTowerCount();
			SetArtKit();
		}

		// start the event timer
		RegisterAIUpdateEvent(UPDATE_PERIOD);
	}

void UpdateTowerCount()
{
	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());

	if(!plr)
		return;

	plr->CastSpell(plr, NA_CAPTURE_BUFF, false);
}

void SpawnA()
{
	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());

	uint32 Team = plr->GetTeam();
	
	GameObject* P1 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182275, -1507.94f, 8132.10f, -19.55f, 0, false, 0, 0, 1);
	GameObject* P2 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182276, -1384.53f, 7779.39f, -11.17f, 0, false, 0, 0, 1);
	GameObject* P3 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182277, -1650.28f, 7732.18f, -15.44f, 0, false, 0, 0, 1);
	GameObject* P4 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182297, -1815.80f, 8036.50f, -26.23f, 0, false, 0, 0, 1);

	P1->SetUInt32Value(GAMEOBJECT_FACTION, 2);
	P2->SetUInt32Value(GAMEOBJECT_FACTION, 2);
	P3->SetUInt32Value(GAMEOBJECT_FACTION, 2);
	P4->SetUInt32Value(GAMEOBJECT_FACTION, 2);

	P1->PushToWorld(_gameobject->GetMapMgr());
	P2->PushToWorld(_gameobject->GetMapMgr());
	P3->PushToWorld(_gameobject->GetMapMgr());
	P4->PushToWorld(_gameobject->GetMapMgr());

	struct snpcdata
{
	uint32 entry;
	float posx;
	float posy;
	float posz;
	float facing;
};

const static snpcdata AllianceControlNPCs[] =
{
    {18817, -1591.18f, 8020.39f, -22.2042f, 4.59022f},//469
    {18822, -1588.0f, 8019.0f, -22.2042f, 4.06662f},
    {21485, -1521.93f, 7927.37f, -20.2299f, 3.24631f},
    {21487, -1540.33f, 7971.95f, -20.7186f, 3.07178f},
    {21488, -1570.01f, 7993.8f, -22.4505f, 5.02655f},
    {18256, -1654.06f, 8000.46f, -26.59f, 3.37f},
    {18256, -1487.18f, 7899.1f, -19.53f, 0.954f},
    {18256, -1480.88f, 7908.79f, -19.19f, 4.485f},
    {18256, -1540.56f, 7995.44f, -20.45f, 0.947f},
    {18256, -1546.95f, 8000.85f, -20.72f, 6.035f},
    {18256, -1595.31f, 7860.53f, -21.51f, 3.747f},
    {18256, -1642.31f, 7995.59f, -25.8f, 3.317f},
    {18256, -1545.46f, 7995.35f, -20.63f, 1.094f},
    {18256, -1487.58f, 7907.99f, -19.27f, 5.567f},
    {18256, -1651.54f, 7988.56f, -26.5289f, 2.98451f},
    {18256, -1602.46f, 7866.43f, -22.1177f, 4.74729f},
    {18256, -1591.22f, 7875.29f, -22.3536f, 4.34587f},
    {18256, -1603.75f, 8000.36f, -24.18f, 4.516f},
    {18256, -1585.73f, 7994.68f, -23.29f, 4.439f},
    {18256, -1595.5f, 7991.27f, -23.53f, 4.738f}
};

	uint32 i;
	const snpcdata* b;
	for(i = 0; i < 20; ++i)
	{
	b = &AllianceControlNPCs[i];

	Creature* B1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(b->entry, b->posx, b->posy, b->posz, b->facing, true, 0, 0, 0, 1);
	if(B1 == NULL)
		continue;

	B1->PushToWorld(_gameobject->GetMapMgr());
	}
}

void SpawnH()
{
	Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());

	GameObject* P1 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182275, -1507.94f, 8132.10f, -19.55f, 0, false, 0, 0, 1);
	GameObject* P2 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182276, -1384.53f, 7779.39f, -11.17f, 0, false, 0, 0, 1);
	GameObject* P3 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182277, -1650.28f, 7732.18f, -15.44f, 0, false, 0, 0, 1);
	GameObject* P4 = _gameobject->GetMapMgr()->GetInterface()->SpawnGameObject(182297, -1815.80f, 8036.50f, -26.23f, 0, false, 0, 0, 1);

	P1->SetUInt32Value(GAMEOBJECT_FACTION, 1);
	P2->SetUInt32Value(GAMEOBJECT_FACTION, 1);
	P3->SetUInt32Value(GAMEOBJECT_FACTION, 1);
	P4->SetUInt32Value(GAMEOBJECT_FACTION, 1);

	P1->PushToWorld(_gameobject->GetMapMgr());
	P2->PushToWorld(_gameobject->GetMapMgr());
	P3->PushToWorld(_gameobject->GetMapMgr());
	P4->PushToWorld(_gameobject->GetMapMgr());

	struct snpcdata
{
	uint32 entry;
	float posx;
	float posy;
	float posz;
	float facing;
};

const static snpcdata HordeControlNPCs[] =
{
    {18816, -1523.92f, 7951.76f, -17.6942f, 3.51172f},//67
    {18821, -1527.75f, 7952.46f, -17.6948f, 3.99317f},
    {21474, -1520.14f, 7927.11f, -20.2527f, 3.39389f},
    {21484, -1524.84f, 7930.34f, -20.182f, 3.6405f},
    {21483, -1570.01f, 7993.8f, -22.4505f, 5.02655f},
    {18192, -1654.06f, 8000.46f, -26.59f, 3.37f},
    {18192, -1487.18f, 7899.1f, -19.53f, 0.954f},
    {18192, -1480.88f, 7908.79f, -19.19f, 4.485f},
    {18192, -1540.56f, 7995.44f, -20.45f, 0.947f},
    {18192, -1546.95f, 8000.85f, -20.72f, 6.035f},
    {18192, -1595.31f, 7860.53f, -21.51f, 3.747f},
    {18192, -1642.31f, 7995.59f, -25.8f, 3.317f},
    {18192, -1545.46f, 7995.35f, -20.63f, 1.094f},
    {18192, -1487.58f, 7907.99f, -19.27f, 5.567f},
    {18192, -1651.54f, 7988.56f, -26.5289f, 2.98451f},
    {18192, -1602.46f, 7866.43f, -22.1177f, 4.74729f},
    {18192, -1591.22f, 7875.29f, -22.3536f, 4.34587f},
    {18192, -1550.6f, 7944.45f, -21.63f, 3.559f},
    {18192, -1545.57f, 7935.83f, -21.13f, 3.448f},
    {18192, -1550.86f, 7937.56f, -21.7f, 3.801f}
};

	uint32 i;
	const snpcdata* b;
	for(i = 0; i < 20; ++i)
	{
	b = &HordeControlNPCs[i];

	Creature* B1 = _gameobject->GetMapMgr()->GetInterface()->SpawnCreature(b->entry, b->posx, b->posy, b->posz, b->facing, true, 0, 0, 0, 1);
	if(B1 == NULL)
		continue;

	B1->PushToWorld(_gameobject->GetMapMgr());
	}
}
};

//////////////////////////////////////////////////////////////////////////
// Hooks
//////////////////////////////////////////////////////////////////////////

void ZoneHookNagrand(Player* plr, uint32 Zone, uint32 OldZone)
{
	if( Zone == ZONE_NAGRAND )
	{
		if( CityOwners == plr->GetTeam() )
			{
			plr->CastSpell(plr, NA_CAPTURE_BUFF, false);
			}
		else
			{
			plr->RemoveAura(NA_CAPTURE_BUFF);
			}
	}
	else if( OldZone == ZONE_NAGRAND )
	{
			plr->RemoveAura(NA_CAPTURE_BUFF);
	}
}

void TokensHalaa(Player* pPlayer, Player* pVictim)
{
if(!pPlayer || !pVictim)
		return;

	if(pPlayer->GetTeam() == pVictim->GetTeam())
		return;

	uint32 TokenSpell = pPlayer->GetTeam() == 0 ? NA_KILL_TOKEN_ALLIANCE : NA_KILL_TOKEN_HORDE;

            pPlayer->CastSpell(pPlayer, TokenSpell, true);
}

//////////////////////////////////////////////////////////////////////////
// Creatures
//////////////////////////////////////////////////////////////////////////

class HalaaniGuard : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HalaaniGuard);
		HalaaniGuard(Creature* pCreature) : CreatureAIScript(pCreature) 
		{
			RegisterAIUpdateEvent(500);
		};

		void OnDied(Unit* mKiller)
		{
		g_guards--;
		_unit->GetMapMgr()->GetWorldStatesHandler().SetWorldStateForZone(ZONE_NAGRAND, _unit->GetMapMgr()->GetAreaID( _unit->GetPositionX(), _unit->GetPositionY() ), NA_UI_GUARDS_LEFT, g_guards);
		}

		void AIUpdate()
		{
			if(g_guards == 0)
				_unit->Despawn(0, 0);
		}
};

class Npcs : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Npcs);
		Npcs(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			RegisterAIUpdateEvent(500);
		};

		void AIUpdate()
		{
			if(g_guards == 0)
				_unit->Despawn(0, 0);
		}
};

uint32 Go[] =
{
	182275,
	182276,
	182277,
	182297,
	0
};

uint32 NPCS[] =
{
	18256,
	18192,
	0
};

uint32 Npc[] =
{
	18816,
	18821,
	21474,
	21484,
	21483,
	18817,
	18822,
	21485,
	21487,
	21488,
	0
};

void SetupPvPNagrand(ScriptMgr* mgr)
{
	//Gameobjects
	mgr->register_gameobject_script(182267, &GryphonHorstAI::Create);
	mgr->register_gameobject_script(182210, &HalaaFlagAI::Create);
	mgr->register_gameobject_script(Go, &GryphonHorstDestroyedAI::Create);

	//Hooks
	mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)TokensHalaa);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void*)ZoneHookNagrand);

	//Creatures
	mgr->register_creature_script(NPCS, &HalaaniGuard::Create);
	mgr->register_creature_script(Npc, &Npcs::Create);
}

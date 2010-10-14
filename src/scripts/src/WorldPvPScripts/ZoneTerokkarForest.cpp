/**
 * Summit MMORPG Server Software
 * Copyright (c) 2008 Summit Server Team
 * See COPYING for license details.
 */

#include "StdAfx.h"

//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (183104, 183411, 182301, 183412, 183413, 183414);

#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 20

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
static const uint32 g_allianceStateFields[5]	= {	WORLDSTATE_TEROKKAR_TOWER1_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER2_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER3_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER4_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER5_ALLIANCE };
static const uint32 g_neutralStateFields[5]		= {	WORLDSTATE_TEROKKAR_TOWER1_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER2_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER3_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER4_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER5_NEUTRAL };

// updates clients visual counter, and adds the buffs to players if needed
HEARTHSTONE_INLINE void UpdateTowerCount(shared_ptr<MapMgr> mgr)
{
	mgr->GetStateManager().UpdateWorldState(WORLDSTATE_TEROKKAR_ALLIANCE_TOWERS_CONTROLLED, TFg_allianceTowers);
	mgr->GetStateManager().UpdateWorldState(WORLDSTATE_TEROKKAR_HORDE_TOWERS_CONTROLLED, TFg_hordeTowers);

	if( TFg_superiorTeam == 0 && TFg_allianceTowers != 5 )
	{
		mgr->RemovePositiveAuraFromPlayers(0, BLESSING_OF_AUCHINDOUND);
		TFg_superiorTeam = -1;
	}

	if( TFg_superiorTeam == 1 && TFg_hordeTowers != 5 )
	{
		mgr->RemovePositiveAuraFromPlayers(1, BLESSING_OF_AUCHINDOUND);
		TFg_superiorTeam = -1;
	}

	if( TFg_allianceTowers == 5 && TFg_superiorTeam != 0 )
	{
		TFg_superiorTeam = 0;
		mgr->CastSpellOnPlayers(0, BLESSING_OF_AUCHINDOUND);
	}

	if( TFg_hordeTowers == 5 && TFg_superiorTeam != 1 )
	{
		TFg_superiorTeam = 1;
		mgr->CastSpellOnPlayers(1, BLESSING_OF_AUCHINDOUND);
	}
}

enum BannerStatus
{
	BANNER_STATUS_NEUTRAL = 0,
	BANNER_STATUS_ALLIANCE = 1,
	BANNER_STATUS_HORDE = 2,
};

class TerokkarForestBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	uint32 m_bannerStatus;

public:

	TerokkarForestBannerAI(GameObjectPointer go) : GameObjectAIScript(go)
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

		unordered_set<PlayerPointer>::iterator itr = _gameobject->GetInRangePlayerSetBegin();		
		unordered_set<PlayerPointer>::iterator itrend = _gameobject->GetInRangePlayerSetEnd();
		map<uint32,uint32>::iterator it2, it3;
		uint32 timeptr = (uint32)UNIXTIME;
		bool in_range;
		bool is_valid;
		PlayerPointer plr;
		
		for(; itr != itrend; ++itr)
		{
			if( !(*itr)->IsPvPFlagged() || (*itr)->InStealth() )
				is_valid = false;
			else
				is_valid = true;

			in_range = (_gameobject->GetDistance2dSq((*itr)) <= BANNER_RANGE) ? true : false;

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				// new player :)
				if( in_range )
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 1);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE, Status);
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
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE, Status);
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
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00%s has been taken by the Alliance!|r", ControlPointName);

			// tower update
			TFg_allianceTowers++;
			UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields[towerid], 1);

			// woot
			TFg_towerOwners[towerid] = 1;
			UpdateInDB();
		}
		else if( Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00%s has been taken by the Horde!|r", ControlPointName);
			
			// tower update
			TFg_hordeTowers++;
			UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields[towerid], 1);

			// woot
			TFg_towerOwners[towerid] = 0;
			UpdateInDB();
		}
		else if( m_bannerStatus != BANNER_STATUS_NEUTRAL )
		{
			// if the difference for the faction is >50, change to neutral
			if( m_bannerStatus == BANNER_STATUS_ALLIANCE && Status <= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();
				
				TFg_allianceTowers--;
				UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00The Alliance have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 1);

				// woot
				TFg_towerOwners[towerid] = -1;
				UpdateInDB();
			}
			else if( m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();

				TFg_hordeTowers--;
				UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00The Horde have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 1);

				// woot
				TFg_towerOwners[towerid] = -1;
				UpdateInDB();
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
				if( plr )
					plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 0);

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
		static const uint32 artkits_flagpole[3] = { 3, 2, 1 };
//		_gameobject->SetUInt32Value(GAMEOBJECT_ARTKIT, artkits_flagpole[m_bannerStatus]);
	}

	void OnSpawn()
	{
		m_bannerStatus = BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( TFg_towerOwners[towerid] == 1 )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			Status = 0;

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);

			// countz
			TFg_hordeTowers++;
			UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
		}
		else if( TFg_towerOwners[towerid] == 0 )
		{
			m_bannerStatus = BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);

			// countz
			TFg_allianceTowers++;
			UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
		}

		// start the event timer
		RegisterAIUpdateEvent(UPDATE_PERIOD);
	}

	//////////////////////////////////////////////////////////////////////////
	// Save Data To DB
	//////////////////////////////////////////////////////////////////////////
	void UpdateInDB()
	{
		static const char * fieldnames[TOWER_COUNT] = { "Terokkar-Tower1-status", "Terokkar-Tower2-status", "Terokkar-Tower3-status", "Terokkar-Tower4-status", "Terokkar-Tower5-status" };
		const char * msg = "-1";
		if( Status == 100 )
			msg = "0";
		else
			msg = "1";
			
		WorldStateManager::SetPersistantSetting(fieldnames[towerid], msg);
	}
};

//////////////////////////////////////////////////////////////////////////
// Zone Hook
//////////////////////////////////////////////////////////////////////////

void TFZoneHook(PlayerPointer plr, uint32 Zone, uint32 OldZone)
{
	if(!plr)
		return;

	if( Zone == ZONE_TEROKKAR_FOREST )
	{
		if( TFg_superiorTeam == plr->GetTeam() )
			plr->CastSpell(plr, dbcSpell.LookupEntry(BLESSING_OF_AUCHINDOUND), true);
	}
	else if( OldZone == ZONE_TEROKKAR_FOREST )
	{
		if( TFg_superiorTeam == plr->GetTeam() )
			plr->RemovePositiveAura(BLESSING_OF_AUCHINDOUND);
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

void TFSpawnObjects(shared_ptr<MapMgr> pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	const static sgodata godata[] = {
		{ 183104,-3081.65f,5335.03f,17.1853f,-2.14675f, 0, 0, 0.878817f,-0.477159f, 1, 32, 0, 1 },
		{ 183411,-2939.9f,4788.73f,18.987f,2.77507f, 0, 0, 0.983255f,0.182236f, 1, 32, 0, 1 },
		{ 183412,-3174.94f,4440.97f,16.2281f,1.86750f, 0, 0, 0.803857f,0.594823f, 1, 32, 0, 1 },
		{ 183413,-3603.31f,4529.15f,20.9077f,0.994838f, 0, 0, 0.477159f,0.878817f, 1, 32, 0, 1 },
		{ 183414,-3812.37f,4899.3f,17.7249f,0.087266f, 0, 0, 0.043619f,0.999048f, 1, 32, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	uint32 i;
	const sgodata *p;
	for(i = 0; i < 5; ++i)
	{
		p = &godata[i];

		GameObjectPointer pGo = NULLGOB;
		pGo = pmgr->GetInterface()->SpawnGameObject(p->entry, p->posx, p->posy, p->posz, p->facing, false, 0, 0);
		if( !pGo )
			continue;

		pGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, p->state);
		pGo->SetUInt32Value(GAMEOBJECT_FLAGS, p->flags);
		pGo->SetUInt32Value(GAMEOBJECT_FACTION, p->faction);

		for(uint32 j = 0; j < 4; ++j)
		{
			pGo->SetFloatValue(GAMEOBJECT_ROTATION+j, p->orientation[j]);
		}

		// now make his script
		pGo->SetScript(new TerokkarForestBannerAI(pGo));

		pGo->PushToWorld(pmgr);

		pGo->GetScript()->OnSpawn();
	}
}

void SetupPvPTerokkarForest(ScriptMgr *mgr)
{
	// register instance hooker
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void *)&TFZoneHook);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_CONTINENT_CREATE, (void *)&TFSpawnObjects);

	// load data
	const string Tower1 = WorldStateManager::GetPersistantSetting("Terokkar-Tower1-status", "-1");
	const string Tower2 = WorldStateManager::GetPersistantSetting("Terokkar-Tower2-status", "-1");
	const string Tower3 = WorldStateManager::GetPersistantSetting("Terokkar-Tower3-status", "-1");
	const string Tower4 = WorldStateManager::GetPersistantSetting("Terokkar-Tower4-status", "-1");
	const string Tower5 = WorldStateManager::GetPersistantSetting("Terokkar-Tower5-status", "-1");

	TFg_towerOwners[TOWER_1] = atoi(Tower1.c_str());
	TFg_towerOwners[TOWER_2] = atoi(Tower2.c_str());
	TFg_towerOwners[TOWER_3] = atoi(Tower3.c_str());
	TFg_towerOwners[TOWER_4] = atoi(Tower4.c_str());
	TFg_towerOwners[TOWER_5] = atoi(Tower5.c_str());
}



/**
 * Summit MMORPG Server Software
 * Copyright (c) 2008 Summit Server Team
 * See COPYING for license details.
 */

#include "StdAfx.h"

// Some customizable defines.
// Maybe move these to config?

#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 20

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

// updates clients visual counter, and adds the buffs to players if needed
HEARTHSTONE_INLINE void UpdateTowerCount(shared_ptr<MapMgr> mgr)
{
	if(!mgr)
		return;

	mgr->GetStateManager().UpdateWorldState(WORLDSTATE_HELLFIRE_ALLIANCE_TOWERS_CONTROLLED, g_allianceTowers);
	mgr->GetStateManager().UpdateWorldState(WORLDSTATE_HELLFIRE_HORDE_TOWERS_CONTROLLED, g_hordeTowers);

	if( g_superiorTeam == 0 && g_allianceTowers != 3 )
	{
		mgr->RemovePositiveAuraFromPlayers(0, HELLFIRE_SUPERORITY_ALLIANCE);
		g_superiorTeam = -1;
	}

	if( g_superiorTeam == 1 && g_hordeTowers != 3 )
	{
		mgr->RemovePositiveAuraFromPlayers(1, HELLFIRE_SUPERORITY_HORDE);
		g_superiorTeam = -1;
	}

	if( g_allianceTowers == 3 && g_superiorTeam != 0 )
	{
		g_superiorTeam = 0;
		mgr->CastSpellOnPlayers(0, HELLFIRE_SUPERORITY_ALLIANCE);
	}

	if( g_hordeTowers == 3 && g_superiorTeam != 1 )
	{
		g_superiorTeam = 1;
		mgr->CastSpellOnPlayers(1, HELLFIRE_SUPERORITY_HORDE);
	}
}

enum BannerStatus
{
	BANNER_STATUS_NEUTRAL = 0,
	BANNER_STATUS_ALLIANCE = 1,
	BANNER_STATUS_HORDE = 2,
};

class HellfirePeninsulaBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	uint32 m_bannerStatus;

public:
	GameObjectPointer  pBanner;

	HellfirePeninsulaBannerAI(GameObjectPointer go) : GameObjectAIScript(go)
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

		unordered_set<PlayerPointer>::iterator itr = _gameobject->GetInRangePlayerSetBegin();		
		unordered_set<PlayerPointer>::iterator itrend = _gameobject->GetInRangePlayerSetEnd();
		map<uint32,uint32>::iterator it2, it3;
		uint32 timeptr = (uint32)UNIXTIME;
		bool in_range;
		bool is_valid;
		PlayerPointer plr = NULLPLR;
		
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
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 1);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE, Status);
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
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE, Status);
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
			UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields[towerid], 1);

			// woot
			g_towerOwners[towerid] = 1;
			UpdateInDB();
		}
		else if( Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00%s has been taken by the Horde!|r", ControlPointName);
			
			// tower update
			g_hordeTowers++;
			UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields[towerid], 1);

			// woot
			g_towerOwners[towerid] = 0;
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
				
				g_allianceTowers--;
				UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00The Alliance have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 1);

				// woot
				g_towerOwners[towerid] = -1;
				UpdateInDB();
			}
			else if( m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				SetArtKit();

				g_hordeTowers--;
				UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00The Horde have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 1);

				// woot
				g_towerOwners[towerid] = -1;
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

		// set away - we don't know the artkits anymore :(((
		//_gameobject->SetUInt32Value(GAMEOBJECT_ARTKIT, artkits_flagpole[m_bannerStatus]);
		//pBanner->SetUInt32Value(GAMEOBJECT_ARTKIT, artkits_towers[towerid][m_bannerStatus]);
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
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);

			// countz
			g_hordeTowers++;
			UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
		}
		else if( g_towerOwners[towerid] == 0 )
		{
			m_bannerStatus = BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields[towerid], 0);

			// countz
			g_allianceTowers++;
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
		static const char * fieldnames[TOWER_COUNT] = { "hellfire-stadium-status", "hellfire-overlook-status", "hellfire-brokenhill-status" };
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

void ZoneHook(PlayerPointer plr, uint32 Zone, uint32 OldZone)
{
	static uint32 spellids[2] = { HELLFIRE_SUPERORITY_ALLIANCE, HELLFIRE_SUPERORITY_HORDE };
	if( Zone == ZONE_HELLFIRE_PENINSULA )
	{
		if( g_superiorTeam == plr->GetTeam() )
			plr->CastSpell(plr, dbcSpell.LookupEntry(spellids[plr->GetTeam()]), true);
	}
	else if( OldZone == ZONE_HELLFIRE_PENINSULA )
	{
		if( g_superiorTeam == plr->GetTeam() )
			plr->RemovePositiveAura(spellids[plr->GetTeam()]);
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

void SpawnObjects(shared_ptr<MapMgr> pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	const static sgodata godata[] = {
		{ 182173, -290.016f, 3702.42f, 56.6729f, 0.0349066f, 0, 0, 0.0174524f, 0.999848f, 1, 32, 0, 1 },			// stadium
		{ 182174, -184.889f, 3476.93f, 38.205f, -0.0174535f, 0, 0, 0.00872664f, -0.999962f, 1, 32, 0, 1 },			// overlook
		{ 182175, -471.462f, 3451.09f, 34.6432f, 0.174533f, 0, 0, 0.0871557f, 0.996195f, 1, 32, 0, 1 },				// brokenhill
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	const static sgodata godata_banner[] = {
		{ 183515, -289.61f, 3696.83f, 75.9447f, 3.12414f, 0, 0, 0.999962f, 0.00872656f, 1, 32, 1375, 1 },					// stadium
		{ 182525, -187.887f, 3459.38f, 60.0403f, -3.12414f, 0, 0, 0.999962f, -0.00872653f, 1, 32, 1375, 1 },				// overlook
		{ 183514, -467.078f, 3528.17f, 64.7121f, 3.14159f, 0, 0, 1, -4.37114E-8f, 1, 32, 1375, 1 },						// brokenhill
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	uint32 i;
	const sgodata *p, *p2;
	for(i = 0; i < 3; ++i)
	{
		p = &godata[i];
		p2 = &godata_banner[i];

		GameObjectPointer pGo = pmgr->GetInterface()->SpawnGameObject(p->entry, p->posx, p->posy, p->posz, p->facing, false, 0, 0);
		if( pGo == NULL )
			continue;

		GameObjectPointer pGo2 = pmgr->GetInterface()->SpawnGameObject(p2->entry, p2->posx, p2->posy, p2->posz, p2->facing, false, 0, 0);
		if( pGo2 == NULL )
			continue;

		pGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, p->state);
		pGo2->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, p2->state);
		pGo->SetUInt32Value(GAMEOBJECT_FLAGS, p->flags);
		pGo2->SetUInt32Value(GAMEOBJECT_FLAGS, p2->flags);
		pGo->SetUInt32Value(GAMEOBJECT_FACTION, p->faction);
		pGo2->SetUInt32Value(GAMEOBJECT_FACTION, p2->faction);

		for(uint32 j = 0; j < 4; ++j)
		{
			pGo->SetFloatValue(GAMEOBJECT_ROTATION+j, p->orientation[j]);
			pGo2->SetFloatValue(GAMEOBJECT_ROTATION+j, p2->orientation[j]);
		}

		// now make his script
		pGo->SetScript(new HellfirePeninsulaBannerAI(pGo));
		((HellfirePeninsulaBannerAI*)pGo->GetScript())->pBanner = pGo2;

		pGo->PushToWorld(pmgr);
		pGo2->PushToWorld(pmgr);

		pGo->GetScript()->OnSpawn();
		printf("Spawned gameobject entry %u for world pvp on hellfire.\n", p->entry);
	}
}


void SetupPvPHellfirePeninsula(ScriptMgr *mgr)
{
	// register instance hooker
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void *)&ZoneHook);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_CONTINENT_CREATE, (void *)&SpawnObjects);

	// load data
	const string tstadium = WorldStateManager::GetPersistantSetting("hellfire-stadium-status", "-1");
	const string toverlook = WorldStateManager::GetPersistantSetting("hellfire-overlook-status", "-1");
	const string tbrokenhill = WorldStateManager::GetPersistantSetting("hellfire-brokenhill-status", "-1");

	g_towerOwners[TOWER_STADIUM] = atoi(tstadium.c_str());
	g_towerOwners[TOWER_OVERLOOK] = atoi(toverlook.c_str());
	g_towerOwners[TOWER_BROKENHILL] = atoi(tbrokenhill.c_str());
}



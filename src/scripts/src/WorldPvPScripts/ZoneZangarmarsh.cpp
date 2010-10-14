/**
 * Summit MMORPG Server Software
 * Copyright (c) 2008 Summit Server Team
 * See COPYING for license details.
 */

#include "StdAfx.h"

//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (182523, 182522);

#define BANNER_RANGE 900
#define UPDATE_PERIOD 5000
#define CAPTURE_RATE 20

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

void SetGrave(shared_ptr<MapMgr> pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	// any better solution?
	uint32 gOwners = ZMCityOwners == 0 ? 0 : 1;
	if( ZMCityOwners == -1 )
		gOwners = 3;

	StorageContainerIterator<GraveyardTeleport> * itr;
	itr = GraveyardStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		GraveyardTeleport *pGrave = itr->Get();
		if( pGrave->ID == 142 )
		{
			pGrave->FactionID = gOwners;
			break;
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	if( ZMCityOwners == 0 )
	{
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 1);
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);			
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else if( ZMCityOwners == 1 )
	{
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_HORDE, 1);
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else
	{
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);
		pmgr->GetStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
	}
}

HEARTHSTONE_INLINE void UpdateTowerCountZM(shared_ptr<MapMgr> mgr)
{
	if(!mgr)
		return;

	if( ZMg_superiorTeam == 0 && ZMg_allianceTowers != TOWER_COUNT )
	{
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE, 1);
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE, 0);
		ZMg_superiorTeam = -1;
	}

	if( ZMg_superiorTeam == 1 && ZMg_hordeTowers != TOWER_COUNT )
	{
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE, 1);
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE, 0);
		ZMg_superiorTeam = -1;
	}

	if( ZMg_allianceTowers == TOWER_COUNT && ZMg_superiorTeam != 0 )
	{
		ZMg_superiorTeam = 0;
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE, 0);
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE, 1);
	}

	if( ZMg_hordeTowers == TOWER_COUNT && ZMg_superiorTeam != 1 )
	{
		ZMg_superiorTeam = 1;
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE, 0);
		mgr->GetStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE, 1);
	}
}

enum BannerStatus
{
	BANNER_STATUS_NEUTRAL = 0,
	BANNER_STATUS_ALLIANCE = 1,
	BANNER_STATUS_HORDE = 2,
};

class ZangarmarshBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	uint32 m_bannerStatus;

public:

	ZangarmarshBannerAI(GameObjectPointer go) : GameObjectAIScript(go)
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
				if( in_range )
				{
					if( towerid == TOWER_WEST )
					{
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 1);
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST, Status);
					}
					else
					{
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 1);
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST, Status);
					}
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
					if( towerid == TOWER_WEST )
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 0);
					else
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					if( towerid == TOWER_WEST )
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST, Status);
					else
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST, Status);
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

			// send message to everyone in the zone, has been captured by the Alliance
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00%s has been taken by the Alliance!|r", ControlPointName);

			// tower update
			ZMg_allianceTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_MAP[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields_MAP[towerid], 1);
			
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_UI[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields_UI[towerid], 1);

			// woot
			ZMg_towerOwners[towerid] = 1;
			UpdateInDB();
		}
		else if( Status == 0 && m_bannerStatus != BANNER_STATUS_HORDE )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00%s has been taken by the Horde!|r", ControlPointName);
			
			// tower update
			ZMg_hordeTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_MAP[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields_MAP[towerid], 1);

			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_UI[towerid], 0);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields_UI[towerid], 1);

			// woot
			ZMg_towerOwners[towerid] = 0;
			UpdateInDB();
		}
		else if( m_bannerStatus != BANNER_STATUS_NEUTRAL )
		{
			// if the difference for the faction is >50, change to neutral
			if( m_bannerStatus == BANNER_STATUS_ALLIANCE && Status <= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;
				
				ZMg_allianceTowers--;
				UpdateTowerCountZM(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00The Alliance have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields_MAP[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_MAP[towerid], 1);

				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields_UI[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_UI[towerid], 1);

				// woot
				ZMg_towerOwners[towerid] = -1;
				UpdateInDB();
			}
			else if( m_bannerStatus == BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = BANNER_STATUS_NEUTRAL;

				ZMg_hordeTowers--;
				UpdateTowerCountZM(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00The Horde have lost control of %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields_MAP[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_MAP[towerid], 1);

				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields_UI[towerid], 0);
				_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_UI[towerid], 1);

				// woot
				ZMg_towerOwners[towerid] = -1;
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
				{
					if( towerid == TOWER_WEST )
						plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 0);
					else
						plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 0);
				}

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
	void OnSpawn()
	{
		m_bannerStatus = BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( ZMg_towerOwners[towerid] == 1 )
		{
			m_bannerStatus = BANNER_STATUS_HORDE;
			Status = 0;

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields_MAP[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_MAP[towerid], 0);

			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_hordeStateFields_UI[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_UI[towerid], 0);

			// countz
			ZMg_hordeTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());
		}
		else if( ZMg_towerOwners[towerid] == 0 )
		{
			m_bannerStatus = BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields_MAP[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_MAP[towerid], 0);

			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_allianceStateFields_UI[towerid], 1);
			_gameobject->GetMapMgr()->GetStateManager().UpdateWorldState(g_neutralStateFields_UI[towerid], 0);

			// countz
			ZMg_allianceTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());
		}

		// start the event timer
		RegisterAIUpdateEvent(UPDATE_PERIOD);
	}

	//////////////////////////////////////////////////////////////////////////
	// Save Data To DB
	//////////////////////////////////////////////////////////////////////////
	void UpdateInDB()
	{
		static const char * fieldnames[TOWER_COUNT] = { "Zangarmarsh-TowerWest-status", "Zangarmarsh-TowerEast-status" };
		const char * msg = "-1";
		if( Status == 100 )
			msg = "0";
		else
			msg = "1";
			
		WorldStateManager::SetPersistantSetting(fieldnames[towerid], msg);
	}
};

//////////////////////////////////////////////////////////////////////////
// Scouts AI
//////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL ZMScouts : public GossipScript
{
public:
	void GossipHello(ObjectPointer pObject, PlayerPointer  plr, bool AutoSend)
	{
		uint32 Team = plr->GetTeam();
		if(Team > 1) Team = 1;

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), Team == 0 ? 9433 : 9750, plr);

		if( pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR )
			Menu->AddItem( 0, "I would like to browse your goods.", 3 );

		if( ZMg_superiorTeam == Team && ZMCityOwners != Team && !plr->HasAura( Team == 0 ? 32430 : 32431 ) )
			Menu->AddItem( 0, Team == 0 ? "Give me the flag, I'll take it to the Central Tower for the glory of the Alliance!" : "Give me the flag, I'll take it to the Central Tower for the glory of the Horde!" , Team+1);
		if(AutoSend)
			Menu->SendTo(plr);
    }

    void GossipSelectOption(ObjectPointer pObject, PlayerPointer  plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if( !plr )
			return;
		
		CreaturePointer  pCreature = NULLCREATURE;
		pCreature = pObject->IsCreature() ? TO_CREATURE( pObject ) : NULLCREATURE;
		if( !pCreature )
			return;

		if( IntId == 3 )
		{
			plr->GetSession()->SendInventoryList(pCreature);
			return;
		}

		uint32 CastSpellID = IntId == 1 ? 32430 : 32431;
		if( CastSpellID != 0 )
			if( !plr->HasAura( CastSpellID ) )
				pCreature->CastSpell(plr,CastSpellID,true);
		return;
    }

	void Destroy()
	{
		delete this;
	};
};

class ZMCityBannerAI : public GameObjectAIScript
{
public:
	ZMCityBannerAI(GameObjectPointer goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObjectPointer  GO) { return new ZMCityBannerAI(GO); }

	void OnActivate(PlayerPointer  pPlayer)
	{
		if( !pPlayer )
			return;

		uint32 pTeam = pPlayer->GetTeam();
		if(pTeam > 1) pTeam = 1;

		if( ZMg_superiorTeam != pTeam || ZMCityOwners == pTeam )
			return;

		uint32 flagaura = ( pTeam == 0 ? 32430 : 32431 );

		if( pPlayer->HasAura( flagaura ) )
			pPlayer->RemoveAura( flagaura );
		else
			return; // we must have flag to capture city
		
		ZMCityOwners = pTeam; // we are now city owners! Yay
		//brrr....
		const char * cOwners = ZMCityOwners == 0 ? "0" : "1";
		if( ZMCityOwners == -1 ) // just in case
			cOwners = "-1";

		WorldStateManager::SetPersistantSetting("Zangarmarsh-city-owners", cOwners);
			
		SetGrave(_gameobject->GetMapMgr());

		uint32 oppositeTeam = ( pTeam == 0 ? 1 : 0 );

		_gameobject->GetMapMgr()->CastSpellOnPlayers(pTeam, TWIN_SPIRE_BLESSING);
		_gameobject->GetMapMgr()->RemovePositiveAuraFromPlayers(oppositeTeam, TWIN_SPIRE_BLESSING);

		if( _gameobject )
			_gameobject->Despawn(0);

		ZMSpawnBanners( pPlayer->GetMapMgr(), pTeam ); // spawn faction banner
	}
};

//////////////////////////////////////////////////////////////////////////
// Zone Hook
//////////////////////////////////////////////////////////////////////////

void ZMZoneHook(PlayerPointer plr, uint32 Zone, uint32 OldZone)
{
	if(!plr)
		return;

	if( Zone == ZONE_ZANGARMARSH )
	{
		if( ZMCityOwners == plr->GetTeam() )
			plr->CastSpell(plr, dbcSpell.LookupEntry(TWIN_SPIRE_BLESSING), true);
	}
	else if( OldZone == ZONE_ZANGARMARSH )
	{
		if( ZMCityOwners == plr->GetTeam() )
			plr->RemovePositiveAura(TWIN_SPIRE_BLESSING);
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

void ZMSpawnBanners(shared_ptr<MapMgr> bmgr, int32 side)
{
	if(!bmgr)
		return;
	// -1 = neutral
	//  0 = alliance
	//  1 = horde

	//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (182529, 182527, 182528);
	const static sgodata gobdata[] = {
		{ 182529, 253.54f, 7083.81f, 36.7728f, -0.017453f, 0, 0, 0.008727f, -0.999962f, 1, 0, 0 },
		{ 182527, 253.54f, 7083.81f, 36.7728f, -0.017453f, 0, 0, 0.008727f, -0.999962f, 1, 0, 0 },
		{ 182528, 253.54f, 7083.81f, 36.7728f, -0.017453f, 0, 0, 0.008727f, -0.999962f, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	uint32 i = side+1;
	if( i > 2 ) // how does that happen? oO
		i = 2;
	const sgodata *b;
	b = &gobdata[i];

	GameObjectPointer bGo = NULLGOB;
	bGo = bmgr->GetInterface()->SpawnGameObject(b->entry, b->posx, b->posy, b->posz, b->facing, false, 0, 0);
	if( !bGo )
		return;

	bGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, b->state);
	bGo->SetUInt32Value(GAMEOBJECT_FLAGS, b->flags);
	bGo->SetUInt32Value(GAMEOBJECT_FACTION, b->faction);

	for(uint32 j = 0; j < 4; ++j)
	{
		bGo->SetFloatValue(GAMEOBJECT_ROTATION+j, b->orientation[j]);
	}

	bGo->PushToWorld(bmgr);
}

void ZMSpawnObjects(shared_ptr<MapMgr> pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	ZMSpawnBanners(pmgr,ZMCityOwners);

	const static sgodata godata[] = {
		{ G_TOWER_EAST, 303.243f, 6841.36f, 40.1245f, -1.58825f, 0, 0, 0.71325f, -0.700909f, 1, 32, 0, 1 },
		{ G_TOWER_WEST, 336.466f, 7340.26f, 41.4984f, -1.58825f, 0, 0, 0.71325f, -0.700909f, 1, 32, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	uint32 i;
	const sgodata *p;
	for(i = 0; i < 2; ++i)
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
		pGo->SetScript(new ZangarmarshBannerAI(pGo));

		pGo->PushToWorld(pmgr);

		pGo->GetScript()->OnSpawn();
	}
}

void Tokens(PlayerPointer pPlayer, PlayerPointer pVictim)
{
	if( !pPlayer || !pVictim )
		return;
	if( !pPlayer->HasAura(TWIN_SPIRE_BLESSING) || pPlayer->GetTeam() == pVictim->GetTeam() )
		return;

	uint32 TokenSpell = pPlayer->GetTeam() == 0 ? 32155 : 32158;

	pPlayer->CastSpell(pPlayer,TokenSpell,true);
}

void SetupPvPZangarmarsh(ScriptMgr *mgr)
{
	// City banners AI
	mgr->register_gameobject_script(182529, &ZMCityBannerAI::Create);
	mgr->register_gameobject_script(182527, &ZMCityBannerAI::Create);
	mgr->register_gameobject_script(182528, &ZMCityBannerAI::Create);
	// scouts AI
	GossipScript *zms = (GossipScript*) new ZMScouts();
	mgr->register_gossip_script(18564, zms);
	mgr->register_gossip_script(18581, zms);

	// load data
	const string Tower1 = WorldStateManager::GetPersistantSetting("Zangarmarsh-TowerWest-status", "-1");
	const string Tower2 = WorldStateManager::GetPersistantSetting("Zangarmarsh-TowerEast-status", "-1");
	const string City = WorldStateManager::GetPersistantSetting("Zangarmarsh-city-owners", "-1");

	ZMg_towerOwners[TOWER_WEST] = atoi(Tower1.c_str());
	ZMg_towerOwners[TOWER_EAST] = atoi(Tower2.c_str());
	ZMCityOwners = atoi(City.c_str());
	mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)Tokens);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, (void *)&ZMZoneHook);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_CONTINENT_CREATE, (void *)&ZMSpawnObjects);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_CONTINENT_CREATE, (void *)&SetGrave);
}



/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#include "plugin.h"

#define ZONE_TEROKKAR_FOREST 3519

#define TF_TOWER_SCAN_UPDATE_FREQ (2*1000)

enum SpiritTowers
{
	TF_TOWER_NW    = 0,
	TF_TOWER_N     = 1,
	TF_TOWER_NE    = 2,
	TF_TOWER_SE    = 3,
	TF_TOWER_S     = 4,
	TF_TOWER_COUNT = 5
};

enum TerokkarBannerGOs
{
	GO_TEROKKAR_BANNER_NW = 183104,
	GO_TEROKKAR_BANNER_N  = 183411,
	GO_TEROKKAR_BANNER_NE = 183412,
	GO_TEROKKAR_BANNER_SE = 183413,
	GO_TEROKKAR_BANNER_S  = 183414
};

enum SpiritTowerArtkit
{
	ARTKIT_BANNER_HORDE    = 1,
	ARTKIT_BANNER_ALLIANCE = 2,
	ARTKIT_BANNER_NEUTRAL  = 21
};

enum TerokkarWorldStates
{
	WORLDSTATE_TF_TIMER_NEUTRAL              = 2508,
	WORLDSTATE_TF_TIMER_HOURS                = 2509,
	WORLDSTATE_TF_TIMER_MINUTES_2            = 2510,
	WORLDSTATE_TF_TIMER_MINUTES_1            = 2512,

	WORLDSTATE_TF_TOWERS_CONTROLLED_UI       = 2620,
	WORLDSTATE_TF_TOWERS_CONTROOLED_ALLIANCE = 2621,
	WORLDSTATE_TF_TOWERS_CONTROOLED_HORDE    = 2622,
	WORLDSTATE_TF_PROGRESS_BAR_UI            = 2623,
	WORLDSTATE_TF_PROGRESS_BAR_PROGRESS      = 2625,

	WORLDSTATE_TF_TOWER_NW_NEUTRAL           = 2681,
	WORLDSTATE_TF_TOWER_NW_HORDE             = 2682,
	WORLDSTATE_TF_TOWER_NW_ALLIANCE          = 2683,

	WORLDSTATE_TF_TOWER_N_ALLIANCE           = 2684,
	WORLDSTATE_TF_TOWER_N_HORDE              = 2685,
	WORLDSTATE_TF_TOWER_N_NEUTRAL            = 2686,

	WORLDSTATE_TF_TOWER_NE_ALLIANCE          = 2688,
	WORLDSTATE_TF_TOWER_NE_HORDE             = 2689,
	WORLDSTATE_TF_TOWER_NE_NEUTRAL           = 2690,

	WORLDSTATE_TF_TOWER_S_ALLIANCE           = 2691,
	WORLDSTATE_TF_TOWER_S_HORDE              = 2692,
	WORLDSTATE_TF_TOWER_S_NEUTRAL            = 2693,

	WORLDSTATE_TF_TOWER_SE_ALLIANCE          = 2694,
	WORLDSTATE_TF_TOWER_SE_HORDE             = 2695,
	WORLDSTATE_TF_TOWER_SE_NEUTRAL           = 2696,	

	WORLDSTATE_TF_TIMER_ALLIANCE             = 2767,
	WORLDSTATE_TF_TIMER_HORDE                = 2768
};

#define TF_TOWER_OWNER_NEUTRAL 2

static uint32 towerOwners[ TF_TOWER_COUNT ] = 
{
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL,
	TF_TOWER_OWNER_NEUTRAL
};

static uint8 towerArtkits[] = {
	ARTKIT_BANNER_ALLIANCE,
	ARTKIT_BANNER_HORDE,
	ARTKIT_BANNER_NEUTRAL
};

class TerokkarSpiritTowerAI : public GameObjectAIScript
{
private:
	uint32 towerId;

public:
	TerokkarSpiritTowerAI( GameObject *go ) : GameObjectAIScript( go )
	{
		towerId = std::numeric_limits< uint32 >::max();
	}

	ADD_GAMEOBJECT_FACTORY_FUNCTION( TerokkarSpiritTowerAI );

	void setArtkit()
	{
		_gameobject->SetArtKit( towerArtkits[ towerOwners[ towerId ] ] );
	}

	void OnSpawn()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}

		switch( _gameobject->GetInfo()->ID )
		{
			case GO_TEROKKAR_BANNER_NW:
				towerId = TF_TOWER_NW;
				break;

			case GO_TEROKKAR_BANNER_N:
				towerId = TF_TOWER_N;
				break;

			case GO_TEROKKAR_BANNER_NE:
				towerId = TF_TOWER_NE;
				break;

			case GO_TEROKKAR_BANNER_SE:
				towerId = TF_TOWER_SE;
				break;

			case GO_TEROKKAR_BANNER_S:
				towerId = TF_TOWER_S;
				break;
		}

		if( towerId >= TF_TOWER_COUNT )
		{
			return;
		}

		setArtkit();

		RegisterAIUpdateEvent( TF_TOWER_SCAN_UPDATE_FREQ );
	}

	void AIUpdate()
	{
		if( _gameobject->GetMapId() != MAP_OUTLAND )
		{
			return;
		}
	}
};

void setupTerokkarForest( ScriptMgr *mgr )
{
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_NW, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_N, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_NE, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_SE, &TerokkarSpiritTowerAI::Create );
	mgr->register_gameobject_script( GO_TEROKKAR_BANNER_S, &TerokkarSpiritTowerAI::Create );
}

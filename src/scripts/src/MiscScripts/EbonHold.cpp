/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
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
#include "Setup.h"

/************* Teleporters **************/
// Notes: Please try to keep this file clean if modifications are made.
// Also, please include references to header files, and locations for easy access, thanks.

// Lower level of Ebon Hold Teleporter / Teleport up
class Doodad_Nox_portal_purple_bossroom01 : public GameObjectAIScript
// Line 226 ScriptMgr.h
{
public:
	Doodad_Nox_portal_purple_bossroom01(GameObject* goinstance) : GameObjectAIScript( goinstance ) {}
	static GameObjectAIScript *Create(GameObject * GO)
	{ 
		return new Doodad_Nox_portal_purple_bossroom01(GO); 
	}

	void OnSpawn()
	{
		RegisterAIUpdateEvent( 1 );
	}

	void AIUpdate()
	{ 
		Player * plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
		if( !plr )
			return;
		// Teleport up (Coordinates should be almost exact to retail)
		// First if the plr is on map 609 then teleport him upstairs on map 609
		if( _gameobject->CalcDistance( _gameobject, plr ) <= 2.0f ) // You need to standing 2 meters from the actual center of the pink ring
		if (plr->GetMapId() == 609) 
		plr->SafeTeleport( 609, plr->GetInstanceID(), 2387.468994f, -5642.544922f, 420.809937f, 0.620461f); // Player.h declaration (Map, Instance, x, y, z, orientation)
		// bool SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O);
	
		// else if the plr is on map 0 then teleport him upstairs on map 0
		if( _gameobject->CalcDistance( _gameobject, plr ) <= 2.0f ) 
		if (plr->GetMapId() == 0)
		plr->SafeTeleport( 0, plr->GetInstanceID(), 2387.468994f, -5642.544922f, 420.809937f, 0.620461f);
	}
};

			
			

// Upper level of Ebon Hold Teleporter / Teleports down

class Doodad_Nox_portal_purple_bossroom17 : public GameObjectAIScript
{
public:
	Doodad_Nox_portal_purple_bossroom17(GameObject* goinstance) : GameObjectAIScript( goinstance ) {}
	static GameObjectAIScript *Create(GameObject * GO)
	{ 
		return new Doodad_Nox_portal_purple_bossroom17(GO); 
	}

	void OnSpawn()
	{
		RegisterAIUpdateEvent( 100 );
	}

	void AIUpdate()
	{ 
		Player * plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords( _gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
		if( !plr )
			return;			
			// Teleport down (Coordinates should be almost exact to retail)
		// First if the plr is on map 609 then teleport him downstairs on map 609
		if( _gameobject->CalcDistance( _gameobject, plr ) <= 2.0f ) // You need to standing 2 meters from the actual center of the pink ring
		if (plr->GetMapId() == 609) 
		plr->SafeTeleport( 609, plr->GetInstanceID(), 2395.233154f, -5637.875488f, 377.086884f, 0.520721f); // Player.h declaration (Map, Instance, x, y, z, orientation)
		// bool SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O);
		
		// else if the plr is on map 0 then teleport him downstairs on map 0
		if( _gameobject->CalcDistance( _gameobject, plr ) <= 2.0f ) 
		if (plr->GetMapId() == 0)
		plr->SafeTeleport( 0, plr->GetInstanceID(), 2395.233154f, -5637.875488f, 377.086884f, 0.520721f);
	}
};


void SetupEbonHold(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(191538, &Doodad_Nox_portal_purple_bossroom01::Create);
	mgr->register_gameobject_script(191539, &Doodad_Nox_portal_purple_bossroom17::Create);
}
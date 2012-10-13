/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

//
// MapMgr.cpp
//

#include "StdAfx.h"
#include "CrashHandler.h"

Arcemu::Utility::TLSObject<MapMgr*> t_currentMapContext;

#define MAP_MGR_UPDATE_PERIOD 100
#define MAPMGR_INACTIVE_MOVE_TIME 30

#define Z_SEARCH_RANGE 2


extern bool bServerShutdown;

MapMgr::MapMgr(Map* map, uint32 mapId, uint32 instanceid) :
CellHandler<MapCell>(map),
_mapId(mapId),
eventHolder(instanceid),
worldstateshandler( mapId )
{
	_terrain = new TerrainHolder(mapId);
	CollideInterface.ActivateMap(mapId);
	_shutdown = false;
	m_instanceID = instanceid;
	pMapInfo = WorldMapInfoStorage.LookupEntry(mapId);
	m_UpdateDistance = pMapInfo->update_distance * pMapInfo->update_distance;
	iInstanceMode = 0;

	// Create script interface
	ScriptInterface = new MapScriptInterface(*this);

	// Set up storage arrays
	CreatureStorage.resize(map->CreatureSpawnCount, NULL);
	GOStorage.resize(map->GameObjectSpawnCount, NULL);

	m_GOHighGuid = m_CreatureHighGuid = 0;
	m_DynamicObjectHighGuid = 0;
	lastUnitUpdate = getMSTime();
	lastGameobjectUpdate = getMSTime();
	m_battleground = NULL;

	m_holder = &eventHolder;
	m_event_Instanceid = eventHolder.GetInstanceID();
	forced_expire = false;
	InactiveMoveTime = 0;
	mLoopCounter = 0;
	pInstance = NULL;
	thread_kill_only = false;
	thread_running = false;

	m_forcedcells.clear();
	m_PlayerStorage.clear();
	m_PetStorage.clear();
	m_DynamicObjectStorage.clear();

	_combatProgress.clear();
	_mapWideStaticObjects.clear();
	//_worldStateSet.clear();
	_updates.clear();
	_processQueue.clear();
	Sessions.clear();

	activeGameObjects.clear();
	activeCreatures.clear();
	creature_iterator = activeCreatures.begin();
	pet_iterator = m_PetStorage.begin();
	m_corpses.clear();
	_sqlids_creatures.clear();
	_sqlids_gameobjects.clear();
	_reusable_guids_gameobject.clear();
	_reusable_guids_creature.clear();

	mInstanceScript = NULL;
}


MapMgr::~MapMgr()
{
	CollideInterface.DeactiveMap(_mapId);
	_shutdown = true;
	sEventMgr.RemoveEvents(this);
	if(ScriptInterface != NULL)
	{
		delete ScriptInterface;
		ScriptInterface = NULL;
	}

	delete _terrain;

	// Remove objects
	if(_cells)
	{
		for(uint32 i = 0; i < _sizeX; i++)
		{
			if(_cells[i] != 0)
			{
				for(uint32 j = 0; j < _sizeY; j++)
				{
					if(_cells[i][j] != 0)
					{
						_cells[i][j]->_unloadpending = false;
						_cells[i][j]->RemoveObjects();
					}
				}
			}
		}
	}

	for(set<Object*>::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
	{
		if((*itr)->IsInWorld())
			(*itr)->RemoveFromWorld(false);
		delete(*itr);
	}
	_mapWideStaticObjects.clear();

	GOStorage.clear();
	CreatureStorage.clear();

	Corpse* pCorpse;
	for(set<Corpse*>::iterator itr = m_corpses.begin(); itr != m_corpses.end();)
	{
		pCorpse = *itr;
		++itr;

		if(pCorpse->IsInWorld())
			pCorpse->RemoveFromWorld(false);

		delete pCorpse;
	}
	m_corpses.clear();

	if(mInstanceScript != NULL)
		mInstanceScript->Destroy();

	// Empty remaining containers
	m_PlayerStorage.clear();
	m_PetStorage.clear();
	m_DynamicObjectStorage.clear();

	_combatProgress.clear();
	_updates.clear();
	_processQueue.clear();
	Sessions.clear();

	activeCreatures.clear();
	activeGameObjects.clear();
	_sqlids_creatures.clear();
	_sqlids_gameobjects.clear();
	_reusable_guids_creature.clear();
	_reusable_guids_gameobject.clear();

	if(m_battleground)
	{
		m_battleground = NULL;
	}

	Log.Notice("MapMgr", "Instance %u shut down. (%s)" , m_instanceID, GetBaseMap()->GetName());
}

uint32 MapMgr::GetTeamPlayersCount(uint32 teamId)
{
	uint32 result = 0;
	PlayerStorageMap::iterator itr = m_PlayerStorage.begin();
	for(; itr != m_PlayerStorage.end(); itr++)
	{
		Player* pPlayer = (itr->second);
		if(pPlayer->GetTeam() == teamId)
			result++;
	}
	return result;
}


void MapMgr::PushObject(Object* obj)
{
	/////////////
	// Assertions
	/////////////
	ARCEMU_ASSERT(obj != NULL);

	// That object types are not map objects. TODO: add AI groups here?
	if(obj->IsItem() || obj->IsContainer())
	{
		// mark object as updatable and exit
		return;
	}

	if(obj->IsCorpse())
	{
		m_corpses.insert(TO< Corpse* >(obj));
	}

	obj->ClearInRangeSet();

	ARCEMU_ASSERT(obj->GetMapId() == _mapId);
	if(!(obj->GetPositionX() < _maxX && obj->GetPositionX() > _minX) ||
	        !(obj->GetPositionY() < _maxY && obj->GetPositionY() > _minY))
	{
		if(obj->IsPlayer())
		{
			Player* plr = TO< Player* >(obj);
			if(plr->GetBindMapId() != GetMapId())
			{
				plr->SafeTeleport(plr->GetBindMapId(), 0, plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ(), 0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ(), 0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				plr->SendTeleportAckMsg(plr->GetPosition());
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0, 0, 0, 0);
		}
	}

	ARCEMU_ASSERT(obj->GetPositionY() < _maxY && obj->GetPositionY() > _minY);
	ARCEMU_ASSERT(_cells != NULL);

	///////////////////////
	// Get cell coordinates
	///////////////////////

	uint32 x = GetPosX(obj->GetPositionX());
	uint32 y = GetPosY(obj->GetPositionY());

	if(x >= _sizeX || y >= _sizeY)
	{
		if(obj->IsPlayer())
		{
			Player* plr = TO< Player* >(obj);
			if(plr->GetBindMapId() != GetMapId())
			{
				plr->SafeTeleport(plr->GetBindMapId(), 0, plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ(), 0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ(), 0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				plr->SendTeleportAckMsg(plr->GetPosition());
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0, 0, 0, 0);
		}

		x = GetPosX(obj->GetPositionX());
		y = GetPosY(obj->GetPositionY());
	}

	MapCell* objCell = GetCell(x, y);
	if(objCell == NULL)
	{
		objCell = Create(x, y);
		objCell->Init(x, y, this);
	}
	ARCEMU_ASSERT(objCell != NULL);

	uint32 endX = (x <= _sizeX) ? x + 1 : (_sizeX - 1);
	uint32 endY = (y <= _sizeY) ? y + 1 : (_sizeY - 1);
	uint32 startX = x > 0 ? x - 1 : 0;
	uint32 startY = y > 0 ? y - 1 : 0;
	uint32 posX, posY;
	MapCell* cell;
	//MapCell::ObjectSet::iterator iter;

	ByteBuffer* buf = 0;
	uint32 count;
	Player* plObj;

	if(obj->IsPlayer())
		plObj = TO< Player* >(obj);
	else
		plObj = NULL;

	if(plObj != NULL)
	{
		LOG_DETAIL("Creating player " I64FMT " for himself.", obj->GetGUID());
		ByteBuffer pbuf(10000);
		count = plObj->BuildCreateUpdateBlockForPlayer(&pbuf, plObj);
		plObj->PushCreationData(&pbuf, count);
	}

	//////////////////////
	// Build in-range data
	//////////////////////

	for(posX = startX; posX <= endX; posX++)
	{
		for(posY = startY; posY <= endY; posY++)
		{
			cell = GetCell(posX, posY);
			if(cell)
			{
				UpdateInRangeSet(obj, plObj, cell, &buf);
			}
		}
	}

	//Add to the cell's object list
	objCell->AddObject(obj);

	obj->SetMapCell(objCell);
	//Add to the mapmanager's object list
	if(plObj != NULL)
	{
		m_PlayerStorage[plObj->GetLowGUID()] = plObj;
		UpdateCellActivity(x, y, 2);
	}
	else
	{
		switch(obj->GetTypeFromGUID())
		{
			case HIGHGUID_TYPE_PET:
				m_PetStorage[obj->GetUIdFromGUID()] = TO< Pet* >(obj);
				break;

			case HIGHGUID_TYPE_UNIT:
			case HIGHGUID_TYPE_VEHICLE:
				{
					ARCEMU_ASSERT(obj->GetUIdFromGUID() <= m_CreatureHighGuid);
					CreatureStorage[ obj->GetUIdFromGUID() ] = TO< Creature* >(obj);
					if(TO_CREATURE(obj)->m_spawn != NULL)
					{
						_sqlids_creatures.insert(make_pair(TO_CREATURE(obj)->m_spawn->id, TO_CREATURE(obj)));
					}
				}
				break;

			case HIGHGUID_TYPE_GAMEOBJECT:
				{
					GOStorage[ obj->GetUIdFromGUID() ] = TO< GameObject* >(obj);
					if(TO_GAMEOBJECT(obj)->m_spawn != NULL)
					{
						_sqlids_gameobjects.insert(make_pair(TO_GAMEOBJECT(obj)->m_spawn->id, TO_GAMEOBJECT(obj)));
					}
				}
				break;

			case HIGHGUID_TYPE_DYNAMICOBJECT:
				m_DynamicObjectStorage[obj->GetLowGUID()] = (DynamicObject*)obj;
				break;
		}
	}

	// Handle activation of that object.
	if(objCell->IsActive() && obj->CanActivate())
		obj->Activate(this);

	// Add the session to our set if it is a player.
	if(plObj != NULL)
	{
		Sessions.insert(plObj->GetSession());

		// Change the instance ID, this will cause it to be removed from the world thread (return value 1)
		plObj->GetSession()->SetInstance(GetInstanceID());

		/* Add the map wide objects */
		if(_mapWideStaticObjects.size())
		{
			uint32 globalcount = 0;
			if(!buf)
				buf = new ByteBuffer(300);

			for(set<Object*>::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
			{
				count = (*itr)->BuildCreateUpdateBlockForPlayer(buf, plObj);
				globalcount += count;
			}
			//VLack: It seems if we use the same buffer then it is a BAD idea to try and push created data one by one, add them at once!
			//       If you try to add them one by one, then as the buffer already contains data, they'll end up repeating some object.
			//       Like 6 object updates for Deeprun Tram, but the built package will contain these entries: 2AFD0, 2AFD0, 2AFD1, 2AFD0, 2AFD1, 2AFD2
			if(globalcount > 0) plObj->PushCreationData(buf, globalcount);
		}
	}

	if(buf)
		delete buf;

	if(plObj != NULL && InactiveMoveTime && !forced_expire)
		InactiveMoveTime = 0;
}


void MapMgr::PushStaticObject(Object* obj)
{
	_mapWideStaticObjects.insert(obj);

	obj->SetInstanceID(GetInstanceID());
	obj->SetMapId(GetMapId());

	switch(obj->GetTypeFromGUID())
	{
		case HIGHGUID_TYPE_UNIT:
		case HIGHGUID_TYPE_VEHICLE:
			CreatureStorage[ obj->GetUIdFromGUID() ] = TO< Creature* >(obj);
			break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			GOStorage[ obj->GetUIdFromGUID() ] = TO< GameObject* >(obj);
			break;

		default:
			// maybe add a warning, shouldn't be needed
			break;
	}
}

#define OPTIONAL_IN_RANGE_SETS

void MapMgr::RemoveObject(Object* obj, bool free_guid)
{
	/////////////
	// Assertions
	/////////////

	ARCEMU_ASSERT(obj != NULL);
	ARCEMU_ASSERT(obj->GetMapId() == _mapId);
	//ARCEMU_ASSERT(   obj->GetPositionX() > _minX && obj->GetPositionX() < _maxX);
	//ARCEMU_ASSERT(   obj->GetPositionY() > _minY && obj->GetPositionY() < _maxY);
	ARCEMU_ASSERT(_cells != NULL);

	if(obj->IsActive())
		obj->Deactivate(this);

	//there is a very small chance that on double player ports on same update player is added to multiple insertpools but not removed
	//one clear example was the double port proc when exploiting double resurrect
	m_objectinsertlock.Acquire();
	m_objectinsertpool.erase(obj);
	m_objectinsertlock.Release();

	_updates.erase(obj);
	obj->ClearUpdateMask();

	///////////////////////////////////////
	// Remove object from all needed places
	///////////////////////////////////////

	switch(obj->GetTypeFromGUID())
	{
		case HIGHGUID_TYPE_UNIT:
		case HIGHGUID_TYPE_VEHICLE:
			ARCEMU_ASSERT(obj->GetUIdFromGUID() <= m_CreatureHighGuid);
			CreatureStorage[ obj->GetUIdFromGUID() ] = NULL;
			if(TO_CREATURE(obj)->m_spawn != NULL)
			{
				_sqlids_creatures.erase(TO_CREATURE(obj)->m_spawn->id);
			}

			if(free_guid)
				_reusable_guids_creature.push_back(obj->GetUIdFromGUID());

			break;

		case HIGHGUID_TYPE_PET:
			if(pet_iterator != m_PetStorage.end() && pet_iterator->second->GetGUID() == obj->GetGUID())
				++pet_iterator;
			m_PetStorage.erase(obj->GetUIdFromGUID());
			break;

		case HIGHGUID_TYPE_DYNAMICOBJECT:
			m_DynamicObjectStorage.erase(obj->GetLowGUID());
			break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			ARCEMU_ASSERT(obj->GetUIdFromGUID() <= m_GOHighGuid);
			GOStorage[ obj->GetUIdFromGUID() ] = NULL;
			if(TO_GAMEOBJECT(obj)->m_spawn != NULL)
			{
				_sqlids_gameobjects.erase(TO_GAMEOBJECT(obj)->m_spawn->id);
			}

			if(free_guid)
				_reusable_guids_gameobject.push_back(obj->GetUIdFromGUID());

			break;
	}

	// That object types are not map objects. TODO: add AI groups here?
	if(obj->IsItem() || obj->IsContainer())
	{
		return;
	}

	if(obj->IsCorpse())
	{
		m_corpses.erase(TO< Corpse* >(obj));
	}

	MapCell* cell = GetCell(obj->GetMapCellX(), obj->GetMapCellY());
	if(cell == NULL)
	{
		/* set the map cell correctly */
		if(obj->GetPositionX() >= _maxX || obj->GetPositionX() <= _minY ||
		        obj->GetPositionY() >= _maxY || obj->GetPositionY() <= _minY)
		{
			// do nothing
		}
		else
		{
			cell = this->GetCellByCoords(obj->GetPositionX(), obj->GetPositionY());
			obj->SetMapCell(cell);
		}
	}

	if(cell != NULL)
	{
		// Remove object from cell
		cell->RemoveObject(obj);

		// Unset object's cell
		obj->SetMapCell(NULL);
	}

	Player* plObj = NULL;
	// Clear any updates pending
	if(obj->IsPlayer())
	{
		plObj = TO_PLAYER(obj);
		_processQueue.erase(plObj);
		plObj->ClearAllPendingUpdates();
	}

	obj->RemoveSelfFromInrangeSets();

	// Clear object's in-range set
	obj->ClearInRangeSet();

	// If it's a player - update his nearby cells
	if(!_shutdown && obj->IsPlayer())
	{
		// get x/y
		if(obj->GetPositionX() >= _maxX || obj->GetPositionX() <= _minY ||
		        obj->GetPositionY() >= _maxY || obj->GetPositionY() <= _minY)
		{
			// do nothing
		}
		else
		{
			uint32 x = GetPosX(obj->GetPositionX());
			uint32 y = GetPosY(obj->GetPositionY());
			UpdateCellActivity(x, y, 2);
		}
		m_PlayerStorage.erase(TO< Player* >(obj)->GetLowGUID());
	}

	// Remove the session from our set if it is a player.
	if(obj->IsPlayer())
	{
		for(set<Object*>::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
		{
			plObj->PushOutOfRange((*itr)->GetNewGUID());
		}

		// Setting an instance ID here will trigger the session to be removed
		// by MapMgr::run(). :)
		plObj->GetSession()->SetInstance(0);

		// Add it to the global session set.
		// Don't "re-add" to session if it is being deleted.
		if(!plObj->GetSession()->bDeleted)
			sWorld.AddGlobalSession(plObj->GetSession());
	}

	if(!HasPlayers())
	{
		if(this->pInstance != NULL && this->pInstance->m_persistent)
			this->pInstance->m_creatorGroup = 0;
		if(!InactiveMoveTime && !forced_expire && GetMapInfo()->type != INSTANCE_NULL)
		{
			InactiveMoveTime = UNIXTIME + (MAPMGR_INACTIVE_MOVE_TIME * 60);
			Log.Debug("MapMgr", "Instance %u is now idle. (%s)", m_instanceID, GetBaseMap()->GetName());
		}
	}
}

void MapMgr::ChangeObjectLocation(Object* obj)
{
	/*
	if ( !obj ) return; // crashfix
	*/

	ARCEMU_ASSERT(obj != NULL);

	// Items and containers are of no interest for us
	if(obj->IsItem() || obj->IsContainer() || obj->GetMapMgr() != this)
	{
		return;
	}

	Player* plObj = NULL;
	ByteBuffer* buf = 0;

	if(obj->IsPlayer())
	{
		plObj = TO< Player* >(obj);
	}

	Object* curObj;
	float fRange = 0.0f;

	///////////////////////////////////////
	// Update in-range data for old objects
	///////////////////////////////////////

	if(obj->HasInRangeObjects())
	{
		for(Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin(); iter != obj->GetInRangeSetEnd();)
		{
			curObj = *iter;
			++iter;

			if(curObj->IsPlayer() && plObj != NULL && plObj->transporter_info.guid && plObj->transporter_info.guid == TO< Player* >(curObj)->transporter_info.guid)
				fRange = 0.0f; // unlimited distance for people on same boat
			else if(curObj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER)
				fRange = 0.0f; // unlimited distance for transporters (only up to 2 cells +/- anyway.)
			//If the object announcing its position is a transport, or other special object, then deleting it from visible objects should be avoided. - By: VLack
			else if(obj->IsGameObject() && (TO< GameObject* >(obj)->GetOverrides() & GAMEOBJECT_INFVIS) && obj->GetMapId() == curObj->GetMapId())
				fRange = 0.0f;
			//If the object we're checking for possible removal is a transport or other special object, and we are players on the same map, don't remove it...
			else if(plObj && curObj->IsGameObject() && (TO< GameObject* >(curObj)->GetOverrides() & GAMEOBJECT_INFVIS) && obj->GetMapId() == curObj->GetMapId())
				fRange = 0.0f;
			else if(curObj->IsPlayer() && TO< Player* >(curObj)->GetFarsightTarget() == obj->GetGUID())
				fRange = 0.0f;//Mind Vision, Eye of Kilrogg
			else
				fRange = m_UpdateDistance; // normal distance

			if(fRange > 0.0f && (curObj->GetDistance2dSq(obj) > fRange))
			{
				if(plObj != NULL)
					plObj->RemoveIfVisible(curObj->GetGUID());

				if(curObj->IsPlayer())
					TO< Player* >(curObj)->RemoveIfVisible(obj->GetGUID());

				curObj->RemoveInRangeObject(obj);

				if(obj->GetMapMgr() != this)
				{
					/* Something removed us. */
					return;
				}
				obj->RemoveInRangeObject(curObj);
			}
		}
	}

	///////////////////////////
	// Get new cell coordinates
	///////////////////////////
	if(obj->GetMapMgr() != this)
	{
		/* Something removed us. */
		return;
	}

	if(obj->GetPositionX() >= _maxX || obj->GetPositionX() <= _minX ||
	        obj->GetPositionY() >= _maxY || obj->GetPositionY() <= _minY)
	{
		if(plObj != NULL)
		{
			if(plObj->GetBindMapId() != GetMapId())
			{
				plObj->SafeTeleport(plObj->GetBindMapId(), 0, plObj->GetBindPositionX(), plObj->GetBindPositionY(), plObj->GetBindPositionZ(), 0);
				plObj->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plObj->GetBindPositionX(), plObj->GetBindPositionY(), plObj->GetBindPositionZ(), 0);
				plObj->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				plObj->SendTeleportAckMsg(plObj->GetPosition());
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0, 0, 0, 0);
		}
	}

	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());

	if(cellX >= _sizeX || cellY >= _sizeY)
	{
		return;
	}

	MapCell* objCell = GetCell(cellX, cellY);
	MapCell* pOldCell = obj->GetMapCell();
	if(objCell == NULL)
	{
		objCell = Create(cellX, cellY);
		objCell->Init(cellX, cellY, this);
	}

	ARCEMU_ASSERT(objCell != NULL);

	// If object moved cell
	if(objCell != pOldCell)
	{
		// THIS IS A HACK!
		// Current code, if a creature on a long waypoint path moves from an active
		// cell into an inactive one, it will disable itself and will never return.
		// This is to prevent cpu leaks. I will think of a better solution very soon :P

		if(!objCell->IsActive() && !plObj && obj->IsActive())
			obj->Deactivate(this);

		if(pOldCell != NULL)
			pOldCell->RemoveObject(obj);

		objCell->AddObject(obj);
		obj->SetMapCell(objCell);

		// if player we need to update cell activity
		// radius = 2 is used in order to update both
		// old and new cells
		if(obj->IsPlayer())
		{
			// have to unlock/lock here to avoid a deadlock situation.
			UpdateCellActivity(cellX, cellY, 2);
			if(pOldCell != NULL)
			{
				// only do the second check if there's -/+ 2 difference
				if(abs((int)cellX - (int)pOldCell->_x) > 2 ||
				        abs((int)cellY - (int)pOldCell->_y) > 2)
				{
					UpdateCellActivity(pOldCell->_x, pOldCell->_y, 2);
				}
			}
		}
	}


	//////////////////////////////////////
	// Update in-range set for new objects
	//////////////////////////////////////

	uint32 endX = cellX <= _sizeX ? cellX + 1 : (_sizeX - 1);
	uint32 endY = cellY <= _sizeY ? cellY + 1 : (_sizeY - 1);
	uint32 startX = cellX > 0 ? cellX - 1 : 0;
	uint32 startY = cellY > 0 ? cellY - 1 : 0;
	uint32 posX, posY;
	MapCell* cell;

	//If the object announcing it's position is a special one, then it should do so in a much wider area - like the distance between the two transport towers in Orgrimmar, or more. - By: VLack
	if(obj->IsGameObject() && (TO< GameObject* >(obj)->GetOverrides() & GAMEOBJECT_ONMOVEWIDE))
	{
		endX = cellX + 5 <= _sizeX ? cellX + 6 : (_sizeX - 1);
		endY = cellY + 5 <= _sizeY ? cellY + 6 : (_sizeY - 1);
		startX = cellX > 5 ? cellX - 6 : 0;
		startY = cellY > 5 ? cellY - 6 : 0;
	}

	for(posX = startX; posX <= endX; ++posX)
	{
		for(posY = startY; posY <= endY; ++posY)
		{
			cell = GetCell(posX, posY);
			if(cell)
				UpdateInRangeSet(obj, plObj, cell, &buf);
		}
	}

	if(buf)
		delete buf;
}

void MapMgr::UpdateInRangeSet(Object* obj, Player* plObj, MapCell* cell, ByteBuffer** buf)
{
#define CHECK_BUF if(!*buf) *buf = new ByteBuffer(2500)

	if(cell == NULL)
		return;

	Object* curObj;
	Player* plObj2;
	int count;
	float fRange;
	bool cansee, isvisible;

	ObjectSet::iterator iter = cell->Begin();
	while(iter != cell->End())
	{
		curObj = *iter;
		++iter;

		if(curObj == NULL)
			continue;

		if(curObj->IsPlayer() && obj->IsPlayer() && plObj != NULL && plObj->transporter_info.guid && plObj->transporter_info.guid == TO< Player* >(curObj)->transporter_info.guid)
			fRange = 0.0f; // unlimited distance for people on same boat
		else if(curObj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER)
			fRange = 0.0f; // unlimited distance for transporters (only up to 2 cells +/- anyway.)

		//If the object announcing its position is a transport, or other special object, then deleting it from visible objects should be avoided. - By: VLack
		else if(obj->IsGameObject() && (TO< GameObject* >(obj)->GetOverrides() & GAMEOBJECT_INFVIS) && obj->GetMapId() == curObj->GetMapId())
			fRange = 0.0f;
		//If the object we're checking for possible removal is a transport or other special object, and we are players on the same map, don't remove it, and add it whenever possible...
		else if(plObj && curObj->IsGameObject() && (TO< GameObject* >(curObj)->GetOverrides() & GAMEOBJECT_INFVIS) && obj->GetMapId() == curObj->GetMapId())
			fRange = 0.0f;
		else
			fRange = m_UpdateDistance; // normal distance

		if(curObj != obj && (curObj->GetDistance2dSq(obj) <= fRange || fRange == 0.0f))
		{
			if(!obj->IsInRangeSet(curObj))
			{
				// Object in range, add to set
				obj->AddInRangeObject(curObj);
				curObj->AddInRangeObject(obj);

				if(curObj->IsPlayer())
				{
					plObj2 = TO< Player* >(curObj);

					if(plObj2->CanSee(obj) && !plObj2->IsVisible(obj->GetGUID()))
					{
						CHECK_BUF;
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2);
						plObj2->PushCreationData(*buf, count);
						plObj2->AddVisibleObject(obj->GetGUID());
						(*buf)->clear();
					}
				}

				if(plObj != NULL)
				{
					if(plObj->CanSee(curObj) && !plObj->IsVisible(curObj->GetGUID()))
					{
						CHECK_BUF;
						count = curObj->BuildCreateUpdateBlockForPlayer(*buf, plObj);
						plObj->PushCreationData(*buf, count);
						plObj->AddVisibleObject(curObj->GetGUID());
						(*buf)->clear();
					}
				}
			}
			else
			{
				// Check visibility
				if(curObj->IsPlayer())
				{
					plObj2 = TO< Player* >(curObj);
					cansee = plObj2->CanSee(obj);
					isvisible = plObj2->IsVisible(obj->GetGUID());
					if(!cansee && isvisible)
					{
						plObj2->PushOutOfRange(obj->GetNewGUID());
						plObj2->RemoveVisibleObject(obj->GetGUID());
					}
					else if(cansee && !isvisible)
					{
						CHECK_BUF;
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2);
						plObj2->PushCreationData(*buf, count);
						plObj2->AddVisibleObject(obj->GetGUID());
						(*buf)->clear();
					}
				}

				if(plObj != NULL)
				{
					cansee = plObj->CanSee(curObj);
					isvisible = plObj->IsVisible(curObj->GetGUID());
					if(!cansee && isvisible)
					{
						plObj->PushOutOfRange(curObj->GetNewGUID());
						plObj->RemoveVisibleObject(curObj->GetGUID());
					}
					else if(cansee && !isvisible)
					{
						CHECK_BUF;
						count = curObj->BuildCreateUpdateBlockForPlayer(*buf, plObj);
						plObj->PushCreationData(*buf, count);
						plObj->AddVisibleObject(curObj->GetGUID());
						(*buf)->clear();
					}
				}
			}
		}
	}
}

void MapMgr::_UpdateObjects()
{
	if(!_updates.size() && !_processQueue.size())
		return;

	Object* pObj;
	Player* pOwner;
	std::set< Object* >::iterator it_start, it_end, itr;
	Player* lplr;
	ByteBuffer update(2500);
	uint32 count = 0;

	m_updateMutex.Acquire();
	UpdateQueue::iterator iter = _updates.begin();
	PUpdateQueue::iterator it, eit;

	for(; iter != _updates.end();)
	{
		pObj = *iter;
		++iter;
		if(!pObj) continue;

		if(pObj->IsItem() || pObj->IsContainer())
		{
			// our update is only sent to the owner here.
			pOwner = TO< Item* >(pObj)->GetOwner();
			if(pOwner != NULL)
			{
				count = TO< Item* >(pObj)->BuildValuesUpdateBlockForPlayer(&update, pOwner);
				// send update to owner
				if(count)
				{
					pOwner->PushUpdateData(&update, count);
					update.clear();
				}
			}
		}
		else
		{
			if(pObj->IsInWorld())
			{
				// players have to receive their own updates ;)
				if(pObj->IsPlayer())
				{
					// need to be different! ;)
					count = pObj->BuildValuesUpdateBlockForPlayer(&update, TO< Player* >(pObj));
					if(count)
					{
						TO< Player* >(pObj)->PushUpdateData(&update, count);
						update.clear();
					}
				}

				if(pObj->IsUnit() && pObj->HasUpdateField(UNIT_FIELD_HEALTH))
					TO< Unit* >(pObj)->EventHealthChangeSinceLastUpdate();

				// build the update
				count = pObj->BuildValuesUpdateBlockForPlayer(&update, static_cast< Player* >(NULL));

				if(count)
				{
					it_start = pObj->GetInRangePlayerSetBegin();
					it_end = pObj->GetInRangePlayerSetEnd();

					for(itr = it_start; itr != it_end;)
					{
						lplr = TO< Player* >(*itr);
						++itr;
						// Make sure that the target player can see us.
						if(lplr->IsVisible(pObj->GetGUID()))
							lplr->PushUpdateData(&update, count);
					}
					update.clear();
				}
			}
		}
		pObj->ClearUpdateMask();
	}
	_updates.clear();
	m_updateMutex.Release();

	// generate pending a9packets and send to clients.
	Player* plyr;
	for(it = _processQueue.begin(); it != _processQueue.end();)
	{
		plyr = *it;
		eit = it;
		++it;
		_processQueue.erase(eit);
		if(plyr->GetMapMgr() == this)
			plyr->ProcessPendingUpdates();
	}
}
void MapMgr::LoadAllCells()
{
	// eek
	MapCell* cellInfo;
	CellSpawns* spawns;

	for(uint32 x = 0 ; x < _sizeX ; x ++)
	{
		for(uint32 y = 0 ; y < _sizeY ; y ++)
		{
			cellInfo = GetCell(x , y);

			if(!cellInfo)
			{
				// Cell doesn't exist, create it.
				// There is no spoon. Err... cell.
				cellInfo = Create(x , y);
				cellInfo->Init(x , y , this);
				LOG_DETAIL("Created cell [%u,%u] on map %u (instance %u)." , x , y , _mapId , m_instanceID);
				cellInfo->SetActivity(true);
				_map->CellGoneActive(x , y);
				ARCEMU_ASSERT(!cellInfo->IsLoaded());

				spawns = _map->GetSpawnsList(x , y);
				if(spawns)
					cellInfo->LoadObjects(spawns);
			}
			else
			{
				// Cell exists, but is inactive
				if(!cellInfo->IsActive())
				{
					LOG_DETAIL("Activated cell [%u,%u] on map %u (instance %u).", x, y, _mapId, m_instanceID);
					_map->CellGoneActive(x , y);
					cellInfo->SetActivity(true);

					if(!cellInfo->IsLoaded())
					{
						//LOG_DETAIL("Loading objects for Cell [%u][%u] on map %u (instance %u)...",
						//	posX, posY, this->_mapId, m_instanceID);
						spawns = _map->GetSpawnsList(x , y);
						if(spawns)
							cellInfo->LoadObjects(spawns);
					}
				}
			}
		}
	}
}

void MapMgr::UpdateCellActivity(uint32 x, uint32 y, uint32 radius)
{
	CellSpawns* sp;
	uint32 endX = (x + radius) <= _sizeX ? x + radius : (_sizeX - 1);
	uint32 endY = (y + radius) <= _sizeY ? y + radius : (_sizeY - 1);
	uint32 startX = x > radius ? x - radius : 0;
	uint32 startY = y > radius ? y - radius : 0;
	uint32 posX, posY;

	MapCell* objCell;

	for(posX = startX; posX <= endX; posX++)
	{
		for(posY = startY; posY <= endY; posY++)
		{
			objCell = GetCell(posX, posY);

			if(!objCell)
			{
				if(_CellActive(posX, posY))
				{
					objCell = Create(posX, posY);
					objCell->Init(posX, posY, this);

					LOG_DETAIL("Cell [%u,%u] on map %u (instance %u) is now active.",
					           posX, posY, this->_mapId, m_instanceID);
					objCell->SetActivity(true);
					_map->CellGoneActive(posX, posY);
					_terrain->LoadTile((int32)posX / 8, (int32)posY / 8);

					ARCEMU_ASSERT(!objCell->IsLoaded());

					LOG_DETAIL("Loading objects for Cell [%u][%u] on map %u (instance %u)...",
					           posX, posY, this->_mapId, m_instanceID);

					sp = _map->GetSpawnsList(posX, posY);
					if(sp) objCell->LoadObjects(sp);
				}
			}
			else
			{
				//Cell is now active
				if(_CellActive(posX, posY) && !objCell->IsActive())
				{
					LOG_DETAIL("Cell [%u,%u] on map %u (instance %u) is now active.",
					           posX, posY, this->_mapId, m_instanceID);
					_map->CellGoneActive(posX, posY);
					_terrain->LoadTile((int32)posX / 8, (int32)posY / 8);
					objCell->SetActivity(true);

					if(!objCell->IsLoaded())
					{
						LOG_DETAIL("Loading objects for Cell [%u][%u] on map %u (instance %u)...",
						           posX, posY, this->_mapId, m_instanceID);
						sp = _map->GetSpawnsList(posX, posY);
						if(sp) objCell->LoadObjects(sp);
					}
				}
				//Cell is no longer active
				else if(!_CellActive(posX, posY) && objCell->IsActive())
				{
					LOG_DETAIL("Cell [%u,%u] on map %u (instance %u) is now idle.", posX, posY, _mapId, m_instanceID);
					_map->CellGoneIdle(posX, posY);
					objCell->SetActivity(false);
					_terrain->UnloadTile((int32)posX / 8, (int32)posY / 8);
				}
			}
		}
	}

}

bool MapMgr::_CellActive(uint32 x, uint32 y)
{
	uint32 endX = ((x + 1) <= _sizeX) ? x + 1 : (_sizeX - 1);
	uint32 endY = ((y + 1) <= _sizeY) ? y + 1 : (_sizeY - 1);
	uint32 startX = x > 0 ? x - 1 : 0;
	uint32 startY = y > 0 ? y - 1 : 0;
	uint32 posX, posY;

	MapCell* objCell;

	for(posX = startX; posX <= endX; posX++)
	{
		for(posY = startY; posY <= endY; posY++)
		{
			objCell = GetCell(posX, posY);

			if(objCell)
			{
				if(objCell->HasPlayers() || m_forcedcells.find(objCell) != m_forcedcells.end())
				{
					return true;
				}
			}
		}
	}

	return false;
}

void MapMgr::ObjectUpdated(Object* obj)
{
	// set our fields to dirty
	// stupid fucked up code in places.. i hate doing this but i've got to :<
	// - burlex
	m_updateMutex.Acquire();
	_updates.insert(obj);
	m_updateMutex.Release();
}

void MapMgr::PushToProcessed(Player* plr)
{
	_processQueue.insert(plr);
}


void MapMgr::ChangeFarsightLocation(Player* plr, DynamicObject* farsight)
{
	if(farsight == 0)
	{
		// We're clearing.
		for(ObjectSet::iterator itr = plr->m_visibleFarsightObjects.begin(); itr != plr->m_visibleFarsightObjects.end();
		        ++itr)
		{
			if(plr->IsVisible((*itr)->GetGUID()) && !plr->CanSee((*itr)))
			{
				// Send destroy
				plr->PushOutOfRange((*itr)->GetNewGUID());
			}
		}
		plr->m_visibleFarsightObjects.clear();
	}
	else
	{
		uint32 cellX = GetPosX(farsight->GetPositionX());
		uint32 cellY = GetPosY(farsight->GetPositionY());
		uint32 endX = (cellX <= _sizeX) ? cellX + 1 : (_sizeX - 1);
		uint32 endY = (cellY <= _sizeY) ? cellY + 1 : (_sizeY - 1);
		uint32 startX = cellX > 0 ? cellX - 1 : 0;
		uint32 startY = cellY > 0 ? cellY - 1 : 0;
		uint32 posX, posY;
		MapCell* cell;
		Object* obj;
		MapCell::ObjectSet::iterator iter, iend;
		uint32 count;
		for(posX = startX; posX <= endX; ++posX)
		{
			for(posY = startY; posY <= endY; ++posY)
			{
				cell = GetCell(posX, posY);
				if(cell)
				{
					iter = cell->Begin();
					iend = cell->End();
					for(; iter != iend; ++iter)
					{
						obj = (*iter);
						if(!plr->IsVisible(obj->GetGUID()) && plr->CanSee(obj) && farsight->GetDistance2dSq(obj) <= m_UpdateDistance)
						{
							ByteBuffer buf;
							count = obj->BuildCreateUpdateBlockForPlayer(&buf, plr);
							plr->PushCreationData(&buf, count);
							plr->m_visibleFarsightObjects.insert(obj);
						}
					}
				}
			}
		}
	}
}

bool MapMgr::run()
{
	bool rv = true;

	THREAD_TRY_EXECUTION
	rv = Do();
	THREAD_HANDLE_CRASH

	return rv;
}

bool MapMgr::Do()
{
#ifdef WIN32
	threadid = GetCurrentThreadId();
#endif

	t_currentMapContext.set(this);

	thread_running = true;
	ThreadState.SetVal(THREADSTATE_BUSY);
	SetThreadName("Map mgr - M%u|I%u", this->_mapId , this->m_instanceID);
	ObjectSet::iterator i;
	uint32 last_exec = getMSTime();

	// Create Instance script
	LoadInstanceScript();

	/* create static objects */
	for(GOSpawnList::iterator itr = _map->staticSpawns.GOSpawns.begin(); itr != _map->staticSpawns.GOSpawns.end(); ++itr)
	{
		GameObject* obj = CreateGameObject((*itr)->entry);
		obj->Load((*itr));
		PushStaticObject(obj);
	}

	// Call script OnLoad virtual procedure
	CALL_INSTANCE_SCRIPT_EVENT(this, OnLoad)();

	for(CreatureSpawnList::iterator itr = _map->staticSpawns.CreatureSpawns.begin(); itr != _map->staticSpawns.CreatureSpawns.end(); ++itr)
	{
		Creature* obj = CreateCreature((*itr)->entry);
		obj->Load(*itr, 0, pMapInfo);
		PushStaticObject(obj);
	}

	/* load corpses */
	objmgr.LoadCorpses(this);
	worldstateshandler.InitWorldStates( objmgr.GetWorldStatesForMap( _mapId ) );
	worldstateshandler.setObserver( this );

	// always declare local variables outside of the loop!
	// otherwise there's a lot of sub esp; going on.

	uint32 exec_time, exec_start;

	while((GetThreadState() != THREADSTATE_TERMINATE) && !_shutdown)
	{
		exec_start = getMSTime();

///////////////////////////////////////////// first push to world new objects ////////////////////////////////////////////

		m_objectinsertlock.Acquire();

		if(m_objectinsertpool.size())
		{
			for(i = m_objectinsertpool.begin(); i != m_objectinsertpool.end(); ++i)
			{
				Object* o = *i;

				o->PushToWorld(this);
			}

			m_objectinsertpool.clear();
		}

		m_objectinsertlock.Release();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Now update sessions of this map + objects
		_PerformObjectDuties();

		last_exec = getMSTime();
		exec_time = last_exec - exec_start;
		if(exec_time < MAP_MGR_UPDATE_PERIOD)
		{

			Arcemu::Sleep(MAP_MGR_UPDATE_PERIOD - exec_time);

		}

		//////////////////////////////////////////////////////////////////////////
		// Check if we have to die :P
		//////////////////////////////////////////////////////////////////////////
		if(InactiveMoveTime && UNIXTIME >= InactiveMoveTime)
			break;
	}

	// Teleport any left-over players out.
	TeleportPlayers();

	// Clear the instance's reference to us.
	if(m_battleground)
	{
		BattlegroundManager.DeleteBattleground(m_battleground);
		sInstanceMgr.DeleteBattlegroundInstance(GetMapId(), GetInstanceID());
	}

	if(pInstance)
	{
		// check for a non-raid instance, these expire after 10 minutes.
		if(GetMapInfo()->type == INSTANCE_NONRAID || pInstance->m_isBattleground)
		{
			pInstance->m_mapMgr = NULL;
			sInstanceMgr._DeleteInstance(pInstance, true);
			pInstance = NULL;
		}
		else
		{
			// just null out the pointer
			pInstance->m_mapMgr = NULL;
		}
	}
	else if(GetMapInfo()->type == INSTANCE_NULL)
		sInstanceMgr.m_singleMaps[GetMapId()] = NULL;

	thread_running = false;
	if(thread_kill_only)
		return false;

	// delete ourselves
	delete this;

	// already deleted, so the threadpool doesn't have to.
	return false;
}

void MapMgr::BeginInstanceExpireCountdown()
{
	WorldPacket data(SMSG_RAID_GROUP_ONLY, 8);
	PlayerStorageMap::iterator itr;

	// so players getting removed don't overwrite us
	forced_expire = true;

	// send our sexy packet
	data << uint32(60000) << uint32(1);
	for(itr = m_PlayerStorage.begin(); itr != m_PlayerStorage.end(); ++itr)
	{
		if(!itr->second->raidgrouponlysent)
			itr->second->GetSession()->SendPacket(&data);
	}

	// set our expire time to 60 seconds.
	InactiveMoveTime = UNIXTIME + 60;
}

void MapMgr::AddObject(Object* obj)
{
	m_objectinsertlock.Acquire();//<<<<<<<<<<<<
	m_objectinsertpool.insert(obj);
	m_objectinsertlock.Release();//>>>>>>>>>>>>
}


Unit* MapMgr::GetUnit(const uint64 & guid)
{
	if(guid == 0)
		return NULL;

	switch(GET_TYPE_FROM_GUID(guid))
	{
		case HIGHGUID_TYPE_UNIT:
		case HIGHGUID_TYPE_VEHICLE:
			return GetCreature(GET_LOWGUID_PART(guid));
			break;

		case HIGHGUID_TYPE_PLAYER:
			return GetPlayer(Arcemu::Util::GUID_LOPART(guid));
			break;

		case HIGHGUID_TYPE_PET:
			return GetPet(GET_LOWGUID_PART(guid));
			break;
	}

	return NULL;
}

Object* MapMgr::_GetObject(const uint64 & guid)
{
	if(!guid)
		return NULL;

	switch(GET_TYPE_FROM_GUID(guid))
	{
		case	HIGHGUID_TYPE_GAMEOBJECT:
			return GetGameObject(GET_LOWGUID_PART(guid));
			break;
		case HIGHGUID_TYPE_UNIT:
		case HIGHGUID_TYPE_VEHICLE:
			return GetCreature(GET_LOWGUID_PART(guid));
			break;
		case	HIGHGUID_TYPE_DYNAMICOBJECT:
			return GetDynamicObject((uint32)guid);
			break;
		case	HIGHGUID_TYPE_TRANSPORTER:
			return objmgr.GetTransporter(Arcemu::Util::GUID_LOPART(guid));
			break;
		default:
			return GetUnit(guid);
			break;
	}
}

void MapMgr::_PerformObjectDuties()
{
	++mLoopCounter;
	uint32 mstime = getMSTime();
	uint32 difftime = mstime - lastUnitUpdate;
	if(difftime > 500)
		difftime = 500;

	// Update any events.
	// we make update of events before objects so in case there are 0 timediff events they do not get deleted after update but on next server update loop
	eventHolder.Update(difftime);

	// Update creatures.
	{
		creature_iterator = activeCreatures.begin();
		Creature* ptr;
		Pet* ptr2;

		for(; creature_iterator != activeCreatures.end();)
		{
			ptr = *creature_iterator;
			++creature_iterator;
			ptr->Update(difftime);
		}

		pet_iterator = m_PetStorage.begin();
		for(; pet_iterator != m_PetStorage.end();)
		{
			ptr2 = pet_iterator->second;
			++pet_iterator;
			ptr2->Update(difftime);
		}
	}

	// Update players.
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.begin();
		Player* ptr;
		for(; itr != m_PlayerStorage.end();)
		{
			ptr = itr->second;
			++itr;
			ptr->Update(difftime);
		}

		lastUnitUpdate = mstime;
	}

	// Dynamic objects
	//
	// We take the pointer, increment, and update in this order because during the update the DynamicObject might get deleted,
	// rendering the iterator unincrementable. Which causes a crash!
	{
		for(DynamicObjectStorageMap::iterator itr = m_DynamicObjectStorage.begin(); itr != m_DynamicObjectStorage.end();)
		{

			DynamicObject* o = itr->second;
			++itr;

			o->UpdateTargets();
		}
	}

	// Update gameobjects (not on every loop, however)
	if(mLoopCounter % 2)
	{
		difftime = mstime - lastGameobjectUpdate;

		GameObjectSet::iterator itr = activeGameObjects.begin();
		GameObject* ptr;
		for(; itr != activeGameObjects.end();)
		{
			ptr = *itr;
			++itr;
			if(ptr != NULL)
				ptr->Update(difftime);
		}

		lastGameobjectUpdate = mstime;
	}

	// Sessions are updated every loop.
	{
		int result;
		WorldSession* session;
		SessionSet::iterator itr = Sessions.begin();
		SessionSet::iterator it2;

		for(; itr != Sessions.end();)
		{
			session = (*itr);
			it2 = itr;
			++itr;

			if(session->GetInstance() != m_instanceID)
			{
				Sessions.erase(it2);
				continue;
			}

			// Don't update players not on our map.
			// If we abort in the handler, it means we will "lose" packets, or not process this.
			// .. and that could be disastrous to our client :P
			if(session->GetPlayer() && (session->GetPlayer()->GetMapMgr() != this && session->GetPlayer()->GetMapMgr() != 0))
			{
				continue;
			}

			if((result = session->Update(m_instanceID)) != 0)
			{
				if(result == 1)
				{
					// complete deletion
					sWorld.DeleteSession(session);
				}
				Sessions.erase(it2);
			}
		}
	}

	// Finally, A9 Building/Distribution
	_UpdateObjects();
}

void MapMgr::EventCorpseDespawn(uint64 guid)
{
	Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
	if(pCorpse == NULL)	// Already Deleted
		return;

	if(pCorpse->GetMapMgr() != this)
		return;

	pCorpse->Despawn();
	delete pCorpse;
}

void MapMgr::TeleportPlayers()
{
	PlayerStorageMap::iterator itr =  m_PlayerStorage.begin();
	if(!bServerShutdown)
	{
		for(; itr !=  m_PlayerStorage.end();)
		{
			Player* p = itr->second;
			++itr;
			p->EjectFromInstance();
		}
	}
	else
	{
		for(; itr !=  m_PlayerStorage.end();)
		{
			Player* p = itr->second;
			++itr;
			if(p->GetSession())
				p->GetSession()->LogoutPlayer(false);
			else
				delete p;
		}
	}
}

void MapMgr::UnloadCell(uint32 x, uint32 y)
{
	MapCell* c = GetCell(x, y);
	if(c == NULL || c->HasPlayers() || _CellActive(x, y) || !c->IsUnloadPending()) return;

	LOG_DETAIL("Unloading Cell [%u][%u] on map %u (instance %u)...",
	           x, y, _mapId, m_instanceID);

	c->Unload();
}

void MapMgr::EventRespawnCreature(Creature* c, uint16 x, uint16 y)
{
	MapCell* cell = GetCell(x, y);
	//cell got deleted while waiting for respawn.
	if(cell == NULL)
		return;

	ObjectSet::iterator itr = cell->_respawnObjects.find(c);
	if(itr != cell->_respawnObjects.end())
	{
		c->m_respawnCell = NULL;
		cell->_respawnObjects.erase(itr);
		c->OnRespawn(this);
	}
}

void MapMgr::EventRespawnGameObject(GameObject* o, uint16 x, uint16 y)
{
	MapCell* cell = GetCell(x, y);
	//cell got deleted while waiting for respawn.
	if(cell == NULL)
		return;

	ObjectSet::iterator itr = cell->_respawnObjects.find(o);
	if(itr != cell->_respawnObjects.end())
	{
		o->m_respawnCell = NULL;
		cell->_respawnObjects.erase(itr);
		o->Spawn(this);
	}
}

void MapMgr::SendChatMessageToCellPlayers(Object* obj, WorldPacket* packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession* originator)
{
	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());
	uint32 endX = ((cellX + cell_radius) <= _sizeX) ? cellX + cell_radius : (_sizeX - 1);
	uint32 endY = ((cellY + cell_radius) <= _sizeY) ? cellY + cell_radius : (_sizeY - 1);
	uint32 startX = cellX > cell_radius ? cellX - cell_radius : 0;
	uint32 startY = cellY > cell_radius ? cellY - cell_radius : 0;

	uint32 posX, posY;
	MapCell* cell;
	MapCell::ObjectSet::iterator iter, iend;
	for(posX = startX; posX <= endX; ++posX)
	{
		for(posY = startY; posY <= endY; ++posY)
		{
			cell = GetCell(posX, posY);
			if(cell && cell->HasPlayers())
			{
				iter = cell->Begin();
				iend = cell->End();
				for(; iter != iend; ++iter)
				{
					if((*iter)->IsPlayer())
					{
						//TO< Player* >(*iter)->GetSession()->SendPacket(packet);
						if(TO< Player* >(*iter)->GetPhase() & obj->GetPhase())
							TO< Player* >(*iter)->GetSession()->SendChatPacket(packet, langpos, lang, originator);
					}
				}
			}
		}
	}
}

Creature* MapMgr::GetSqlIdCreature(uint32 sqlid)
{
	CreatureSqlIdMap::iterator itr = _sqlids_creatures.find(sqlid);
	return (itr == _sqlids_creatures.end()) ? NULL : itr->second;
}

GameObject* MapMgr::GetSqlIdGameObject(uint32 sqlid)
{
	GameObjectSqlIdMap::iterator itr = _sqlids_gameobjects.find(sqlid);
	return (itr == _sqlids_gameobjects.end()) ? NULL : itr->second;
}

uint64 MapMgr::GenerateCreatureGUID(uint32 entry)
{
	uint64 newguid = 0;

	CreatureProto *proto = CreatureProtoStorage.LookupEntry( entry );
	if( ( proto == NULL ) || ( proto->vehicleid == 0 ) )
		newguid = static_cast< uint64 >( HIGHGUID_TYPE_UNIT ) << 32;
	else
		newguid = static_cast< uint64 >( HIGHGUID_TYPE_VEHICLE ) << 32;

	char* pHighGuid = reinterpret_cast< char* >(&newguid);
	char* pEntry = reinterpret_cast< char* >(&entry);

	pHighGuid[ 3 ] |= pEntry[ 0 ];
	pHighGuid[ 4 ] |= pEntry[ 1 ];
	pHighGuid[ 5 ] |= pEntry[ 2 ];
	pHighGuid[ 6 ] |= pEntry[ 3 ];

	uint32 guid = 0;

	if(_reusable_guids_creature.size() > 0)
	{
		guid = _reusable_guids_creature.front();
		_reusable_guids_creature.pop_front();

	}
	else
	{
		m_CreatureHighGuid++;

		if(m_CreatureHighGuid >= CreatureStorage.size())
		{
			// Reallocate array with larger size.
			size_t newsize = CreatureStorage.size() + RESERVE_EXPAND_SIZE;
			CreatureStorage.resize(newsize, NULL);
		}
		guid = m_CreatureHighGuid;
	}

	newguid |= guid;

	return newguid;
}

Creature* MapMgr::CreateCreature(uint32 entry)
{
	uint64 guid = GenerateCreatureGUID(entry);

	return new Creature(guid);
}


Summon* MapMgr::CreateSummon(uint32 entry, SummonType type)
{
	uint64 guid = GenerateCreatureGUID(entry);

	switch(type)
	{
		case SUMMONTYPE_GUARDIAN:
			return new GuardianSummon(guid);
			break;

		case SUMMONTYPE_WILD:
			return new WildSummon(guid);
			break;

		case SUMMONTYPE_TOTEM:
			return new TotemSummon(guid);
			break;

		case SUMMONTYPE_COMPANION:
			return new CompanionSummon(guid);
			break;

		case SUMMONTYPE_POSSESSED:
			return new PossessedSummon(guid);
			break;
	}

	return new Summon(guid);
}


// Spawns the object too, without which you can not interact with the object
GameObject* MapMgr::CreateAndSpawnGameObject(uint32 entryID, float x, float y, float z, float o, float scale)
{
	GameObjectInfo* goi = GameObjectNameStorage.LookupEntry(entryID);
	if(!goi)
	{
		LOG_DEBUG("Error looking up entry in CreateAndSpawnGameObject");
		return NULL;
	}

	LOG_DEBUG("CreateAndSpawnGameObject: By Entry '%u'", entryID);

	GameObject* go = CreateGameObject(entryID);

	//Player *chr = m_session->GetPlayer();
	uint32 mapid = GetMapId();
	// Setup game object
	go->CreateFromProto(entryID, mapid, x, y, z, o);
	go->SetScale(scale);
	go->InitAI();
	go->PushToWorld(this);

	// Create spawn instance
	GOSpawn* gs = new GOSpawn;
	gs->entry = go->GetEntry();
	gs->facing = go->GetOrientation();
	gs->faction = go->GetFaction();
	gs->flags = go->GetUInt32Value(GAMEOBJECT_FLAGS);
	gs->id = objmgr.GenerateGameObjectSpawnID();
	gs->o = 0.0f;
	gs->o1 = go->GetParentRotation(0);
	gs->o2 = go->GetParentRotation(2);
	gs->o3 = go->GetParentRotation(3);
	gs->scale = go->GetScale();
	gs->x = go->GetPositionX();
	gs->y = go->GetPositionY();
	gs->z = go->GetPositionZ();
	gs->state = go->GetByte(GAMEOBJECT_BYTES_1, 0);
	//gs->stateNpcLink = 0;
	gs->overrides = go->GetOverrides();

	uint32 cx = GetPosX(x);
	uint32 cy = GetPosY(y);

	GetBaseMap()->GetSpawnsListAndCreate(cx, cy)->GOSpawns.push_back(gs);
	go->m_spawn = gs;

	MapCell* mCell = GetCell(cx, cy);

	if(mCell != NULL)
		mCell->SetLoaded();

	return go;
}

GameObject* MapMgr::CreateGameObject(uint32 entry)
{
	if(_reusable_guids_gameobject.size() > GO_GUID_RECYCLE_INTERVAL)
	{
		uint32 guid = _reusable_guids_gameobject.front();
		_reusable_guids_gameobject.pop_front();
		return new GameObject((uint64)HIGHGUID_TYPE_GAMEOBJECT << 32 | guid);
	}

	if(++m_GOHighGuid  >= GOStorage.size())
	{
		// Reallocate array with larger size.
		size_t newsize = GOStorage.size() + RESERVE_EXPAND_SIZE;
		GOStorage.resize(newsize, NULL);
	}
	return new GameObject((uint64)HIGHGUID_TYPE_GAMEOBJECT << 32 | m_GOHighGuid);
}

DynamicObject* MapMgr::CreateDynamicObject()
{
	return new DynamicObject(HIGHGUID_TYPE_DYNAMICOBJECT, (++m_DynamicObjectHighGuid));
}

void MapMgr::AddForcedCell(MapCell* c)
{
	m_forcedcells.insert(c);
	UpdateCellActivity(c->GetPositionX(), c->GetPositionY(), 1);
}
void MapMgr::RemoveForcedCell(MapCell* c)
{
	m_forcedcells.erase(c);
	UpdateCellActivity(c->GetPositionX(), c->GetPositionY(), 1);
}

float MapMgr::GetFirstZWithCPZ(float x, float y, float z)
{
	if(!sWorld.Collision)
		return NO_WMO_HEIGHT;

	float posZ = NO_WMO_HEIGHT;
	for(int i = Z_SEARCH_RANGE; i >= -Z_SEARCH_RANGE; i--)
	{
		//if ( i== 0 && !IsUnderground(x,y,z) ) return GetBaseMap()->GetLandHeight(x, y);
		posZ = CollideInterface.GetHeight(GetMapId(), x, y, z + (float)i);
		if(posZ != NO_WMO_HEIGHT)
			break;
	}
	return posZ;
}

void MapMgr::SendPvPCaptureMessage(int32 ZoneMask, uint32 ZoneId, const char* Message, ...)
{
	va_list ap;
	va_start(ap, Message);

	WorldPacket data(SMSG_DEFENSE_MESSAGE, 208);
	char msgbuf[200];
	vsnprintf(msgbuf, 200, Message, ap);
	va_end(ap);

	data << ZoneId;
	data << uint32(strlen(msgbuf) + 1);
	data << msgbuf;

	PlayerStorageMap::iterator itr = m_PlayerStorage.begin();
	for(; itr !=  m_PlayerStorage.end();)
	{
		Player* plr = itr->second;
		++itr;

		if((ZoneMask != ZONE_MASK_ALL && plr->GetZoneId() != (uint32)ZoneMask))
			continue;

		plr->GetSession()->SendPacket(&data);
	}
}

void MapMgr::SendPacketToAllPlayers( WorldPacket *packet ) const{
	for( PlayerStorageMap::const_iterator itr = m_PlayerStorage.begin(); itr != m_PlayerStorage.end(); ++itr ){
		Player *p = itr->second;

		if( p->GetSession() != NULL )
			p->GetSession()->SendPacket( packet );
	}
}

void MapMgr::SendPacketToPlayersInZone( uint32 zone, WorldPacket *packet ) const{
	for( PlayerStorageMap::const_iterator itr = m_PlayerStorage.begin(); itr != m_PlayerStorage.end(); ++itr ){
		Player *p = itr->second;

		if( ( p->GetSession() != NULL ) && ( p->GetZoneId() == zone ) )
			p->GetSession()->SendPacket( packet );
	}
}

void MapMgr::LoadInstanceScript()
{
	mInstanceScript = sScriptMgr.CreateScriptClassForInstance(_mapId, this);
};

void MapMgr::CallScriptUpdate()
{
	ARCEMU_ASSERT(mInstanceScript  != NULL);
	mInstanceScript->UpdateEvent();
};

uint16 MapMgr::GetAreaID(float x, float y)
{
	uint32 exploreFlag = _terrain->GetAreaFlag(x, y);

	std::map<uint32, AreaTable*>::iterator itr = sWorld.mAreaIDToTable.find(exploreFlag);

	if(itr == sWorld.mAreaIDToTable.end())
		return 0;
	return itr->second->AreaId;
}

void MapMgr::onWorldStateUpdate( uint32 zone, uint32 field, uint32 value )
{
	WorldPacket data( SMSG_UPDATE_WORLD_STATE, 8 );
	data << uint32( field );
	data << uint32( value );
	
	SendPacketToPlayersInZone( zone, &data );
}


/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
#define MAP_MGR_UPDATE_PERIOD 100
#define MAPMGR_INACTIVE_MOVE_TIME 30

#define Z_SEARCH_RANGE 2


extern bool bServerShutdown;

MapMgr::MapMgr(Map *map, uint32 mapId, uint32 instanceid) : CellHandler<MapCell>(map), _mapId(mapId), eventHolder(instanceid)
{
	_shutdown = false;
	m_instanceID = instanceid;
	pMapInfo = WorldMapInfoStorage.LookupEntry(mapId);
	m_UpdateDistance = pMapInfo->update_distance * pMapInfo->update_distance;
	iInstanceMode = 0;

	// Create script interface
	ScriptInterface = new MapScriptInterface(*this);

	// Set up storage arrays
	m_CreatureArraySize = map->CreatureSpawnCount;
	m_GOArraySize = map->GameObjectSpawnCount;

	//m_CreatureStorage = new Creature*[m_CreatureArraySize];
	m_CreatureStorage = (Creature**)malloc(sizeof(Creature*) * m_CreatureArraySize);
	memset(m_CreatureStorage,0,sizeof(Creature*)*m_CreatureArraySize);

	//m_GOStorage = new GameObject*[m_GOArraySize];
	m_GOStorage = (GameObject**)malloc(sizeof(GameObject*) * m_GOArraySize);
	memset(m_GOStorage,0,sizeof(GameObject*)*m_GOArraySize);
	m_GOHighGuid = m_CreatureHighGuid = 0;
	m_DynamicObjectHighGuid=0; 
	lastUnitUpdate = getMSTime();
	lastGameobjectUpdate = getMSTime();
	m_battleground = 0;

	m_holder = &eventHolder;
	m_event_Instanceid = eventHolder.GetInstanceID();
	forced_expire = false;
	InactiveMoveTime = 0;
	mLoopCounter=0;
	pInstance = NULL;
	thread_kill_only = false;
	thread_running = false;

	m_forcedcells.clear();
	m_PlayerStorage.clear();
	m_PetStorage.clear();
	m_DynamicObjectStorage.clear();

	_combatProgress.clear();
	_mapWideStaticObjects.clear();
	_worldStateSet.clear();
	_updates.clear();
	_processQueue.clear();
	Sessions.clear();

	activeGameObjects.clear();
	activeCreatures.clear();
	m_corpses.clear();
	_sqlids_creatures.clear();
	_sqlids_gameobjects.clear();
	_reusable_guids_gameobject.clear();
	_reusable_guids_creature.clear();
}


MapMgr::~MapMgr()
{
	_shutdown=true;
	sEventMgr.RemoveEvents(this);
	if (ScriptInterface != NULL) {
		delete ScriptInterface;
		ScriptInterface = NULL;
	}
	
	// Remove objects
	if(_cells)
	{
		for (uint32 i = 0; i < _sizeX; i++)
		{
			if(_cells[i] != 0)
			{
				for (uint32 j = 0; j < _sizeY; j++)
				{
					if(_cells[i][j] != 0)
					{
						_cells[i][j]->_unloadpending=false;
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
		delete (*itr);
	}
	_mapWideStaticObjects.clear();

	if (m_GOStorage != NULL) {
		free(m_GOStorage);
		m_GOStorage = NULL;
	}
	if (m_CreatureStorage != NULL) {
		free(m_CreatureStorage);
		m_CreatureStorage = NULL;
	}

	Corpse * pCorpse;
	for(set<Corpse*>::iterator itr = m_corpses.begin(); itr != m_corpses.end();)
	{
		pCorpse = *itr;
		++itr;

		if(pCorpse->IsInWorld())
			pCorpse->RemoveFromWorld(false);

		delete pCorpse;
	}
	m_corpses.clear();

	Log.Notice("MapMgr", "Instance %u shut down. (%s)" , m_instanceID, GetBaseMap()->GetName());
}

WorldPacket* MapMgr::BuildInitialWorldState()
{
	WorldPacket * data = new WorldPacket(SMSG_INIT_WORLD_STATES, 99);
	*data << GetMapId();
	*data << (uint32)0; // zone id?
	*data << (uint32)0; // area id?
	*data << (uint16)_worldStateSet.size();
	std::map<uint32,uint32>::iterator itr = _worldStateSet.begin();
	for(; itr != _worldStateSet.end(); itr++)
	{
		*data << (uint32)itr->first;
		*data << (uint32)itr->second;
	}

	return data;
}

void MapMgr::SetWorldState(uint32 state, uint32 value)
{
	if(_worldStateSet.find(state) == _worldStateSet.end())
	{
		_worldStateSet.insert( make_pair(state, value) );
	}
	else
	{
		_worldStateSet[state] = value;
	}

	// Distribute this update
	
	SessionSet::iterator itr = Sessions.begin();
	for(; itr != Sessions.end(); itr++)
	{
		WorldSession * pSession = (*itr);
		if(!pSession)
			continue;

		WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);
		data << state;
		data << value;
		pSession->SendPacket(&data);
	}
}

uint32 MapMgr::GetWorldState(uint32 state)
{
	if(_worldStateSet.find(state) == _worldStateSet.end())
		return 0;

	return _worldStateSet[state];
}

uint32 MapMgr::GetTeamPlayersCount(uint32 teamId)
{
	uint32 result = 0;
	PlayerStorageMap::iterator itr = m_PlayerStorage.begin();
	for(; itr != m_PlayerStorage.end(); itr++)
	{
		Player * pPlayer = (itr->second);
		if(pPlayer->GetTeam() == teamId)
			result++;
	}
	return result;
}


void MapMgr::PushObject(Object *obj)
{
	/////////////
	// Assertions
	/////////////
	ASSERT(obj);
	
	// That object types are not map objects. TODO: add AI groups here?
	if(obj->GetTypeId() == TYPEID_ITEM || obj->GetTypeId() == TYPEID_CONTAINER)
	{
		// mark object as updatable and exit
		return;
	}

	if(obj->GetTypeId() == TYPEID_CORPSE)
	{
		m_corpses.insert(((Corpse*)obj));
	}	
	
	obj->ClearInRangeSet();
	ASSERT(obj->GetMapId() == _mapId);
	if(!(obj->GetPositionX() < _maxX && obj->GetPositionX() > _minX) || 
	   !(obj->GetPositionY() < _maxY && obj->GetPositionY() > _minY))
	{
		if(obj->IsPlayer())
		{
			Player * plr = static_cast< Player* >( obj );
			if(plr->GetBindMapId() != GetMapId())
			{
				plr->SafeTeleport(plr->GetBindMapId(),0,plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				WorldPacket * data = plr->BuildTeleportAckMsg(plr->GetPosition());
				plr->GetSession()->SendPacket(data);
				delete data;
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0,0,0,0);
		}
	}

	ASSERT(obj->GetPositionY() < _maxY && obj->GetPositionY() > _minY);
	ASSERT(_cells);

	///////////////////////
	// Get cell coordinates
	///////////////////////

	uint32 x = GetPosX(obj->GetPositionX());
	uint32 y = GetPosY(obj->GetPositionY());

	if(x >= _sizeX || y >= _sizeY)
	{
		if(obj->IsPlayer())
		{
			Player * plr = static_cast< Player* >( obj );
			if(plr->GetBindMapId() != GetMapId())
			{
				plr->SafeTeleport(plr->GetBindMapId(),0,plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				WorldPacket * data = plr->BuildTeleportAckMsg(plr->GetPosition());
				plr->GetSession()->SendPacket(data);
				delete data;
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0,0,0,0);
		}

		x = GetPosX(obj->GetPositionX());
		y = GetPosY(obj->GetPositionY());
	}

	MapCell *objCell = GetCell(x,y);
	if (!objCell)
	{
		objCell = Create(x,y);
		objCell->Init(x, y, _mapId, this);
	}

	uint32 endX = (x <= _sizeX) ? x + 1 : (_sizeX-1);
	uint32 endY = (y <= _sizeY) ? y + 1 : (_sizeY-1);
	uint32 startX = x > 0 ? x - 1 : 0;
	uint32 startY = y > 0 ? y - 1 : 0;
	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter;

	ByteBuffer * buf = 0;
	uint32 count;
	Player *plObj;

	if(obj->GetTypeId() == TYPEID_PLAYER)
		plObj = static_cast< Player* >( obj );
	else
		plObj = NULL;

	if(plObj)
	{
		sLog.outDetail("Creating player "I64FMT" for himself.", obj->GetGUID());
		ByteBuffer pbuf(10000);
		count = plObj->BuildCreateUpdateBlockForPlayer(&pbuf, plObj);
		plObj->PushCreationData(&pbuf, count);
	}

	//////////////////////
	// Build in-range data
	//////////////////////

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			cell = GetCell(posX, posY);
			if (cell)
			{
				UpdateInRangeSet(obj, plObj, cell, &buf);
			}
		}
	}

	//Add to the cell's object list
	objCell->AddObject(obj);

	obj->SetMapCell(objCell);
	 //Add to the mapmanager's object list
	if(plObj)
	{
	   m_PlayerStorage[plObj->GetLowGUID()] = plObj;
	   UpdateCellActivity(x, y, 2);
	}
	else
	{
		switch(obj->GetTypeFromGUID())
		{
		case HIGHGUID_TYPE_PET:
			m_PetStorage[obj->GetUIdFromGUID()] = static_cast< Pet* >( obj );
			break;

		case HIGHGUID_TYPE_UNIT:
			{
				ASSERT((obj->GetUIdFromGUID()) <= m_CreatureHighGuid);
				m_CreatureStorage[obj->GetUIdFromGUID()] = (Creature*)obj;
				if(((Creature*)obj)->m_spawn != NULL)
				{
					_sqlids_creatures.insert(make_pair( ((Creature*)obj)->m_spawn->id, ((Creature*)obj) ) );
				}
			}break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			{
				m_GOStorage[obj->GetUIdFromGUID()] = (GameObject*)obj;
				if(((GameObject*)obj)->m_spawn != NULL)
				{
					_sqlids_gameobjects.insert(make_pair( ((GameObject*)obj)->m_spawn->id, ((GameObject*)obj) ) );
				}
			}break;

		case HIGHGUID_TYPE_DYNAMICOBJECT:
			m_DynamicObjectStorage[obj->GetLowGUID()] = (DynamicObject*)obj;
			break;
		}
	}

	// Handle activation of that object.
	if(objCell->IsActive() && obj->CanActivate())
		obj->Activate(this);

	// Add the session to our set if it is a player.
	if(plObj)
	{
		Sessions.insert(plObj->GetSession());

		// Change the instance ID, this will cause it to be removed from the world thread (return value 1)
		plObj->GetSession()->SetInstance(GetInstanceID());

		/* Add the map wide objects */
		if(_mapWideStaticObjects.size())
		{
			if(!buf)
				buf = new ByteBuffer(300);

			for(set<Object*>::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
			{
				count = (*itr)->BuildCreateUpdateBlockForPlayer(buf, plObj);
				plObj->PushCreationData(buf, count);
			}
		}
	}

	if(buf)
		delete buf;

	if(plObj && InactiveMoveTime && !forced_expire)
		InactiveMoveTime = 0;
}


void MapMgr::PushStaticObject(Object *obj)
{
	_mapWideStaticObjects.insert(obj);

	switch(obj->GetTypeFromGUID())
	{
		case HIGHGUID_TYPE_UNIT:
			m_CreatureStorage[obj->GetUIdFromGUID()] = (Creature*)obj;
			break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			m_GOStorage[obj->GetUIdFromGUID()] = (GameObject*)obj;
			break;

		default:
			// maybe add a warning, shouldnt be needed
			break;
	}
}

#define OPTIONAL_IN_RANGE_SETS

void MapMgr::RemoveObject(Object *obj, bool free_guid)
{
	/////////////
	// Assertions
	/////////////

	ASSERT(obj);
	ASSERT(obj->GetMapId() == _mapId);
	//ASSERT(obj->GetPositionX() > _minX && obj->GetPositionX() < _maxX);
	//ASSERT(obj->GetPositionY() > _minY && obj->GetPositionY() < _maxY);
	ASSERT(_cells);

	if(obj->Active)
		obj->Deactivate(this);

	//there is a very small chance that on double player ports on same update player is added to multiple insertpools but not removed
	//one clear example was the double port proc when exploiting double resurect 
	m_objectinsertlock.Acquire();
	m_objectinsertpool.erase( obj );
	m_objectinsertlock.Release();

	_updates.erase( obj );
	obj->ClearUpdateMask();
	Player* plObj = (obj->GetTypeId() == TYPEID_PLAYER) ? static_cast< Player* >( obj ) : 0;

	///////////////////////////////////////
	// Remove object from all needed places
	///////////////////////////////////////
 
	switch(obj->GetTypeFromGUID())
	{
		case HIGHGUID_TYPE_UNIT:
			ASSERT(obj->GetUIdFromGUID() <= m_CreatureHighGuid);
			m_CreatureStorage[obj->GetUIdFromGUID()] = 0;
			if(((Creature*)obj)->m_spawn != NULL)
			{
				_sqlids_creatures.erase(((Creature*)obj)->m_spawn->id);
			}

			if(free_guid)
				_reusable_guids_creature.push_back(obj->GetUIdFromGUID());

			  break;

		case HIGHGUID_TYPE_PET:
			m_PetStorage.erase(obj->GetUIdFromGUID());
			break;

		case HIGHGUID_TYPE_DYNAMICOBJECT:
			m_DynamicObjectStorage.erase(obj->GetLowGUID());
			break;

		case HIGHGUID_TYPE_GAMEOBJECT:
			ASSERT(obj->GetUIdFromGUID() <= m_GOHighGuid);
			m_GOStorage[obj->GetUIdFromGUID()] = 0;
			if(((GameObject*)obj)->m_spawn != NULL)
			{
				_sqlids_gameobjects.erase(((GameObject*)obj)->m_spawn->id);
			}

			if(free_guid)
				_reusable_guids_gameobject.push_back(obj->GetUIdFromGUID());

			break;
	}

	// That object types are not map objects. TODO: add AI groups here?
	if(obj->GetTypeId() == TYPEID_ITEM || obj->GetTypeId() == TYPEID_CONTAINER || obj->GetTypeId()==10)
	{
		return;
	}

	if(obj->GetTypeId() == TYPEID_CORPSE)
	{
		m_corpses.erase(((Corpse*)obj));
	}

	if(!obj->GetMapCell())
	{
		/* set the map cell correctly */
		if(obj->GetPositionX() >= _maxX || obj->GetPositionX() <= _minY ||
			obj->GetPositionY() >= _maxY || obj->GetPositionY() <= _minY)
		{
			// do nothing
		}
		else
		{
			obj->SetMapCell(this->GetCellByCoords(obj->GetPositionX(), obj->GetPositionY()));
		}		
	}

	if(obj->GetMapCell())
	{
		ASSERT(obj->GetMapCell());
	
		// Remove object from cell
		obj->GetMapCell()->RemoveObject(obj);
	
		// Unset object's cell
		obj->SetMapCell(NULL);
	}

	// Clear any updates pending
	if(obj->GetTypeId() == TYPEID_PLAYER)
	{
		_processQueue.erase( static_cast< Player* >( obj ) );
		static_cast< Player* >( obj )->ClearAllPendingUpdates();
	}
	
	// Remove object from all objects 'seeing' him
	//there is a small chance for deadlock here. On the a situation i never understood how it can happen and corrupt the list too.
	obj->AquireInrangeLock(); //make sure to release lock before exit function !
	for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin();
		iter != obj->GetInRangeSetEnd(); ++iter)
	{
		if( (*iter) )
		{
			if( (*iter)->GetTypeId() == TYPEID_PLAYER )
			{
				if( static_cast< Player* >( *iter )->IsVisible( obj ) && static_cast< Player* >( *iter )->m_TransporterGUID != obj->GetGUID() )
					static_cast< Player* >( *iter )->PushOutOfRange(obj->GetNewGUID());
			}
			(*iter)->RemoveInRangeObject(obj);
		}
	}
	obj->ReleaseInrangeLock();
	
	// Clear object's in-range set
	obj->ClearInRangeSet();

	// If it's a player - update his nearby cells
	if(!_shutdown && obj->GetTypeId() == TYPEID_PLAYER)
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
		m_PlayerStorage.erase( static_cast< Player* >( obj )->GetLowGUID() );
	}

	// Remove the session from our set if it is a player.
	if(plObj)
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

void MapMgr::ChangeObjectLocation( Object *obj )
{
	// Items and containers are of no interest for us
	if ( !obj ) return; // crashfix

	if( obj->GetTypeId() == TYPEID_ITEM || obj->GetTypeId() == TYPEID_CONTAINER || obj->GetMapMgr() != this )
	{
		return;
	}

	Player* plObj;
	ByteBuffer * buf = 0;

	if( obj->GetTypeId() == TYPEID_PLAYER )
	{
		plObj = static_cast< Player* >( obj );
	}
	else
	{
		plObj = NULL;
	}

	Object* curObj;
	float fRange;

	///////////////////////////////////////
	// Update in-range data for old objects
	///////////////////////////////////////

	/** let's duplicate some code here :P Less branching is always good.
	 * - Burlex
	 */
/*#define IN_RANGE_LOOP \
	for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin(), iter2; \
		iter != obj->GetInRangeSetEnd();) \
	{ \
		curObj = *iter; \
		iter2 = iter; \
		++iter; \
		if(curObj->IsPlayer() && obj->IsPlayer() && plObj->m_TransporterGUID && plObj->m_TransporterGUID == static_cast< Player* >( curObj )->m_TransporterGUID ) \
			fRange = 0.0f;		\
		else if((curObj->GetGUIDHigh() == HIGHGUID_TRANSPORTER || obj->GetGUIDHigh() == HIGHGUID_TRANSPORTER)) \
			fRange = 0.0f;		\
		else if((curObj->GetGUIDHigh() == HIGHGUID_GAMEOBJECT && curObj->GetUInt32Value(GAMEOBJECT_TYPE_ID) == GAMEOBJECT_TYPE_TRANSPORT || obj->GetGUIDHigh() == HIGHGUID_GAMEOBJECT && obj->GetUInt32Value(GAMEOBJECT_TYPE_ID) == GAMEOBJECT_TYPE_TRANSPORT)) \
			fRange = 0.0f;		\
		else \
			fRange = m_UpdateDistance;	\
		if (curObj->GetDistance2dSq(obj) > fRange && fRange > 0) \

#define END_IN_RANGE_LOOP } \

	if(plObj)
	{
		IN_RANGE_LOOP
		{
			plObj->RemoveIfVisible(curObj);
			plObj->RemoveInRangeObject(iter2);

			if(curObj->NeedsInRangeSet())
				curObj->RemoveInRangeObject(obj);

			if(curObj->IsPlayer())
				static_cast< Player* >( curObj )->RemoveIfVisible(obj);
		}
		END_IN_RANGE_LOOP
	}
	else if(obj->NeedsInRangeSet())
	{
		IN_RANGE_LOOP
		{
			if(curObj->NeedsInRangeSet())
				curObj->RemoveInRangeObject(obj);

			if(curObj->IsPlayer())
				static_cast< Player* >( curObj )->RemoveIfVisible(obj);

			obj->RemoveInRangeObject(iter2);
		}
		END_IN_RANGE_LOOP
	}
	else
	{
		IN_RANGE_LOOP
		{
			if(curObj->NeedsInRangeSet())
				curObj->RemoveInRangeObject(obj);

			if(curObj->IsPlayer())
			{
				static_cast< Player* >( curObj )->RemoveIfVisible(obj);
				obj->RemoveInRangePlayer(curObj);
			}
		}
		END_IN_RANGE_LOOP
	}

#undef IN_RANGE_LOOP
#undef END_IN_RANGE_LOOP*/

	if(obj->HasInRangeObjects()) 
	{
		obj->AquireInrangeLock(); //make sure to release lock before exit function !
		for (Object::InRangeSet::iterator iter = obj->GetInRangeSetBegin(), iter2;
			iter != obj->GetInRangeSetEnd();)
		{
			curObj = *iter;
			iter2 = iter++;
			if( curObj->IsPlayer() && obj->IsPlayer() && plObj->m_TransporterGUID && plObj->m_TransporterGUID == static_cast< Player* >( curObj )->m_TransporterGUID )
				fRange = 0.0f; // unlimited distance for people on same boat
			else if( curObj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER )
				fRange = 0.0f; // unlimited distance for transporters (only up to 2 cells +/- anyway.)
			else
				fRange = m_UpdateDistance; // normal distance

			if( fRange > 0.0f && curObj->GetDistance2dSq(obj) > fRange )
			{
				if( plObj )
					plObj->RemoveIfVisible(curObj);

				if( curObj->IsPlayer() )
					static_cast< Player* >( curObj )->RemoveIfVisible(obj);

				curObj->RemoveInRangeObject(obj);

				if( obj->GetMapMgr() != this )
				{
					/* Something removed us. */
					obj->ReleaseInrangeLock();
					return;
				}
				obj->RemoveInRangeObject(iter2);
			}
		}
		obj->ReleaseInrangeLock();
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
		if(obj->IsPlayer())
		{
			Player* plr = static_cast< Player* >( obj );
			if(plr->GetBindMapId() != GetMapId())
			{
				plr->SafeTeleport(plr->GetBindMapId(),0,plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				return;
			}
			else
			{
				obj->GetPositionV()->ChangeCoords(plr->GetBindPositionX(),plr->GetBindPositionY(),plr->GetBindPositionZ(),0);
				plr->GetSession()->SystemMessage("Teleported you to your hearthstone location as you were out of the map boundaries.");
				WorldPacket * data = plr->BuildTeleportAckMsg(plr->GetPosition());
				plr->GetSession()->SendPacket(data);
				delete data;
			}
		}
		else
		{
			obj->GetPositionV()->ChangeCoords(0,0,0,0);
		}
	}

	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());

	if(cellX >= _sizeX || cellY >= _sizeY)
	{
		return;
	}

	MapCell *objCell = GetCell(cellX, cellY);
	MapCell * pOldCell = obj->GetMapCell();
	if (!objCell)
	{
		objCell = Create(cellX,cellY);
		objCell->Init(cellX, cellY, _mapId, this);
	}

	// If object moved cell
	if (objCell != obj->GetMapCell())
	{
		// THIS IS A HACK!
		// Current code, if a creature on a long waypoint path moves from an active
		// cell into an inactive one, it will disable itself and will never return.
		// This is to prevent cpu leaks. I will think of a better solution very soon :P

		if(!objCell->IsActive() && !plObj && obj->Active)
			obj->Deactivate(this);

		if(obj->GetMapCell())
			obj->GetMapCell()->RemoveObject(obj);
	
		objCell->AddObject(obj);
		obj->SetMapCell(objCell);

		// if player we need to update cell activity
		// radius = 2 is used in order to update both
		// old and new cells
		if(obj->GetTypeId() == TYPEID_PLAYER)
		{
			// have to unlock/lock here to avoid a deadlock situation.
			UpdateCellActivity(cellX, cellY, 2);
			if( pOldCell != NULL )
			{
				// only do the second check if theres -/+ 2 difference
				if( abs( (int)cellX - (int)pOldCell->_x ) > 2 ||
					abs( (int)cellY - (int)pOldCell->_y ) > 2 )
				{
					UpdateCellActivity( pOldCell->_x, pOldCell->_y, 2 );
				}
			}
		}
	}


	//////////////////////////////////////
	// Update in-range set for new objects
	//////////////////////////////////////

	uint32 endX = cellX <= _sizeX ? cellX + 1 : (_sizeX-1);
	uint32 endY = cellY <= _sizeY ? cellY + 1 : (_sizeY-1);
	uint32 startX = cellX > 0 ? cellX - 1 : 0;
	uint32 startY = cellY > 0 ? cellY - 1 : 0;
	uint32 posX, posY;
	MapCell *cell;

	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell)
				UpdateInRangeSet(obj, plObj, cell, &buf);
		}
	}

	if(buf)
		delete buf;
}

void MapMgr::UpdateInRangeSet( Object *obj, Player *plObj, MapCell* cell, ByteBuffer ** buf )
{
	#define CHECK_BUF if(!*buf) *buf = new ByteBuffer(2500)

	if( cell == NULL )
		return;

	Object* curObj;
	Player* plObj2;
	int count;
	ObjectSet::iterator iter = cell->Begin();
	ObjectSet::iterator itr;
	float fRange;
	bool cansee, isvisible;

	while( iter != cell->End() )
	{
		curObj = *iter;
		++iter;

		if( curObj == NULL )
			continue;

		if( curObj->IsPlayer() && obj->IsPlayer() && plObj->m_TransporterGUID && plObj->m_TransporterGUID == static_cast< Player* >( curObj )->m_TransporterGUID )
			fRange = 0.0f; // unlimited distance for people on same boat
		else if( curObj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER )
			fRange = 0.0f; // unlimited distance for transporters (only up to 2 cells +/- anyway.)
		else
			fRange = m_UpdateDistance; // normal distance

		if ( curObj != obj && ( curObj->GetDistance2dSq( obj ) <= fRange || fRange == 0.0f ) )
		{
			if( !obj->IsInRangeSet( curObj ) )
			{
				// Object in range, add to set
				obj->AddInRangeObject( curObj );
				curObj->AddInRangeObject( obj );

				if( curObj->IsPlayer() )
				{
					plObj2 = static_cast< Player* >( curObj );

					if( plObj2->CanSee( obj ) && !plObj2->IsVisible( obj ) )
					{
						CHECK_BUF;
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2);
						plObj2->PushCreationData(*buf, count);
						plObj2->AddVisibleObject(obj);
						(*buf)->clear();
					}
				}

				if( plObj != NULL )
				{
					if( plObj->CanSee( curObj ) && !plObj->IsVisible( curObj ) )
					{
						CHECK_BUF;
						count = curObj->BuildCreateUpdateBlockForPlayer( *buf, plObj );
						plObj->PushCreationData( *buf, count );
						plObj->AddVisibleObject( curObj );
						(*buf)->clear();
					}
				}
			}
			else
			{
				// Check visiblility
				if( curObj->IsPlayer() )
				{
					plObj2 = static_cast< Player* >( curObj );
					cansee = plObj2->CanSee(obj);
					isvisible = plObj2->GetVisibility(obj, &itr);
					if(!cansee && isvisible)
					{
						plObj2->PushOutOfRange(obj->GetNewGUID());
						plObj2->RemoveVisibleObject(itr);
					}
					else if(cansee && !isvisible)
					{
						CHECK_BUF;
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2);
						plObj2->PushCreationData(*buf, count);
						plObj2->AddVisibleObject(obj);
						(*buf)->clear();
					}
				}

				if( plObj )
				{
					cansee = plObj->CanSee( curObj );
					isvisible = plObj->GetVisibility( curObj, &itr );
					if(!cansee && isvisible)
					{
						plObj->PushOutOfRange( curObj->GetNewGUID() );
						plObj->RemoveVisibleObject( itr );
					}
					else if(cansee && !isvisible)
					{
						CHECK_BUF;
						count = curObj->BuildCreateUpdateBlockForPlayer( *buf, plObj );
						plObj->PushCreationData( *buf, count );
						plObj->AddVisibleObject( curObj );
						(*buf)->clear();
					}
				}
			}
		}
	}
/*
#define IN_RANGE_LOOP_P1 \
	while(iter != cell->End()) \
	{ \
		curObj = *iter; \
		++iter; \
		if(curObj->IsPlayer() && obj->IsPlayer() && plObj && plObj->m_TransporterGUID && plObj->m_TransporterGUID == static_cast< Player* >( curObj )->m_TransporterGUID) \
			fRange = 0.0f; \
		else if((curObj->GetGUIDHigh() == HIGHGUID_TRANSPORTER ||obj->GetGUIDHigh() == HIGHGUID_TRANSPORTER)) \
			fRange = 0.0f; \
		else if((curObj->GetGUIDHigh() == HIGHGUID_GAMEOBJECT && curObj->GetUInt32Value(GAMEOBJECT_TYPE_ID) == GAMEOBJECT_TYPE_TRANSPORT || obj->GetGUIDHigh() == HIGHGUID_GAMEOBJECT && obj->GetUInt32Value(GAMEOBJECT_TYPE_ID) == GAMEOBJECT_TYPE_TRANSPORT)) \
			fRange = 0.0f; \
		else \
			fRange = m_UpdateDistance; \
		if ( curObj != obj && (fRange == 0.0f || curObj->GetDistance2dSq(obj) < fRange ) ) \
		{ \
			if(!obj->IsInRangeSet(curObj)) \
			{ \
				if(curObj->NeedsInRangeSet()) \
				{ \
					curObj->AddInRangeObject(obj); \
				} else if(obj->IsPlayer()) \
				{ \
					curObj->AddInRangePlayer(obj); \
				} \
				if(curObj->IsPlayer()) \
				{ \
					plObj2 = static_cast< Player* >( curObj ); \
					if (plObj2->CanSee(obj) && !plObj2->IsVisible(obj))  \
					{ \
						CHECK_BUF; \
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2); \
						plObj2->PushCreationData(*buf, count); \
						plObj2->AddVisibleObject(obj); \
						(*buf)->clear(); \
					} \
				} 

#define IN_RANGE_LOOP_P2 \
			} \
			else \
			{ \
				if(curObj->IsPlayer()) \
				{ \
					plObj2 = static_cast< Player* >( curObj ); \
					cansee = plObj2->CanSee(obj); \
					isvisible = plObj2->GetVisibility(obj, &itr); \
					if(!cansee && isvisible) \
					{ \
						plObj2->RemoveVisibleObject(itr); \
						plObj2->PushOutOfRange(obj->GetNewGUID()); \
					} \
					else if(cansee && !isvisible) \
					{ \
						CHECK_BUF; \
						count = obj->BuildCreateUpdateBlockForPlayer(*buf, plObj2); \
						plObj2->PushCreationData(*buf, count); \
						plObj2->AddVisibleObject(obj); \
						(*buf)->clear(); \
					} \
				} \

#define IN_RANGE_LOOP_P3 \
			} \
		} \
	} \


	if(plObj)
	{
		IN_RANGE_LOOP_P1

			obj->AddInRangeObject(curObj);
			if(plObj->CanSee(curObj) && !plObj->IsVisible(curObj))
			{
				CHECK_BUF;
				count = curObj->BuildCreateUpdateBlockForPlayer(*buf, plObj);
				plObj->PushCreationData(*buf, count);
				plObj->AddVisibleObject(curObj);
				(*buf)->clear();
			}

		IN_RANGE_LOOP_P2

			if(plObj)
			{
				cansee = plObj->CanSee(curObj);
				isvisible = plObj->GetVisibility(curObj, &itr);
				if(!cansee && isvisible)
				{
					plObj->PushOutOfRange(curObj->GetNewGUID());
					plObj->RemoveVisibleObject(itr);
				}
				else if(cansee && !isvisible)
				{
					CHECK_BUF;
					count = curObj->BuildCreateUpdateBlockForPlayer(*buf, plObj);
					plObj->PushCreationData(*buf, count);
					plObj->AddVisibleObject(curObj);
					(*buf)->clear();
				}
			}

		IN_RANGE_LOOP_P3
	} else if(obj->NeedsInRangeSet())
	{
		IN_RANGE_LOOP_P1
			obj->AddInRangeObject(curObj);
		IN_RANGE_LOOP_P2
		IN_RANGE_LOOP_P3
	}
	else
	{
		IN_RANGE_LOOP_P1
			if(curObj->IsPlayer())
				obj->AddInRangePlayer(obj);

		IN_RANGE_LOOP_P2
		IN_RANGE_LOOP_P3
	}

#undef IN_RANGE_LOOP_P1
#undef IN_RANGE_LOOP_P2
#undef IN_RANGE_LOOP_P3*/
}

void MapMgr::_UpdateObjects()
{
	if(!_updates.size() && !_processQueue.size())
		return;

	Object *pObj;
	Player *pOwner;
	//std::set<Object*>::iterator it_start, it_end, itr;
	std::set<Player*>::iterator it_start, it_end, itr;
	Player * lplr;
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

		if(pObj->GetTypeId() == TYPEID_ITEM || pObj->GetTypeId() == TYPEID_CONTAINER)
		{
			// our update is only sent to the owner here.
			pOwner = static_cast< Item* >(pObj)->GetOwner();
			if( pOwner != NULL )
			{
				count = static_cast< Item* >( pObj )->BuildValuesUpdateBlockForPlayer( &update, pOwner );
				// send update to owner
				if( count )
				{
					pOwner->PushUpdateData( &update, count );
					update.clear();
				}
			}
		}
		else
		{
			if( pObj->IsInWorld() )
			{
				// players have to receive their own updates ;)
				if( pObj->GetTypeId() == TYPEID_PLAYER )
				{
					// need to be different! ;)
					count = pObj->BuildValuesUpdateBlockForPlayer( &update, static_cast< Player* >( pObj ) );
					if( count )
					{
						static_cast< Player* >( pObj )->PushUpdateData( &update, count );
						update.clear();
					}
				}

				if( pObj->IsUnit() && pObj->HasUpdateField( UNIT_FIELD_HEALTH ) )
					static_cast< Unit* >( pObj )->EventHealthChangeSinceLastUpdate();

				// build the update
				count = pObj->BuildValuesUpdateBlockForPlayer( &update, static_cast< Player* >( NULL ) );

				if( count )
				{
					it_start = pObj->GetInRangePlayerSetBegin();
					it_end = pObj->GetInRangePlayerSetEnd();
					for(itr = it_start; itr != it_end;)
					{
						lplr = *itr;
						++itr;
						// Make sure that the target player can see us.
						if( lplr->GetTypeId() == TYPEID_PLAYER && lplr->IsVisible( pObj ) )
							lplr->PushUpdateData( &update, count );
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
	Player *plyr;
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
	MapCell * cellInfo;
	CellSpawns * spawns;

	for( uint32 x = 0 ; x < _sizeX ; x ++ )
	{
		for( uint32 y = 0 ; y < _sizeY ; y ++ )
		{
			cellInfo = GetCell( x , y );
			
			if( !cellInfo )
			{
				// Cell doesn't exist, create it.
				// There is no spoon. Err... cell.
				cellInfo = Create( x , y );
				cellInfo->Init( x , y , _mapId , this );
				sLog.outDetail( "Created cell [%u,%u] on map %d (instance %d)." , x , y , _mapId , m_instanceID );
				cellInfo->SetActivity( true );
				_map->CellGoneActive( x , y );
				ASSERT( !cellInfo->IsLoaded() );

				spawns = _map->GetSpawnsList( x , y );
				if( spawns )
					cellInfo->LoadObjects( spawns );
			}
			else
			{
				// Cell exists, but is inactive
				if ( !cellInfo->IsActive() )
				{
					sLog.outDetail("Activated cell [%u,%u] on map %d (instance %d).", x, y, _mapId, m_instanceID );
					_map->CellGoneActive( x , y );
					cellInfo->SetActivity( true );

					if (!cellInfo->IsLoaded())
					{
						//sLog.outDetail("Loading objects for Cell [%d][%d] on map %d (instance %d)...", 
						//	posX, posY, this->_mapId, m_instanceID);
						spawns = _map->GetSpawnsList( x , y );
						if( spawns )
							cellInfo->LoadObjects( spawns );
					}
				}
			}
		}
	}
}

void MapMgr::UpdateCellActivity(uint32 x, uint32 y, int radius)
{
	CellSpawns * sp;
	uint32 endX = (x + radius) <= _sizeX ? x + radius : (_sizeX-1);
	uint32 endY = (y + radius) <= _sizeY ? y + radius : (_sizeY-1);
	uint32 startX = x - radius > 0 ? x - radius : 0;
	uint32 startY = y - radius > 0 ? y - radius : 0;
	uint32 posX, posY;

	MapCell *objCell;

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			objCell = GetCell(posX, posY);

			if (!objCell)
			{
				if (_CellActive(posX, posY))
				{
					objCell = Create(posX, posY);
					objCell->Init(posX, posY, _mapId, this);

					sLog.outDetail("Cell [%d,%d] on map %d (instance %d) is now active.", 
						posX, posY, this->_mapId, m_instanceID);
					objCell->SetActivity(true);
					_map->CellGoneActive(posX, posY);

					ASSERT(!objCell->IsLoaded());

					sLog.outDetail("Loading objects for Cell [%d][%d] on map %d (instance %d)...", 
						posX, posY, this->_mapId, m_instanceID);

					sp = _map->GetSpawnsList(posX, posY);
					if(sp) objCell->LoadObjects(sp);
				}
			}
			else
			{
				//Cell is now active
				if (_CellActive(posX, posY) && !objCell->IsActive())
				{
					sLog.outDetail("Cell [%d,%d] on map %d (instance %d) is now active.", 
						posX, posY, this->_mapId, m_instanceID);
					_map->CellGoneActive(posX, posY);
					objCell->SetActivity(true);

					if (!objCell->IsLoaded())
					{
						sLog.outDetail("Loading objects for Cell [%d][%d] on map %d (instance %d)...", 
							posX, posY, this->_mapId, m_instanceID);
						sp = _map->GetSpawnsList(posX, posY);
						if(sp) objCell->LoadObjects(sp);
					}
				}
				//Cell is no longer active
				else if (!_CellActive(posX, posY) && objCell->IsActive())
				{
					sLog.outDetail("Cell [%d,%d] on map %d (instance %d) is now idle.", 
						posX, posY, this->_mapId, m_instanceID);
					_map->CellGoneIdle(posX, posY);
					objCell->SetActivity(false);
				}
			}
		}
	}

}

bool MapMgr::_CellActive(uint32 x, uint32 y)
{
	uint32 endX = ((x+1) <= _sizeX) ? x + 1 : (_sizeX-1);
	uint32 endY = ((y+1) <= _sizeY) ? y + 1 : (_sizeY-1);
	uint32 startX = x > 0 ? x - 1 : 0;
	uint32 startY = y > 0 ? y - 1 : 0;
	uint32 posX, posY;

	MapCell *objCell;

	for (posX = startX; posX <= endX; posX++ )
	{
		for (posY = startY; posY <= endY; posY++ )
		{
			objCell = GetCell(posX, posY);

			if (objCell)
			{
				if ( objCell->HasPlayers() || m_forcedcells.find( objCell ) != m_forcedcells.end() )
				{
					return true;
				}
			}
		}
	}

	return false;
}

void MapMgr::ObjectUpdated(Object *obj)
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


void MapMgr::ChangeFarsightLocation(Player *plr, DynamicObject *farsight)
{
	if(farsight == 0)
	{
		// We're clearing.
		for(ObjectSet::iterator itr = plr->m_visibleFarsightObjects.begin(); itr != plr->m_visibleFarsightObjects.end();
			++itr)
		{
			if(plr->IsVisible((*itr)) && !plr->CanSee((*itr)))
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
		uint32 endX = (cellX <= _sizeX) ? cellX + 1 : (_sizeX-1);
		uint32 endY = (cellY <= _sizeY) ? cellY + 1 : (_sizeY-1);
		uint32 startX = cellX > 0 ? cellX - 1 : 0;
		uint32 startY = cellY > 0 ? cellY - 1 : 0;
		uint32 posX, posY;
		MapCell *cell;
		Object *obj;
		MapCell::ObjectSet::iterator iter, iend;
		uint32 count;
		for (posX = startX; posX <= endX; ++posX )
		{
			for (posY = startY; posY <= endY; ++posY )
			{
				cell = GetCell(posX, posY);
				if (cell)
				{
					iter = cell->Begin();
					iend = cell->End();
					for(; iter != iend; ++iter)
					{
						obj = (*iter);
						if(!plr->IsVisible(obj) && plr->CanSee(obj) && farsight->GetDistance2dSq(obj) <= m_UpdateDistance)
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

/* new stuff
*/

//#ifdef FORCED_SERVER_KEEPALIVE

//there is a chance at least one of the players got corrupted
void MapMgr::TeleportCorruptedPlayers()
{
	PlayerStorageMap::iterator itr =  m_PlayerStorage.begin();
	for(; itr !=  m_PlayerStorage.end();)
	{
		Player *p = itr->second;
		++itr;
		try	
		{ 
//			p->EjectFromInstance();
			if( p->GetSession() )
			{
				p->GetSession()->LogoutPlayer(false);
			}
			else
			{
				delete p;
			}
		}catch(int er)	{ er = 1; }//lols for the warning
	}
}
//an exception ocured somewhere. We try to cleanup if there is anything to clean up and then kill this thread.
//We should not save data in case some memory corruption ocured
//we should be prepared that during cleanup process we meet the same exeption and we have to skip that (or handle it)
//there might be cases when we should restart the mapmanager made for the the map (like always loaded maps)
void MapMgr::KillThreadWithCleanup()
{
	try
	{
		// remove all events regarding to this map
		sEventMgr.RemoveEvents( this );

		// Teleport any left-over players out.
		TeleportCorruptedPlayers();	

		if(m_battleground)
		{
			BattlegroundManager.DeleteBattleground(m_battleground);
			sInstanceMgr.DeleteBattlegroundInstance( GetMapId(), GetInstanceID() );
		}

		if(pInstance)
		{
			// check for a non-raid instance, these expire after 10 minutes.
			if(GetMapInfo()->type == INSTANCE_NONRAID || pInstance->m_isBattleground)
			{
				pInstance->m_mapMgr = NULL;
				sInstanceMgr._DeleteInstance(pInstance, true);
			}
			else
			{
				// just null out the pointer
				pInstance->m_mapMgr=NULL;
			}
		}
		else if(GetMapInfo()->type == INSTANCE_NULL)
		{
			uint32 this_mapid = GetMapId();
			sInstanceMgr.m_singleMaps[ this_mapid ] = NULL;
			//if we are really cocky we try to create a new map instead of the crashed one
			sInstanceMgr._CreateInstance( this_mapid, sInstanceMgr.GenerateInstanceID() );
		}

		thread_running = false;

		// delete ourselves
//		delete this; //threadpool will delete us ?
	}
	catch(int error)
	{
		error = 1;//deal with this later
		//we are just avoiding the process to catch this exeption. We know it was comming
	}

//	KillThread();
}
//#endif

bool MapMgr::run()
{
	bool rv=true;
	THREAD_TRY_EXECUTION_MapMgr
		rv = Do();
	THREAD_HANDLE_CRASH_MapMgr
	return rv;
}

bool MapMgr::Do()
{
#ifdef WIN32
	threadid=GetCurrentThreadId();
#endif
	thread_running = true;
	ThreadState =THREADSTATE_BUSY;
	SetThreadName("Map mgr - M%u|I%u",this->_mapId ,this->m_instanceID);
	ObjectSet::iterator i;
	uint32 last_exec=getMSTime();

	/* create static objects */
	for(GOSpawnList::iterator itr = _map->staticSpawns.GOSpawns.begin(); itr != _map->staticSpawns.GOSpawns.end(); ++itr)
	{
		GameObject * obj = CreateGameObject((*itr)->entry);
		obj->Load((*itr));
		_mapWideStaticObjects.insert(obj);
	}

	for(CreatureSpawnList::iterator itr = _map->staticSpawns.CreatureSpawns.begin(); itr != _map->staticSpawns.CreatureSpawns.end(); ++itr)
	{
		Creature * obj = CreateCreature((*itr)->entry);
		obj->Load(*itr, 0, pMapInfo);
		_mapWideStaticObjects.insert(obj);
	}

	/* add static objects */
	for(set<Object*>::iterator itr = _mapWideStaticObjects.begin(); itr != _mapWideStaticObjects.end(); ++itr)
		PushStaticObject(*itr);

	/* load corpses */
	objmgr.LoadCorpses(this);

	// always declare local variables outside of the loop!
	// otherwise theres a lot of sub esp; going on.

	uint32 exec_time, exec_start;
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif
	while((ThreadState != THREADSTATE_TERMINATE) && !_shutdown)
	{
		exec_start=getMSTime();
		//first push to world new objects
		m_objectinsertlock.Acquire();//<<<<<<<<<<<<<<<<
		if(m_objectinsertpool.size())
		{
			for(i=m_objectinsertpool.begin();i!=m_objectinsertpool.end();i++)
			{
				//PushObject(*i);
				(*i)->PushToWorld(this);
			}
			m_objectinsertpool.clear();
		}
		m_objectinsertlock.Release();//>>>>>>>>>>>>>>>>
		//-------------------------------------------------------
				
		//Now update sessions of this map + objects
		_PerformObjectDuties();

		last_exec=getMSTime();
		exec_time=last_exec-exec_start;
		if(exec_time<MAP_MGR_UPDATE_PERIOD)
		{
			/*
				The common place I see this is waiting for a Win32 thread to exit. I used to come up with all sorts of goofy,
				elaborate event-based systems to do this myself until I discovered that thread handles are waitable. Just use
				WaitForSingleObject() on the thread handle and you're done. No risking race conditions with the thread exit code.
				I think pthreads has pthread_join() for this too.

				- http://www.virtualdub.org/blog/pivot/entry.php?id=62
			*/

#ifdef WIN32
			WaitForSingleObject(hThread, MAP_MGR_UPDATE_PERIOD-exec_time);
#else
			Sleep(MAP_MGR_UPDATE_PERIOD-exec_time);
#endif
		}

		//////////////////////////////////////////////////////////////////////////
		// Check if we have to die :P
		//////////////////////////////////////////////////////////////////////////
		if(InactiveMoveTime && UNIXTIME >= InactiveMoveTime)
			break;
	}

	// Clear the instance's reference to us.
	if(m_battleground)
	{
		BattlegroundManager.DeleteBattleground(m_battleground);
		sInstanceMgr.DeleteBattlegroundInstance( GetMapId(), GetInstanceID() );
	}

	if(pInstance)
	{
		// check for a non-raid instance, these expire after 10 minutes.
		if(GetMapInfo()->type == INSTANCE_NONRAID || pInstance->m_isBattleground)
		{
			pInstance->m_mapMgr = NULL;
			sInstanceMgr._DeleteInstance(pInstance, true);
		}
		else
		{
			// just null out the pointer
			pInstance->m_mapMgr=NULL;
		}
	}
	else if(GetMapInfo()->type == INSTANCE_NULL)
		sInstanceMgr.m_singleMaps[GetMapId()] = NULL;

	// Teleport any left-over players out.
	TeleportPlayers();	

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

void MapMgr::AddObject(Object *obj)
{
	m_objectinsertlock.Acquire();//<<<<<<<<<<<<
	m_objectinsertpool.insert(obj);
	m_objectinsertlock.Release();//>>>>>>>>>>>>
}


Unit* MapMgr::GetUnit(const uint64 & guid)
{
/*#ifdef USING_BIG_ENDIAN
	switch (((uint32*)&guid)[0])
#else
	switch (((uint32*)&guid)[1])
#endif
	{
	case	HIGHGUID_PLAYER:
		return GetPlayer((uint32)guid);
		break;
	case	HIGHGUID_UNIT:
		return GetCreature((uint32)guid);
		break;
	case	HIGHGUID_PET:
		return GetPet((uint32)guid);
		break;
	default:
		return NULL;
	}*/

	/*uint32 highguid = GUID_HIPART(guid);
	if( highguid & HIGHGUID_UNIT )
		return GetCreature( ((uint32)guid&LOWGUID_UNIT_MASK) );
	else if( highguid == HIGHGUID_PLAYER )			// players are always zero
		return GetPlayer( (uint32)guid );
	else if( highguid & HIGHGUID_PET )
		return GetPet( (uint32)guid );
	else
		return NULL;*/

	if ( guid == 0 )
		return NULL;

	switch(GET_TYPE_FROM_GUID(guid))
	{
	case HIGHGUID_TYPE_UNIT:
		return GetCreature( GET_LOWGUID_PART(guid) );
		break;

	case HIGHGUID_TYPE_PLAYER:
		return GetPlayer( (uint32)guid );
		break;

	case HIGHGUID_TYPE_PET:
		return GetPet( (uint32)guid );
		break;
	}

	return NULL;
}

Object* MapMgr::_GetObject(const uint64 & guid)
{
	if (!guid)
		return NULL;

	switch(GET_TYPE_FROM_GUID(guid))
	{
	case	HIGHGUID_TYPE_GAMEOBJECT:
		return GetGameObject(GET_LOWGUID_PART(guid));
		break;
	case	HIGHGUID_TYPE_UNIT:
		return GetCreature(GET_LOWGUID_PART(guid));
		break;
	case	HIGHGUID_TYPE_DYNAMICOBJECT:
		return GetDynamicObject((uint32)guid);
		break;
	case	HIGHGUID_TYPE_TRANSPORTER:
		return objmgr.GetTransporter(GUID_LOPART(guid));
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
		CreatureSet creatures(activeCreatures);
		CreatureSet::iterator itr = creatures.begin();
		PetStorageMap::iterator it2 = m_PetStorage.begin();
		Creature * ptr;
		Pet * ptr2;
		
		for(; itr != creatures.end();)
		{
			ptr = *itr;
			++itr;
//			if (ptr) //Zack : we never set this to null. We simply remove it from list
			ptr->Update(difftime);
		}

		for(; it2 != m_PetStorage.end();)
		{
			ptr2 = it2->second;
			++it2;

			ptr2->Update(difftime);
		}		
	}

	// Update players.
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.begin();
		Player* ptr;
		for(; itr != m_PlayerStorage.end(); )
		{
			ptr = static_cast< Player* >( (itr->second) );
			++itr;
			if( ptr != NULL )
				ptr->Update( difftime );
		}

		lastUnitUpdate = mstime;
	}

	// Update gameobjects (not on every loop, however)
	if( mLoopCounter % 2 )
	{
		difftime = mstime - lastGameobjectUpdate;

		GameObjectSet::iterator itr = activeGameObjects.begin();
		GameObject * ptr;
		for(; itr != activeGameObjects.end(); )
		{
			ptr = *itr;
			++itr;
			ptr->Update( difftime );
		}

		lastGameobjectUpdate = mstime;
	}	

	// Sessions are updated every loop.
	{
		int result;
		WorldSession * session;
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
			// .. and that could be diasterous to our client :P
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
	Corpse * pCorpse = objmgr.GetCorpse((uint32)guid);
	if(pCorpse == 0)	// Already Deleted
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
			Player *p = itr->second;
			++itr;
			p->EjectFromInstance();
		}
	}
	else
	{
		for(; itr !=  m_PlayerStorage.end();)
		{
			Player *p = itr->second;
			++itr;
			if(p->GetSession())
				p->GetSession()->LogoutPlayer(false);
			else
				delete p;
		}
	}
}

void MapMgr::UnloadCell(uint32 x,uint32 y)
{
	MapCell * c = GetCell(x,y);
	if(c == NULL || c->HasPlayers() || _CellActive(x,y) || !c->IsUnloadPending()) return;

	sLog.outDetail("Unloading Cell [%d][%d] on map %d (instance %d)...", 
		x,y,_mapId,m_instanceID);

	c->Unload();
}

void MapMgr::EventRespawnCreature(Creature * c, MapCell * p)
{
	ObjectSet::iterator itr = p->_respawnObjects.find( ((Object*)c) );
	if(itr != p->_respawnObjects.end())
	{
		c->m_respawnCell=NULL;
		p->_respawnObjects.erase(itr);
		c->OnRespawn(this);
	}
}

void MapMgr::EventRespawnGameObject(GameObject * o, MapCell * c)
{
	ObjectSet::iterator itr = c->_respawnObjects.find( ((Object*)o) );
	if(itr != c->_respawnObjects.end())
	{
		o->m_respawnCell=NULL;
		c->_respawnObjects.erase(itr);
		o->Spawn(this);
	}
}

void MapMgr::SendMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius /* = 2 */)
{
	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());
	uint32 endX = ((cellX+cell_radius) <= _sizeX) ? cellX + cell_radius : (_sizeX-1);
	uint32 endY = ((cellY+cell_radius) <= _sizeY) ? cellY + cell_radius : (_sizeY-1);
	uint32 startX = (cellX-cell_radius) > 0 ? cellX - cell_radius : 0;
	uint32 startY = (cellY-cell_radius) > 0 ? cellY - cell_radius : 0;

	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter, iend;
	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell && cell->HasPlayers() )
			{
				iter = cell->Begin();
				iend = cell->End();
				for(; iter != iend; ++iter)
				{
					if((*iter)->IsPlayer())
					{
						static_cast< Player* >(*iter)->GetSession()->SendPacket(packet);
					}
				}
			}
		}
	}
}

void MapMgr::SendChatMessageToCellPlayers(Object * obj, WorldPacket * packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession * originator)
{
	uint32 cellX = GetPosX(obj->GetPositionX());
	uint32 cellY = GetPosY(obj->GetPositionY());
	uint32 endX = ((cellX+cell_radius) <= _sizeX) ? cellX + cell_radius : (_sizeX-1);
	uint32 endY = ((cellY+cell_radius) <= _sizeY) ? cellY + cell_radius : (_sizeY-1);
	uint32 startX = (cellX-cell_radius) > 0 ? cellX - cell_radius : 0;
	uint32 startY = (cellY-cell_radius) > 0 ? cellY - cell_radius : 0;

	uint32 posX, posY;
	MapCell *cell;
	MapCell::ObjectSet::iterator iter, iend;
	for (posX = startX; posX <= endX; ++posX )
	{
		for (posY = startY; posY <= endY; ++posY )
		{
			cell = GetCell(posX, posY);
			if (cell && cell->HasPlayers() )
			{
				iter = cell->Begin();
				iend = cell->End();
				for(; iter != iend; ++iter)
				{
					if((*iter)->IsPlayer())
					{
						//static_cast< Player* >(*iter)->GetSession()->SendPacket(packet);
						static_cast< Player* >(*iter)->GetSession()->SendChatPacket(packet, langpos, lang, originator);
					}
				}
			}
		}
	}
}

Creature * MapMgr::GetSqlIdCreature(uint32 sqlid)
{
	CreatureSqlIdMap::iterator itr = _sqlids_creatures.find(sqlid);
	return (itr == _sqlids_creatures.end()) ? NULL : itr->second;
}

GameObject * MapMgr::GetSqlIdGameObject(uint32 sqlid)
{
	GameObjectSqlIdMap::iterator itr = _sqlids_gameobjects.find(sqlid);
	return (itr == _sqlids_gameobjects.end()) ? NULL : itr->second;
}

void MapMgr::HookOnAreaTrigger(Player * plr, uint32 id)
{
	switch (id)
	{
	case 4591:
		//Only opens when the first one steps in, if 669 if you find a way, put it in :P (else was used to increase the time the door stays opened when another one steps on it)
		GameObject *door = GetInterface()->GetGameObjectNearestCoords(803.827f, 6869.38f, -38.5434f, 184212);
		if (door && (door->GetUInt32Value(GAMEOBJECT_STATE) == 1))
		{
			door->SetUInt32Value(GAMEOBJECT_STATE, 0);
			//sEventMgr.AddEvent(door, &GameObject::SetUInt32Value, GAMEOBJECT_STATE, 1, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
		}
		//else
		//{
			//sEventMgr.RemoveEvents(door);
			//sEventMgr.AddEvent(door, &GameObject::SetUInt32Value,GAMEOBJECT_STATE, 0, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
		//}
		break;
	}
}

Creature * MapMgr::CreateCreature(uint32 entry)
{
	uint64 newguid = (uint64)HIGHGUID_TYPE_UNIT << 32;
	char * pHighGuid = (char*)&newguid;
	char * pEntry = (char*)&entry;
	pHighGuid[3] |= pEntry[0];
	pHighGuid[4] |= pEntry[1];
	pHighGuid[5] |= pEntry[2];
	pHighGuid[6] |= pEntry[3];

	if(_reusable_guids_creature.size())
	{
		uint32 guid = _reusable_guids_creature.front();
		_reusable_guids_creature.pop_front();

		newguid |= guid;
		return new Creature(newguid);
	}

	if(++m_CreatureHighGuid  >= m_CreatureArraySize)
	{
		// Reallocate array with larger size.
		m_CreatureArraySize += RESERVE_EXPAND_SIZE;
		m_CreatureStorage = (Creature**)realloc(m_CreatureStorage, sizeof(Creature*) * m_CreatureArraySize);
		memset(&m_CreatureStorage[m_CreatureHighGuid],0,(m_CreatureArraySize-m_CreatureHighGuid)*sizeof(Creature*));
	}

	newguid |= m_CreatureHighGuid;
	return new Creature(newguid);
}

GameObject * MapMgr::CreateGameObject(uint32 entry)
{
	if( _reusable_guids_gameobject.size() > GO_GUID_RECYCLE_INTERVAL )
	{
		uint32 guid = _reusable_guids_gameobject.front();
		_reusable_guids_gameobject.pop_front();
		return new GameObject((uint64)HIGHGUID_TYPE_GAMEOBJECT<<32 | guid);
	}

	if(++m_GOHighGuid  >= m_GOArraySize)
	{
		// Reallocate array with larger size.
		m_GOArraySize += RESERVE_EXPAND_SIZE;
		m_GOStorage = (GameObject**)realloc(m_GOStorage, sizeof(GameObject*) * m_GOArraySize);
		memset(&m_GOStorage[m_GOHighGuid],0,(m_GOArraySize-m_GOHighGuid)*sizeof(GameObject*));
	}
	return new GameObject((uint64)HIGHGUID_TYPE_GAMEOBJECT<<32 | m_GOHighGuid);
}

DynamicObject * MapMgr::CreateDynamicObject()
{
	return new DynamicObject(HIGHGUID_TYPE_DYNAMICOBJECT,(++m_DynamicObjectHighGuid));
}

void MapMgr::AddForcedCell( MapCell * c )
{
	m_forcedcells.insert( c );
	UpdateCellActivity( c->GetPositionX(), c->GetPositionY(), 1 );
}
void MapMgr::RemoveForcedCell(MapCell* c)
{
	m_forcedcells.erase( c );
	UpdateCellActivity( c->GetPositionX(), c->GetPositionY(), 1 );
}

float MapMgr::GetFirstZWithCPZ(float x,float y ,float z)
{
	if (!sWorld.Collision) return NO_WMO_HEIGHT;
	float posZ;
	for (int i=Z_SEARCH_RANGE;i>= (Z_SEARCH_RANGE-Z_SEARCH_RANGE*2);i--)
	{
		//if ( i==0 && !IsUnderground(x,y,z) ) return GetBaseMap()->GetLandHeight(x, y);
		posZ = CollideInterface.GetHeight(GetMapId(), x, y,z+(float)i);
		if ( posZ!=NO_WMO_HEIGHT ) break;
	}
	return posZ;
}

void MapMgr::SendPvPCaptureMessage(int32 ZoneMask, uint32 ZoneId, const char * Message, ...)
{
	va_list ap;
	va_start(ap, Message);

	WorldPacket data(SMSG_PVP_CAPTURE_STATE_MSG, 208);
	char msgbuf[200];
	vsnprintf(msgbuf, 200, Message, ap);
	va_end(ap);

	data << ZoneId;
	data << uint32(strlen(msgbuf)+1);
	data << msgbuf;

	PlayerStorageMap::iterator itr = m_PlayerStorage.begin();
	for(; itr !=  m_PlayerStorage.end();)
	{
		Player *plr = itr->second;
		++itr;

		if( ( ZoneMask != ZONE_MASK_ALL && plr->GetZoneId() != (uint32)ZoneMask) )
			continue;

		plr->GetSession()->SendPacket(&data);
	}
}

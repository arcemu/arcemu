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
// MapCell.cpp
//
#include "StdAfx.h"

extern bool bServerShutdown;

MapCell::~MapCell()
{
	RemoveObjects();
}

void MapCell::Init(uint32 x, uint32 y, MapMgr* mapmgr)
{
	_mapmgr = mapmgr;
	_active = false;
	_loaded = false;
	_playerCount = 0;
	_corpses.clear();
	_x = static_cast<uint16>(x);
	_y = static_cast<uint16>(y);
	_unloadpending = false;
	_objects.clear();
	objects_iterator = _objects.begin();
}

void MapCell::AddObject(Object* obj)
{
	if(obj->IsPlayer())
		++_playerCount;
	else if(obj->IsCorpse())
	{
		_corpses.push_back(obj);
		if(_unloadpending)
			CancelPendingUnload();
	}

	_objects.insert(obj);
}

void MapCell::RemoveObject(Object* obj)
{
	if(obj->IsPlayer())
		--_playerCount;
	else if(obj->IsCorpse())
		_corpses.remove(obj);

	if(objects_iterator != _objects.end() && (*objects_iterator) == obj)
		++objects_iterator;

	_objects.erase(obj);
}

void MapCell::SetActivity(bool state)
{
	if(!_active && state)
	{
		// Move all objects to active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if(!(*itr)->IsActive() && (*itr)->CanActivate())
				(*itr)->Activate(_mapmgr);
		}

		if(_unloadpending)
			CancelPendingUnload();

		if(sWorld.Collision)
		{
			CollideInterface.ActivateTile(_mapmgr->GetMapId(), _x / 8, _y / 8);
		}
	}
	else if(_active && !state)
	{
		// Move all objects from active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if((*itr)->IsActive())
				(*itr)->Deactivate(_mapmgr);
		}

		if(!_unloadpending && CanUnload())
			QueueUnloadPending();

		if(sWorld.Collision)
		{
			CollideInterface.DeactivateTile(_mapmgr->GetMapId(), _x / 8, _y / 8);
		}
	}

	_active = state;

}
void MapCell::RemoveObjects()
{
	ObjectSet::iterator itr;
	//uint32 ltime = getMSTime();

	//Zack : we are delaying cell removal so transports can see objects far away. We are waiting for the event to remove us
	if(_unloadpending == true)
		return;

	/* delete objects in pending respawn state */
	for(itr = _respawnObjects.begin(); itr != _respawnObjects.end(); ++itr)
	{
		switch((*itr)->GetTypeId())
		{
			case TYPEID_UNIT:
				if(!(*itr)->IsPet())
				{
					_mapmgr->_reusable_guids_creature.push_back((*itr)->GetUIdFromGUID());
					TO< Creature* >(*itr)->m_respawnCell = NULL;
					delete TO< Creature* >(*itr);
				}
				break;
			case TYPEID_GAMEOBJECT:
				_mapmgr->_reusable_guids_gameobject.push_back((*itr)->GetUIdFromGUID());
				TO< GameObject* >(*itr)->m_respawnCell = NULL;
				delete TO< GameObject* >(*itr);
				break;
		}
	}
	_respawnObjects.clear();

	//This time it's simpler! We just remove everything :)
	for(objects_iterator = _objects.begin(); objects_iterator != _objects.end();)
	{
		Object* obj = (*objects_iterator);

		objects_iterator++;

		//If MapUnloadTime is non-zero, a transport could get deleted here (when it arrives to a cell that's scheduled to be unloaded because players left from it), so don't delete it! - By: VLack aka. VLsoft
		if(!bServerShutdown && obj->IsGameObject() && TO< GameObject* >(obj)->GetInfo()->Type == GAMEOBJECT_TYPE_TRANSPORT)
			continue;

		if(obj->IsActive())
			obj->Deactivate(_mapmgr);

		if(obj->IsInWorld())
			obj->RemoveFromWorld(true);

		delete obj;
	}
	_objects.clear();
	_corpses.clear();
	_playerCount = 0;
	_loaded = false;
}


void MapCell::LoadObjects(CellSpawns* sp)
{
	//we still have mobs loaded on cell. There is no point of loading them again
	if(_loaded == true)
		return;

	_loaded = true;
	Instance* pInstance = _mapmgr->pInstance;
	InstanceBossInfoMap* bossInfoMap = objmgr.m_InstanceBossInfoMap[_mapmgr->GetMapId()];

	if(sp->CreatureSpawns.size())//got creatures
	{
		for(CreatureSpawnList::iterator i = sp->CreatureSpawns.begin(); i != sp->CreatureSpawns.end(); ++i)
		{
			uint32 respawnTimeOverride = 0;
			if(pInstance)
			{
				if(bossInfoMap != NULL && IS_PERSISTENT_INSTANCE(pInstance))
				{
					bool skip = false;
					for(std::set<uint32>::iterator killedNpc = pInstance->m_killedNpcs.begin(); killedNpc != pInstance->m_killedNpcs.end(); ++killedNpc)
					{
						// Do not spawn the killed boss.
						if((*killedNpc) == (*i)->entry)
						{
							skip = true;
							break;
						}
						// Do not spawn the killed boss' trash.
						InstanceBossInfoMap::const_iterator bossInfo = bossInfoMap->find((*killedNpc));
						if(bossInfo != bossInfoMap->end() && bossInfo->second->trash.find((*i)->id) != bossInfo->second->trash.end())
						{
							skip = true;
							break;
						}
					}
					if(skip)
						continue;

					for(InstanceBossInfoMap::iterator bossInfo = bossInfoMap->begin(); bossInfo != bossInfoMap->end(); ++bossInfo)
					{
						if(pInstance->m_killedNpcs.find(bossInfo->second->creatureid) == pInstance->m_killedNpcs.end() && bossInfo->second->trash.find((*i)->id) != bossInfo->second->trash.end())
						{
							respawnTimeOverride = bossInfo->second->trashRespawnOverride;
						}
					}
				}
				else
				{
					// No boss information available ... fallback ...
					if(pInstance->m_killedNpcs.find((*i)->id) != pInstance->m_killedNpcs.end())
						continue;
				}
			}

			Creature* c = _mapmgr->CreateCreature((*i)->entry);

			c->m_loadedFromDB = true;
			if(respawnTimeOverride > 0)
				c->m_respawnTimeOverride = respawnTimeOverride;

			if(c->Load(*i, _mapmgr->iInstanceMode, _mapmgr->GetMapInfo()) && c->CanAddToWorld())
			{
				c->PushToWorld(_mapmgr);
			}
			else
			{
				CreatureSpawn* spawn = (*i);
				Log.Error("MapCell", "Failed spawning Creature %u with spawnId %u MapId %u", spawn->entry, spawn->id, _mapmgr->GetMapId());
				delete c;//missing proto or something of that kind
			}
		}
	}

	if(sp->GOSpawns.size())//got GOs
	{
		for(GOSpawnList::iterator i = sp->GOSpawns.begin(); i != sp->GOSpawns.end(); ++i)
		{
			GameObject* go = _mapmgr->CreateGameObject((*i)->entry);
			if(go->Load(*i))
			{
				//uint32 state = go->GetByte(GAMEOBJECT_BYTES_1, 0);

				// FIX ME - burlex
				/*
				if(pInstance && pInstance->FindObject((*i)->stateNpcLink))
				{
					go->SetByte(GAMEOBJECT_BYTES_1, 0, (state ? 0 : 1));
				}*/

				go->m_loadedFromDB = true;
				go->PushToWorld(_mapmgr);
			}
			else
			{
				GOSpawn* spawn = (*i);
				Log.Error("MapCell", "Failed spawning GameObject %u with spawnId %u MapId %u", spawn->entry, spawn->id, _mapmgr->GetMapId());
				delete go;//missing proto or something of that kind
			}
		}
	}
}


void MapCell::QueueUnloadPending()
{
	if(_unloadpending)
		return;

	_unloadpending = true;
	Log.Debug("MapCell", "Queueing pending unload of cell %u %u", _x, _y);
	sEventMgr.AddEvent(_mapmgr, &MapMgr::UnloadCell, (uint32)_x, (uint32)_y, MAKE_CELL_EVENT(_x, _y), sWorld.map_unload_time * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void MapCell::CancelPendingUnload()
{
	Log.Debug("MapCell", "Cancelling pending unload of cell %u %u", _x, _y);
	if(!_unloadpending)
		return;

	sEventMgr.RemoveEvents(_mapmgr, MAKE_CELL_EVENT(_x, _y));
	_unloadpending = false;
}

void MapCell::Unload()
{
	Log.Debug("MapCell", "Unloading cell %u %u", _x, _y);
	ARCEMU_ASSERT(_unloadpending);
	if(_active)
	{
		_unloadpending = false;
		return;
	}

	_unloadpending = false;

	//in ~MapCell RemoveObjects() can delete an Object without removing it from the MapCell.cpp
	//Example:
	//Creature A has guardian B. MapCell is unloaded, _mapmgr->Remove(_x, _y) is called, nullifying the reference to the cell
	//in CellHandler. ~MapCell is called, RemoveObjects() is called and despawns A which despawns B, calling Object::RemoveFromWorld()
	//which calls MapMgr::RemoveObject(B) which calls cell->RemoveObject(obj) ONLY if cell is not NULL, but in this case is NULL, leaving
	//a reference to a deleted Object in MapCell::_objects, iterated in RemoveObjects(). Calling it here fixes this issue.
	//Note: RemoveObjects() is still called in ~MapCell, due to fancy ArcEmu behaviors, like the in-game command ".mapcell delete <x> <y>

	RemoveObjects();

	_mapmgr->Remove(_x, _y);
}

void MapCell::CorpseGoneIdle(Object* corpse)
{
	_corpses.remove(corpse);
	CheckUnload();
}

void MapCell::CheckUnload()
{
	if(!_active && !_unloadpending && CanUnload())
		QueueUnloadPending();
}

bool MapCell::CanUnload()
{
	if(_corpses.size() == 0 && _mapmgr->m_battleground == NULL)
		return true;
	else
		return false;
}
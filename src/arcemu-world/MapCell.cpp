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
// MapCell.cpp
//
#include "StdAfx.h"

MapCell::~MapCell()
{
	RemoveObjects();
}

void MapCell::Init(uint32 x, uint32 y, uint32 mapid, MapMgr *mapmgr)
{
	_mapmgr = mapmgr;
	_active = false;
	_loaded = false;
	_playerCount = 0;
	_x=x;
	_y=y;
	_unloadpending=false;
	_objects.clear();
}

void MapCell::AddObject(Object *obj)
{
	if(obj->IsPlayer())
		++_playerCount;

	_objects.insert(obj);
}

void MapCell::RemoveObject(Object *obj)
{
	if(obj->IsPlayer())
		--_playerCount;

	_objects.erase(obj);
}

void MapCell::SetActivity(bool state)
{	
	if(!_active && state)
	{
		// Move all objects to active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if(!(*itr)->Active && (*itr)->CanActivate())
				(*itr)->Activate(_mapmgr);
		}

		if(_unloadpending)
			CancelPendingUnload();

		if (sWorld.Collision) {
			CollideInterface.ActivateTile(_mapmgr->GetMapId(), _x/8, _y/8);
		}
	} else if(_active && !state)
	{
		// Move all objects from active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if((*itr)->Active)
				(*itr)->Deactivate(_mapmgr);
		}

		if(sWorld.map_unload_time && !_unloadpending)
			QueueUnloadPending();

		if (sWorld.Collision) {
			CollideInterface.DeactivateTile(_mapmgr->GetMapId(), _x/8, _y/8);
		}
	}

	_active = state; 

}
void MapCell::RemoveObjects()
{
	ObjectSet::iterator itr;
	uint32 count = 0;
	//uint32 ltime = getMSTime();

	//Zack : we are delaying cell removal so transports can see objects far away. We are waiting for the event to remove us
	if( _unloadpending == true )
		return;

	/* delete objects in pending respawn state */
	for(itr = _respawnObjects.begin(); itr != _respawnObjects.end(); ++itr)
	{
		switch((*itr)->GetTypeId())
		{
		case TYPEID_UNIT: {
				if( !(*itr)->IsPet() )
				{
					_mapmgr->_reusable_guids_creature.push_back( (*itr)->GetUIdFromGUID() );
					static_cast< Creature* >( *itr )->m_respawnCell=NULL;
					delete static_cast< Creature* >( *itr );
				}
			}break;

		case TYPEID_GAMEOBJECT: {
			_mapmgr->_reusable_guids_gameobject.push_back( (*itr)->GetUIdFromGUID() );
			static_cast< GameObject* >( *itr )->m_respawnCell=NULL;
			delete static_cast< GameObject* >( *itr );
			}break;
		}
	}
	_respawnObjects.clear();

	//This time it's simpler! We just remove everything :)
	for(itr = _objects.begin(); itr != _objects.end(); )
	{
		count++;

		Object *obj = (*itr);

		itr++;

		//zack : we actually never set this to null. Useless check for lucky memory corruption hit.
		if(!obj)
			continue;

		if( _unloadpending )
		{
			if(obj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER)
				continue;
			if(obj->GetTypeId()==TYPEID_CORPSE && obj->GetUInt32Value(CORPSE_FIELD_OWNER) != 0)
				continue;
			if(!obj->m_loadedFromDB)
				continue;
		}

		if( obj->Active )
			obj->Deactivate( _mapmgr );

		if( obj->IsInWorld() )
			obj->RemoveFromWorld( true );

		delete obj;
	}
	_objects.clear();

	_playerCount = 0;
	_loaded = false;
}


void MapCell::LoadObjects(CellSpawns * sp)
{
	//we still have mobs loaded on cell. There is no point of loading them again
	if( _loaded == true )
		return; 

	_loaded = true;
	Instance * pInstance = _mapmgr->pInstance;
	InstanceBossInfoMap *bossInfoMap = objmgr.m_InstanceBossInfoMap[_mapmgr->GetMapId()];

	if(sp->CreatureSpawns.size())//got creatures
	{
		for(CreatureSpawnList::iterator i=sp->CreatureSpawns.begin();i!=sp->CreatureSpawns.end();i++)
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
						if (bossInfo != bossInfoMap->end() && bossInfo->second->trash.find((*i)->id) != bossInfo->second->trash.end())
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

			Creature * c=_mapmgr->CreateCreature((*i)->entry);

			c->SetMapId(_mapmgr->GetMapId());
			c->SetInstanceID(_mapmgr->GetInstanceID());
			c->m_loadedFromDB = true;
			if(respawnTimeOverride > 0)
				c->m_respawnTimeOverride = respawnTimeOverride;

			if( c->Load(*i, _mapmgr->iInstanceMode, _mapmgr->GetMapInfo()) && c->CanAddToWorld() )
			{
				c->PushToWorld(_mapmgr);
			}
			else
				delete c;//missing proto or smth of that kind
		}
	}

	if(sp->GOSpawns.size())//got GOs
	{
		for(GOSpawnList::iterator i=sp->GOSpawns.begin();i!=sp->GOSpawns.end();i++)
		{
			GameObject * go=_mapmgr->CreateGameObject((*i)->entry);
			go->SetInstanceID(_mapmgr->GetInstanceID());
			if(go->Load(*i))
			{
				//uint32 state = go->GetUInt32Value(GAMEOBJECT_STATE);

				// FIXME - burlex
				/*
				if(pInstance && pInstance->FindObject((*i)->stateNpcLink))
				{
					go->SetUInt32Value(GAMEOBJECT_STATE, (state ? 0 : 1));
				}*/			   

				go->m_loadedFromDB = true;
				go->PushToWorld(_mapmgr);
			}
			else
				delete go;//missing proto or smth of that kind
		}
	}
}


void MapCell::QueueUnloadPending()
{
	if(_unloadpending)
		return;

	_unloadpending = true;
	//Log.Debug("MapCell", "Queueing pending unload of cell %u %u", _x, _y);
	sEventMgr.AddEvent(_mapmgr, &MapMgr::UnloadCell,(uint32)_x,(uint32)_y,MAKE_CELL_EVENT(_x,_y),sWorld.map_unload_time * 1000,1,0);
}

void MapCell::CancelPendingUnload()
{
	//Log.Debug("MapCell", "Cancelling pending unload of cell %u %u", _x, _y);
	if(!_unloadpending)
		return;

	sEventMgr.RemoveEvents(_mapmgr,MAKE_CELL_EVENT(_x,_y));
}

void MapCell::Unload()
{
	//Log.Debug("MapCell", "Unloading cell %u %u", _x, _y);
	ASSERT(_unloadpending);
	if(_active)
	{
		_unloadpending=false;
		return;
	}

	_unloadpending=false;
	RemoveObjects();
}

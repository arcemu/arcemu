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


/* * Class MapScriptInterface
   * Provides an interface to mapmgr for scripts, to obtain objects,
   * get players, etc.
*/

#include "StdAfx.h"
createFileSingleton(StructFactory);

MapScriptInterface::MapScriptInterface(MapMgr & mgr) : mapMgr(mgr)
{

}

MapScriptInterface::~MapScriptInterface()
{
	mapMgr.ScriptInterface = 0;
}

uint32 MapScriptInterface::GetPlayerCountInRadius(float x, float y, float z /* = 0.0f */, float radius /* = 5.0f */)
{
	// use a cell radius of 2
	uint32 PlayerCount = 0;
	uint32 cellX = mapMgr.GetPosX(x);
	uint32 cellY = mapMgr.GetPosY(y);

	uint32 endX = cellX < _sizeX ? cellX + 1 : _sizeX;
	uint32 endY = cellY < _sizeY ? cellY + 1 : _sizeY;
	uint32 startX = cellX > 0 ? cellX - 1 : 0;
	uint32 startY = cellY > 0 ? cellY - 1 : 0;
	MapCell* pCell;
	ObjectSet::iterator iter, iter_end;

	for(uint32 cx = startX; cx < endX; ++cx)
	{
		for(uint32 cy = startY; cy < endY; ++cy)
		{
			pCell = mapMgr.GetCell(cx, cy);
			if(pCell == 0 || pCell->GetPlayerCount() == 0)
				continue;

			iter = pCell->Begin();
			iter_end = pCell->End();

			for(; iter != iter_end; ++iter)
			{
				if((*iter)->IsPlayer() &&
				        (*iter)->CalcDistance(x, y, (z == 0.0f ? (*iter)->GetPositionZ() : z)) < radius)
				{
					++PlayerCount;
				}
			}
		}
	}

	return PlayerCount;
}

void MapScriptInterface::AddGameObjectSpawn( GOSpawn *gs )
{
	uint32 cx = mapMgr.GetPosX( gs->x );
	uint32 cy = mapMgr.GetPosY( gs->y );

	CellSpawns *cellSpawns = mapMgr.GetBaseMap()->CreateAndGetSpawnsList( cx, cy );
	GOSpawnList &spawns = cellSpawns->GOSpawns;

	GOSpawnList::iterator itr = std::find( spawns.begin(), spawns.end(), gs );
	if( itr != spawns.end() )
	{
		return;
	}

	spawns.push_back( gs );
}

void MapScriptInterface::RemoveGameObjectSpawn( GOSpawn *gs )
{
	uint32 cx = mapMgr.GetPosX( gs->x );
	uint32 cy = mapMgr.GetPosY( gs->y );

	CellSpawns *cellSpawns = mapMgr.GetBaseMap()->GetSpawnsList( cx, cy );
	if( cellSpawns == NULL )
	{
		return;
	}

	GOSpawnList &spawns = cellSpawns->GOSpawns;

	GOSpawnList::iterator itr = std::find( spawns.begin(), spawns.end(), gs );
	if( itr == spawns.end() )
	{
		return;
	}

	spawns.erase( itr );
}

void MapScriptInterface::spawnPersistentGameObject( uint32 entry, LocationVector &location, uint32 phase )
{
	GameObjectProto *proto = GameObjectProtoStorage.LookupEntry( entry );
	if( proto == NULL )
	{
		return;
	}

	/// Create and add the spawn
	GOSpawn *spawn = new GOSpawn();
	spawn->id = 0;
	spawn->entry = entry;
	spawn->x = location.x;
	spawn->y = location.y;
	spawn->z = location.z;
	spawn->facing = location.o;
	spawn->o = spawn->o1 = spawn->o2 = spawn->o3 = 0;
	spawn->state = GAMEOBJECT_STATE_CLOSED;
	spawn->flags = 0;
	spawn->faction = proto->faction;
	spawn->scale = proto->Size;
	spawn->phase = phase;
	spawn->overrides = 0;

	AddGameObjectSpawn( spawn );

	/// If the cell is loaded, push the spawn to the world
	MapCell *cell = mapMgr.GetCellByCoords( location.x, location.y );
	if( ( cell != NULL ) && cell->IsLoaded() )
	{
		SpawnGameObject( spawn, true );
	}	
}

void MapScriptInterface::removePersistentGameObject( uint32 entry, LocationVector &location )
{
	uint32 cx = mapMgr.GetPosX( location.x );
	uint32 cy = mapMgr.GetPosY( location.y );

	/// First remove the spawn entry
	CellSpawns *cellSpawns = mapMgr.GetBaseMap()->GetSpawnsList( cx, cy );
	if( cellSpawns != NULL )
	{
		GOSpawnList &spawns = cellSpawns->GOSpawns;
		GOSpawnList::iterator itr = spawns.begin();
		while( itr != spawns.end() )
		{
			GOSpawn *spawn = *itr;

			if( ( spawn->entry == entry ) && ( spawn->x == location.x ) && ( spawn->y == location.y ) && ( spawn->z == location.z ) )
			{
				itr = spawns.erase( itr );
			}
			else
			{
				++itr;
			}
		}
	}

	/// If the GO is in world despawn it
	GameObject *go = GetGameObjectNearestCoords( location.x, location.y, location.z, entry );
	if( go != NULL )
	{
		go->Despawn( 1, 0 );
	}
}

GameObject* MapScriptInterface::SpawnGameObject(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, uint32 Misc1, uint32 Misc2, uint32 phase)
{

	GameObject* pGameObject = mapMgr.CreateGameObject(Entry);
	if(!pGameObject->CreateFromProto(Entry, mapMgr.GetMapId(), cX, cY, cZ, cO))
	{
		delete pGameObject;
		return NULL;
	}
	pGameObject->m_phase = phase;
	pGameObject->m_spawn = 0;

	if(AddToWorld)
		pGameObject->PushToWorld(&mapMgr);

	return pGameObject;
}

GameObject* MapScriptInterface::SpawnGameObject(GOSpawn* gs, bool AddToWorld)
{
	if(!gs)
		return NULL;

	GameObject* pGameObject = mapMgr.CreateGameObject(gs->entry);
	if(!pGameObject->Load(gs))
	{
		delete pGameObject;
		return NULL;
	}

	if(AddToWorld)
		pGameObject->PushToWorld(&mapMgr);

	return pGameObject;
}


void MapScriptInterface::addCreatureSpawn( CreatureSpawn *spawn )
{
	uint32 cx = mapMgr.GetPosX( spawn->x );
	uint32 cy = mapMgr.GetPosY( spawn->y );

	CellSpawns *cellSpawns = mapMgr.GetBaseMap()->CreateAndGetSpawnsList( cx, cy );
	CreatureSpawnList &spawns = cellSpawns->CreatureSpawns;

	CreatureSpawnList::iterator itr = std::find( spawns.begin(), spawns.end(), spawn );
	if( itr != spawns.end() )
	{
		return;
	}

	spawns.push_back( spawn );
}

void MapScriptInterface::removeCreatureSpawn( CreatureSpawn *spawn )
{
	uint32 cx = mapMgr.GetPosX( spawn->x );
	uint32 cy = mapMgr.GetPosY( spawn->y );

	CellSpawns *cellSpawns = mapMgr.GetBaseMap()->GetSpawnsList( cx, cy );
	if( cellSpawns == NULL )
	{
		return;
	}

	CreatureSpawnList &spawns = cellSpawns->CreatureSpawns;

	CreatureSpawnList::iterator itr = std::find( spawns.begin(), spawns.end(), spawn );
	if( itr == spawns.end() )
	{
		return;
	}

	spawns.erase( itr );
}

void MapScriptInterface::spawnPersistentCreature( uint32 entry, LocationVector &location, uint32 phase )
{
	CreatureProto *proto = CreatureProtoStorage.LookupEntry( entry );
	if( proto == NULL )
	{
		return;
	}

	uint32 displayId;
	proto->GenerateModelId( &displayId );

	CreatureSpawn *spawn = new CreatureSpawn();
	spawn->id = 0;
	spawn->form = NULL;
	spawn->entry = entry;
	spawn->x = location.x;
	spawn->y = location.y;
	spawn->z = location.z;
	spawn->o = location.o;
	spawn->movetype = MOVEMENTTYPE_DONTMOVEWP;	
	spawn->displayid = displayId;
	spawn->factionid = proto->Faction;
	spawn->flags = 0;
	spawn->bytes0 = 0;
	spawn->bytes1 = 0;
	spawn->bytes2 = 0;
	spawn->emote_state = 0;
	spawn->channel_spell = 0;
	spawn->channel_target_go = 0;
	spawn->channel_target_creature = 0;
	spawn->stand_state = 0;
	spawn->death_state = 0;
	spawn->MountedDisplayID = proto->mountDisplayId;
	spawn->Item1SlotDisplay = proto->itemSlot1Display;
	spawn->Item2SlotDisplay = proto->itemSlot2Display;
	spawn->Item3SlotDisplay = proto->itemSlot3Display;
	spawn->CanFly = 0;
	spawn->phase = phase;

	addCreatureSpawn( spawn );

	/// If the cell is loaded, push the spawn to the world
	MapCell *cell = mapMgr.GetCellByCoords( location.x, location.y );
	if( ( cell != NULL ) && cell->IsLoaded() )
	{
		SpawnCreature( spawn, true );
	}
}

void MapScriptInterface::removePersistentCreature( uint32 entry, LocationVector &location )
{
	uint32 cx = mapMgr.GetPosX( location.x );
	uint32 cy = mapMgr.GetPosY( location.y );

	/// First remove the spawn entry
	CellSpawns *cellSpawns = mapMgr.GetBaseMap()->GetSpawnsList( cx, cy );
	if( cellSpawns != NULL )
	{
		CreatureSpawnList &spawns = cellSpawns->CreatureSpawns;
		CreatureSpawnList::iterator itr = spawns.begin();
		while( itr != spawns.end() )
		{
			CreatureSpawn *spawn = *itr;

			if( ( spawn->entry == entry ) && ( spawn->x == location.x ) && ( spawn->y == location.y ) && ( spawn->z == location.z ) )
			{
				itr = spawns.erase( itr );
			}
			else
			{
				++itr;
			}
		}
	}

	/// If the Creature is in world despawn it
	Creature *creature = GetCreatureNearestCoords( location.x, location.y, location.z, entry );
	if( creature != NULL )
	{
		creature->Despawn( 1, 0 );
	}
}

Creature* MapScriptInterface::SpawnCreature(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, bool tmplate, uint32 Misc1, uint32 Misc2, uint32 phase)
{
	CreatureProto* proto = CreatureProtoStorage.LookupEntry(Entry);
	if(proto == NULL)
	{
		return 0;
	}

	CreatureSpawn* sp = new CreatureSpawn;
	sp->entry = Entry;
	uint32 DisplayID = 0;
	uint8 Gender = proto->GenerateModelId(&DisplayID);
	sp->displayid = DisplayID;
	sp->form = 0;
	sp->id = 0;
	sp->movetype = 0;
	sp->x = cX;
	sp->y = cY;
	sp->z = cZ;
	sp->o = cO;
	sp->emote_state = 0;
	sp->flags = 0;
	sp->factionid = proto->Faction;
	sp->bytes0 = 0;
	sp->bytes1 = 0;
	sp->bytes2 = 0;
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->death_state = 0;
	sp->channel_target_creature = sp->channel_target_go = sp->channel_spell = 0;
	sp->MountedDisplayID = 0;
	sp->Item1SlotDisplay = 0;
	sp->Item2SlotDisplay = 0;
	sp->Item3SlotDisplay = 0;
	sp->CanFly = 0;
	sp->phase = phase;

	Creature* p = this->mapMgr.CreateCreature(Entry);
	ARCEMU_ASSERT(p != NULL);
	p->Load(sp, (uint32)NULL, NULL);
	p->setGender(Gender);
	p->spawnid = 0;
	p->m_spawn = 0;
	delete sp;
	if(AddToWorld)
		p->PushToWorld(&mapMgr);
	return p;
}

Creature* MapScriptInterface::SpawnCreature(CreatureSpawn* sp, bool AddToWorld)
{
	if(!sp)
		return NULL;

	CreatureProto* proto = CreatureProtoStorage.LookupEntry(sp->entry);
	if(proto == NULL)
	{
		return 0;
	}

	uint8 Gender = proto->GenerateModelId(&sp->displayid);
	Creature* p = this->mapMgr.CreateCreature(sp->entry);
	ARCEMU_ASSERT(p != NULL);
	p->Load(sp, (uint32)NULL, NULL);
	p->setGender(Gender);
	p->spawnid = 0;
	p->m_spawn = 0;
	if(AddToWorld)
		p->PushToWorld(&mapMgr);
	return p;
}

void MapScriptInterface::DeleteCreature(Creature* ptr)
{
	delete ptr;
}

void MapScriptInterface::DeleteGameObject(GameObject* ptr)
{
	delete ptr;
}

WayPoint* StructFactory::CreateWaypoint()
{
	return new WayPoint;
}

/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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
	MapCell * pCell;
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
				if((*iter)->GetTypeId() == TYPEID_PLAYER &&
					(*iter)->CalcDistance(x, y, (z == 0.0f ? (*iter)->GetPositionZ() : z)) < radius)
				{
					++PlayerCount;
				}
			}
		}
	}

	return PlayerCount;
}

Creature * MapScriptInterface::SpawnCreature(uint32 entry_id, LocationVector & location, uint32 phase)
{
	CreatureProto * cproto = CreatureProtoStorage.LookupEntry(entry_id);
	CreatureInfo * cinfo = CreatureNameStorage.LookupEntry(entry_id);
	Creature * ncreature = NULL;
	if(cproto != NULL && cinfo != NULL)
	{
		ncreature = mapMgr.CreateCreature(entry_id);
		Arcemu::Util::ARCEMU_ASSERT(ncreature != NULL);
		ncreature->Load(cproto, location.x, location.y, location.z, location.o);
		ncreature->spawnid = 0;
		ncreature->m_spawn = NULL;
		ncreature->m_phase = phase;
		ncreature->AddToWorld(&mapMgr);
	}
	return ncreature;
}
GameObject* MapScriptInterface::SpawnGameObject(uint32 Entry, LocationVector & location, uint32 phase)
{

	GameObject *pGameObject = mapMgr.CreateGameObject(Entry);
	if(!pGameObject->CreateFromProto(Entry, mapMgr.GetMapId(), location.x, location.y, location.z, location.o) )
	{
		delete pGameObject;
		return NULL;
	}
	pGameObject->SetInstanceID(mapMgr.GetInstanceID());
	pGameObject->m_phase = phase;
	pGameObject->AddToWorld(&mapMgr);
	return pGameObject;
}

void MapScriptInterface::DeleteCreature(Creature* ptr)
{
	delete ptr;
}

void MapScriptInterface::DeleteGameObject(GameObject *ptr)
{
	delete ptr;
}

WayPoint * StructFactory::CreateWaypoint()
{
	return new WayPoint;
}

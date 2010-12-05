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

GameObject* MapScriptInterface::SpawnGameObject(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, uint32 Misc1, uint32 Misc2, uint32 phase)
{

	GameObject *pGameObject = mapMgr.CreateGameObject(Entry);
	if(!pGameObject->CreateFromProto(Entry, mapMgr.GetMapId(), cX, cY, cZ, cO))
	{
		delete pGameObject;
		return NULL;
	}
	pGameObject->m_phase = phase;

	if(AddToWorld)
		pGameObject->PushToWorld(&mapMgr);

	return pGameObject;
}

GameObject * MapScriptInterface::SpawnGameObject(GOSpawn * gs, bool AddToWorld)
{
	if(!gs)
		return NULL;

	GameObject *pGameObject = mapMgr.CreateGameObject(gs->entry);
	if(!pGameObject->Load(gs))
	{
		delete pGameObject;
		return NULL;
	}

	if(AddToWorld)
		pGameObject->PushToWorld(&mapMgr);

	return pGameObject;
}

Creature* MapScriptInterface::SpawnCreature(uint32 Entry, float cX, float cY, float cZ, float cO, bool AddToWorld, bool tmplate, uint32 Misc1, uint32 Misc2, uint32 phase)
{
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(Entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(Entry);
	if(proto == 0 || info == 0)
	{
		return 0;
	}

	CreatureSpawn * sp = new CreatureSpawn;
	sp->entry = Entry;
	uint32 DisplayID = 0;
	uint8 Gender = info->GenerateModelId(&DisplayID);
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

	Creature * p = this->mapMgr.CreateCreature(Entry);
	Arcemu::Util::ARCEMU_ASSERT(   p != NULL );
	p->Load(sp, (uint32)NULL, NULL);
	p->setGender(Gender);
	p->spawnid = 0;
	p->m_spawn = 0;
	delete sp;
	if (AddToWorld)
		p->PushToWorld(&mapMgr);
	return p;
}

Creature * MapScriptInterface::SpawnCreature(CreatureSpawn * sp, bool AddToWorld)
{
	if(!sp)
		return NULL;

	CreatureProto * proto = CreatureProtoStorage.LookupEntry(sp->entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(sp->entry);
	if(proto == 0 || info == 0)
	{
		return 0;
	}

	uint8 Gender = info->GenerateModelId(&sp->displayid);
	Creature * p = this->mapMgr.CreateCreature(sp->entry);
	Arcemu::Util::ARCEMU_ASSERT(   p != NULL );
	p->Load(sp, (uint32)NULL, NULL);
	p->setGender(Gender);
	p->spawnid = 0;
	p->m_spawn = 0;
	if (AddToWorld)
		p->PushToWorld(&mapMgr);
	return p;
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

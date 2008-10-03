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

#include "StdAfx.h"

#define MAX_MAP 600

CCollideInterface CollideInterface;
IVMapManager * CollisionMgr;
Mutex m_loadLock;
uint32 m_tilesLoaded[MAX_MAP][64][64];

#ifdef WIN32
#ifdef COLLISION_DEBUG

uint64 c_GetTimerValue()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	return li.QuadPart;
}

uint32 c_GetNanoSeconds(uint64 t1, uint64 t2)
{
	LARGE_INTEGER li;
	double val;
	QueryPerformanceFrequency( &li );
	val = double( t1 - t2 ) * 1000000;
	val /= li.QuadPart;
    return long2int32( val );	
}

#define COLLISION_BEGINTIMER uint64 v1 = c_GetTimerValue();

#endif	// COLLISION_DEBUG
#endif	// WIN32

#ifdef WIN32
#pragma comment(lib, "collision.lib")
#endif

// Debug functions
#ifdef COLLISION_DEBUG

void CCollideInterface::Init()
{
	Log.Notice("CollideInterface", "Init");
	COLLISION_BEGINTIMER;
	CollisionMgr = ((IVMapManager*)collision_init());
	printf("[%u ns] collision_init\n", c_GetNanoSeconds(c_GetTimerValue(), v1));
}

void CCollideInterface::ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	m_loadLock.Acquire();
	if(m_tilesLoaded[mapId][tileX][tileY] == 0)
	{
		COLLISION_BEGINTIMER;
		CollisionMgr->loadMap(sWorld.vMapPath.c_str, mapId, tileY, tileX);
		printf("[%u ns] collision_activate_cell %u %u %u\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, tileX, tileY);
	}

	++m_tilesLoaded[mapId][tileX][tileY];
	m_loadLock.Release();
}

void CCollideInterface::DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	m_loadLock.Acquire();
	if(!(--m_tilesLoaded[mapId][tileX][tileY]))
	{
		COLLISION_BEGINTIMER;
		CollisionMgr->unloadMap(mapId, tileY, tileX);
		printf("[%u ns] collision_deactivate_cell %u %u %u\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, tileX, tileY);
	}

	m_loadLock.Release();
}

void CCollideInterface::DeInit()
{
	Log.Notice("CollideInterface", "DeInit");
	COLLISION_BEGINTIMER;
	collision_shutdown();
	printf("[%u ns] collision_shutdown\n", c_GetNanoSeconds(c_GetTimerValue(), v1));
}

float CCollideInterface::GetHeight(uint32 mapId, float x, float y, float z)
{
	COLLISION_BEGINTIMER;
	float v = CollisionMgr->getHeight(mapId, x, y, z);
	printf("[%u ns] GetHeight Map:%u %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, x, y, z);
	return v;
}

float CCollideInterface::GetHeight(uint32 mapId, LocationVector & pos)
{
	COLLISION_BEGINTIMER;
	float v = CollisionMgr->getHeight(mapId, pos);
	printf("[%u ns] GetHeight Map:%u %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, pos.x, pos.y, pos.z);
	return v;
}

bool CCollideInterface::IsIndoor(uint32 mapId, LocationVector & pos)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->isInDoors(mapId, pos);
	printf("[%u ns] IsIndoor Map:%u %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, pos.x, pos.y, pos.z);
	return r;
}

bool CCollideInterface::IsOutdoor(uint32 mapId, LocationVector & pos)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->isOutDoors(mapId, pos);
	printf("[%u ns] IsOutdoor Map:%u %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, pos.x, pos.y, pos.z);
	return r;
}

bool CCollideInterface::IsIndoor(uint32 mapId, float x, float y, float z)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->isInDoors(mapId, x, y, z);
	printf("[%u ns] IsIndoor Map:%u %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, x, y, z);
	return r;
}

bool CCollideInterface::IsOutdoor(uint32 mapId, float x, float y, float z)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->isOutDoors(mapId, x, y, z);
	printf("[%u ns] IsOutdoor Map:%u %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, x, y, z);
	return r;
}

bool CCollideInterface::CheckLOS(uint32 mapId, LocationVector & pos1, LocationVector & pos2)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->isInLineOfSight(mapId, pos1, pos2);
	printf("[%u ns] CheckLOS Map:%u %f %f %f -> %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
	return r;
}

bool CCollideInterface::CheckLOS(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->isInLineOfSight(mapId, x1, y1, z1, x2, y2, z2);
	printf("[%u ns] CheckLOS Map:%u %f %f %f -> %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, x1, y1, z1, x2, y2, z2);
	return r;
}

bool CCollideInterface::GetFirstPoint(uint32 mapId, LocationVector & pos1, LocationVector & pos2, LocationVector & outvec, float distmod)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->getObjectHitPos(mapId, pos1, pos2, outvec, distmod);
	printf("[%u ns] GetFirstPoint Map:%u %f %f %f -> %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
	return r;
}

bool CCollideInterface::GetFirstPoint(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod)
{
	bool r;
	COLLISION_BEGINTIMER;
	r = CollisionMgr->getObjectHitPos(mapId, x1, y1, z1, x2, y2, z2, outx, outy, outz, distmod);
	printf("[%u ns] GetFirstPoint Map:%u %f %f %f -> %f %f %f\n", c_GetNanoSeconds(c_GetTimerValue(), v1), mapId, x1, y1, z1, x2, y2, z2);
	return r;
}

#else

void CCollideInterface::Init()
{
	Log.Notice("CollideInterface", "Init");
	CollisionMgr = ((IVMapManager*)collision_init());
}

void CCollideInterface::ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	m_loadLock.Acquire();
	if(m_tilesLoaded[mapId][tileX][tileY] == 0)
		CollisionMgr->loadMap(sWorld.vMapPath.c_str(), mapId, tileY, tileX);

	++m_tilesLoaded[mapId][tileX][tileY];
	m_loadLock.Release();
}

void CCollideInterface::DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
	m_loadLock.Acquire();
	if(!(--m_tilesLoaded[mapId][tileX][tileY]))
		CollisionMgr->unloadMap(mapId, tileY, tileX);

	m_loadLock.Release();
}

void CCollideInterface::DeInit()
{
	Log.Notice("CollideInterface", "DeInit");
	collision_shutdown();
}

#endif		// COLLISION_DEBUG

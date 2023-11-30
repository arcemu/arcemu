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

#ifndef _COLLIDEINTERFACE_H
#define _COLLIDEINTERFACE_H

/* imports */
#define NO_WMO_HEIGHT -200000

//#define COLLISION_DEBUG 1

#ifdef WIN32
#define COLLISION_IMPORT __declspec(dllimport)
#else
#define COLLISION_IMPORT
#endif

#define MMAP_MAGIC 0x4d4d4150   // 'MMAP'
#define MMAP_VERSION 3

enum NavTerrain
{
    NAV_EMPTY   = 0x00,
    NAV_GROUND  = 0x01,
    NAV_MAGMA   = 0x02,
    NAV_SLIME   = 0x04,
    NAV_WATER   = 0x08,
    NAV_UNUSED1 = 0x10,
    NAV_UNUSED2 = 0x20,
    NAV_UNUSED3 = 0x40,
    NAV_UNUSED4 = 0x80
                  // we only have 8 bits
};

struct MmapTileHeader
{
	uint32 mmapMagic;
	uint32 dtVersion;
	uint32 mmapVersion;
	uint32 size;
	bool usesLiquids : 1;

	MmapTileHeader() : mmapMagic(MMAP_MAGIC), dtVersion(DT_NAVMESH_VERSION),
		mmapVersion(MMAP_VERSION), size(0), usesLiquids(true) {}
};

class NavMeshData;
class NavMeshTile
{
	public:
		Arcemu::Threading::AtomicCounter refs;
		dtTileRef dtref;
};

class NavMeshData
{
	public:
		dtNavMesh* mesh;
		dtNavMeshQuery* query;

		Arcemu::Threading::AtomicCounter refs;

		FastMutex tilelock;
		std::map<uint32, dtTileRef> tilerefs; //key by tile, x | y <<  16

		~NavMeshData()
		{
			dtFreeNavMesh(mesh);
			dtFreeNavMeshQuery(query);
		}

		void AddRef() { ++refs; }
		bool DecRef() { if((--refs) == 0) { delete this; return true; } return false; }
};

class CCollideInterface
{
	public:
		void Init();
		void DeInit();

		//Key: mapid
		FastMutex m_navmaplock;
		std::map<uint32, NavMeshData*> m_navdata;

		void ActivateMap(uint32 mapid);
		void DeactiveMap(uint32 mapid);
		void ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
		void DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY);


		NavMeshData* GetNavMesh(uint32 mapId);
		void LoadNavMeshTile(uint32 mapId, uint32 tileX, uint32 tileY);


#ifdef COLLISION_DEBUG

		bool CheckLOS(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2);
		bool GetFirstPoint(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod);
		bool IsIndoor(uint32 mapId, float x, float y, float z);
		bool IsOutdoor(uint32 mapId, float x, float y, float z);

		float GetHeight(uint32 mapId, float x, float y, float z);
		bool CheckLOS(uint32 mapId, LocationVector & pos1, LocationVector & pos2);
		bool GetFirstPoint(uint32 mapId, LocationVector & pos1, LocationVector & pos2, LocationVector & outvec, float distmod);
		bool IsIndoor(uint32 mapId, LocationVector & pos);
		bool IsOutdoor(uint32 mapId, LocationVector & pos);
		float GetHeight(uint32 mapId, LocationVector & pos);

#else

		ARCEMU_INLINE bool CheckLOS(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2)
		{
			VMAP::IVMapManager* mgr = VMAP::VMapFactory::createOrGetVMapManager();
			return mgr->isInLineOfSight(mapId, x1, y1, z1, x2, y2, z2);
		}

		ARCEMU_INLINE bool GetFirstPoint(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod)
		{
			VMAP::IVMapManager* mgr = VMAP::VMapFactory::createOrGetVMapManager();
			return mgr->getObjectHitPos(mapId, x1, y1, z1, x2, y2, z2, outx, outy, outz, distmod);
		}

		ARCEMU_INLINE bool IsIndoor(uint32 mapId, float x, float y, float z)
		{
			return !IsOutdoor(mapId, x, y, z);
		}

		ARCEMU_INLINE bool IsOutdoor(uint32 mapId, float x, float y, float z)
		{
			VMAP::IVMapManager* mgr = VMAP::VMapFactory::createOrGetVMapManager();

			uint32 flags;
			int32 adtid, rootid, groupid;

			if(!mgr->getAreaInfo(mapId, x, y, z, flags, adtid, rootid, groupid))
				return true;

			WMOAreaTableEntry* wmoArea = sWorld.GetWMOAreaData(rootid, adtid, groupid);
			AreaTable* area = NULL;

			if(wmoArea != NULL)
			{
				area = dbcArea.LookupEntryForced(wmoArea->areaId);

				if(area != NULL)
				{
					if(area->AreaFlags & 0x04000000)  //outdoor
						return true;
					if(area->AreaFlags & 0x02000000)  //indoor
						return false;
				}

				if(wmoArea->flags & 4)  //outdoor
					return true;
				if(wmoArea->flags & 2)
					return false;
			}
			return (flags & 0x08) != 0;
		}

		ARCEMU_INLINE float GetHeight(uint32 mapId, float x, float y, float z)
		{
			VMAP::IVMapManager* mgr = VMAP::VMapFactory::createOrGetVMapManager();
			return mgr->getHeight(mapId, x, y, z, 10000.0f);
		}

		ARCEMU_INLINE bool CheckLOS(uint32 mapId, LocationVector & pos1, LocationVector & pos2)
		{
			return CheckLOS(mapId, pos1.x, pos1.y, pos1.z + 2, pos2.x, pos2.y, pos2.z + 2);
		}

		ARCEMU_INLINE bool GetFirstPoint(uint32 mapId, LocationVector & pos1, LocationVector & pos2, LocationVector & outvec, float distmod)
		{
			return GetFirstPoint(mapId, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, outvec.x, outvec.y, outvec.z, distmod);
		}

		ARCEMU_INLINE bool IsIndoor(uint32 mapId, LocationVector & pos)
		{
			return !IsOutdoor(mapId, pos);
		}

		ARCEMU_INLINE bool IsOutdoor(uint32 mapId, LocationVector & pos)
		{
			return IsOutdoor(mapId, pos.x, pos.y, pos.z);
		}

		ARCEMU_INLINE float GetHeight(uint32 mapId, LocationVector & pos)
		{
			return GetHeight(mapId, pos.x, pos.y, pos.z);
		}

#endif

};

SERVER_DECL extern CCollideInterface CollideInterface;

#endif

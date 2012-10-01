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

#ifndef _TERRAIN_H
#define _TERRAIN_H

#define TERRAIN_INVALID_HEIGHT -50000
#define TERRAIN_TILE_SIZE 533.3333333f
#define TERRAIN_NUM_TILES 64
#define TERRAIN_MAP_RESOLUTION 128

class TerrainHolder;
class TerrainTile;

#define MAP_AREA_NO_AREA      0x0001

#define MAP_HEIGHT_NO_HEIGHT  0x0001
#define MAP_HEIGHT_AS_INT16   0x0002
#define MAP_HEIGHT_AS_INT8    0x0004

#define MAP_LIQUID_NO_TYPE    0x0001
#define MAP_LIQUID_NO_HEIGHT  0x0002


struct TileMapHeader
{
	uint32 mapMagic;
	uint32 versionMagic;
	uint32 buildMagic;
	uint32 areaMapOffset;
	uint32 areaMapSize;
	uint32 heightMapOffset;
	uint32 heightMapSize;
	uint32 liquidMapOffset;
	uint32 liquidMapSize;
	uint32 holesOffset;
	uint32 holesSize;
};

struct TileMapAreaHeader
{
	uint32 fourcc;
	uint16 flags;
	uint16 gridArea;
};

struct TileMapHeightHeader
{
	uint32 fourcc;
	uint32 flags;
	float gridHeight;
	float gridMaxHeight;
};

struct TileMapLiquidHeader
{
	uint32 fourcc;
	uint16 flags;
	uint16 liquidType;
	uint8 offsetX;
	uint8 offsetY;
	uint8 width;
	uint8 height;
	float liquidLevel;
};

class TileMap
{
	public:

		//Area Map
		uint16 m_area;
		uint16* m_areaMap;

		//Height Map
		union
		{
			float* m_heightMap8F;
			uint16* m_heightMap8S;
			uint8* m_heightMap8B;
		};
		union
		{
			float* m_heightMap9F;
			uint16* m_heightMap9S;
			uint8* m_heightMap9B;
		};
		uint32 m_heightMapFlags;
		float m_heightMapMult;
		float m_tileHeight;

		//Liquid Map
		uint8* m_liquidType;
		float* m_liquidMap;
		float m_liquidLevel;
		uint8 m_liquidOffX;
		uint8 m_liquidOffY;
		uint8 m_liquidHeight;
		uint8 m_liquidWidth;
		uint16 m_defaultLiquidType;

		TileMap()
		{
			m_areaMap = NULL;
			m_area = 0;
			m_tileHeight = 0;
			m_heightMapFlags = 0;
			m_heightMap8F = NULL;
			m_heightMap9F = NULL;
			m_heightMapMult = 1;

			m_liquidType = NULL;
			m_liquidMap = NULL;
			m_liquidLevel = 0;
			m_liquidOffX = 0;
			m_liquidOffY = 0;
			m_liquidHeight = 0;
			m_liquidWidth = 0;
			m_defaultLiquidType = 0;
		}

		~TileMap()
		{
			delete[] m_areaMap;
			delete[] m_heightMap8F;
			delete[] m_heightMap9F;

			delete[] m_liquidType;
			delete[] m_liquidMap;
		}


		void Load(char* filename);

		void LoadLiquidData(FILE* f, TileMapHeader & header);
		void LoadHeightData(FILE* f, TileMapHeader & header);
		void LoadAreaData(FILE* f, TileMapHeader & header);

		float GetHeight(float x, float y);
		float GetHeightB(float x, float y, int x_int, int y_int);
		float GetHeightS(float x, float y, int x_int, int y_int);
		float GetHeightF(float x, float y, int x_int, int y_int);

		float GetLiquidHeight(float x, float y);
		uint8 GetLiquidType(float x, float y);

		uint32 GetArea(float x, float y);
};

class TerrainTile
{
	public:
		Arcemu::Threading::AtomicCounter m_refs;

		TerrainHolder* m_parent;
		uint32 m_mapid;
		int32 m_tx;
		int32 m_ty;

		//Children
		TileMap m_map;

		TerrainTile(TerrainHolder* parent, uint32 mapid, int32 x, int32 y);
		~TerrainTile();
		void AddRef() { ++m_refs; }
		void DecRef() { if(--m_refs == 0) delete this; }

		void Load()
		{
			char filename[1024];

			//Normal map stuff
			sprintf(filename, "maps/%03u%02u%02u.map", m_mapid, m_tx, m_ty);
			m_map.Load(filename);
		}
};

class TerrainHolder
{
	public:
		uint32 m_mapid;
		TerrainTile* m_tiles[TERRAIN_NUM_TILES][TERRAIN_NUM_TILES];
		FastMutex m_lock[TERRAIN_NUM_TILES][TERRAIN_NUM_TILES];
		Arcemu::Threading::AtomicCounter m_tilerefs[TERRAIN_NUM_TILES][TERRAIN_NUM_TILES];

		TerrainHolder(uint32 mapid)
		{
			for(int32 i = 0; i < TERRAIN_NUM_TILES; ++i)
				for(int32 j = 0; j < TERRAIN_NUM_TILES; ++j)
					m_tiles[i][j] = NULL;
			m_mapid = mapid;
		}

		~TerrainHolder()
		{
			for(int32 i = 0; i < TERRAIN_NUM_TILES; ++i)
				for(int32 j = 0; j < TERRAIN_NUM_TILES; ++j)
					UnloadTile(i, j);
		}

		TerrainTile* GetTile(float x, float y);
		TerrainTile* GetTile(int32 tx, int32 ty)
		{
			TerrainTile* rv = NULL;
			m_lock[tx][ty].Acquire();
			rv = m_tiles[tx][ty];
			if(rv != NULL)
				rv->AddRef();
			m_lock[tx][ty].Release();

			return rv;
		}

		void LoadTile(float x, float y)
		{
			int32 tx = (int32)(32 - (x / TERRAIN_TILE_SIZE));
			int32 ty = (int32)(32 - (y / TERRAIN_TILE_SIZE));
			LoadTile(tx, ty);
		}
		void LoadTile(int32 tx, int32 ty)
		{
			m_lock[tx][ty].Acquire();
			++m_tilerefs[tx][ty];
			if(m_tiles[tx][ty] == NULL)
			{
				m_tiles[tx][ty] = new TerrainTile(this, m_mapid, tx, ty);
				m_tiles[tx][ty]->Load();
			}
			m_lock[tx][ty].Release();
		}
		void UnloadTile(float x, float y)
		{
			int32 tx = (int32)(32 - (x / TERRAIN_TILE_SIZE));
			int32 ty = (int32)(32 - (y / TERRAIN_TILE_SIZE));
			UnloadTile(tx, ty);
		}

		void UnloadTile(int32 tx, int32 ty)
		{
			m_lock[tx][ty].Acquire();
			if(m_tiles[tx][ty] == NULL)
			{
				m_lock[tx][ty].Release();
				return;
			}
			m_lock[tx][ty].Release();

			if(--m_tilerefs[tx][ty] == 0)
			{
				m_lock[tx][ty].Acquire();
				if(m_tiles[tx][ty] != NULL)
					m_tiles[tx][ty]->DecRef();
				m_tiles[tx][ty] = NULL;
				m_lock[tx][ty].Release();
			}
		}

		//test
		float GetADTLandHeight(float x, float y)
		{
			TerrainTile* tile = GetTile(x, y);

			if(tile == NULL)
				return TERRAIN_INVALID_HEIGHT;
			float rv = tile->m_map.GetHeight(x, y);
			tile->DecRef();
			return rv;
		}

		float GetLandHeight(float x, float y, float z)
		{
			float adtheight = GetADTLandHeight(x, y);

			VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
			float vmapheight = vmgr->getHeight(m_mapid, x, y, z + 0.5f, 10000.0f);

			if(adtheight > z && vmapheight > -1000)
				return vmapheight; //underground
			return std::max(vmapheight, adtheight);
		}

		float GetLiquidHeight(float x, float y)
		{
			TerrainTile* tile = GetTile(x, y);

			if(tile == NULL)
				return TERRAIN_INVALID_HEIGHT;
			float rv = tile->m_map.GetLiquidHeight(x, y);
			tile->DecRef();
			return rv;
		}

		uint8 GetLiquidType(float x, float y)
		{
			TerrainTile* tile = GetTile(x, y);

			if(tile == NULL)
				return 0;
			uint8 rv = tile->m_map.GetLiquidType(x, y);
			tile->DecRef();
			return rv;
		}

		uint32 GetAreaFlag(float x, float y)
		{
			TerrainTile* tile = GetTile(x, y);

			if(tile == NULL)
				return 0;
			uint32 rv = tile->m_map.GetArea(x, y);
			tile->DecRef();
			return rv;
		}

		AreaTable* GetArea(float x, float y, float z);

		AreaTable* GetArea2D(float x, float y);

		bool GetLiquidInfo(float x, float y, float z, float & liquidlevel, uint32 & liquidtype)
		{
			VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();

			float flr;
			if(vmgr->GetLiquidLevel(m_mapid, x, y, z, 0xFF, liquidlevel, flr, liquidtype))
				return true;

			liquidlevel = GetLiquidHeight(x, y);
			liquidtype = GetLiquidType(x, y);

			if(liquidtype == 0)
				return false;
			return true;
		}

		bool InLineOfSight(float x, float y, float z, float x2, float y2, float z2)
		{
			VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();

			return vmgr->isInLineOfSight(m_mapid, x, y, z, x2, y2, z2);
		}
};

#endif

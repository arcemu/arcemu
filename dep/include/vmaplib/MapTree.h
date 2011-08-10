/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _MAPTREE_H
#define _MAPTREE_H

#include "BIH.h"

namespace VMAP
{
	class ModelInstance;
	class GroupModel;
	class VMapManager2;

	struct LocationInfo
	{
		LocationInfo(): hitInstance(0), hitModel(0), ground_Z(-G3D::inf()) {};
		const ModelInstance* hitInstance;
		const GroupModel* hitModel;
		float ground_Z;
	};

	class StaticMapTree
	{
		typedef std::tr1::unordered_map<G3D::uint32, bool> loadedTileMap;
		typedef std::tr1::unordered_map<G3D::uint32, G3D::uint32> loadedSpawnMap;

		private:
			G3D::uint32 iMapID;
			bool iIsTiled;
			BIH iTree;
			ModelInstance* iTreeValues; // the tree entries
			G3D::uint32 iNTreeValues;

			// Store all the map tile idents that are loaded for that map
			// some maps are not splitted into tiles and we have to make sure, not removing the map before all tiles are removed
			// empty tiles have no tile file, hence map with bool instead of just a set (consistency check)
			loadedTileMap iLoadedTiles;
			// stores <tree_index, reference_count> to invalidate tree values, unload map, and to be able to report errors
			loadedSpawnMap iLoadedSpawns;
			std::string iBasePath;

		private:
			bool getIntersectionTime(const G3D::Ray & pRay, float & pMaxDist, bool pStopAtFirstHit) const;
			//bool containsLoadedMapTile(unsigned int pTileIdent) const { return(iLoadedMapTiles.containsKey(pTileIdent)); }
		public:
			static std::string getTileFileName(G3D::uint32 mapID, G3D::uint32 tileX, G3D::uint32 tileY);
			static G3D::uint32 packTileID(G3D::uint32 tileX, G3D::uint32 tileY) { return tileX << 16 | tileY; }
			static void unpackTileID(G3D::uint32 ID, G3D::uint32 & tileX, G3D::uint32 & tileY) { tileX = ID >> 16; tileY = ID & 0xFF; }
			static bool CanLoadMap(const std::string & basePath, G3D::uint32 mapID, G3D::uint32 tileX, G3D::uint32 tileY);

			StaticMapTree(G3D::uint32 mapID, const std::string & basePath);
			~StaticMapTree();

			bool isInLineOfSight(const G3D::Vector3 & pos1, const G3D::Vector3 & pos2) const;
			bool getObjectHitPos(const G3D::Vector3 & pos1, const G3D::Vector3 & pos2, G3D::Vector3 & pResultHitPos, float pModifyDist) const;
			float getHeight(const G3D::Vector3 & pPos, float maxSearchDist) const;
			bool getAreaInfo(G3D::Vector3 & pos, G3D::uint32 & flags, G3D::int32 & adtId, G3D::int32 & rootId, G3D::int32 & groupId) const;
			bool GetLocationInfo(const G3D::Vector3 & pos, LocationInfo & info) const;

			bool InitMap(const std::string & fname, VMapManager2* vm);
			void UnloadMap(VMapManager2* vm);
			bool LoadMapTile(G3D::uint32 tileX, G3D::uint32 tileY, VMapManager2* vm);
			void UnloadMapTile(G3D::uint32 tileX, G3D::uint32 tileY, VMapManager2* vm);
			bool isTiled() const { return iIsTiled; }
			G3D::uint32 numLoadedTiles() const { return iLoadedTiles.size(); }

#ifdef MMAP_GENERATOR
		public:
			void getModelInstances(ModelInstance* &models, G3D::uint32 & count);
#endif
	};

	struct AreaInfo
	{
		AreaInfo(): result(false), ground_Z(-G3D::inf()) {};
		bool result;
		float ground_Z;
		G3D::uint32 flags;
		G3D::int32 adtId;
		G3D::int32 rootId;
		G3D::int32 groupId;
	};
}                                                           // VMAP

#endif // _MAPTREE_H

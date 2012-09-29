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

#include "MapTree.h"
#include "ModelInstance.h"
#include "VMapManager2.h"
#include "VMapDefinitions.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <limits>

namespace VMAP
{

	class MapRayCallback
	{
		public:
			MapRayCallback(ModelInstance* val): prims(val), hit(false) {}
			bool operator()(const G3D::Ray & ray, G3D::uint32 entry, float & distance, bool pStopAtFirstHit = true)
			{
				bool result = prims[entry].intersectRay(ray, distance, pStopAtFirstHit);
				if(result)
					hit = true;
				return result;
			}
			bool didHit() { return hit; }
		protected:
			ModelInstance* prims;
			bool hit;
	};

	class AreaInfoCallback
	{
		public:
			AreaInfoCallback(ModelInstance* val): prims(val) {}
			void operator()(const G3D::Vector3 & point, G3D::uint32 entry)
			{
#ifdef VMAP_DEBUG
				DEBUG_LOG("trying to intersect '%s'", prims[entry].name.c_str());
#endif
				prims[entry].intersectPoint(point, aInfo);
			}

			ModelInstance* prims;
			AreaInfo aInfo;
	};

	class LocationInfoCallback
	{
		public:
			LocationInfoCallback(ModelInstance* val, LocationInfo & info): prims(val), locInfo(info), result(false) {}
			void operator()(const G3D::Vector3 & point, G3D::uint32 entry)
			{
#ifdef VMAP_DEBUG
				DEBUG_LOG("trying to intersect '%s'", prims[entry].name.c_str());
#endif
				if(prims[entry].GetLocationInfo(point, locInfo))
					result = true;
			}

			ModelInstance* prims;
			LocationInfo & locInfo;
			bool result;
	};


	//=========================================================

	std::string StaticMapTree::getTileFileName(G3D::uint32 mapID, G3D::uint32 tileX, G3D::uint32 tileY)
	{
		std::stringstream tilefilename;
		tilefilename.fill('0');
		tilefilename << std::setw(3) << mapID << "_";
		//tilefilename << std::setw(2) << tileX << "_" << std::setw(2) << tileY << ".vmtile";
		tilefilename << std::setw(2) << tileY << "_" << std::setw(2) << tileX << ".vmtile";
		return tilefilename.str();
	}

	bool StaticMapTree::getAreaInfo(G3D::Vector3 & pos, G3D::uint32 & flags, G3D::int32 & adtId, G3D::int32 & rootId, G3D::int32 & groupId) const
	{
		AreaInfoCallback intersectionCallBack(iTreeValues);
		iTree.intersectPoint(pos, intersectionCallBack);
		if(intersectionCallBack.aInfo.result)
		{
			flags = intersectionCallBack.aInfo.flags;
			adtId = intersectionCallBack.aInfo.adtId;
			rootId = intersectionCallBack.aInfo.rootId;
			groupId = intersectionCallBack.aInfo.groupId;
			pos.z = intersectionCallBack.aInfo.ground_Z;
			return true;
		}
		return false;
	}

	bool StaticMapTree::GetLocationInfo(const G3D::Vector3 & pos, LocationInfo & info) const
	{
		LocationInfoCallback intersectionCallBack(iTreeValues, info);
		iTree.intersectPoint(pos, intersectionCallBack);
		return intersectionCallBack.result;
	}

	StaticMapTree::StaticMapTree(G3D::uint32 mapID, const std::string & basePath):
		iMapID(mapID), iTreeValues(0), iBasePath(basePath)
	{
		if(iBasePath.length() > 0 && (iBasePath[iBasePath.length() - 1] != '/' || iBasePath[iBasePath.length() - 1] != '\\'))
		{
			iBasePath.append("/");
		}
	}

	//=========================================================
	//! Make sure to call unloadMap() to unregister acquired model references before destroying
	StaticMapTree::~StaticMapTree()
	{
		delete[] iTreeValues;
	}

	//=========================================================
	/**
	If intersection is found within pMaxDist, sets pMaxDist to intersection distance and returns true.
	Else, pMaxDist is not modified and returns false;
	*/

	bool StaticMapTree::getIntersectionTime(const G3D::Ray & pRay, float & pMaxDist, bool pStopAtFirstHit) const
	{
		float distance = pMaxDist;
		MapRayCallback intersectionCallBack(iTreeValues);
		iTree.intersectRay(pRay, intersectionCallBack, distance, pStopAtFirstHit);
		if(intersectionCallBack.didHit())
			pMaxDist = distance;
		return intersectionCallBack.didHit();
	}
	//=========================================================

	bool StaticMapTree::isInLineOfSight(const G3D::Vector3 & pos1, const G3D::Vector3 & pos2) const
	{
		float maxDist = (pos2 - pos1).magnitude();
		// valid map coords should *never ever* produce float overflow, but this would produce NaNs too:
		(maxDist < std::numeric_limits<float>::max());
		// prevent NaN values which can cause BIH intersection to enter infinite loop
		if(maxDist < 1e-10f)
			return true;
		// direction with length of 1
		G3D::Ray ray = G3D::Ray::fromOriginAndDirection(pos1, (pos2 - pos1) / maxDist);
		if(getIntersectionTime(ray, maxDist, true))
			return false;

		return true;
	}
	//=========================================================
	/**
	When moving from pos1 to pos2 check if we hit an object. Return true and the position if we hit one
	Return the hit pos or the original dest pos
	*/

	bool StaticMapTree::getObjectHitPos(const G3D::Vector3 & pPos1, const G3D::Vector3 & pPos2, G3D::Vector3 & pResultHitPos, float pModifyDist) const
	{
		bool result = false;
		float maxDist = (pPos2 - pPos1).magnitude();
		// valid map coords should *never ever* produce float overflow, but this would produce NaNs too:
		
		if( maxDist > std::numeric_limits<float>::max() ){
			printf( "maxDist is out of range, aborting." );
			((void(*)())0)();
		}

		// prevent NaN values which can cause BIH intersection to enter infinite loop
		if(maxDist < 1e-10f)
		{
			pResultHitPos = pPos2;
			return false;
		}
		G3D::Vector3 dir = (pPos2 - pPos1) / maxDist;            // direction with length of 1
		G3D::Ray ray(pPos1, dir);
		float dist = maxDist;
		if(getIntersectionTime(ray, dist, false))
		{
			pResultHitPos = pPos1 + dir * dist;
			if(pModifyDist < 0)
			{
				if((pResultHitPos - pPos1).magnitude() > -pModifyDist)
				{
					pResultHitPos = pResultHitPos + dir * pModifyDist;
				}
				else
				{
					pResultHitPos = pPos1;
				}
			}
			else
			{
				pResultHitPos = pResultHitPos + dir * pModifyDist;
			}
			result = true;
		}
		else
		{
			pResultHitPos = pPos2;
			result = false;
		}
		return result;
	}

	//=========================================================

	float StaticMapTree::getHeight(const G3D::Vector3 & pPos, float maxSearchDist) const
	{
		float height = G3D::inf();
		G3D::Vector3 dir = G3D::Vector3(0, 0, -1);
		G3D::Ray ray(pPos, dir);   // direction with length of 1
		float maxDist = maxSearchDist;
		if(getIntersectionTime(ray, maxDist, false))
		{
			height = pPos.z - maxDist;
		}
		return(height);
	}

	//=========================================================

	bool StaticMapTree::CanLoadMap(const std::string & vmapPath, G3D::uint32 mapID, G3D::uint32 tileX, G3D::uint32 tileY)
	{
		std::string basePath = vmapPath;
		if(basePath.length() > 0 && (basePath[basePath.length() - 1] != '/' || basePath[basePath.length() - 1] != '\\'))
			basePath.append("/");
		std::string fullname = basePath + VMapManager2::getMapFileName(mapID);
		bool success = true;
		FILE* rf = fopen(fullname.c_str(), "rb");
		if(!rf)
			return false;
		// TODO: check magic number when implemented...
		char tiled;
		char chunk[8];
		if(!readChunk(rf, chunk, VMAP_MAGIC, 8) || fread(&tiled, sizeof(char), 1, rf) != 1)
		{
			fclose(rf);
			return false;
		}
		if(tiled)
		{
			std::string tilefile = basePath + getTileFileName(mapID, tileX, tileY);
			FILE* tf = fopen(tilefile.c_str(), "rb");
			if(!tf)
				success = false;
			else
			{
				if(!readChunk(tf, chunk, VMAP_MAGIC, 8))
					success = false;
				fclose(tf);
			}
		}
		fclose(rf);
		return success;
	}

	//=========================================================

	bool StaticMapTree::InitMap(const std::string & fname, VMapManager2* vm)
	{
		printf("Initializing StaticMapTree '%s'\n", fname.c_str());
		bool success = true;
		std::string fullname = iBasePath + fname;
		FILE* rf = fopen(fullname.c_str(), "rb");
		if(!rf)
			return false;
		else
		{
			char chunk[8];
			//general info
			if(!readChunk(rf, chunk, VMAP_MAGIC, 8)) success = false;
			char tiled;
			if(success && fread(&tiled, sizeof(char), 1, rf) != 1) success = false;
			iIsTiled = (tiled != 0);
			// Nodes
			if(success && !readChunk(rf, chunk, "NODE", 4)) success = false;
			if(success) success = iTree.readFromFile(rf);
			if(success)
			{
				iNTreeValues = iTree.primCount();
				iTreeValues = new ModelInstance[iNTreeValues];
			}

			if(success && !readChunk(rf, chunk, "GOBJ", 4)) success = false;
			// global model spawns
			// only non-tiled maps have them, and if so exactly one (so far at least...)
			ModelSpawn spawn;
#ifdef VMAP_DEBUG
			DEBUG_LOG("Map isTiled: %u", static_cast<G3D::uint32>(iIsTiled));
#endif
			if(!iIsTiled && ModelSpawn::readFromFile(rf, spawn))
			{
				WorldModel* model = vm->acquireModelInstance(iBasePath, spawn.name);
				printf("StaticMapTree::InitMap(): loading %s", spawn.name.c_str());
				if(model)
				{
					// assume that global model always is the first and only tree value (could be improved...)
					iTreeValues[0] = ModelInstance(spawn, model);
					iLoadedSpawns[0] = 1;
				}
				else
				{
					success = false;
					printf("StaticMapTree::InitMap() could not acquire WorldModel pointer for '%s'!", spawn.name.c_str());
				}
			}

			fclose(rf);
		}
		return success;
	}

	//=========================================================

	void StaticMapTree::UnloadMap(VMapManager2* vm)
	{
		for(loadedSpawnMap::iterator i = iLoadedSpawns.begin(); i != iLoadedSpawns.end(); ++i)
		{
			iTreeValues[i->first].setUnloaded();
			for(G3D::uint32 refCount = 0; refCount < i->second; ++refCount)
				vm->releaseModelInstance(iTreeValues[i->first].name);
		}
		iLoadedSpawns.clear();
		iLoadedTiles.clear();
	}

	//=========================================================

	bool StaticMapTree::LoadMapTile(G3D::uint32 tileX, G3D::uint32 tileY, VMapManager2* vm)
	{
		if(!iIsTiled)
		{
			// currently, core creates grids for all maps, whether it has terrain tiles or not
			// so we need "fake" tile loads to know when we can unload map geometry
			iLoadedTiles[packTileID(tileX, tileY)] = false;
			return true;
		}
		if(!iTreeValues)
		{
			printf("StaticMapTree::LoadMapTile(): Tree has not been initialized! [%u,%u]", tileX, tileY);
			return false;
		}
		bool result = true;

		std::string tilefile = iBasePath + getTileFileName(iMapID, tileX, tileY);
		FILE* tf = fopen(tilefile.c_str(), "rb");
		if(tf)
		{
			char chunk[8];
			if(!readChunk(tf, chunk, VMAP_MAGIC, 8))
				result = false;
			G3D::uint32 numSpawns;
			if(result && fread(&numSpawns, sizeof(G3D::uint32), 1, tf) != 1)
				result = false;
			for(G3D::uint32 i = 0; i < numSpawns && result; ++i)
			{
				// read model spawns
				ModelSpawn spawn;
				result = ModelSpawn::readFromFile(tf, spawn);
				if(result)
				{
					// acquire model instance
					WorldModel* model = vm->acquireModelInstance(iBasePath, spawn.name);
					if(!model)
						printf("StaticMapTree::LoadMapTile() could not acquire WorldModel pointer for '%s'!", spawn.name.c_str());

					// update tree
					G3D::uint32 referencedVal;

					fread(&referencedVal, sizeof(G3D::uint32), 1, tf);
					if(!iLoadedSpawns.count(referencedVal))
					{
#ifdef VMAP_DEBUG
						if(referencedVal > iNTreeValues)
						{
							DEBUG_LOG("invalid tree element! (%u/%u)", referencedVal, iNTreeValues);
							continue;
						}
#endif
						iTreeValues[referencedVal] = ModelInstance(spawn, model);
						iLoadedSpawns[referencedVal] = 1;
					}
					else
					{
						++iLoadedSpawns[referencedVal];
#ifdef VMAP_DEBUG
						if(iTreeValues[referencedVal].ID != spawn.ID)
							DEBUG_LOG("Error: trying to load wrong spawn in node!");
						else if(iTreeValues[referencedVal].name != spawn.name)
							DEBUG_LOG("Error: name mismatch on GUID=%u", spawn.ID);
#endif
					}
				}
			}
			iLoadedTiles[packTileID(tileX, tileY)] = true;
			fclose(tf);
		}
		else
			iLoadedTiles[packTileID(tileX, tileY)] = false;
		return result;
	}

	//=========================================================

	void StaticMapTree::UnloadMapTile(G3D::uint32 tileX, G3D::uint32 tileY, VMapManager2* vm)
	{
		G3D::uint32 tileID = packTileID(tileX, tileY);
		loadedTileMap::iterator tile = iLoadedTiles.find(tileID);
		if(tile == iLoadedTiles.end())
		{
			printf("StaticMapTree::UnloadMapTile(): Trying to unload non-loaded tile. Map:%u X:%u Y:%u", iMapID, tileX, tileY);
			return;
		}
		if(tile->second)  // file associated with tile
		{
			std::string tilefile = iBasePath + getTileFileName(iMapID, tileX, tileY);
			FILE* tf = fopen(tilefile.c_str(), "rb");
			if(tf)
			{
				bool result = true;
				char chunk[8];
				if(!readChunk(tf, chunk, VMAP_MAGIC, 8))
					result = false;
				G3D::uint32 numSpawns;
				if(fread(&numSpawns, sizeof(G3D::uint32), 1, tf) != 1)
					result = false;
				for(G3D::uint32 i = 0; i < numSpawns && result; ++i)
				{
					// read model spawns
					ModelSpawn spawn;
					result = ModelSpawn::readFromFile(tf, spawn);
					if(result)
					{
						// release model instance
						vm->releaseModelInstance(spawn.name);

						// update tree
						G3D::uint32 referencedNode;

						fread(&referencedNode, sizeof(G3D::uint32), 1, tf);
						if(!iLoadedSpawns.count(referencedNode))
						{
							printf("Trying to unload non-referenced model '%s' (ID:%u)", spawn.name.c_str(), spawn.ID);
						}
						else if(--iLoadedSpawns[referencedNode] == 0)
						{
							iTreeValues[referencedNode].setUnloaded();
							iLoadedSpawns.erase(referencedNode);
						}
					}
				}
				fclose(tf);
			}
		}
		iLoadedTiles.erase(tile);
	}

}

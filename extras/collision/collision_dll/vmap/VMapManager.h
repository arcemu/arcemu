/* 
 * Copyright (C) 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
 * Copyright (C) 2007-2008 Ascent Team <http://www.ascentemu.com/>
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

#ifndef _VMAPMANAGER_H
#define _VMAPMANAGER_H

// load our modified version first !!
#include "AABSPTree.h"
#include "ManagedModelContainer.h"
#include "IVMapManager.h"
#include "../LocationVector.h"

#include <G3D/Table.h>

//===========================================================

#define DIR_FILENAME_EXTENSION ".vmdir"
#define MAX_CAN_FALL_DISTANCE 10.0
#define FILENAMEBUFFER_SIZE 500

/**
This is the main Class to manage loading and unloading of maps, line of sight, height calculation and so on.
For each map or map tile to load it reads a directory file that contains the ModelContainer files used by this map or map tile.
Each global map or instance has its own dynamic BSP-Tree.
The loaded ModelContainers are included in one of these BSP-Trees.
Additionally a table to match map ids and map names is used.
*/

// Create a value describing the map tile
#define MAP_TILE_IDENT(x,y) ((x<<16) + y)
//===========================================================

namespace VMAP
{
    //===========================================================

    class FilesInDir
    {
        private:
            int iRefCount;
            Array<std::string> iFiles;
        public:

            FilesInDir() { iRefCount = 0; }
            void append(const std::string& pName) { iFiles.append(pName); }
            void incRefCount() { iRefCount++; }
            void decRefCount() { if(iRefCount > 0) iRefCount--; }
            int getRefCount() { return iRefCount; }
            const Array<std::string>& getFiles() const { return iFiles; }
    };

    //===========================================================
    //===========================================================
    //===========================================================
    //===========================================================

    class MapTree
    {
        private:
            AABSPTree<ModelContainer *> *iTree;

            // Key: filename, value ModelContainer
            Table<std::string, ManagedModelContainer *> iLoadedModelContainer;

            // Key: dir file name, value FilesInDir
            Table<std::string, FilesInDir> iLoadedDirFiles;

            // Store all the map tile idents that are loaded for that map
            // some maps are not splitted into tiles and we have to make sure, not removing the map before all tiles are removed
            Table<unsigned int, bool> iLoadedMapTiles;
            std::string iBasePath;

        private:
            float getIntersectionTime(const Ray& pRay, float pMaxDist, bool pStopAtFirstHit);
            bool isAlreadyLoaded(const std::string& pName) { return(iLoadedModelContainer.containsKey(pName)); }
            RayIntersectionIterator<AABSPTree<ModelContainer*>::Node, ModelContainer*> beginRayIntersection(const Ray& ray, bool skipAABoxTests = false) const;
            RayIntersectionIterator<AABSPTree<ModelContainer*>::Node, ModelContainer*> endRayIntersection() const;
            void setLoadedMapTile(unsigned int pTileIdent) { iLoadedMapTiles.set(pTileIdent, true); }
            void removeLoadedMapTile(unsigned int pTileIdent) { iLoadedMapTiles.remove(pTileIdent); }
            bool hasLoadedMapTiles() { return(iLoadedMapTiles.size() > 0); }
            bool containsLoadedMapTile(unsigned int pTileIdent) { return(iLoadedMapTiles.containsKey(pTileIdent)); }
        public:
            ManagedModelContainer *getModelContainer(const std::string& pName) { return(iLoadedModelContainer.get(pName)); }
            const bool hasDirFile(const std::string& pDirName) const { return(iLoadedDirFiles.containsKey(pDirName)); }
            FilesInDir& getDirFiles(const std::string& pDirName) const { return(iLoadedDirFiles.get(pDirName)); }
        public:
            MapTree(const char *pBasePath);
            ~MapTree();

            bool isInLineOfSight(const Vector3& pos1, const Vector3& pos2);
            bool getObjectHitPos(const Vector3& pos1, const Vector3& pos2, Vector3& pResultHitPos, float pModifyDist);
			bool isInDoors(const Vector3& pos);
			bool isOutDoors(const Vector3& pos);
            float getHeight(const Vector3& pPos);

            bool loadMap(const std::string& pDirFileName, unsigned int pMapTileIdent);
            void addModelConatiner(const std::string& pName, ManagedModelContainer *pMc);
            void unloadMap(const std::string& dirFileName, unsigned int pMapTileIdent);

            void getModelContainer(Array<ModelContainer *>& pArray ) { iTree->getMembers(pArray); }
            inline const void addDirFile(const std::string& pDirName, const FilesInDir& pFilesInDir) { iLoadedDirFiles.set(pDirName, pFilesInDir); }
            inline size_t size() { return(iTree->size()); }
    };

    //===========================================================
    class MapIdNames
    {
        public:
            std::string iDirName;
            std::string iMapGroupName;
    };

    //===========================================================
    class VMapManager : public IVMapManager
    {
        private:
            bool _loadMap(const char* pBasePath, unsigned int pMapId, int x, int y, bool pForceTileLoad=false);
            void _unloadMap(unsigned int  pMapId, int x, int y);

        public:
            // public for debug
            Vector3 convertPositionToInternalRep(float x, float y, float z) const;
            Vector3 convertPositionToMangosRep(float x, float y, float z) const;
			Vector3 convertPositionToInternalRep(LocationVector & vec) const;
			Vector3 convertPositionToInternalRepMod(LocationVector & vec) const;
			Vector3 convertPositionToMangosRep(LocationVector & vec) const;
			LocationVector convertPositionToMangosRep(Vector3 & src) const;
            std::string getDirFileName(unsigned int pMapId) const;
            std::string getDirFileName(unsigned int pMapId, int x, int y) const;
        public:
            VMapManager();
            ~VMapManager(void);

            int loadMap(const char* pBasePath, unsigned int pMapId, int x, int y);

            void unloadMap(unsigned int pMapId, int x, int y);
            void unloadMap(unsigned int pMapId);

			// LOS
			virtual bool isInLineOfSight(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2);
			virtual bool isInLineOfSight(unsigned int pMapId, LocationVector & v1, LocationVector & v2);

			// Height
			virtual float getHeight(unsigned int pMapId, float x, float y, float z);
			virtual float getHeight(unsigned int mapid, LocationVector & vec);

			// Indoors
			virtual bool isInDoors(unsigned int mapid, float x, float y, float z);
			virtual bool isInDoors(unsigned int mapid, LocationVector & vec);

			// Outdoors
			virtual bool isOutDoors(unsigned int mapid, float x, float y, float z);
			virtual bool isOutDoors(unsigned int mapid, LocationVector & vec);

			/**
			test if we hit an object. return true if we hit one. rx,ry,rz will hold the hit position or the dest position, if no intersection was found
			return a position, that is pReduceDist closer to the origin
			*/

			// Closest Point
			virtual bool getObjectHitPos(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float &ry, float& rz, float pModifyDist);
			virtual bool getObjectHitPos(unsigned int pMapId, LocationVector & v1, LocationVector & v2, LocationVector & vout, float pModifyDist);
    };
}
#endif

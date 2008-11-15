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

#ifndef _IVMAPMANAGER_H
#define _IVMAPMANAGER_H

#include<string>
class LocationVector;
//===========================================================

/**
This is the minimum interface to the VMapMamager.
*/

namespace VMAP
{

    enum VMAP_LOAD_RESULT
    {
        VMAP_LOAD_RESULT_ERROR,
        VMAP_LOAD_RESULT_OK,
        VMAP_LOAD_RESULT_IGNORED,
    };

    #define VMAP_INVALID_HEIGHT -100000.0f

    //===========================================================
    class IVMapManager
    {
        public:
            IVMapManager() {}

            virtual ~IVMapManager(void) {}

            virtual int loadMap(const char* pBasePath, unsigned int pMapId, int x, int y) = 0;

            virtual void unloadMap(unsigned int pMapId, int x, int y) = 0;
            virtual void unloadMap(unsigned int pMapId) = 0;

			// LOS
            virtual bool isInLineOfSight(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2) = 0;
			virtual bool isInLineOfSight(unsigned int pMapId, LocationVector & v1, LocationVector & v2) = 0;

			// Height
            virtual float getHeight(unsigned int pMapId, float x, float y, float z) = 0;
			virtual float getHeight(unsigned int mapid, LocationVector & vec) = 0;

			// Indoors
			virtual bool isInDoors(unsigned int mapid, float x, float y, float z) = 0;
			virtual bool isInDoors(unsigned int mapid, LocationVector & vec) = 0;

			// Outdoors
			virtual bool isOutDoors(unsigned int mapid, float x, float y, float z) = 0;
			virtual bool isOutDoors(unsigned int mapid, LocationVector & vec) = 0;

            /**
            test if we hit an object. return true if we hit one. rx,ry,rz will hold the hit position or the dest position, if no intersection was found
            return a position, that is pReduceDist closer to the origin
            */

			// Closest Point
            virtual bool getObjectHitPos(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float &ry, float& rz, float pModifyDist) = 0;
			virtual bool getObjectHitPos(unsigned int pMapId, LocationVector & v1, LocationVector & v2, LocationVector & vout, float pModifyDist) = 0;


            virtual std::string getDirFileName(unsigned int pMapId, int x, int y) const =0;
    };

}
#endif

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

#include "VMapManager.h"

#if defined(WIN32)
#define snprintf _snprintf
#endif

#define MAX_MAPS 600

namespace VMAP
{
	inline bool IsTileMap(unsigned int mapid)
	{
		switch( mapid )
		{
		case 509:
		case 469:
		case 189:
		case 30:
		case 37:
		case 33:
		case 533:
		case 209:
		case 309:
		case 560:
		case 534:
		case 532:
		case 543:
		case 568:
		case 564:
		case 0:
		case 1:
		case 530:
			return true;
			break;

		default:
			return false;
			break;
		}
	}

	static MapTree * m_maps[MAX_MAPS];

    //=========================================================

    VMapManager::VMapManager()
    {
		memset( m_maps, 0, sizeof(MapTree*) * MAX_MAPS );
    }

    //=========================================================

    VMapManager::~VMapManager(void)
    {
		for(unsigned int i = 0; i < MAX_MAPS; ++i)
		{
			if( m_maps[i] != NULL )
				delete m_maps[i];

			m_maps[i] = NULL;
		}
    }

    //=========================================================

    inline Vector3 VMapManager::convertPositionToInternalRep(float x, float y, float z) const
    {
        float pos[3];
        pos[0] = y;
        pos[1] = z;
        pos[2] = x;
        double full = 64.0*533.33333333;
        double mid = full/2.0;
        pos[0] = full- (pos[0] + mid);
        pos[2] = full- (pos[2] + mid);

        return(Vector3(pos));
    }

    //=========================================================

    inline Vector3 VMapManager::convertPositionToMangosRep(float x, float y, float z) const
    {
        float pos[3];
        pos[0] = z;
        pos[1] = x;
        pos[2] = y;
        double full = 64.0*533.33333333;
        double mid = full/2.0;
        pos[0] = -((mid+pos[0])-full);
        pos[1] = -((mid+pos[1])-full);

        return(Vector3(pos));
    }
    //=========================================================
	//=========================================================

	inline Vector3 VMapManager::convertPositionToInternalRep(LocationVector & vec) const
	{
		float pos[3];
		pos[0] = vec.y;
		pos[1] = vec.z;
		pos[2] = vec.x;
		double full = 64.0*533.33333333;
		double mid = full/2.0;
		pos[0] = full- (pos[0] + mid);
		pos[2] = full- (pos[2] + mid);

		return(Vector3(pos));
	}

	inline Vector3 VMapManager::convertPositionToInternalRepMod(LocationVector & vec) const
	{
		float pos[3];
		pos[0] = vec.y;
		pos[1] = vec.z + 2.0f;
		pos[2] = vec.x;
		double full = 64.0*533.33333333;
		double mid = full/2.0;
		pos[0] = full- (pos[0] + mid);
		pos[2] = full- (pos[2] + mid);

		return(Vector3(pos));
	}


	//=========================================================

	inline Vector3 VMapManager::convertPositionToMangosRep(LocationVector & vec) const
	{
		float pos[3];
		pos[0] = vec.y;
		pos[1] = vec.z;
		pos[2] = vec.x;
		double full = 64.0*533.33333333;
		double mid = full/2.0;
		pos[0] = -((mid+pos[0])-full);
		pos[1] = -((mid+pos[1])-full);

		return(Vector3(pos));
	}

	inline LocationVector VMapManager::convertPositionToMangosRep(Vector3 & src) const
	{
		float pos[3];
		pos[0] = src.z;
		pos[1] = src.x;
		pos[2] = src.y;
		double full = 64.0*533.33333333;
		double mid = full/2.0;
		pos[0] = -((mid+pos[0])-full);
		pos[1] = -((mid+pos[1])-full);

		return LocationVector(pos[0], pos[1], pos[2]);
	}
	//=========================================================

    std::string VMapManager::getDirFileName(unsigned int pMapId, int x, int y) const
    {
        char name[FILENAMEBUFFER_SIZE];

        snprintf(name, FILENAMEBUFFER_SIZE, "%03u_%d_%d%s",pMapId, x, y, DIR_FILENAME_EXTENSION);
        return(std::string(name));
    }

    //=========================================================
    std::string VMapManager::getDirFileName(unsigned int pMapId) const
    {
        char name[FILENAMEBUFFER_SIZE];

        snprintf(name, FILENAMEBUFFER_SIZE, "%03d%s",pMapId, DIR_FILENAME_EXTENSION);
        return(std::string(name));
    }
    //=========================================================
    // remote last return or LF
    void chomp(std::string& str)
    {
        while(str.length() >0)
        {
            char lc = str[str.length()-1];
            if(lc == '\r' || lc == '\n')
            {
                str = str.substr(0,str.length()-1);
            }
            else
            {
                break;
            }
        }
    }
    //=========================================================

    void chompAndTrim(std::string& str)
    {
        while(str.length() >0)
        {
            char lc = str[str.length()-1];
            if(lc == '\r' || lc == '\n' || lc == ' ' || lc == '"' || lc == '\'')
            {
                str = str.substr(0,str.length()-1);
            }
            else
            {
                break;
            }
        }
        while(str.length() >0)
        {
            char lc = str[0];
            if(lc == ' ' || lc == '"' || lc == '\'')
            {
                str = str.substr(1,str.length()-1);
            }
            else
            {
                break;
            }
        }
    }

    //=========================================================

    int VMapManager::loadMap(const char* pBasePath, unsigned int pMapId, int x, int y)
    {
		bool result = false;
		std::string dirFileName;
		if( pMapId >= MAX_MAPS )
			return false;

		if( IsTileMap( pMapId ) )
			dirFileName = getDirFileName( pMapId, x, y );
		else
			dirFileName = getDirFileName( pMapId );

		MapTree* instanceTree = m_maps[pMapId];
		if( instanceTree == NULL )
		{
			instanceTree = new MapTree( pBasePath );
			m_maps[pMapId] = instanceTree;
		}

		unsigned int mapTileIdent = MAP_TILE_IDENT(x,y);
		result = instanceTree->loadMap(dirFileName, mapTileIdent);
		if(!result)                                         // remove on fail
		{
			if(instanceTree->size() == 0)
			{
				m_maps[pMapId] = NULL;
				delete instanceTree;
			}
		}
		return(result);
    }


    //=========================================================

    void VMapManager::unloadMap(unsigned int pMapId, int x, int y)
    {
		if( m_maps[pMapId] != NULL )
		{
			MapTree * instanceTree = m_maps[pMapId];
			std::string dirFileName;

			if( IsTileMap( pMapId ) )
				dirFileName = getDirFileName( pMapId, x, y );
			else
				dirFileName = getDirFileName( pMapId );

			unsigned int mapTileIdent = MAP_TILE_IDENT(x,y);
			instanceTree->unloadMap(dirFileName, mapTileIdent);
			if(instanceTree->size() == 0)
			{
				m_maps[ pMapId ] = NULL;
				delete instanceTree;
			}
		}
    }

    //=========================================================

    void VMapManager::unloadMap(unsigned int pMapId)
    {
		if( m_maps[pMapId] != NULL )
        {
            MapTree* instanceTree = m_maps[ pMapId ];
            std::string dirFileName = getDirFileName(pMapId);
            instanceTree->unloadMap(dirFileName, 0);
            if(instanceTree->size() == 0)
            {
				m_maps[pMapId]=NULL;
                delete instanceTree;
            }
        }
    }
    //==========================================================

    bool VMapManager::isInLineOfSight(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2)
    {
        bool result = true;
		if( m_maps[pMapId] != NULL )
        {
            Vector3 pos1 = convertPositionToInternalRep(x1,y1,z1);
            Vector3 pos2 = convertPositionToInternalRep(x2,y2,z2);
            if(pos1 != pos2)
            {
                MapTree* mapTree = m_maps[pMapId];
                result = mapTree->isInLineOfSight(pos1, pos2);
            }
        }
        return(result);
    }

	bool VMapManager::isInLineOfSight(unsigned int pMapId, LocationVector & v1, LocationVector & v2)
	{
		bool result = true;
		if( m_maps[pMapId] != NULL )
		{
			Vector3 pos1 = convertPositionToInternalRepMod(v1);
			Vector3 pos2 = convertPositionToInternalRepMod(v2);
			if(pos1 != pos2)
			{
				MapTree* mapTree = m_maps[pMapId];
				result = mapTree->isInLineOfSight(pos1, pos2);
			}
		}
		return(result);
	}

	bool VMapManager::isInDoors(unsigned int mapid, float x, float y, float z)
	{
		bool result = false;
		if( m_maps[mapid] != NULL )
		{
			Vector3 pos = convertPositionToInternalRep(x,y,z);
			MapTree* mapTree = m_maps[mapid];
			result = mapTree->isInDoors(pos);
		}
		return(result);
	}

	bool VMapManager::isInDoors(unsigned int mapid, LocationVector & vec)
	{
		bool result = false;
		if( m_maps[mapid] != NULL )
		{
			Vector3 pos = convertPositionToInternalRepMod(vec);
			MapTree* mapTree = m_maps[mapid];
			result = mapTree->isInDoors(pos);
		}
		return(result);
	}

	bool VMapManager::isOutDoors(unsigned int mapid, float x, float y, float z)
	{
		bool result = false;
		if( m_maps[mapid] != NULL )
		{
			Vector3 pos = convertPositionToInternalRep(x,y,z);
			MapTree* mapTree = m_maps[mapid];
			result = mapTree->isOutDoors(pos);
		}
		return(result);
	}

	bool VMapManager::isOutDoors(unsigned int mapid, LocationVector & vec)
	{
		bool result = false;
		if( m_maps[mapid] != NULL )
		{
			Vector3 pos = convertPositionToInternalRepMod(vec);
			MapTree* mapTree = m_maps[mapid];
			result = mapTree->isOutDoors(pos);
		}
		return(result);
	}


    //=========================================================
    /**
    get the hit position and return true if we hit something
    otherwise the result pos will be the dest pos
    */
    bool VMapManager::getObjectHitPos(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float &ry, float& rz, float pModifyDist)
    {
        bool result = false;
        rx=x2;
        ry=y2;
        rz=z2;
		if( m_maps[pMapId] != NULL )
		{
                Vector3 pos1 = convertPositionToInternalRep(x1,y1,z1);
                Vector3 pos2 = convertPositionToInternalRep(x2,y2,z2);
                Vector3 resultPos;
                MapTree* mapTree = m_maps[pMapId];
                result = mapTree->getObjectHitPos(pos1, pos2, resultPos, pModifyDist);
                resultPos = convertPositionToMangosRep(resultPos.x,resultPos.y,resultPos.z);
                rx = resultPos.x;
                ry = resultPos.y;
                rz = resultPos.z;
        }
        return result;
    }

	bool VMapManager::getObjectHitPos(unsigned int pMapId, LocationVector & v1, LocationVector & v2, LocationVector & vout, float pModifyDist)
	{
		bool result = false;
		vout = v2;
		if( m_maps[pMapId] != NULL )
		{
			Vector3 pos1 = convertPositionToInternalRepMod(v1);
			Vector3 pos2 = convertPositionToInternalRepMod(v2);
			Vector3 resultPos;
			MapTree* mapTree = m_maps[pMapId];
			result = mapTree->getObjectHitPos(pos1, pos2, resultPos, pModifyDist);
			vout = convertPositionToMangosRep(resultPos);
		}
		return result;
	}

    //=========================================================
    /**
    get height or INVALID_HEIGHT if to hight was calculated
    */

    //int gGetHeightCounter = 0;
    float VMapManager::getHeight(unsigned int pMapId, float x, float y, float z)
    {
        float height = VMAP_INVALID_HEIGHT;                 //no height
		if( m_maps[pMapId] != NULL )
		{
            Vector3 pos = convertPositionToInternalRep(x,y,z);
            height = m_maps[pMapId]->getHeight(pos);
            if(!(height < inf()))
            {
				return VMAP_INVALID_HEIGHT;
            }
        }
        return(height);
    }
	float VMapManager::getHeight(unsigned int mapid, LocationVector & vec)
	{
		float height = VMAP_INVALID_HEIGHT;                 //no height
		if( m_maps[mapid] != NULL )
		{
			Vector3 pos = convertPositionToInternalRepMod(vec);
			height = m_maps[mapid]->getHeight(pos);
			if(!(height < inf()))
			{
				return VMAP_INVALID_HEIGHT;
			}
		}
		return(height);
	}


    //=========================================================
    //=========================================================
    //=========================================================

    MapTree::MapTree(const char* pBaseDir)
    {
        iBasePath = std::string(pBaseDir);
        if(iBasePath.length() > 0 && (iBasePath[iBasePath.length()-1] != '/' || iBasePath[iBasePath.length()-1] != '\\'))
        {
            iBasePath.append("/");
        }
        iTree = new AABSPTree<ModelContainer *>();
    }

    //=========================================================
    MapTree::~MapTree()
    {
        Array<ModelContainer *> mcArray;
        iTree->getMembers(mcArray);
        int no = mcArray.size();
        while(no >0)
        {
            --no;
            delete mcArray[no];
        }
        delete iTree;
    }
    //=========================================================

    // just for visual debugging with an external debug class
    #ifdef _DEBUG_VMAPS
    #ifndef gBoxArray
    extern Vector3 p1,p2,p3,p4,p5,p6,p7;
    extern Array<AABox>gBoxArray;
    extern int gCount1, gCount2, gCount3, gCount4;
    extern bool myfound;
    #endif
    #endif

    typedef AABSPTree<ModelContainer*>::RayIntersectionIterator IT;
    //=========================================================
    /**
    return dist to hit or inf() if no hit
    */

    float MapTree::getIntersectionTime(const Ray& pRay, float pMaxDist, bool pStopAtFirstHit)
    {
        double  firstDistance = inf();

        const IT end = iTree->endRayIntersection();
        IT obj = iTree->beginRayIntersection(pRay, pMaxDist);

        for ( ;obj != end; ++obj)                           // (preincrement is *much* faster than postincrement!)
        {
            /*
            Call your accurate intersection test here.  It is guaranteed
            that the ray hits the bounding box of obj.  (*obj) has type T,
            so you can call methods directly using the "->" operator.
            */
            ModelContainer *model = (ModelContainer *) (*obj);

            float t = model->getIntersectionTime(pRay, pStopAtFirstHit, pMaxDist);

            // just for visual debugging with an external debug class
            #ifdef _DEBUG_VMAPS
            if(gCount3 == gCount1)
            {
                AABox testBox;
                testBox = model->getAABoxBounds();
                gBoxArray.append(testBox);
            }
            ++gCount3;
            #endif
            if(t > 0 && t < inf())
            {
                obj.markBreakNode();
                if(firstDistance > t && pMaxDist >= t)
                {
                    firstDistance = t;
                    if(pStopAtFirstHit) break;
                }
            }
        }
        return firstDistance;
    }
    //=========================================================

    bool MapTree::isInLineOfSight(const Vector3& pos1, const Vector3& pos2)
    {
        bool result = true;
        float maxDist = abs((pos2 - pos1).magnitude());
                                                            // direction with length of 1
        Ray ray = Ray::fromOriginAndDirection(pos1, (pos2 - pos1)/maxDist);
        if(getIntersectionTime(ray, maxDist, true) < inf())
        {
            result = false;
        }
        return result;
    }
    //=========================================================
    /**
    When moving from pos1 to pos2 check if we hit an object. Return true and the position if we hit one
    Return the hit pos or the original dest pos
    */

    bool MapTree::getObjectHitPos(const Vector3& pPos1, const Vector3& pPos2, Vector3& pResultHitPos, float pModifyDist)
    {
        bool result;
        float maxDist = abs((pPos2 - pPos1).magnitude());
        Vector3 dir = (pPos2 - pPos1)/maxDist;              // direction with length of 1
        Ray ray = Ray::fromOriginAndDirection(pPos1, dir);
        float dist = getIntersectionTime(ray, maxDist, false);
        if(dist < inf())
        {
            pResultHitPos = pPos1 + dir * dist;
            if(pModifyDist < 0)
            {
                if(abs((pResultHitPos - pPos1).magnitude()) > -pModifyDist)
                {
                    pResultHitPos = pResultHitPos + dir*pModifyDist;
                }
                else
                {
                    pResultHitPos = pPos1;
                }
            }
            else
            {
                pResultHitPos = pResultHitPos + dir*pModifyDist;
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

    float MapTree::getHeight(const Vector3& pPos)
    {
        float height = inf();
        Vector3 dir = Vector3(0,-1,0);
        Ray ray = Ray::fromOriginAndDirection(pPos, dir);   // direction with length of 1
        float dist = getIntersectionTime(ray, MAX_CAN_FALL_DISTANCE, false);
        if(dist < inf())
        {
            height = (pPos + dir * dist).y;
        }
        return(height);
    }

	//=========================================================
	bool MapTree::isInDoors(const Vector3& pos)
	{
		Vector3 dir = Vector3(0,-1,0);
		Ray ray = Ray::fromOriginAndDirection(pos, dir);   // direction with length of 1
		unsigned int flags;

		AABSPTree<ModelContainer*>::RayIntersectionIterator MITR;
		AABSPTree<ModelContainer*>::RayIntersectionIterator MITREND;
		RayIntersectionIterator<TreeNode, SubModel> SMITR;
		RayIntersectionIterator<TreeNode, SubModel> SMITREND;
		ModelContainer * mc;
		SubModel * sm;

		MITR = iTree->beginRayIntersection( ray, MAX_CAN_FALL_DISTANCE );
		MITREND = iTree->endRayIntersection();

		for(; MITR != MITREND; ++MITR)
		{
			mc = ((ModelContainer*)*MITR);
			
			Ray relativeRay = Ray::fromOriginAndDirection(ray.origin - mc->getBasePosition(), ray.direction);
			SMITR = mc->beginRayIntersection( relativeRay, MAX_CAN_FALL_DISTANCE );
			SMITREND = mc->endRayIntersection();

			for( ; SMITR != SMITREND; ++SMITR )
			{
				sm = ((SubModel*)&(*SMITR));
#if 0
				if( sm->getIndoorFlag() != 0 )
				{
					unsigned int i = sm->getIndoorFlag();
					printf("=========================================================\n");
					printf("Model indoor flags: %u\n", sm->getIndoorFlag());
					unsigned int z, b;
					for(z = 1, b = 1; b < 32;)
					{
						if(i & z)
							printf("   Bit %u (0x%.8X or %u) is set!\n", b, z, z);

						z <<= 1;
						b+=1;
					}
					printf("=========================================================\n");
				}
#else
				flags = sm->getIndoorFlag();
				if( flags != 0 )
				{
					/* From WoWdev:
					 Flag 	 Meaning
					0x1 	Always set
					0x4 	Has vertex colors (MOCV chunk)
					0x8 	Outdoor
					0x200 	Has lights (MOLR chunk)
					0x800 	Has doodads (MODR chunk)
					0x1000 	Has water (MLIQ chunk)
					0x2000 	Indoor
					0x40000 	Show skybox

					**********************

					0x8000 seems to be set in the areas in citys (while it has the indoor flag, its not
					an indoor area

					*/

					if( flags & 0x2000 && !(flags & 0x8000) )
						return true;
				}
#endif
			}
		}

		return false;
	}

	bool MapTree::isOutDoors(const Vector3& pos)
	{
		Vector3 dir = Vector3(0,-1,0);
		Ray ray = Ray::fromOriginAndDirection(pos, dir);   // direction with length of 1
		unsigned int flags;

		AABSPTree<ModelContainer*>::RayIntersectionIterator MITR;
		AABSPTree<ModelContainer*>::RayIntersectionIterator MITREND;
		RayIntersectionIterator<TreeNode, SubModel> SMITR;
		RayIntersectionIterator<TreeNode, SubModel> SMITREND;
		ModelContainer * mc;
		SubModel * sm;

		MITR = iTree->beginRayIntersection( ray, MAX_CAN_FALL_DISTANCE );
		MITREND = iTree->endRayIntersection();

		for(; MITR != MITREND; ++MITR)
		{
			mc = ((ModelContainer*)*MITR);
			
			Ray relativeRay = Ray::fromOriginAndDirection(ray.origin - mc->getBasePosition(), ray.direction);
			SMITR = mc->beginRayIntersection( relativeRay, MAX_CAN_FALL_DISTANCE );
			SMITREND = mc->endRayIntersection();

			for( ; SMITR != SMITREND; ++SMITR )
			{
				sm = ((SubModel*)&(*SMITR));
#if 0
				if( sm->getIndoorFlag() != 0 )
				{
					unsigned int i = sm->getIndoorFlag();
					printf("=========================================================\n");
					printf("Model indoor flags: %u\n", sm->getIndoorFlag());
					unsigned int z, b;
					for(z = 1, b = 1; b < 32;)
					{
						if(i & z)
							printf("   Bit %u (0x%.8X or %u) is set!\n", b, z, z);

						z <<= 1;
						b+=1;
					}
					printf("=========================================================\n");
				}
#else
				flags = sm->getIndoorFlag();
				if( flags != 0 )
				{
					/* From WoWdev:
					 Flag 	 Meaning
					0x1 	Always set
					0x4 	Has vertex colors (MOCV chunk)
					0x8 	Outdoor
					0x200 	Has lights (MOLR chunk)
					0x800 	Has doodads (MODR chunk)
					0x1000 	Has water (MLIQ chunk)
					0x2000 	Indoor
					0x40000 	Show skybox

					**********************

					0x8000 seems to be set in the areas in citys (while it has the indoor flag, its not
					an indoor area

					*/

					if( !(flags & 0x8) )
						return false;
				}
#endif
			}
		}

		return true;
	}

    //=========================================================

    bool MapTree::loadMap(const std::string& pDirFileName, unsigned int pMapTileIdent)
    {
        bool result = true;
        size_t len = iBasePath.length() + pDirFileName.length();
        char *filenameBuffer = new char[len+1];
        if(!hasDirFile(pDirFileName))
        {
            FilesInDir filesInDir;
            result = false;
            sprintf(filenameBuffer, "%s%s", iBasePath.c_str(), pDirFileName.c_str());
            FILE* df = fopen(filenameBuffer, "rb");
            if(df)
            {
                char lineBuffer[FILENAMEBUFFER_SIZE];
                result = true;
                bool newModelLoaded = false;
                while(result && (fgets(lineBuffer, FILENAMEBUFFER_SIZE-1, df) != 0))
                {
                    std::string name = std::string(lineBuffer);
                    chomp(name);
                    if(name.length() >1)
                    {
                        filesInDir.append(name);
                        ManagedModelContainer *mc;
                        if(!isAlreadyLoaded(name))
                        {
                            std::string fname = iBasePath;
                            fname.append(name);
                            mc = new ManagedModelContainer();
                            result = mc->readFile(fname.c_str());
                            if(result)
                            {
                                addModelConatiner(name, mc);
                                newModelLoaded = true;
                            }
							else
								delete mc;
                        }
                        else
                        {
                            mc = getModelContainer(name);
                        }
                        mc->incRefCount();
                    }
                }
                if(result && newModelLoaded)
                {
                    iTree->balance();
                }
                if(result && ferror(df) != 0)
                {
                    result = false;
                }
                fclose(df);
                if(result)
                {
                    filesInDir.incRefCount();
                    addDirFile(pDirFileName, filesInDir);
                    setLoadedMapTile(pMapTileIdent);
                }
            }
        }
        else
        {
            // Already loaded, so just inc. the ref count if mapTileIdent is new
            if(!containsLoadedMapTile(pMapTileIdent))
            {
                setLoadedMapTile(pMapTileIdent);
                FilesInDir& filesInDir = getDirFiles(pDirFileName);
                filesInDir.incRefCount();
            }
        }
        delete [] filenameBuffer;
        return (result);
    }

    //=========================================================

    void MapTree::unloadMap(const std::string& dirFileName, unsigned int pMapTileIdent)
    {
        if(hasDirFile(dirFileName) && containsLoadedMapTile(pMapTileIdent))
        {
			if(containsLoadedMapTile(pMapTileIdent))
			   removeLoadedMapTile(pMapTileIdent);
            FilesInDir& filesInDir = getDirFiles(dirFileName);
            filesInDir.decRefCount();
            if(filesInDir.getRefCount() <= 0)
            {
                Array<std::string> fileNames = filesInDir.getFiles();
                bool treeChanged = false;
                for(int i=0; i<fileNames.size(); ++i)
                {
                    std::string name = fileNames[i];
                    ManagedModelContainer* mc = getModelContainer(name);
                    mc->decRefCount();
                    if(mc->getRefCount() <= 0)
                    {
                        iLoadedModelContainer.remove(name);
                        iTree->remove(mc);
                        delete mc;
                        treeChanged = true;
                    }
                }
                iLoadedDirFiles.remove(dirFileName);
                if(treeChanged)
                {
                    iTree->balance();
                }
            }
        }
    }

    //=========================================================
    //=========================================================

    void MapTree::addModelConatiner(const std::string& pName, ManagedModelContainer *pMc)
    {
        iLoadedModelContainer.set(pName, pMc);
        iTree->insert(pMc);
    }
    //=========================================================
    //=========================================================
    //=========================================================
}

/* 
 * Copyright (C) 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
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

#ifndef _TILEASSEMBLER_H_
#define _TILEASSEMBLER_H_

// load our modified version first !!
#include "AABSPTree.h"

#include <G3D/Vector3.h>

#include "CoordModelMapping.h"
#include "SubModel.h"
#include "ModelContainer.h"

namespace VMAP
{
    /**
    This Class is used to convert raw vector data into balanced BSP-Trees.
    To start the conversion call convertWorld().
    */
    //===============================================

    class ModelPosition
    {
        private:
            Matrix3 ixMatrix;
            Matrix3 iyMatrix;
            Matrix3 izMatrix;
        public:
            Vector3 iPos;
            Vector3 iDir;
            float iScale;
            void init()
            {

                // Swap x and y the raw data uses the axis differently
                ixMatrix = Matrix3::fromAxisAngle(Vector3::UNIT_Y,-(G3D_PI*iDir.x/180.0));
                iyMatrix = Matrix3::fromAxisAngle(Vector3::UNIT_X,-(G3D_PI*iDir.y/180.0));
                izMatrix = Matrix3::fromAxisAngle(Vector3::UNIT_Z,-(G3D_PI*iDir.z/180.0));

            }
            Vector3 transform(const Vector3& pIn) const;
            void moveToBasePos(const Vector3& pBasePos) { iPos -= pBasePos; }
    };
    //===============================================

    class TileAssembler
    {
        private:
            CoordModelMapping *iCoordModelMapping;
            std::string iDestDir;
            std::string iSrcDir;
            bool (*iFilterMethod)(char *pName);
            Table<std::string, unsigned int > iUniqueNameIds;
            unsigned int iCurrentUniqueNameId;

        public:
            TileAssembler(const std::string& pSrcDirName, const std::string& pDestDirName);
            virtual ~TileAssembler();

            bool fillModelContainerArray(const std::string& pDirFileName, unsigned int pMapId, int pXPos, int pYPos, Array<ModelContainer*>& pMC);
            ModelContainer* processNames(const Array<std::string>& pPosFileNames, const char* pDestFileName);

            void init();
            bool convertWorld();

            bool fillModelIntoTree(AABSPTree<SubModel *> *pMainTree, const Vector3& pBasePos, std::string& pPosFilename, std::string& pModelFilename);
            void getModelPosition(std::string& pPosString, ModelPosition& pModelPosition);
            bool readRawFile(std::string& pModelFilename,  ModelPosition& pModelPosition, AABSPTree<SubModel *> *pMainTree);
            void addWorldAreaMapId(unsigned int pMapId) { iCoordModelMapping->addWorldAreaMap(pMapId); }
            void setModelNameFilterMethod(bool (*pFilterMethod)(char *pName)) { iFilterMethod = pFilterMethod; }
            std::string getDirEntryNameFromModName(unsigned int pMapId, const std::string& pModPosName);
            unsigned int getUniqueNameId(const std::string pName);
    };
    //===============================================
}                                                           // VMAP
#endif                                                      /*_TILEASSEMBLER_H_*/

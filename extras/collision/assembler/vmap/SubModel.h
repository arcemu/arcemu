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

#ifndef _SUBMODEL_H
#define _SUBMODEL_H

// load our modified version first !!
#include "AABSPTree.h"

#include "ShortVector.h"
#include "ShortBox.h"
#include "TreeNode.h"
#include "RayIntersectionIterator.h"
#include "BaseModel.h"

namespace VMAP
{
    /**
    This is a balanced static BSP-Tree of triangles.
    The memory for the tree nodes and the triangles are managed by the ModelContainer.
    The exception to this is during the conversion of raw data info balanced BSP-Trees.
    During this conversion the memory management is done internally.
    */
    class SubModel : public BaseModel
    {
        private:
            unsigned int iNodesPos;
            unsigned int iTrianglesPos;
            bool iHasInternalMemAlloc;
			unsigned int iIsIndoor;
            ShortBox iBox;
        #ifdef _DEBUG_VIEW
            Array<TriangleBox *> iDrawBox;
        #endif
        public:
            SubModel() : BaseModel(){ };

            SubModel(unsigned int pNTriangles, TriangleBox *pTriangles, unsigned int pTrianglesPos, unsigned int pNNodes, TreeNode *pTreeNodes, unsigned int pNodesPos);
            SubModel(AABSPTree<Triangle> *pTree);
            ~SubModel(void);
            //Gets a 50 byte binary block
            void initFromBinBlock(void *pBinBlock);

            RayIntersectionIterator<TreeNode, TriangleBox> beginRayIntersection(const Ray& ray, double pMaxTime, bool skipAABoxTests = false) const;

            RayIntersectionIterator<TreeNode, TriangleBox> endRayIntersection() const;

            void fillRenderArray(Array<TriangleBox> &pArray, const TreeNode* pTreeNode);

            RealTime getIntersectionTime(const Ray& pRay, bool pExitAtFirst, float pMaxDist) const;

            void countNodesAndTriangles(AABSPTree<Triangle>::Node& pNode, int &pNNodes, int &pNTriabgles);

            void fillContainer(const AABSPTree<Triangle>::Node& pNode, int &pTreeNodePos, int &pTrianglePos, Vector3& pLo, Vector3& pHi);

            inline const ShortBox& getReletiveBounds() const { return(iBox); }

            inline void setReletiveBounds(const ShortVector& lo, const ShortVector& hi) { iBox.setLo(lo); iBox.setHi(hi); }

            inline const AABox getAABoxBounds() const { return(AABox(iBox.getLo().getVector3() + getBasePosition(), iBox.getHi().getVector3()+ getBasePosition())); }

            // get start pos bases on the global array
            inline TriangleBox const* getTriangles() const { return &BaseModel::getTriangle(iTrianglesPos); }
            inline TriangleBox      * getTriangles()       { return &BaseModel::getTriangle(iTrianglesPos); }

            // get start pos bases on the global array
            inline TreeNode const* getTreeNodes() const { return &BaseModel::getTreeNode(iNodesPos); }
            inline TreeNode      * getTreeNodes()       { return &BaseModel::getTreeNode(iNodesPos); }

            // internal method usign internal offset
            inline const TreeNode& getTreeNode(int pPos) const { return(SubModel::getTreeNodes()[pPos]); }

            // internal method usign internal offset
            inline const TriangleBox& getTriangle(int pPos) const { return(SubModel::getTriangles()[pPos]); }

            inline unsigned int getNodesPos() const { return(iNodesPos); }
            inline unsigned int getTrianglesPos() const { return(iTrianglesPos); }

			inline void setIndoorFlag(unsigned int val) { iIsIndoor = val; }
			inline const unsigned int getIndoorFlag() const { return iIsIndoor; }
			bool WriteToFile(FILE * pFile);
    };

    unsigned int hashCode(const SubModel& pSm);
    bool operator==(const SubModel& pSm1, const SubModel& pSm2);
    void getBounds(const SubModel& pSm, G3D::AABox& pAABox);
    void getBounds(const SubModel* pSm, G3D::AABox& pAABox);
    //====================================
}                                                           // VMAP
#endif

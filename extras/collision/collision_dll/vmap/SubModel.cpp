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

#include "SubModel.h"

#ifdef _ASSEMBLER_DEBUG
extern FILE *::g_df;
#endif

namespace VMAP
{

    //==========================================================
    /**
    Functions to use ModelContainer with a AABSPTree
    */
    size_t hashCode(const SubModel& pSm)
    {
        return pSm.getNTriangles();
    }

    bool operator==(const SubModel& pSm1, const SubModel& pSm2)
    {
        bool result = false;

        if(pSm1.getNNodes() == pSm2.getNNodes() &&
            pSm1.getNTriangles() == pSm2.getNTriangles() &&
            pSm1.getBasePosition() == pSm2.getBasePosition() &&
            pSm1.getNodesPos() == pSm2.getNodesPos() &&
            pSm1.getTrianglesPos() == pSm2.getTrianglesPos())
        {
            result = true;
        }
        return result;
    }

    void getBounds(const SubModel& pSm, G3D::AABox& pAABox)
    {
        ShortBox box = pSm.getReletiveBounds();
        pAABox.set(box.getLo().getVector3()+pSm.getBasePosition(), box.getHi().getVector3()+pSm.getBasePosition());
    }

    void getBounds(const SubModel* pSm, G3D::AABox& pAABox)
    {
        ShortBox box = pSm->getReletiveBounds();
        pAABox.set(box.getLo().getVector3()+pSm->getBasePosition(), box.getHi().getVector3()+pSm->getBasePosition());
    }

    //==========================================================
    //==========================================================
    //==========================================================
    //==========================================================
    SubModel::SubModel(unsigned int pNTriangles, TriangleBox *pTriangles, unsigned int pTrianglesPos, unsigned int pNNodes, TreeNode *pTreeNodes, unsigned int pNodesPos) :
    BaseModel(pNNodes, pTreeNodes, pNTriangles, pTriangles)
    {
        iTrianglesPos = pTrianglesPos;
        iNodesPos = pNodesPos;
		iIsIndoor = 0;
        iHasInternalMemAlloc = false;
    }

    //==========================================================

    SubModel::~SubModel(void)
    {
        if(iHasInternalMemAlloc)
        {
            free();
        }
    }

    //==========================================================

	enum BIN_POSITIONS
	{
		BP_iNTriangles=0,
		BP_iNNodes=4,
		BP_iBasePosition=8,
		BP_iNodesPos=20,
		BP_iTrianglesPos=24,
		BP_iHasInternalMemAlloc=28,
		BP_iIsIndoor=29,
		BP_iBox=33,
	};
	/**
	This is ugly, but due to compatibility and 64 bit support we have to do that ... sorry
	*/
	void SubModel::initFromBinBlock(void *pBinBlock)
	{
		iNTriangles =  *((unsigned int *)(((char *) pBinBlock) + BP_iNTriangles));
		iNNodes =  *((unsigned int *) (((char *) pBinBlock) + BP_iNNodes));
		iBasePosition =  *((Vector3 *) (((char *) pBinBlock) + BP_iBasePosition));
		iNodesPos =  *((unsigned int *) (((char *) pBinBlock) + BP_iNodesPos));
		iTrianglesPos =  *((unsigned int *) (((char *) pBinBlock) + BP_iTrianglesPos));
		iHasInternalMemAlloc = *((bool *) (((char *) pBinBlock) + BP_iHasInternalMemAlloc));
		iIsIndoor = *((unsigned int*) (((char *) pBinBlock) + BP_iIsIndoor));
		iBox =  *((ShortBox *) (((char *) pBinBlock) + BP_iBox));
	}

    //==========================================================

    void SubModel::countNodesAndTriangles(AABSPTree<Triangle>::Node& pNode, int &pNNodes, int &pNTriabgles)
    {
        pNNodes++;
        pNTriabgles += pNode.valueArray.size();

        #ifdef _ASSEMBLER_DEBUG
        fprintf(::g_df, "Nodes: %d, Tris: %d\n",pNNodes, pNTriabgles);
        #endif

        if(pNode.child[0] != 0)
        {
            countNodesAndTriangles(*pNode.child[0], pNNodes, pNTriabgles);
        }
        if(pNode.child[1] != 0)
        {
            countNodesAndTriangles(*pNode.child[1], pNNodes, pNTriabgles);
        }
    }

    //==========================================================

    void SubModel::fillContainer(const AABSPTree<Triangle>::Node& pNode, int &pTreeNodePos, int &pTrianglePos, Vector3& pLo, Vector3& pHi)
    {
        TreeNode treeNode = TreeNode(pNode.valueArray.size(), pTrianglePos);
        treeNode.setSplitAxis(pNode.splitAxis);
        treeNode.setSplitLocation(pNode.splitLocation);

        int currentTreeNodePos = pTreeNodePos++;

        Vector3 lo = Vector3(inf(),inf(),inf());
        Vector3 hi = Vector3(-inf(),-inf(),-inf());

        for(int i=0;i<pNode.valueArray.size(); i++)
        {
            AABSPTree<Triangle>::Handle h= pNode.valueArray[i];
            Triangle t = h.value;
            TriangleBox triangleBox = TriangleBox(t.vertex(0),t.vertex(1), t.vertex(2));
            lo = lo.min(triangleBox.getBounds().getLo().getVector3());
            hi = hi.max(triangleBox.getBounds().getHi().getVector3());

            getTriangles()[pTrianglePos++] = triangleBox;
        }

        if(pNode.child[0] != 0)
        {
            treeNode.setChildPos(0, pTreeNodePos);
            fillContainer(*pNode.child[0], pTreeNodePos, pTrianglePos, lo, hi);
        }
        if(pNode.child[1] != 0)
        {
            treeNode.setChildPos(1, pTreeNodePos);
            fillContainer(*pNode.child[1], pTreeNodePos, pTrianglePos, lo, hi);
        }

        treeNode.setBounds(lo,hi);

        // get absolute bounds
        pLo = pLo.min(lo);
        pHi = pHi.max(hi);

        getTreeNodes()[currentTreeNodePos] = treeNode;
    }

    //==========================================================

    SubModel::SubModel(AABSPTree<Triangle> *pTree)
    {
        int nNodes, nTriangles;
        nNodes = nTriangles = 0;
        countNodesAndTriangles(*pTree->root, nNodes, nTriangles);

        init(nNodes, nTriangles);

        iTrianglesPos = 0;                                  // this is the global array
        iNodesPos = 0;                                      // this is the global array
        iHasInternalMemAlloc = true;
		iIsIndoor = 0;
        int treeNodePos, trianglePos;
        treeNodePos = trianglePos = 0;

        Vector3 lo = Vector3(inf(),inf(),inf());
        Vector3 hi = Vector3(-inf(),-inf(),-inf());

        fillContainer(*pTree->root, treeNodePos, trianglePos, lo, hi);
        setReletiveBounds(lo, hi);
    }

    //==========================================================

    RayIntersectionIterator<TreeNode, TriangleBox> SubModel::beginRayIntersection(const Ray& ray, double pMaxTime, bool skipAABoxTests) const
    {
        NodeValueAccess<TreeNode, TriangleBox> vna = NodeValueAccess<TreeNode, TriangleBox>(getTreeNodes(), getTriangles());
        return RayIntersectionIterator<TreeNode, TriangleBox>(vna, ray, &getTreeNode(0), pMaxTime, skipAABoxTests);
    }

    RayIntersectionIterator<TreeNode, TriangleBox> SubModel::endRayIntersection() const
    {
        return RayIntersectionIterator<TreeNode, TriangleBox>();
    }

    //==========================================================================

    typedef RayIntersectionIterator<TreeNode, TriangleBox> IT;

    inline RealTime testIntersectionWithTriangle(const IT& pIterater,const Triangle& pTriangle, const Ray& pRay)
    {
        static const double epsilon = 0.00001;

        RealTime t = pRay.intersectionTime(pTriangle);
        /*
        Often methods like "distanceUntilIntersection" can be made more
        efficient by providing them with the time at which to start and
        to give up looking for an intersection; that is,
        obj.minDistance and iMin(firstDistance, obj.maxDistance).
        */
        if ((t >= inf()) ||
            (t > pIterater.maxDistance + epsilon) ||
            (t < pIterater.minDistance - epsilon))
        {
            t = inf();
        }
        return(t);
    }
    //==========================================================
    #ifdef _DEBUG_VMAPS
    #ifndef gBoxArray
    extern Vector3 p1,p2,p3,p4,p5,p6,p7;
    extern Array<AABox>gBoxArray;
    extern int gCount1, gCount2, gCount3, gCount4;
    extern bool myfound;
    #endif
    #endif

    RealTime SubModel::getIntersectionTime(const Ray& pRay, bool pExitAtFirst, float pMaxDist) const
    {
        TriangleBox const *firstObject;
        double  firstDistance = inf();

        #ifdef _DEBUG_VMAPS
        int debugCount =0;
        #endif
        Ray relativeRay = Ray::fromOriginAndDirection(pRay.origin - getBasePosition(), pRay.direction);

        const IT end = endRayIntersection();
        IT obj = beginRayIntersection(relativeRay,pMaxDist,false);

        Triangle testT;
        for ( ;obj != end; ++obj)                           // (preincrement is *much* faster than postincrement!)
        {
            /*
            Call your accurate intersection test here.  It is guaranteed
            that the ray hits the bounding box of obj.  (*obj) has type T,
            so you can call methods directly using the "->" operator.
            */
            const TriangleBox *tri = &(*obj);

            testT = Triangle(tri->vertex(0).getVector3(),tri->vertex(1).getVector3(),tri->vertex(2).getVector3());
            double t = testIntersectionWithTriangle(obj,testT, relativeRay);
            #ifdef _DEBUG_VMAPS
            if(debugCount == 5)
            {
                firstObject = tri;
                firstDistance = 1;
            }
            ++debugCount;
            #endif
            if(t != inf())
            {
                /*
                Tell the iterator that we've found at least one
                intersection.  It will finish looking at all
                objects in this node and then terminate.
                */
                obj.markBreakNode();
                if(firstDistance > t && pMaxDist >= t)
                {
                    firstDistance = t;
                    firstObject   = tri;
                    if(pExitAtFirst) break;
                }
            }
            else
            {
                // This might the wrong side of the triangle... Turn it and test again
                testT = Triangle(tri->vertex(2).getVector3(),tri->vertex(1).getVector3(),tri->vertex(0).getVector3());
                t = testIntersectionWithTriangle(obj, testT,relativeRay);
                if(t != inf())
                {
                    obj.markBreakNode();
                    if(firstDistance > t && pMaxDist >= t)
                    {
                        firstDistance = t;
                        firstObject   = tri;
                        if(pExitAtFirst) break;
                    }
                }
            }
        }
        #ifdef _DEBUG_VMAPS
        if(firstDistance < inf())
        {
            myfound = true;
            p1 = firstObject->vertex(0).getVector3()+ getBasePosition();
            p2 = firstObject->vertex(1).getVector3()+ getBasePosition();
            p3 = firstObject->vertex(2).getVector3()+ getBasePosition();
            p4 = relativeRay.origin + getBasePosition();
            p5 =  relativeRay.intersection(testT.plane()) + getBasePosition();
            float dist1 = (p5-p4).magnitude();
            double dist2 = relativeRay.intersectionTime(testT);
            float dist3 = relativeRay.direction.magnitude();
            double dist4 = relativeRay.intersectionTime(testT);
        }
        #endif

        return(firstDistance);
    }

    //==========================================================

}

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

#ifndef _BIH_H
#define _BIH_H

#include <G3D/Vector3.h>
#include <G3D/Ray.h>
#include <G3D/AABox.h>


#include <stdexcept>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

#ifdef WIN32
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

#define MAX_STACK_SIZE 64

#ifdef _MSC_VER
#define isnan(x) _isnan(x)
#else
#define isnan(x) std::isnan(x)
#endif

static inline G3D::uint32 floatToRawIntBits(float f)
{
	union
	{
		G3D::uint32 ival;
		float fval;
	} temp;
	temp.fval = f;
	return temp.ival;
}

static inline float intBitsToFloat(G3D::uint32 i)
{
	union
	{
		G3D::uint32 ival;
		float fval;
	} temp;
	temp.ival = i;
	return temp.fval;
}

struct AABound
{
	G3D::Vector3 lo, hi;
};

/** Bounding Interval Hierarchy Class.
    Building and Ray-Intersection functions based on BIH from
    Sunflow, a Java Raytracer, released under MIT/X11 License
    http://sunflow.sourceforge.net/
    Copyright (c) 2003-2007 Christopher Kulla
*/

class BIH
{
	public:
		BIH() {};
		template< class T, class BoundsFunc >
		void build(const std::vector<T> &primitives, BoundsFunc & getBounds, G3D::uint32 leafSize = 3, bool printStats = false)
		{
			if(primitives.size() == 0)
				return;
			buildData dat;
			dat.maxPrims = leafSize;
			dat.numPrims = primitives.size();
			dat.indices = new G3D::uint32[dat.numPrims];
			dat.primBound = new G3D::AABox[dat.numPrims];
			getBounds(primitives[0], bounds);
			for(G3D::uint32 i = 0; i < dat.numPrims; ++i)
			{
				dat.indices[i] = i;
				G3D::AABox tb;
				getBounds(primitives[i], dat.primBound[i]);
				bounds.merge(dat.primBound[i]);
			}
			std::vector<G3D::uint32> tempTree;
			BuildStats stats;
			buildHierarchy(tempTree, dat, stats);
			if(printStats)
				stats.printStats();

			objects.resize(dat.numPrims);
			for(G3D::uint32 i = 0; i < dat.numPrims; ++i)
				objects[i] = dat.indices[i];
			//nObjects = dat.numPrims;
			tree = tempTree;
			delete[] dat.primBound;
			delete[] dat.indices;
		}
		G3D::uint32 primCount() { return objects.size(); }

		template<typename RayCallback>
		void intersectRay(const G3D::Ray & r, RayCallback & intersectCallback, float & maxDist, bool stopAtFirst = false) const
		{
			float intervalMin = -1.f;
			float intervalMax = -1.f;
			G3D::Vector3 org = r.origin();
			G3D::Vector3 dir = r.direction();
			G3D::Vector3 invDir;
			for(int i = 0; i < 3; ++i)
			{
				invDir[i] = 1.f / dir[i];
				if(dir[i] != 0.f)
				{
					float t1 = (bounds.low()[i]  - org[i]) * invDir[i];
					float t2 = (bounds.high()[i] - org[i]) * invDir[i];
					if(t1 > t2)
						std::swap(t1, t2);
					if(t1 > intervalMin)
						intervalMin = t1;
					if(t2 < intervalMax || intervalMax < 0.f)
						intervalMax = t2;
					// intervalMax can only become smaller for other axis,
					//  and intervalMin only larger respectively, so stop early
					if(intervalMax <= 0 || intervalMin >= maxDist)
						return;
				}
			}

			if(intervalMin > intervalMax)
				return;
			intervalMin = std::max(intervalMin, 0.f);
			intervalMax = std::min(intervalMax, maxDist);

			G3D::uint32 offsetFront[3];
			G3D::uint32 offsetBack[3];
			G3D::uint32 offsetFront3[3];
			G3D::uint32 offsetBack3[3];
			// compute custom offsets from direction sign bit

			for(int i = 0; i < 3; ++i)
			{
				offsetFront[i] = floatToRawIntBits(dir[i]) >> 31;
				offsetBack[i] = offsetFront[i] ^ 1;
				offsetFront3[i] = offsetFront[i] * 3;
				offsetBack3[i] = offsetBack[i] * 3;

				// avoid always adding 1 during the inner loop
				++offsetFront[i];
				++offsetBack[i];
			}

			StackNode stack[MAX_STACK_SIZE];
			int stackPos = 0;
			int node = 0;

			while(true)
			{
				while(true)
				{
					G3D::uint32 tn = tree[node];
					G3D::uint32 axis = (tn & (3 << 30)) >> 30;
					bool BVH2 = (tn & (1 << 29)) != 0;
					int offset = tn & ~(7 << 29);
					if(!BVH2)
					{
						if(axis < 3)
						{
							// "normal" interior node
							float tf = (intBitsToFloat(tree[node + offsetFront[axis]]) - org[axis]) * invDir[axis];
							float tb = (intBitsToFloat(tree[node + offsetBack[axis]]) - org[axis]) * invDir[axis];
							// ray passes between clip zones
							if(tf < intervalMin && tb > intervalMax)
								break;
							int back = offset + offsetBack3[axis];
							node = back;
							// ray passes through far node only
							if(tf < intervalMin)
							{
								intervalMin = (tb >= intervalMin) ? tb : intervalMin;
								continue;
							}
							node = offset + offsetFront3[axis]; // front
							// ray passes through near node only
							if(tb > intervalMax)
							{
								intervalMax = (tf <= intervalMax) ? tf : intervalMax;
								continue;
							}
							// ray passes through both nodes
							// push back node
							stack[stackPos].node = back;
							stack[stackPos].tnear = (tb >= intervalMin) ? tb : intervalMin;
							stack[stackPos].tfar = intervalMax;
							stackPos++;
							// update ray interval for front node
							intervalMax = (tf <= intervalMax) ? tf : intervalMax;
							continue;
						}
						else
						{
							// leaf - test some objects
							int n = tree[node + 1];
							while(n > 0)
							{
								bool hit = intersectCallback(r, objects[offset], maxDist, stopAtFirst);
								if(stopAtFirst && hit) return;
								--n;
								++offset;
							}
							break;
						}
					}
					else
					{
						if(axis > 2)
							return; // should not happen
						float tf = (intBitsToFloat(tree[node + offsetFront[axis]]) - org[axis]) * invDir[axis];
						float tb = (intBitsToFloat(tree[node + offsetBack[axis]]) - org[axis]) * invDir[axis];
						node = offset;
						intervalMin = (tf >= intervalMin) ? tf : intervalMin;
						intervalMax = (tb <= intervalMax) ? tb : intervalMax;
						if(intervalMin > intervalMax)
							break;
						continue;
					}
				} // traversal loop
				do
				{
					// stack is empty?
					if(stackPos == 0)
						return;
					// move back up the stack
					stackPos--;
					intervalMin = stack[stackPos].tnear;
					if(maxDist < intervalMin)
						continue;
					node = stack[stackPos].node;
					intervalMax = stack[stackPos].tfar;
					break;
				}
				while(true);
			}
		}

		template<typename IsectCallback>
		void intersectPoint(const G3D::Vector3 & p, IsectCallback & intersectCallback) const
		{
			if(!bounds.contains(p))
				return;

			StackNode stack[MAX_STACK_SIZE];
			int stackPos = 0;
			int node = 0;

			while(true)
			{
				while(true)
				{
					G3D::uint32 tn = tree[node];
					G3D::uint32 axis = (tn & (3 << 30)) >> 30;
					bool BVH2 = (tn & (1 << 29)) != 0;
					int offset = tn & ~(7 << 29);
					if(!BVH2)
					{
						if(axis < 3)
						{
							// "normal" interior node
							float tl = intBitsToFloat(tree[node + 1]);
							float tr = intBitsToFloat(tree[node + 2]);
							// point is between clip zones
							if(tl < p[axis] && tr > p[axis])
								break;
							int right = offset + 3;
							node = right;
							// point is in right node only
							if(tl < p[axis])
							{
								continue;
							}
							node = offset; // left
							// point is in left node only
							if(tr > p[axis])
							{
								continue;
							}
							// point is in both nodes
							// push back right node
							stack[stackPos].node = right;
							stackPos++;
							continue;
						}
						else
						{
							// leaf - test some objects
							int n = tree[node + 1];
							while(n > 0)
							{
								intersectCallback(p, objects[offset]); // !!!
								--n;
								++offset;
							}
							break;
						}
					}
					else // BVH2 node (empty space cut off left and right)
					{
						if(axis > 2)
							return; // should not happen
						float tl = intBitsToFloat(tree[node + 1]);
						float tr = intBitsToFloat(tree[node + 2]);
						node = offset;
						if(tl > p[axis] || tr < p[axis])
							break;
						continue;
					}
				} // traversal loop

				// stack is empty?
				if(stackPos == 0)
					return;
				// move back up the stack
				stackPos--;
				node = stack[stackPos].node;
			}
		}

		bool writeToFile(FILE* wf) const;
		bool readFromFile(FILE* rf);

	protected:
		std::vector<G3D::uint32> tree;
		std::vector<G3D::uint32> objects;
		G3D::AABox bounds;

		struct buildData
		{
			G3D::uint32* indices;
			G3D::AABox* primBound;
			G3D::uint32 numPrims;
			int maxPrims;
		};
		struct StackNode
		{
			G3D::uint32 node;
			float tnear;
			float tfar;
		};

		class BuildStats
		{
			private:
				int numNodes;
				int numLeaves;
				int sumObjects;
				int minObjects;
				int maxObjects;
				int sumDepth;
				int minDepth;
				int maxDepth;
				int numLeavesN[6];
				int numBVH2;

			public:
				BuildStats():
					numNodes(0), numLeaves(0), sumObjects(0), minObjects(0x0FFFFFFF),
					maxObjects(0xFFFFFFFF), sumDepth(0), minDepth(0x0FFFFFFF),
					maxDepth(0xFFFFFFFF), numBVH2(0)
				{
					for(int i = 0; i < 6; ++i) numLeavesN[i] = 0;
				}

				void updateInner() { numNodes++; }
				void updateBVH2() { numBVH2++; }
				void updateLeaf(int depth, int n);
				void printStats();
		};

		void buildHierarchy(std::vector<G3D::uint32> &tempTree, buildData & dat, BuildStats & stats);

		void createNode(std::vector<G3D::uint32> &tempTree, int nodeIndex, G3D::uint32 left, G3D::uint32 right)
		{
			// write leaf node
			tempTree[nodeIndex + 0] = (3 << 30) | left;
			tempTree[nodeIndex + 1] = right - left + 1;
		}

		void subdivide(int left, int right, std::vector<G3D::uint32> &tempTree, buildData & dat, AABound & gridBox, AABound & nodeBox, int nodeIndex, int depth, BuildStats & stats);
};

#endif // _BIH_H

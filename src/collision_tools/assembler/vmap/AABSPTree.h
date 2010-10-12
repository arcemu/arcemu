/**
@file AABSPTree.h

@maintainer Morgan McGuire, matrix@graphics3d.com

@created 2004-01-11
@edited  2005-02-24

Copyright 2000-2005, Morgan McGuire.
All rights reserved.

*/

#ifndef G3D_AABSPTREE_H
#define G3D_AABSPTREE_H

#include "G3D/platform.h"
#include "G3D/Array.h"
#include "G3D/Table.h"
#include "G3D/Vector3.h"
#include "G3D/AABox.h"
#include "G3D/Sphere.h"
#include "G3D/Box.h"
#include "G3D/Triangle.h"
#include "G3D/GCamera.h"
//#include "G3D/BinaryInput.h"
//#include "G3D/BinaryOutput.h"
#include "G3D/CollisionDetection.h"
#include <algorithm>

#include "RayIntersectionIterator.h"

#ifdef _ASSEMBLER_DEBUG
#ifndef g_df
extern FILE *g_df;
#endif
#endif


inline void getBounds(const G3D::Vector3& v, G3D::AABox& out) {
    out = G3D::AABox(v);
}


inline void getBounds(const G3D::AABox& a, G3D::AABox& out) {
    out = a;
}


inline void getBounds(const G3D::Sphere& s, G3D::AABox& out) {
    s.getBounds(out);
}


inline void getBounds(const G3D::Box& b, G3D::AABox& out) {
    b.getBounds(out);
}


inline void getBounds(const G3D::Triangle& t, G3D::AABox& out) {
    t.getBounds(out);
}

namespace G3D {

    /**
    A set that supports spatial queries using an axis-aligned
    BSP tree for speed.

    AABSPTree is as powerful as but more general than a Quad Tree, Oct Tree, or KD Tree,
    but less general than an unconstrained BSP tree (which is much slower
    to create).

    Internally, objects
    are arranged into an axis-aligned BSP-tree according to their 
    axis-aligned bounds.  This increases the cost of insertion to
    O(log n) but allows fast overlap queries.

    <B>Template Parameters</B>
    <DT>The template parameter <I>T</I> must be one for which
    the following functions are overloaded:

    <P><CODE>void ::getBounds(const T&, G3D::AABox&);</CODE>
    <DT><CODE>bool operator==(const T&, const T&);</CODE>
    <DT><CODE>unsigned int ::hashCode(const T&);</CODE>
    <DT><CODE>T::T();</CODE> <I>(public constructor of no arguments)</I>

    <B>Moving %Set Members</B>
    <DT>It is important that objects do not move without updating the
    AABSPTree.  If the axis-aligned bounds of an object are about
    to change, AABSPTree::remove it before they change and 
    AABSPTree::insert it again afterward.  For objects
    where the hashCode and == operator are invariant with respect 
    to the 3D position,
    you can use the AABSPTree::update method as a shortcut to
    insert/remove an object in one step after it has moved.


    Note: Do not mutate any value once it has been inserted into AABSPTree. Values
    are copied interally. All AABSPTree iterators convert to pointers to constant
    values to reinforce this.

    If you want to mutate the objects you intend to store in a AABSPTree simply
    insert <I>pointers</I> to your objects instead of the objects themselves, and ensure
    that the above operations are defined. (And actually, because values are
    copied, if your values are large you may want to insert pointers anyway, to
    save space and make the balance operation faster.)

    <B>Dimensions</B>
    Although designed as a 3D-data structure, you can use the AABSPTree
    for data distributed along 2 or 1 axes by simply returning bounds
    that are always zero along one or more dimensions.

    */
    template<class T> class AABSPTree {
    private:
    public:
        /** Wrapper for a value that includes a cache of its bounds. */
        class Handle {
        public:
            T                   value;

            /** The bounds of each object are constrained to AABox::maxFinite */
            AABox               bounds;

            /** Center of bounds.  We cache this value to avoid recomputing it
            during the median sort, and because MSVC 6 std::sort goes into
            an infinite loop if we compute the midpoint on the fly (possibly
            a floating point roundoff issue, where B<A and A<B both are true).*/
            Vector3             center;

            Handle() {}

            inline Handle(const T& v) : value(v) {
                getBounds(v, bounds);
                bounds = bounds.intersect(AABox::maxFinite());
                center = bounds.center();
            }
        };

        /** Returns the bounds of the sub array. Used by makeNode. */
        static AABox computeBounds(
            const Array<Handle>&  point, 
            int                   beginIndex,
            int                   endIndex) {

                Vector3 lo = Vector3::inf();
                Vector3 hi = -lo;

                for (int p = beginIndex; p <= endIndex; ++p) {
                    lo = lo.min(point[p].bounds.low());
                    hi = hi.max(point[p].bounds.high());
                }

                return AABox(lo, hi);
            }


            /**
            A sort predicate that returns true if the midpoint of the
            first argument is less than the midpoint of the second
            along the specified axis.

            Used by makeNode.
            */
            class CenterLT {
            public:
                Vector3::Axis           sortAxis;

                CenterLT(Vector3::Axis a) : sortAxis(a) {}

                inline bool operator()(const Handle& a, const Handle& b) const {
                    return a.center[sortAxis] < b.center[sortAxis];
                }
            };


            /**
            A sort predicate based on a box's location on the specified axis. Each
            box is given a value -1, 0, or 1 based on whether it is strictly less
            than, overlapping, or strictly greater than the value on the specified
            axis. This predicate compares the values for two boxes. The result is
            that the array is separated into three contiguous (though possibly empty)
            groups: less than, overlapping, or greater than.
            */
            class PivotLT {
            public:
                Vector3::Axis sortAxis;
                float sortLocation;

                PivotLT(Vector3::Axis a, float l) : sortAxis(a), sortLocation(l) {}

                inline int location(const AABox& box) const {
                    if(box.low()[sortAxis] < sortLocation) {
                        if(box.high()[sortAxis] < sortLocation) {
                            return -1;
                        } else {
                            return 0;
                        }
                    } else if(box.low()[sortAxis] > sortLocation) {
                        return 1;
                    } else {
                        return 0;
                    }
                }

                inline bool operator()(const Handle&a, const Handle& b) const {
                    const AABox& A = a.bounds;
                    const AABox& B = b.bounds;

                    return location(A) < location(B);
                }
            };

            class Node {
            public:

                /** Spatial bounds on all values at this node and its children, based purely on
                the parent's splitting planes.  May be infinite */
                AABox               splitBounds;

                Vector3::Axis       splitAxis;

                /** Location along the specified axis */
                float               splitLocation;

                /** child[0] contains all values strictly 
                smaller than splitLocation along splitAxis.

                child[1] contains all values strictly
                larger.

                Both may be NULL if there are not enough
                values to bother recursing.
                */
                Node*               child[2];

                /** Array of values at this node (i.e. values
                straddling the split plane + all values if
                this is a leaf node). */
                Array<Handle>       valueArray;

                /** Creates node with NULL children */
                Node() {
                    splitAxis     = Vector3::X_AXIS;
                    splitLocation = 0;
                    splitBounds   = AABox(-Vector3::inf(), Vector3::inf());
                    for (int i = 0; i < 2; ++i) {
                        child[i] = NULL;
                    }
                }

                /**
                Doesn't clone children.
                */
                Node(const Node& other) : valueArray(other.valueArray) {
                    splitAxis       = other.splitAxis;
                    splitLocation   = other.splitLocation;
                    splitBounds     = other.splitBounds;            
                    for (int i = 0; i < 2; ++i) {
                        child[i] = NULL;
                    }
                }

                /** Copies the specified subarray of pt into point, NULLs the children.
                Assumes a second pass will set splitBounds. */
                Node(const Array<Handle>& pt, int beginIndex, int endIndex) {
                    splitAxis     = Vector3::X_AXIS;
                    splitLocation = 0;
                    for (int i = 0; i < 2; ++i) {
                        child[i] = NULL;
                    }

                    int n = endIndex - beginIndex + 1;

                    valueArray.resize(n);
                    for (int i = n - 1; i >= 0; --i) {
                        valueArray[i] = pt[i + beginIndex];
                    }
                }


                /** Deletes the children (but not the values) */
                ~Node() {
                    for (int i = 0; i < 2; ++i) {
                        delete child[i];
                    }
                }


                /** Returns true if this node is a leaf (no children) */
                inline bool isLeaf() const {
                    return (child[0] == NULL) && (child[1] == NULL);
                }


                /**
                Recursively appends all handles and children's handles
                to the array.
                */
                void getHandles(Array<Handle>& handleArray) const {
                    handleArray.append(valueArray);
                    for (int i = 0; i < 2; ++i) {
                        if (child[i] != NULL) {
                            child[i]->getHandles(handleArray);
                        }
                    }
                }


                void verifyNode(const Vector3& lo, const Vector3& hi) {
                    //		debugPrintf("Verifying: split %d @ %f [%f, %f, %f], [%f, %f, %f]\n",
                    //			    splitAxis, splitLocation, lo.x, lo.y, lo.z, hi.x, hi.y, hi.z);

                    debugAssert(lo == splitBounds.low());
                    debugAssert(hi == splitBounds.high());

                    for (int i = 0; i < valueArray.length(); ++i) {
                        const AABox& b = valueArray[i].bounds;

                        for(int axis = 0; axis < 3; ++axis) {
                            debugAssert(b.low()[axis] <= b.high()[axis]);
                            debugAssert(b.low()[axis] >= lo[axis]);
                            debugAssert(b.high()[axis] <= hi[axis]);
                        }
                    }

                    if (child[0] || child[1]) {
                        debugAssert(lo[splitAxis] < splitLocation);
                        debugAssert(hi[splitAxis] > splitLocation);
                    }

                    Vector3 newLo = lo;
                    newLo[splitAxis] = splitLocation;
                    Vector3 newHi = hi;
                    newHi[splitAxis] = splitLocation;

                    if (child[0] != NULL) {
                        child[0]->verifyNode(lo, newHi);
                    }

                    if (child[1] != NULL) {
                        child[1]->verifyNode(newLo, hi);
                    }
                }

#if 0
                /**
                Stores the locations of the splitting planes (the structure but not the content)
                so that the tree can be quickly rebuilt from a previous configuration without 
                calling balance.
                */
                static void serializeStructure(const Node* n, BinaryOutput& bo) {
                    if (n == NULL) {
                        bo.writeUInt8(0);
                    } else {
                        bo.writeUInt8(1);
                        n->splitBounds.serialize(bo);
                        serialize(n->splitAxis, bo);
                        bo.writeFloat32(n->splitLocation);
                        for (int c = 0; c < 2; ++c) {
                            serializeStructure(n->child[c], bo);
                        }
                    }
                }

                /** Clears the member table */
                static Node* deserializeStructure(BinaryInput& bi) {
                    if (bi.readUInt8() == 0) {
                        return NULL;
                    } else {
                        Node* n = new Node();
                        n->splitBounds.deserialize(bi);
                        deserialize(n->splitAxis, bi);
                        n->splitLocation = bi.readFloat32();
                        for (int c = 0; c < 2; ++c) {
                            n->child[c] = deserializeStructure(bi);
                        }
                    }
                }
#endif

                /** Returns the deepest node that completely contains bounds. */
                Node* findDeepestContainingNode(const AABox& bounds) {

                    // See which side of the splitting plane the bounds are on
                    if (bounds.high()[splitAxis] < splitLocation) {
                        // Bounds are on the low side.  Recurse into the child
                        // if it exists.
                        if (child[0] != NULL) {
                            return child[0]->findDeepestContainingNode(bounds);
                        }
                    } else if (bounds.low()[splitAxis] > splitLocation) {
                        // Bounds are on the high side, recurse into the child
                        // if it exists.
                        if (child[1] != NULL) {
                            return child[1]->findDeepestContainingNode(bounds);
                        }
                    }

                    // There was no containing child, so this node is the
                    // deepest containing node.
                    return this;
                }


                /** Appends all members that intersect the box. 
                If useSphere is true, members that pass the box test
                face a second test against the sphere. */
                void getIntersectingMembers(
                    const AABox&        box, 
                    const Sphere&       sphere,
                    Array<T>&           members,
                    bool                useSphere) const {

                        // Test all values at this node
                        for (int v = 0; v < valueArray.size(); ++v) {
                            const AABox& bounds = valueArray[v].bounds;
                            if (bounds.intersects(box) &&
                                (! useSphere || bounds.intersects(sphere))) {
                                    members.append(valueArray[v].value);
                                }
                        }

                        // If the left child overlaps the box, recurse into it
                        if ((child[0] != NULL) && (box.low()[splitAxis] < splitLocation)) {
                            child[0]->getIntersectingMembers(box, sphere, members, useSphere);
                        }

                        // If the right child overlaps the box, recurse into it
                        if ((child[1] != NULL) && (box.high()[splitAxis] > splitLocation)) {
                            child[1]->getIntersectingMembers(box, sphere, members, useSphere);
                        }
                    }

                    /**
                    Recurse through the tree, assigning splitBounds fields.
                    */
                    void assignSplitBounds(const AABox& myBounds) {
                        splitBounds = myBounds;

                        AABox childBounds[2];
                        myBounds.split(splitAxis, splitLocation, childBounds[0], childBounds[1]);

                        for (int c = 0; c < 2; ++c) {
                            if (child[c]) {
                                child[c]->assignSplitBounds(childBounds[c]);
                            }
                        }
                    }
            };


            /**
            Recursively subdivides the subarray.
            Begin and end indices are inclusive.

            Call assignSplitBounds() on the root node after making a tree.
            */
            Node* makeNode(Array<Handle>& point, int beginIndex, 
                int endIndex, int valuesPerNode, 
                int numMeanSplits)  {

                    Node* node = NULL;

                    if (endIndex - beginIndex + 1 <= valuesPerNode) {
                        // Make a new leaf node
                        node = new Node(point, beginIndex, endIndex);

                        // Set the pointers in the memberTable
                        for (int i = beginIndex; i <= endIndex; ++i) {
                            memberTable.set(point[i].value, node);
                        }

                    } else {
                        // Make a new internal node
                        node = new Node();

                        const AABox bounds = computeBounds(point, beginIndex, endIndex);
                        const Vector3 extent = bounds.high() - bounds.low();

                        Vector3::Axis splitAxis = extent.primaryAxis();

                        double splitLocation;

                        if (numMeanSplits > 0) {
                            // Compute the mean along the axis

                            splitLocation = (bounds.high()[splitAxis] + 
                                bounds.low()[splitAxis]) / 2.0;

                        } else {

                            // Compute the median along the axis

                            // Sort only the subarray
                            std::sort(
                                point.getCArray() + beginIndex,
                                point.getCArray() + endIndex + 1,
                                CenterLT(splitAxis));

                            // Intentional integer divide
                            int midIndex = (beginIndex + endIndex) / 2;

                            // Choose the split location between the two middle elements
                            const Vector3 median = 
                                (point[midIndex].bounds.high() +
                                point[iMin(midIndex + 1, 
                                point.size() - 1)].bounds.low()) * 0.5;

                            splitLocation = median[splitAxis];
                        }


                        // Re-sort around the split. This will allow us to easily
                        // test for overlap
                        std::sort(
                            point.getCArray() + beginIndex,
                            point.getCArray() + endIndex + 1,
                            PivotLT(splitAxis, splitLocation));

                        // Some number of nodes may actually overlap the midddle, so
                        // they must be found and added to *this* node, not one of
                        // its children
                        int overlapBeginIndex, overlapEndIndex;

                        for (overlapBeginIndex = beginIndex;
                            (overlapBeginIndex <= endIndex) &&
                            (point[overlapBeginIndex].bounds.high()[splitAxis] <
                            splitLocation);
                        ++overlapBeginIndex) {}

                        for (overlapEndIndex = endIndex;
                            (overlapEndIndex >= beginIndex) &&
                            (point[overlapEndIndex].bounds.low()[splitAxis] >
                            splitLocation);
                        --overlapEndIndex) {}
#ifdef _ASSEMBLER_DEBUG
                        fprintf(::g_df,"splitLocation: %f, beginIndex: %d, endIndex:%d, overlapBeginIndex: %d, overlapEndIndex: %d\n",splitLocation, beginIndex, endIndex, overlapBeginIndex, overlapEndIndex);
                        if(beginIndex == 220) {
                            float oldf = -100000.0f;

                            for (int xxx = beginIndex;
                                (xxx <= endIndex);
                                ++xxx) {
                                    fprintf(::g_df,"    xxx:%d, point[xxx].bounds.high()[splitAxis]: %f\n",xxx, point[xxx].bounds.high()[splitAxis]);
                                    if(oldf > point[xxx].bounds.high()[splitAxis]) {
                                        fprintf(::g_df,"    huch ...\n");
                                    }
                                    oldf = point[xxx].bounds.high()[splitAxis];
                                }
                        }
#endif
                        // put overlapping boxes in this node
                        for (int i = overlapBeginIndex; i <= overlapEndIndex; ++i) {
                            node->valueArray.push(point[i]);
                            memberTable.set(point[i].value, node);
                        }

                        node->splitAxis     = splitAxis;
                        node->splitLocation = splitLocation;

                        if (overlapBeginIndex > beginIndex) {
                            node->child[0]      = 
                                makeNode(point, beginIndex, overlapBeginIndex - 1, 
                                valuesPerNode, numMeanSplits - 1);
                        }

                        if (overlapEndIndex < endIndex) {
                            node->child[1]      = 
                                makeNode(point, overlapEndIndex + 1, endIndex, valuesPerNode, numMeanSplits - 1);
                        }

                    }

                    return node;
                }

                /**
                Recursively clone the passed in node tree, setting
                pointers for members in the memberTable as appropriate.
                called by the assignment operator.
                */
                Node* cloneTree(Node* src) {
                    Node* dst = new Node(*src);

                    // Make back pointers
                    for (int i = 0; i < dst->valueArray.size(); ++i) {
                        memberTable.set(dst->valueArray[i].value, dst);
                    }

                    // Clone children
                    for (int i = 0; i < 2; ++i) {
                        if (src->child[i] != NULL) {
                            dst->child[i] = cloneTree(src->child[i]);
                        }
                    }

                    return dst;
                }

                /** Maps members to the node containing them */
                Table<T, Node*>         memberTable;

                Node*                   root;



                /** To construct a balanced tree, insert the elements and then call
                AABSPTree::balance(). */
                AABSPTree() : root(NULL) {}


                AABSPTree(const AABSPTree& src) : root(NULL) {
                    *this = src;
                }


                AABSPTree& operator=(const AABSPTree& src) {
                    delete root;
                    // Clone tree takes care of filling out the memberTable.
                    root = cloneTree(src.root);
                }


                ~AABSPTree() {
                    clear();
                }

                /**
                Throws out all elements of the set.
                */
                void clear() {
                    memberTable.clear();
                    delete root;
                    root = NULL;
                }

                int size() const {
                    return memberTable.size();
                }

                /**
                Inserts an object into the set if it is not
                already present.  O(log n) time.  Does not
                cause the tree to be balanced.
                */
                void insert(const T& value) {
                    if (contains(value)) {
                        // Already in the set
                        return;
                    }

                    Handle h(value);

                    if (root == NULL) {
                        // This is the first node; create a root node
                        root = new Node();
                    }

                    Node* node = root->findDeepestContainingNode(h.bounds);

                    // Insert into the node
                    node->valueArray.append(h);

                    // Insert into the node table
                    memberTable.set(value, node);
                }

                /** Inserts each elements in the array in turn.  If the tree
                begins empty (no structure and no elements), this is faster
                than inserting each element in turn.  You still need to balance
                the tree at the end.*/
                void insert(const Array<T>& valueArray) {
                    if (root == NULL) {
                        // Optimized case for an empty tree; don't bother
                        // searching or reallocating the root node's valueArray
                        // as we incrementally insert.
                        root = new Node();
                        root->valueArray.resize(valueArray.size());
                        for (int i = 0; i < valueArray.size(); ++i) {
                            // Insert in opposite order so that we have the exact same
                            // data structure as if we inserted each (i.e., order is reversed
                            // from array).
                            root->valueArray[valueArray.size() - i - 1] = Handle(valueArray[i]);
                            memberTable.set(valueArray[i], root);
                        }
                    } else {
                        // Insert at appropriate tree depth.
                        for (int i = 0; i < valueArray.size(); ++i) {
                            insert(valueArray[i]);
                        }
                    }
                }


                /**
                Returns true if this object is in the set, otherwise
                returns false.  O(1) time.
                */
                bool contains(const T& value) {
                    return memberTable.containsKey(value);
                }


                /**
                Removes an object from the set in O(1) time.
                It is an error to remove members that are not already
                present.  May unbalance the tree.  

                Removing an element never causes a node (split plane) to be removed...
                nodes are only changed when the tree is rebalanced.  This behavior
                is desirable because it allows the split planes to be serialized,
                and then deserialized into an empty tree which can be repopulated.
                */
                void remove(const T& value) {
                    debugAssertM(contains(value),
                        "Tried to remove an element from a "
                        "AABSPTree that was not present");

                    Array<Handle>& list = memberTable[value]->valueArray;

                    // Find the element and remove it
                    for (int i = list.length() - 1; i >= 0; --i) {
                        if (list[i].value == value) {
                            list.fastRemove(i);
                            break;
                        }
                    }
                    memberTable.remove(value);
                }


                /**
                If the element is in the set, it is removed.
                The element is then inserted.

                This is useful when the == and hashCode methods
                on <I>T</I> are independent of the bounds.  In
                that case, you may call update(v) to insert an
                element for the first time and call update(v)
                again every time it moves to keep the tree 
                up to date.
                */
                void update(const T& value) {
                    if (contains(value)) {
                        remove(value);
                    }
                    insert(value);
                }


                /**
                Rebalances the tree (slow).  Call when objects
                have moved substantially from their original positions
                (which unbalances the tree and causes the spatial
                queries to be slow).

                @param valuesPerNode Maximum number of elements to put at
                a node.

                @param numMeanSplits numMeanSplits = 0 gives a 
                fully axis aligned BSP-tree, where the balance operation attempts to balance
                the tree so that every splitting plane has an equal number of left
                and right children (i.e. it is a <B>median</B> split along that axis).  
                This tends to maximize average performance.  

                You can override this behavior by
                setting a number of <B>mean</B> (average) splits.  numMeanSplits = MAX_INT
                creates a full oct-tree, which tends to optimize peak performance at the expense of
                average performance.  It tends to have better clustering behavior when
                members are not uniformly distributed.
                */
                void balance(int valuesPerNode = 5, int numMeanSplits = 3) {
                    if (root == NULL) {
                        // Tree is empty
                        return;
                    }

                    Array<Handle> handleArray;
                    root->getHandles(handleArray);

                    // Delete the old tree
                    clear();

                    root = makeNode(handleArray, 0, handleArray.size() - 1, 
                        valuesPerNode, numMeanSplits);

                    // Walk the tree, assigning splitBounds.  We start with unbounded
                    // space.
                    root->assignSplitBounds(AABox::maxFinite());

#ifdef _DEBUG
                    root->verifyNode(Vector3::minFinite(), Vector3::maxFinite());
#endif
                }

    private:

        /**
        @param parentMask The mask that this node returned from culledBy.
        */
        static void getIntersectingMembers(
            const Array<Plane>&         plane,
            Array<T>&                   members,
            Node*                       node,
            uint32                      parentMask) {

                int dummy;

                if (parentMask == 0) {
                    // None of these planes can cull anything
                    for (int v = node->valueArray.size() - 1; v >= 0; --v) {
                        members.append(node->valueArray[v].value);
                    }

                    // Iterate through child nodes
                    for (int c = 0; c < 2; ++c) {
                        if (node->child[c]) {
                            getIntersectingMembers(plane, members, node->child[c], 0);
                        }
                    }
                } else {

                    // Test values at this node against remaining planes
                    for (int v = node->valueArray.size() - 1; v >= 0; --v) {
                        if (! node->valueArray[v].bounds.culledBy(plane, dummy, parentMask)) {
                            members.append(node->valueArray[v].value);
                        }
                    }

                    uint32 childMask  = 0xFFFFFF;

                    // Iterate through child nodes
                    for (int c = 0; c < 2; ++c) {
                        if (node->child[c] &&
                            ! node->child[c]->splitBounds.culledBy(plane, dummy, parentMask, childMask)) {
                                // This node was not culled
                                getIntersectingMembers(plane, members, node->child[c], childMask);
                            }
                    }
                }
            }

    public:

        /**
        Returns all members inside the set of planes.  
        @param members The results are appended to this array.
        */
        void getIntersectingMembers(const Array<Plane>& plane, Array<T>& members) const {
            if (root == NULL) {
                return;
            }

            getIntersectingMembers(plane, members, root, 0xFFFFFF);
        }

        /**
        Typically used to find all visible
        objects inside the view frustum (see also GCamera::getClipPlanes)... i.e. all objects
        <B>not<B> culled by frustum.

        Example:
        <PRE>
        Array<Object*>  visible;
        tree.getIntersectingMembers(camera.frustum(), visible);
        // ... Draw all objects in the visible array.
        </PRE>
        @param members The results are appended to this array.
        */
        void getIntersectingMembers(const GCamera::Frustum& frustum, Array<T>& members) const {
            Array<Plane> plane;

            for (int i = 0; i < frustum.faceArray.size(); ++i) {
                plane.append(frustum.faceArray[i].plane);
            }

            getIntersectingMembers(plane, members);
        }

        /**
        C++ STL style iterator variable.  See beginBoxIntersection().
        The iterator overloads the -> (dereference) operator, so this acts like a pointer
        to the current member.
        */
        // This iterator turns Node::getIntersectingMembers into a
        // coroutine.  It first translates that method from recursive to
        // stack based, then captures the system state (analogous to a Scheme
        // continuation) after each element is appended to the member array,
        // and allowing the computation to be restarted.
        class BoxIntersectionIterator {
        private:
            friend class AABSPTree<T>;

            /** True if this is the "end" iterator instance */
            bool            isEnd;

            /** The box that we're testing against. */
            AABox           box;

            /** Node that we're currently looking at.  Undefined if isEnd is true. */
            Node*           node;

            /** Nodes waiting to be processed */
            // We could use backpointers within the tree and careful
            // state management to avoid ever storing the stack-- but
            // it is much easier this way and only inefficient if the
            // caller uses post increment (which they shouldn't!).
            Array<Node*>    stack;

            /** The next index of current->valueArray to return. 
            Undefined when isEnd is true.*/
            int             nextValueArrayIndex;

            BoxIntersectionIterator() : isEnd(true) {}

            BoxIntersectionIterator(const AABox& b, const Node* root) : 
            box(b), isEnd(root == NULL), nextValueArrayIndex(-1), node(const_cast<Node*>(root)) {

                // We intentionally start at the "-1" index of the current node
                // so we can use the preincrement operator to move ourselves to
                // element 0 instead of repeating all of the code from the preincrement
                // method.  Note that this might cause us to become the "end"
                // instance.
                ++(*this);
            }

        public:

            inline bool operator!=(const BoxIntersectionIterator& other) const {
                return ! (*this == other);
            }

            bool operator==(const BoxIntersectionIterator& other) const {
                if (isEnd) {
                    return other.isEnd;
                } else if (other.isEnd) {
                    return false;
                } else {
                    // Two non-end iterators; see if they match.  This is kind of 
                    // silly; users shouldn't call == on iterators in general unless
                    // one of them is the end iterator.
                    if ((box != other.box) || (node != other.node) || 
                        (nextValueArrayIndex != other.nextValueArrayIndex) ||
                        (stack.length() != other.stack.length())) {
                            return false;
                        }

                        // See if the stacks are the same
                        for (int i = 0; i < stack.length(); ++i) {
                            if (stack[i] != other.stack[i]) {
                                return false;
                            }
                        }

                        // We failed to find a difference; they must be the same
                        return true;
                }
            }

            /**
            Pre increment.
            */
            BoxIntersectionIterator& operator++() {
                ++nextValueArrayIndex;

                bool foundIntersection = false;
                while (! isEnd && ! foundIntersection) {

                    // Search for the next node if we've exhausted this one
                    while ((! isEnd) &&  (nextValueArrayIndex >= node->valueArray.length())) {
                        // If we entered this loop, then the iterator has exhausted the elements at 
                        // node (possibly because it just switched to a child node with no members).
                        // This loop continues until it finds a node with members or reaches
                        // the end of the whole intersection search.

                        // If the right child overlaps the box, push it onto the stack for
                        // processing.
                        if ((node->child[1] != NULL) &&
                            (box.high()[node->splitAxis] > node->splitLocation)) {
                                stack.push(node->child[1]);
                            }

                            // If the left child overlaps the box, push it onto the stack for
                            // processing.
                            if ((node->child[0] != NULL) &&
                                (box.low()[node->splitAxis] < node->splitLocation)) {
                                    stack.push(node->child[0]);
                                }

                                if (stack.length() > 0) {
                                    // Go on to the next node (which may be either one of the ones we 
                                    // just pushed, or one from farther back the tree).
                                    node = stack.pop();
                                    nextValueArrayIndex = 0;
                                } else {
                                    // That was the last node; we're done iterating
                                    isEnd = true;
                                }
                    }

                    // Search for the next intersection at this node until we run out of children
                    while (! isEnd && ! foundIntersection && (nextValueArrayIndex < node->valueArray.length())) {
                        if (box.intersects(node->valueArray[nextValueArrayIndex].bounds)) {
                            foundIntersection = true;
                        } else {
                            ++nextValueArrayIndex;
                            // If we exhaust this node, we'll loop around the master loop 
                            // to find a new node.
                        }
                    }
                }

                return *this;
            }

            /**
            Post increment (much slower than preincrement!).
            */
            BoxIntersectionIterator operator++(int) {
                BoxIntersectionIterator old = *this;
                ++this;
                return old;
            }

            /** Overloaded dereference operator so the iterator can masquerade as a pointer
            to a member */
            const T& operator*() const {
                alwaysAssertM(! isEnd, "Can't dereference the end element of an iterator");
                return node->valueArray[nextValueArrayIndex].value;
            }

            /** Overloaded dereference operator so the iterator can masquerade as a pointer
            to a member */
            T const * operator->() const {
                alwaysAssertM(! isEnd, "Can't dereference the end element of an iterator");
                return &(stack.last()->valueArray[nextValueArrayIndex].value);
            }

            /** Overloaded cast operator so the iterator can masquerade as a pointer
            to a member */
            operator T*() const {
                alwaysAssertM(! isEnd, "Can't dereference the end element of an iterator");
                return &(stack.last()->valueArray[nextValueArrayIndex].value);
            }
        };


        /**
        Iterates through the members that intersect the box
        */
        BoxIntersectionIterator beginBoxIntersection(const AABox& box) const {
            return BoxIntersectionIterator(box, root);
        }

        BoxIntersectionIterator endBoxIntersection() const {
            // The "end" iterator instance
            return BoxIntersectionIterator();
        }

        /**
        Appends all members whose bounds intersect the box.
        See also AABSPTree::beginBoxIntersection.
        */
        void getIntersectingMembers(const AABox& box, Array<T>& members) const {
            if (root == NULL) {
                return;
            }
            root->getIntersectingMembers(box, Sphere(Vector3::ZERO, 0), members, false);
        }


        /**
        @param members The results are appended to this array.
        */
        void getIntersectingMembers(const Sphere& sphere, Array<T>& members) const {
            if (root == NULL) {
                return;
            }

            AABox box;
            sphere.getBounds(box);
            root->getIntersectingMembers(box, sphere, members, true);

        }

        /** See AABSPTree::beginRayIntersection */
        class RayIntersectionIterator {
        private:
            friend class AABSPTree<T>;

            /** The stack frame contains all the info needed to resume
            computation for the RayIntersectionIterator */

            struct StackFrame {
                const Node* node;

                /** the total checking bounds for this frame */
                float minTime;

                /** minTime^2 */
                float minTime2;
                float maxTime;

                /** what we're checking right now, either from minTime to the
                split, or from the split to maxTime (more or less...
                there are edge cases) */
                float startTime;
                float endTime;

                /** endTime^2 */
                float endTime2;

                int nextChild;

                /** current index into node's valueArray */
                int valIndex;


                /** cache intersection values when they're checked on the preSide,
                split so they don't need to be checked again after the split. */
                Array<float> intersectionCache;

                void init(const Node* inNode, const Ray& ray,
                    float inMinTime, float inMaxTime) {
                        node     = inNode;
                        minTime  = inMinTime;
                        maxTime  = inMaxTime;
                        minTime2 = square(minTime);
                        valIndex = -1;

                        intersectionCache.resize(node->valueArray.length());

                        if (node->child[0] == NULL && node->child[1] == NULL) {
                            startTime = minTime;
                            endTime   = maxTime;
                            endTime2  = square(maxTime);
                            nextChild = -1;
                            return;
                        }

                        Vector3::Axis splitAxis     = node->splitAxis;
                        double        splitLocation = node->splitLocation;

                        // this is the time along the ray until the split.
                        // could be negative if the split is behind.
                        double splitTime =
                            (splitLocation - ray.origin[splitAxis]) /
                            ray.direction[splitAxis];

                        // If splitTime <= minTime we'll never reach the
                        // split, so set it to inf so as not to confuse endTime.
                        // It will be noted below that when splitTime is inf
                        // only one of this node's children will be searched
                        // (the pre child). Therefore it is critical that
                        // the correct child is gone too.
                        if (splitTime <= minTime) {
                            splitTime = inf();
                        }

                        startTime = minTime;
                        endTime   = min((double)maxTime, splitTime);
                        endTime2  = square(endTime);

                        double rayLocation = ray.origin[splitAxis] +
                            ray.direction[splitAxis] * minTime;

                        if (rayLocation == splitLocation) {
                            // We're right on the split. Look ahead.
                            rayLocation = ray.origin[splitAxis] +
                                ray.direction[splitAxis] * maxTime;
                        }

                        if (rayLocation == splitLocation) {
                            // right on the split, looking exactly along
                            // it, so consider no children.
                            nextChild = -1;
                        } else if(rayLocation <= splitLocation) {
                            nextChild = 0;
                        } else {
                            nextChild = 1;
                        }
                    }
            };

        public:
            /** A minimum bound on the distance to the intersection. */
            double minDistance;

            /** A maximum bound on the distance to the intersection. */
            double maxDistance;

            /** Counts how many bounding box intersection tests have been made so
            far. */
            int debugCounter;

        private:
            Ray                 ray;
            bool                isEnd;

            Array<StackFrame>   stack;
            int                 stackLength;
            int                 stackIndex;
            int                 breakFrameIndex;
            bool                skipAABoxTests;
            double				boxMaxDist2;

            RayIntersectionIterator(const Ray& r, const Node* root, double pMaxTime, bool skip)
                : minDistance(0), maxDistance(G3D::inf()), debugCounter(0),
                ray(r), isEnd(root == NULL),
                stackLength(20), stackIndex(0), breakFrameIndex(-1),
                skipAABoxTests(skip)
            {
                stack.resize(stackLength);
                stack[stackIndex].init(root, ray, 0, G3D::inf());
                boxMaxDist2 = pMaxTime*pMaxTime;

                ++(*this);
            }


        public:
            /* public so we can have empty ones */
            RayIntersectionIterator() : isEnd(true) {}

            inline bool operator!=(const RayIntersectionIterator& other) const {
                return ! (*this == other);
            }

            /** Compares two iterators, but will only return true if both are at
            the end. */
            bool operator==(const RayIntersectionIterator& other) const {
                if (isEnd) {
                    return other.isEnd;
                }

                return false;
            }

            /**
            Marks the node where the most recent intersection occurred. If
            the iterator exhausts this node it will stop and set itself to
            the end iterator.

            Use this after you find a true intersection to stop the iterator
            from searching more than necessary.
            <B>Beta API-- subject to change</B>
            */
            // In theory this method could be smarter: the caller could pass in
            // the distance of the actual collision and the iterator would keep
            // itself from checking nodes or boxes beyond that distance.
            void markBreakNode() {
                breakFrameIndex = stackIndex;
            }

            /**
            Clears the break node. Can be used before or after the iterator
            stops from a break.
            <B>Beta API-- subject to change</B>
            */
            void clearBreakNode() {
                if (breakFrameIndex < 0) {
                    return;
                }

                if (isEnd && stackIndex >= 0) {
                    isEnd = false;
                }

                breakFrameIndex = -1;
            }


            RayIntersectionIterator& operator++() {
                alwaysAssertM(!isEnd, "Can't increment the end element of an iterator");

                StackFrame* s = &stack[stackIndex];

                // leave the loop if:
                //    end is reached (ie: stack is empty)
                //    found an intersection

                while (true) {
                    ++s->valIndex;

                    if (s->valIndex >= s->node->valueArray.length()) {
                        // This node is exhausted, look at its
                        // children.

                        Node* child = (s->nextChild >= 0) ?
                            s->node->child[s->nextChild] : NULL;
                        double childStartTime = s->startTime;
                        double childEndTime   = s->endTime;

                        if (s->endTime < s->maxTime) {
                            // we can come back to this frame,
                            // so reset it
                            s->valIndex  = -1;
                            s->startTime = s->endTime;
                            s->endTime   = s->maxTime;
                            s->endTime2  = square(s->maxTime);
                            s->nextChild = (s->nextChild >= 0) ?
                                (1 - s->nextChild) : -1;

                            // this could be changed somehow,
                            // since Array already does the
                            // power-of-two growth stuff
                            if (stackIndex == stackLength) {
                                stackLength *= 2;
                                stack.resize(stackLength);
                            }
                        } else {
                            // tail-recursion: we won't come
                            // back to this frame, so we can
                            // remove it.

                            if (stackIndex == breakFrameIndex) {
                                // This will be the case if the
                                // break frame is set on a node, but
                                // the node is exhausted so it won't
                                // be put on the stack. Here we
                                // decrement the break frame so that
                                // the break occurs when the current
                                // frame's parent is resumed.
                                --breakFrameIndex;
                            }

                            --stackIndex;
                        }

                        // There could have been a resize on the array, so
                        // do not use s (pointer into the array)!

                        if (child != NULL) {
                            ++stackIndex;
                            stack[stackIndex].init(
                                child, ray,
                                childStartTime, childEndTime);
                        }

                        if ((stackIndex < 0) || (stackIndex == breakFrameIndex)) {
                            isEnd = true;
                            break;
                        }

                        s = &stack[stackIndex];
                        continue;
                    }

                    if (skipAABoxTests) {
                        // No AABox test-- return everything
                        minDistance = s->startTime;
                        maxDistance = s->endTime;
                        break;
                    } else {
                        double t2;
                        // this can be an exact equals because the two
                        // variables are initialized to the same thing
                        if (s->startTime == s->minTime) {
                            Vector3 location;
                            bool insiteOk;
                            Vector3 mynormal;
                            if (
                                VMAP::MyCollisionDetection::collisionLocationForMovingPointFixedAABox(
                                ray.origin, ray.direction,
                                s->node->valueArray[s->valIndex].bounds,
                                location,insiteOk, mynormal )) {
                                    // Optimization: store t-squared 
                                    t2 = (location - ray.origin).squaredLength();
                                } else {
                                    t2 = inf();
                                }
                                if(t2 > boxMaxDist2) 
                                    t2=inf(); // too far off
                                //t = ray.intersectionTime(s->node->valueArray[s->valIndex].bounds);
                                s->intersectionCache[s->valIndex] = t2;
                                ++debugCounter;
                        } else {
                            t2 = s->intersectionCache[s->valIndex];
                        }

                        // use minTime here because intersection may be
                        // detected pre-split, but be valid post-split, too.
                        if ((t2 >= s->minTime2) && (t2 < s->endTime2)) {
                            // Gives slightly tighter bounds but runs slower:
                            // minDistance = max(t, s->startTime);
                            minDistance = s->startTime;
                            maxDistance = s->endTime;
                            break;
                        }
                    }

                }

                return *this;
            }


            /** Overloaded dereference operator so the iterator can masquerade as a pointer
            to a member */
            const T& operator*() const {
                alwaysAssertM(! isEnd, "Can't dereference the end element of an iterator");
                return stack[stackIndex].node->valueArray[stack[stackIndex].valIndex].value;
            }

            /** Overloaded dereference operator so the iterator can masquerade as a pointer
            to a member */
            T const * operator->() const {
                alwaysAssertM(! isEnd, "Can't dereference the end element of an iterator");
                return &(stack[stackIndex].node->valueArray[stack[stackIndex].valIndex].value);
            }

            /** Overloaded cast operator so the iterator can masquerade as a pointer
            to a member */
            operator T*() const {
                alwaysAssertM(! isEnd, "Can't dereference the end element of an iterator");
                return &(stack[stackIndex].node->valueArray[stack[stackIndex].valIndex].value);
            }

        };


        /**
        Generates a RayIntersectionIterator that produces successive
        elements from the set whose bounding boxes are intersected by the ray.
        Typically used for ray tracing, hit-scan, and collision detection.

        The elements are generated mostly in the order that they are hit by the
        ray, so that iteration may end abruptly when the closest intersection to
        the ray origin has been reached. Because the elements within a given
        kd-tree node are unordered, iteration may need to proceed a little past
        the first member returned in order to find the closest intersection. The
        iterator doesn't automatically find the first intersection because it is
        looking at bounding boxes, not the true intersections.

        When the caller finds a true intersection it should call markBreakNode()
        on the iterator. This will stop the iterator (setting it to the end
        iterator) when the current node and relevant children are exhausted.

        Complicating the matter further, some members straddle the plane. The
        iterator produces these members <I>twice</I>. The first time it is
        produced the caller should only consider intersections on the near side of
        the split plane. The second time, the caller should only consider
        intersections on the far side. The minDistance and maxDistance fields
        specify the range on which intersections should be considered. Be aware
        that they may be inf or zero.

        An example of how to use the iterator follows. Almost all ray intersection
        tests will have identical structure.

        <PRE>

        void findFirstIntersection(
        const Ray&  ray,
        Object*&    firstObject,
        double&     firstTime) {

        firstObject   = NULL;
        firstDistance = inf();

        typedef AABSPTree<Object*>::RayIntersectionIterator IT;
        const IT end = tree.endRayIntersection();

        for (IT obj = tree.beginRayIntersection(ray);
        obj != end;
        ++obj) {  // (preincrement is *much* faster than postincrement!) 

        // Call your accurate intersection test here.  It is guaranteed
        // that the ray hits the bounding box of obj.  (*obj) has type T,
        // so you can call methods directly using the "->" operator.
        double t = obj->distanceUntilIntersection(ray);

        // Often methods like "distanceUntilIntersection" can be made more
        // efficient by providing them with the time at which to start and
        // to give up looking for an intersection; that is, 
        // obj.minDistance and iMin(firstDistance, obj.maxDistance).

        static const double epsilon = 0.00001;
        if ((t < firstDistance) && 
        (t <= obj.maxDistance + epsilon) &&
        (t >= obj.minDistance - epsilon)) {

        // This is the new best collision time
        firstObject   = obj;
        firstDistance = t;

        // Tell the iterator that we've found at least one 
        // intersection.  It will finish looking at all
        // objects in this node and then terminate.
        obj.markBreakNode();
        }
        }
        }
        </PRE>


        @param skipAABoxTests Set to true when the intersection test for a
        member is faster than an AABox-ray intersection test.  In that case, 
        the iterator will not use a bounding box test on values that are
        returned.  Leave false (the default) for objects with slow intersection
        tests.  In that case, the iterator guarantees that the ray hits the
        bounds of any object returned.

        @cite Implementation by Pete Hopkins
        */
        RayIntersectionIterator beginRayIntersection(const Ray& ray, double pMaxTime, bool skipAABoxTests = false) const
        {
            return RayIntersectionIterator(ray, root, pMaxTime, skipAABoxTests);
        }

        RayIntersectionIterator endRayIntersection() const
        {
            return RayIntersectionIterator();
        }


        /**
        Returns an array of all members of the set.  See also AABSPTree::begin.
        */
        void getMembers(Array<T>& members) const {
            memberTable.getKeys(members);
        }


        /**
        C++ STL style iterator variable.  See begin().
        Overloads the -> (dereference) operator, so this acts like a pointer
        to the current member.
        */
        class Iterator {
        private:
            friend class AABSPTree<T>;

            // Note: this is a Table iterator, we are currently defining
            // Set iterator
            typename Table<T, Node*>::Iterator it;

            Iterator(const typename Table<T, Node*>::Iterator& it) : it(it) {}

        public:
            inline bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            bool operator==(const Iterator& other) const {
                return it == other.it;
            }

            /**
            Pre increment.
            */
            Iterator& operator++() {
                ++it;
                return *this;
            }

            /**
            Post increment (slower than preincrement).
            */
            Iterator operator++(int) {
                Iterator old = *this;
                ++(*this);
                return old;
            }

            const T& operator*() const {
                return it->key;
            }

            T* operator->() const {
                return &(it->key);
            }

            operator T*() const {
                return &(it->key);
            }
        };


        /**
        C++ STL style iterator method.  Returns the first member.  
        Use preincrement (++entry) to get to the next element (iteration
        order is arbitrary).  
        Do not modify the set while iterating.
        */
        Iterator begin() const {
            return Iterator(memberTable.begin());
        }


        /**
        C++ STL style iterator method.  Returns one after the last iterator
        element.
        */
        Iterator end() const {
            return Iterator(memberTable.end());
        }
    };

#define KDTreeSet AABSPTree

}

#endif


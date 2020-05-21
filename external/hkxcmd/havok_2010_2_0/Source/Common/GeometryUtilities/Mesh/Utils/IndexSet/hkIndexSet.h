/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_INDEX_SET_H
#define HK_INDEX_SET_H

/// Holds a unique set of int indices.
///
/// This implements a 'mathematical' style set of int values or 'indices'.
///
/// A set either contains or does not contain an index. Sets can be combined via the standard set
/// operations of setIntersection, setUnion and setDifference. This operations are designed to be relatively
/// fast. Note that the sets passed in cannot be the same as the set that is being modified.
///
/// Its possible to modify the contents using startUpdate/endUpdate. Members can be added/removed from the
/// set by manipulating the array. Doing endUpdate will sort the members, and remove any duplicates.
///
/// The implementation holds the indices held in the set in a sorted array. Having a sorted list, allows for fast
/// comparison, and the boolean operations. It also allows relatively fast containment testing - using a binary chop
/// giving O(log2(n)) performance.
class hkIndexSet
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA, hkIndexSet);

            /// Returns the index of index if found, else -1 if not
            /// Complexity - O(log(n)) n is size of set
        int findIndex(int index) const;

            /// Returns true if the set contains the index
            /// Complexity - O(log(n)) n is size of set
        hkBool contains(int index) const { return findIndex(index) >= 0; }

            /// Returns the indices which are in both sets
            /// Complexity O(n + m) n is size of a, m is size of b
        void setIntersection(const hkIndexSet& a, const hkIndexSet& b);
            /// Returns the union of both sets
            /// Complexity O(n + m) n is size of a, m is size of b
        void setUnion(const hkIndexSet& a, const hkIndexSet& b);

            /// Set the set as the members of a minus the members of b
            /// Complexity O(n + m) n is size of a, m is size of b
        void setDifference(const hkIndexSet& a, const hkIndexSet& b);

            /// Read only access to contained indices
        const hkArray<int>& getIndices() const { return m_indices; }

            /// Start updating the contents
            /// Complexity - O(1)
        hkArray<int>& startUpdate();
            /// End the update
            /// Complexity - O(n), will reorder indices and remove duplicates
        void endUpdate();

            /// Get the number of indices. Note the can be incorrect in an update if there are repeated indices.
		int getSize() const { return m_indices.getSize(); }

            /// Remove a value (can only be done in an update)
            /// Complextiy - O(n)
        void removeIndex(int index);
            /// Add a value (can only be done in an update)
            /// Complexity O(1). Note that each addition will consume memory, duplicates are only removed in the end update
        void addIndex(int index);

            /// Remove all of the contents
        void clear() { m_indices.clear(); }

            /// Optimizes memory allocation
        void optimizeAllocation();

            /// Clears and deallocates
        void clearAndDeallocate() { m_indices.clearAndDeallocate(); }

            /// Swap
        void swap(hkIndexSet& rhs) { m_indices.swap(rhs.m_indices); }

            /// Returns true if the index sets are the same
        hkBool operator==(const hkIndexSet& rhs) const;
            /// Returns true if the index sets are not equal
        hkBool operator!=(const hkIndexSet& rhs) const { return !(*this == rhs); }

            /// Assignment
        void operator=(const hkIndexSet& rhs);

            /// Calculate the number members of both this and the parameter
        int calculateNumIntersecting(const hkIndexSet& rhs) const;

            /// Calculate the total unique members in both
        int calculateNumUnion(const hkIndexSet& rhs) const;

            /// Ctor
        hkIndexSet();

            /// Copy ctor
        hkIndexSet(const hkIndexSet& rhs);

            /// Calculates the number of members which intersect and are union
        static void HK_CALL calculateNumIntersectAndUnion(const hkIndexSet& setA, const hkIndexSet& setB, int& numIntersectOut, int& numUnionOut);

#ifdef HK_DEBUG
        // Used to test this class is functioning correctly
        static void HK_CALL selfTest();
#endif

    protected:
        hkArray<int> m_indices;
        hkBool m_inUpdate;
};

#endif // HK_INDEX_SET_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/

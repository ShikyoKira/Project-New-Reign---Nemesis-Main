/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIND_UNIQUE_INDICES_UTIL_H
#define HK_FIND_UNIQUE_INDICES_UTIL_H

/// A simple utility to determine the unique set of used indices
///
/// The utility needs to know the index of the maximum vertex index that is going to be processed
/// doing so allows it to use very fast array lookups to determine if an index has already been added.
///
/// Before any indices can be added the 'initialize' method must be called with the number of vertex indices
/// (i.e., 1 more than the maximum index value). Now indices can be added by calling 'addIndex' - it will return
/// the index given to that index.
///
/// The m_uniqueIndices member will hold the original indices of the indices added. The position in uniqueIndices
/// determines what value was assigned to that index.
///
/// The member m_indexMap - holds -1 for any source index which wasn't added, or the index of the output index for the
/// the source index otherwise.
class hkFindUniqueIndicesUtil
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkFindUniqueIndicesUtil );

            /// Initialize, must be called before anything is added
        void initialize(int maxIndex);

            /// Returns the output index
        int addIndex(int index);

            /// Add indices
        void addIndices(const hkUint16* indices, int numIndices);

            /// Get the index in the unique indices list, or -1 if not added
        int getIndex(int index) const { return m_indicesMap[index]; }

        hkArray<int> m_indicesMap;
        hkArray<int> m_uniqueIndices;
};

#endif // HK_FIND_UNIQUE_INDICES_UTIL_H

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

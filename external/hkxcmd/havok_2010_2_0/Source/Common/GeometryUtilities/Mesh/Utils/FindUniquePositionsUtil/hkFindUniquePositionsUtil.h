/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIND_UNIQUE_POSITIONS_UTIL_H
#define HK_FIND_UNIQUE_POSITIONS_UTIL_H

/// hkFindUniquePositionsUtil is a utility to find vectors which have exactly the same x,y and z member values
///
/// To use, an object of hkFindUniquePositionsUtil should be instantiated. hkVector4 positions can then be added. Using
/// the addPosition method the index of the position added will be returned. If the position has already been added the
/// previously found values index will be returned. To check rapidly if a position has already been added - the
/// findPosition method will return the index of the position if it has been added, or -1 if not.
///
/// Note that the algorithm is effectively using the precise bit combinations of the x, y and z values to determine
/// equality - there is no threshold for equality for example.
///
/// To get the array of all of the unique positions that have been added the m_positions member variable holds this
/// and is publicly accessible.
///
/// The utility works by converting the x,y and z values of a hkVector4 into a hash value, and then keeping all the
/// indices to the values with the same has in a singly linked list.
class hkFindUniquePositionsUtil
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkFindUniquePositionsUtil );

			/// Remove all entries and reserve space for next adding
		void reset(int sizeEstimate = 1024);

            /// Returns the unique position index
        int addPosition(const hkVector4& pos);

			/// Add lots of positions
		void addPositions(const hkVector4* pos, int numPos);

            /// Returns the index of that position or -1 if not found
        int findPosition(const hkVector4& pos) const;

            /// Work out a reasonable hash value for a vector4
        HK_FORCE_INLINE static hkUint32 hashVector(const hkVector4& vec)
        {
            const hkUint32 v0 = *(hkUint32*)&vec(0);
            const hkUint32 v1 = *(hkUint32*)&vec(1);
            const hkUint32 v2 = *(hkUint32*)&vec(2);

            // The bottom bit is removed to stop it ever being 0xffffffff
            hkUint32 hash = (v0 ^ ((v1 >> 16) | (v1 << 16)) ^ ((v2 >> 3) | (v2 << 29))) & ~hkUint32(1);

			return hash;
        }

        hkArray<hkVector4> m_positions;
        struct Entry
        {
            int m_positionIndex;                    ///< The position index
            int m_nextEntryIndex;                   ///< Next entries index (of -1 if at the end)
        };

        hkPointerMap<hkUint32, int> m_hashMap;      ///< Goes from a hash to a vertex entry index
        hkArray<Entry> m_entries;                   ///< All of the entries
};

#endif // HK_FIND_UNIQUE_POSITIONS_UTIL_H

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

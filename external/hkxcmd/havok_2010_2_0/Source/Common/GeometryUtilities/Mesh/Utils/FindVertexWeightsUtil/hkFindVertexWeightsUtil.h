/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FIND_VERTEX_WEIGHTS_UTIL_H
#define HK_FIND_VERTEX_WEIGHTS_UTIL_H

#include <Common/GeometryUtilities/Mesh/Utils/SkinningUtil/hkSkinningUtil.h>

class hkFindVertexWeightsUtil
{
    public:
        typedef hkSkinningUtil::Entry Entry;

            /// Insert an entry, returns the current amount of values.
        HK_FORCE_INLINE static int HK_CALL insertEntry(Entry* entries, int numValues, int weightsPerVertex, hkReal distSq, int boneIndex);
            /// Get max value. Can be called after insert entry to get the current largest value held
        HK_FORCE_INLINE static hkReal HK_CALL getMaxValue(Entry* entries, int weightsPerVertex) {  return entries[weightsPerVertex - 1].m_distanceSquared; }
            /// Set up the values
        static void HK_CALL reset(Entry* entries, int weightsPerVertex, int numVertices);
};


HK_FORCE_INLINE /* static */int hkFindVertexWeightsUtil::insertEntry(hkSkinningUtil::Entry* entries, int numValues, int weightsPerVertex, hkReal distSq, int boneIndex)
{
	for (int i = 0; i < numValues; i++)
	{
		if (entries[i].m_distanceSquared > distSq)
		{
            if (numValues < weightsPerVertex)
			{
				numValues++;
			}

			// Copy down
			for (int j = numValues - 1; j > i; j--)
			{
				entries[j] = entries[j - 1];
			}

			entries[i].m_distanceSquared = distSq;
			entries[i].m_index = boneIndex;
			return numValues;
		}
	}

    if (numValues < weightsPerVertex)
	{
		// Just stick at the end
		entries[numValues].m_distanceSquared = distSq;
		entries[numValues].m_index = boneIndex;

		return numValues + 1;
	}

	return numValues;
}


#endif // HK_FIND_VERTEX_WEIGHTS_UTIL_H

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

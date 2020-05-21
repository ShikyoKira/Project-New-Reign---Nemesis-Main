/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_MATH_H
#define HK_KDTREE_MATH_H

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.h>
#include <Common/Base/Math/Vector/hkIntVector.h>

class hkKdTreeMath
{
public:
	static HK_FORCE_INLINE void setInverseScale( hkVector4& vecOut );

		/// Checks whether the specified component of v is 0
	static HK_FORCE_INLINE hkBool32 isComponentEqZero(hkVector4Parameter v, int component);
	static HK_FORCE_INLINE hkBool32 isComponentGtZero(hkVector4Parameter v, int component);

	static HK_FORCE_INLINE void vectorBroadcastMax3(hkVector4& v);
	static HK_FORCE_INLINE void vectorBroadcastMin3(hkVector4& v);

	static HK_FORCE_INLINE void vectorBroadcastMax3(hkVector4& v, hkVector4ComparisonParameter ignoreMask);
	static HK_FORCE_INLINE void vectorBroadcastMin3(hkVector4& v, hkVector4ComparisonParameter ignoreMask);

		/// Used for checking whether a ray hits the specified AABB
	static HK_FORCE_INLINE hkBool rayAabbCheckSlow(hkVector4Parameter orig, hkVector4Parameter invDir, hkReal tNear, hkReal tFar, const hkAabb& aabb);
	static HK_FORCE_INLINE hkBool32 rayAabbCheck(hkVector4Parameter orig, hkVector4Parameter invDir, hkReal tNear, hkReal tFar, const hkAabb& aabb);

		/// Used for checking whether a ray bundle hits the specified AABB
		/// aabbData stored in 6 vectors, each with all 4 components the same, as produced by getAabbVecsFromSplitArray
	static HK_FORCE_INLINE hkVector4Comparison rayBundleAabbCheck(const struct hkKdTreeUtils::RayBundle& orig, const struct hkKdTreeUtils::RayBundle& invDir, hkVector4Parameter _tNear, hkVector4Parameter _tFar, const hkVector4* aabbData);

		/// Used for extracting the AABBs from the tree when a leaf is encountered
	static HK_FORCE_INLINE void convertProjectedEntryToHkVector4(const hkKdTreeProjectedEntry* splits, hkVector4& v1, hkVector4& v2);
	static HK_FORCE_INLINE void getAabbFromProjectedEntry(const hkKdTreeProjectedEntry* aabbData, hkAabb& aabb, hkVector4Parameter offset, hkVector4Parameter _scale);
	static HK_FORCE_INLINE void getAabbVecsFromProjectedEntry(const hkKdTreeProjectedEntry* aabbData, hkVector4* HK_RESTRICT vecsOut, hkVector4Parameter offset, hkVector4Parameter scaleIn);


	// Out-of-line versions so that we can examine the generated code


	static hkBool32 _isComponentEqZero(hkVector4Parameter v, int component);
	static hkBool32 _isComponentGtZero(hkVector4Parameter v, int component);

	/// Used for checking whether a ray hits the specified AABB
	//static hkBool _rayAabbCheckSlow(hkVector4Parameter orig, hkVector4Parameter invDir, hkReal tNear, hkReal tFar, const hkAabb& aabb);
	static hkBool32 _rayAabbCheck(hkVector4Parameter orig, hkVector4Parameter invDir, hkReal tNear, hkReal tFar, const hkAabb& aabb);

	/// Used for checking whether a ray bundle hits the specified AABB
	/// aabbData stored in 6 vectors, each with all 4 components the same, as produced by getAabbVecsFromSplitArray
	static hkVector4Comparison _rayBundleAabbCheck(const struct hkKdTreeUtils::RayBundle& orig, const struct hkKdTreeUtils::RayBundle& invDir, hkVector4Parameter _tNear, hkVector4Parameter _tFar, const hkVector4* aabbData);

	/// Used for extracting the AABBs from the tree when a leaf is encountered
	static void _convertSplitTypeToHkVector4(const hkSplitType* splits, hkVector4& v1, hkVector4& v2);
	static void _getAabbFromSplitArray(const hkSplitType* aabbData, hkAabb& aabb, hkVector4Parameter offset, hkVector4Parameter _scale);
	static void _getAabbVecsFromSplitArray(const hkSplitType* aabbData, hkVector4* HK_RESTRICT vecsOut, hkVector4Parameter offset, hkVector4Parameter scaleIn);


};

#include <Common/Internal/KdTree/hkKdTreeMath.inl>

#endif // HK_KDTREE_MATH_H

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

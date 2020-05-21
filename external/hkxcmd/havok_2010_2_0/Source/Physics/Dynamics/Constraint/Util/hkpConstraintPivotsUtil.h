/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_PIVOTS_UTIL_H
#define HK_DYNAMICS2_CONSTRAINT_PIVOTS_UTIL_H

class hkpConstraintData;
class hkpWorld;
class hkpShape;

	/// Simple utility for cloning of constraint datas (which reference motors).
class hkpConstraintPivotsUtil
{
	public:
			/// Checks if the constraints supports getting and setting of its pivot points.
		static hkBool HK_CALL constraintSupportsPivotGetSet(const hkpConstraintData* data);

			/// Get the constraint's pivot for body A.
		static const hkVector4& HK_CALL getPivotA(const hkpConstraintData* data);

			/// Get the constraint's pivot for body B.
		static const hkVector4& HK_CALL getPivotB(const hkpConstraintData* data);

			/// Set the constraint's pivot for body A.
		static void HK_CALL setPivot(hkpConstraintData* data, const hkVector4& pivot, int pivotIndex);

			/// Set the constraint's pivot for body A.
		static void HK_CALL setPivotTransform(hkpConstraintData* data, const hkTransform& pivot, int pivotIndex);

			/// Get the pivot transform
		static void HK_CALL getPivotTransform( const hkpConstraintData* data, int index, hkTransform& pivot );

			/// Finds the hkpShapeKey of the child closest to the pivot.
		static hkpShapeKey HK_CALL findClosestShapeKey(const hkpWorld* world, const hkpShape* shape, const hkVector4& pivotInBodySpace);

};

#endif // HK_DYNAMICS2_CONSTRAINT_PIVOTS_UTIL_H

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

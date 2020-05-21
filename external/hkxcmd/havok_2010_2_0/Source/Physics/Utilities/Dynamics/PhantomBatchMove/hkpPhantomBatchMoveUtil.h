/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_PHANTOM_BATCH_MOVE_H
#define HK_PHANTOM_BATCH_MOVE_H
	
class hkpPhantom;

	/// A utility class for batch moving phantoms
class hkpPhantomBatchMoveUtil
{
	public:
			/// Takes an array of phantoms and an array of positions to move them to.
			/// This function will update the broadphase once using a batch of AABBs representing the given phantoms.
			/// The phantoms can any combination of hkpShapePhanoms (Simple or Caching) or hkpAabbPhantoms.
			/// This can give a significant performance improvement when compared to setting the phantoms' positions sequentially.
			/// \param extraTolerance For hkpShapePhantoms this parameter defines tolerance additional to the world
			///		tolerance which will be applied when calculating the shape's AABB.
		static void HK_CALL setPositionBatch( hkArray<hkpPhantom*>& phantoms, const hkArray<hkVector4>& positions, hkReal extraTolerance = 0.0f);
						
			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [shapePhantom,HK_ACCESS_RW] );
		static void HK_CALL setPositionBatch( hkpPhantom** phantoms, const hkVector4* positions, int numPhantoms, hkReal extraTolerance = 0.0f);

};

#endif //HK_PHANTOM_BATCH_MOVE_H

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

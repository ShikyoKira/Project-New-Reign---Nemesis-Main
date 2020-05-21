/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SHAPE_DEPTH_UTIL
#define HK_SHAPE_DEPTH_UTIL

class hkpShape;

/// This class supports a routine for traversing shapes hierarchy to discover their depth.
class hkpShapeDepthUtil
{
	public:
			/// Calculates the depth of the shape.
			/// \return0 for leaf shapes, e.g., sphere shapes.
			/// \return1 + the maximum depth of its children for compound shapes.
		static hkUint8 HK_CALL getShapeDepth( const hkpShape* shape );
};

#endif // HK_SHAPE_DEPTH_UTIL

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

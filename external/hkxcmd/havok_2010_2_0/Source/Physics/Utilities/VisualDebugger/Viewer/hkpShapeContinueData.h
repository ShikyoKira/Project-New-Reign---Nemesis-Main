/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SHAPE_CONTINUE_DATA
#define HK_SHAPE_CONTINUE_DATA

#include <Physics/Collide/Shape/hkpShape.h>

class hkpShapeContinueData : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpShapeContinueData() : m_i( -1 ), m_j( -1 ) {}

			/// The branch of shape keys.
			/// We do not add entries for single shape containers (e.g., convex transform shapes).
		hkArray<hkpShapeKey> m_shapeKeys;

			/// The x iterator if we're traversing a height-field
			/// OR a sphere in a multi-sphere shape
			/// OR a ray in a multi-ray shape
		int m_i;
			/// The z iterator if we're traversing a height-field.
		int m_j;
};

#endif // HK_SHAPE_CONTINUE_DATA

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

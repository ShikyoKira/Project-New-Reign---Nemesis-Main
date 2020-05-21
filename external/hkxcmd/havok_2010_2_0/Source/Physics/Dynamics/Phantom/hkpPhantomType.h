/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_PHANTOM_TYPE_H
#define HK_DYNAMICS2_PHANTOM_TYPE_H

/// All phantom types.
enum hkpPhantomType
{

		/// The type for hkpAabbPhantom
	HK_PHANTOM_AABB = 0,

		/// The type for hkpSimpleShapePhantom
	HK_PHANTOM_SIMPLE_SHAPE = 1,

		/// The type for hkpCachingShapePhantom
	HK_PHANTOM_CACHING_SHAPE = 2,

		//
		// User types
		//

		/// There are 10 user types HK_PHANTOM_USER0 to HK_PHANTOM_USER9, if you wish to implement your own phantom class
	HK_PHANTOM_USER0,
	HK_PHANTOM_USER1,
	HK_PHANTOM_USER2,
	HK_PHANTOM_USER3,
	HK_PHANTOM_USER4,
	HK_PHANTOM_USER5,
	HK_PHANTOM_USER6,
	HK_PHANTOM_USER7,
	HK_PHANTOM_USER8,
	HK_PHANTOM_USER9,
	
		//
		//	The end of the list
		//
	HK_PHANTOM_MAX_ID
};


#endif // HK_DYNAMICS2_PHANTOM_TYPES_H

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

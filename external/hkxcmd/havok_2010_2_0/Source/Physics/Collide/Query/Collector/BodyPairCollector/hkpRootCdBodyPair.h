/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ROOT_CD_BODY_PAIR_H
#define HK_ROOT_CD_BODY_PAIR_H

#include <Physics/Collide/Shape/hkpShape.h>

	/// As most of data in a hkBody is just temporary,
	/// we can only extract the root information from a hkBody
	///
	/// If you need more detailed information, you have to implement a collector
	/// yourself.
	///
	/// Note: If you use hkpWorld/hkpPhantom::getPenetrations(), than object A will always be the query
	/// object and object B will be the hit object
struct hkpRootCdBodyPair
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpRootCdBodyPair );

		/// The root collidable of object A
	const hkpCollidable*	m_rootCollidableA;

		/// The shape key of the colliding shape of object A
		/// Note: if your root collidable is a hkpMeshShape, this will be the subpart and triangle index
	hkpShapeKey			m_shapeKeyA;

		/// The root collidable of object B
	const hkpCollidable*	m_rootCollidableB;

		/// The shape key of the colliding shape of object B
	hkpShapeKey			m_shapeKeyB;
};


#endif //HK_ROOT_CD_BODY_PAIR_H

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

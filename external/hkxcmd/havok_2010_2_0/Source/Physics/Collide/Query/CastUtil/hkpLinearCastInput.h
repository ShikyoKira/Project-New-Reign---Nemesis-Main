/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_LINEAR_CAST_INPUT
#define HK_LINEAR_CAST_INPUT

#include <Common/Base/hkBase.h>

	/// This class is used for the input to linear casts for both hkpPhantom objects, and hkpWorld.
struct hkpLinearCastInput
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpLinearCastInput );

		/// The target position in world space
		/// Note: the start position is defined by the translation component of the collidable associated with the linear cast.
	hkVector4 m_to;

		/// If the cast is parallel to a given surface, then it does not report a hit
		/// as long it does not get closer than m_maxExtraPenetration
	hkReal	m_maxExtraPenetration;

		/// the collision tolerance for the start point collector
	hkReal	m_startPointTolerance;

	inline hkpLinearCastInput(): m_maxExtraPenetration( HK_REAL_EPSILON), m_startPointTolerance( HK_REAL_EPSILON ) {}
};


#endif //HK_LINEAR_CAST_INPUT

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

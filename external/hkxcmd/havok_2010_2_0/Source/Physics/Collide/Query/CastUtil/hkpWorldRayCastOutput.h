/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_WORLD_RAY_CAST_OUTPUT
#define HK_WORLD_RAY_CAST_OUTPUT

#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>


class hkpCollidable;

	/// A structure holding the raycast information of an hkpWorld::castRay() or hkpAabbPhantom::castRay()
struct hkpWorldRayCastOutput: public hkpShapeRayCastOutput
{


	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpWorldRayCastOutput );

		/// Constructor.
	inline hkpWorldRayCastOutput();

		/// Returns true if the raycast has hit an object
	inline hkBool hasHit() const;

	/// Comparison operator required for sorting
	inline hkBool operator<( const hkpWorldRayCastOutput& b ) const;


		/// Resets this structure if you want to reuse it for another raycast, by setting the hitFraction to 1
	inline void reset();

		/// The root collidable. Use getOwner() to get to the hkpEntity or hkpPhantom
	const hkpCollidable* m_rootCollidable;
};

	typedef hkpWorldRayCastOutput hkpWorldRayCastOutputPpu;
#	include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.inl>


#endif //HK_WORLD_RAY_CAST_OUTPUT

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

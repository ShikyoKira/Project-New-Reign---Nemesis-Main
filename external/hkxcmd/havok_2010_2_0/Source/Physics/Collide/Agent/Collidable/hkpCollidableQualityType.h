/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLIDABLE_QUALITY_TYPE_H
#define HK_COLLIDE2_COLLIDABLE_QUALITY_TYPE_H

	/// This type is used to specify the quality type for collidables (used by hkpRigidBody)
	/// This is mainly used to identify objects, which require the more expensive
	/// continuous simulation (compared to the faster, Havok2 style discrete) simulation.
	/// See the continuous simulation user guide, of the hkpCollisionDispatcher for more details
enum hkpCollidableQualityType
{
		/// Invalid or unassigned type. If you add a hkpRigidBody to the hkpWorld,
		/// this type automatically gets converted to either
		/// HK_COLLIDABLE_QUALITY_FIXED, HK_COLLIDABLE_QUALITY_KEYFRAMED or HK_COLLIDABLE_QUALITY_DEBRIS
	HK_COLLIDABLE_QUALITY_INVALID = -1,

		/// Use this for fixed bodies.
	HK_COLLIDABLE_QUALITY_FIXED = 0,

		/// Use this for moving objects with infinite mass.
	HK_COLLIDABLE_QUALITY_KEYFRAMED,

		/// Use this for all your debris objects.
	HK_COLLIDABLE_QUALITY_DEBRIS,

		/// Use this for debris objects that should have simplified TOI collisions with fixed/landscape objects.
	HK_COLLIDABLE_QUALITY_DEBRIS_SIMPLE_TOI,

		/// Use this for moving bodies, which should not leave the world,
		/// but you rather prefer those objects to tunnel through the world than
		/// dropping frames because the engine .
	HK_COLLIDABLE_QUALITY_MOVING,

		/// Use this for all objects, which you cannot afford to tunnel through
		/// the world at all.
	HK_COLLIDABLE_QUALITY_CRITICAL,

		/// Use this for very fast objects.
	HK_COLLIDABLE_QUALITY_BULLET,

		/// For user. If you want to use this, you have to modify hkpCollisionDispatcher::initCollisionQualityInfo()
	HK_COLLIDABLE_QUALITY_USER,

		/// Use this for rigid body character controllers.
	HK_COLLIDABLE_QUALITY_CHARACTER,

		/// Use this for moving objects with infinite mass which should report contact points and TOI-collisions against all other bodies, including other fixed and keyframed bodies.
		///
		/// Note that only non-TOI contact points are reported in collisions against debris-quality objects.
	HK_COLLIDABLE_QUALITY_KEYFRAMED_REPORTING,

		/// End of this list
	HK_COLLIDABLE_QUALITY_MAX
};


#endif // HK_COLLIDE2_COLLIDABLE_QUALITY_TYPE_H

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILS_GROUP_FILTER_UTIL_H
#define HK_UTILS_GROUP_FILTER_UTIL_H

#include <Common/Base/hkBase.h>

	/// A collection of helper utilities for setting up the group filter
class hkpGroupFilterUtil
{
	public:
			/// This utility class helps setting up hkpCollidable::m_collisionFilterInfo for the group filter.
			/// The purpose is to disable collisions between object pairs which are connected by a constraint.
			/// This class only works if all the rigid bodies and constraints
			/// can be organized in a hierarchy and the input constraints are sorted
			/// so that the root bodies come first.
			/// \param groupFilterSystemGroup is a unique identifier for this constraint system you can get by
			///       calling hkpGroupFilter::getNewSystemGroup(). This system group will by applied to the m_collisionFilterInfo
			///       of all involved bodies. If the bodies already have a system group assigned to, this function will assert.
		static void HK_CALL disableCollisionsBetweenConstraintBodies( const class hkpConstraintInstance*const* constraints, int numConstraints, int groupFilterSystemGroup );
};


#endif // HK_UTILS_GROUP_FILTER_UTIL_H

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

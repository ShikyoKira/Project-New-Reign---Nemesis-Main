/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_GROUP_FILTER_SETUP_H
#define HK_DYNAMICS2_GROUP_FILTER_SETUP_H

#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>

/// A helper class to create a nice predefined set of collision groups (of course you can use your own enums).
class hkpGroupFilterSetup
{
	public:
		/// Basic groups for game objects.
		enum DefaultCollisionGroups
		{
			/// collides always with everything (no layer collision detection used).
			LAYER_NONE = 0,

			/// collides with everything but LAYER_KEYFRAME.
			LAYER_STATIC,

			/// collides with everything.
			LAYER_DYNAMIC,

			/// collides with everything.
			LAYER_PLAYER,

			/// collides with everything.
			LAYER_AI,

			/// collides with everything but LAYER_KEYFRAME, LAYER_STATIC (good for keyframe animation).
			LAYER_KEYFRAME,

			/// collides with everything.
			LAYER_DEBRIS,

			/// collides with everything but LAYER_FAST_DEBRIS.
			LAYER_FAST_DEBRIS
		};

			/// initialize the group filter (see hkpGroupFilter for details).
		static void HK_CALL setupGroupFilter( hkpGroupFilter* filter )
		{
			hkUint32 allBits = (1<<LAYER_STATIC) | (1<<LAYER_DYNAMIC) | (1<<LAYER_PLAYER) | (1<<LAYER_AI) | (1<<LAYER_KEYFRAME) |(1<<LAYER_DEBRIS) | (1<<LAYER_FAST_DEBRIS);
			filter->enableCollisionsUsingBitfield ( allBits, allBits );
			filter->enableCollisionsUsingBitfield ( 1<<LAYER_NONE, 0xffffffff );
			filter->enableCollisionsUsingBitfield ( 0xffffffff, 1<<LAYER_NONE );
			filter->disableCollisionsBetween( LAYER_KEYFRAME,     LAYER_KEYFRAME);
			filter->disableCollisionsBetween( LAYER_KEYFRAME,     LAYER_STATIC );
			filter->disableCollisionsBetween( LAYER_FAST_DEBRIS,  LAYER_FAST_DEBRIS );
		}
};

#endif // HK_DYNAMICS2_GROUP_FILTER_SETUP_H

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

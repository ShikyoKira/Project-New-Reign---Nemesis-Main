/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DYNAMICS2_WORLD_POST_DETECTION_LISTENER_H
#define HK_DYNAMICS2_WORLD_POST_DETECTION_LISTENER_H

class hkpWorld;
class hkStepInfo;

	/// Derive from this class to receive callbacks after all collision detection has been performed for all entities
	/// Add to the world using hkpWorld::addWorldPostCollideListener
class hkpWorldPostCollideListener
{
	//+hk.MemoryTracker(ignore=True)
	public:

		virtual ~hkpWorldPostCollideListener() {}

			/// Called after collision detection is performed.
			/// The stepInfo passed to this function is the step info that was used to process
			/// the last psi (integrate and collide). When this function is called, the world
			/// current psi time will be the same as the stepInfo end time, (i.e., the psi time
			/// has been incremented), but the world current time will be the same as the step
			/// info start time, as it is not incremented until advanceTime is called.
		virtual void postCollideCallback( hkpWorld* world, const hkStepInfo& stepInfo ) = 0;
};

#endif // HK_DYNAMICS2_WORLD_POST_DETECTION_LISTENER_H

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

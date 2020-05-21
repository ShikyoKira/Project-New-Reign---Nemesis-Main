/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_PRODUCT_FEATURES_H
#define HK_PRODUCT_FEATURES_H

#include <Common/Base/hkBase.h>


	/// Singleton class whose constructor by default registers all product features.
	/// The constructor is generated as a side effect of #include<Common/Base/Config/hkProductFeatures.cxx>
class hkProductFeatures: public hkReferencedObject, public hkSingleton<hkProductFeatures>
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

		hkProductFeatures();
		~hkProductFeatures() {}
};


// You only need to look further if you're doing something specialized.
// Mostly you'll just use the macros in hkProductFeatures.txt to control
// what gets linked or stripped.


//
// Common
//
		/// You'll need this in order to load assets which were generated prior to version 7.0.0.
		/// It's rather large and shouldn't be linked unless you definitely need it.
	extern void HK_CALL hkFeature_serializeDeprecatedPre700();

//
// Physics
//
#ifdef HK_FEATURE_PRODUCT_PHYSICS
		/// Register height field collision agent
	extern void HK_CALL hkpHeightFieldAgent_registerSelf();

		/// Register constructor and aabbQuery for AABB tree world manager
	extern void HK_CALL hkpAabbTreeWorldManager_registerSelf();
		/// Register constructor and aabbQuery for KdTree world manager
	extern void HK_CALL hkpKdTreeWorldManager_registerSelf();

		/// Calculate accurate inertia tensor for convex hulls, otherwise calculate based on their bounding boxes
	extern void HK_CALL hkpAccurateInertiaTensorComputer_registerSelf();

		/// You'll need at least one simulation type for most of the functions in hkpWorld
	extern void HK_CALL hkpSimulation_registerSelf();
		/// You'll need at least one simulation type for most of the functions in hkpWorld
	extern void HK_CALL hkpContinuousSimulation_registerSelf();

	#if (HK_CONFIG_THREAD != HK_CONFIG_SINGLE_THREADED)
		/// You'll need at least one simulation type for most of the functions in hkpWorld
	extern void HK_CALL hkpMultiThreadedSimulation_registerSelf();
	#endif

		/// Register the Compound Shape and Compound Agents
	extern void HK_CALL hkpHybridBroadPhase_registerSelf();
#endif

#endif//HK_PRODUCT_FEATURES_H

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

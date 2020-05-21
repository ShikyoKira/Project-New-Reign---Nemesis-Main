/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This file contains a list of feature macros. Generally you'll copy
// it to a local file ("MyFeatures.h"), tweak the defines and use it thus:
// #include <keycode.cxx>
// #include "MyFeatures.h"
// #include <Common/Base/Config/hkProductFeatures.cxx>


//////////////////
// Product inclusion
//////////////////

// The first block adds product support at a coarse level.
// Normally, these defines are automatically set by inclusion
// of keycode.inl. The detected values may be #undef-ed to
// override the product selection.
// #define HK_FEATURE_PRODUCT_AI
// #define HK_FEATURE_PRODUCT_ANIMATION
// #define HK_FEATURE_PRODUCT_CLOTH
// #define HK_FEATURE_PRODUCT_DESTRUCTION
// #define HK_FEATURE_PRODUCT_BEHAVIOR
// #define HK_FEATURE_PRODUCT_PHYSICS


//////////////////
// Library exclusion
//////////////////

// Finer grained stripping support at a library level.
// Some products are composed of several libraries, not all of which need be used.

	// Ai
// #define HK_EXCLUDE_LIBRARY_hkaiAiPhysicsBridge

	// Animation
// #define HK_EXCLUDE_LIBRARY_hkaRagdoll

	// Behavior
// #define HK_EXCLUDE_LIBRARY_hkbUtilities

	// Cloth
// #define HK_EXCLUDE_LIBRARY_hclPhysicsBridge
// #define HK_EXCLUDE_LIBRARY_hclSetup
// #define HK_EXCLUDE_LIBRARY_hclAnimationBridge

	// Physics
// #define HK_EXCLUDE_LIBRARY_hkpUtilities
// #define HK_EXCLUDE_LIBRARY_hkpVehicle

	// Common
// #define HK_EXCLUDE_LIBRARY_hkSceneData
// #define HK_EXCLUDE_LIBRARY_hkVisualize
// #define HK_EXCLUDE_LIBRARY_hkGeometryUtilities
// #define HK_EXCLUDE_LIBRARY_hkCompat

	// Convex Decomposition
// #define HK_EXCLUDE_LIBRARY_hkgpConvexDecomposition

//////////////////
// Individual feature exclusion
//////////////////

// At the finest level we can exclude individual features
// from libraries.

	// This is support for packfiles before 7.0.0 and for xml packfiles.
	// You can use the asset compiler AssetCc2 to bring old files up to date
	// as an offline process instead of linking all the versioning support.
	// You can optionally define HK_COMPAT_FILE to choose a subset of compatible
	// versions for some data savings.
// #define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700

	// This is support for versioning packfiles and tagfiles from 7.0.0 onwards.
	// This is much smaller than the pre700 data. Note that if you need to load
	// packfiles which are not the current version, you'll generally need metadata
	// for that version either in the file or compiled into the executable.
// #define HK_EXCLUDE_FEATURE_RegisterVersionPatches

	// hkClass reflection is needed to use havok serialization.
	// You may also define HK_CLASSES_FILE to trim down the list of
	// linked classes. Alternatively excluding libraries and/or other
	// features will cause less classes to be registered.
	// You can turn this feature off if you're registering classes
	// elsewhere or don't need havok serialization.
// #define HK_EXCLUDE_FEATURE_RegisterReflectedClasses

	// Turn off support for the new memory reporting system.
	// This saves some static data.
// #define HK_EXCLUDE_FEATURE_MemoryTracker

	// Convex decomposition is mostly used in offline processing.
// #define HK_EXCLUDE_FEATURE_ConvexDecomposition


// #define HK_EXCLUDE_FEATURE_hkMonitorStream
// #define HK_EXCLUDE_FEATURE_hkpAabbTreeWorldManager
// #define HK_EXCLUDE_FEATURE_hkpAccurateInertiaTensorComputer
// #define HK_EXCLUDE_FEATURE_hkpCompressedMeshShape
// #define HK_EXCLUDE_FEATURE_hkpContinuousSimulation
// #define HK_EXCLUDE_FEATURE_hkpConvexPieceMeshShape
// #define HK_EXCLUDE_FEATURE_hkpExtendedMeshShape
// #define HK_EXCLUDE_FEATURE_hkpHeightField
// #define HK_EXCLUDE_FEATURE_hkpKdTreeWorldManager
// #define HK_EXCLUDE_FEATURE_hkpMeshShape
// #define HK_EXCLUDE_FEATURE_hkpMultiThreadedSimulation
// #define HK_EXCLUDE_FEATURE_hkpPoweredChainData
// #define HK_EXCLUDE_FEATURE_hkpSimpleMeshShape
// #define HK_EXCLUDE_FEATURE_hkpSimulation

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

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>

extern void HK_CALL hkpCollidehkpHeightFieldRegister();
extern void HK_CALL hkpCollidehkpCompressedMeshShapeRegister();
extern void HK_CALL hkpCollidehkpConvexPieceMeshShapeRegister();
extern void HK_CALL hkpCollidehkpExtendedMeshShapeRegister();
extern void HK_CALL hkpCollidehkpMeshShapeRegister();
extern void HK_CALL hkpCollidehkpSimpleMeshShapeRegister();
extern void HK_CALL hkpDynamicshkpPoweredChainDataRegister();
extern void HK_CALL hkBasehkMonitorStreamRegister();

extern void HK_CALL hkaiAiPhysicsBridgeRegister();

extern void HK_CALL hkaiInternalClientRegister();
extern void HK_CALL hkaiInternalInternalRegister();
extern void HK_CALL hkaiInternalRegister();
extern void HK_CALL hkaiPathfindingRegister();

extern void HK_CALL hkaAnimationRegister();
extern void HK_CALL hkaInternalInternalRegister();
extern void HK_CALL hkaInternalRegister();
extern void HK_CALL hkaRagdollRegister();

extern void HK_CALL hkbBehaviorClientRegister();
extern void HK_CALL hkbBehaviorRegister();
extern void HK_CALL hkbUtilitiesRegister();

extern void HK_CALL hclAnimationBridgeRegister();

extern void HK_CALL hclPhysicsBridgeRegister();

extern void HK_CALL hclClothRegister();
extern void HK_CALL hclSetupRegister();

extern void HK_CALL hkdDestructionRegister();
extern void HK_CALL hkdInternalInternalRegister();
extern void HK_CALL hkdInternalRegister();

extern void HK_CALL hkpCollideClientRegister();
extern void HK_CALL hkpCollideRegister();
extern void HK_CALL hkpConstraintSolverClientRegister();
extern void HK_CALL hkpConstraintSolverInternalRegister();
extern void HK_CALL hkpConstraintSolverRegister();
extern void HK_CALL hkpDynamicsClientRegister();
extern void HK_CALL hkpDynamicsRegister();
extern void HK_CALL hkpInternalClientRegister();
extern void HK_CALL hkpInternalInternalRegister();
extern void HK_CALL hkpInternalRegister();
extern void HK_CALL hkpUtilitiesRegister();
extern void HK_CALL hkpVehicleRegister();
extern void HK_CALL hkpInternalHybridBroadPhaseRegister();

extern void HK_CALL hkBaseRegister();
extern void HK_CALL hkCompatRegister();
extern void HK_CALL hkgpConvexDecompositionInternalRegister();
extern void HK_CALL hkgpConvexDecompositionRegister();
extern void HK_CALL hkGeometryUtilitiesRegister();
extern void HK_CALL hkInternalClientRegister();
extern void HK_CALL hkInternalInternalRegister();
extern void HK_CALL hkInternalRegister();
extern void HK_CALL hkSceneDataRegister();
extern void HK_CALL hkSerializeRegister();
extern void HK_CALL hkVisualizeRegister();

static void hkRegisterMemoryTracker()
{
	// In alphabetical order by product
#if defined(HK_MEMORY_TRACKER_ENABLE) && !defined(HK_EXCLUDE_FEATURE_MemoryTracker)


	#if defined(HK_FEATURE_PRODUCT_AI)
		hkaiInternalClientRegister();
		hkaiInternalInternalRegister();
		hkaiInternalRegister();

		hkaiPathfindingRegister();

		#if defined(HK_FEATURE_PRODUCT_PHYSICS) && !defined(HK_EXCLUDE_LIBRARY_hkaiAiPhysicsBridge)
			hkaiAiPhysicsBridgeRegister();
		#endif
	#endif

	
	#if defined(HK_FEATURE_PRODUCT_ANIMATION)
		hkaAnimationRegister();

		hkaInternalInternalRegister();
		hkaInternalRegister();

		#if defined(HK_FEATURE_PRODUCT_PHYSICS) && !defined(HK_EXCLUDE_LIBRARY_hkaRagdoll)
			hkaRagdollRegister();
		#endif
	#endif


	#if defined(HK_FEATURE_PRODUCT_BEHAVIOR)
		hkbBehaviorClientRegister();
		hkbBehaviorRegister();

		#if !defined(HK_EXCLUDE_LIBRARY_hkbUtilities)
			hkbUtilitiesRegister();
		#endif
	#endif


	#if defined(HK_FEATURE_PRODUCT_CLOTH)
		hclClothRegister();

		#if !defined(HK_EXCLUDE_LIBRARY_hclSetup)
			hclSetupRegister();
		#endif

		#if defined(HK_FEATURE_PRODUCT_ANIMATION) && !defined(HK_EXCLUDE_LIBRARY_hclAnimationBridge)
			hclAnimationBridgeRegister();
		#endif

		#if defined(HK_FEATURE_PRODUCT_PHYSICS) && !defined(HK_EXCLUDE_LIBRARY_hclPhysicsBridge)
			hclPhysicsBridgeRegister();
		#endif
	#endif

	#if 1 // PRODUCT_Common
		hkBaseRegister();

		#if !defined(HK_EXCLUDE_LIBRARY_hkCompat)
			hkCompatRegister();
		#endif

		#if !defined(HK_EXCLUDE_LIBRARY_hkSceneData)
			hkSceneDataRegister();
		#endif

		#if !defined(HK_EXCLUDE_LIBRARY_hkSerialize)
			hkSerializeRegister();
		#endif

		#if !defined(HK_EXCLUDE_LIBRARY_hkInternal)
			hkInternalClientRegister();
			hkInternalInternalRegister();
			hkInternalRegister();
		#endif

		#if !defined(HK_EXCLUDE_LIBRARY_hkGeometryUtilities)
			hkGeometryUtilitiesRegister();
		#endif

		#if !defined(HK_EXCLUDE_LIBRARY_hkVisualize)
			hkVisualizeRegister();
		#endif

		#if !defined(HK_EXCLUDE_FEATURE_hkMonitorStream)
			hkBasehkMonitorStreamRegister();
		#endif
	#endif


	#if defined(HK_FEATURE_PRODUCT_DESTRUCTION)
		hkdDestructionRegister();
		hkdInternalInternalRegister();
		hkdInternalRegister();

		// Convex Decomposition is treated as a Destruction component for memory tracking purposes.
		#if !defined(HK_EXCLUDE_LIBRARY_hkgpConvexDecomposition)
			hkgpConvexDecompositionInternalRegister();
			hkgpConvexDecompositionRegister();
		#endif
	#endif


	#if defined(HK_FEATURE_PRODUCT_PHYSICS)
		hkpConstraintSolverClientRegister();
		hkpConstraintSolverInternalRegister();
		hkpConstraintSolverRegister();

		hkpCollideClientRegister();
		hkpCollideRegister();
		#if !defined(HK_EXCLUDE_FEATURE_hkpHeightField)
			hkpCollidehkpHeightFieldRegister();
		#endif
		#if !defined(HK_EXCLUDE_FEATURE_hkpCompressedMeshShape)
			hkpCollidehkpCompressedMeshShapeRegister();
		#endif
		#if !defined(HK_EXCLUDE_FEATURE_hkpConvexPieceMeshShape)
			hkpCollidehkpConvexPieceMeshShapeRegister();
		#endif
		#if !defined(HK_EXCLUDE_FEATURE_hkpExtendedMeshShape)
			hkpCollidehkpExtendedMeshShapeRegister();
		#endif
		#if !defined(HK_EXCLUDE_FEATURE_hkpMeshShape)
			hkpCollidehkpMeshShapeRegister();
		#endif
		#if !defined(HK_EXCLUDE_FEATURE_hkpSimpleMeshShape)
			hkpCollidehkpSimpleMeshShapeRegister();
		#endif

		hkpDynamicsClientRegister();
		hkpDynamicsRegister();
		hkpDynamicshkpPoweredChainDataRegister();
		
		hkpInternalClientRegister();
		hkpInternalInternalRegister();

#if !defined(HK_EXCLUDE_FEATURE_HybridBroadPhase)
		hkpInternalHybridBroadPhaseRegister();
#endif
		hkpInternalRegister();

		#if !defined(HK_EXCLUDE_LIBRARY_hkpUtilities)
			hkpUtilitiesRegister();
		#endif
		#if !defined(HK_EXCLUDE_LIBRARY_hkpVehicle)
			hkpVehicleRegister();
		#endif
	#endif
#endif
}

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

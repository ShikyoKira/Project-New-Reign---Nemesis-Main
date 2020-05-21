// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Animation specific product patches.
// This file is #included by hkaPatches.cpp

//=======
// 650b1
//=======

HK_PATCH_BEGIN("hkaSkeletonMapperData", 0, "hkaSkeletonMapperData", 1)
        HK_PATCH_MEMBER_ADDED("extractedMotionMapping", TYPE_VEC_12, HK_NULL, 0)
        HK_PATCH_MEMBER_ADDED_INT("mappingType", 0)
HK_PATCH_END()
HK_PATCH_BEGIN("hkaSkeleton", 0, "hkaSkeleton", 1)
	HK_PATCH_MEMBER_RENAMED("bones", "old_bones")
	HK_PATCH_MEMBER_ADDED("bones", TYPE_ARRAY_STRUCT, "hkaBone", 0)
	HK_PATCH_FUNCTION(hkaSkeleton_0_to_1)
	HK_PATCH_MEMBER_REMOVED("old_bones", TYPE_ARRAY_OBJECT, "hkaBone", 0)
	HK_PATCH_DEPENDS("hkaBone", 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkaSkeleton", 1, "hkaSkeleton", 2)
	HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()

HK_PATCH_BEGIN("hkaAnimationBinding", 0, "hkaAnimationBinding", 1)
	HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()

HK_PATCH_BEGIN("hkaBoneAttachment", 0, "hkaBoneAttachment", 1)
	HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()

HK_PATCH_BEGIN("hkaMeshBinding", 0, "hkaMeshBinding", 1)
	HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()

HK_PATCH_BEGIN("hkaAnimation", 0, "hkaAnimation", 1)
	HK_PATCH_MEMBER_RENAMED("annotationTracks", "old_annotationTracks")
	HK_PATCH_MEMBER_ADDED("annotationTracks", TYPE_ARRAY_OBJECT, "hkaAnnotationTrack", 0)
	HK_PATCH_FUNCTION(hkaAnimation_0_to_1)
	HK_PATCH_MEMBER_REMOVED("old_annotationTracks", TYPE_ARRAY_OBJECT, "hkaAnnotationTrack", 0)
	HK_PATCH_DEPENDS("hkaAnnotationTrack", 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkaAnimationContainer", 0, "hkaAnimationContainer", 1)
	HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()


HK_PATCH_BEGIN("hkaAnimationPreviewColorContainer", 0, "hkaAnimationPreviewColorContainer", 1)
		HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
        HK_PATCH_MEMBER_REMOVED("previewColor", TYPE_ARRAY_OBJECT, "hkaAnimationPreviewColor", 0)
        HK_PATCH_MEMBER_ADDED("previewColor", TYPE_ARRAY_INT, HK_NULL, 0)
        HK_PATCH_DEPENDS("hkaAnimationPreviewColor", 0)
	// This class cannot be exported.  Hence it is not versioned.
HK_PATCH_END()



HK_PATCH_BEGIN("hkaAnimationPreviewColor", 0, HK_NULL, HK_CLASS_REMOVED)
        HK_PATCH_MEMBER_REMOVED("color", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()


HK_PATCH_BEGIN("hkaFootstepAnalysisInfo", 0, "hkaFootstepAnalysisInfo", 1)
        HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()

HK_PATCH_BEGIN("hkaFootstepAnalysisInfoContainer", 0, "hkaFootstepAnalysisInfoContainer", 1)
        HK_PATCH_PARENT_SET(HK_NULL, "hkReferencedObject")
HK_PATCH_END()

HK_PATCH_BEGIN("hkaSkeleton", 2, "hkaSkeleton", 3)
        HK_PATCH_MEMBER_ADDED("referenceFloats", TYPE_ARRAY_REAL, HK_NULL, 0)
		HK_PATCH_FUNCTION(hkaSkeleton_2_to_3)
HK_PATCH_END()

HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkaQuantizedAnimationTrackCompressionParams", 0)
        HK_PATCH_MEMBER_ADDED("rotationTolerance", TYPE_REAL, HK_NULL, 0)
        HK_PATCH_MEMBER_ADDED("translationTolerance", TYPE_REAL, HK_NULL, 0)
        HK_PATCH_MEMBER_ADDED("scaleTolerance", TYPE_REAL, HK_NULL, 0)
        HK_PATCH_MEMBER_ADDED("floatingTolerance", TYPE_REAL, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkaQuantizedAnimation", 0)
        HK_PATCH_PARENT_SET(HK_NULL, "hkaAnimation")
        HK_PATCH_MEMBER_ADDED("data", TYPE_ARRAY_BYTE, HK_NULL, 0)
        HK_PATCH_MEMBER_ADDED("endian", TYPE_INT, HK_NULL, 0)
        HK_PATCH_DEPENDS("hkBaseObject", 0)
        HK_PATCH_DEPENDS("hkaAnimation", 1)
        HK_PATCH_DEPENDS("hkReferencedObject", 0)
HK_PATCH_END()

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

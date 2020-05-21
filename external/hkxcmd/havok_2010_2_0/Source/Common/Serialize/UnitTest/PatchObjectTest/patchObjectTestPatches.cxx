// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "PatchObjectTestSubClass", 0)
	HK_PATCH_MEMBER_ADDED("testBigMember", TYPE_INT, HK_NULL, 0)
	HK_PATCH_MEMBER_ADDED("testRealMember", TYPE_REAL, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "PatchObjectTest", 0)
	HK_PATCH_MEMBER_ADDED("firstMember", TYPE_INT, HK_NULL, 0)
	HK_PATCH_MEMBER_ADDED("testStructArray", TYPE_ARRAY_STRUCT, "PatchObjectTestSubClass", 0)
	HK_PATCH_MEMBER_ADDED("middleMember", TYPE_INT, HK_NULL, 0)
	HK_PATCH_MEMBER_ADDED("embeddedStruct", TYPE_STRUCT, "PatchObjectTestSubClass", 0)
	HK_PATCH_MEMBER_ADDED("lastMember", TYPE_INT, HK_NULL, 0)
	HK_PATCH_DEPENDS("PatchObjectTestSubClass", 0)
HK_PATCH_END()

HK_PATCH_BEGIN("PatchObjectTestSubClass", 0, "PatchObjectTestSubClass", 1)
	HK_PATCH_MEMBER_REMOVED("testRealMember", TYPE_REAL, HK_NULL, 0)
	HK_PATCH_MEMBER_ADDED("testAddedMember", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "PatchObjectTestExtraAdded", 0)
	HK_PATCH_MEMBER_ADDED_INT("extraInt", 1)
HK_PATCH_END()

HK_PATCH_BEGIN("PatchObjectTestSubClass", 1, "PatchObjectTestSubClass", 2)
	HK_PATCH_MEMBER_ADDED("testEmbedStructAdd", TYPE_STRUCT, "PatchObjectTestExtraAdded", 0)
	HK_PATCH_FUNCTION(PatchObjectTestSubClass_1_to_2)
	HK_PATCH_DEPENDS("PatchObjectTestExtraAdded", 0)
HK_PATCH_END()

HK_PATCH_BEGIN("PatchObjectTest", 0, "PatchObjectTest", 1)
	HK_PATCH_FUNCTION(PatchObjectTest_0_to_1)
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

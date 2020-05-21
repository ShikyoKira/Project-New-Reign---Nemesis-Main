// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// ---------------------
// C(100-102) -> NONE
// ---------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestC", 100) // 0
	HK_PATCH_MEMBER_ADDED("number1", TYPE_INT, HK_NULL, 0)
	HK_PATCH_MEMBER_ADDED("number2", TYPE_INT, HK_NULL, 0)
	HK_PATCH_MEMBER_ADDED("number3", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestC", 100, "hkTestC", 101) // 1
	HK_PATCH_MEMBER_REMOVED("number1", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestC", 101, "hkTestC", 102) // 2
	HK_PATCH_MEMBER_REMOVED("number2", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestC", 102, HK_NULL, HK_CLASS_REMOVED) // 3
	HK_PATCH_MEMBER_REMOVED("number3", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

// ---------------------
// A(0-2...) -> C(0-1)
// ---------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestA", 0) // 4
	HK_PATCH_MEMBER_ADDED("object", TYPE_OBJECT, "hkTestC", 0)
	HK_PATCH_MEMBER_ADDED("float", TYPE_REAL, HK_NULL, 0)
	HK_PATCH_DEPENDS("hkTestC", 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestA", 0, "hkTestA", 1) // 5
	HK_PATCH_DEPENDS("hkTestC", 1)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestA", 1, "hkTestA", 2) // 6
	HK_PATCH_MEMBER_REMOVED("object", TYPE_OBJECT, "hkTestC", 0)
	HK_PATCH_DEPENDS("hkTestC", 1)
HK_PATCH_END()

// ---------------------------
// D(0-2) -> C(101-102), A(0-1)
// ---------------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestD", 0) // 7
	HK_PATCH_MEMBER_ADDED("object", TYPE_OBJECT, "hkTestC", 0)
	HK_PATCH_DEPENDS("hkTestC", 101)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestD", 0, "hkTestD", 1) // 8
	HK_PATCH_MEMBER_REMOVED("object", TYPE_OBJECT, "hkTestC", 0)
	HK_PATCH_MEMBER_ADDED("number", TYPE_INT, HK_NULL, 0)
	HK_PATCH_DEPENDS("hkTestC", 102)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestD", 1, "hkTestD", 2) // 9
	HK_PATCH_MEMBER_ADDED("object", TYPE_OBJECT, "hkTestA", 0)
	HK_PATCH_DEPENDS("hkTestA", 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestD", 2, HK_NULL, HK_CLASS_REMOVED) // 10
	HK_PATCH_MEMBER_REMOVED("object", TYPE_OBJECT, "hkTestA", 0)
	HK_PATCH_MEMBER_REMOVED("number", TYPE_INT, HK_NULL, 0)
	HK_PATCH_DEPENDS("hkTestA", 1)
HK_PATCH_END()

// ---------------------
// B(0-1...) -> C(2)
// ---------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestB", 0) // 11
	HK_PATCH_MEMBER_ADDED("string", TYPE_CSTRING, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestB", 0, "hkTestB", 1) // 12
	HK_PATCH_MEMBER_ADDED("object", TYPE_OBJECT, "hkTestC", 0)
	HK_PATCH_DEPENDS("hkTestC", 2)
HK_PATCH_END()

// ---------------------
// C(0-1) -> NONE
// ---------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestC", 0) // 13
	HK_PATCH_MEMBER_ADDED("string", TYPE_CSTRING, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestC", 0, "hkTestC", 1) // 14
	HK_PATCH_MEMBER_ADDED("number", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestC", 1, HK_NULL, HK_CLASS_REMOVED) // 15
	HK_PATCH_MEMBER_REMOVED("string", TYPE_CSTRING, HK_NULL, 0)
	HK_PATCH_MEMBER_REMOVED("number", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

// ---------------------
// C(2-...) -> NONE
// ---------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestC", 2) // 16
	HK_PATCH_MEMBER_ADDED("userData", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestC", 2, "hkTestC", 3) // 17
	HK_PATCH_MEMBER_ADDED("userData2", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

// ---------------------
// PatchObject(0) -> (2)
// ---------------------
HK_PATCH_BEGIN(HK_NULL, HK_CLASS_ADDED, "hkTestPatchObject", 0)
	HK_PATCH_MEMBER_ADDED("intMember", TYPE_INT, HK_NULL, 0)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestPatchObject", 0, "hkTestPatchObject", 1)
	HK_PATCH_MEMBER_ADDED("structMember", TYPE_STRUCT, "hkTestA", 0)
	HK_PATCH_DEPENDS("hkTestA", 2)
HK_PATCH_END()

HK_PATCH_BEGIN("hkTestPatchObject", 1, "hkTestPatchObject", 2)
	HK_PATCH_FUNCTION(hkTestPatchObject_1_to_2)
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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_PATCHOBJECT_TEST_H
#define HK_PATCHOBJECT_TEST_H

#include <Common/Serialize/hkSerialize.h>

extern const class hkClass PatchObjectTestClass;
extern const class hkClass PatchObjectTestSubClassClass;

// define to write tagfile, undefine to read
//#define WRITE_TAGFILE

#ifdef WRITE_TAGFILE
struct PatchObjectTest {
	//+version(0)
public:
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, PatchObjectTest);

	hkInt32 m_firstMember;
	struct SubClass {
		//+version(0)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, PatchObjectTest::SubClass);
		hkInt64 m_testBigMember;
		hkReal m_testRealMember;
	};
	hkArray<SubClass> m_testStructArray;
	hkInt32 m_middleMember;
	SubClass m_embeddedStruct;
	hkInt32 m_lastMember;
};
#else

extern const class hkClass PatchObjectTestExtraAddedClass;

struct PatchObjectTestExtraAdded
{
	//+version(0)
public:
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, PatchObjectTestExtraAdded);

	hkInt32 m_extraInt;
};

struct PatchObjectTest {
	//+version(1)
public:
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, PatchObjectTest);

	hkInt32 m_firstMember;
	struct SubClass {
		//+version(2)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, PatchObjectTest::SubClass);
		hkInt32 m_testAddedMember;
		hkInt64 m_testBigMember;
		PatchObjectTestExtraAdded m_testEmbedStructAdd;
	};
	hkArray<SubClass> m_testStructArray;
	hkInt32 m_middleMember;
	SubClass m_embeddedStruct;
	hkInt32 m_lastMember;
};

#endif

#endif

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

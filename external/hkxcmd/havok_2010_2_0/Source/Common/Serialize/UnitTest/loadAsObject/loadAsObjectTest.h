/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKSERIALIZE_LOAD_AS_OBJECT
#define HKSERIALIZE_LOAD_AS_OBJECT

#include <Common/Base/UnitTest/hkUnitTest.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Math/hkMath.h>
#include <Common/Base/Reflection/hkTypeInfo.h>

struct TestSimpleStruct
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestSimpleStruct);

	hkInt32 m_int;
	hkReal m_real;
};

extern const hkClass TestSimpleStructClass;
extern const hkTypeInfo TestSimpleStructTypeInfo;

// Unsupported data

struct TestUnsupportedCstring
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestUnsupportedCstring);

	TestUnsupportedCstring()
	{
		m_string = "Test string.";
	}

	TestUnsupportedCstring(hkFinishLoadedObjectFlag f)
	{
		if( f.m_finishing )
		{
			HK_TEST(m_string != HK_NULL);
		}
	}

	const char* m_string;
};

extern const hkClass TestUnsupportedCstringClass;
extern const hkTypeInfo TestUnsupportedCstringTypeInfo;

struct TestUnsupportedCstringArray
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestUnsupportedCstringArray);

	TestUnsupportedCstringArray()
	{
		m_strings.expandOne() = "Test string item.";
	}

	TestUnsupportedCstringArray(hkFinishLoadedObjectFlag f) : m_strings(f)
	{
		if( f.m_finishing )
		{
			HK_TEST(m_strings.getSize() == 1);
			HK_TEST(m_strings[0] != HK_NULL);
		}
	}

	hkArray<const char*> m_strings;
};

extern const hkClass TestUnsupportedCstringArrayClass;
extern const hkTypeInfo TestUnsupportedCstringArrayTypeInfo;

struct TestUnsupportedVariant
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestUnsupportedVariant);

	TestUnsupportedVariant()
	{
		m_variant.m_class = &TestUnsupportedCstringClass;
		m_variant.m_object = new TestUnsupportedCstring();
	}

	TestUnsupportedVariant(hkFinishLoadedObjectFlag f)
	{
		if( f.m_finishing )
		{
			HK_TEST(m_variant.m_class == &TestUnsupportedCstringClass);
			HK_TEST(m_variant.m_object != HK_NULL);
			m_variant.m_object = new TestUnsupportedCstring();
		}
	}

	~TestUnsupportedVariant()
	{
		delete static_cast<TestUnsupportedCstring*>(m_variant.m_object);
	}

	hkVariant m_variant;
};

extern const hkClass TestUnsupportedVariantClass;
extern const hkTypeInfo TestUnsupportedVariantTypeInfo;

struct TestUnsupportedVariantArray
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestUnsupportedVariantArray);

	TestUnsupportedVariantArray()
	{
		hkVariant& v = m_variant.expandOne();
		v.m_class = &TestUnsupportedCstringClass;
		v.m_object = new TestUnsupportedCstring();
	}

	TestUnsupportedVariantArray(hkFinishLoadedObjectFlag f) : m_variant(f)
	{
		if( f.m_finishing )
		{
			HK_TEST(m_variant.getSize() == 1);
			HK_TEST(m_variant[0].m_class == &TestUnsupportedCstringClass);
			HK_TEST(m_variant[0].m_object != HK_NULL);
			m_variant[0].m_object = new TestUnsupportedCstring();
		}
	}

	~TestUnsupportedVariantArray()
	{
		delete static_cast<TestUnsupportedCstring*>(m_variant[0].m_object);
	}

	hkArray<hkVariant> m_variant;
};

extern const hkClass TestUnsupportedVariantArrayClass;
extern const hkTypeInfo TestUnsupportedVariantArrayTypeInfo;

struct TestUnsupportedSimpleArray
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestUnsupportedSimpleArray);

	TestUnsupportedSimpleArray()
	{
		static int v = -1;
		m_ints = &v;
		m_numInts = 1;
	}

	TestUnsupportedSimpleArray(hkFinishLoadedObjectFlag f)
	{
		if( f.m_finishing )
		{
			HK_TEST(m_numInts == 1);
			HK_TEST(m_ints != HK_NULL);
			HK_TEST(m_ints[0] == -1);
		}
	}

	int* m_ints;
	int m_numInts;
};

extern const hkClass TestUnsupportedSimpleArrayClass;
extern const hkTypeInfo TestUnsupportedSimpleArrayTypeInfo;

struct TestUnsupportedHomogeneousArray
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestUnsupportedHomogeneousArray);

	TestUnsupportedHomogeneousArray()
	{
		m_dataClass = &TestUnsupportedCstringClass;
		m_data = new TestUnsupportedCstring();
		m_numData = 1;
	}

	TestUnsupportedHomogeneousArray(hkFinishLoadedObjectFlag f)
	{
		if( f.m_finishing )
		{
			HK_TEST(m_numData == 1);
			HK_TEST(m_dataClass == &TestUnsupportedCstringClass);
			HK_TEST(m_data != HK_NULL);
			m_data = new TestUnsupportedCstring();
		}
	}

	~TestUnsupportedHomogeneousArray()
	{
		delete static_cast<TestUnsupportedCstring*>(m_data);
	}

	const hkClass* m_dataClass;
	void* m_data;
	int m_numData;
};

extern const hkClass TestUnsupportedHomogeneousArrayClass;
extern const hkTypeInfo TestUnsupportedHomogeneousArrayTypeInfo;

struct TestMemberOfUnsupportedClass
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestMemberOfUnsupportedClass);

	TestMemberOfUnsupportedClass() {}
	TestMemberOfUnsupportedClass(hkFinishLoadedObjectFlag f) : m_simpleArray(f)
	{
	}

	struct ::TestUnsupportedSimpleArray m_simpleArray;
};

extern const hkClass TestMemberOfUnsupportedClassClass;
extern const hkTypeInfo TestMemberOfUnsupportedClassTypeInfo;
// Supported data examples

class TestSupportedVirtual : public hkReferencedObject
{
	// +vtable(true)
	public:
		HK_DECLARE_REFLECTION();

		struct SimpleData
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestSupportedVirtual::SimpleData);

			hkReal m_real;
			hkInt32 m_int;
		};

		TestSupportedVirtual()
		{
			m_string = "Test string";
			m_stringArray.expandOne() = "Test string item.";
			m_half = 0.1f;
			SimpleData data;
			data.m_real = 10.0f;
			data.m_int = -1;
			m_simpleData.expandOne() = data;
		}

		TestSupportedVirtual(hkFinishLoadedObjectFlag f) :
			hkReferencedObject(f),
			m_string(f),
			m_stringArray(f),
			m_simpleData(f)
		{
			if( f.m_finishing )
			{
				HK_TEST(m_string != HK_NULL);
				HK_TEST(m_stringArray.getSize() == 1);
				HK_TEST(m_stringArray[0] != HK_NULL);
				HK_TEST(m_half == hkHalf(0.1f));
				HK_TEST(m_simpleData.getSize() == 1);
				HK_TEST(m_simpleData[0].m_real == 10.0f);
				HK_TEST(m_simpleData[0].m_int == -1);
				if( getAllocatedSize() != 0 )
				{
					hkUlong strAddr = *reinterpret_cast<hkUlong*>(&m_string);
					HK_TEST((strAddr & hkStringPtr::OWNED_FLAG) == hkStringPtr::OWNED_FLAG);
					strAddr = *reinterpret_cast<hkUlong*>(&m_stringArray[0]);
					HK_TEST((strAddr & hkStringPtr::OWNED_FLAG) == hkStringPtr::OWNED_FLAG);
				}
			}
		}

	private:

		hkStringPtr m_string;
		hkArray<hkStringPtr> m_stringArray;
		hkHalf m_half;
		hkArray<struct SimpleData> m_simpleData;
		hkRefVariant m_variant;
		hkArray<hkRefVariant> m_variants;
};

extern const hkClass TestSupportedVirtualClass;
extern const hkTypeInfo TestSupportedVirtualTypeInfo;
extern const hkClass TestSupportedVirtualSimpleDataClass;
extern const hkTypeInfo TestSupportedVirtualSimpleDataTypeInfo;

class TestSupportedNonVirtual
{
	public:
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, TestSupportedNonVirtual);

		TestSupportedNonVirtual()
		{
			m_object = new TestSupportedVirtual();
		}

		TestSupportedNonVirtual(hkFinishLoadedObjectFlag f)
		{
			if( f.m_finishing )
			{
				HK_TEST(m_object != HK_NULL);
				HK_TEST(m_object->getReferenceCount() == 1);
			}
		}

		~TestSupportedNonVirtual()
		{
			m_object->removeReference();
		}

	private:

		class ::TestSupportedVirtual* m_object;
		hkArray<struct ::TestSimpleStruct> m_simple;
};

extern const hkClass TestSupportedNonVirtualClass;
extern const hkTypeInfo TestSupportedNonVirtualTypeInfo;

#endif // HKSERIALIZE_LOAD_AS_OBJECT

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

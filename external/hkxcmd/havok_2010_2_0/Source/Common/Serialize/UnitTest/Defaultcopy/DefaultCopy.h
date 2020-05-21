/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_COPY_DEFAULT
#define HKSERIALIZE_COPY_DEFAULT


extern const class hkClass Original_WithNestedNestedClass;

extern const class hkClass Modified_WithNestedNestedClass;

extern const class hkClass Original_DefaultCopyClass;
class Original_DefaultCopy
{
	public:
		HK_DECLARE_REFLECTION();
};

extern const class hkClass Modified_DefaultCopyClass;
class Modified_DefaultCopy
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Modified_DefaultCopy );

		HK_DECLARE_REFLECTION();

		enum Value
		{
			VALUE_FIRST=111,
			VALUE_SECOND=156,
			VALUE_THIRD=123,
			VALUE_ONLY_IN_MODIFIED=99
		};

		int m_int0; //+default(100) +absmin(0)
		hkBool m_bool0; //+default(true)
		hkBool m_bool1; //+default(false)
		hkBool m_bool2; //+default(true)
		hkEnum<Value, hkInt8> m_value8; //+default( Modified_DefaultCopy::VALUE_THIRD )
		hkVector4 m_vec0;
		hkVector4 m_vec1; //+default(44,55,66,77)
		hkVector4 m_vec2; //+default(88,99,11)
};

extern const class hkClass Original_WithNestedClass;
struct Original_WithNested
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Original_WithNested );
	HK_DECLARE_REFLECTION();

	Original_WithNested() : m_foo(100), m_bar(200.4f) {	}

	struct Nested
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Original_WithNested::Nested );
		HK_DECLARE_REFLECTION();
		Nested() : m_enabled(false), m_radius(10) {}
		hkBool m_enabled;
		float m_radius;
	};
	int m_foo;
	struct Nested m_nested;
	float m_bar;
};

extern const class hkClass Modified_WithNestedClass;
struct Modified_WithNested
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Modified_WithNested );
	HK_DECLARE_REFLECTION();

	struct Nested
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Modified_WithNested::Nested );
		HK_DECLARE_REFLECTION();
		int m_pad[2];
		hkBool m_enabled2; //+default(true)
		float m_radius;
	};

	int m_foo;
	int m_foo2;
	struct Nested m_nested;
	float m_bar;
	float m_bar2;
};

#endif // HKSERIALIZE_COPY_DEFAULT

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

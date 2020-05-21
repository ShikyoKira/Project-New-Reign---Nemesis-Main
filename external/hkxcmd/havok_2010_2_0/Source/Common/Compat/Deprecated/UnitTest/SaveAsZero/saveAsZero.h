/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_SAVE_AS_ZERO_H
#define HKSERIALIZE_SAVE_AS_ZERO_H

extern const class hkClass TestZeroClass;
class TestZero
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, TestZero );

		HK_DECLARE_REFLECTION();

		enum Value
		{
			VALUE_0=0,
			VALUE_1=1,
			VALUE_2=2,
			VALUE_3=3,
			VALUE_8=8,
			VALUE_16=16,
			VALUE_32=32,
		};

		TestZero()
			:
			m_value8( VALUE_8 ),
			m_value16( VALUE_16 ),
			m_value32( VALUE_32 ),
			m_zero8( VALUE_8 ),
			m_zero16( VALUE_16 ),
			m_zero32( VALUE_32 )
		{
		}

	public:

		hkEnum<Value, hkInt8> m_value8;
		hkEnum<Value, hkInt16> m_value16;
		hkEnum<Value, hkInt32> m_value32;
		hkEnum<Value, hkInt8> m_zero8; //+nosave
		hkEnum<Value, hkInt16> m_zero16; //+nosave
		hkEnum<Value, hkInt32> m_zero32; //+nosave
};

extern const hkClass TestArrayNullsClass;
class TestArrayNulls
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, TestArrayNulls );

		HK_DECLARE_REFLECTION();

		TestArrayNulls()
		{
			m_array.pushBack(HK_NULL);
			m_array.pushBack(this);
			m_array.pushBack(HK_NULL);
			m_array.pushBack(HK_NULL);
			m_array.pushBack(this);
			m_array.pushBack(HK_NULL);
		}

		hkArray<class ::TestArrayNulls*> m_array;
};

#endif // HKSERIALIZE_SAVE_AS_ZERO_H

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

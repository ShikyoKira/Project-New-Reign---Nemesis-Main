/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSERIALIZE_ENUM_COPY_HKCLASS_H
#define HKSERIALIZE_ENUM_COPY_HKCLASS_H

extern const class hkClass OriginalClass;
class Original
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Original );

		HK_DECLARE_REFLECTION();

		enum Value
		{
			VALUE_FIRST=1,
			VALUE_SECOND=2,
			VALUE_THIRD=3,
			VALUE_ONLY_IN_ORIGINAL=4
		};

		Original() { }

	public:

		hkEnum<Value, hkInt8> m_value8;
		hkEnum<Value, hkInt16> m_value16;
		hkEnum<Value, hkInt32> m_value32;
		hkEnum<Value, hkInt32> m_valueBad0;
		hkEnum<Value, hkInt32> m_valueBad1;
};
extern const class hkClass ModifiedClass;
class Modified
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Modified );

		HK_DECLARE_REFLECTION();

		enum Value
		{
			VALUE_FIRST=111,
			VALUE_SECOND=156,
			VALUE_THIRD=123,
			VALUE_ONLY_IN_MODIFIED=99
		};

		Modified() { }

	public:

		int m_padpad[10];
		hkEnum<Value, hkInt8> m_value8;
		hkEnum<Value, hkInt16> m_value16;
		hkEnum<Value, hkInt32> m_value32;
		hkEnum<Value, hkInt32> m_valueBad0;
		hkEnum<Value, hkInt32> m_valueBad1;
};
#endif // HKSERIALIZE_ENUM_COPY_HKCLASS_H

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

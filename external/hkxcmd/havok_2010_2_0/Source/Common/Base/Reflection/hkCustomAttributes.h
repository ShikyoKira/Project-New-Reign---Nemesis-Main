/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_CUSTOM_ATTRIBUTE_H
#define HK_BASE_CUSTOM_ATTRIBUTE_H


extern const class hkClass hkCustomAttributesAttributeClass;

extern const class hkClass hkCustomAttributesClass;

class hkCustomAttributes
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, hkCustomAttributes );

		const hkVariant* getAttribute(const char* id) const
		{
			for( int i = 0; i < m_numAttributes; ++i )
			{
				if( hkString::strCmp( m_attributes[i].m_name, id ) == 0 )
				{
					return &m_attributes[i].m_value;
				}
			}
			return HK_NULL;
		}

		struct Attribute
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, hkCustomAttributes::Attribute );
			const char* m_name;
			hkVariant m_value;
		};

	private:

		const struct Attribute* m_attributes;
		int m_numAttributes;	
};

#endif // HK_BASE_CUSTOM_ATTRIBUTE_H

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

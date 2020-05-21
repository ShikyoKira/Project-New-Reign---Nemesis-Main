/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_UNION_H
#define HK_BASE_UNION_H

#include <Common/Base/Reflection/hkClassMember.h>

/// Class that can be set to hold any type specified in hkClassMember::Type.
/// Basic POD types are copied, objects are referenced.
class hkTypedUnion
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkTypedUnion);

			/// Empty constructor.
		inline hkTypedUnion();

			/// Return HK_SUCCESS if the value held is of the right type, HK_FAILURE otherwise.
		/*template<typename VALUE_T>
		inline hkResult getValue(VALUE_T& dest) const;
		
			/// Sets the value for basic types
		template<typename VALUE_T>
		inline void setSimple(const VALUE_T& orig);*/

			/// For pointers to reflected objects, you need to provide the hkClass too.
		inline void setObject(const void* orig, const hkClass* klass);

			/// For enum values, you need to provide the hkClass too.
		inline void setEnum(int value, const hkClassEnum* eclass);

			/// Generic setValue, only works for POD types.
		inline hkResult setSimple(const void* orig, const hkClassMember::Type type);

			/// Like a hkVariant, but for enums.
		struct EnumVariant
		{
			int m_value;
			const hkClassEnum* m_enum;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkTypedUnion::EnumVariant );
		};
		
			///
		union Storage
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkTypedUnion::Storage );

			bool m_bool;
			char m_char;
			hkInt8 m_int8;
			hkUint8 m_uint8;
			hkInt16 m_int16;
			hkUint16 m_uint16;
			hkInt32 m_int32;
			hkUint32 m_uint32;
			hkInt64 m_int64;
			hkUint64 m_uint64;
			float m_float;
			float m_float4[4];
			float m_float12[12];
			float m_float16[16];
			hkVariant m_variant;
			EnumVariant m_enumVariant;
			hkUlong m_ulong;
			hkInt32 m_maxSize[16];
		};

			/// Query the stored type.
		inline hkClassMember::Type getType() const;

			/// Access the stored value.
		inline const Storage& getStorage() const;
		
	private:   

			/// Type of the value held.
		hkEnum<hkClassMember::Type, hkInt32> m_type;

			/// Storage element.
		HK_ALIGN16( Storage m_elem );  
};

#include <Common/Base/Types/hkTypedUnion.inl>

#endif //HK_BASE_UNION_H

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

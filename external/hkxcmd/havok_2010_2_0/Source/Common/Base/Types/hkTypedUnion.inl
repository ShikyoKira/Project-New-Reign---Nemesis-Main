/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/*
// Default empty
template<typename VALUE_T>
struct hkTypedUnion_value_traits;

// supported types
template<>
struct hkTypedUnion_value_traits<hkBool>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_BOOL; }
};

template<>
struct hkTypedUnion_value_traits<bool> : public hkTypedUnion_value_traits<hkBool> { };

template<>
struct hkTypedUnion_value_traits<char>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_CHAR; }
};

template<>
struct hkTypedUnion_value_traits<hkInt8>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_INT8; }
};

template<>
struct hkTypedUnion_value_traits<hkUint8>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_UINT8; }
};

template<>
struct hkTypedUnion_value_traits<hkInt16>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_INT16; }
};

template<>
struct hkTypedUnion_value_traits<hkUint16>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_UINT16; }
};

template<>
struct hkTypedUnion_value_traits<hkInt32>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_INT32; }
};

template<>
struct hkTypedUnion_value_traits<hkUint32>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_UINT32; }
};

template<>
struct hkTypedUnion_value_traits<hkInt64>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_INT64; }
};

template<>
struct hkTypedUnion_value_traits<hkUint64>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_UINT64; }
};

template<>
struct hkTypedUnion_value_traits<float>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_REAL; }
};

template<>
struct hkTypedUnion_value_traits<hkVector4>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_VECTOR4; }
};

template<>
struct hkTypedUnion_value_traits<hkQuaternion>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_QUATERNION; }
};

template<>
struct hkTypedUnion_value_traits<hkVariant>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_POINTER; }
};

template<>
struct hkTypedUnion_value_traits<char*>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_CSTRING; }
};

template<typename VALUE_T>
struct hkTypedUnion_value_traits<VALUE_T*>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_POINTER; }
};

template<>
struct hkTypedUnion_value_traits<hkUlong>
{
	static inline hkClassMember::Type type() { return hkClassMember::TYPE_ULONG; }
};

template<typename VALUE_T>
inline hkResult hkTypedUnion::getValue(VALUE_T& dest) const
{
	if(m_type == hkTypedUnion_value_traits<VALUE_T>::type() )
	{
		HK_COMPILE_TIME_ASSERT( sizeof(dest) < sizeof(m_elem) );
		hkString::memCpy( &dest, &m_elem, sizeof(dest));
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

template<typename VALUE_T>
inline void hkTypedUnion::setSimple(const VALUE_T& orig) 
{
	m_elem.m_maxSize[0] = 0;
	m_elem.m_maxSize[1] = 0;
	m_elem.m_maxSize[2] = 0;
	m_elem.m_maxSize[3] = 0;
	// set new type and value
	m_type = hkTypedUnion_value_traits<VALUE_T>::type();
	//HK_ASSERT(0x10d761b4,  );
	hkString::memCpy( &m_elem, &orig, sizeof(orig) );
}

*/
///////////////////////////////////////////   Public access methods

inline hkTypedUnion::hkTypedUnion() :
	m_type(hkClassMember::TYPE_VOID)
{
}

inline void hkTypedUnion::setObject(const void* orig, const hkClass* klass)
{
	m_type = hkClassMember::TYPE_POINTER;
	m_elem.m_variant.m_object = const_cast<void*>(orig);
	m_elem.m_variant.m_class = klass;
}

inline void hkTypedUnion::setEnum(int val, const hkClassEnum* eklass)
{
	m_type = hkClassMember::TYPE_ENUM;
	m_elem.m_enumVariant.m_value = val;
	m_elem.m_enumVariant.m_enum = eklass;
}

inline hkResult hkTypedUnion::setSimple(const void* orig, const hkClassMember::Type type)
{
	HK_ASSERT(0x773e3daa,
		(type >= hkClassMember::TYPE_BOOL && type <= hkClassMember::TYPE_TRANSFORM)
		|| (type >= hkClassMember::TYPE_VARIANT && type <= hkClassMember::TYPE_STRINGPTR) );
	const hkClassMember::TypeProperties& props = hkClassMember::getClassMemberTypeProperties(type);
	if( unsigned(props.m_size) <= sizeof(m_elem))
	{
		m_type = type;
		hkString::memCpy(&m_elem, orig, props.m_size);
		return HK_SUCCESS;
	}
	return HK_FAILURE;
}

inline hkClassMember::Type hkTypedUnion::getType() const
{
	return m_type;
}

inline const hkTypedUnion::Storage& hkTypedUnion::getStorage() const
{
	return m_elem;
}

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

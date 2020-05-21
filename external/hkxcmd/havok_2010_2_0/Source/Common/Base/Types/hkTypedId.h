/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_TYPEDID_H
#define HK_BASE_TYPEDID_H

#include <Common/Base/Reflection/hkClassMember.h>

/// Type safe nullable and orderable ID wrapper.
/// Meant for safer typedefs (i.e.: typedef hkUint16 ThingId) and actual value usages explicit.
/// Use HK_DECLARE_TYPED_ID( name , type , invalid value ) macro for declarations.
template <typename TYPE, int INVALID_VALUE, typename DISCRIMINANT>
struct hkTypedId
{
	/// Underlying type.
	typedef TYPE	Type;

	/// Invalid value.
	enum { InvalidValue = INVALID_VALUE };
	
	/// Only allow for explicit or default construction.
	explicit HK_FORCE_INLINE			hkTypedId(int v=INVALID_VALUE) : m_value((TYPE)v) {}
	
	/// Check for '=='.
	HK_FORCE_INLINE bool				operator ==(hkTypedId other) const	{ return m_value == other.value(); }

	/// Check for '!='.
	HK_FORCE_INLINE bool				operator !=(hkTypedId other) const	{ return m_value != other.value(); }

	/// Check for '<'.
	HK_FORCE_INLINE bool				operator <(hkTypedId other) const	{ return m_value < other.value(); }

	/// Check for '<='.
	HK_FORCE_INLINE bool				operator <=(hkTypedId other) const	{ return m_value <= other.value(); }

	/// Check for '>'.
	HK_FORCE_INLINE bool				operator >(hkTypedId other) const	{ return m_value > other.value(); }

	/// Check for '>='.
	HK_FORCE_INLINE bool				operator >=(hkTypedId other) const	{ return m_value >= other.value(); }	

	/// Increment ID.
	HK_FORCE_INLINE void				increment()							{ ++m_value; }

	/// Decrement ID.
	HK_FORCE_INLINE void				decrement()							{ --m_value; }

	/// Read-only access to the actual index value.
	HK_FORCE_INLINE	TYPE				value() const						{ return m_value; }

	/// Read-write access to the actual index value.
	HK_FORCE_INLINE	TYPE&				value()								{ return m_value; }
	
	/// Check if the ID is valid ( != INVALID_VALUE / != invalid() )
	HK_FORCE_INLINE bool				isValid() const						{ return m_value != INVALID_VALUE; }
	
	/// Get an invalid ID ( equivalent to hkTypedId() ).
	static HK_FORCE_INLINE hkTypedId	invalid()							{ return hkTypedId(); }

protected:

	TYPE	m_value;	///< Actual ID value.
};

#define HK_DECLARE_TYPED_ID(_name_ , _type_ , _nullvalue_) struct _name_##Disciminant {}; typedef hkTypedId<_type_,_nullvalue_,_name_##Disciminant> _name_;

#endif //HK_BASE_TYPEDID_H

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

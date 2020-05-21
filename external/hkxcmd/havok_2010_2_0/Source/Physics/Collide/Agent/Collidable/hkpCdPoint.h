/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CD_POINT
#define HK_COLLIDE2_CD_POINT

#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>

class hkpCdBody;
	/// A simple structure used for a contact point between two hkpCdBody objects
	/// IMPORTANT NOTE: The distance value in the contact point is a fraction (between 0 and 1) when this struct is
	/// used with linear cast
struct hkpCdPoint
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpCdPoint);

		/// Constructor takes two hkpCdBody objects
	hkpCdPoint( const hkpCdBody& a, const hkpCdBody& b ) : m_cdBodyA(a), m_cdBodyB(b) {}
	hkpCdPoint( const hkpCdBody& a, const hkpCdBody& b, const hkContactPoint& cp ) : m_contact(cp), m_cdBodyA(a), m_cdBodyB(b) { m_unweldedNormal = cp.getNormal(); }
	HK_FORCE_INLINE const hkContactPoint& getContact() const { return m_contact; }
	HK_FORCE_INLINE void setContact(const hkContactPoint& cp) { m_contact = cp; m_unweldedNormal = cp.getNormal(); }
	HK_FORCE_INLINE void setContact(hkVector4Parameter pos, hkVector4Parameter norm, hkReal d) { m_contact.set(pos,norm,d); m_unweldedNormal = norm; }
	HK_FORCE_INLINE void setContact(hkVector4Parameter pos, hkVector4Parameter norm, hkVector4Parameter unweldedNorm, hkReal d) { m_contact.set(pos,norm,d); m_unweldedNormal = unweldedNorm; }
	HK_FORCE_INLINE void setContactDistance(hkReal d) { m_contact.setDistance(d); }
	HK_FORCE_INLINE void setUnweldedNormal(hkVector4Parameter norm) { m_unweldedNormal = norm; }
	HK_FORCE_INLINE const hkVector4& getUnweldedNormal() const { return m_unweldedNormal; }

protected:
		/// The contact point
	hkContactPoint m_contact;

		/// The unwelded contact normal
	hkVector4 m_unweldedNormal;
public:

		/// body A
	const hkpCdBody& m_cdBodyA;

		/// body B
	const hkpCdBody& m_cdBodyB;
};

#endif // HK_COLLIDE2_CD_POINT

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

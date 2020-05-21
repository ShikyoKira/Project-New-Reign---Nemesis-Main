/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HKNP_AABB_H
#define HKNP_AABB_H


struct hkAabb16	
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkAabb16 );

		hkAabb16(): m_key(0), m_key1(0){}

		HK_FORCE_INLINE	friend hkBool32 operator < (const hkAabb16& a, const hkAabb16& b ){ return a.m_min[0] < b.m_min[0]; }

		template<typename KeyType>
		HK_FORCE_INLINE	void setKey(KeyType key);

		HK_FORCE_INLINE	void getKey(hkUint16& key) const;
		HK_FORCE_INLINE	void getKey(hkUint32& key) const;

		HK_FORCE_INLINE	void setEmpty();

		HK_FORCE_INLINE	bool isValid() const;

		HK_FORCE_INLINE	void getExtent(hkUint16 extent[3]) const;

		HK_FORCE_INLINE	hkUint16 getMajorExtent() const;

		HK_FORCE_INLINE	hkBool32 disjoint(const hkAabb16& testAabb) const;

		HK_FORCE_INLINE	hkBool32 overlaps(const hkUint16 p[3]) const;

		HK_FORCE_INLINE	void includePoint( hkUint16 c[3] );

		HK_FORCE_INLINE	void includeAabb( const hkAabb16& in );

		/// returns true if test AABB is fully inside this, touching the border is allowed
		HK_FORCE_INLINE	hkBool32 contains( const hkAabb16& testAabb ) const;

		/// returns true if test AABB is fully inside this and is not touching the border
		HK_FORCE_INLINE	hkBool32 containsDontTouch( const hkAabb16& testAabb ) const;

		HK_FORCE_INLINE	void getCenter(hkVector4& c) const;

		HK_FORCE_INLINE	void getCenter(hkUint16 c[3]) const;

		HK_FORCE_INLINE	void getCenter2(hkUint32 c[3]) const;

		static HK_FORCE_INLINE hkUint32 yzDisjoint( const hkAabb16& a, const hkAabb16& b );

		HK_FORCE_INLINE hkBool32 equals(const hkAabb16& aabb) const;

	public:
		HK_ALIGN16(hkUint16 m_min[3]);
		hkUint16 m_key;
		hkUint16 m_max[3];
		hkUint16 m_key1;
};


#include <Common/Base/Types/Geometry/Aabb16/hkAabb16.inl>

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

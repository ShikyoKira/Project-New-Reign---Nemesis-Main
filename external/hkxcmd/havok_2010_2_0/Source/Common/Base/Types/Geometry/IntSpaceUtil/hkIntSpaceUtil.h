/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKNP_INT_SPACE_UTIL_H
#define HKNP_INT_SPACE_UTIL_H

#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Types/Geometry/Aabb16/hkAabb16.h>

class hkIntSpaceUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CDINFO, hkIntSpaceUtil );

		void set( const hkAabb& aabb );

		HK_FORCE_INLINE void convertWithOffsetLow( const hkVector4& vIn, hkUint16 vOut[3] ) const;

		HK_FORCE_INLINE	void convertAabb(const hkAabb& aabbF, hkAabb16& aabbOut) const;
		HK_FORCE_INLINE	void restoreAabb(const hkAabb16& aabbI, hkAabb& aabbOut) const;

		void debugDisplayAabb(const hkAabb16& aabb) const;

	public:

		enum { AABB_UINT16_MAX_VALUE = 0x7fff };
		static const hkQuadReal s_aabb16Max;

		hkVector4 m_bitOffsetLow;
		hkVector4 m_bitOffsetHigh;
		hkVector4 m_bitScale;
		hkVector4 m_bitScaleInv;
};

#include <Common/Base/Types/Geometry/IntSpaceUtil/hkIntSpaceUtil.inl>


#endif // HKNP_INT_SPACE_UTIL_H

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

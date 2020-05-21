/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_COLLIDE2_26DOP_H
#define HK_COLLIDE2_26DOP_H

#include <Common/Base/hkBase.h>


///
/// A 26 KDOP. It stores the distance to each plane without the div by root2 and root3 etc.
/// so call getLowDistance(i) and getHighDistance(i) to get the actual distance to the planes on the axis i.
///
class hkp26Dop
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkp26Dop);

		/// Expand (extend) the extents by the given amount, by just adding the given value to each of the extents
		inline void expand(const hkReal tolerance);

		/// Translate the KDOP.
		void translate( const hkVector4& t );

		inline hkReal getLow(int i ) const;
		inline hkReal getHigh(int i ) const;

		inline void setLow(int i,hkReal l );
		inline void setHigh(int i, hkReal h);

		inline hkReal getLowDistance(int i ) const;
		inline hkReal getHighDistance(int i ) const;

		void getPlaneEquations( const hkTransform& dopToWorld, hkVector4* planeEquationsOut );

		static inline const hkVector4& getAxis(int i);

		enum DOP_AXIS
		{
			AXIS_X     = 0,
			AXIS_Y     = 1,
			AXIS_Z     = 2,
			AXIS_YZ    = 3,
			AXIS_YMZ   = 4,
			AXIS_XZ    = 5,
			AXIS_XMZ   = 6,
			AXIS_XY    = 7,
			AXIS_XMY   = 8,
			AXIS_XYZ   = 9,
			AXIS_XYMZ  = 10,
			AXIS_XMYZ  = 11,
			AXIS_XMYMZ = 12
		};

	public:

		// :DON'T CHANGE ORDER: Indexed directly by the set and get (i) methods.

		// The 6DOP (AABB for the OBB)
		hkReal m_lx, m_hx;
		hkReal m_ly, m_hy;
		hkReal m_lz, m_hz;

		// The remaining 20 extents of the 26DOP

		hkReal m_lyz,    m_hyz;	  //YZ
		hkReal m_lymz,   m_hymz;  //YMZ

		hkReal m_lxz,    m_hxz;	  //XZ
		hkReal m_lxmz,   m_hxmz;  //XMZ

		hkReal m_lxy,    m_hxy;	  //XY
		hkReal m_lxmy,   m_hxmy;  //XMY

		hkReal m_lxyz,   m_hxyz;  //XYZ
		hkReal m_lxymz,  m_hxymz; //XYMZ

		hkReal m_lxmyz,  m_hxmyz; //XMYZ
		hkReal m_lxmymz, m_hxmymz;//XMYMZ

		static const hkReal ROOT2;
		static const hkReal ROOT3;
};

#include <Physics/Internal/Collide/Mopp/Machine/hkp26Dop.inl>

#endif // HK_COLLIDE2_26DOP_H

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VISUALIZE_SHAPE_SEMICIRCLE_H
#define HK_VISUALIZE_SHAPE_SEMICIRCLE_H

#include <Common/Visualize/Shape/hkDisplayGeometry.h>

class hkDisplaySemiCircle : public hkDisplayGeometry
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkDisplaySemiCircle(const hkVector4& center, 
							const hkVector4& normal, 
							const hkVector4& startPerp,
							const hkReal thetaMin,
							const hkReal thetaMax,
							const hkReal radius,
							const int numSegments);

		hkDisplaySemiCircle();
		
		virtual void buildGeometry();

		virtual void getWireframeGeometry( hkArrayBase<hkVector4>& lines, hkMemoryAllocator& a );

		void setParameters(const hkReal radius, 
						   const hkReal thetaMin,
						   const hkReal thetaMax,
						   const int numSegments, 
						   const hkVector4& center,
						   const hkVector4& normal, 
						   const hkVector4& startPerp);

		// get normal to plane containing semi-circle
		hkVector4 getNormal();
	
		// get center of semi-circle in WS
		hkVector4 getCenter();
		
		// get perp to normal in WS
		hkVector4 getPerp();
		
		// get min angular extent
		hkReal getThetaMin();
		
		// get max angular extent
		hkReal getThetaMax();
		
		// get radius
		hkReal getRadius();

		int getNumSegments();

	protected:

		hkVector4 m_normal;
		
		hkVector4 m_perp;
		
		hkVector4 m_center;
		
		hkReal m_thetaMin;
		
		hkReal m_thetaMax;
		
		hkReal m_radius;
		
		int m_numSegments;

		void generatePoints(hkArray<hkVector4>& points);
};


#endif // HK_VISUALIZE_SHAPE_SEMICIRCLE_H

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

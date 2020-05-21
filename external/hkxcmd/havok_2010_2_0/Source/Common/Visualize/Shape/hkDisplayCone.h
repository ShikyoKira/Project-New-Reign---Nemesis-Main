/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_SHAPE_CONE_H
#define HK_VISUALIZE_SHAPE_CONE_H

#include <Common/Visualize/Shape/hkDisplayGeometry.h>

class hkDisplayCone : public hkDisplayGeometry
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkDisplayCone(const hkReal coneAngle, 
					  const hkReal coneHeight,
					  const int numSegments, 
					  const hkVector4& coneAxis, 
					  const hkVector4& startPos);

		hkDisplayCone();
		
		void setParameters(const hkReal coneAngle, 
						   const hkReal coneHeight,
						   const int numSegments, 
						   const hkVector4& coneAxis, 
						   const hkVector4& startPos);
		
		virtual void buildGeometry();
		
		virtual void getWireframeGeometry( hkArrayBase<hkVector4>& lines, hkMemoryAllocator& a );

		// get apex of cone in WS
		hkVector4 getPosition();
		
		// get axis of cone in WS
		hkVector4 getAxis();
		
		// get height along axis
		hkReal getHeight();
		
		// get angle of cone
		hkReal getAngle();
		
		// get number of segments
		int getNumSegments();


	protected:

		hkVector4 m_startPos;

		hkVector4 m_coneAxis;
		
		int m_numSegments;
		
		hkReal m_coneAngle;
		
		hkReal m_coneHeight;

		void generateConeVertices(hkArray<hkVector4>& conePoints);
};


#endif // HK_VISUALIZE_SHAPE_CONE_H

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

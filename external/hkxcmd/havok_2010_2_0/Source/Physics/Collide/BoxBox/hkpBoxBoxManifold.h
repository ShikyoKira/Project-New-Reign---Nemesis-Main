/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_FEATURE_MANIFOLD_H
#define HK_COLLIDE2_FEATURE_MANIFOLD_H

#include <Physics/Collide/BoxBox/hkpBoxBoxContactPoint.h>

class hkpCdBody;
struct hkpProcessCollisionOutput;
struct hkpCollisionInput;


#define HK_BOXBOX_MANIFOLD_MAX_POINTS 8

class hkpBoxBoxManifold
{
 
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpBoxBoxManifold );

		hkpBoxBoxManifold();

		// for 2.0 bridge
		int addPoint( const hkpCdBody& bodyA, const hkpCdBody& bodyB, hkpFeatureContactPoint& fcp );

		void removePoint( int index );

		inline int getNumPoints();

		inline hkpFeatureContactPoint& operator[]( int index );

		inline hkBool findInManifold( const hkpFeatureContactPoint& fcp );

		inline hkBool isComplete();
		inline void setComplete( hkBool complete );
		
		inline hkBool hasNoPointsLeft();

	public:
		hkpFeatureContactPoint m_contactPoints[HK_BOXBOX_MANIFOLD_MAX_POINTS];
		
		hkUchar  m_faceVertexFeatureCount;
		hkUchar  m_numPoints;
		
		hkBool m_isComplete;  // a complete manifold is one where no new points can be added via small rotations.
		hkBool m_manifoldNormalInitialized;

		hkUint32 m_manifoldNormalB;

		hkVector4 m_manifoldNormalA;
};


#include <Physics/Collide/BoxBox/hkpBoxBoxManifold.inl>


#endif // HK_COLLIDE2_FEATURE_MANIFOLD_H

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

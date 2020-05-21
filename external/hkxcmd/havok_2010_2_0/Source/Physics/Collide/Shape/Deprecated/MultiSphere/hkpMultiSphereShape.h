/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_MULTI_SPHERE_SHAPE_H
#define HK_COLLIDE2_MULTI_SPHERE_SHAPE_H

#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Shape/HeightField/hkpSphereRepShape.h>

extern const hkClass hkpMultiSphereShapeClass;

/// DEPRECATED. This shape will be removed in future releases of Havok Physics.
///
/// A compound shape made up of a number of spheres. This is useful as a fast approximation for complex surfaces,
/// as collision detection for spheres is very fast.
/// However, if two hkpMultiSphereShape shapes collide, every sphere needs to be checked against every other sphere.
/// E.g. 10 spheres colliding with 10 spheres will result in 100 collision checks. Therefore higher order shapes like
/// hkpCapsuleShape or hkpConvexVerticesShape should be preferred.
class hkpMultiSphereShape : public hkpSphereRepShape
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

            ///
		enum
		{
				/// The maximum number of spheres allowed in this hkpMultiSphereShape.
			MAX_SPHERES = 8
		};

			/// The w component of each vector3 is the sphere radius.
		hkpMultiSphereShape(const hkVector4* spheres, int numSpheres);

			/// Get the shape's hkVector4 array that defines its spheres.
			/// Note: the .w component of the hkVector4 contains the radius of the sphere.
		inline const hkVector4* getSpheres() const;

			/// Get the number of spheres in the shape
		inline int getNumSpheres() const;


		//
		// hkpSphereRepShape implementation
		//

			/// hkpSphereRepShape implementation
		virtual int getNumCollisionSpheresImpl( )  const;

			/// hkpSphereRepShape implementation
		virtual const hkSphere* getCollisionSpheresImpl( hkSphere* sphereBuffer ) const;


		//
		// hkpShape implementation
		//

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

			// hkpShape implementation
 		virtual void getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) const;

			/// output.m_extrainfo is set to the index of the sphere which is hit.
		virtual hkBool castRayImpl( const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& output ) const;

			// hkpShape implementation
		virtual void castRayWithCollectorImpl( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

	protected:

		int		m_numSpheres;
		hkVector4 m_spheres[8/*hkpMultiSphereShape::MAX_SPHERES*/];

	public:

		hkpMultiSphereShape( hkFinishLoadedObjectFlag flag ) : hkpSphereRepShape(flag) { m_type = HK_SHAPE_MULTI_SPHERE; }

};

#include <Physics/Collide/Shape/Deprecated/MultiSphere/hkpMultiSphereShape.inl>

#endif // HK_COLLIDE2_MULTI_SPHERE_SHAPE_H

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

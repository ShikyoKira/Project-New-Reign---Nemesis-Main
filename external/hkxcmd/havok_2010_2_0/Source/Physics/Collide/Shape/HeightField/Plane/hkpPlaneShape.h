/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_PLANE_SHAPE_H
#define HK_COLLIDE2_PLANE_SHAPE_H

#include <Physics/Collide/Shape/HeightField/hkpHeightFieldShape.h>

extern const hkClass hkpPlaneShapeClass;

	/// A hkpPlaneShape containing a normal and distance from the origin.
	/// This plane shape is also bounded by a given AABB.
	/// The plane shape does not collide with its edges.
	/// The user has to make sure, that the AABB is set correctly, else
	/// collision will be filtered away. This includes that the AABB is slightly bigger
	/// than the shape (use the tolerance as the thickness in the positive direction and
	/// some 'maximum penetration depth' in the other.
class hkpPlaneShape : public hkpHeightFieldShape
{
	//+hk.ReflectedFile("hkpHeightField")
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Create a hkpPlaneShape from a normal and a distance (from the origin to the hkpPlaneShape, in the opposite direction of the normal).
			/// Also you need to specify an AABB, which is used to restrict the extents of the shape
		hkpPlaneShape(const hkVector4& plane, const hkAabb& aabb);

			/// Create a planeshape using a given direction, a center point and the halfExtents
		hkpPlaneShape( const hkVector4& direction, const hkVector4& center, const hkVector4& halfExtents );

			///	hkpShape Interface implementation
 		virtual void getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out  ) const;

        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

			/// hkpShape interface implementation
		virtual hkBool castRayImpl(const hkpShapeRayCastInput& input, hkpShapeRayCastOutput& results) const;

			/// collector driven raycast implementation using the data driven
		virtual void castRayWithCollectorImpl( const hkpShapeRayCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

			/// cast a sphere
		virtual void castSphere( const hkpSphereCastInput& input, const hkpCdBody& cdBody, hkpRayHitCollector& collector ) const;

			/// hkpHeightFieldShape interface implementation.
		virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const;

			/// get the plane equation.
		inline const hkVector4& getPlane() const { return m_plane; }

			/// Get the center of the AABB.
		inline const hkVector4& getAabbCenter() const { return m_aabbCenter; }

			/// Get the half-extents of the AABB.
		inline const hkVector4& getAabbHalfExtents() const { return m_aabbHalfExtents; }

	protected:
		hkVector4 m_plane;
		hkVector4 m_aabbCenter;
		hkVector4 m_aabbHalfExtents;

	public:

		hkpPlaneShape( hkFinishLoadedObjectFlag flag ) : hkpHeightFieldShape(flag) { m_type = HK_SHAPE_PLANE; }

};


#endif // HK_COLLIDE2_PLANE_SHAPE_H

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

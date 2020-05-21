/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_AABB_H
#define HK_MATH_AABB_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkAabbClass;

extern const class hkClass hkAabbUint32Class;

/// Axis aligned bounding box.
class hkAabb
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkAabb);
		HK_DECLARE_REFLECTION();

			/// An empty constructor, does not initialize anything.
		hkAabb() { }

			///Creates a new AABB with the specified dimensions.
		HK_FORCE_INLINE hkAabb(const hkVector4& min, const hkVector4& max);

			/// Returns true if the given AABB overlaps with this one. Zero volume overlaps are reported as an overlap.
		HK_FORCE_INLINE hkBool32 overlaps( const hkAabb& testAabb ) const;

			/// Is this a valid AABB? I.e. no NaNs and min[i] <= max[i].
		hkBool isValid() const;

			/// Return true if 'other' is enclosed in this AABB.
			/// Boundaries are inclusive.
		HK_FORCE_INLINE hkBool contains(const hkAabb& other) const;

			/// Return true if 'other' is enclosed in this AABB.
			/// Boundaries are inclusive.
		HK_FORCE_INLINE hkBool32 containsPoint(const hkVector4& other) const;

		/// Extends the AABB to include the given point.
		HK_FORCE_INLINE void includePoint (const hkVector4& point);

		/// Extends the AABB to include the given AABB.
		HK_FORCE_INLINE void includeAabb (const hkAabb& aabb);

			/// Sets the AABB to an empty (invalid) MAX_REAL/MIN_REAL box.
		HK_FORCE_INLINE void setEmpty ();

			/// Returns true if the AABB is empty, e.g., its min is greater than its max for any dimension.
		HK_FORCE_INLINE hkBool isEmpty() const;

			/// Returns the centre of the AABB.
		HK_FORCE_INLINE void getCenter( hkVector4& c ) const;

			/// Returns the half extents of the AABB.
		HK_FORCE_INLINE void getHalfExtents( hkVector4& he ) const;

			/// Increase the maximum and decrease the minimum in each direction by the given amount
		HK_FORCE_INLINE void expandBy( hkSimdRealParameter exp );

			/// Returns true if the AABB is strictly the same as \a a
		HK_FORCE_INLINE hkBool32 equals(const hkAabb& aabb) const;

	public:

			/// The minimum boundary of the AABB (i.e., the coordinates of the corner with the lowest numerical values).
		hkVector4 m_min;

			/// The maximum boundary of the AABB (i.e., the coordinates of the corner with the highest numerical values).
		hkVector4 m_max;
};

HK_CLASSALIGN16(struct) hkAabbUint32
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkAabbUint32);
	HK_DECLARE_REFLECTION();

	inline void operator=( const hkAabbUint32& other );

		/// set completely invalid
	inline void setInvalid();

		/// just make it non colliding for the midphase agent
	inline void setInvalidY();

	HK_ALIGN16( hkUint32 m_min[3] );
	hkUint8 m_expansionMin[3];
	hkUint8 m_expansionShift;
	hkUint32 m_max[3];
	hkUint8 m_expansionMax[3];
	hkUint8 m_shapeKeyByte; // lowest byte of the hkpShapeKey. Useful for hkpListShapes of less then 256 bodies.
};

#include <Common/Base/Types/Geometry/Aabb/hkAabb.inl>

#endif // HK_MATH_AABB_H

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

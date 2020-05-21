/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_SPHERE_H
#define HK_MATH_SPHERE_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkSphereClass;

	/// A sphere defined by a center and radius.
class hkSphere
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_GEOMETRY, hkSphere);
		HK_DECLARE_REFLECTION();

			/// Creates an arbitrary sphere with given center and radius.
			/// The sphere class is a utility class for storing information representing a
			/// sphere. This constructor creates a sphere with the given center and radius.
			/// \param pt Center of the sphere
			/// \param radius Radius of the sphere
		HK_FORCE_INLINE hkSphere();

			/// Creates an arbitrary sphere with given center and radius.
		inline hkSphere(const hkVector4& pt, hkReal radius);

			/// This function returns the radius of the sphere.
		inline hkReal getRadius() const;

			/// This function changes the radius of the sphere.
			/// \param newRadius The new radius
		inline void setRadius(hkReal newRadius);

			/// Returns the position of the center of the sphere.
		inline const hkVector4& getPosition() const;

			/// Returns the center of the sphere, the .w is the radius
		inline const hkVector4& getPositionAndRadius() const;

			/// Returns the center of the sphere, the .w is the radius
		inline hkVector4& getPositionAndRadius();

			/// This function changes the position of the center of the sphere.
			/// \param newPos The desired center of the sphere
		inline void setPosition(const hkVector4& newPos);

			/// Same as setPosition, using the hkVector4's w component as the radius.
		inline void setPositionAndRadius( const hkVector4& newPos );

	protected:

		hkVector4 m_pos;		// note: the w part is the radius
};

#include <Common/Base/Types/Geometry/Sphere/hkSphere.inl>

#endif // HK_MATH_SPHERE_H

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

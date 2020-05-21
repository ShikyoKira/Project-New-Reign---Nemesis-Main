/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_AIR_PROPERTIES_H
#define HK_AIR_PROPERTIES_H

#include <Common/Base/hkBase.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>

/// Provides an interface for a vector field (windVector) and useful methods
/// for applying wind or resistance due to motion to rigid bodies.
/// Subclasses must implement getWindVector.
/// In the case where resistance only is required, no wind object need be created
/// and the static applyResistance method can be called directly.
/// The default units for the wind vector is Newtons / m^2 / (m/s).
class hkpWind : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Obtains the wind vector at position pos.
			/// \param windOut the force that should be applied to the centroid
			/// of a surface of unit area perpendicular to the wind.
		virtual void getWindVector( const hkVector4& pos, hkVector4& windOut ) const = 0;

			/// Destructor.
		virtual ~hkpWind() { }

			/// Apply wind to a rigid body.
			/// \param obbFactor if nonzero, then treat wind on hkpConvexVerticesShapes as if
			/// they were oriented bounding boxes scaled by obbFactor.
			/// \param deltaTime the time step over which the force should be applied.
		void applyWind( hkpRigidBody* rb, hkReal deltaTime, hkReal obbFactor = 0.0f ) const;
			
			/// Apply wind and resistance due to motion to a rigid body.
			/// \param resistanceFactor multiple of relative motion experienced as wind.
			/// (A resistanceFactor of r applies r*speed units of force against a unit of flat area.)
			/// \param obbFactor if nonzero, then treat wind on hkpConvexVerticesShapes as if
			///	they were oriented bounding boxes scaled by obbFactor.
			/// \param deltaTime the time step over which the force should be applied.
		void applyWindAndResistance( hkpRigidBody* body, hkReal deltaTime, hkReal resistanceFactor, hkReal obbFactor = 0.0f ) const;

			/// Apply resistance due to motion to a rigid body.
			/// Since no wind object is involved, this is a static method.
			/// \param resistanceFactor multiple of relative motion experienced as wind.
			/// (A resistanceFactor of r applies r*speed units of force against a unit of projected area.)
			/// \param obbFactor if nonzero, then treat wind on hkpConvexVerticesShapes as if they were
			/// oriented bounding boxes scaled by obbFactor.
			/// \param deltaTime the time step over which the force should be applied.
		static void HK_CALL applyResistance( hkpRigidBody* rb, hkReal deltaTime, hkReal resistanceFactor, hkReal obbFactor = 0.0f );

	private:
		// Implementation class.
		class WindOnShape;
};

#endif // HK_AIR_PROPERTIES_H

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

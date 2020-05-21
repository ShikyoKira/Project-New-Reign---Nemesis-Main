/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_SPHERE_MOTION_H
#define HK_DYNAMICS2_SPHERE_MOTION_H

#include <Physics/Dynamics/Motion/hkpMotion.h>

extern const hkClass hkpSphereMotionClass;

/// This class is contained by the hkpRigidBody class.
/// It should not be accessed directly by the user.
class hkpSphereMotion : public hkpMotion
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpSphereMotion(const hkVector4& position, const hkQuaternion& rotation):
		  hkpMotion( position, rotation )
		{
			m_type = MOTION_SPHERE_INERTIA;
		}


		/*
		** INERTIA
		*/

			/// Get the inertia tensor of the rigid body in local space
		virtual void getInertiaLocal(hkMatrix3& inertia) const;

			/// Get the inertia tensor of the rigid body in world space
		virtual void getInertiaWorld(hkMatrix3& inertia) const;

			/// Sets the inertia tensor of the rigid body in local space. Advanced use only.
		virtual void setInertiaLocal(const hkMatrix3& inertia);


			/// Sets the inertia tensor of the rigid body by supplying its inverse. Advanced use only.
		virtual void setInertiaInvLocal(const hkMatrix3& inertiaInv);

			/// Get the inverse inertia tensor in local space
		virtual void getInertiaInvLocal(hkMatrix3& inertiaInv) const;

			/// Get the inverse inertia tensor in local space
		virtual void getInertiaInvWorld(hkMatrix3& inertiaInv) const;


		/*
		** VELOCITY ACCESS
		*/

			// Implements hkpMotion.
		virtual void getProjectedPointVelocity(const hkVector4& p, const hkVector4& normal, hkReal& velOut, hkReal& invVirtMassOut) const;


		/*
		** IMPULSE APPLICATION
		*/


			/// Apply an impulse at the point p in world space.
		virtual void applyPointImpulse(const hkVector4& imp, const hkVector4& p);


			/// Apply an instantaneous change in angular velocity around center of mass.
		virtual void applyAngularImpulse(const hkVector4& imp);



		/*
		** FORCE AND TORQUE APPLICATION
		*/

			/// Applies a force (in world space) to the rigid body. The force is applied to the center of mass.
		virtual void applyForce(const hkReal deltaTime, const hkVector4& force);


			/// Applies a force (in world space) to the rigid body at the point p in world space.
		virtual void applyForce(const hkReal deltaTime, const hkVector4& force, const hkVector4& p);


			/// Applies the specified torque (in world space) to the rigid body. (note: the inline is for internal use only)
		virtual void applyTorque(const hkReal deltaTime, const hkVector4& torque);

	private:

		HK_FORCE_INLINE void _applyForces(const hkReal deltaTime);

	public:

		hkpSphereMotion( class hkFinishLoadedObjectFlag flag ) : hkpMotion( flag ) {}
};


#endif // HK_DYNAMICS2_SPHERE_MOTION_H

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

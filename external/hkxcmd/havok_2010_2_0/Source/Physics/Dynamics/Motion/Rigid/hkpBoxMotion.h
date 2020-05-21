/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_BOX_MOTION_H
#define HK_DYNAMICS2_BOX_MOTION_H

#include <Physics/Dynamics/Motion/hkpMotion.h>

extern const hkClass hkpBoxMotionClass;

/// This class is contained by the hkpRigidBody class.
/// It should not be accessed directly by the user.
class hkpBoxMotion : public hkpMotion 
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor - sets the initial velocity to zero
		hkpBoxMotion(const hkVector4& position, const hkQuaternion& rotation);

			/// Gets the inertia tensor of the rigid body in local space.
		virtual void getInertiaLocal(hkMatrix3& inertia) const;

			/// Get the inertia tensor of the rigid body in world space
		virtual void getInertiaWorld(hkMatrix3& inertiaOut) const;

			/// Sets the inertia tensor of the rigid body in local space. Advanced use only.
		virtual void setInertiaLocal(const hkMatrix3& inertia);

			/// Set the mass of the rigid body. (Note: it is better to use setMassInv if possible)
		virtual void setMass(hkReal m);



			/// Sets the inertia tensor of the rigid body by supplying its inverse. Advanced use only.
		virtual void setInertiaInvLocal(const hkMatrix3& inertiaInv);

			/// Gets the inverse inertia tensor in local space.
		virtual void getInertiaInvLocal(hkMatrix3& inertiaInv) const;

			/// Get the inverse inertia tensor in local space
		virtual void getInertiaInvWorld(hkMatrix3& inertiaInvOut) const;

		/// Applies an impulse (in world space) at the point p in world space.
		virtual void applyPointImpulse(const hkVector4& imp, const hkVector4& p);

			/// Applies an instantaneous change in angular velocity (in world space) around the center of mass.
		virtual void applyAngularImpulse(const hkVector4& imp);

			/// Applies a force (in world space) to the rigid body. The force is applied to the center of mass.
		virtual void applyForce(const hkReal deltaTime, const hkVector4& force);

			/// Applies a force (in world space) to the rigid body at the point p in world space.
		virtual void applyForce(const hkReal deltaTime, const hkVector4& force, const hkVector4& p);

			/// Applies the specified torque (in world space) to the rigid body. (note: the inline is for internal use only)
		virtual void applyTorque(const hkReal deltaTime, const hkVector4& torque);



			// Implements hkpMotion.
		virtual void getProjectedPointVelocity(const hkVector4& p, const hkVector4& normal, hkReal& velOut, hkReal& invVirtMassOut) const;

	public:

		hkpBoxMotion( class hkFinishLoadedObjectFlag flag ) : hkpMotion( flag ) {}
};

#endif // HK_DYNAMICS2_BOX_MOTION_H

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

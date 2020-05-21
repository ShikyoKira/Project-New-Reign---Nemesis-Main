/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_KEYFRAMED_RIGID_MOTION
#define HK_DYNAMICS2_KEYFRAMED_RIGID_MOTION

#include <Physics/Dynamics/Motion/hkpMotion.h>

extern const class hkClass hkpMaxSizeMotionClass;

extern const hkClass hkpKeyframedRigidMotionClass;

/// This class is contained by the hkpRigidBody class.
/// It should not be accessed directly by the user.
class hkpKeyframedRigidMotion : public hkpMotion
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Construct a keyframed rigid body motion object, given position and rotation.
			/// Set the mass and the inertia to be infinite to achieve the
			/// special behavior of the keyframed motion. This is a property
			/// of the keyframed motion and should not be changed.
		hkpKeyframedRigidMotion( const hkVector4& position, const hkQuaternion& rotation );

		virtual ~hkpKeyframedRigidMotion();

			/// Set the mass of the rigid body. (Note: it is better to use setMassInv if possible).
		virtual void setMass(hkReal m);

			/// Set the inverse mass of the rigid body.
		virtual void setMassInv(hkReal mInv);

		/*
		** INERTIA
		*/

			/// Get the inertia tensor of the rigid body in local space.
		virtual void getInertiaLocal(hkMatrix3& inertia) const;

			/// Users should not set the inertia of a keyframed body.
			/// This will result in an ASSERT. Inertia is infinite for keyframed bodies.
		virtual void setInertiaLocal(const hkMatrix3& inertia);

			/// Get the inertia tensor of the rigid body in world space.
		virtual void getInertiaWorld(hkMatrix3& inertiaOut) const;

			/// Users should not set the inertia inverse of a keyframed body.
			/// This will result in an ASSERT. Inertia is infinite for keyframed bodies.
		virtual void setInertiaInvLocal(const hkMatrix3& inertiaInv);

			/// Get the inverse inertia tensor in local space.
		virtual void getInertiaInvLocal(hkMatrix3& inertiaInv) const;

			/// Get the inverse inertia tensor in local space.
		virtual void getInertiaInvWorld(hkMatrix3& inertiaInvOut) const;


		/*
		** VELOCITY ACCESS
		*/

			// Implements hkpMotion.
		virtual void getProjectedPointVelocity(const hkVector4& p, const hkVector4& normal, hkReal& velOut, hkReal& invVirtMassOut) const;


		/*
		** IMPULSE APPLICATION
		*/

		
			/// Users should not apply a linear impulse to a keyframed body. Try setting the velocity instead.
			/// Keyframed bodies are infinitely heavy so applying impulses doesn't have any effect.
		virtual void applyLinearImpulse(const hkVector4& imp);

			/// Users should not apply a point impulse to a keyframed body. Try setting the velocity instead.
			/// Keyframed bodies are infinitely heavy so applying impulses doesn't have any effect.
		virtual void applyPointImpulse(const hkVector4& imp, const hkVector4& p);

			/// Users should not apply an angular impulse to a keyframed body. Try setting the angular velocity instead.
			/// Keyframed bodies are infinitely heavy so applying angular impulses doesn't have any effect.
		virtual void applyAngularImpulse(const hkVector4& imp);


		/*
		** FORCE AND TORQUE APPLICATION
		*/

			/// Users should not apply a force to a keyframed body. Try setting the velocity instead.
			/// Keyframed bodies are infinitely heavy so applying forces doesn't have any effect.
		virtual void applyForce(const hkReal deltaTime, const hkVector4& force);

			/// Users should not apply a force to a keyframed body. Try setting the velocity instead.
			/// Keyframed bodies are infinitely heavy so applying forces doesn't have any effect.
		virtual void applyForce(const hkReal deltaTime, const hkVector4& force, const hkVector4& p);

			/// Users should not apply a torque to a keyframed body. Try setting the angular velocity instead.
			/// Keyframed bodies are infinitely heavy so applying torque doesn't have any effect.
		virtual void applyTorque(const hkReal deltaTime, const hkVector4& torque);

	protected:

		hkpKeyframedRigidMotion(): hkpMotion(){ m_savedMotion = HK_NULL; }

	public:



	public:

		// hkpMotion interface

		virtual void setStepPosition( hkReal position, hkReal timestep );

		virtual void setStoredMotion( hkpMaxSizeMotion* savedMotion );

	public:

		hkpKeyframedRigidMotion( class hkFinishLoadedObjectFlag flag ) : hkpMotion( flag ) {}
};



// This class is just used as a large-enough placeholder for all available motion classes
class hkpMaxSizeMotion : public hkpKeyframedRigidMotion
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		//+vtable(1)
		HK_DECLARE_REFLECTION();

		hkpMaxSizeMotion() : hkpKeyframedRigidMotion( ) {}
		hkpMaxSizeMotion( class hkFinishLoadedObjectFlag flag ) : hkpKeyframedRigidMotion( flag ) {}
	private:
		void operator=(const hkpMaxSizeMotion& other);
		hkpMaxSizeMotion( const hkpMaxSizeMotion& other );
};





#endif //HK_DYNAMICS2_KEYFRAMED_RIGID_MOTION

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

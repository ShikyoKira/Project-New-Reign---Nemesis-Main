/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKDYNAMICS_ENTITY_HKRIGIDBODYCINFO_XML_H
#define HKDYNAMICS_ENTITY_HKRIGIDBODYCINFO_XML_H

#include <Physics/Collide/Agent/Collidable/hkpCollidableQualityType.h>
#include <Physics/Dynamics/Motion/hkpMotion.h>
#include <Physics/Dynamics/Entity/hkpEntity.h>

class hkpHandleCollection;
class hkLocalFrame;

/// A struct containing all the information needed to construct a rigid body.
class hkpRigidBodyCinfo
{
	public:
		
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ENTITY, hkpRigidBodyCinfo);

			/// A list of possible solver deactivation settings. This value defines how the
			/// solver deactivates objects. The solver works on a per object basis.
			/// Note: Solver deactivation does not save CPU, but reduces creeping of
			/// movable objects in a pile quite dramatically.
		enum SolverDeactivation
		{
				/// 
			SOLVER_DEACTIVATION_INVALID,
				/// No solver deactivation
			SOLVER_DEACTIVATION_OFF,
				/// Very conservative deactivation, typically no visible artifacts.
			SOLVER_DEACTIVATION_LOW,
				/// Normal deactivation, no serious visible artifacts in most cases
			SOLVER_DEACTIVATION_MEDIUM,
				/// Fast deactivation, visible artifacts
			SOLVER_DEACTIVATION_HIGH,
				/// Very fast deactivation, visible artifacts
			SOLVER_DEACTIVATION_MAX
		};
		
			/// Default constructor - initializes all default values.
		hkpRigidBodyCinfo();
		
			/// Sets the mass properties
		void setMassProperties(const struct hkpMassProperties& mp);

			/// Sets position and rotation
		void setTransform( const hkTransform& transform );

		//
		// Members
		//
	public:

			/// This value can be used by collision filters to identify the entity - for
			/// example, if a group collision filter is used, this value would specify the
			/// entity's collision group.
			/// This defaults to 0.
		hkUint32 m_collisionFilterInfo;
		
			/// The collision detection representation for this entity.
			/// This defaults to HK_NULL, and must be set before constructing a hkpRigidBody.
		const hkpShape* m_shape;

			/// The local frame hierarchy that is attached to the rigid body.
		hkLocalFrame* m_localFrame;

			/// The collision response. See hkpMaterial::hkResponseType for hkpWorld default
			/// implementations.
			/// This defaults to hkpMaterial::RESPONSE_SIMPLE_CONTACT.
		hkEnum<hkpMaterial::ResponseType, hkInt8> m_collisionResponse;
		
			/// Delays the firing of contactPointCallbacks for all the contact points in the contact
			/// manifold (contactPointCallbacks are always fired for new contact points.)
			/// This defaults to 0xffff.
		hkUint16 m_contactPointCallbackDelay;

			/// The initial position of the body.
			/// This defaults to 0,0,0.
		hkVector4 m_position;
		
			/// The initial rotation of the body.
			/// This defaults to the Identity quaternion.
		hkQuaternion m_rotation;
		
			/// The initial linear velocity of the body.
			/// This defaults to 0,0,0.
		hkVector4 m_linearVelocity;
		
			/// The initial angular velocity of the body.
			/// This defaults to 0,0,0.
		hkVector4 m_angularVelocity;
		
			/// The inertia tensor of the rigid body. Use the hkpInertiaTensorComputer class to
			/// set the inertia to suitable values.
			/// This defaults to the identity matrix.
		hkMatrix3 m_inertiaTensor;
		
			/// The center of mass in the local space of the rigid body.
			/// This defaults to 0,0,0.
		hkVector4 m_centerOfMass;
		
			/// The mass of the body.
			/// This defaults to 1.
		hkReal m_mass;
		
			/// The initial linear damping of the body.
			/// This defaults to 0.
		hkReal m_linearDamping;
		
			/// The initial angular damping of the body.
			/// This defaults to 0.05.
		hkReal m_angularDamping;

			/// The initial time factor of the body.
			/// This defaults to 1.
		hkReal m_timeFactor;

			/// Gravity factor used to control gravity on a per body basis. Defaults to 1.0
		hkReal m_gravityFactor;
		
			/// The initial friction of the body.
			/// This defaults to 0.5.
		hkReal m_friction;
		
			/// BETA: Rolling friction is a work in progress. It is experimental code and has significant behavior artifacts.
			///
			/// The initial rolling friction multiplier of the body.
			/// The multiplier is combined with the normal friction.
			/// This defaults to 0.0f;
			///
			/// \warning This feature is beta and may change interface and behavior in the next release.
		hkReal m_rollingFrictionMultiplier; //+default(0.0f)
		
			/// The initial restitution of the body.
			/// This defaults to 0.4.
			/// If the restitution is not 0.0 the object will need extra CPU
			/// for all new collisions. Try to set restitution to 0 for maximum
			/// performance (e.g., collapsing buildings)
		hkReal m_restitution;
		
			/// The maximum linear velocity of the body (in m/s).
			/// This defaults to 200.
		hkReal m_maxLinearVelocity;

			/// The maximum angular velocity of the body (in rad/s).
			/// This defaults to 200.
		hkReal m_maxAngularVelocity;
		
			/// The maximum allowed penetration for this object. The default is -1.
			/// This is a hint to the engine to see how much CPU the engine should
			/// invest to keep this object from penetrating. A good choice is 5% - 20% of the
			/// smallest diameter of the object. Setting the initial value less than zero
			/// allows the penetration depth to be estimated by the RigidBody upon creation.
			/// This estimated value is 1/5th of the smallest dimension of the object's radius.
		hkReal m_allowedPenetrationDepth;
		
			/// The initial motion type of the body.
			/// This defaults to hkpMotion::MOTION_DYNAMIC
		hkEnum<hkpMotion::MotionType, hkInt8> m_motionType;
		
			/// Enables or disables deactivation for the rigid body.
			/// This defaults to true.
		hkBool m_enableDeactivation; //+default(true)
		
			/// Allows you to enable an extra single object deactivation schema.
			/// That means the engine will try to "deactivate" single objects (not just entire islands)
			/// if those objects get very slow.
			/// This does not save CPU, however it can reduce small movements in big stacks of objects dramatically.
			/// This defaults to SOLVER_DEACTIVATION_LOW.
		hkEnum<SolverDeactivation, hkInt8> m_solverDeactivation;
		
			/// The quality type, used to specify when to use continuous physics
			/// This defaults to HK_COLLIDABLE_QUALITY_INVALID
			/// If you add a hkpRigidBody to the hkpWorld, this type automatically gets converted to either
			/// HK_COLLIDABLE_QUALITY_FIXED, HK_COLLIDABLE_QUALITY_KEYFRAMED or HK_COLLIDABLE_QUALITY_DEBRIS
		hkEnum<hkpCollidableQualityType, hkInt8> m_qualityType;
	
			/// This is a user flag which you can set to give you a hint as to which objects to remove from
			/// the simulation if the memory overhead becomes too high. It defaults to 0.
		hkInt8 m_autoRemoveLevel;

			/// If this entity may add response modifiers to its collisions during a contactPointCallback, then
			/// flags must be set here in order that space is made available in advance.
			/// See hkpResponseModifier::Flags.
		hkUint8 m_responseModifierFlags;

			/// Requests a number of extra fields in each hkpContactPointProperties for this rigid body.
			/// This defaults to zero. If it is set to a negative value, then the corresponding value in hkpRigidBody
			/// will be set to the depth of the shape which is calculated by traversing its shape hierarchy.
			/// (The latter behavior is intended only for off-line use.)
		hkInt8 m_numShapeKeysInContactPointProperties;

			/// PlayStation(R)3 only: If this flag is set, all collision agents attached to this body will run on the PPU only.
			/// If this flag is set it will set the FORCE_PPU_USER_REQUEST bit in hkpCollidable::m_forceCollideOntoPpu bitfield
		hkBool m_forceCollideOntoPpu;
};

#endif // HKDYNAMICS_ENTITY_HKRIGIDBODYCINFO_XML_H

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

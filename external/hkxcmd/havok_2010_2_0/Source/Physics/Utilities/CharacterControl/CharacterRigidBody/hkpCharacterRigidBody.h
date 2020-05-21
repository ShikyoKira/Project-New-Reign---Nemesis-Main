/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_RIGID_BODY__H
#define HK_CHARACTER_RIGID_BODY__H

#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBodyCinfo.h>
#include <Physics/Utilities/CharacterControl/hkpCharacterControl.h>

class hkpRigidBody;
class hkpShape;
class hkpCdPointCollector;
class hkpCharacterRigidBodyListener;
class hkpSimpleConstraintContactMgr;

/// The character rigid body class is used to represent a rigid body that can move dynamically around
/// the scene. It is called character rigid body because it is usually used to represent game characters.
/// It is a rigid body analogy to hkpCharacterProxy controller. But unlike hkpCharacterProxy, which
/// is represented by virtual phantom shape, hkpCharacterRigidBody controller is physically presented in the dynamic scene.
/// Its movement is controlled by directly setting its linear and angular velocities.
///
/// If a hkpCharacterRigidBodyListener has been added, then the character rigid body issues a callback to it every
/// simulation step. The default behavior of hkpCharacterRigidBodyListener has two main purposes.
///
/// The first effectively modifies the mass of a rigid body character during collision with dynamic objects in the scene.
/// This approach allows continuous regulation of "maximal force (power)" of the character during interaction with other dynamics objects.
/// (The modification factor decreases the mass of the rigid body during response calculation)
/// The modification factor is calculated in every step in contactProcessCallback() using the following formulas:
///
///  - Calculate angle alpha between contact normal and current acceleration
///  - Calculate current force applied to interaction object as F = mass*acceleration*cos(alpha)
///  - If (F > Fmax) calculate mass modification factor
///  - Apply mass modification factor to impulse calculation using hkpResponseModifier::setInvMassScalingForContact()
///
/// The acceleration is calculated as the difference between the required output velocity from state machine and the current
/// velocity of rigid body, divided by timestep.
///
/// The second purpose is the addition of extra contact points to avoid movement of character on "steep" planes. "Steep" planes are defined
/// by m_maxSlope parameter. It's the angle between the UP direction and the normal of the plane. So vertical planes have PI/2 (90 Deg) and horizontal planes 0.
/// Default value is PI/3 (60 Deg).
///
/// Implementation detail:
/// In order to issue the calls to the listener's callback, the character rigid body is a hkpWorldPostSimulationListener.
/// It is automatically added as a listener to the world when its rigid body is added as an entity. This is achieved
/// by being an hkpEntityListener to its rigid body. The user need not add or remove the character rigid body as an
/// hkpEntityListener or a hkpWorldPostSimulationListener. This is done automatically to preserve the following invariant:
/// ( object is a PostSimulationListener ) iff ( ( m_character added to world ) and ( m_listener non-null ) )
class hkpCharacterRigidBody : public hkReferencedObject, public hkpEntityListener, public hkpWorldPostSimulationListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CHARACTER);

			/// Constructor.
			/// Creates the rigid body and a listener.
		hkpCharacterRigidBody( const hkpCharacterRigidBodyCinfo& info );

			/// Destructor
		~hkpCharacterRigidBody();

			/// Sets the character's listener.
		void setListener( hkpCharacterRigidBodyListener* listener );

			/// Removes the current listener.
		void removeListener();

			/// Get rigid body of character
		hkpRigidBody* getRigidBody() const;

		// Determining support information.

			/// Check to see if the character is supported from below.
			/// This call checks the geometry immediately around the character, and takes neither velocity nor friction
			/// into account.
			/// i.e., if the character is moving away from the ground, but is still touching the ground, this function
			/// will return SUPPORTED as the supported state.
		virtual void checkSupport( const hkStepInfo& stepInfo, hkpSurfaceInfo& ground ) const;	

			/// A structure describing a supporting surface and the contact point between it and the character.
			/// The contact point normal always points towards the character.
		struct SupportInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCharacterRigidBody::SupportInfo);
			hkContactPoint m_point;
			hkpRigidBody* m_rigidBody;
		};

			/// Determines whether the character is supported and, if so, provides the supporting surfaces.
			/// \param stepInfo the step info of
			/// \param supportInfo an array into which support information will be placed, if the character is supported or
			///   sliding.
		virtual hkpSurfaceInfo::SupportedState getSupportInfo( const hkStepInfo& stepInfo, hkArray<SupportInfo>& supportInfo ) const;

			/// Calculates an "average" surface given an array of support information.
			/// This method does not modify the supportedState member of ground.
			/// If at least one of surfaces is dynamic, the ground is regarded as dynamic.
			/// \param ground the structure into which the surface information is placed.
			/// \param useDynamicBodyVelocities Do the surface velocities of dynamic bodies contribute to the resulting surface?
		virtual void getGround( const hkArray<SupportInfo>& supportInfo, hkBool useDynamicBodyVelocities, hkpSurfaceInfo& ground ) const;

			/// Set linear acceleration for mass modifier
		void setLinearAccelerationToMassModifier(const hkVector4& newAcc);

			/// Set the maximum force the character is able to apply to another physical object
		void setMaxForce( hkReal maxForce );

		//
		// Simple wrappers for rigid body functions
		//

			/// Set character linear velocity. The timestep argument should be given the same timestep that is passed
			/// to the physics simulation step. Note, the timestep must be greater than 0.0f. A value of 0.0f or less
			/// will set hkpCharacterRigidBodyCollisionListener::m_acceleration to be INF, which could cause crashes.
		void setLinearVelocity(const hkVector4& newVel, hkReal timestep );

			/// Get character linear velocity
		const hkVector4& getLinearVelocity() const;

			/// Set character angular velocity
		void setAngularVelocity(const hkVector4& newAVel);

			/// Get character angular velocity
		const hkVector4& getAngularVelocity() const;

			/// Get character position
		const hkVector4& getPosition() const;

	public:
		// Listener methods.
		virtual void entityAddedCallback( hkpEntity* entity );

		virtual void entityRemovedCallback( hkpEntity* entity );

		virtual void postSimulationCallback( hkpWorld* world );

	public:

		hkpRigidBody* m_character;
		
		hkpCharacterRigidBodyListener* m_listener;

		hkVector4 m_up;

			/// If the character's shape is a capsule, then this is used to define the height of a region around its center
			/// where we redirect contact point normals. The region extends from above the capsule's upper vertex to below
			/// the lower vertex by this height, expressed as a factor of the capsule's radius.
		hkReal m_unweldingHeightOffsetFactor;

		hkReal m_maxSlopeCosine;

		hkReal m_maxSpeedForSimplexSolver;

			/// A character is considered supported if it is less than this distance from its supporting planes.
			/// This should be less than or equal to the world's collision tolerance.
		hkReal m_supportDistance;

			/// A character will keep falling if it is greater than this distance from its supporting planes.
			/// This should be less than or equal to m_supportDistance.
		hkReal m_hardSupportDistance;

	public:
		hkVector4 m_acceleration;
		hkReal	m_maxForce;

	private:
		struct CollectorPair;

	public:
		// These members are maintained by the hkpCharacterRigidBodyListener.

			/// The information needed to delete a vertical point.
		struct VertPointInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCharacterRigidBody::VertPointInfo);
			hkContactPoint m_vertPoint;
			hkpSimpleConstraintContactMgr* m_mgr;
		};

			/// The vertical points we added in the previous step.
		hkArray<VertPointInfo> m_verticalContactPoints;

			/// We set the w component of the position of vertical contact points to this value.
		static const int m_magicNumber = 0x008df4a7;

			/// When removing a contact point, we set the w component of its position to this value.
		static const int m_notMagicNumber = 0x00fa2bb3;
};

#endif // HK_CHARACTER_RIGID_BODY__H

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

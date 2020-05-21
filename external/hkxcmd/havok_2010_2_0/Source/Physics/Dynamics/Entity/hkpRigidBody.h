/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_RIGID_BODY_H
#define HK_DYNAMICS2_RIGID_BODY_H

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpEntity.h>
#include <Physics/Dynamics/Entity/hkpRigidBodyCinfo.h>

extern const hkClass hkpRigidBodyClass;

/// Helper function that returns a hkpRigidBody if the collidable's broadphase handle is of type hkpWorldObject::BROAD_PHASE_ENTITY
inline class hkpRigidBody* hkpGetRigidBody( const hkpCollidable* collidable );
inline class hkpRigidBody* hkpGetRigidBodyUnchecked( const hkpCollidable* collidable );


	/// This is the basic rigid body class. Rigid bodies are objects whose shape never changes.
	/// They are central to Havok dynamics, as they easily allow for rapid physical simulation
	/// in real time.
	/// An hkpRigidBody wraps an hkpMotion that stores all the information relating to how
	/// the rigid body moves, including its mass and velocity. It also has an hkpCollidable member
	/// that contains the information needed for the body to work with the collision detection
	/// system, such as its hkpShape.
class hkpRigidBody : public hkpEntity
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// The constructor takes the information from the passed info parameter.
			/// This contains all the information necessary to create a valid rigid body.
		hkpRigidBody( const hkpRigidBodyCinfo& info );
	
			/// Default destructor.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		virtual ~hkpRigidBody();

			/// Get construction info from this rigid body. Note that this info will reflect the current state of this rigid body, not its original construction info.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		void getCinfo(hkpRigidBodyCinfo& info) const;

			/// Constructor used by Havok serialization
		hkpRigidBody( class hkFinishLoadedObjectFlag flag );


		//
		// MASS, INERTIA AND DENSITY PROPERTIES.
		//

			/// Gets the mass of the rigid body.
		inline hkReal getMass() const;

			/// Gets the 1.0/mass of the rigid body.
		inline hkReal getMassInv() const;

			/// Sets the mass of the rigid body. N.B. This does NOT automatically update other dependent mass properties i.e., the inertia tensor.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setMass(hkReal m);
		
			/// Sets the mass of the rigid body.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setMassInv(hkReal mInv);

			/// Gets the inertia tensor (around the center of mass) in local space.
		inline void getInertiaLocal(hkMatrix3& inertiaOut) const;

			/// Gets the inertia tensor (around the center of mass) in world space.
		inline void getInertiaWorld(hkMatrix3& inertiaOut) const;

			/// Sets the inertia tensor of the rigid body. Advanced use only.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setInertiaLocal(const hkMatrix3& inertia);

			/// Sets the inertia tensor of the rigid body by supplying its inverse. Advanced use only.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setInertiaInvLocal(const hkMatrix3& inertiaInv);

			/// Gets the inverse inertia tensor in local space.
		inline void getInertiaInvLocal(hkMatrix3& inertiaInv) const;

			/// Gets the inverse inertia tensor in world space.
		inline void getInertiaInvWorld(hkMatrix3& inertiaInvOut) const;


		//
		// CENTER OF MASS.
		//

			/// Explicitly sets the center of mass of the rigid body in local space.
			/// Note that this does \b not change the position of the rigid body.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setCenterOfMassLocal(const hkVector4& centerOfMass);
		
			/// Gets the center of mass of the rigid body in the rigid body's local space.
		inline const hkVector4& getCenterOfMassLocal() const;
	
			/// Gets the center of mass of the rigid body in world space.
		inline const hkVector4& getCenterOfMassInWorld() const;


		//
		// POSITION ACCESS.
		//

			/// Returns the position (the local space origin) for this rigid body, in world space.
			/// Note that the center of mass may no longer be the local space origin.
		inline const hkVector4& getPosition() const;

			/// Sets the position (the local space origin) of this rigid body, in world space.
			/// Note that this function may cause contactPointRemovedCallbacks fired.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setPosition(const hkVector4& position);

			/// Returns the rotation from local to world space for this rigid body.
		inline const hkQuaternion& getRotation() const;

			/// Sets the rotation from local to world Space for this rigid body.
			/// This activates the body and its simulation island if it is inactive.
			/// Note that this function may cause contactPointRemovedCallbacks fired.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setRotation(const hkQuaternion& rotation);

			/// Sets the position and rotation of the rigid body, in world space.
			/// Note that this function may cause contactPointRemovedCallbacks fired.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setPositionAndRotation(const hkVector4& position, const hkQuaternion& rotation);

			/// Returns the rigid body (local) to world transformation.
			/// This is the transform at the end of the timestep. If you run physics
			/// and graphics asynchronously you should use approxTransformAt
		inline const hkTransform& getTransform() const;

			/// Sets the rigid body (local) to world transformation
			/// Note that this function may cause contactPointRemovedCallbacks fired.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setTransform(const hkTransform& transform);

			/// like setPositionAndRotation, except its effect will be delayed if the
			/// hkpWorld is locked. That means this function is multi threaded safe
			/// and should be used if you want to change the position of an object
			/// from a callback and you cannot use the setPositionAndRotation (access check asserts
			/// are getting fired.
			/// Note: If you call this function from callbacks in a multithreaded environment,
			/// the engine becomes non deterministic.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		void setPositionAndRotationAsCriticalOperation(const hkVector4& position, const hkQuaternion& rotation);

			/// Interpolate the matrix for the 'time' time; for asynchronous simulation only.
			/// You should use this function to display the rigid bodies position if
			/// you use an asynchronous simulation. To get the current time,
			/// use world->m_currentTime.
		inline void approxTransformAt( hkTime time, hkTransform& transformOut ) const;


			/// Approximates the body's transform at the current world's time.
		inline void approxCurrentTransform( hkTransform& transformOut ) const;


		//
		// VELOCITY ACCESS.
		//

			/// Returns the linear velocity of the center of mass of the rigid body, in world space.
		inline const hkVector4& getLinearVelocity() const;

			/// Sets the linear velocity at the center of mass, in world space.
			/// This activates the body and its simulation island if it is inactive.
		inline void	setLinearVelocity(const hkVector4& newVel);

			/// Returns the angular velocity around the center of mass, in world space.
		inline const hkVector4& getAngularVelocity() const;

			/// Sets the angular velocity around the center of mass, in world space.
			/// This activates the body and its simulation island if it is inactive.
		inline void	setAngularVelocity(const hkVector4& newVel);

			/// Gets the velocity of point p on the rigid body in world space.
		inline void getPointVelocity(const hkVector4& p, hkVector4& vecOut) const;

			/// Like setLinearVelocity, but effect will be delayed if world is locked.
			/// See setPositionAndRotationAsCriticalOperation() for details
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setLinearVelocityAsCriticalOperation(const hkVector4& newVel);

			/// Like setAngularVelocity, but effect will be delayed if world is locked.
			/// See setPositionAndRotationAsCriticalOperation() for details
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setAngularVelocityAsCriticalOperation(const hkVector4& newVel);


		//
		// IMPULSE APPLICATION.
		//

			/// Applies an impulse (in world space) to the center of mass.
			/// This activates the body and its simulation island if it is inactive.
		inline void applyLinearImpulse(const hkVector4& imp);

			/// Applies an impulse (in world space) at the point p in world space.
			/// This activates the body and its simulation island if it is inactive.
		inline void applyPointImpulse(const hkVector4& imp, const hkVector4& p);

			/// Applies an instantaneous change in angular velocity (in world space) around
			/// the center of mass.
			/// This activates the body and its simulation island if it is inactive.
		inline void applyAngularImpulse(const hkVector4& imp);

			/// World-lock--safe version of applyLinearImpulse. The effect of the call may be postponed until the world is unlocked.
			/// See setPositionAndRotationAsCriticalOperation() description for more info on thread-safe usage.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void applyLinearImpulseAsCriticalOperation(const hkVector4& imp);

			/// World-lock--safe version of applyPointImpulse. The effect of the call may be postponed until the world is unlocked.
			/// See setPositionAndRotationAsCriticalOperation() description for more info on thread-safe usage.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void applyPointImpulseAsCriticalOperation(const hkVector4& imp, const hkVector4& p);

			/// World-lock--safe version of applyAngularImpulse. The effect of the call may be postponed until the world is unlocked.
			/// See setPositionAndRotationAsCriticalOperation() description for more info on thread-safe usage.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void applyAngularImpulseAsCriticalOperation(const hkVector4& imp);


		//
		// FORCE AND TORQUE APPLICATION.
		//

			/// Applies a force (in world space) to the rigid body. The force is applied to the
			/// center of mass.
			/// This activates the body and its simulation island if it is inactive.
			/// As forces are immediately converted to impulses and applied , you must pass a correct step delta-time related to the simulation's hkStepInfo.
		inline void applyForce(const hkReal deltaTime, const hkVector4& force);

			/// Applies a force (in world space) to the rigid body at the point p in world space.
			/// This activates the body and its simulation island if it is inactive.
			/// As forces are immediately converted to impulses and applied , you must pass a correct step delta-time related to the simulation's hkStepInfo.
		inline void applyForce(const hkReal deltaTime, const hkVector4& force, const hkVector4& p);

			/// Applies the specified torque (in world space) to the rigid body.
			/// Specify the torque as an hkVector4. The direction of the vector indicates the axis (in
			/// world space) that you want the body to rotate around, and the magnitude of the vector indicates
			/// the strength of the force applied. The change in the body's angular velocity after torques are
			/// applied is proportional to the simulation delta time value and inversely proportional to the body's
			/// inertia.
			/// This activates the body and its simulation island if it is inactive.
			/// As forces are immediately converted to impulses and applied , you must pass a correct step delta-time related to the simulation's hkStepInfo.
		inline void applyTorque(const hkReal deltaTime, const hkVector4& torque);


		//
		// DAMPING.
		//

			/// Naive momentum damping.
		inline hkReal getLinearDamping() const;

			/// Naive momentum damping.
		inline void setLinearDamping( hkReal d );

			/// Naive momentum damping.
		inline hkReal getAngularDamping() const;

			/// Naive momentum damping.
		inline void setAngularDamping( hkReal d );		

			/// Time factor.
		inline hkReal getTimeFactor() const;

			/// Time factor.
		inline void setTimeFactor( hkReal f);

			/// Gravity scaling accessor
		inline hkReal getGravityFactor( void ) const;

			/// Gravity scaling mutator
		inline void setGravityFactor( hkReal f );


		//
		//	Clipping Velocities
		//

			/// Get the linear velocity cap for this rigid body
		inline hkReal getMaxLinearVelocity() const;

			/// Sets the linear velocity maximum
		inline void setMaxLinearVelocity( hkReal maxVel );

			/// Get the Angular velocity cap for this rigid body
		inline hkReal getMaxAngularVelocity() const;

			/// Sets the Angular velocity maximum
			/// The units used here are radians per second.
		inline void setMaxAngularVelocity( hkReal maxVel );

		//
		// DEACTIVATION
		//

			/// Enables or disabled deactivation for this rigid body.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_RO] [this,HK_ACCESS_RW] );
		void enableDeactivation( bool _enableDeactivation );

			/// Checks if deactivation is enabled.
		bool isDeactivationEnabled() const ;

	
		//
		// COLLISION FILTERS
		//

			///Gets the collision filter info. This is an identifying value used by collision filters
			/// - for example, if a group filter is used, this value would encode a collision group and a system group
			///
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		HK_FORCE_INLINE hkUint32 getCollisionFilterInfo() const;

			///Sets the collision filter info. This is an identifying value used by collision filters
			/// - for example, if a group filter is used, this value would encode a collision group and a system group
			///
			/// Note: this function call does not update internal collision detection caches.
			///       Please read hkpWorld::updateCollisionFilterOnEntity()
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		HK_FORCE_INLINE void setCollisionFilterInfo( hkUint32 info );

		//
		// QUALITY TYPE
		//

			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		HK_FORCE_INLINE hkpCollidableQualityType getQualityType() const;

			/// Sets the quality type of this collidable.
			/// Note: Changing this value only affects new agents and constraints,
			/// existing collision agent are still unchanged.
			/// Please read hkpWorld::updateCollisionFilterOnEntity()
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		HK_FORCE_INLINE void setQualityType(hkpCollidableQualityType type);

		//
		// PENETRATION DEPTH
		//

			/// Gets the current allowed penetration depth.
			/// This is a hint to the continuous physics to allow some penetration for this object
			/// to reduce CPU load. Note: this is not a hard limit but more a guideline to the engine.
			/// Depending on the qualityType, this allowed penetration can be breached sooner or later.
			/// See user guide on continuous physics for details.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		HK_FORCE_INLINE hkReal getAllowedPenetrationDepth() const;

			/// Sets the current allowed penetration depth. See getAllowedPenetrationDepth for details.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		HK_FORCE_INLINE void setAllowedPenetrationDepth( hkReal val );

		//
		// MOTION
		//

			/// Change the motion type. The motion type can be fixed, keyframed or dynamic.
			/// When changing to KEYFRAMED or DYNAMIC state you should specify appropriate activation state (acts analogically to
			/// addEntity(entity, activationState)).
			/// You may choose to perform a full broadphase query to check whether new agents should be created or decide to only verify validity of currently
			/// present agents. The latter option is useful when pinning a body and we know that we only are going to remove the
			/// body's collision agents with other fixed or keyframed bodies.
			///
			/// Important: This function updates hkpCollidableQualityType of this rigid body when changing to/from fixed or keyframed state. When
			/// motion type is changed from a dynamic motion, the original quality type of the rigid body is stored in the new keyframed motion
			/// and replaced by a fixed/keyframed quality appropriately. When a body is then changed back to its dynamic state, its original
			/// quality type is restored (along with its also-stored original dynamic motion).
			/// Similarly, when changing a motion's type from dynamic to fixed or keyframed, the original dynamic motion's data is saved and can be later
			/// restored when the motion's type is switched back to dynamic.
			///
			/// Note that this doesn't recreate agents (and doesn't switch between their predictive and non-predictive versions), therefore
			/// having two non-continuously colliding dynamic objects, and fixing one of them, doesn't result in continuous collision detection,
			/// between the objects if the agent is already created. The current agent will be replaced by its continuous version only after the
			/// bodies separate and loose their broadphase overlap (when their agent is destroyed) and then come into proximity again (creating
			/// a new agent of type conforming to their current qualityType settings).
			///
			/// -   Fixed (hkpMotion::MOTION_FIXED) entities are unmovable and effectively have infinite mass.
			/// -   Keyframed (hkpMotion::MOTION_KEYFRAMED) entities can have their kinematics explicitly altered,
			///     but ignore external impulses and forces e.g., from collisions or actions.
			/// -   Dynamic (e.g., hkpMotion::MOTION_DYNAMIC, hkpMotion::MOTION_BOX_INERTIA) entities are
			///     affected by external forces and impulses.
			///
			/// For keyframed and fixed types there is some special treatment.
			/// To see whether a keyframed/fixed body has been changed from a dynamic state, use
			/// hkpRigidBody::getStoredDynamicMotion().
			///
			/// Note: If an object is created as keyframed or fixed it cannot be changed to be dynamic later
			/// but dynamic objects can be changed to fixed or keyframed.
			///
			/// Note: When a body is made keyframed its velocities are zeroed. Its transform and centre of
			/// mass are preserved. It is expected that should a user wish to transfer velocities during such a
			/// transition they can get and set as necessary using getLinearVelocity(), getAngularVelocity() and
			/// setLinearVelocity(), setAngularVelocity(). This means that passing hkpMotion::MOTION_FIXED to setMotionType()
			/// will "stop" a body, but not move it. When a body is "unkeyframed" by passing hkpMotion::MOTION_DYNAMIC to setMotionType()
			/// it preserves both the transform \b and velocities of the current motion, as well as its original center of mass.
			/// It effectively continues on from the same position and with the same velocity that it had just
			/// prior to being "unkeyframed" but is now under dynamic control. All position and velocity information
			/// stored at the point in time when the body was actually "made" keyframed are lost, it is overwritten
			/// with the current values. Fixed bodies' velocities will always be zeroed.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		void setMotionType( hkpMotion::MotionType newState, 
							hkpEntityActivation preferredActivationState = HK_ENTITY_ACTIVATION_DO_ACTIVATE, 
							hkpUpdateCollisionFilterOnEntityMode collisionFilterUpdateMode = HK_UPDATE_FILTER_ON_ENTITY_FULL_CHECK);



			/// Returns the body's motion type.
		inline hkpMotion::MotionType getMotionType() const;

			/// If the rigid body has been "set" to be keyframed, by passing hkpMotion::MOTION_KEYFRAMED to
			/// setMotionType(), this allows you to get the original dynamic motion (which is stored), and hence get
			/// the "unkeyframed" mass and inertia of the rigid body. Otherwise e.g., if the body was constructed
			/// using hkpMotion::MOTION_KEYFRAMED type, this function returns HK_NULL.
		hkpMotion* getStoredDynamicMotion();
			
			/// Const version of the above.
		const hkpMotion* getStoredDynamicMotion() const;

			/// Updates shape-based cached motion information in the rigid body. It also returns the extents (size of the AABB).
		void updateCachedShapeInfo (const hkpShape* shape, hkVector4& extentOut );

		//
		// Friction and Restitution
		//

			/// Returns the friction coefficient (dynamic and static) from the material.
		inline hkReal getFriction() const;

			/// BETA: Rolling friction is a work in progress. It is experimental code and has significant behavior artifacts.
			/// Returns the multiplier for the rolling friction coefficient.
			/// The multiplier is combined with the normal friction coefficient.
			///
			/// \warning This feature is beta and may change interface and behavior in the next release.
		inline hkReal getRollingFrictionMultiplier() const;

			/// Returns the default restitution from the material.
			//  restitution = bounciness (1 should give object all its energy back, 0 means it just sits there, etc.).
		inline hkReal getRestitution() const;

			/// Sets the friction coefficient of the material. Note: Setting this will not update existing contact information.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setFriction( hkReal newFriction );

			/// BETA: Rolling friction is a work in progress. It is experimental code and has significant behavior artifacts.
			/// Sets the multiplier for the rolling friction coefficient of the material. 
			/// The multiplier is combined with the normal friction coefficient.
			/// In order to use this functions, bodies must be initiazlied with non-zero value before they're added
			/// to the hkpWorld.
			///
			/// \warning This feature is beta and may change interface and behavior in the next release.
			///
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setRollingFrictionMultiplier(hkReal multiplier);

			/// Sets the restitution coefficient of the material. Note: Setting this will not update existing contact information.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		void setRestitution( hkReal newRestitution );

		//
		// LAZY CONSTRUCTION
		//

			/// Sets shape and recalculates cached data.
			/// NB: This function is NOT intended to be called at runtime i.e., after
			/// the hkpRigidBody has been added to an hkpWorld. It is intended as
			/// a feature for use during setup e.g., if a complex toolchain makes
			/// filling all members of hkpRigidBodyCinfo difficult.
			///
			/// This function resets the shape of the rigid body, it also performs all necessary reinitialization
			/// except mass and volume properties.
			/// NB: It is NOT recommended to compute mass and volume properties at runtime, the calculations should
			/// be done by calling methods of the hkpInertiaTensorComputer class.
			///
			/// Note: if you call this function for a body with no shape, it checks whether m_allowedPenetrationDepth is set to -1.0f
			/// if so it performs automatic calculation of allowedPenetrationDepth, otherwise it does not alter the value.
			/// If you alternatively call this function for a body with an non-null old shape, it always automatically recalculates
			/// its new allowed penetration depth. You can later override it.
			/// Note2: this function removes and re-adds the body to broadphase, therefore all agents and all cached agents of
			/// hkCachingShapePhantoms will be correctly reset.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		hkWorldOperation::Result setShape(const hkpShape* shape);

			////	Called when the rigid body shape has changed its data (used by destruction deformables)
		hkWorldOperation::Result updateShape(hkpShapeModifier* shapeModifier);

			/// Given a rigidbody (this), return a new rigidbody that shares all static data
			/// such as the shapes, but clones the dynamic runtime data such as the current
			/// motions and user defined properties. This method does NOT clone any attached
			/// listeners.
			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		virtual hkpRigidBody* clone() const; 

	public:

		//
		// INTERNAL FUNCTIONS
		// 

		inline hkpMotion* getRigidMotion() const;

		hkBool checkPerformance() const;

	public:

			/// Internal function. Used by hkpWorldOperationUtil::setRigidBodyMotionType().
		static void HK_CALL createDynamicRigidMotion( hkpMotion::MotionType motionType, 
																const hkVector4& position, const hkQuaternion& rotation, 
																hkReal mass, const hkMatrix3& inertiaLocal, const hkVector4& centreOfMassLocal,
																hkReal maxLinearVelocity, hkReal maxAngularVelocity,
																hkpMaxSizeMotion* motionBufferOut );

			/// ###ACCESS_CHECKS###( [world,HK_ACCESS_RW] [entity,HK_ACCESS_RW] );
		static void HK_CALL updateBroadphaseAndResetCollisionInformationOfWarpedBody( hkpEntity* entity );

	protected:

			/// ###ACCESS_CHECKS###( [getWorld(),HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		virtual hkMotionState* getMotionState();
		


};


#include <Physics/Dynamics/Entity/hkpRigidBody.inl>

#endif // HK_DYNAMICS2_RIGID_BODY_H

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

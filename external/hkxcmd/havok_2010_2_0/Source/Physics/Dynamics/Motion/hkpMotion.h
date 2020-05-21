/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_MOTION_H
#define HK_DYNAMICS2_MOTION_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/MotionState/hkMotionState.h>

extern const hkClass hkpMotionClass;

	/// This class is used internally.
	/// An object's hkpMotion stores all the information relating to
	/// how it moves, including its mass, position, and velocity.
	/// A hkpMotion is owned by exactly one object, usually a hkpEntity
	/// or another hkpMotion.
	/// Each hkpRigidBody has an hkpMotion that stores transform, velocity, and mass information
	/// for the object.
	/// Do not access this class directly - use hkpRigidBody functions instead.
class hkpMotion : public hkReferencedObject
{
public:
	// +version(3)

	HK_DECLARE_REFLECTION();

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_MOTION);

	/// A list of all motion types. The motion type of a hkpRigidBody determines what
	/// happens when the rigid body is simulated.
	enum MotionType
	{
			/// 
		MOTION_INVALID,

			/// A fully-simulated, movable rigid body. At construction time the engine checks
			/// the input inertia and selects MOTION_SPHERE_INERTIA or MOTION_BOX_INERTIA as
			/// appropriate.
		MOTION_DYNAMIC,

			/// Simulation is performed using a sphere inertia tensor. (A multiple of the
			/// Identity matrix). The highest value of the diagonal of the rigid body's
			/// inertia tensor is used as the spherical inertia.
		MOTION_SPHERE_INERTIA,

			/// Simulation is performed using a box inertia tensor. The non-diagonal elements
			/// of the inertia tensor are set to zero. This is slower than the
			/// MOTION_SPHERE_INERTIA motions, however it can produce more accurate results,
			/// especially for long thin objects.
		MOTION_BOX_INERTIA,

			/// Simulation is not performed as a normal rigid body. During a simulation step,
			/// the velocity of the rigid body is used to calculate the new position of the
			/// rigid body, however the velocity is NOT updated. The user can keyframe a rigid
			/// body by setting the velocity of the rigid body to produce the desired keyframe
			/// positions. The hkpKeyFrameUtility class can be used to simply apply keyframes
			/// in this way. The velocity of a keyframed rigid body is NOT changed by the
			/// application of impulses or forces. The keyframed rigid body has an infinite
			/// mass when viewed by the rest of the system.
		MOTION_KEYFRAMED,

			/// This motion type is used for the static elements of a game scene, e.g., the
			/// landscape. Fixed rigid bodies are treated in a special way by the system. They
			/// have the same effect as a rigid body with a motion of type MOTION_KEYFRAMED
			/// and velocity 0, however they are much faster to use, incurring no simulation
			/// overhead, except in collision with moving bodies.
		MOTION_FIXED,

			/// A box inertia motion which is optimized for thin boxes and has less stability problems
		MOTION_THIN_BOX_INERTIA,

			/// A specialized motion used for character controllers
		MOTION_CHARACTER,

			/// 
		MOTION_MAX_ID
	};

public:
	inline MotionType getType() const { return m_type; }

	/// Default constructor - sets all values to zero.
	hkpMotion();

	/// Default constructor - sets initial velocity to zero.
	hkpMotion(const hkVector4& position, const hkQuaternion& rotation, bool wantDeactivation = false);

	/// Get the mass of the rigid body (Note: it is better to use getMassInv if possible).
	hkReal getMass() const;

	/// Get the inverse mass.
	inline hkSimdReal getMassInv() const;

	/// Set the mass of the rigid body. (Note: it is better to use setMassInv if possible).
	virtual void setMass(hkReal m);

	/// Set the inverse mass of the rigid body.
	virtual void setMassInv(hkReal mInv);

	/// Get the inertia tensor of the rigid body in local space.
	virtual void getInertiaLocal(hkMatrix3& inertiaOut) const = 0;

	/// Get the inertia tensor of the rigid body in world space.
	virtual void getInertiaWorld(hkMatrix3& inertiaOut) const = 0;

	/// Sets the inertia tensor of the rigid body in local space. Advanced use only.
	virtual void setInertiaLocal(const hkMatrix3& inertia) = 0;

	/// Sets the inertia tensor of the rigid body by supplying its inverse. Advanced use only.
	virtual void setInertiaInvLocal(const hkMatrix3& inertiaInv) = 0;

	/// Get the inverse inertia tensor in local space.
	virtual void getInertiaInvLocal(hkMatrix3& inertiaInvOut) const = 0;

	/// Get the inverse inertia tensor in local space.
	virtual void getInertiaInvWorld(hkMatrix3& inertiaInvOut) const = 0;

	/// Explicitly set the center of mass of the rigid body in local space.
	/// This does not change the position of the rigid body.
	virtual void setCenterOfMassInLocal(const hkVector4& centerOfMass);

	/// Get the center of mass in local space.
	inline const hkVector4& getCenterOfMassLocal() const;

	/// Get the center of mass of the rigid body in world space.
	inline const hkVector4& getCenterOfMassInWorld() const;

	inline hkMotionState* getMotionState();

	/// Get access to it's internal motion state
	inline const hkMotionState* getMotionState() const;

	//
	// POSITION ACCESS
	//

	/// Return the position (Local Space origin) for this rigid body, in World space.
	/// Note: The center of mass is not necessarily the local space origin.
	inline const hkVector4& getPosition() const;

	/// Set the position (Local Space origin) of this rigid body, in World space.
	virtual void setPosition(const hkVector4& position);

	/// Returns the rotation from Local to World space for this rigid body.
	inline const hkQuaternion& getRotation() const;

	/// Set the rotation from Local to World Space for this rigid body.
	virtual void setRotation(const hkQuaternion& rotation);

	/// Set the position and rotation of the rigid body, in World space.
	virtual void setPositionAndRotation(const hkVector4& position, const hkQuaternion& rotation);

	/// Returns the rigid body (local) to world transformation.
	inline const hkTransform& getTransform() const;

	/// approximate a transform for any given time T
	///
	/// If T is between t0 and t1 of the motion state than the algorithm is doing in interpolation
	/// otherwise the result is an extrapolation.
	/// Note: This function has a lower accuracy than hkSweptTransformUtil::lerp2 as used by Havok's collision detection
	/// and should be used when you care about speed not accuracy (e.g., graphics)
	void approxTransformAt( hkTime time, hkTransform& transformOut );

	/// Sets the rigid body (local) to world transformation.
	virtual void setTransform(const hkTransform& transform);


	//
	// VELOCITY ACCESS
	//

	/// Return the linear velocity of the center of mass of the rigid body, in world space.
	inline const hkVector4& getLinearVelocity() const;

	/// Sets the linear velocity at the center of mass, in World space.
	virtual void setLinearVelocity(const hkVector4& newVel);

	/// Returns the angular velocity around the center of mass, in world space.
	inline const hkVector4& getAngularVelocity() const;

	/// Sets the angular velocity around the center of mass, in world space.
	virtual void setAngularVelocity(const hkVector4& newVel);

	/// Velocity of point p on the rigid body in World space.
	HK_FORCE_INLINE void getPointVelocity(const hkVector4& p, hkVector4& vecOut) const;

	/// Velocity and inverse virtual mass of point p along the supplied normal.
	virtual void getProjectedPointVelocity(const hkVector4& p, const hkVector4& normal, hkReal& velOut, hkReal& invVirtMassOut) const = 0;

	//
	// IMPULSE APPLICATION
	//

	/// Apply an impulse (in world space) to the center of mass.
	virtual void applyLinearImpulse(const hkVector4& imp);

	/// Apply an impulse (in world space) at the point p in world space.
	virtual void applyPointImpulse(const hkVector4& imp, const hkVector4& p) = 0;

	/// Apply an instantaneous change in angular velocity (in world space) around
	/// center of mass.
	virtual void applyAngularImpulse(const hkVector4& imp) = 0;


	//
	// FORCE AND TORQUE APPLICATION
	//

	/// Applies a force (in world space) to the rigid body. The force is applied to the
	/// center of mass.
	virtual void applyForce(const hkReal deltaTime, const hkVector4& force) = 0;

	/// Applies a force (in world space) to the rigid body at the point p in world space.
	virtual void applyForce(const hkReal deltaTime, const hkVector4& force, const hkVector4& p) = 0;

	/// Applies the specified torque (in world space) to the rigid body. (Note: the inline
	/// is for internal use only).
	virtual void applyTorque(const hkReal deltaTime, const hkVector4& torque) = 0;


	//
	// DAMPING
	//

	/// Naive momentum damping.
	inline hkReal getLinearDamping();

	/// Naive momentum damping.
	inline void setLinearDamping( hkReal d );

	/// Naive momentum damping.
	inline hkReal getAngularDamping();

	/// Naive momentum damping.
	inline void setAngularDamping( hkReal d );

	/// Time factor.
	inline hkReal getTimeFactor();

	/// Time factor.
	inline void setTimeFactor( hkReal f );


	//
	// GRAVITY FACTOR
	//

	/// Get the current gravity factor.
	inline hkReal getGravityFactor();

	/// Set the gravity factor.
	inline void setGravityFactor( hkReal gravityFactor );

	//
	//	SOLVER DEACTIVATION SETTINGS
	//
	/// get the deactivation class as defined in hkpSolverInfo::DeactivationClass
	inline int getDeactivationClass();

	/// set the deactivation class as defined in hkpSolverInfo::DeactivationClass
	void setDeactivationClass(int deactivationClass);

		/// enables deactivation for this motion. Random number should be a random number.
		/// This helps to evenly distribute the workload over several frames
	inline void enableDeactivation( bool value, int randomNumber = 0, int worldFlag0 = 0, int worldFlag1 = 0, int worldDeactivationIntegrateCounter = 0);

	inline bool isDeactivationEnabled() const;
public:


	//
	// INTERNAL FUNCTIONS
	//

		// serialization constructor. Note that the time in the motion state may 
		// be wrong (commonly reset in world addEntity etc)
	hkpMotion( class hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) { if ( flag.m_finishing ) { m_gravityFactor = 1.f; } }

	// This method updates a given motion with the dynamic properties of the current motion, namely
	// m_linearVelocity, m_angularVelocity, m_rotation, m_oldCenterOfMassInWorld, m_centerOfMassInWorld and m_localToWorld.
	virtual void getMotionStateAndVelocitiesAndDeactivationType(hkpMotion* motionOut);

		// Gets number of frames during which the body movement was withing the 'inactive' threshold.
	inline int getNumInactiveFrames(int select);

		// Gets number of frames during which the body movement was withing the 'inactive' threshold.
		// This is used by the engine when querying numInactiveFrame, while other jobs modify the counters.
		// This is used by split-island job.
	inline int getNumInactiveFramesMt(int select, int worldDeactivationNumInactiveFramesSelectFlag);

		// This sets the deactivation counters and synchronizes them with the world's deactivation counters.
	inline void setWorldSelectFlagsNeg(int worldFlag0, int worldFlag1, int worldDeactivationIntegrateCounter);

		// Increases the numInactiveFrames counter in a multithreading-safe way.
	inline void incrementNumInactiveFramesMt(int select, int worldDeactivationNumInactiveFramesSelectFlag);

		// Zeroes the numInactiveFrames counter in a multithreading-safe way.
	inline void zeroNumInactiveFramesMt(int select, int worldDeactivationNumInactiveFramesSelectFlag);

private:

	HK_FORCE_INLINE void init(const hkVector4& position, const hkQuaternion& rotation, bool wantDeactivation = false);

public:

	enum { NUM_INACTIVE_FRAMES_TO_DEACTIVATE = 5 };

public:
	hkEnum<MotionType, hkUint8>	m_type;

		// a counter reducing the frequency of deactivation checks.
		// The counter is incremented every time the body is integrated.
		// Every time the lower 2 bits of the count are 0 (every 4th frame), a deactivation
		// check (index=0) is performed. If the 4 lower bits are 0 (every 16th frame), a deactivation
		// check (index=1) is performed.
		// if the counter is 0xff, no check will performed
	hkUint8		m_deactivationIntegrateCounter;

		// the number of frames this object is inactive
	hkUint16	m_deactivationNumInactiveFrames[2];

	class hkMotionState m_motionState;

	// inverted inertia (xyz) and inverted mass (w)
	hkVector4 m_inertiaAndMassInv;

	// ------------------ 2nd CacheLine64 -------------------------
	// velocity data (used by buildAccumulator)
	hkVector4		m_linearVelocity;
	hkVector4		m_angularVelocity;

		// the reference position for the deactivator. .w component is the max velocity squared since last setting this position
		// to deactivate deactivation, simply set m_deactivationIntegrateCounter to 0xff
	hkVector4 m_deactivationRefPosition[ 2 ];

		// the packed m_deactivationRefOrientation (stored in the motionState motion (so that sizeof(hkpEntity) < 512))
	hkUint32 m_deactivationRefOrientation[2];

	// Stored _DYNAMIC_ motion
	class hkpMaxSizeMotion*	m_savedMotion; //+hk.DataObjectType("hkpMotion")

	// Stored quality type of the rigid body, which refers to its saved _DYNAMIC_ motion.
	hkUint16 m_savedQualityTypeIndex;

	// Scale the world gravity to disable gravity or reverse the direction
	hkHalf m_gravityFactor;

};

class hkpRigidMotion: public hkpMotion
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	hkpRigidMotion( class hkFinishLoadedObjectFlag flag ): hkpMotion(flag) { }
};

#include <Physics/Dynamics/Motion/hkpMotion.inl>


#endif // HK_DYNAMICS2_MOTION_H

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

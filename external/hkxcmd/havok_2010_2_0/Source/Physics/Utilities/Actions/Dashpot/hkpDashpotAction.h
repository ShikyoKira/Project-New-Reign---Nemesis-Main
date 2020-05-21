/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DASHPOT_H
#define HK_DASHPOT_H

#include <Physics/Dynamics/Action/hkpBinaryAction.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

extern const hkClass hkpDashpotActionClass;

/// A dashpot is a simple version of a constraint that works like a heavily damped spring.
/// You can use a dashpot to attach two bodies together, or a body to a point in world space.
/// It is similar to a point-to-point constraint, although it is an hkpAction rather
/// an hkpConstraintInstance.
///
/// A dashpot works by applying forces to its object or objects in an attempt
/// to keep the attachment points at the same position in world space.
/// These attachment points are specified in the local space of the rigid bodies.
/// Strong forces (such as a Mouse Spring) or impulses may momentarily break this constraint, but the bodies will
/// automatically snap back to reinstate it.
/// 
/// In general, dashpots are faster than constraints, but have less stability.
/// For example, high strength values can add instability to the system, and can cause the system
/// to gain energy and even explode. It is advisable to have a strength that is equal to the heaviest
/// rigid body - for instance, if that body has a mass of 10 kg, a good value for strength
/// is 10. It is also advisable to have a damping that is 1/10 the value of the strength.
/// Values of much higher than this can make the system explode.
/// 
/// The stability of the system is also linked to
/// the size of the timesteps taken during simulation - a bigger timestep can make
/// the system more unstable. A timestep of .01666 (60 Hz) with two or three
/// substeps is generally required.
///
/// \param str The strength of the dashpot range[see notes] default==1
/// \param damp The damping coefficient of the dashpot - this determines the rate of decay of oscillations of the dashpot. A good value for damping is 1/10 the value of the strength. Range[see notes] default==0.1
class hkpDashpotAction : public hkpBinaryAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Creates a dashpot with the specified construction info.
		hkpDashpotAction(hkpRigidBody* entityA = HK_NULL, hkpRigidBody* entityB = HK_NULL, hkUlong userData = 0);

			/// Apply the hkpDashpotAction to the two bodies.
			/// A dashpot is an action and so it has an apply method. This method is called
			/// every timestep and applies the necessary impulses to ra and rb to achieve the
			/// dashpot motion.
		void applyAction(const hkStepInfo& stepInfo);

			/// Sets the attach points with world space information.
			/// Make sure to set the body pointers before calling this function.
		inline void setInWorldSpace(const hkVector4& pivotA, const hkVector4& pivotB);

			/// Sets the attach points with local space information.
		inline void setInBodySpace(const hkVector4& pivotA, const hkVector4& pivotB);

			/// Gets the point of attachment to body A, in body A space.
		inline const hkVector4& getPointA() const;

			///Gets the point of attachment to body B, in body B space
		inline const hkVector4& getPointB() const;

			/// Sets the point of attachment to body A, in body A space.
		inline void setPointA(const hkVector4& pa);

			///Sets the point of attachment to body B, in body B space
		inline void setPointB(const hkVector4& pb);

			/// Return positions in local space on bodies A and B to which the hkpDashpotAction is attached.
			/// \param pa The hkpDashpotAction application point on bodyA, in the local space of bodyA, are copied into pa.
			/// \param pb The hkpDashpotAction application point on bodyB, in the local space of bodyB, are copied into pb.
		inline void getPoints(hkVector4& pa, hkVector4& pb);

			/// Dynamically change points (in local space) on bodies A and B to which the hkpDashpotAction is attached.
			/// \param pa The position of the point on bodyA in the local space of bodyA.
			/// \param pb The position of the point on bodyB in the local space of bodyB.
		inline void setPoints(const hkVector4& pa, const hkVector4& pb);

			/// Returns the strength of the hkpDashpotAction.
			/// This value is the strength of the restoring force between the two points. An appropriate strength for
			/// a hkpDashpotAction is the weight of the heavier of the two rigid bodies.
		inline hkReal getStrength() const;

			/// Dynamically sets the strength of the hkpDashpotAction.
			/// This value is the strength of the restoring force between the two points. An appropriate strength for a
			/// hkpDashpotAction is the weight of the heavier of the two rigid bodies
			/// \param s The new strength.
		inline void setStrength(hkReal s);

			/// Returns the damping coefficient of the hkpDashpotAction.
			/// The damping parameter specifies the rate of decay of the oscillations the hkpDashpotAction produces.
			/// Values higher than 1/10 of the hkpDashpotAction strength can cause instability.
		inline hkReal getDamping() const;

			/// Dynamically sets the damping coefficient of the hkpDashpotAction.
			/// The damping parameter specifies the rate of decay of the oscillations the hkpDashpotAction produces.
			/// In general the damping should be 10% of the strength of the hkpDashpotAction.
			/// \param d The damping coefficient of the hkpDashpotAction.
		inline void setDamping(hkReal d);

			/// This function returns the last impulse has been applied to the bodies by the dashpot.
		inline const hkVector4 & getImpulse();

			/// hkpAction clone interface.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;

	protected:

		hkVector4	m_point[2];
		hkReal		m_strength;
		hkReal		m_damping;
		hkVector4	m_impulse;

	public:

		hkpDashpotAction( class hkFinishLoadedObjectFlag flag ) : hkpBinaryAction(flag) {}

};

#include <Physics/Utilities/Actions/Dashpot/hkpDashpotAction.inl>


#endif // HAVOK_DASHPO

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

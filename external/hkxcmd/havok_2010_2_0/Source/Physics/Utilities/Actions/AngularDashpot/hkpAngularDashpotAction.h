/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANGULARDASHPOT_H
#define HK_ANGULARDASHPOT_H

#include <Physics/Dynamics/Action/hkpBinaryAction.h>

extern const hkClass hkpAngularDashpotActionClass;

/// An angular dashpot is an action that can be used to maintain the orientation of a
/// rigid body - either relative to another body or the world. It can be used as a cheap alternative to simple orientation
/// constraints, and is the rotational equivalent of an hkpDashpotAction.
/// When one of the dashpot's bodies is oriented away from the specified rotation, the dashpot works so that it
/// will tend to return to that rotation. You construct the dashpot by specifying the rigid
/// body or bodies, and the desired orientation (rot). You can also specify stiffness
/// and damping parameters.
/// 
/// The default strength and damping parameters are suitable for dashpots connected
/// to rigid bodies of mass 1kg. For rigid bodies with larger masses, higher
/// strengths may be needed.
class hkpAngularDashpotAction : public hkpBinaryAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Creates an angular dashpot with the specified construction info.
		hkpAngularDashpotAction(hkpRigidBody* entityA = HK_NULL, hkpRigidBody* entityB = HK_NULL, hkUlong userData = 0);

			/// Applies the angular dashpot to the rigid body/bodies.
			/// This method is called internally by the Havok system.
		void applyAction(const hkStepInfo& stepInfo);

			/// Returns the rotation of the angular dashpot.
			/// The rotation value specifies the relative orientation between the two bodies - or
			/// between the body and the world - that
			/// the angular dashpot tries to maintain.
		const hkQuaternion& getRotation() const;

			/// Sets the rotation of the angular dashpot.
			/// The rotation value specifies the relative orientation between the two bodies - or
			/// between the body and the world - that
			/// the angular dashpot tries to maintain. The rotation
			/// value is expressed as a quaternion.
			/// \param r The relative orientation between the two bodies, or between the body and the world.
		void setRotation(const hkQuaternion& r);

			/// Returns the strength of the dashpot.
			/// The default value is 1.
		hkReal getStrength() const;

			/// Sets the strength of the dashpot.
			/// The default strength value is 1.
			/// If the weight of the bodies is much greater then 1 it may be necessary to
			/// increase the strength for the dashpot to have an appropriate affect.
			/// \param s The strength of the dashpot.
		void setStrength(hkReal s);

			/// Returns the damping coefficient of the angular dashpot.
			/// The damping parameter specifies the rate of decay of the oscillations the
			/// dashpot produces. Values higher than 10% of the strength of the Dashpot can cause instability.
		hkReal getDamping() const;

			/// Sets the damping coefficient of the angular dashpot.
			/// The damping parameter specifies the rate of decay of the oscillations the
			/// dashpot produces. Values higher than 10% of the strength of the Dashpot can
			/// cause instability.
			/// \param s The damping coefficient of the dashpot.
		void setDamping(hkReal s);

			/// hkpAction clone interface.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;

	public:

		hkQuaternion	m_rotation;
		hkReal			m_strength;
		hkReal			m_damping;

	public:

		hkpAngularDashpotAction( class hkFinishLoadedObjectFlag flag ) : hkpBinaryAction(flag) {}

};

#include <Physics/Utilities/Actions/AngularDashpot/hkpAngularDashpotAction.inl>

#endif

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

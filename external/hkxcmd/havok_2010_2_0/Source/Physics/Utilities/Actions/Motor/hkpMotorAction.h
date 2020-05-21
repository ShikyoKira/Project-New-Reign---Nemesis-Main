/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MOTORACTION_H
#define HK_MOTORACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

extern const hkClass hkpMotorActionClass;
class hkpRigidBody;

class hkpRigidBody;

	/// This action applies torque to the rigid body passed as a parameter until it
	/// reaches the desired angular velocity. The motor acts on 'body' spinning about
	/// 'axis' at 'spin_rate' [radians/sec]. 'gain' specifies the rate at which the
	/// desired velocity is attained [(angular - current) * gain].
class hkpMotorAction: public hkpUnaryAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// This is a very basic motor to test actions. An appropriate torque is applied to the
			/// identified rigid body at each step.
			///
			/// \param body The body the action is applied to.
			/// \param axis The axis of rotation of the body.
			/// \param spinRate The desired angular velocity we want to reach [rad/s].
			/// \param gain Specified how fast do we want to reach the desired angular velocity range[>0] default == 2.0f.
		inline hkpMotorAction();

			/// Creates a new hkpMotorAction.
		hkpMotorAction(hkpRigidBody* body, const hkVector4& axis, hkReal spinRate, hkReal gain = 2.0f);

			/// Apply the action to the rigid body. Called every simulation step.
		void applyAction(const hkStepInfo& stepInfo);

			/// Gets the spin rate (angular velocity) the action is trying to reach.
			/// This value is set on construction and can be changed by calling setSpinRate().
		inline hkReal getSpinRate() const;

			/// Sets the new desired spin rate the motor action will try to reach.
			/// \param new_rate The new desired spin rate.
		inline void setSpinRate(hkReal new_rate);

			/// Gets the current gain.
			/// This is the gain that the action is currently using to reach the desired spin speed.
			/// The gain is set on construction and can be modified by the function setGain().
		inline hkReal getGain() const;

			/// Sets the a new gain rate for the action.
			/// The gain is the rate at which the action will try to reach the desired angular velocity.
			///
			/// \param new_gain The new gain. Range [>0].
		inline void setGain(hkReal new_rate);

			/// Gets the axis around which the object should spin, in local space.
			/// This axis is specified on construction.
		inline const hkVector4& getAxis() const;

			/// Sets the axis around which the object should spin, in local space.
		inline void setAxis(const hkVector4& axis);

			/// Gets the current activation state for this motor
		inline hkBool isActive() const;

			/// Activate/deactivate
		inline void setActivation(hkBool b);

			/// hkpAction clone interface.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;

	protected:

			
		hkVector4 m_axis;

			
		hkReal m_spinRate;
		
			
		hkReal m_gain;

			
		hkBool m_active;

	public:

		hkpMotorAction( class hkFinishLoadedObjectFlag flag ) : hkpUnaryAction(flag) {}

};

#include <Physics/Utilities/Actions/Motor/hkpMotorAction.inl>

#endif // HK_MOTOR_ACTION_H

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

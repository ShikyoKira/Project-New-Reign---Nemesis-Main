/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_STATE_ON_GROUND
#define HK_CHARACTER_STATE_ON_GROUND

#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterState.h>

/// This state controls character movement when on the ground.
/// The implementation uses a feedback controller. The controller attempts
/// to bring the velocity of the character to the walking speed. All measurements
/// are taken in the coordinate frame specified by the character input - this
/// means that the character will maintain the same speed when travelling up and down
/// slopes as it will when running on the flat.
class hkpCharacterStateOnGround : public hkpCharacterState
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpCharacterStateOnGround();

			/// Returns the state type
		virtual hkpCharacterStateType getType() const;

			/// Process the user input - causes state actions.
		virtual void update(hkpCharacterContext& context, const hkpCharacterInput& input, hkpCharacterOutput& output);
			
			///	Process the user input - causes state transitions.
		virtual void change(hkpCharacterContext& context, const hkpCharacterInput& input, hkpCharacterOutput& output);

			/// Gets the gain for this feedback controller
		hkReal getGain() const;

			/// Sets the gain for this feedback controller
		void setGain(hkReal newGain);

			/// Gets the walking speed for this state
		hkReal getSpeed() const;

			/// Sets the walking speed for this state
		void setSpeed(hkReal newGain);
		
			/// Gets the maximal linear acceleration for this state
		hkReal getMaxLinearAcceleration() const;

			/// Set the maximal linear acceleration for this state
		void setMaxLinearAcceleration(hkReal newMaxAcceleration);


			/// Returns the state of the ground hugging flag. By default this is set to true.
			/// If this flag is set then the character will automatically clamp it's vertical velocity
			/// to zero when transitioning from onGround to inAir. As the state transition happens
			/// when the character is no longer supported, it cannot use the surface velocity, so this
			/// only works effectively on static or slow moving surfaces.
		hkBool getGroundHugging() const;

			/// Sets the state of the ground hugging flag.
			/// If this flag is set then the character will automatically clamp it's vertical velocity
			/// to zero when transitioning from onGround to inAir. As the state transition happens
			/// when the character is no longer supported, it cannot use the surface velocity, so this
			/// only works effectively on static or slow moving surfaces.
		void setGroundHugging(hkBool newVal);

			/// Returns the state of the limit downward velocity flag. By default this value is false.
			/// If this flag is set then vertical velocity of the character will be clamped to never
			/// exceed character gravity. This means that if you are running down a steep slope
			/// the vertical component of the character velocity will be clamped against gravity.
			/// Similarly if you are on an elevator moving downward you will move away from the elevator
			/// as its velocity exceeds gravity.
		hkBool getLimitDownwardVelocity() const;

			/// Sets the state of the limit downward velocity flag.
			/// If this flag is set then vertical velocity of the character will be clamped to never
			/// exceed character gravity. This means that if you are running down a steep slope
			/// the vertical component of the character velocity will be clamped against gravity.
			/// Similarly if you are on an elevator moving downward you will move away from the elevator
			/// as its velocity exceeds gravity.
		void setLimitDownwardVelocity(hkBool newVal);

			/// When the character is walking up a slope, a velocity is calculated on the surface
			/// of the slope that keeps the character moving as though it was on level ground. i.e
			/// the character does not slow down, or change direction as it walks up a slope.
			/// However this upwards velocity can cause the character to jitter in certain circumstances.
			/// For example when moving up a slope and hitting a wall, the character will rise slightly
			/// up the wall due to its upwards velocity.
			/// The solution for this is to project this upwards velocity into the horizontal plane, which
			/// is done by default (new to 3.1). The behavior change should be imperceptible (apart from the
			/// removed jitter), however you can disable this projection if you wish to keep the 3.0 behavior
			/// using this flag.
		hkBool getDisableHorizontalProjection() const;

			/// When the character is walking up a slope, a velocity is calculated on the surface
			/// of the slope that keeps the character moving as though it was on level ground. i.e
			/// the character does not slow down, or change direction as it walks up a slope.
			/// However this upwards velocity can cause the character to jitter in certain circumstances.
			/// For example when moving up a slope and hitting a wall, the character will rise slightly
			/// up the wall due to its upwards velocity.
			/// The solution for this is to project this upwards velocity into the horizontal plane, which
			/// is done by default (new to 3.1). The behavior change should be imperceptible (apart from the
			/// removed jitter), however you can disable this projection if you wish to keep the 3.0 behavior
			/// using this flag.
		void setDisableHorizontalProjection( hkBool newVal );

	protected:

		hkReal	m_gain;

		hkReal	m_walkSpeed;

		hkReal  m_maxLinearAcceleration;

		hkBool	m_killVelocityOnLaunch;

		hkBool  m_limitVerticalVelocity;

		hkBool  m_disableHorizontalProjection;
};

#endif // HK_CHARACTER_STATE_ON_GROUND

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

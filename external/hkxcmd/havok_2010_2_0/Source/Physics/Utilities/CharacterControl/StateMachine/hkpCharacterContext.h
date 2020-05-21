/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_CONTEXT_H
#define HK_CHARACTER_CONTEXT_H

#include <Common/Base/hkBase.h>
#include <Common/Base/hkBase.h>
#include <Common/Base/Types/Physics/hkStepInfo.h>
#include <Physics/Dynamics/Motion/hkpMotion.h>
#include <Physics/Utilities/CharacterControl/hkpCharacterControl.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterState.h>

class hkpCharacterStateManager;
class hkpWorld;

/// The input to the character state machine.
/// Fill in the details and pass to a Character Context to cause a transitions in the state machine and produce and ouput
struct hkpCharacterInput
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpCharacterInput );

	//
	// User input
	//

	/// Input X range -1 to 1 (left / right)
	hkReal				m_inputLR;	

	/// Input Y range -1 to 1 (forward / back)
	hkReal				m_inputUD;	

	/// Set this if you want the character to try and jump
	hkBool				m_wantJump;	

	//
	// Orientation information
	//
	
	/// Up vector in world space - should generally point in the opposite direction to gravity
	hkVector4			m_up;		

	/// Forward vector in world space - point in the direction the character is facing
	hkVector4			m_forward;	

	//
	// Spatial info
	//

	/// Set this if the character is at a ladder and you want it to start to climb
	hkBool				m_atLadder;			

		/// The surface information.
	hkpSurfaceInfo		m_surfaceInfo;

	//
	// Simulation info
	//

	/// Set this to the timestep between calls to the state machine
	hkStepInfo			m_stepInfo;

	/// Set this to the current position
	hkVector4			m_position;

	/// Set this to the current Velocity
	hkVector4			m_velocity;

	/// The gravity that is applied to the character when in the air
	hkVector4			m_characterGravity;

	//
	// User Data
	//

	/// Tag in extra user data for new user states here
	hkUlong m_userData; // +default(0)
};

/// The output from the state machine
struct hkpCharacterOutput
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpCharacterOutput );

		/// The output velocity of the character
	hkVector4 m_velocity;	
};


/// The character context holds the current state of the state machine and is the interface that handles all state machine requests.
class hkpCharacterContext : public hkReferencedObject
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CHARACTER);

			/// This enum defines type of character controlled by state machine
		enum CharacterType
		{
			HK_CHARACTER_PROXY = 0,
			HK_CHARACTER_RIGIDBODY = 1
		};

			/// Initializes the character context and give the state machine an initial state
			/// This adds a reference to the state manager
		hkpCharacterContext(const hkpCharacterStateManager* manager, hkpCharacterStateType initialState);

			/// Removes the reference from the state manager
		~hkpCharacterContext();

			/// Returns the current state
		hkpCharacterStateType getState() const;

			/// Causes a state transition. This also calls the leaveState and enterState methods
			/// for the appropriate states
		void setState(hkpCharacterStateType state, const hkpCharacterInput& input, hkpCharacterOutput& output );

			/// Updates the state machine using the given input
			/// The output structure in initialized before being passed to the state
			/// This initialization copies the velocity from the input
		void update(const hkpCharacterInput& input, hkpCharacterOutput& output);

			/// Set type of character (proxy or rigid body)
		void setCharacterType(const CharacterType newType);

			/// Enable final output velocity filtration
		void setFilterEnable(const hkBool status);
		
			/// Set parameters for final output velocity filtration
		void setFilterParameters(const hkReal gain, const hkReal maxVelocity, const hkReal maxAcceleration);
		
	protected:

			/// Current character type
		CharacterType m_characterType;

		const hkpCharacterStateManager* m_stateManager;

		hkpCharacterStateType m_currentState;

		hkpCharacterStateType m_previousState;
		
			/// Parameters for final output velocity filtering
		hkBool m_filterEnable; 

		hkReal m_maxLinearAcceleration;

		hkReal m_maxLinearVelocity;

		hkReal m_gain;
		
};

#endif // HK_CHARACTER_CONTEXT_H

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

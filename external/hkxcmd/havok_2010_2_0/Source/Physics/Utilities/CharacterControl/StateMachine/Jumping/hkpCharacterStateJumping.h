/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_STATE_JUMPING
#define HK_CHARACTER_STATE_JUMPING

#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterState.h>

/// A jumping state.
/// During update this state adjusts the upward velocity based on current world gravity.
/// The character remains in this state for an extra frame, and then transitions to the InAir state.
/// This is to make sure the character is clear of the ground when the character enters the inAir state:
/// if the character is not clear of the ground, it will transition back to onGround, and groundHugging
/// will kill the upwards velocity of the jump.
class hkpCharacterStateJumping : public hkpCharacterState
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpCharacterStateJumping();

			/// Return the state type
		virtual hkpCharacterStateType getType() const;

			/// Process the user input - causes state actions.
		virtual void update(hkpCharacterContext& context, const hkpCharacterInput& input, hkpCharacterOutput& output);

			///	Process the user input - causes state transitions.
		virtual void change(hkpCharacterContext& context, const hkpCharacterInput& input, hkpCharacterOutput& output);
			/// Get the jump height
		hkReal getJumpHeight() const;

			/// Set the jump height
		void setJumpHeight( hkReal jumpHeight );

		virtual void enterState( hkpCharacterContext& context, hkpCharacterStateType prevState, const hkpCharacterInput& input, hkpCharacterOutput& output );

	protected:

		hkReal m_jumpHeight;
		int m_jumpCounter;
};

#endif // HK_CHARACTER_STATE_JUMPING

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

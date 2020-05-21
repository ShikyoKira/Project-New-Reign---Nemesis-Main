/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_FIRST_PERSON_CHARACTER__H
#define HKP_FIRST_PERSON_CHARACTER__H

class hkpCharacterRigidBody;
struct hkpCharacterInput;
class hkpCharacterContext;
struct hkpFirstPersonCharacterCinfo;
#include <Physics/Utilities/Weapons/hkpFirstPersonGun.h>


class hkpFirstPersonCharacter : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			// Create a default first person character
		hkpFirstPersonCharacter( const hkpFirstPersonCharacterCinfo& cinfo );

		~hkpFirstPersonCharacter();


		struct CharacterControls 
		{
			// These values are from -1 to 1, and m_forward is a unit vector for convenience we are using the default character controller state machine
			// TODO - update the default state machine (or do a new one) that is more usable for AI
			hkReal m_forwardBack;
			hkReal m_straffeLeftRight;
			hkVector4 m_forward;

			hkBool m_fire; // Left mouse button
			hkBool m_fireRmb; // Right mouse button

			hkBool m_wantJump;
			hkBool m_disableDPad;
		};


	public:

		void update( hkReal timestep, const CharacterControls& controls, bool showForwardDirection = false);

		hkpFirstPersonGun* setGun( hkpFirstPersonGun* listener );

			/// create a simple character with a capsule shape, the \a capsuleHeight is the total height of the capsule
		void createDefaultCharacterController( const hkVector4& position, const hkVector4& up, hkReal capsuleHeight = 2.0f, hkReal capsuleRadius = 0.6f );

			/// Utility to create a default state machine
		void createDefaultCharacterContext();

		void getForwardDir( hkVector4& forward ) const;

			/// sets the current angles based on the forward direction
		void setForwardDir( const hkVector4& forward );

			// Helper functions

			/// Calls check support
		void setCharacterInputFromCharacterRb( hkpCharacterInput& input );

		HK_FORCE_INLINE void reset() { if (m_currentGun) m_currentGun->reset(m_world); }

		void getViewTransform(hkTransform& viewTransformOut) const;

		hkpRigidBody* getRigidBody() const;
	

	public:

		hkReal m_verticalSensitivity;
		hkReal m_horizontalSensitivity;
		hkReal m_sensivityPadX;
		hkReal m_sensivityPadY;
		hkReal m_eyeHeight;
		hkReal m_gravityStrength;

		hkReal m_maxUpDownAngle;
		hkUint32 m_numFramesPerShot;
		hkReal m_forwardBackwardSpeedModifier;
		hkReal m_leftRightSpeedModifier;

		// Flags
		enum ControlFlags
		{
			NO_FLAGS = 0,
			CAN_DETACH_FROM_CHAR = 1,
			HAS_USER_CONTROL = 2,
			MAKE_OCCLUDING_OBJECTS_TRANSPARENT = 4,
			DISABLE_JUMP = 8,
			INVERT_UP_DOWN = 16,
			DISABLE_DPAD = 32
		};
		hkUint32 m_flags;

		int m_gunCounter;
		int m_gunCounterRmb;

		hkpWorld* m_world;
		hkpCharacterRigidBody* m_characterRb;
		hkpCharacterContext* m_characterRbContext;

		hkReal m_currentAngle;
		hkReal m_currentElevation;

		hkBool m_specialGravity; // if true, use below vector instead of m_world->getGravity
		hkVector4 m_gravity;

		hkpFirstPersonGun* m_currentGun;
};

struct hkpFirstPersonCharacterCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES, hkpFirstPersonCharacterCinfo);

	hkpFirstPersonCharacterCinfo();

	hkVector4 m_position;
	hkVector4 m_direction;
	hkVector4 m_up;
	hkReal m_gravityStrength;
	hkReal m_capsuleHeight;		// the total height
	hkReal m_capsuleRadius;
	hkReal m_eyeHeight;		// the height of the eyes relative to the characters center

	hkReal m_verticalSensitivity;
	hkReal m_horizontalSensitivity;
	hkReal m_sensivityPadX;
	hkReal m_sensivityPadY;

	hkpFirstPersonCharacter::ControlFlags m_flags;

	hkReal m_maxUpDownAngle;
	hkUint32 m_numFramesPerShot;
	hkReal m_forwardBackwardSpeedModifier;
	hkReal m_leftRightSpeedModifier;

	// Optional, default to HK_NULL. Specify if you want a custom character representation.
	hkpCharacterRigidBody* m_characterRb;
	hkpCharacterContext* m_context;

	// must exist
	hkpWorld* m_world;
};

#endif // HKP_FIRST_PERSON_CHARACTER__H

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

/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_RIGID_BODY_CINFO__H
#define HK_CHARACTER_RIGID_BODY_CINFO__H

#include <Physics/Utilities/CharacterControl/hkpCharacterControllerCinfo.h>
#include <Common/Base/Reflection/Attributes/hkAttributes.h>

extern const class hkClass hkpCharacterRigidBodyCinfoClass;

class hkpShape;

/// Character controller cinfo
struct hkpCharacterRigidBodyCinfo : public hkpCharacterControllerCinfo
{
	//+vtable(true)
	HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER );
	HK_DECLARE_REFLECTION();

	//
	// Values used to set up rigid body
	//

		/// The collision filter info
		/// See hkpRigidBodyCinfo for details
	hkUint32 m_collisionFilterInfo; //+hk.Ui(visible=false)

		/// The shape
		/// See hkpRigidBodyCinfo for details
	hkpShape* m_shape; //+hk.Ui(visible=false)

		/// Initial position
		/// See hkpRigidBodyCinfo for details
	hkVector4 m_position; //+hk.Ui(visible=false)

		/// Initial rotation
		/// See hkpRigidBodyCinfo for details
	hkQuaternion m_rotation; //+hk.Ui(visible=false)

		///	The mass of character
		/// See hkpRigidBodyCinfo for details
	hkReal m_mass;	//+default(100.0f)
					//+hk.RangeReal(absmin=0.0,absmax=1000.0)
					//+hk.Description("The character rigid body mass.")

		/// Set friction of character
		/// See hkpRigidBodyCinfo for details
	hkReal m_friction;	//+default(0.0f)
						//+hk.RangeReal(absmin=0.0,absmax=1.0)
						//+hk.Description("The friction of the character rigid body.")

		/// Set maximal linear velocity
		/// See hkpRigidBodyCinfo for details
	hkReal m_maxLinearVelocity;	//+default(20.0f)
								//+hk.RangeReal(absmin=0.0,absmax=100.0)
								//+hk.Description("The maximum linear velocity of the character rigid body.")

		/// Set maximal allowed penetration depth
		/// See hkpRigidBodyCinfo for details
	hkReal m_allowedPenetrationDepth;	//+default(-0.1f)
										//+hk.Description("The maximum allowed penetration for this object. The default is -0.1. This is a hint to the engine to see how much CPU the engine should invest to keep this object from penetrating. A good choice is 5% - 20% of the smallest diameter of the object.")  


	//
	// Character controller specific values
	//

		/// Set up direction
	hkVector4 m_up;	//+hk.Ui(visible=false)

		/// Set maximal slope
	hkReal m_maxSlope;	//+default(1.04719755f)
						//+hk.RangeReal(absmin=0.0,absmax=1.57079633)
						//+hk.Description("The maximum slope that the character can walk up. This angle is measured in radians from the horizontal. The default value is pi / 3.")

		/// Set maximal force of character
	hkReal m_maxForce;	//+default(1000.0f)
						//+hk.RangeReal(absmin=0.0,absmax=100000.0)
						//+hk.Description(" The maximum force of character.")

		/// If the character's shape is a capsule, then this is used to define the height of a region around its center
		/// where we redirect contact point normals. The region extends from above the capsule's upper vertex to below
		/// the lower vertex by this height, expressed as a factor of the capsule's radius.
	hkReal m_unweldingHeightOffsetFactor;	//+default(0.5f)
											//+hk.RangeReal(absmin=0.0,absmax=100.0)
											//+hk.Description("If the character's shape is a capsule, then this is used to define the height of a region around its center where we redirect contact point normals. The region extends from above the capsule's upper vertex to below the lower vertex by this height, expressed as a factor of the capsule's radius.")

	//
	// Parameters used by checkSupport
	//

		/// Set maximal speed for simplex solver
	hkReal m_maxSpeedForSimplexSolver;	//+default(10.0f)
										//+hk.RangeReal(absmin=0.0,absmax=100.0)
										//+hk.Description("The maximum speed for the simplex solver.")

		/// A character is considered supported if it is less than this distance above its supporting planes.
	hkReal m_supportDistance;	//+default(0.1f)
								//+hk.RangeReal(absmin=0.0,absmax=1.0)
								//+hk.Description("A character is considered supported if it is less than this distance above its supporting planes.")

		/// A character should keep falling until it is this distance or less from its supporting planes.
	hkReal m_hardSupportDistance;	//+default(0.0f)
									//+hk.RangeReal(absmin=0.0,absmax=1.0)
									//+hk.Description("A character should keep falling until it is this distance or less from its supporting planes.")

		/// Set color of character for the visual debugger
	hkInt32	m_vdbColor; //+hk.Ui(visible=false)

		/// Constructor. Sets some defaults.
	hkpCharacterRigidBodyCinfo()
	{
		m_mass = 100.0f;
		m_maxForce = 1000.0f;
		m_friction = 0.0f;
		m_maxSlope = HK_REAL_PI / 3.0f;
		m_unweldingHeightOffsetFactor = 0.5f;
		m_up.set( 0,1,0 );
		m_maxLinearVelocity = 20.0f;
		m_allowedPenetrationDepth = -0.1f;
		m_maxSpeedForSimplexSolver = 10.0f;
		m_collisionFilterInfo = 0;
		m_position.setZero4();
		m_rotation.setIdentity();
		m_supportDistance = 0.1f;
		m_hardSupportDistance = 0.0f;
		m_vdbColor = 0xA0FF0000;
		m_shape = HK_NULL;
	}

	public:

		hkpCharacterRigidBodyCinfo( hkFinishLoadedObjectFlag flag ) : hkpCharacterControllerCinfo(flag) {}
};

#endif // HK_CHARACTER_RIGID_BODY_CINFO__H

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

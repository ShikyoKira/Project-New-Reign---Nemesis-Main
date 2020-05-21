/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_FIRST_PERSON_GUN_H
#define HKP_FIRST_PERSON_GUN_H

#include <Common/Base/Reflection/Attributes/hkAttributes.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Collide/Agent/Collidable/hkpCollidable.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>


/// Class for reacting on bullet creation/deletion etc.
/// Guns that use a physics object as a bullet, should call the methods at appropriate places.
/// \warning This class is reference counted.
class hkpFirstPersonGunBulletListener : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Called after a bullet has been created before it is added to the world.
		virtual void bulletCreatedCallback(hkpRigidBody* bullet) {}

			/// Called before the bullet is removed from the world and destroyed.
		virtual void bulletDeletedCallback(hkpRigidBody* bullet) {}

			/// Called whenever the gun is fired. Both \a from and \a to are allowed to be NULL if the gun has no specific origin or target.
		virtual void gunFiredCallback(const hkVector4* from, const hkVector4* to) {}
};


extern const hkClass hkpFirstPersonGunClass;

/// Base class for weapons, used by the Havok demos.
/// One can register listeners to obtain bullet created/deleted events.
/// Typical usage would be for creating/deleting graphics representations of the bullet.
class hkpFirstPersonGun : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		HK_DECLARE_REFLECTION();

		enum Type
		{
			WEAPON_TYPE_INVALID = 0,
			WEAPON_TYPE_BALLGUN,
			WEAPON_TYPE_GRENADEGUN,
			WEAPON_TYPE_GRAVITYGUN,
			WEAPON_TYPE_MOUNTEDBALLGUN,
			WEAPON_TYPE_TWEAKERGUN,
			WEAPON_TYPE_MISSILEGUN,
			WEAPON_TYPE_RAYCASTGUN,
			WEAPON_TYPE_SPHEREGUN,
			WEAPON_TYPE_STICKYGUN,
			WEAPON_TYPE_NUM_TYPES
		};

		enum KeyboardKey
		{
			KEY_F1 = 0x70,
			KEY_F2 = 0x71,
			KEY_F3 = 0x72,
			KEY_F4 = 0x73,
			KEY_F5 = 0x74,
			KEY_F6 = 0x75,
			KEY_F7 = 0x76,
			KEY_F8 = 0x77,
			KEY_F9 = 0x78,
			KEY_F10 = 0x79,
			KEY_F11 = 0x7A,
			KEY_F12 = 0x7B
		};

		hkpFirstPersonGun();

		hkpFirstPersonGun(class hkFinishLoadedObjectFlag flag);

		~hkpFirstPersonGun();

	public:

			/// Use to create gun display
		virtual void initGun( hkpWorld* world ) {}

			/// Fire bullets
		virtual void fireGun( hkpWorld* world, const hkTransform& viewTransform ) {}

			/// Use to update display, remove bullets from world etc
		virtual void stepGun( hkReal timeStep, hkpWorld* world, const hkpRigidBody* characterBody, const hkTransform& viewTransform, bool fire, bool fireRmb ) {}

			/// Use to clean up gun display
		virtual void quitGun( hkpWorld* world );

			/// Get the name of the gun
		virtual const char* getName() const;

		virtual void reset(hkpWorld* world) {}

		struct SweepSphereOut
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkpFirstPersonGun::SweepSphereOut);

			hkContactPoint m_contactPoint;
			hkpShapeKey m_shapeKey;
			hkpRigidBody* m_body;
		};

		static hkResult HK_CALL sweepSphere(const hkpWorld* world, const hkVector4& sweepStart, hkReal radius,  const hkVector4& sweepEnd, SweepSphereOut& out );

		static void     HK_CALL calcVelocityToTarget(const hkVector4& position, const hkVector4& target, const hkVector4& gravity, hkReal speed, hkVector4& velocity);

	public:

		hkEnum<Type, hkUint8> m_type;               //+nosave

		hkStringPtr m_name;                         //+default("") +hk.Ui(visible=false)

		hkEnum<KeyboardKey, hkUint8> m_keyboardKey; //+default(hkpFirstPersonGun::KEY_F2) +hk.Ui(visible=true)

		hkArray<hkpFirstPersonGunBulletListener*> m_listeners; //+nosave
};


#endif // HKP_FIRST_PERSON_GUN_H

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

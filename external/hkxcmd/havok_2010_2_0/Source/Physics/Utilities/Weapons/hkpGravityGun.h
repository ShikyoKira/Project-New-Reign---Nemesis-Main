/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_GRAVITY_GUN_H
#define HKP_GRAVITY_GUN_H

#include <Physics/Utilities/Weapons/hkpFirstPersonGun.h>

extern const hkClass hkpGravityGunClass;

/// Simple gravity gun implementation.
/// This gun does not call the listeners methods.
class hkpGravityGun : public hkpFirstPersonGun
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		HK_DECLARE_REFLECTION();

		hkpGravityGun();
		hkpGravityGun(class hkFinishLoadedObjectFlag flag);

	public:

		virtual void stepGun( hkReal timeStep, hkpWorld* world, const hkpRigidBody* body, const hkTransform& viewTransform, bool fire, bool fireRmb );
		virtual void reset( hkpWorld* world );

	private:

		hkpRigidBody* pickBody( const hkTransform& viewTransform, const hkpRigidBody* characterBody, const hkpWorld* world ) const;
		void takeObject( hkpRigidBody* body );
		hkpRigidBody* dropObject( int index );
		void gravityGunPull(const hkTransform& viewTransform, const hkpRigidBody* characterBody, const hkpWorld* world);
		void gravityGunPush(const hkTransform& viewTransform, const hkpRigidBody* characterBody, const hkpWorld* world);
		void gravityGunStep(hkReal timeStep, const hkTransform& viewTransform, const hkpRigidBody* characterBody);
		void gravityGunPutDown(const hkTransform& viewTransform, const hkpRigidBody* characterBody, const hkpWorld* world);

	public:

		hkArray<class hkpRigidBody*> m_grabbedBodies; //+nosave

		int m_maxNumObjectsPicked;                    //+default(10) +hk.RangeInt32(absmin=0,absmax=1000) +hk.Ui(visible=true, label="Max. Objects Picked")

		hkReal m_maxMassOfObjectPicked;               //+default(200.0f) +hk.RangeReal(absmin=0.0,absmax=1000000.0) +hk.Ui(visible=true, label="Max. Object Mass")

		hkReal m_maxDistOfObjectPicked;				  //+default(50.0f)	+hk.Semantics("DISTANCE") +hk.RangeReal(absmin=0.0,absmax=10000.0) +hk.Ui(visible=true, label="Max. Distance")

		hkReal m_impulseAppliedWhenObjectNotPicked;   //+default(100.0f) +hk.RangeReal(absmin=0.0,absmax=1000.0) +hk.Ui(visible=true, label="Impulse on Unpicked")

		hkReal m_throwVelocity;						  //+default(50.0f)	+hk.RangeReal(absmin=0.0,absmax=1000.0,softmin=0.0,softmax=100.0) +hk.Ui(visible=true, label="Throw Velocity")

		hkVector4 m_capturedObjectPosition;			  //+default(2.5f,0.6f,0.0f) +hk.Semantics("POSITION") +hk.RangeReal(absmin=-10000.0,absmax=100000.0) +hk.Ui(visible=true, label="Captured Object's Position", advanced=true)

		hkVector4 m_capturedObjectsOffset;			  //+default(0.0f,1.0f,0.0f) +hk.Semantics("POSITION") +hk.RangeReal(absmin=-10000.0,absmax=100000.0) +hk.Ui(visible=true, label="Captured Object's Offset", advanced=true)
};

#endif // HKP_GRAVITY_GUN_H

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

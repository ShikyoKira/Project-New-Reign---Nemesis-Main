/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_BALL_GUN_H
#define HKP_BALL_GUN_H

#include <Common/Base/Container/Queue/hkQueue.h>
#include <Physics/Utilities/Weapons/hkpFirstPersonGun.h>

extern const hkClass hkpBallGunClass;

/// A simple ball gun that throws objects (physics shape = sphere) from the camera position into the world.
class hkpBallGun : public hkpFirstPersonGun
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		HK_DECLARE_REFLECTION();

		hkpBallGun( int numBullets=100 );
		hkpBallGun(class hkFinishLoadedObjectFlag flag);

		virtual ~hkpBallGun();

	public:

		virtual void fireGun( hkpWorld* world, const hkTransform& viewTransform );

		virtual void reset( hkpWorld* world );

	public:

		hkReal m_bulletRadius;						//+default(0.2f) +hk.Semantics("DISTANCE") +hk.RangeReal(absmin=0.0,absmax=100.0,softmin=0.0,softmax=1.0) +hk.Ui(visible=true, label="Radius", group="Bullet")

		hkReal m_bulletVelocity;					//+default(40.0f) +hk.RangeReal(absmin=0.0,absmax=10000.0,softmin=0.0,softmax=100.0) +hk.Ui(visible=true, label="Velocity")

		hkReal m_bulletMass;						//+default(50.0f) +hk.RangeReal(absmin=0.0,absmax=1000000.0,softmin=0.0,softmax=100.0) +hk.Ui(visible=true, label="Mass")

		hkReal m_damageMultiplier;					//+default(50.0f) +hk.RangeReal(absmin=0.0,absmax=10000.0,softmin=0.0,softmax=10.0) +hk.Ui(visible=true, endGroup=true)

		int m_maxBulletsInWorld;					//+default(100) +hk.RangeInt32(absmin=0,absmax=100) +hk.Ui(visible=true)

		hkVector4 m_bulletOffsetFromCenter;			//+default(0.0f,0.0f,0.0f) +hk.Semantics("POSITION") +hk.RangeReal(absmin=-10000.0,absmax=100000.0) +hk.Ui(visible=true, label="Center Offset")

		hkQueue<class hkpRigidBody*>* m_addedBodies;//+nosave
};

#endif // HKP_BALL_GUN_H

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

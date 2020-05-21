/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKP_MOUNTED_BALL_GUN_H
#define HKP_MOUNTED_BALL_GUN_H

#include <Physics/Utilities/Weapons/hkpBallGun.h>

extern const hkClass hkpMountedBallGunClass;

/// A ball gun that is fixed at a position in the world and shoots from there.
class hkpMountedBallGun : public hkpBallGun
{
    public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		HK_DECLARE_REFLECTION();

		hkpMountedBallGun(int numBullets=100);
		hkpMountedBallGun(class hkFinishLoadedObjectFlag flag);

	public:

		virtual void fireGun(hkpWorld* world, const hkTransform& viewTransform);

	public:

			/// The gun will be 'positioned' at this location, i.e., its projectiles will be fired from here.
		hkVector4 m_position; //+default(0.0f,100.0f,0.0f) +hk.Semantics("POSITION") +hk.RangeReal(absmin=-10000.0,absmax=100000.0) +hk.Ui(visible=true, label="World Position")
};

#endif // HKP_MOUNTED_BALL_GUN_H

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

#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "BSLookAtModifierBoneData_0.h"
#include "hkbEventProperty_1.h"

class BSLookAtModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSLookAtModifier(void) {}
   HK_FORCE_INLINE BSLookAtModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_bones(flag)
   , m_eyeBones(flag)
   , m_pSkeletonMemory(flag)
      {}
   // Properties
   hkBool m_lookAtTarget;
   hkArray<BSLookAtModifierBoneData> m_bones;
   hkArray<BSLookAtModifierBoneData> m_eyeBones;
   hkReal m_limitAngleDegrees;
   hkReal m_limitAngleThresholdDegrees;
   hkBool m_continueLookOutsideOfLimit;
   hkReal m_onGain;
   hkReal m_offGain;
   hkBool m_useBoneGains;
   hkVector4 m_targetLocation;
   hkBool m_targetOutsideLimits;
   hkbEventProperty m_targetOutOfLimitEvent;
   hkBool m_lookAtCamera;
   hkReal m_lookAtCameraX;
   hkReal m_lookAtCameraY;
   hkReal m_lookAtCameraZ;
   hkReal m_timeStep;
   hkBool m_ballBonesValid;
   hkRefVariant m_pSkeletonMemory;
};
extern const hkClass BSLookAtModifierClass;


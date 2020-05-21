#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSLimbIKModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSLimbIKModifier(void) {}
   HK_FORCE_INLINE BSLimbIKModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_pSkeletonMemory(flag)
      {}
   // Properties
   hkReal m_limitAngleDegrees;
   hkReal m_currentAngle;
   hkInt16 m_startBoneIndex;
   hkInt16 m_endBoneIndex;
   hkReal m_gain;
   hkReal m_boneRadius;
   hkReal m_castOffset;
   hkReal m_timeStep;
   hkRefVariant m_pSkeletonMemory;
};
extern const hkClass BSLimbIKModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSDirectAtModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSDirectAtModifier(void) {}
   HK_FORCE_INLINE BSDirectAtModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_pSkeletonMemory(flag)
   , m_boneChainIndices(flag)
      {}
   // Properties
   hkBool m_directAtTarget;
   hkInt16 m_sourceBoneIndex;
   hkInt16 m_startBoneIndex;
   hkInt16 m_endBoneIndex;
   hkReal m_limitHeadingDegrees;
   hkReal m_limitPitchDegrees;
   hkReal m_offsetHeadingDegrees;
   hkReal m_offsetPitchDegrees;
   hkReal m_onGain;
   hkReal m_offGain;
   hkVector4 m_targetLocation;
   hkUint32 m_userInfo;
   hkBool m_directAtCamera;
   hkReal m_directAtCameraX;
   hkReal m_directAtCameraY;
   hkReal m_directAtCameraZ;
   hkBool m_active;
   hkReal m_currentHeadingOffset;
   hkReal m_currentPitchOffset;
   hkReal m_timeStep;
   hkRefVariant m_pSkeletonMemory;
   hkBool m_hasTarget;
   hkVector4 m_directAtTargetLocation;
   hkArray<hkRefVariant> m_boneChainIndices;
};
extern const hkClass BSDirectAtModifierClass;


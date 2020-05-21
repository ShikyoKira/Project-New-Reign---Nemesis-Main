#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSComputeAddBoneAnimModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSComputeAddBoneAnimModifier(void) {}
   HK_FORCE_INLINE BSComputeAddBoneAnimModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_pSkeletonMemory(flag)
      {}
   // Properties
   hkInt16 m_boneIndex;
   hkVector4 m_translationLSOut;
   hkQuaternion m_rotationLSOut;
   hkVector4 m_scaleLSOut;
   hkRefVariant m_pSkeletonMemory;
};
extern const hkClass BSComputeAddBoneAnimModifierClass;


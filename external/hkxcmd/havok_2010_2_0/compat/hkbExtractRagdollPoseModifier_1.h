#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbExtractRagdollPoseModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbExtractRagdollPoseModifier(void) {}
   HK_FORCE_INLINE hkbExtractRagdollPoseModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkInt16 m_poseMatchingBone0;
   hkInt16 m_poseMatchingBone1;
   hkInt16 m_poseMatchingBone2;
   hkBool m_enableComputeWorldFromModel;
};
extern const hkClass hkbExtractRagdollPoseModifierClass;


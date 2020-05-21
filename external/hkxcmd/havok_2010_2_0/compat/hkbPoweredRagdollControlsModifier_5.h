#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbPoweredRagdollControlData_3.h"
#include "hkbBoneIndexArray_0.h"
#include "hkbWorldFromModelModeData_0.h"
#include "hkbBoneWeightArray_0.h"

class hkbPoweredRagdollControlsModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbPoweredRagdollControlsModifier(void) {}
   HK_FORCE_INLINE hkbPoweredRagdollControlsModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_bones(flag)
   , m_boneWeights(flag)
      {}
   // Properties
   hkChar m_padding[4];
   hkbPoweredRagdollControlData m_controlData;
   hkRefPtr<hkbBoneIndexArray> m_bones;
   hkbWorldFromModelModeData m_worldFromModelModeData;
   hkRefPtr<hkbBoneWeightArray> m_boneWeights;
};
extern const hkClass hkbPoweredRagdollControlsModifierClass;


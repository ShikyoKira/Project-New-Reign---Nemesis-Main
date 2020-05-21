#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbRigidBodyRagdollControlData_1.h"
#include "hkbBoneIndexArray_0.h"

class hkbRigidBodyRagdollControlsModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRigidBodyRagdollControlsModifier(void) {}
   HK_FORCE_INLINE hkbRigidBodyRagdollControlsModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_bones(flag)
      {}
   // Properties
   hkChar m_padding0[4];
   hkbRigidBodyRagdollControlData m_controlData;
   hkRefPtr<hkbBoneIndexArray> m_bones;
   hkChar m_padding1[12];
};
extern const hkClass hkbRigidBodyRagdollControlsModifierClass;


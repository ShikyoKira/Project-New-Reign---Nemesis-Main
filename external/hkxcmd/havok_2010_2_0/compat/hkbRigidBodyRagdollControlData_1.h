#pragma once

#include <Common/Base/hkBase.h>
#include <Animation/Ragdoll/Controller/RigidBody/hkaKeyFrameHierarchyUtility.h>

struct hkbRigidBodyRagdollControlData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbRigidBodyRagdollControlData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbRigidBodyRagdollControlData(void) {}
   // Properties
   hkaKeyFrameHierarchyUtility::ControlData m_keyFrameHierarchyControlData;
   hkReal m_durationToBlend;
   hkChar m_padding[12];
};
extern const hkClass hkbRigidBodyRagdollControlDataClass;


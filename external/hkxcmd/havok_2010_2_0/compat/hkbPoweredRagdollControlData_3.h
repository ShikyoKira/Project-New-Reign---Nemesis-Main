#pragma once

#include <Common/Base/hkBase.h>

struct hkbPoweredRagdollControlData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbPoweredRagdollControlData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbPoweredRagdollControlData(void) {}
   // Properties
   hkReal m_maxForce;
   hkReal m_tau;
   hkReal m_damping;
   hkReal m_proportionalRecoveryVelocity;
   hkReal m_constantRecoveryVelocity;
   char m_padding[12];
};
extern const hkClass hkbPoweredRagdollControlDataClass;


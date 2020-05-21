#pragma once

#include <Common/Base/hkBase.h>

struct BSLookAtModifierBoneData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, BSLookAtModifierBoneData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE BSLookAtModifierBoneData(void) {}
   // Properties
   hkInt16 m_index;
   hkVector4 m_fwdAxisLS;
   hkReal m_limitAngleDegrees;
   hkReal m_onGain;
   hkReal m_offGain;
   hkBool m_enabled;
   hkVector4 m_currentFwdAxisLS;
};
extern const hkClass BSLookAtModifierBoneDataClass;


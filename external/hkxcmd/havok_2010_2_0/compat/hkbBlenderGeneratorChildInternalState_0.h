#pragma once

#include <Common/Base/hkBase.h>

struct hkbBlenderGeneratorChildInternalState
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbBlenderGeneratorChildInternalState );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbBlenderGeneratorChildInternalState(void) {}
   // Properties
   hkBool m_isActive;
   hkBool m_syncNextFrame;
};
extern const hkClass hkbBlenderGeneratorChildInternalStateClass;


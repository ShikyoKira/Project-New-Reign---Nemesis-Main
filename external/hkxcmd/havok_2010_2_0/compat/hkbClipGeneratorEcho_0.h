#pragma once

#include <Common/Base/hkBase.h>

struct hkbClipGeneratorEcho
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbClipGeneratorEcho );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbClipGeneratorEcho(void) {}
   // Properties
   hkReal m_offsetLocalTime;
   hkReal m_weight;
   hkReal m_dwdt;
   hkChar m_padding[4];
};
extern const hkClass hkbClipGeneratorEchoClass;


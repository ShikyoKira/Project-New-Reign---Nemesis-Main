#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventProperty_1.h"

struct hkbClipTrigger
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbClipTrigger );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbClipTrigger(void) {}
   // Properties
   hkReal m_localTime;
   hkbEventProperty m_event;
   hkBool m_relativeToEndOfClip;
   hkBool m_acyclic;
   hkBool m_isAnnotation;
};
extern const hkClass hkbClipTriggerClass;


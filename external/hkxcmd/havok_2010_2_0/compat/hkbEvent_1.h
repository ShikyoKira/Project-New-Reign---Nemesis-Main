#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventBase_0.h"

struct hkbEvent : public hkbEventBase
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEvent );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEvent(void) {}
   // Properties
   hkRefVariant m_sender;
};
extern const hkClass hkbEventClass;


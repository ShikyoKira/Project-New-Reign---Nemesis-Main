#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEvent_1.h"

struct hkbEventSequencedDataSequencedEvent
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEventSequencedDataSequencedEvent );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEventSequencedDataSequencedEvent(void) {}
   // Properties
   hkbEvent m_event;
   hkReal m_time;
};
extern const hkClass hkbEventSequencedDataSequencedEventClass;


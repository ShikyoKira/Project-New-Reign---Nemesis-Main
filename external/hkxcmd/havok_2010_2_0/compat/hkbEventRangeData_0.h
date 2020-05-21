#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventProperty_1.h"

struct hkbEventRangeData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEventRangeData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEventRangeData(void) {}
   enum EventRangeMode
   {
      EVENT_MODE_SEND_ON_ENTER_RANGE = 0,
      EVENT_MODE_SEND_WHEN_IN_RANGE = 1,
   };

   // Properties
   hkReal m_upperBound;
   hkbEventProperty m_event;
   hkEnum<EventRangeMode,hkInt8> m_eventMode;
};
extern const hkClass hkbEventRangeDataClass;


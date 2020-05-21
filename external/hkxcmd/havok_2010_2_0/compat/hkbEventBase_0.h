#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

struct hkbEventBase
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEventBase );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEventBase(void) {}
   enum SystemEventIds
   {
      EVENT_ID_NULL = -1,
   };

   // Properties
   hkInt32 m_id;
   hkRefPtr<hkbEventPayload> m_payload;
};
extern const hkClass hkbEventBaseClass;


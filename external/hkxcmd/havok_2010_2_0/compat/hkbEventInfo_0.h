#pragma once

#include <Common/Base/hkBase.h>

struct hkbEventInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEventInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEventInfo(void) {}
   enum Flags
   {
      FLAG_SILENT = 1,
      FLAG_SYNC_POINT = 2,
   };

   // Properties
   hkFlags<Flags,hkUint32> m_flags;
};
extern const hkClass hkbEventInfoClass;


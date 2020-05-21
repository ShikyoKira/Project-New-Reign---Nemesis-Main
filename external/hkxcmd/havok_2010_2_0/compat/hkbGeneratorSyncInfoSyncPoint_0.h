#pragma once

#include <Common/Base/hkBase.h>

struct hkbGeneratorSyncInfoSyncPoint
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbGeneratorSyncInfoSyncPoint );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbGeneratorSyncInfoSyncPoint(void) {}
   // Properties
   hkInt32 m_id;
   hkReal m_time;
};
extern const hkClass hkbGeneratorSyncInfoSyncPointClass;


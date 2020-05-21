#pragma once

#include <Common/Base/hkBase.h>

struct hkbStateMachineTimeInterval
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineTimeInterval );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineTimeInterval(void) {}
   // Properties
   hkInt32 m_enterEventId;
   hkInt32 m_exitEventId;
   hkReal m_enterTime;
   hkReal m_exitTime;
};
extern const hkClass hkbStateMachineTimeIntervalClass;


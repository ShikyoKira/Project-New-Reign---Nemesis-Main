#pragma once

#include <Common/Base/hkBase.h>

struct hkbStateMachineNestedStateMachineData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineNestedStateMachineData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineNestedStateMachineData(void) {}
   // Properties
   hkRefVariant m_nestedStateMachine;
   hkRefVariant m_eventIdMap;
};
extern const hkClass hkbStateMachineNestedStateMachineDataClass;


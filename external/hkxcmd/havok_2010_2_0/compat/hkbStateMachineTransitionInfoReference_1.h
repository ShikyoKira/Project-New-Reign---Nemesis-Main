#pragma once

#include <Common/Base/hkBase.h>

struct hkbStateMachineTransitionInfoReference
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineTransitionInfoReference );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineTransitionInfoReference(void) {}
   // Properties
   hkInt16 m_fromStateIndex;
   hkInt16 m_transitionIndex;
   hkInt16 m_stateMachineId;
};
extern const hkClass hkbStateMachineTransitionInfoReferenceClass;


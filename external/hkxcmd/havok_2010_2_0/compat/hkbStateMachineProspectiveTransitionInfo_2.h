#pragma once

#include <Common/Base/hkBase.h>
#include "hkbStateMachineTransitionInfoReference_1.h"

struct hkbStateMachineProspectiveTransitionInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineProspectiveTransitionInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineProspectiveTransitionInfo(void) {}
   // Properties
   hkbStateMachineTransitionInfoReference m_transitionInfoReference;
   hkbStateMachineTransitionInfoReference m_transitionInfoReferenceForTE;
   hkInt32 m_toStateId;
};
extern const hkClass hkbStateMachineProspectiveTransitionInfoClass;


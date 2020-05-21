#pragma once

#include <Common/Base/hkBase.h>
#include "hkbStateMachineProspectiveTransitionInfo_2.h"

struct hkbStateMachineDelayedTransitionInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineDelayedTransitionInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineDelayedTransitionInfo(void) {}
   // Properties
   hkbStateMachineProspectiveTransitionInfo m_delayedTransition;
   hkReal m_timeDelayed;
   hkBool m_isDelayedTransitionReturnToPreviousState;
   hkBool m_wasInAbutRangeLastFrame;
};
extern const hkClass hkbStateMachineDelayedTransitionInfoClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbStateMachineTimeInterval_0.h"
#include "hkbTransitionEffect_0.h"
#include "hkbCondition_0.h"

struct hkbStateMachineTransitionInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineTransitionInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineTransitionInfo(void) {}
   enum TransitionFlags
   {
      FLAG_USE_TRIGGER_INTERVAL = 1,
      FLAG_USE_INITIATE_INTERVAL = 2,
      FLAG_UNINTERRUPTIBLE_WHILE_PLAYING = 4,
      FLAG_UNINTERRUPTIBLE_WHILE_DELAYED = 8,
      FLAG_DELAY_STATE_CHANGE = 16,
      FLAG_DISABLED = 32,
      FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE = 64,
      FLAG_DISALLOW_RANDOM_TRANSITION = 128,
      FLAG_DISABLE_CONDITION = 256,
      FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE = 512,
      FLAG_IS_GLOBAL_WILDCARD = 1024,
      FLAG_IS_LOCAL_WILDCARD = 2048,
      FLAG_FROM_NESTED_STATE_ID_IS_VALID = 4096,
      FLAG_TO_NESTED_STATE_ID_IS_VALID = 8192,
      FLAG_ABUT_AT_END_OF_FROM_GENERATOR = 16384,
   };

   enum InternalFlagBits
   {
      FLAG_INTERNAL_IN_TRIGGER_INTERVAL = 1,
      FLAG_INTERNAL_IN_INITIATE_INTERVAL = 2,
   };

   // Properties
   hkbStateMachineTimeInterval m_triggerInterval;
   hkbStateMachineTimeInterval m_initiateInterval;
   hkRefPtr<hkbTransitionEffect> m_transition;
   hkRefPtr<hkbCondition> m_condition;
   hkInt32 m_eventId;
   hkInt32 m_toStateId;
   hkInt32 m_fromNestedStateId;
   hkInt32 m_toNestedStateId;
   hkInt16 m_priority;
   hkFlags<TransitionFlags,hkInt16> m_flags;
};
extern const hkClass hkbStateMachineTransitionInfoClass;


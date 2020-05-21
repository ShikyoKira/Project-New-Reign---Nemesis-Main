#pragma once

#include <Common/Base/hkBase.h>
#include "hkbNodeInternalStateInfo_0.h"
#include "hkbStateMachineTransitionInfoReference_1.h"

struct hkbStateMachineActiveTransitionInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbStateMachineActiveTransitionInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbStateMachineActiveTransitionInfo(void) {}
   // Properties
   hkRefVariant m_transitionEffect;
   hkRefPtr<hkbNodeInternalStateInfo> m_transitionEffectInternalStateInfo;
   hkbStateMachineTransitionInfoReference m_transitionInfoReference;
   hkbStateMachineTransitionInfoReference m_transitionInfoReferenceForTE;
   hkInt32 m_fromStateId;
   hkInt32 m_toStateId;
   hkBool m_isReturnToPreviousState;
};
extern const hkClass hkbStateMachineActiveTransitionInfoClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbStateMachineActiveTransitionInfo_1.h"
#include "hkbStateMachineDelayedTransitionInfo_1.h"

class hkbStateMachineInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStateMachineInternalState(void) {}
   HK_FORCE_INLINE hkbStateMachineInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_activeTransitions(flag)
   , m_transitionFlags(flag)
   , m_wildcardTransitionFlags(flag)
   , m_delayedTransitions(flag)
      {}
   // Properties
   hkArray<hkbStateMachineActiveTransitionInfo> m_activeTransitions;
   hkArray<hkUint8> m_transitionFlags;
   hkArray<hkUint8> m_wildcardTransitionFlags;
   hkArray<hkbStateMachineDelayedTransitionInfo> m_delayedTransitions;
   hkReal m_timeInState;
   hkReal m_lastLocalTime;
   hkInt32 m_currentStateId;
   hkInt32 m_previousStateId;
   hkInt32 m_nextStartStateIndexOverride;
   hkBool m_stateOrTransitionChanged;
   hkBool m_echoNextUpdate;
};
extern const hkClass hkbStateMachineInternalStateClass;


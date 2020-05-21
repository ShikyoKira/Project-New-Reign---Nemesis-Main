#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "hkbEvent_1.h"
#include "hkbStateChooser_0.h"
#include "hkbStateMachineStateInfo_4.h"
#include "hkbStateMachineTransitionInfoArray_0.h"

class hkbStateMachine : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStateMachine(void) {}
   HK_FORCE_INLINE hkbStateMachine( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_startStateChooser(flag)
   , m_states(flag)
   , m_wildcardTransitions(flag)
   , m_stateIdToIndexMap(flag)
   , m_activeTransitions(flag)
   , m_transitionFlags(flag)
   , m_wildcardTransitionFlags(flag)
   , m_delayedTransitions(flag)
      {}
   enum StartStateMode
   {
      START_STATE_MODE_DEFAULT = 0,
      START_STATE_MODE_SYNC = 1,
      START_STATE_MODE_RANDOM = 2,
      START_STATE_MODE_CHOOSER = 3,
   };

   enum StateMachineSelfTransitionMode
   {
      SELF_TRANSITION_MODE_NO_TRANSITION = 0,
      SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE = 1,
      SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE = 2,
   };

   // Properties
   hkbEvent m_eventToSendWhenStateOrTransitionChanges;
   hkRefPtr<hkbStateChooser> m_startStateChooser;
   hkInt32 m_startStateId;
   hkInt32 m_returnToPreviousStateEventId;
   hkInt32 m_randomTransitionEventId;
   hkInt32 m_transitionToNextHigherStateEventId;
   hkInt32 m_transitionToNextLowerStateEventId;
   hkInt32 m_syncVariableIndex;
   hkInt32 m_currentStateId;
   hkBool m_wrapAroundStateId;
   hkInt8 m_maxSimultaneousTransitions;
   hkEnum<StartStateMode,hkInt8> m_startStateMode;
   hkEnum<StateMachineSelfTransitionMode,hkInt8> m_selfTransitionMode;
   hkBool m_isActive;
   hkArray<hkbStateMachineStateInfo*> m_states;
   hkRefPtr<hkbStateMachineTransitionInfoArray> m_wildcardTransitions;
   hkRefVariant m_stateIdToIndexMap;
   hkArray<hkRefVariant> m_activeTransitions;
   hkArray<hkRefVariant> m_transitionFlags;
   hkArray<hkRefVariant> m_wildcardTransitionFlags;
   hkArray<hkRefVariant> m_delayedTransitions;
   hkReal m_timeInState;
   hkReal m_lastLocalTime;
   hkInt32 m_previousStateId;
   hkInt32 m_nextStartStateIndexOverride;
   hkBool m_stateOrTransitionChanged;
   hkBool m_echoNextUpdate;
   hkUint16 m_sCurrentStateIndexAndEntered;
};
extern const hkClass hkbStateMachineClass;


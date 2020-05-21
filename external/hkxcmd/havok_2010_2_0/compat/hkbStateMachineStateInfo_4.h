#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"
#include "hkbStateListener_0.h"
#include "hkbStateMachineEventPropertyArray_0.h"
#include "hkbStateMachineTransitionInfoArray_0.h"
#include "hkbGenerator_0.h"

class hkbStateMachineStateInfo : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStateMachineStateInfo(void) {}
   HK_FORCE_INLINE hkbStateMachineStateInfo( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_listeners(flag)
   , m_enterNotifyEvents(flag)
   , m_exitNotifyEvents(flag)
   , m_transitions(flag)
   , m_generator(flag)
   , m_name(flag)
      {}
   // Properties
   hkArray<hkbStateListener*> m_listeners;
   hkRefPtr<hkbStateMachineEventPropertyArray> m_enterNotifyEvents;
   hkRefPtr<hkbStateMachineEventPropertyArray> m_exitNotifyEvents;
   hkRefPtr<hkbStateMachineTransitionInfoArray> m_transitions;
   hkRefPtr<hkbGenerator> m_generator;
   hkStringPtr m_name;
   hkInt32 m_stateId;
   hkReal m_probability;
   hkBool m_enable;
};
extern const hkClass hkbStateMachineStateInfoClass;


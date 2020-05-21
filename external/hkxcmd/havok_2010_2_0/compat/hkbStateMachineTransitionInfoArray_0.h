#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbStateMachineTransitionInfo_1.h"

class hkbStateMachineTransitionInfoArray : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStateMachineTransitionInfoArray(void) {}
   HK_FORCE_INLINE hkbStateMachineTransitionInfoArray( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_transitions(flag)
      {}
   // Properties
   hkArray<hkbStateMachineTransitionInfo> m_transitions;
};
extern const hkClass hkbStateMachineTransitionInfoArrayClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbEventProperty_1.h"

class hkbStateMachineEventPropertyArray : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStateMachineEventPropertyArray(void) {}
   HK_FORCE_INLINE hkbStateMachineEventPropertyArray( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_events(flag)
      {}
   // Properties
   hkArray<hkbEventProperty> m_events;
};
extern const hkClass hkbStateMachineEventPropertyArrayClass;


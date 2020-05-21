#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbClipTrigger_1.h"

class hkbClipTriggerArray : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbClipTriggerArray(void) {}
   HK_FORCE_INLINE hkbClipTriggerArray( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_triggers(flag)
      {}
   // Properties
   hkArray<hkbClipTrigger> m_triggers;
};
extern const hkClass hkbClipTriggerArrayClass;


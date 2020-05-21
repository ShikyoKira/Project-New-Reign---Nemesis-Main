#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbNamedEventPayload : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbNamedEventPayload(void) {}
   HK_FORCE_INLINE hkbNamedEventPayload( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
   , m_name(flag)
      {}
   // Properties
   hkStringPtr m_name;
};
extern const hkClass hkbNamedEventPayloadClass;


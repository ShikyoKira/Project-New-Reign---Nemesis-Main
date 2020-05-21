#pragma once

#include <Common/Base/hkBase.h>
#include "hkbNamedEventPayload_0.h"

class hkbNamedStringEventPayload : public hkbNamedEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbNamedStringEventPayload(void) {}
   HK_FORCE_INLINE hkbNamedStringEventPayload( hkFinishLoadedObjectFlag flag ) : hkbNamedEventPayload(flag) 
   , m_data(flag)
      {}
   // Properties
   hkStringPtr m_data;
};
extern const hkClass hkbNamedStringEventPayloadClass;


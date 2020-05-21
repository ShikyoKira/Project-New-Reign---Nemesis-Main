#pragma once

#include <Common/Base/hkBase.h>
#include "hkbNamedEventPayload_0.h"

class hkbNamedIntEventPayload : public hkbNamedEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbNamedIntEventPayload(void) {}
   HK_FORCE_INLINE hkbNamedIntEventPayload( hkFinishLoadedObjectFlag flag ) : hkbNamedEventPayload(flag) 
      {}
   // Properties
   hkInt32 m_data;
};
extern const hkClass hkbNamedIntEventPayloadClass;


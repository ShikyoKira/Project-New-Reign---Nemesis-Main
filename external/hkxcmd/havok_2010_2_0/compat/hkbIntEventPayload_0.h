#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbIntEventPayload : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbIntEventPayload(void) {}
   HK_FORCE_INLINE hkbIntEventPayload( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
      {}
   // Properties
   hkInt32 m_data;
};
extern const hkClass hkbIntEventPayloadClass;


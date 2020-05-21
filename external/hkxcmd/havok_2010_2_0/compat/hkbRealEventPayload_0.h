#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbRealEventPayload : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRealEventPayload(void) {}
   HK_FORCE_INLINE hkbRealEventPayload( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
      {}
   // Properties
   hkReal m_data;
};
extern const hkClass hkbRealEventPayloadClass;


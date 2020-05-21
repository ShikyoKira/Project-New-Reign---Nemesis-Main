#pragma once

#include <Common/Base/hkBase.h>
#include "hkbNamedEventPayload_0.h"

class hkbNamedRealEventPayload : public hkbNamedEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbNamedRealEventPayload(void) {}
   HK_FORCE_INLINE hkbNamedRealEventPayload( hkFinishLoadedObjectFlag flag ) : hkbNamedEventPayload(flag) 
      {}
   // Properties
   hkReal m_data;
};
extern const hkClass hkbNamedRealEventPayloadClass;


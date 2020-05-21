#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbStringEventPayload : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStringEventPayload(void) {}
   HK_FORCE_INLINE hkbStringEventPayload( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
   , m_data(flag)
      {}
   // Properties
   hkStringPtr m_data;
};
extern const hkClass hkbStringEventPayloadClass;


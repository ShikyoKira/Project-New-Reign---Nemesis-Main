#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbEventPayloadList : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventPayloadList(void) {}
   HK_FORCE_INLINE hkbEventPayloadList( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
   , m_payloads(flag)
      {}
   // Properties
   hkArray<hkbEventPayload*> m_payloads;
};
extern const hkClass hkbEventPayloadListClass;


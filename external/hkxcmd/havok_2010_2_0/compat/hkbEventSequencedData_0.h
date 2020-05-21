#pragma once

#include <Common/Base/hkBase.h>
#include "hkbSequencedData_0.h"
#include "hkbEventSequencedDataSequencedEvent_0.h"

class hkbEventSequencedData : public hkbSequencedData
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventSequencedData(void) {}
   HK_FORCE_INLINE hkbEventSequencedData( hkFinishLoadedObjectFlag flag ) : hkbSequencedData(flag) 
   , m_events(flag)
      {}
   // Properties
   hkArray<hkbEventSequencedDataSequencedEvent> m_events;
};
extern const hkClass hkbEventSequencedDataClass;


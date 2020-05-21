#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbEventRangeData_0.h"

class hkbEventRangeDataArray : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventRangeDataArray(void) {}
   HK_FORCE_INLINE hkbEventRangeDataArray( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_eventData(flag)
      {}
   // Properties
   hkArray<hkbEventRangeData> m_eventData;
};
extern const hkClass hkbEventRangeDataArrayClass;


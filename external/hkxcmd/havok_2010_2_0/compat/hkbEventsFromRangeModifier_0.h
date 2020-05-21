#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventRangeDataArray_0.h"

class hkbEventsFromRangeModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventsFromRangeModifier(void) {}
   HK_FORCE_INLINE hkbEventsFromRangeModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_eventRanges(flag)
   , m_wasActiveInPreviousFrame(flag)
      {}
   // Properties
   hkReal m_inputValue;
   hkReal m_lowerBound;
   hkRefPtr<hkbEventRangeDataArray> m_eventRanges;
   hkArray<hkRefVariant> m_wasActiveInPreviousFrame;
};
extern const hkClass hkbEventsFromRangeModifierClass;


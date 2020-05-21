#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class BSEventEveryNEventsModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSEventEveryNEventsModifier(void) {}
   HK_FORCE_INLINE BSEventEveryNEventsModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkbEventProperty m_eventToCheckFor;
   hkbEventProperty m_eventToSend;
   hkInt8 m_numberOfEventsBeforeSend;
   hkInt8 m_minimumNumberOfEventsBeforeSend;
   hkBool m_randomizeNumberOfEvents;
   hkInt32 m_numberOfEventsSeen;
   hkInt8 m_calculatedNumberOfEventsBeforeSend;
};
extern const hkClass BSEventEveryNEventsModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class hkbTimerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbTimerModifier(void) {}
   HK_FORCE_INLINE hkbTimerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkReal m_alarmTimeSeconds;
   hkbEventProperty m_alarmEvent;
   hkReal m_secondsElapsed;
};
extern const hkClass hkbTimerModifierClass;


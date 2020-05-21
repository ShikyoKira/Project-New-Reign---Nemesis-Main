#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class BSTimerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSTimerModifier(void) {}
   HK_FORCE_INLINE BSTimerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkReal m_alarmTimeSeconds;
   hkbEventProperty m_alarmEvent;
   hkBool m_resetAlarm;
   hkReal m_secondsElapsed;
};
extern const hkClass BSTimerModifierClass;


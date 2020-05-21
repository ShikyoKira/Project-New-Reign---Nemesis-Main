#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifierWrapper_0.h"

class hkbDelayedModifier : public hkbModifierWrapper
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbDelayedModifier(void) {}
   HK_FORCE_INLINE hkbDelayedModifier( hkFinishLoadedObjectFlag flag ) : hkbModifierWrapper(flag) 
      {}
   // Properties
   hkReal m_delaySeconds;
   hkReal m_durationSeconds;
   hkReal m_secondsElapsed;
   hkBool m_isActive;
};
extern const hkClass hkbDelayedModifierClass;


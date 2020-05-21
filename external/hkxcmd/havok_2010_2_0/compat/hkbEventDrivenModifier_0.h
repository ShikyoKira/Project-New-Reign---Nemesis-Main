#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifierWrapper_0.h"

class hkbEventDrivenModifier : public hkbModifierWrapper
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventDrivenModifier(void) {}
   HK_FORCE_INLINE hkbEventDrivenModifier( hkFinishLoadedObjectFlag flag ) : hkbModifierWrapper(flag) 
      {}
   // Properties
   hkInt32 m_activateEventId;
   hkInt32 m_deactivateEventId;
   hkBool m_activeByDefault;
   hkBool m_isActive;
};
extern const hkClass hkbEventDrivenModifierClass;


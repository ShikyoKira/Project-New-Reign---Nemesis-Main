#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class BSEventOnDeactivateModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSEventOnDeactivateModifier(void) {}
   HK_FORCE_INLINE BSEventOnDeactivateModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkbEventProperty m_event;
};
extern const hkClass BSEventOnDeactivateModifierClass;


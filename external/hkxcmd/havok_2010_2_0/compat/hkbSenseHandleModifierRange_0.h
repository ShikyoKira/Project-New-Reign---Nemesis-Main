#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventProperty_1.h"

struct hkbSenseHandleModifierRange
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbSenseHandleModifierRange );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbSenseHandleModifierRange(void) {}
   // Properties
   hkbEventProperty m_event;
   hkReal m_minDistance;
   hkReal m_maxDistance;
   hkBool m_ignoreHandle;
};
extern const hkClass hkbSenseHandleModifierRangeClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbHandIkControlData_2.h"

struct hkbHandIkControlsModifierHand
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbHandIkControlsModifierHand );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbHandIkControlsModifierHand(void) {}
   // Properties
   hkbHandIkControlData m_controlData;
   hkInt32 m_handIndex;
   hkBool m_enable;
};
extern const hkClass hkbHandIkControlsModifierHandClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventProperty_1.h"

struct hkbFootIkControlsModifierLeg
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbFootIkControlsModifierLeg );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbFootIkControlsModifierLeg(void) {}
   // Properties
   hkVector4 m_groundPosition;
   hkbEventProperty m_ungroundedEvent;
   hkReal m_verticalError;
   hkBool m_hitSomething;
   hkBool m_isPlantedMS;
};
extern const hkClass hkbFootIkControlsModifierLegClass;


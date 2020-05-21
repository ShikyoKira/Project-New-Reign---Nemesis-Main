#pragma once

#include <Common/Base/hkBase.h>

struct hkbCharacterControllerControlData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbCharacterControllerControlData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbCharacterControllerControlData(void) {}
   // Properties
   hkVector4 m_desiredVelocity;
   hkReal m_verticalGain;
   hkReal m_horizontalCatchUpGain;
   hkReal m_maxVerticalSeparation;
   hkReal m_maxHorizontalSeparation;
};
extern const hkClass hkbCharacterControllerControlDataClass;


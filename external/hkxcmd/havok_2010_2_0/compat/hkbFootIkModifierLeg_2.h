#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventProperty_1.h"

struct hkbFootIkModifierLeg
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbFootIkModifierLeg );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbFootIkModifierLeg(void) {}
   // Properties
   hkQsTransform m_originalAnkleTransformMS;
   hkQuaternion m_prevAnkleRotLS;
   hkVector4 m_kneeAxisLS;
   hkVector4 m_footEndLS;
   hkbEventProperty m_ungroundedEvent;
   hkReal m_footPlantedAnkleHeightMS;
   hkReal m_footRaisedAnkleHeightMS;
   hkReal m_maxAnkleHeightMS;
   hkReal m_minAnkleHeightMS;
   hkReal m_maxKneeAngleDegrees;
   hkReal m_minKneeAngleDegrees;
   hkReal m_verticalError;
   hkReal m_maxAnkleAngleDegrees;
   hkInt16 m_hipIndex;
   hkInt16 m_kneeIndex;
   hkInt16 m_ankleIndex;
   hkBool m_hitSomething;
   hkBool m_isPlantedMS;
   hkBool m_isOriginalAnkleTransformMSSet;
};
extern const hkClass hkbFootIkModifierLegClass;


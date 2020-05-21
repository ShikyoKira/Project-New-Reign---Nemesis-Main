#pragma once

#include <Common/Base/hkBase.h>

struct hkbFootIkGains
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbFootIkGains );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbFootIkGains(void) {}
   // Properties
   hkReal m_onOffGain;
   hkReal m_groundAscendingGain;
   hkReal m_groundDescendingGain;
   hkReal m_footPlantedGain;
   hkReal m_footRaisedGain;
   hkReal m_footUnlockGain;
   hkReal m_worldFromModelFeedbackGain;
   hkReal m_errorUpDownBias;
   hkReal m_alignWorldFromModelGain;
   hkReal m_hipOrientationGain;
   hkReal m_maxKneeAngleDifference;
   hkReal m_ankleOrientationGain;
};
extern const hkClass hkbFootIkGainsClass;


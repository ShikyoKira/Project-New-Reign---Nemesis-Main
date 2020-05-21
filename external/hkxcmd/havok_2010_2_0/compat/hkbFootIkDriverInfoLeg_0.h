#pragma once

#include <Common/Base/hkBase.h>

struct hkbFootIkDriverInfoLeg
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbFootIkDriverInfoLeg );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbFootIkDriverInfoLeg(void) {}
   // Properties
   hkQuaternion m_prevAnkleRotLS;
   hkVector4 m_kneeAxisLS;
   hkVector4 m_footEndLS;
   hkReal m_footPlantedAnkleHeightMS;
   hkReal m_footRaisedAnkleHeightMS;
   hkReal m_maxAnkleHeightMS;
   hkReal m_minAnkleHeightMS;
   hkReal m_maxKneeAngleDegrees;
   hkReal m_minKneeAngleDegrees;
   hkReal m_maxAnkleAngleDegrees;
   hkInt16 m_hipIndex;
   hkInt16 m_kneeIndex;
   hkInt16 m_ankleIndex;
};
extern const hkClass hkbFootIkDriverInfoLegClass;


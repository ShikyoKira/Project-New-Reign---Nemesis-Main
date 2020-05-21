#pragma once

#include <Common/Base/hkBase.h>

struct hkbHandIkModifierHand
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbHandIkModifierHand );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbHandIkModifierHand(void) {}
   // Properties
   hkVector4 m_elbowAxisLS;
   hkVector4 m_backHandNormalLS;
   hkVector4 m_handOffsetLS;
   hkQuaternion m_handOrienationOffsetLS;
   hkReal m_maxElbowAngleDegrees;
   hkReal m_minElbowAngleDegrees;
   hkInt16 m_shoulderIndex;
   hkInt16 m_shoulderSiblingIndex;
   hkInt16 m_elbowIndex;
   hkInt16 m_elbowSiblingIndex;
   hkInt16 m_wristIndex;
   hkBool m_enforceEndPosition;
   hkBool m_enforceEndRotation;
   hkStringPtr m_localFrameName;
};
extern const hkClass hkbHandIkModifierHandClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbLookAtModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbLookAtModifier(void) {}
   HK_FORCE_INLINE hkbLookAtModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_targetWS;
   hkVector4 m_headForwardLS;
   hkVector4 m_neckForwardLS;
   hkVector4 m_neckRightLS;
   hkVector4 m_eyePositionHS;
   hkReal m_newTargetGain;
   hkReal m_onGain;
   hkReal m_offGain;
   hkReal m_limitAngleDegrees;
   hkReal m_limitAngleLeft;
   hkReal m_limitAngleRight;
   hkReal m_limitAngleUp;
   hkReal m_limitAngleDown;
   hkInt16 m_headIndex;
   hkInt16 m_neckIndex;
   hkBool m_isOn;
   hkBool m_individualLimitsOn;
   hkBool m_isTargetInsideLimitCone;
   hkVector4 m_lookAtLastTargetWS;
   hkReal m_lookAtWeight;
};
extern const hkClass hkbLookAtModifierClass;


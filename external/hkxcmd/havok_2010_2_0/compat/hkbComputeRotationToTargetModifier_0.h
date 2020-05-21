#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbComputeRotationToTargetModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbComputeRotationToTargetModifier(void) {}
   HK_FORCE_INLINE hkbComputeRotationToTargetModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkQuaternion m_rotationOut;
   hkVector4 m_targetPosition;
   hkVector4 m_currentPosition;
   hkQuaternion m_currentRotation;
   hkVector4 m_localAxisOfRotation;
   hkVector4 m_localFacingDirection;
   hkBool m_resultIsDelta;
};
extern const hkClass hkbComputeRotationToTargetModifierClass;


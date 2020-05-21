#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbRotateCharacterModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRotateCharacterModifier(void) {}
   HK_FORCE_INLINE hkbRotateCharacterModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkReal m_degreesPerSecond;
   hkReal m_speedMultiplier;
   hkVector4 m_axisOfRotation;
   hkReal m_angle;
};
extern const hkClass hkbRotateCharacterModifierClass;


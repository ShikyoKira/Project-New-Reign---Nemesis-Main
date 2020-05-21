#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSSpeedSamplerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSSpeedSamplerModifier(void) {}
   HK_FORCE_INLINE BSSpeedSamplerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkInt32 m_state;
   hkReal m_direction;
   hkReal m_goalSpeed;
   hkReal m_speedOut;
};
extern const hkClass BSSpeedSamplerModifierClass;


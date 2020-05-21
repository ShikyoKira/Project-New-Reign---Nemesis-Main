#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSInterpValueModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSInterpValueModifier(void) {}
   HK_FORCE_INLINE BSInterpValueModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkReal m_source;
   hkReal m_target;
   hkReal m_result;
   hkReal m_gain;
   hkReal m_timeStep;
};
extern const hkClass BSInterpValueModifierClass;


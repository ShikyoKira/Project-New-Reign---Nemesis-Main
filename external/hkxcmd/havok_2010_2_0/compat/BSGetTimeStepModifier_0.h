#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSGetTimeStepModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSGetTimeStepModifier(void) {}
   HK_FORCE_INLINE BSGetTimeStepModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkReal m_timeStep;
};
extern const hkClass BSGetTimeStepModifierClass;


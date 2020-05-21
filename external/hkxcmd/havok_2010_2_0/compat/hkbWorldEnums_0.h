#pragma once

#include <Common/Base/hkBase.h>

struct hkbWorldEnums
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbWorldEnums );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbWorldEnums(void) {}
   enum SimulationState
   {
      SIMULATION_STATE_PLAY = 0,
      SIMULATION_STATE_PAUSE = 1,
      SIMULATION_STATE_STEP = 2,
      SIMULATION_STATE_STOP = 3,
   };

   enum AccumulateMotionState
   {
      ACCUMULATE_MOTION = 0,
      DO_NOT_ACCUMULATE_MOTION = 1,
   };

   // Properties
};
extern const hkClass hkbWorldEnumsClass;


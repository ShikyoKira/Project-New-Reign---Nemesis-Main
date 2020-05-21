#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class hkbTransitionEffect : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbTransitionEffect(void) {}
   HK_FORCE_INLINE hkbTransitionEffect( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
      {}
   enum SelfTransitionMode
   {
      SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC = 0,
      SELF_TRANSITION_MODE_CONTINUE = 1,
      SELF_TRANSITION_MODE_RESET = 2,
      SELF_TRANSITION_MODE_BLEND = 3,
   };

   enum EventMode
   {
      EVENT_MODE_DEFAULT = 0,
      EVENT_MODE_PROCESS_ALL = 1,
      EVENT_MODE_IGNORE_FROM_GENERATOR = 2,
      EVENT_MODE_IGNORE_TO_GENERATOR = 3,
   };

   // Properties
   hkEnum<SelfTransitionMode,hkInt8> m_selfTransitionMode;
   hkEnum<EventMode,hkInt8> m_eventMode;
   /*enum*/ hkInt8 m_defaultEventMode;
};
extern const hkClass hkbTransitionEffectClass;


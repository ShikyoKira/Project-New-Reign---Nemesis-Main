#pragma once

#include <Common/Base/hkBase.h>
#include "hkbTransitionEffect_0.h"
#include "hkbGenerator_0.h"

class hkbGeneratorTransitionEffect : public hkbTransitionEffect
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGeneratorTransitionEffect(void) {}
   HK_FORCE_INLINE hkbGeneratorTransitionEffect( hkFinishLoadedObjectFlag flag ) : hkbTransitionEffect(flag) 
   , m_transitionGenerator(flag)
   , m_fromGenerator(flag)
   , m_toGenerator(flag)
      {}
   enum ToGeneratorState
   {
      STATE_INACTIVE = 0,
      STATE_READY_FOR_SET_LOCAL_TIME = 1,
      STATE_READY_FOR_APPLY_SELF_TRANSITION_MODE = 2,
      STATE_ACTIVE = 3,
   };

   enum Stage
   {
      STAGE_BLENDING_IN = 0,
      STAGE_PLAYING_TRANSITION_GENERATOR = 1,
      STAGE_BLENDING_OUT = 2,
   };

   // Properties
   hkRefPtr<hkbGenerator> m_transitionGenerator;
   hkReal m_blendInDuration;
   hkReal m_blendOutDuration;
   hkBool m_syncToGeneratorStartTime;
   hkRefVariant m_fromGenerator;
   hkRefVariant m_toGenerator;
   hkReal m_timeInTransition;
   hkReal m_duration;
   hkReal m_effectiveBlendInDuration;
   hkReal m_effectiveBlendOutDuration;
   /*enum*/ hkInt8 m_toGeneratorState;
   hkBool m_echoTransitionGenerator;
   hkBool m_echoToGenerator;
   hkBool m_justActivated;
   hkBool m_updateActiveNodes;
   /*enum*/ hkInt8 m_stage;
};
extern const hkClass hkbGeneratorTransitionEffectClass;


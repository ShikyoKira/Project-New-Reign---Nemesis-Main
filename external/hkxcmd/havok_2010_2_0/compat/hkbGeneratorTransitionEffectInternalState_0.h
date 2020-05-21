#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbGeneratorTransitionEffect_1.h"

class hkbGeneratorTransitionEffectInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGeneratorTransitionEffectInternalState(void) {}
   HK_FORCE_INLINE hkbGeneratorTransitionEffectInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkReal m_timeInTransition;
   hkReal m_duration;
   hkReal m_effectiveBlendInDuration;
   hkReal m_effectiveBlendOutDuration;
   hkEnum<hkbGeneratorTransitionEffect::ToGeneratorState,hkInt8> m_toGeneratorState;
   hkBool m_echoTransitionGenerator;
   hkBool m_echoToGenerator;
   hkBool m_justActivated;
   hkBool m_updateActiveNodes;
   hkEnum<hkbGeneratorTransitionEffect::Stage,hkInt8> m_stage;
};
extern const hkClass hkbGeneratorTransitionEffectInternalStateClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbTransitionEffect_0.h"
#include "hkbBlendCurveUtils_0.h"

class hkbBlendingTransitionEffect : public hkbTransitionEffect
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBlendingTransitionEffect(void) {}
   HK_FORCE_INLINE hkbBlendingTransitionEffect( hkFinishLoadedObjectFlag flag ) : hkbTransitionEffect(flag) 
   , m_fromGenerator(flag)
   , m_toGenerator(flag)
   , m_characterPoseAtBeginningOfTransition(flag)
      {}
   enum FlagBits
   {
      FLAG_NONE = 0,
      FLAG_IGNORE_FROM_WORLD_FROM_MODEL = 1,
      FLAG_SYNC = 2,
      FLAG_IGNORE_TO_WORLD_FROM_MODEL = 4,
   };

   enum EndMode
   {
      END_MODE_NONE = 0,
      END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR = 1,
      END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR = 2,
   };

   // Properties
   hkReal m_duration;
   hkReal m_toGeneratorStartTimeFraction;
   hkFlags<FlagBits,hkUint16> m_flags;
   hkEnum<EndMode,hkInt8> m_endMode;
   hkEnum<hkbBlendCurveUtils::BlendCurve,hkInt8> m_blendCurve;
   hkRefVariant m_fromGenerator;
   hkRefVariant m_toGenerator;
   hkArray<hkRefVariant> m_characterPoseAtBeginningOfTransition;
   hkReal m_timeRemaining;
   hkReal m_timeInTransition;
   hkBool m_applySelfTransition;
   hkBool m_initializeCharacterPose;
};
extern const hkClass hkbBlendingTransitionEffectClass;


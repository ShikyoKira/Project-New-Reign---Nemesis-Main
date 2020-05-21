#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "hkbClipTriggerArray_0.h"

class hkbClipGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbClipGenerator(void) {}
   HK_FORCE_INLINE hkbClipGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_animationName(flag)
   , m_triggers(flag)
   , m_animDatas(flag)
   , m_animationControl(flag)
   , m_originalTriggers(flag)
   , m_mapperData(flag)
   , m_binding(flag)
   , m_mirroredAnimation(flag)
   , m_echos(flag)
      {}
   enum PlaybackMode
   {
      MODE_SINGLE_PLAY = 0,
      MODE_LOOPING = 1,
      MODE_USER_CONTROLLED = 2,
      MODE_PING_PONG = 3,
      MODE_COUNT = 4,
   };

   enum ClipFlags
   {
      FLAG_CONTINUE_MOTION_AT_END = 1,
      FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE = 2,
      FLAG_MIRROR = 4,
      FLAG_FORCE_DENSE_POSE = 8,
      FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS = 16,
      FLAG_IGNORE_MOTION = 32,
   };

   // Properties
   hkStringPtr m_animationName;
   hkRefPtr<hkbClipTriggerArray> m_triggers;
   hkReal m_cropStartAmountLocalTime;
   hkReal m_cropEndAmountLocalTime;
   hkReal m_startTime;
   hkReal m_playbackSpeed;
   hkReal m_enforcedDuration;
   hkReal m_userControlledTimeFraction;
   hkInt16 m_animationBindingIndex;
   hkEnum<PlaybackMode,hkInt8> m_mode;
   hkInt8 m_flags;
   hkArray<hkRefVariant> m_animDatas;
   hkRefVariant m_animationControl;
   hkRefVariant m_originalTriggers;
   hkRefVariant m_mapperData;
   hkRefVariant m_binding;
   hkRefVariant m_mirroredAnimation;
   hkQsTransform m_extractedMotion;
   hkArray<hkRefVariant> m_echos;
   hkReal m_localTime;
   hkReal m_time;
   hkReal m_previousUserControlledTimeFraction;
   hkInt32 m_bufferSize;
   hkInt32 m_echoBufferSize;
   hkBool m_atEnd;
   hkBool m_ignoreStartTime;
   hkBool m_pingPongBackward;
};
extern const hkClass hkbClipGeneratorClass;


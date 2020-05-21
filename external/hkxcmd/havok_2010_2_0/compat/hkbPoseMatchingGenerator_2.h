#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBlenderGenerator_1.h"

class hkbPoseMatchingGenerator : public hkbBlenderGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbPoseMatchingGenerator(void) {}
   HK_FORCE_INLINE hkbPoseMatchingGenerator( hkFinishLoadedObjectFlag flag ) : hkbBlenderGenerator(flag) 
   , m_poseMatchingUtility(flag)
      {}
   enum Mode
   {
      MODE_MATCH = 0,
      MODE_PLAY = 1,
   };

   // Properties
   hkQuaternion m_worldFromModelRotation;
   hkReal m_blendSpeed;
   hkReal m_minSpeedToSwitch;
   hkReal m_minSwitchTimeNoError;
   hkReal m_minSwitchTimeFullError;
   hkInt32 m_startPlayingEventId;
   hkInt32 m_startMatchingEventId;
   hkInt16 m_rootBoneIndex;
   hkInt16 m_otherBoneIndex;
   hkInt16 m_anotherBoneIndex;
   hkInt16 m_pelvisIndex;
   hkEnum<Mode,hkInt8> m_mode;
   hkInt32 m_currentMatch;
   hkInt32 m_bestMatch;
   hkReal m_timeSinceBetterMatch;
   hkReal m_error;
   hkBool m_resetCurrentMatchLocalTime;
   hkRefVariant m_poseMatchingUtility;
};
extern const hkClass hkbPoseMatchingGeneratorClass;


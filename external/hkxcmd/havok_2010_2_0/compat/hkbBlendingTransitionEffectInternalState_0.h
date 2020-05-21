#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbBlendingTransitionEffectInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBlendingTransitionEffectInternalState(void) {}
   HK_FORCE_INLINE hkbBlendingTransitionEffectInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_characterPoseAtBeginningOfTransition(flag)
      {}
   // Properties
   hkArray<hkQsTransform> m_characterPoseAtBeginningOfTransition;
   hkReal m_timeRemaining;
   hkReal m_timeInTransition;
   hkBool m_applySelfTransition;
   hkBool m_initializeCharacterPose;
};
extern const hkClass hkbBlendingTransitionEffectInternalStateClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbPoseMatchingGeneratorInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbPoseMatchingGeneratorInternalState(void) {}
   HK_FORCE_INLINE hkbPoseMatchingGeneratorInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkInt32 m_currentMatch;
   hkInt32 m_bestMatch;
   hkReal m_timeSinceBetterMatch;
   hkReal m_error;
   hkBool m_resetCurrentMatchLocalTime;
};
extern const hkClass hkbPoseMatchingGeneratorInternalStateClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbClipGeneratorEcho_0.h"

class hkbClipGeneratorInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbClipGeneratorInternalState(void) {}
   HK_FORCE_INLINE hkbClipGeneratorInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_echos(flag)
      {}
   // Properties
   hkQsTransform m_extractedMotion;
   hkArray<hkbClipGeneratorEcho> m_echos;
   hkReal m_localTime;
   hkReal m_time;
   hkReal m_previousUserControlledTimeFraction;
   hkInt32 m_bufferSize;
   hkInt32 m_echoBufferSize;
   hkBool m_atEnd;
   hkBool m_ignoreStartTime;
   hkBool m_pingPongBackward;
};
extern const hkClass hkbClipGeneratorInternalStateClass;


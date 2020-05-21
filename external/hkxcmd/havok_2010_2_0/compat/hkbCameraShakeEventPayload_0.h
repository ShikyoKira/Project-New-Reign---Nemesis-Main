#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbCameraShakeEventPayload : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCameraShakeEventPayload(void) {}
   HK_FORCE_INLINE hkbCameraShakeEventPayload( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
      {}
   // Properties
   hkReal m_amplitude;
   hkReal m_halfLife;
};
extern const hkClass hkbCameraShakeEventPayloadClass;


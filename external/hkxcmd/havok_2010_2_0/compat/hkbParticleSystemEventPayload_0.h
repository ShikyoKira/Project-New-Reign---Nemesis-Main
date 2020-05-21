#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventPayload_0.h"

class hkbParticleSystemEventPayload : public hkbEventPayload
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbParticleSystemEventPayload(void) {}
   HK_FORCE_INLINE hkbParticleSystemEventPayload( hkFinishLoadedObjectFlag flag ) : hkbEventPayload(flag) 
      {}
   enum SystemType
   {
      DEBRIS = 0,
      DUST = 1,
      EXPLOSION = 2,
      SMOKE = 3,
      SPARKS = 4,
   };

   // Properties
   hkEnum<SystemType,hkUint8> m_type;
   hkInt16 m_emitBoneIndex;
   hkVector4 m_offset;
   hkVector4 m_direction;
   hkInt32 m_numParticles;
   hkReal m_speed;
};
extern const hkClass hkbParticleSystemEventPayloadClass;


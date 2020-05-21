#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"
#include "hkbGenerator_0.h"

class hkbRegisteredGenerator : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRegisteredGenerator(void) {}
   HK_FORCE_INLINE hkbRegisteredGenerator( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_generator(flag)
      {}
   // Properties
   hkRefPtr<hkbGenerator> m_generator;
   hkVector4 m_relativePosition;
   hkVector4 m_relativeDirection;
};
extern const hkClass hkbRegisteredGeneratorClass;


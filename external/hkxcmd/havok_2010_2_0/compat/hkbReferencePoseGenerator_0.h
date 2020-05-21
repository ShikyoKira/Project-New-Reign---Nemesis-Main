#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class hkbReferencePoseGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbReferencePoseGenerator(void) {}
   HK_FORCE_INLINE hkbReferencePoseGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_skeleton(flag)
      {}
   // Properties
   hkRefVariant m_skeleton;
};
extern const hkClass hkbReferencePoseGeneratorClass;


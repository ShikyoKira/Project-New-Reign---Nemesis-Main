#pragma once

#include <Common/Base/hkBase.h>
#include "hkbNode_1.h"

class hkbGenerator : public hkbNode
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGenerator(void) {}
   HK_FORCE_INLINE hkbGenerator( hkFinishLoadedObjectFlag flag ) : hkbNode(flag) 
      {}
   // Properties
};
extern const hkClass hkbGeneratorClass;


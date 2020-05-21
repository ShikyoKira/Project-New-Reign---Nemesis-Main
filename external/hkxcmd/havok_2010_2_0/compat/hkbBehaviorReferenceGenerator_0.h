#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class hkbBehaviorReferenceGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorReferenceGenerator(void) {}
   HK_FORCE_INLINE hkbBehaviorReferenceGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_behaviorName(flag)
   , m_behavior(flag)
      {}
   // Properties
   hkStringPtr m_behaviorName;
   hkRefVariant m_behavior;
};
extern const hkClass hkbBehaviorReferenceGeneratorClass;


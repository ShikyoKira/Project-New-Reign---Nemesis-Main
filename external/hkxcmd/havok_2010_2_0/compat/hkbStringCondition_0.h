#pragma once

#include <Common/Base/hkBase.h>
#include "hkbCondition_0.h"

class hkbStringCondition : public hkbCondition
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStringCondition(void) {}
   HK_FORCE_INLINE hkbStringCondition( hkFinishLoadedObjectFlag flag ) : hkbCondition(flag) 
   , m_conditionString(flag)
      {}
   // Properties
   hkStringPtr m_conditionString;
};
extern const hkClass hkbStringConditionClass;


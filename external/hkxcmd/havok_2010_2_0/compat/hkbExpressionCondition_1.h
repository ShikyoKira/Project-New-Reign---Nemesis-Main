#pragma once

#include <Common/Base/hkBase.h>
#include "hkbCondition_0.h"

class hkbExpressionCondition : public hkbCondition
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbExpressionCondition(void) {}
   HK_FORCE_INLINE hkbExpressionCondition( hkFinishLoadedObjectFlag flag ) : hkbCondition(flag) 
   , m_expression(flag)
   , m_compiledExpressionSet(flag)
      {}
   // Properties
   hkStringPtr m_expression;
   hkRefVariant m_compiledExpressionSet;
};
extern const hkClass hkbExpressionConditionClass;


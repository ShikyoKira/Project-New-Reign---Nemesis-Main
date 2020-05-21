#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbExpressionDataArray_0.h"

class hkbEvaluateExpressionModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEvaluateExpressionModifier(void) {}
   HK_FORCE_INLINE hkbEvaluateExpressionModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_expressions(flag)
   , m_compiledExpressionSet(flag)
   , m_internalExpressionsData(flag)
      {}
   // Properties
   hkRefPtr<hkbExpressionDataArray> m_expressions;
   hkRefVariant m_compiledExpressionSet;
   hkArray<hkRefVariant> m_internalExpressionsData;
};
extern const hkClass hkbEvaluateExpressionModifierClass;


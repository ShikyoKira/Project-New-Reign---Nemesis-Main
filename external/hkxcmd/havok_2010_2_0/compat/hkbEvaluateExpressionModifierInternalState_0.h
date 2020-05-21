#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbEvaluateExpressionModifierInternalExpressionData_0.h"

class hkbEvaluateExpressionModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEvaluateExpressionModifierInternalState(void) {}
   HK_FORCE_INLINE hkbEvaluateExpressionModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_internalExpressionsData(flag)
      {}
   // Properties
   hkArray<hkbEvaluateExpressionModifierInternalExpressionData> m_internalExpressionsData;
};
extern const hkClass hkbEvaluateExpressionModifierInternalStateClass;


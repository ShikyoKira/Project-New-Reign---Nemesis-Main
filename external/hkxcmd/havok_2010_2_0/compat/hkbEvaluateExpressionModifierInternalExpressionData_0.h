#pragma once

#include <Common/Base/hkBase.h>

struct hkbEvaluateExpressionModifierInternalExpressionData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEvaluateExpressionModifierInternalExpressionData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEvaluateExpressionModifierInternalExpressionData(void) {}
   // Properties
   hkBool m_raisedEvent;
   hkBool m_wasTrueInPreviousFrame;
};
extern const hkClass hkbEvaluateExpressionModifierInternalExpressionDataClass;


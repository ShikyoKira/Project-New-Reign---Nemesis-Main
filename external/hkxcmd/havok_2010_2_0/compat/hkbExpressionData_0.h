#pragma once

#include <Common/Base/hkBase.h>

struct hkbExpressionData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbExpressionData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbExpressionData(void) {}
   enum ExpressionEventMode
   {
      EVENT_MODE_SEND_ONCE = 0,
      EVENT_MODE_SEND_ON_TRUE = 1,
      EVENT_MODE_SEND_ON_FALSE_TO_TRUE = 2,
      EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE = 3,
   };

   // Properties
   hkStringPtr m_expression;
   hkInt32 m_assignmentVariableIndex;
   hkInt32 m_assignmentEventIndex;
   hkEnum<ExpressionEventMode,hkInt8> m_eventMode;
   hkBool m_raisedEvent;
   hkBool m_wasTrueInPreviousFrame;
};
extern const hkClass hkbExpressionDataClass;


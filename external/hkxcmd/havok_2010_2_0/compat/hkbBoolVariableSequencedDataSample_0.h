#pragma once

#include <Common/Base/hkBase.h>

struct hkbBoolVariableSequencedDataSample
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbBoolVariableSequencedDataSample );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbBoolVariableSequencedDataSample(void) {}
   // Properties
   hkReal m_time;
   hkBool m_value;
};
extern const hkClass hkbBoolVariableSequencedDataSampleClass;


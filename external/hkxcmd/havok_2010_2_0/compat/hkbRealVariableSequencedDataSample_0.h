#pragma once

#include <Common/Base/hkBase.h>

struct hkbRealVariableSequencedDataSample
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbRealVariableSequencedDataSample );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbRealVariableSequencedDataSample(void) {}
   // Properties
   hkReal m_time;
   hkReal m_value;
};
extern const hkClass hkbRealVariableSequencedDataSampleClass;


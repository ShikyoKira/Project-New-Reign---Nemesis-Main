#pragma once

#include <Common/Base/hkBase.h>

struct hkbIntVariableSequencedDataSample
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbIntVariableSequencedDataSample );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbIntVariableSequencedDataSample(void) {}
   // Properties
   hkReal m_time;
   hkInt32 m_value;
};
extern const hkClass hkbIntVariableSequencedDataSampleClass;


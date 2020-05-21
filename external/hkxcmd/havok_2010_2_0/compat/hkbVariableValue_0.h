#pragma once

#include <Common/Base/hkBase.h>

struct hkbVariableValue
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbVariableValue );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbVariableValue(void) {}
   // Properties
   hkInt32 m_value;
};
extern const hkClass hkbVariableValueClass;


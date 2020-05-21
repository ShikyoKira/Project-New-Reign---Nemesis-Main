#pragma once

#include <Common/Base/hkBase.h>
#include "hkbRoleAttribute_0.h"

struct hkbVariableInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbVariableInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbVariableInfo(void) {}
   enum VariableType
   {
      VARIABLE_TYPE_INVALID = -1,
      VARIABLE_TYPE_BOOL = 0,
      VARIABLE_TYPE_INT8 = 1,
      VARIABLE_TYPE_INT16 = 2,
      VARIABLE_TYPE_INT32 = 3,
      VARIABLE_TYPE_REAL = 4,
      VARIABLE_TYPE_POINTER = 5,
      VARIABLE_TYPE_VECTOR3 = 6,
      VARIABLE_TYPE_VECTOR4 = 7,
      VARIABLE_TYPE_QUATERNION = 8,
   };

   // Properties
   hkbRoleAttribute m_role;
   hkEnum<VariableType,hkInt8> m_type;
};
extern const hkClass hkbVariableInfoClass;


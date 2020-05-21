#pragma once

#include <Common/Base/hkBase.h>

struct hkbVariableBindingSetBinding
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbVariableBindingSetBinding );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbVariableBindingSetBinding(void) {}
   enum BindingType
   {
      BINDING_TYPE_VARIABLE = 0,
      BINDING_TYPE_CHARACTER_PROPERTY = 1,
   };

   enum InternalBindingFlags
   {
      FLAG_NONE = 0,
      FLAG_OUTPUT = 1,
   };

   // Properties
   hkStringPtr m_memberPath;
   hkRefVariant m_memberClass;
   hkInt32 m_offsetInObjectPlusOne;
   hkInt32 m_offsetInArrayPlusOne;
   hkInt32 m_rootVariableIndex;
   hkInt32 m_variableIndex;
   hkInt8 m_bitIndex;
   hkEnum<BindingType,hkInt8> m_bindingType;
   /*enum*/ hkUint8 m_memberType;
   hkInt8 m_variableType;
   /*flags*/ hkInt8 m_flags;
};
extern const hkClass hkbVariableBindingSetBindingClass;


#pragma once

#include <Common/Base/hkBase.h>

struct hkbAttributeModifierAssignment
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbAttributeModifierAssignment );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbAttributeModifierAssignment(void) {}
   // Properties
   hkInt32 m_attributeIndex;
   hkReal m_attributeValue;
};
extern const hkClass hkbAttributeModifierAssignmentClass;


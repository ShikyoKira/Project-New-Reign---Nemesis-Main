#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbAttributeModifierAssignment_0.h"

class hkbAttributeModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbAttributeModifier(void) {}
   HK_FORCE_INLINE hkbAttributeModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_assignments(flag)
      {}
   // Properties
   hkArray<hkbAttributeModifierAssignment> m_assignments;
};
extern const hkClass hkbAttributeModifierClass;


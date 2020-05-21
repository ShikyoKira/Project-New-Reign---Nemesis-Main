#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbModifierList : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbModifierList(void) {}
   HK_FORCE_INLINE hkbModifierList( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_modifiers(flag)
      {}
   // Properties
   hkArray<hkbModifier*> m_modifiers;
};
extern const hkClass hkbModifierListClass;


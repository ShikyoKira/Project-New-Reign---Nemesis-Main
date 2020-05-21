#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbModifierWrapper : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbModifierWrapper(void) {}
   HK_FORCE_INLINE hkbModifierWrapper( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_modifier(flag)
      {}
   // Properties
   hkRefPtr<hkbModifier> m_modifier;
};
extern const hkClass hkbModifierWrapperClass;


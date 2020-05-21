#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbMirrorModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbMirrorModifier(void) {}
   HK_FORCE_INLINE hkbMirrorModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkBool m_isAdditive;
};
extern const hkClass hkbMirrorModifierClass;


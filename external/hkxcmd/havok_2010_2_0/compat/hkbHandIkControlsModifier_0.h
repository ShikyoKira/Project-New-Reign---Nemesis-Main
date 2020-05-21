#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbHandIkControlsModifierHand_0.h"

class hkbHandIkControlsModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbHandIkControlsModifier(void) {}
   HK_FORCE_INLINE hkbHandIkControlsModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_hands(flag)
      {}
   // Properties
   hkArray<hkbHandIkControlsModifierHand> m_hands;
};
extern const hkClass hkbHandIkControlsModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbNode_1.h"

class hkbModifier : public hkbNode
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbModifier(void) {}
   HK_FORCE_INLINE hkbModifier( hkFinishLoadedObjectFlag flag ) : hkbNode(flag) 
      {}
   // Properties
   hkBool m_enable;
   hkBool m_padModifier[3];
};
extern const hkClass hkbModifierClass;


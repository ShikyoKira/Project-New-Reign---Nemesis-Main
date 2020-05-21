#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbMoveCharacterModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbMoveCharacterModifier(void) {}
   HK_FORCE_INLINE hkbMoveCharacterModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_offsetPerSecondMS;
   hkReal m_timeSinceLastModify;
};
extern const hkClass hkbMoveCharacterModifierClass;


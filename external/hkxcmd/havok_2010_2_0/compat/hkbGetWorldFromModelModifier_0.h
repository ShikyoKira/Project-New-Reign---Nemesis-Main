#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbGetWorldFromModelModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGetWorldFromModelModifier(void) {}
   HK_FORCE_INLINE hkbGetWorldFromModelModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_translationOut;
   hkQuaternion m_rotationOut;
};
extern const hkClass hkbGetWorldFromModelModifierClass;


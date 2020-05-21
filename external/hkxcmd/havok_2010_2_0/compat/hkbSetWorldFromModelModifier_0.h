#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbSetWorldFromModelModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSetWorldFromModelModifier(void) {}
   HK_FORCE_INLINE hkbSetWorldFromModelModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_translation;
   hkQuaternion m_rotation;
   hkBool m_setTranslation;
   hkBool m_setRotation;
};
extern const hkClass hkbSetWorldFromModelModifierClass;


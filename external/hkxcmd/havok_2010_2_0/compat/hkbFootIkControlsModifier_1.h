#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbFootIkControlData_0.h"
#include "hkbFootIkControlsModifierLeg_0.h"

class hkbFootIkControlsModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbFootIkControlsModifier(void) {}
   HK_FORCE_INLINE hkbFootIkControlsModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_legs(flag)
      {}
   // Properties
   hkChar m_padding[4];
   hkbFootIkControlData m_controlData;
   hkArray<hkbFootIkControlsModifierLeg> m_legs;
   hkVector4 m_errorOutTranslation;
   hkQuaternion m_alignWithGroundRotation;
};
extern const hkClass hkbFootIkControlsModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbHandIkModifierHand_3.h"
#include "hkbBlendCurveUtils_0.h"

class hkbHandIkModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbHandIkModifier(void) {}
   HK_FORCE_INLINE hkbHandIkModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_hands(flag)
   , m_internalHandData(flag)
      {}
   // Properties
   hkArray<hkbHandIkModifierHand> m_hands;
   hkEnum<hkbBlendCurveUtils::BlendCurve,hkInt8> m_fadeInOutCurve;
   hkArray<hkRefVariant> m_internalHandData;
};
extern const hkClass hkbHandIkModifierClass;


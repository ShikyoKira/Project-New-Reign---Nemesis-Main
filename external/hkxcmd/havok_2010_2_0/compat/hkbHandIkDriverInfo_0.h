#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbHandIkDriverInfoHand_1.h"
#include "hkbBlendCurveUtils_0.h"

class hkbHandIkDriverInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbHandIkDriverInfo(void) {}
   HK_FORCE_INLINE hkbHandIkDriverInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_hands(flag)
      {}
      // Properties
   hkArray<hkbHandIkDriverInfoHand> m_hands;
   hkEnum<hkbBlendCurveUtils::BlendCurve,hkInt8> m_fadeInOutCurve;
};
extern const hkClass hkbHandIkDriverInfoClass;


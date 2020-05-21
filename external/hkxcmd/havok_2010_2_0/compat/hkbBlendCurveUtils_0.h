#pragma once

#include <Common/Base/hkBase.h>

struct hkbBlendCurveUtils
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbBlendCurveUtils );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbBlendCurveUtils(void) {}
   enum BlendCurve
   {
      BLEND_CURVE_SMOOTH = 0,
      BLEND_CURVE_LINEAR = 1,
      BLEND_CURVE_LINEAR_TO_SMOOTH = 2,
      BLEND_CURVE_SMOOTH_TO_LINEAR = 3,
   };

   // Properties
};
extern const hkClass hkbBlendCurveUtilsClass;


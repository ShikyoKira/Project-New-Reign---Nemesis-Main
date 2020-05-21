#pragma once

#include <Common/Base/hkBase.h>
#include "hkbFootIkGains_0.h"

struct hkbFootIkControlData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbFootIkControlData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbFootIkControlData(void) {}
   // Properties
   hkbFootIkGains m_gains;
};
extern const hkClass hkbFootIkControlDataClass;


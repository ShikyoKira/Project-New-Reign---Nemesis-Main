#pragma once

#include <Common/Base/hkBase.h>
#include "hkbEventBase_0.h"

struct hkbEventProperty : public hkbEventBase
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbEventProperty );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbEventProperty(void) {}
   // Properties
};
extern const hkClass hkbEventPropertyClass;


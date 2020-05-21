#pragma once

#include <Common/Base/hkBase.h>

struct hkbFootIkModifierInternalLegData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbFootIkModifierInternalLegData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbFootIkModifierInternalLegData(void) {}
   // Properties
   hkVector4 m_groundPosition;
   hkRefVariant m_footIkSolver;
};
extern const hkClass hkbFootIkModifierInternalLegDataClass;


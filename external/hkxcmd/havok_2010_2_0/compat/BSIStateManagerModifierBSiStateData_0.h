#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

struct BSIStateManagerModifierBSiStateData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, BSIStateManagerModifierBSiStateData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE BSIStateManagerModifierBSiStateData(void) {}
   // Properties
   hkRefPtr<hkbGenerator> m_pStateMachine;
   hkInt32 m_StateID;
   hkInt32 m_iStateToSetAs;
};
extern const hkClass BSIStateManagerModifierBSiStateDataClass;


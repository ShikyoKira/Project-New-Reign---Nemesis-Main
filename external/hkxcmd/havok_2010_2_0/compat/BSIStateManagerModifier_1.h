#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "BSIStateManagerModifierBSiStateData_0.h"
#include "BSIStateManagerModifierBSIStateManagerStateListener_0.h"

class BSIStateManagerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSIStateManagerModifier(void) {}
   HK_FORCE_INLINE BSIStateManagerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_stateData(flag)
      {}
   // Properties
   hkInt32 m_iStateVar;
   hkArray<BSIStateManagerModifierBSiStateData> m_stateData;
   BSIStateManagerModifierBSIStateManagerStateListener m_myStateListener;
};
extern const hkClass BSIStateManagerModifierClass;


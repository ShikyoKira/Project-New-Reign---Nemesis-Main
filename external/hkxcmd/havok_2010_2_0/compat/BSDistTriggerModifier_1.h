#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class BSDistTriggerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSDistTriggerModifier(void) {}
   HK_FORCE_INLINE BSDistTriggerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_targetPosition;
   hkReal m_distance;
   hkReal m_distanceTrigger;
   hkbEventProperty m_triggerEvent;
};
extern const hkClass BSDistTriggerModifierClass;


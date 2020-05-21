#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class BSPassByTargetTriggerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSPassByTargetTriggerModifier(void) {}
   HK_FORCE_INLINE BSPassByTargetTriggerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_targetPosition;
   hkReal m_radius;
   hkVector4 m_movementDirection;
   hkbEventProperty m_triggerEvent;
   hkBool m_targetPassed;
};
extern const hkClass BSPassByTargetTriggerModifierClass;


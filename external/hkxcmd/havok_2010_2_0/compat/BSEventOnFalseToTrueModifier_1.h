#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class BSEventOnFalseToTrueModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSEventOnFalseToTrueModifier(void) {}
   HK_FORCE_INLINE BSEventOnFalseToTrueModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkBool m_bEnableEvent1;
   hkBool m_bVariableToTest1;
   hkbEventProperty m_EventToSend1;
   hkBool m_bEnableEvent2;
   hkBool m_bVariableToTest2;
   hkbEventProperty m_EventToSend2;
   hkBool m_bEnableEvent3;
   hkBool m_bVariableToTest3;
   hkbEventProperty m_EventToSend3;
   hkBool m_bSlot1ActivatedLastFrame;
   hkBool m_bSlot2ActivatedLastFrame;
   hkBool m_bSlot3ActivatedLastFrame;
};
extern const hkClass BSEventOnFalseToTrueModifierClass;


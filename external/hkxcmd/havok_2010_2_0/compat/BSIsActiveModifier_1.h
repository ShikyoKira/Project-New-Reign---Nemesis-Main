#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSIsActiveModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSIsActiveModifier(void) {}
   HK_FORCE_INLINE BSIsActiveModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkBool m_bIsActive0;
   hkBool m_bInvertActive0;
   hkBool m_bIsActive1;
   hkBool m_bInvertActive1;
   hkBool m_bIsActive2;
   hkBool m_bInvertActive2;
   hkBool m_bIsActive3;
   hkBool m_bInvertActive3;
   hkBool m_bIsActive4;
   hkBool m_bInvertActive4;
};
extern const hkClass BSIsActiveModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSModifyOnceModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSModifyOnceModifier(void) {}
   HK_FORCE_INLINE BSModifyOnceModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_pOnActivateModifier(flag)
   , m_pOnDeactivateModifier(flag)
      {}
   // Properties
   hkChar m_padding_1[4];
   hkRefPtr<hkbModifier> m_pOnActivateModifier;
   hkChar m_padding_2[12];
   hkRefPtr<hkbModifier> m_pOnDeactivateModifier;
   hkChar m_padding_3[12];
};
extern const hkClass BSModifyOnceModifierClass;


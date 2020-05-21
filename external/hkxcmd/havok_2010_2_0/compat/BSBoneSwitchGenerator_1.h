#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "BSBoneSwitchGeneratorBoneData_1.h"

class BSBoneSwitchGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSBoneSwitchGenerator(void) {}
   HK_FORCE_INLINE BSBoneSwitchGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_pDefaultGenerator(flag)
   , m_ChildrenA(flag)
      {}
   // Properties
   hkChar m_padding_1[8];
   hkRefPtr<hkbGenerator> m_pDefaultGenerator;
   hkArray<BSBoneSwitchGeneratorBoneData*> m_ChildrenA;
};
extern const hkClass BSBoneSwitchGeneratorClass;


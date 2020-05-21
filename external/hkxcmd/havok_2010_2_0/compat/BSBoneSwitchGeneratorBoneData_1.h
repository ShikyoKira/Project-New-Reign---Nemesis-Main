#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"
#include "hkbGenerator_0.h"
#include "hkbBoneWeightArray_0.h"

class BSBoneSwitchGeneratorBoneData : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSBoneSwitchGeneratorBoneData(void) {}
   HK_FORCE_INLINE BSBoneSwitchGeneratorBoneData( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_pGenerator(flag)
   , m_spBoneWeight(flag)
      {}
   // Properties
   hkChar m_padding_1[4];
   hkRefPtr<hkbGenerator> m_pGenerator;
   hkRefPtr<hkbBoneWeightArray> m_spBoneWeight;
   hkChar m_padding_2[8];
};
extern const hkClass BSBoneSwitchGeneratorBoneDataClass;


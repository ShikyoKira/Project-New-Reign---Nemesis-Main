#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"
#include "hkbGenerator_0.h"
#include "hkbBoneWeightArray_0.h"

class hkbBlenderGeneratorChild : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBlenderGeneratorChild(void) {}
   HK_FORCE_INLINE hkbBlenderGeneratorChild( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_generator(flag)
   , m_boneWeights(flag)
      {}
   // Properties
   hkChar m_padding[4];
   hkRefPtr<hkbGenerator> m_generator;
   hkRefPtr<hkbBoneWeightArray> m_boneWeights;
   hkReal m_weight;
   hkReal m_worldFromModelWeight;
};
extern const hkClass hkbBlenderGeneratorChildClass;


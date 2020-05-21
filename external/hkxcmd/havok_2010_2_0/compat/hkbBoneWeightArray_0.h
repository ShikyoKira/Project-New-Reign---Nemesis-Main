#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"

class hkbBoneWeightArray : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBoneWeightArray(void) {}
   HK_FORCE_INLINE hkbBoneWeightArray( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_boneWeights(flag)
      {}
   // Properties
   hkArray<hkReal> m_boneWeights;
};
extern const hkClass hkbBoneWeightArrayClass;


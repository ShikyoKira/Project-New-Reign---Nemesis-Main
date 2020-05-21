#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"

class hkbBoneIndexArray : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBoneIndexArray(void) {}
   HK_FORCE_INLINE hkbBoneIndexArray( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_boneIndices(flag)
      {}
   // Properties
   hkArray<hkInt16> m_boneIndices;
};
extern const hkClass hkbBoneIndexArrayClass;


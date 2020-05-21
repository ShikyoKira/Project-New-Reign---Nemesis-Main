#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSDecomposeVectorModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSDecomposeVectorModifier(void) {}
   HK_FORCE_INLINE BSDecomposeVectorModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_vector;
   hkReal m_x;
   hkReal m_y;
   hkReal m_z;
   hkReal m_w;
};
extern const hkClass BSDecomposeVectorModifierClass;


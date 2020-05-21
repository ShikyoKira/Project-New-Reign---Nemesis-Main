#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbComputeRotationFromAxisAngleModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbComputeRotationFromAxisAngleModifier(void) {}
   HK_FORCE_INLINE hkbComputeRotationFromAxisAngleModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkQuaternion m_rotationOut;
   hkVector4 m_axis;
   hkReal m_angleDegrees;
};
extern const hkClass hkbComputeRotationFromAxisAngleModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbComputeDirectionModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbComputeDirectionModifier(void) {}
   HK_FORCE_INLINE hkbComputeDirectionModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_pointIn;
   hkVector4 m_pointOut;
   hkReal m_groundAngleOut;
   hkReal m_upAngleOut;
   hkReal m_verticalOffset;
   hkBool m_reverseGroundAngle;
   hkBool m_reverseUpAngle;
   hkBool m_projectPoint;
   hkBool m_normalizePoint;
   hkBool m_computeOnlyOnce;
   hkBool m_computedOutput;
};
extern const hkClass hkbComputeDirectionModifierClass;


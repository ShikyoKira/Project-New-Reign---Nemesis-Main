#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbDampingModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbDampingModifier(void) {}
   HK_FORCE_INLINE hkbDampingModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkReal m_kP;
   hkReal m_kI;
   hkReal m_kD;
   hkBool m_enableScalarDamping;
   hkBool m_enableVectorDamping;
   hkReal m_rawValue;
   hkReal m_dampedValue;
   hkVector4 m_rawVector;
   hkVector4 m_dampedVector;
   hkVector4 m_vecErrorSum;
   hkVector4 m_vecPreviousError;
   hkReal m_errorSum;
   hkReal m_previousError;
};
extern const hkClass hkbDampingModifierClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbDampingModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbDampingModifierInternalState(void) {}
   HK_FORCE_INLINE hkbDampingModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkVector4 m_dampedVector;
   hkVector4 m_vecErrorSum;
   hkVector4 m_vecPreviousError;
   hkReal m_dampedValue;
   hkReal m_errorSum;
   hkReal m_previousError;
};
extern const hkClass hkbDampingModifierInternalStateClass;


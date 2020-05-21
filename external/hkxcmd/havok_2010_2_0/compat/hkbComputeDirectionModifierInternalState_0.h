#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbComputeDirectionModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbComputeDirectionModifierInternalState(void) {}
   HK_FORCE_INLINE hkbComputeDirectionModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkVector4 m_pointOut;
   hkReal m_groundAngleOut;
   hkReal m_upAngleOut;
   hkBool m_computedOutput;
};
extern const hkClass hkbComputeDirectionModifierInternalStateClass;


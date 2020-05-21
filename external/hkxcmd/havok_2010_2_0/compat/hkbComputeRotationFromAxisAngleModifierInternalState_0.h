#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbComputeRotationFromAxisAngleModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbComputeRotationFromAxisAngleModifierInternalState(void) {}
   HK_FORCE_INLINE hkbComputeRotationFromAxisAngleModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkQuaternion m_rotationOut;
};
extern const hkClass hkbComputeRotationFromAxisAngleModifierInternalStateClass;


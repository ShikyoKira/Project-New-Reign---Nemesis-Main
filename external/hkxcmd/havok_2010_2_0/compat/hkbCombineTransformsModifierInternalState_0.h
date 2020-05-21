#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCombineTransformsModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCombineTransformsModifierInternalState(void) {}
   HK_FORCE_INLINE hkbCombineTransformsModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkVector4 m_translationOut;
   hkQuaternion m_rotationOut;
};
extern const hkClass hkbCombineTransformsModifierInternalStateClass;


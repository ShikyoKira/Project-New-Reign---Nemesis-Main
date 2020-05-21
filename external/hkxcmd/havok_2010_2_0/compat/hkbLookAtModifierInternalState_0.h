#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbLookAtModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbLookAtModifierInternalState(void) {}
   HK_FORCE_INLINE hkbLookAtModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkVector4 m_lookAtLastTargetWS;
   hkReal m_lookAtWeight;
   hkBool m_isTargetInsideLimitCone;
};
extern const hkClass hkbLookAtModifierInternalStateClass;


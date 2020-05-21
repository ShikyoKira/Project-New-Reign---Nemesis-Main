#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCharacterControllerModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterControllerModifierInternalState(void) {}
   HK_FORCE_INLINE hkbCharacterControllerModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkVector4 m_gravity;
   hkReal m_timestep;
   hkBool m_isInitialVelocityAdded;
   hkBool m_isTouchingGround;
};
extern const hkClass hkbCharacterControllerModifierInternalStateClass;


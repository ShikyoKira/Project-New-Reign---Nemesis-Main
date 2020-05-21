#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbRotateCharacterModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRotateCharacterModifierInternalState(void) {}
   HK_FORCE_INLINE hkbRotateCharacterModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkReal m_angle;
};
extern const hkClass hkbRotateCharacterModifierInternalStateClass;


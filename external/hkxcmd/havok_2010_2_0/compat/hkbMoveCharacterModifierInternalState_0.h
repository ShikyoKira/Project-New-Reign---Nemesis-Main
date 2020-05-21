#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbMoveCharacterModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbMoveCharacterModifierInternalState(void) {}
   HK_FORCE_INLINE hkbMoveCharacterModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkReal m_timeSinceLastModify;
};
extern const hkClass hkbMoveCharacterModifierInternalStateClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbGetUpModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGetUpModifierInternalState(void) {}
   HK_FORCE_INLINE hkbGetUpModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkReal m_timeSinceBegin;
   hkReal m_timeStep;
   hkBool m_initNextModify;
};
extern const hkClass hkbGetUpModifierInternalStateClass;


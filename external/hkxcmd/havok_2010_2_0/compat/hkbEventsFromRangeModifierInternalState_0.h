#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbEventsFromRangeModifierInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventsFromRangeModifierInternalState(void) {}
   HK_FORCE_INLINE hkbEventsFromRangeModifierInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_wasActiveInPreviousFrame(flag)
      {}
   // Properties
   hkArray<hkBool> m_wasActiveInPreviousFrame;
};
extern const hkClass hkbEventsFromRangeModifierInternalStateClass;


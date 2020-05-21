#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbSequenceInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSequenceInternalState(void) {}
   HK_FORCE_INLINE hkbSequenceInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_nextSampleEvents(flag)
   , m_nextSampleReals(flag)
   , m_nextSampleBools(flag)
   , m_nextSampleInts(flag)
      {}
   // Properties
   hkArray<hkInt32> m_nextSampleEvents;
   hkArray<hkInt32> m_nextSampleReals;
   hkArray<hkInt32> m_nextSampleBools;
   hkArray<hkInt32> m_nextSampleInts;
   hkReal m_time;
   hkBool m_isEnabled;
};
extern const hkClass hkbSequenceInternalStateClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbBlenderGeneratorChildInternalState_0.h"

class hkbBlenderGeneratorInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBlenderGeneratorInternalState(void) {}
   HK_FORCE_INLINE hkbBlenderGeneratorInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_childrenInternalStates(flag)
   , m_sortedChildren(flag)
      {}
   // Properties
   hkArray<hkbBlenderGeneratorChildInternalState> m_childrenInternalStates;
   hkArray<hkInt16> m_sortedChildren;
   hkReal m_endIntervalWeight;
   hkInt32 m_numActiveChildren;
   hkInt16 m_beginIntervalIndex;
   hkInt16 m_endIntervalIndex;
   hkBool m_initSync;
   hkBool m_doSubtractiveBlend;
};
extern const hkClass hkbBlenderGeneratorInternalStateClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbNodeInternalStateInfo_0.h"
#include "hkbVariableValueSet_0.h"

class hkbBehaviorGraphInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorGraphInternalState(void) {}
   HK_FORCE_INLINE hkbBehaviorGraphInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_nodeInternalStateInfos(flag)
   , m_variableValueSet(flag)
      {}
   // Properties
   hkArray<hkbNodeInternalStateInfo*> m_nodeInternalStateInfos;
   hkRefPtr<hkbVariableValueSet> m_variableValueSet;
};
extern const hkClass hkbBehaviorGraphInternalStateClass;


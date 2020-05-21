#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbBehaviorGraphInternalState_0.h"
#include "hkbAuxiliaryNodeInfo_1.h"

class hkbBehaviorGraphInternalStateInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorGraphInternalStateInfo(void) {}
   HK_FORCE_INLINE hkbBehaviorGraphInternalStateInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_internalState(flag)
   , m_auxiliaryNodeInfo(flag)
   , m_activeEventIds(flag)
   , m_activeVariableIds(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkRefPtr<hkbBehaviorGraphInternalState> m_internalState;
   hkArray<hkbAuxiliaryNodeInfo*> m_auxiliaryNodeInfo;
   hkArray<hkInt16> m_activeEventIds;
   hkArray<hkInt16> m_activeVariableIds;
};
extern const hkClass hkbBehaviorGraphInternalStateInfoClass;


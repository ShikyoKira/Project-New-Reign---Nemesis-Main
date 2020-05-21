#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbGeneratorSyncInfo_0.h"

class hkbNodeInternalStateInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbNodeInternalStateInfo(void) {}
   HK_FORCE_INLINE hkbNodeInternalStateInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_name(flag)
   , m_internalState(flag)
      {}
   // Properties
   hkbGeneratorSyncInfo m_syncInfo;
   hkStringPtr m_name;
   hkRefPtr<hkReferencedObject> m_internalState;
   hkInt16 m_nodeId;
   hkBool m_hasActivateBeenCalled;
};
extern const hkClass hkbNodeInternalStateInfoClass;


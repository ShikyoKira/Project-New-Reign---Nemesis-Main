#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbBehaviorGraphData_2.h"
#include "hkbBehaviorInfoIdToNamePair_0.h"

class hkbBehaviorInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorInfo(void) {}
   HK_FORCE_INLINE hkbBehaviorInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_data(flag)
   , m_idToNamePairs(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkRefPtr<hkbBehaviorGraphData> m_data;
   hkArray<hkbBehaviorInfoIdToNamePair> m_idToNamePairs;
};
extern const hkClass hkbBehaviorInfoClass;


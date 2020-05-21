#pragma once

#include <Common/Base/hkBase.h>
#include "hkbBindable_0.h"

class hkbNode : public hkbBindable
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbNode(void) {}
   HK_FORCE_INLINE hkbNode( hkFinishLoadedObjectFlag flag ) : hkbBindable(flag) 
   , m_name(flag)
      {}
   enum GetChildrenFlagBits
   {
      FLAG_ACTIVE_ONLY = 1,
      FLAG_GENERATORS_ONLY = 2,
      FLAG_IGNORE_REFERENCED_BEHAVIORS = 4,
   };

   enum CloneState
   {
      CLONE_STATE_DEFAULT = 0,
      CLONE_STATE_TEMPLATE = 1,
      CLONE_STATE_CLONE = 2,
      CLONE_STATE_SHARABLE = 3,
   };

   // Properties
   hkUlong m_userData;
   hkStringPtr m_name;
   hkInt16 m_id;
   /*enum*/ hkInt8 m_cloneState;
   hkBool m_padNode[1];
};
extern const hkClass hkbNodeClass;


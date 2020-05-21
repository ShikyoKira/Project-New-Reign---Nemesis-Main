#pragma once

#include <Common/Base/hkBase.h>
#include "hkbStateListener_0.h"

class BSIStateManagerModifierBSIStateManagerStateListener : public hkbStateListener
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSIStateManagerModifierBSIStateManagerStateListener(void) {}
   HK_FORCE_INLINE BSIStateManagerModifierBSIStateManagerStateListener( hkFinishLoadedObjectFlag flag ) : hkbStateListener(flag) 
   , m_pStateManager(flag)
      {}
   // Properties
   hkRefVariant m_pStateManager;
};
extern const hkClass BSIStateManagerModifierBSIStateManagerStateListenerClass;


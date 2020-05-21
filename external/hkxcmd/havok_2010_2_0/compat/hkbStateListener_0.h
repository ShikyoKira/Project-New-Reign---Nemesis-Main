#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbStateListener : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbStateListener(void) {}
   HK_FORCE_INLINE hkbStateListener( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
};
extern const hkClass hkbStateListenerClass;


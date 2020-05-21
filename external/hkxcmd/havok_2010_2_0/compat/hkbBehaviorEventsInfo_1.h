#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbBehaviorEventsInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorEventsInfo(void) {}
   HK_FORCE_INLINE hkbBehaviorEventsInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_externalEventIds(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkArray<hkInt16> m_externalEventIds;
   hkInt32 m_padding;
};
extern const hkClass hkbBehaviorEventsInfoClass;


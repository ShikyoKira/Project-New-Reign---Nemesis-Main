#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbEventRaisedInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEventRaisedInfo(void) {}
   HK_FORCE_INLINE hkbEventRaisedInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_eventName(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkStringPtr m_eventName;
   hkBool m_raisedBySdk;
   hkInt32 m_senderId;
   hkInt32 m_padding;
};
extern const hkClass hkbEventRaisedInfoClass;


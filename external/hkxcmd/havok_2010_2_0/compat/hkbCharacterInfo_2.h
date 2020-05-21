#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCharacterInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterInfo(void) {}
   HK_FORCE_INLINE hkbCharacterInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   enum Event
   {
      REMOVED_FROM_WORLD = 0,
      SHOWN = 1,
      HIDDEN = 2,
   };

   // Properties
   hkUint64 m_characterId;
   hkEnum<Event,hkUint8> m_event;
   hkInt32 m_padding;
};
extern const hkClass hkbCharacterInfoClass;


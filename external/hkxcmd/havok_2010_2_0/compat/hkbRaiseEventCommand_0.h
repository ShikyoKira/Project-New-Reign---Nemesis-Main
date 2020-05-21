#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbRaiseEventCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRaiseEventCommand(void) {}
   HK_FORCE_INLINE hkbRaiseEventCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkUint64 m_characterId;
   hkBool m_global;
   hkInt32 m_externalId;
};
extern const hkClass hkbRaiseEventCommandClass;


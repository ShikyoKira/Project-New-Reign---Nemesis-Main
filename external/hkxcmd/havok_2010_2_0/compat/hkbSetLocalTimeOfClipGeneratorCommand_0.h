#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbSetLocalTimeOfClipGeneratorCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSetLocalTimeOfClipGeneratorCommand(void) {}
   HK_FORCE_INLINE hkbSetLocalTimeOfClipGeneratorCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkUint64 m_characterId;
   hkReal m_localTime;
   hkInt16 m_nodeId;
};
extern const hkClass hkbSetLocalTimeOfClipGeneratorCommandClass;


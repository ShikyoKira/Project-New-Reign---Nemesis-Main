#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCharacterControlCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterControlCommand(void) {}
   HK_FORCE_INLINE hkbCharacterControlCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   enum CharacterControlCommand
   {
      COMMAND_HIDE = 0,
      COMMAND_SHOW = 1,
   };

   // Properties
   hkUint64 m_characterId;
   hkEnum<CharacterControlCommand,hkUint8> m_command;
   hkInt32 m_padding;
};
extern const hkClass hkbCharacterControlCommandClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbSimulationControlCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSimulationControlCommand(void) {}
   HK_FORCE_INLINE hkbSimulationControlCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   enum SimulationControlCommand
   {
      COMMAND_PLAY = 0,
      COMMAND_PAUSE = 1,
      COMMAND_STEP = 2,
      COMMAND_STOP = 3,
      COMMAND_ACCUMULATE_MOTION = 4,
      COMMAND_DO_NOT_ACCUMULATE_MOTION = 5,
   };

   // Properties
   hkEnum<SimulationControlCommand,hkUint8> m_command;
};
extern const hkClass hkbSimulationControlCommandClass;


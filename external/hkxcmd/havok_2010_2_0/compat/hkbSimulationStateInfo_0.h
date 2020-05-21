#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbWorldEnums_0.h"

class hkbSimulationStateInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSimulationStateInfo(void) {}
   HK_FORCE_INLINE hkbSimulationStateInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkEnum<hkbWorldEnums::SimulationState,hkUint8> m_simulationState;
};
extern const hkClass hkbSimulationStateInfoClass;


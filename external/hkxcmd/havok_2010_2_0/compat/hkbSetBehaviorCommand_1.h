#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbBehaviorGraph_1.h"
#include "hkbGenerator_0.h"

class hkbSetBehaviorCommand : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSetBehaviorCommand(void) {}
   HK_FORCE_INLINE hkbSetBehaviorCommand( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_behavior(flag)
   , m_rootGenerator(flag)
   , m_referencedBehaviors(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkRefPtr<hkbBehaviorGraph> m_behavior;
   hkRefPtr<hkbGenerator> m_rootGenerator;
   hkArray<hkbBehaviorGraph*> m_referencedBehaviors;
   hkInt32 m_startStateIndex;
   hkBool m_randomizeSimulation;
   hkInt32 m_padding;
};
extern const hkClass hkbSetBehaviorCommandClass;


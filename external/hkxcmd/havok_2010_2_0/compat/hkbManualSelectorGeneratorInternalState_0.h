#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbManualSelectorGeneratorInternalState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbManualSelectorGeneratorInternalState(void) {}
   HK_FORCE_INLINE hkbManualSelectorGeneratorInternalState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
      {}
   // Properties
   hkInt8 m_currentGeneratorIndex;
};
extern const hkClass hkbManualSelectorGeneratorInternalStateClass;


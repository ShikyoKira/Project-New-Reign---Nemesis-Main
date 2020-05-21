#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class hkbManualSelectorGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbManualSelectorGenerator(void) {}
   HK_FORCE_INLINE hkbManualSelectorGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_generators(flag)
      {}
   // Properties
   hkArray<hkbGenerator*> m_generators;
   hkInt8 m_selectedGeneratorIndex;
   hkInt8 m_currentGeneratorIndex;
};
extern const hkClass hkbManualSelectorGeneratorClass;


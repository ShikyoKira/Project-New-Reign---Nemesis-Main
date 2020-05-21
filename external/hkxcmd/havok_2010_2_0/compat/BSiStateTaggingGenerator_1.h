#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class BSiStateTaggingGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSiStateTaggingGenerator(void) {}
   HK_FORCE_INLINE BSiStateTaggingGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_pDefaultGenerator(flag)
      {}
   // Properties
   hkChar m_padding_1[8];
   hkRefPtr<hkbGenerator> m_pDefaultGenerator;
   hkInt32 m_iStateToSetAs;
   hkInt32 m_iPriority;
   hkChar m_padding_2[4];
};
extern const hkClass BSiStateTaggingGeneratorClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "hkbModifier_0.h"

class hkbModifierGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbModifierGenerator(void) {}
   HK_FORCE_INLINE hkbModifierGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_modifier(flag)
   , m_generator(flag)
      {}
   // Properties
   hkRefPtr<hkbModifier> m_modifier;
   hkRefPtr<hkbGenerator> m_generator;
};
extern const hkClass hkbModifierGeneratorClass;


#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class BGSGamebryoSequenceGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BGSGamebryoSequenceGenerator(void) {}
   HK_FORCE_INLINE BGSGamebryoSequenceGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_events(flag)
      {}
   enum BlendModeFunction
   {
      BMF_NONE = 0,
      BMF_PERCENT = 1,
      BMF_ONE_MINUS_PERCENT = 2,
   };

   // Properties
   char* m_pSequence;
   hkEnum<BlendModeFunction,hkInt8> m_eBlendModeFunction;
   hkReal m_fPercent;
   hkArray<hkRefVariant> m_events;
   hkReal m_fTime;
   hkBool m_bDelayedActivate;
   hkBool m_bLooping;
};
extern const hkClass BGSGamebryoSequenceGeneratorClass;


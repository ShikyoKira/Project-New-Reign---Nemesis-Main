#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "hkbEventProperty_1.h"
#include "hkbBlendCurveUtils_0.h"

class BSCyclicBlendTransitionGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSCyclicBlendTransitionGenerator(void) {}
   HK_FORCE_INLINE BSCyclicBlendTransitionGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_pBlenderGenerator(flag)
   , m_pTransitionBlenderGenerator(flag)
   , m_pTransitionEffect(flag)
      {}
   enum CurrentBlendMode
   {
      MODE_INACTIVE = -1,
      MODE_DEFAULT = 0,
      MODE_FROZEN = 1,
      MODE_BLENDING = 2,
      MODE_WAITINGFORBLENDING = 3,
   };

   // Properties
   hkChar m_padding_1[8];
   hkRefPtr<hkbGenerator> m_pBlenderGenerator;
   hkbEventProperty m_EventToFreezeBlendValue;
   hkbEventProperty m_EventToCrossBlend;
   hkReal m_fBlendParameter;
   hkReal m_fTransitionDuration;
   hkEnum<hkbBlendCurveUtils::BlendCurve,hkInt8> m_eBlendCurve;
   hkRefVariant m_pTransitionBlenderGenerator;
   hkChar m_padding_2[12];
   hkRefVariant m_pTransitionEffect;
   /*enum*/ hkInt8 m_currentMode;
   hkChar m_padding_3[8];
};
extern const hkClass BSCyclicBlendTransitionGeneratorClass;


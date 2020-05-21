#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class BSOffsetAnimationGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSOffsetAnimationGenerator(void) {}
   HK_FORCE_INLINE BSOffsetAnimationGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_pDefaultGenerator(flag)
   , m_pOffsetClipGenerator(flag)
   , m_BoneOffsetA(flag)
   , m_BoneIndexA(flag)
      {}
   // Properties
   hkChar m_padding_1[8];
   hkRefPtr<hkbGenerator> m_pDefaultGenerator;
   hkChar m_padding_2[12];
   hkRefPtr<hkbGenerator> m_pOffsetClipGenerator;
   hkReal m_fOffsetVariable;
   hkReal m_fOffsetRangeStart;
   hkReal m_fOffsetRangeEnd;
   hkArray<hkRefVariant> m_BoneOffsetA;
   hkArray<hkRefVariant> m_BoneIndexA;
   hkReal m_fCurrentPercentage;
   hkUint32 m_iCurrentFrame;
   hkBool m_bZeroOffset;
   hkBool m_bOffsetValid;
   hkChar m_padding_3[14];
};
extern const hkClass BSOffsetAnimationGeneratorClass;


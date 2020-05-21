#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "hkbBlenderGeneratorChild_2.h"

class hkbBlenderGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBlenderGenerator(void) {}
   HK_FORCE_INLINE hkbBlenderGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_children(flag)
   , m_childrenInternalStates(flag)
   , m_sortedChildren(flag)
      {}
   enum BlenderFlags
   {
      FLAG_SYNC = 1,
      FLAG_SMOOTH_GENERATOR_WEIGHTS = 4,
      FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS = 8,
      FLAG_PARAMETRIC_BLEND = 16,
      FLAG_IS_PARAMETRIC_BLEND_CYCLIC = 32,
      FLAG_FORCE_DENSE_POSE = 64,
   };

   // Properties
   hkReal m_referencePoseWeightThreshold;
   hkReal m_blendParameter;
   hkReal m_minCyclicBlendParameter;
   hkReal m_maxCyclicBlendParameter;
   hkInt16 m_indexOfSyncMasterChild;
   hkInt16 m_flags;
   hkBool m_subtractLastChild;
   hkArray<hkbBlenderGeneratorChild*> m_children;
   hkArray<hkRefVariant> m_childrenInternalStates;
   hkArray<hkRefVariant> m_sortedChildren;
   hkReal m_endIntervalWeight;
   hkInt32 m_numActiveChildren;
   hkInt16 m_beginIntervalIndex;
   hkInt16 m_endIntervalIndex;
   hkBool m_initSync;
   hkBool m_doSubtractiveBlend;
};
extern const hkClass hkbBlenderGeneratorClass;


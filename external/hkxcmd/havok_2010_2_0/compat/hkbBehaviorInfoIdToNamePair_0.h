#pragma once

#include <Common/Base/hkBase.h>

struct hkbBehaviorInfoIdToNamePair
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbBehaviorInfoIdToNamePair );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbBehaviorInfoIdToNamePair(void) {}
   enum ToolNodeType
   {
      NODE_TYPE_UNKNOWN = 0,
      NODE_TYPE_STATE_MACHINE = 1,
      NODE_TYPE_CLIP = 2,
      NODE_TYPE_BLEND = 3,
      NODE_TYPE_MODIFIER = 4,
      NODE_TYPE_GENERATOR = 5,
      NODE_TYPE_MODIFIER_GENERATOR = 6,
      NODE_TYPE_TRANSITION_EFFECT = 7,
      NODE_TYPE_BEHAVIOR_FILE_REFERENCE = 8,
   };

   // Properties
   hkStringPtr m_behaviorName;
   hkStringPtr m_nodeName;
   hkEnum<ToolNodeType,hkUint8> m_toolType;
   hkInt16 m_id;
};
extern const hkClass hkbBehaviorInfoIdToNamePairClass;


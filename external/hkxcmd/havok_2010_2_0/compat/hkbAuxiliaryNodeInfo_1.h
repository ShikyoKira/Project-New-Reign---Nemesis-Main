#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbAuxiliaryNodeInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbAuxiliaryNodeInfo(void) {}
   HK_FORCE_INLINE hkbAuxiliaryNodeInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_referenceBehaviorName(flag)
   , m_selfTransitionNames(flag)
      {}
   enum NodeType
   {
      NODE_TYPE_UNKNOWN = 0,
      NODE_TYPE_NODE = 1,
      NODE_TYPE_TRANSITION = 2,
      NODE_TYPE_WILDCARD_TRANSITION = 3,
      NODE_TYPE_STATE = 4,
      NODE_TYPE_STATE_MACHINE = 5,
      NODE_TYPE_MODIFIER_GENERATOR = 6,
      NODE_TYPE_MODIFIER = 7,
      NODE_TYPE_CLIP = 8,
      NODE_TYPE_BLEND = 9,
      NODE_TYPE_TRANSITION_EFFECT = 10,
   };

   // Properties
   hkEnum<NodeType,hkUint8> m_type;
   hkUint8 m_depth;
   hkStringPtr m_referenceBehaviorName;
   hkArray<hkStringPtr> m_selfTransitionNames;
};
extern const hkClass hkbAuxiliaryNodeInfoClass;


#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbAuxiliaryNodeInfo_1.h"
#include "hkbBehaviorGraphData_2.h"
#include "hkbBehaviorGraphInternalState_0.h"
#include <Animation/Animation/Rig/hkaSkeleton.h>

class hkbClientCharacterState : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbClientCharacterState(void) {}
   HK_FORCE_INLINE hkbClientCharacterState( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_deformableSkinIds(flag)
   , m_rigidSkinIds(flag)
   , m_externalEventIds(flag)
   , m_auxiliaryInfo(flag)
   , m_activeEventIds(flag)
   , m_activeVariableIds(flag)
   , m_instanceName(flag)
   , m_templateName(flag)
   , m_fullPathToProject(flag)
   , m_behaviorData(flag)
   , m_behaviorInternalState(flag)
   , m_nodeIdToInternalStateMap(flag)
   , m_skeleton(flag)
   , m_poseModelSpace(flag)
   , m_rigidAttachmentTransforms(flag)
      {}
   // Properties
   hkArray<hkUint64> m_deformableSkinIds;
   hkArray<hkUint64> m_rigidSkinIds;
   hkArray<hkInt16> m_externalEventIds;
   hkArray<hkbAuxiliaryNodeInfo*> m_auxiliaryInfo;
   hkArray<hkInt16> m_activeEventIds;
   hkArray<hkInt16> m_activeVariableIds;
   hkUint64 m_characterId;
   hkStringPtr m_instanceName;
   hkStringPtr m_templateName;
   hkStringPtr m_fullPathToProject;
   hkRefPtr<hkbBehaviorGraphData> m_behaviorData;
   hkRefPtr<hkbBehaviorGraphInternalState> m_behaviorInternalState;
   hkRefVariant m_nodeIdToInternalStateMap;
   hkBool m_visible;
   hkReal m_elapsedSimulationTime;
   hkRefPtr<hkaSkeleton> m_skeleton;
   hkQsTransform m_worldFromModel;
   hkArray<hkQsTransform> m_poseModelSpace;
   hkArray<hkQsTransform> m_rigidAttachmentTransforms;
};
extern const hkClass hkbClientCharacterStateClass;


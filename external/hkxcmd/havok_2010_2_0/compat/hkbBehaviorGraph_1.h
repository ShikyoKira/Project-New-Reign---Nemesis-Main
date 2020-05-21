#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"
#include "hkbBehaviorGraphData_2.h"

class hkbBehaviorGraph : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorGraph(void) {}
   HK_FORCE_INLINE hkbBehaviorGraph( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_uniqueIdPool(flag)
   , m_idToStateMachineTemplateMap(flag)
   , m_mirroredExternalIdMap(flag)
   , m_pseudoRandomGenerator(flag)
   , m_rootGenerator(flag)
   , m_data(flag)
   , m_rootGeneratorClone(flag)
   , m_activeNodes(flag)
   , m_activeNodeTemplateToIndexMap(flag)
   , m_activeNodesChildrenIndices(flag)
   , m_globalTransitionData(flag)
   , m_eventIdMap(flag)
   , m_attributeIdMap(flag)
   , m_variableIdMap(flag)
   , m_characterPropertyIdMap(flag)
   , m_variableValueSet(flag)
   , m_nodeTemplateToCloneMap(flag)
   , m_nodeCloneToTemplateMap(flag)
   , m_stateListenerTemplateToCloneMap(flag)
   , m_nodePartitionInfo(flag)
   , m_jobs(flag)
   , m_allPartitionMemory(flag)
      {}
   enum VariableMode
   {
      VARIABLE_MODE_DISCARD_WHEN_INACTIVE = 0,
      VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE = 1,
   };

   // Properties
   hkEnum<VariableMode,hkInt8> m_variableMode;
   hkArray<hkRefVariant> m_uniqueIdPool;
   hkRefVariant m_idToStateMachineTemplateMap;
   hkArray<hkRefVariant> m_mirroredExternalIdMap;
   hkRefVariant m_pseudoRandomGenerator;
   hkRefPtr<hkbGenerator> m_rootGenerator;
   hkRefPtr<hkbBehaviorGraphData> m_data;
   hkRefVariant m_rootGeneratorClone;
   hkRefVariant m_activeNodes;
   hkRefVariant m_activeNodeTemplateToIndexMap;
   hkRefVariant m_activeNodesChildrenIndices;
   hkRefVariant m_globalTransitionData;
   hkRefVariant m_eventIdMap;
   hkRefVariant m_attributeIdMap;
   hkRefVariant m_variableIdMap;
   hkRefVariant m_characterPropertyIdMap;
   hkRefVariant m_variableValueSet;
   hkRefVariant m_nodeTemplateToCloneMap;
   hkRefVariant m_nodeCloneToTemplateMap;
   hkRefVariant m_stateListenerTemplateToCloneMap;
   hkRefVariant m_nodePartitionInfo;
   hkInt32 m_numIntermediateOutputs;
   hkArray<hkRefVariant> m_jobs;
   hkArray<hkRefVariant> m_allPartitionMemory;
   hkInt16 m_numStaticNodes;
   hkInt16 m_nextUniqueId;
   hkBool m_isActive;
   hkBool m_isLinked;
   hkBool m_updateActiveNodes;
   hkBool m_stateOrTransitionChanged;
};
extern const hkClass hkbBehaviorGraphClass;


#include "Havok/hkbBehaviorGraph.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorGraph,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorGraph::Class; });

nemesis::hkbBehaviorGraph::hkbBehaviorGraph() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkbGenerator* nemesis::hkbBehaviorGraph::GetRootGenerator() const
{
    return m_rootGenerator.IsNull() ? nullptr : &m_rootGenerator.GetReferencedObject();
}

const nemesis::hkbBehaviorGraphData* nemesis::hkbBehaviorGraph::GetData() const
{
    return m_data.IsNull() ? nullptr : &m_data.GetReferencedObject();
}

const nemesis::hkClass* nemesis::hkbBehaviorGraph::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorGraph::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbGenerator::SerializeTo(serializer);        // offset: 0/0 size: 40/72 align: 4/8
    serializer.WriteValue("variableMode", m_variableMode); // offset: 40/72 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                          // offset: 41/73 size: 3/7

    serializer.WriteSerializeIgnoredObject("uniqueIdPool",
                                           m_uniqueIdPool); // offset: 44/80 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "idToStateMachineTemplateMap", m_idToStateMachineTemplateMap); // offset: 56/96 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("mirroredExternalIdMap",
                                           m_mirroredExternalIdMap); // offset: 60/104 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("pseudoRandomGenerator",
                                           m_pseudoRandomGenerator); // offset: 72/120 size: 4/8 align: 4/8
    serializer.WriteObject("rootGenerator", m_rootGenerator);        // offset: 76/128 size: 4/8 align: 4/8
    serializer.WriteObject("data", m_data);                          // offset: 80/136 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("rootGeneratorClone",
                                           m_rootGeneratorClone); // offset: 84/144 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("activeNodes",
                                           m_activeNodes); // offset: 88/152 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "activeNodeTemplateToIndexMap",
        m_activeNodeTemplateToIndexMap); // offset: 92/160 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "activeNodesChildrenIndices", m_activeNodesChildrenIndices); // offset: 96/168 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("globalTransitionData",
                                           m_globalTransitionData); // offset: 100/176 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("eventIdMap",
                                           m_eventIdMap); // offset: 104/184 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("attributeIdMap",
                                           m_attributeIdMap); // offset: 108/192 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("variableIdMap",
                                           m_variableIdMap); // offset: 112/200 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("characterPropertyIdMap",
                                           m_characterPropertyIdMap); // offset: 116/208 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("variableValueSet",
                                           m_variableValueSet); // offset: 120/216 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("nodeTemplateToCloneMap",
                                           m_nodeTemplateToCloneMap); // offset: 124/224 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("nodeCloneToTemplateMap",
                                           m_nodeCloneToTemplateMap); // offset: 128/232 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "stateListenerTemplateToCloneMap",
        m_stateListenerTemplateToCloneMap); // offset: 132/240 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("nodePartitionInfo",
                                           m_nodePartitionInfo); // offset: 136/248 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("numIntermediateOutputs",
                                          m_numIntermediateOutputs); // offset: 140/256 size: 4/4 align: 4/4
    serializer.Pad(padding_size);                                    // offset: 144/260 size: 0/4

    serializer.WriteSerializeIgnoredObject("jobs", m_jobs); // offset: 144/264 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("allPartitionMemory",
                                           m_allPartitionMemory); // offset: 156/280 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("numStaticNodes",
                                          m_numStaticNodes); // offset: 168/296 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("nextUniqueId",
                                          m_nextUniqueId);         // offset: 170/298 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("isActive", m_isActive); // offset: 172/300 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("isLinked", m_isLinked); // offset: 173/301 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("updateActiveNodes",
                                          m_updateActiveNodes); // offset: 174/302 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("stateOrTransitionChanged",
                                          m_stateOrTransitionChanged); // offset: 175/303 size: 1/1 align: 1/1
    // class size: 176/304 align: 4/8
}

void nemesis::hkbBehaviorGraph::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbGenerator::DeserializeFrom(deserializer);   // offset: 0/0 size: 40/72 align: 4/8
    deserializer.ReadValue("variableMode", m_variableMode); // offset: 40/72 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                         // offset: 41/73 size: 3/7

    deserializer.ReadObject("uniqueIdPool", m_uniqueIdPool); // offset: 44/80 size: 12/16 align: 4/8
    deserializer.ReadObject("idToStateMachineTemplateMap",
                            m_idToStateMachineTemplateMap); // offset: 56/96 size: 4/8 align: 4/8
    deserializer.ReadObject("mirroredExternalIdMap",
                            m_mirroredExternalIdMap); // offset: 60/104 size: 12/16 align: 4/8
    deserializer.ReadObject("pseudoRandomGenerator",
                            m_pseudoRandomGenerator);          // offset: 72/120 size: 4/8 align: 4/8
    deserializer.ReadObject("rootGenerator", m_rootGenerator); // offset: 76/128 size: 4/8 align: 4/8
    deserializer.ReadObject("data", m_data);                   // offset: 80/136 size: 4/8 align: 4/8
    deserializer.ReadObject("rootGeneratorClone",
                            m_rootGeneratorClone);         // offset: 84/144 size: 4/8 align: 4/8
    deserializer.ReadObject("activeNodes", m_activeNodes); // offset: 88/152 size: 4/8 align: 4/8
    deserializer.ReadObject("activeNodeTemplateToIndexMap",
                            m_activeNodeTemplateToIndexMap); // offset: 92/160 size: 4/8 align: 4/8
    deserializer.ReadObject("activeNodesChildrenIndices",
                            m_activeNodesChildrenIndices); // offset: 96/168 size: 4/8 align: 4/8
    deserializer.ReadObject("globalTransitionData",
                            m_globalTransitionData);             // offset: 100/176 size: 4/8 align: 4/8
    deserializer.ReadObject("eventIdMap", m_eventIdMap);         // offset: 104/184 size: 4/8 align: 4/8
    deserializer.ReadObject("attributeIdMap", m_attributeIdMap); // offset: 108/192 size: 4/8 align: 4/8
    deserializer.ReadObject("variableIdMap", m_variableIdMap);   // offset: 112/200 size: 4/8 align: 4/8
    deserializer.ReadObject("characterPropertyIdMap",
                            m_characterPropertyIdMap);               // offset: 116/208 size: 4/8 align: 4/8
    deserializer.ReadObject("variableValueSet", m_variableValueSet); // offset: 120/216 size: 4/8 align: 4/8
    deserializer.ReadObject("nodeTemplateToCloneMap",
                            m_nodeTemplateToCloneMap); // offset: 124/224 size: 4/8 align: 4/8
    deserializer.ReadObject("nodeCloneToTemplateMap",
                            m_nodeCloneToTemplateMap); // offset: 128/232 size: 4/8 align: 4/8
    deserializer.ReadObject("stateListenerTemplateToCloneMap",
                            m_stateListenerTemplateToCloneMap);        // offset: 132/240 size: 4/8 align: 4/8
    deserializer.ReadObject("nodePartitionInfo", m_nodePartitionInfo); // offset: 136/248 size: 4/8 align: 4/8
    deserializer.ReadValue("numIntermediateOutputs",
                           m_numIntermediateOutputs); // offset: 140/256 size: 4/4 align: 4/4
    deserializer.Pad(padding_size);                   // offset: 144/260 size: 0/4

    deserializer.ReadObject("jobs", m_jobs); // offset: 144/264 size: 12/16 align: 4/8
    deserializer.ReadObject("allPartitionMemory",
                            m_allPartitionMemory);              // offset: 156/280 size: 12/16 align: 4/8
    deserializer.ReadValue("numStaticNodes", m_numStaticNodes); // offset: 168/296 size: 2/2 align: 2/2
    deserializer.ReadValue("nextUniqueId", m_nextUniqueId);     // offset: 170/298 size: 2/2 align: 2/2
    deserializer.ReadValue("isActive", m_isActive);             // offset: 172/300 size: 1/1 align: 1/1
    deserializer.ReadValue("isLinked", m_isLinked);             // offset: 173/301 size: 1/1 align: 1/1
    deserializer.ReadValue("updateActiveNodes", m_updateActiveNodes); // offset: 174/302 size: 1/1 align: 1/1
    deserializer.ReadValue("stateOrTransitionChanged",
                           m_stateOrTransitionChanged); // offset: 175/303 size: 1/1 align: 1/1
    // class size: 176/304 align: 4/8
}

#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbBehaviorGraphData.h"

namespace nemesis
{
    struct hkbBehaviorGraph : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xb1218f86,
                                                "hkbBehaviorGraph",
                                                &nemesis::hkbGenerator::Class,
                                                304,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorGraph);

        using VariableMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<VariableMode, char> m_variableMode = {
            {"VARIABLE_MODE_DISCARD_WHEN_INACTIVE", 0},
            {"VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE", 1},
        };
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_uniqueIdPool;
        nemesis::hkRefPtr<nemesis::HavokObject> m_idToStateMachineTemplateMap;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_mirroredExternalIdMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_pseudoRandomGenerator;
        nemesis::hkRefPtr<nemesis::hkbGenerator> m_rootGenerator;
        nemesis::hkRefPtr<nemesis::hkbBehaviorGraphData> m_data;
        nemesis::hkRefPtr<nemesis::HavokObject> m_rootGeneratorClone;
        nemesis::hkRefPtr<nemesis::HavokObject> m_activeNodes;
        nemesis::hkRefPtr<nemesis::HavokObject> m_activeNodeTemplateToIndexMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_activeNodesChildrenIndices;
        nemesis::hkRefPtr<nemesis::HavokObject> m_globalTransitionData;
        nemesis::hkRefPtr<nemesis::HavokObject> m_eventIdMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_attributeIdMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_variableIdMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_characterPropertyIdMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_variableValueSet;
        nemesis::hkRefPtr<nemesis::HavokObject> m_nodeTemplateToCloneMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_nodeCloneToTemplateMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_stateListenerTemplateToCloneMap;
        nemesis::hkRefPtr<nemesis::HavokObject> m_nodePartitionInfo;
        int m_numIntermediateOutputs;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_jobs;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_allPartitionMemory;
        short m_numStaticNodes;
        short m_nextUniqueId;
        bool m_isActive;
        bool m_isLinked;
        bool m_updateActiveNodes;
        bool m_stateOrTransitionChanged;

    public:
        hkbBehaviorGraph() noexcept;

        const nemesis::hkbGenerator* GetRootGenerator() const;
        const nemesis::hkbBehaviorGraphData* GetData() const;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorGraph::Class;
}

#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpEntity.h"
#include "Havok/hkpSimpleContactConstraintAtom.h"
#include "Havok/hkContactPoint.h"
#include "Havok/hkpSerializedTrack1nInfo.h"

namespace nemesis
{
    struct hkpSerializedAgentNnEntry : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x49ec7de3,
                                                "hkpSerializedAgentNnEntry",
                                                &nemesis::hkReferencedObject::Class,
                                                368,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSerializedAgentNnEntry);

        using SerializedAgentType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkRefPtr<nemesis::hkpEntity> m_bodyA;
        nemesis::hkRefPtr<nemesis::hkpEntity> m_bodyB;
        uint64_t m_bodyAId{};
        uint64_t m_bodyBId{};
        bool m_useEntityIds{};
        nemesis::hkEnum<SerializedAgentType, char> m_agentType = {
            {"INVALID_AGENT_TYPE", 0},
            {"BOX_BOX_AGENT3", 1},
            {"CAPSULE_TRIANGLE_AGENT3", 2},
            {"PRED_GSK_AGENT3", 3},
            {"PRED_GSK_CYLINDER_AGENT3", 4},
            {"CONVEX_LIST_AGENT3", 5},
            {"LIST_AGENT3", 6},
            {"BV_TREE_AGENT3", 7},
            {"COLLECTION_COLLECTION_AGENT3", 8},
            {"COLLECTION_AGENT3", 9},
        };
        nemesis::hkpSimpleContactConstraintAtom m_atom;
        nemesis::hkArray<unsigned char> m_propertiesStream;
        nemesis::hkArray<nemesis::hkContactPoint> m_contactPoints;
        nemesis::hkArray<unsigned char> m_cpIdMgr;
        std::array<unsigned char, 160> m_nnEntryData_0{};
        std::array<unsigned char, 192> m_nnEntryData_1{};
        nemesis::hkpSerializedTrack1nInfo m_trackInfo;
        std::array<unsigned char, 4> m_endianCheckBuffer{};
        unsigned int m_version{};

    public:
        hkpSerializedAgentNnEntry() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSerializedAgentNnEntry::Class;
}

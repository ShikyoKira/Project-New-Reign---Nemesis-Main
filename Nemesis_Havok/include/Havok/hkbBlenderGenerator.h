#pragma once

#include "Havok/hkbBlenderGeneratorChild.h"

namespace nemesis
{
    struct hkbBlenderGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x22df7147,
                                                "hkbBlenderGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBlenderGenerator);

        float m_referencePoseWeightThreshold{};
        float m_blendParameter{};
        float m_minCyclicBlendParameter{};
        float m_maxCyclicBlendParameter{};
        short m_indexOfSyncMasterChild{};
        short m_flags{};
        bool m_subtractLastChild{};
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbBlenderGeneratorChild>> m_children;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_childrenInternalStates;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_sortedChildren;
        float m_endIntervalWeight{};
        int m_numActiveChildren{};
        short m_beginIntervalIndex{};
        short m_endIntervalIndex{};
        bool m_initSync{};
        bool m_doSubtractiveBlend{};

    public:
        hkbBlenderGenerator() noexcept;
        hkbBlenderGenerator(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBlenderGenerator::Class;
}

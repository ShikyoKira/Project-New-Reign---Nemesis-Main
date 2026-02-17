#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct BSOffsetAnimationGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xb8571122,
                                                "BSOffsetAnimationGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSOffsetAnimationGenerator);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pDefaultGenerator;    // align 16
        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pOffsetClipGenerator; // align 16
        float m_fOffsetVariable{};
        float m_fOffsetRangeStart{};
        float m_fOffsetRangeEnd{};
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_BoneOffsetA;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_BoneIndexA;
        float m_fCurrentPercentage{};
        unsigned int m_iCurrentFrame{};
        bool m_bZeroOffset{};
        bool m_bOffsetValid{};

    public:
        BSOffsetAnimationGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSOffsetAnimationGenerator::Class;
}

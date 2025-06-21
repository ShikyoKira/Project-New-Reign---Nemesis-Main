#pragma once

#include "Havok/BSBoneSwitchGeneratorBoneData.h"

namespace nemesis
{
    struct BSBoneSwitchGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xf33d3eea,
                                                "BSBoneSwitchGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSBoneSwitchGenerator);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pDefaultGenerator; // align 16
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::BSBoneSwitchGeneratorBoneData>> m_ChildrenA;

    public:
        BSBoneSwitchGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSBoneSwitchGenerator::Class;
}

#pragma once

#include "Havok/Base/hkbGenerator.h"
#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbModifierGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x1f81fae6,
                                                "hkbModifierGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbModifierGenerator);

        nemesis::hkRefPtr<nemesis::hkbModifier> m_modifier;
        nemesis::hkRefPtr<nemesis::hkbGenerator> m_generator;

    public:
        hkbModifierGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbModifierGenerator::Class;
}

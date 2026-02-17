#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbMoveCharacterModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x8f7492a0,
                                                "hkbMoveCharacterModifier",
                                                &nemesis::hkbModifier::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbMoveCharacterModifier);

        nemesis::hkVector4 m_offsetPerSecondMS;
        float m_timeSinceLastModify{};

    public:
        hkbMoveCharacterModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbMoveCharacterModifier::Class;
}

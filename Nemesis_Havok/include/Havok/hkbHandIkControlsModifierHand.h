#pragma once

#include "Havok/hkbHandIkControlData.h"

namespace nemesis
{
    struct hkbHandIkControlsModifierHand : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9c72e9e3,
                                                "hkbHandIkControlsModifierHand",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbHandIkControlsModifierHand);

        nemesis::hkbHandIkControlData m_controlData;
        int m_handIndex{};
        bool m_enable{};

    public:
        hkbHandIkControlsModifierHand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandIkControlsModifierHand::Class;
}

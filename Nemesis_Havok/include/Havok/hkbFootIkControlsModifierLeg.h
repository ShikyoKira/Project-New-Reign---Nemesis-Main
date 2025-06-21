#pragma once

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct hkbFootIkControlsModifierLeg : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x9e17091a,
                                                "hkbFootIkControlsModifierLeg",
                                                nullptr,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbFootIkControlsModifierLeg);

        nemesis::hkVector4 m_groundPosition;
        nemesis::hkbEventProperty m_ungroundedEvent;
        float m_verticalError;
        bool m_hitSomething;
        bool m_isPlantedMS;
        bool m_enabled;

    public:
        hkbFootIkControlsModifierLeg() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbFootIkControlsModifierLeg::Class;
}

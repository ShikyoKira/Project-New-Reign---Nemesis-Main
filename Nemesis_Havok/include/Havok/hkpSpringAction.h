#pragma once

#include "Havok/Base/hkpBinaryAction.h"

namespace nemesis
{
    struct hkpSpringAction : nemesis::hkpBinaryAction
    {
        static constexpr nemesis::hkClass Class{0x88fc09fa,
                                                "hkpSpringAction",
                                                &nemesis::hkpBinaryAction::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSpringAction);

        nemesis::hkVector4 m_lastForce;
        nemesis::hkVector4 m_positionAinA;
        nemesis::hkVector4 m_positionBinB;
        float m_restLength{};
        float m_strength{};
        float m_damping{};
        bool m_onCompression{};
        bool m_onExtension{};

    public:
        hkpSpringAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSpringAction::Class;
}

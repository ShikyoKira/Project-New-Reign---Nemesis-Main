#pragma once

#include "Havok/Base/hkpUnaryAction.h"

namespace nemesis
{
    struct hkpMotorAction : nemesis::hkpUnaryAction
    {
        static constexpr nemesis::hkClass Class{0x8ff131d9,
                                                "hkpMotorAction",
                                                &nemesis::hkpUnaryAction::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMotorAction);

        nemesis::hkVector4 m_axis;
        float m_spinRate;
        float m_gain;
        bool m_active;

    public:
        hkpMotorAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMotorAction::Class;
}

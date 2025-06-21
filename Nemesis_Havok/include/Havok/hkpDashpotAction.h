#pragma once

#include "Havok/Base/hkpBinaryAction.h"

namespace nemesis
{
    struct hkpDashpotAction : nemesis::hkpBinaryAction
    {
        static constexpr nemesis::hkClass Class{0x50746c6e,
                                                "hkpDashpotAction",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpDashpotAction);

        std::array<nemesis::hkVector4, 2> m_point;
        float m_strength;
        float m_damping;
        nemesis::hkVector4 m_impulse;

    public:
        hkpDashpotAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpDashpotAction::Class;
}

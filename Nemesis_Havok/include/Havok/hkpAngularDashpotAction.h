#pragma once

#include "Havok/Base/hkpBinaryAction.h"

namespace nemesis
{
    struct hkpAngularDashpotAction : nemesis::hkpBinaryAction
    {
        static constexpr nemesis::hkClass Class{0x35f4c487,
                                                "hkpAngularDashpotAction",
                                                &nemesis::hkpBinaryAction::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpAngularDashpotAction);

        nemesis::hkQuaternion m_rotation;
        float m_strength;
        float m_damping;

    public:
        hkpAngularDashpotAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAngularDashpotAction::Class;
}

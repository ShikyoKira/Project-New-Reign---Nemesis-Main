#pragma once

#include "Havok/Base/hkpUnaryAction.h"

namespace nemesis
{
    struct hkpReorientAction : nemesis::hkpUnaryAction
    {
        static constexpr nemesis::hkClass Class{0x2dc0ec6a,
                                                "hkpReorientAction",
                                                &nemesis::hkpUnaryAction::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpReorientAction);

        nemesis::hkVector4 m_rotationAxis;
        nemesis::hkVector4 m_upAxis;
        float m_strength;
        float m_damping;

    public:
        hkpReorientAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpReorientAction::Class;
}

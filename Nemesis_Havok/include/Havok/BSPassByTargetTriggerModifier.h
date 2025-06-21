#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSPassByTargetTriggerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x703d7b66,
                                                "BSPassByTargetTriggerModifier",
                                                &nemesis::hkbModifier::Class,
                                                160,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSPassByTargetTriggerModifier);

        nemesis::hkVector4 m_targetPosition;
        float m_radius;
        nemesis::hkVector4 m_movementDirection;
        nemesis::hkbEventProperty m_triggerEvent;
        bool m_targetPassed;

    public:
        BSPassByTargetTriggerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSPassByTargetTriggerModifier::Class;
}

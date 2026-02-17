#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSDistTriggerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xb34d2bbd,
                                                "BSDistTriggerModifier",
                                                &nemesis::hkbModifier::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(BSDistTriggerModifier);

        nemesis::hkVector4 m_targetPosition;
        float m_distance{};
        float m_distanceTrigger{};
        nemesis::hkbEventProperty m_triggerEvent;

    public:
        BSDistTriggerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSDistTriggerModifier::Class;
}

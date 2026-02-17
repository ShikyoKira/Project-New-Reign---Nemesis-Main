#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSTweenerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xd2d9a04,
                                                "BSTweenerModifier",
                                                &nemesis::hkbModifier::Class,
                                                208,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSTweenerModifier);

        bool m_tweenPosition{};
        bool m_tweenRotation{};
        bool m_useTweenDuration{};
        float m_tweenDuration{};
        nemesis::hkVector4 m_targetPosition;
        nemesis::hkQuaternion m_targetRotation;
        float m_duration{};
        nemesis::hkQsTransform m_startTransform;
        float m_time{};

    public:
        BSTweenerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSTweenerModifier::Class;
}

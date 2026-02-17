#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbComputeDirectionModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xdf358bd3,
                                                "hkbComputeDirectionModifier",
                                                &nemesis::hkbModifier::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbComputeDirectionModifier);

        nemesis::hkVector4 m_pointIn;
        nemesis::hkVector4 m_pointOut;
        float m_groundAngleOut{};
        float m_upAngleOut{};
        float m_verticalOffset{};
        bool m_reverseGroundAngle{};
        bool m_reverseUpAngle{};
        bool m_projectPoint{};
        bool m_normalizePoint{};
        bool m_computeOnlyOnce{};
        bool m_computedOutput{};

    public:
        hkbComputeDirectionModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbComputeDirectionModifier::Class;
}

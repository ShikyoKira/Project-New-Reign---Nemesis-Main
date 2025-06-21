#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbComputeRotationFromAxisAngleModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x9b3f6936,
                                                "hkbComputeRotationFromAxisAngleModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbComputeRotationFromAxisAngleModifier);

        nemesis::hkQuaternion m_rotationOut;
        nemesis::hkVector4 m_axis;
        float m_angleDegrees;

    public:
        hkbComputeRotationFromAxisAngleModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbComputeRotationFromAxisAngleModifier::Class;
}

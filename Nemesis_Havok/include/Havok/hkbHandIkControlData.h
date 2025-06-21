#pragma once

#include "Havok/hkbHandle.h"

namespace nemesis
{
    struct hkbHandIkControlData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xd72b8d17,
                                                "hkbHandIkControlData",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbHandIkControlData);

        using HandleChangeMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkVector4 m_targetPosition;
        nemesis::hkQuaternion m_targetRotation;
        nemesis::hkVector4 m_targetNormal;
        nemesis::hkRefPtr<nemesis::hkbHandle> m_targetHandle;
        float m_transformOnFraction;
        float m_normalOnFraction;
        float m_fadeInDuration;
        float m_fadeOutDuration;
        float m_extrapolationTimeStep;
        float m_handleChangeSpeed;
        nemesis::hkEnum<HandleChangeMode, char> m_handleChangeMode = {
            {"HANDLE_CHANGE_MODE_ABRUPT", 0},
            {"HANDLE_CHANGE_MODE_CONSTANT_VELOCITY", 1},
        };
        bool m_fixUp;

    public:
        hkbHandIkControlData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandIkControlData::Class;
}

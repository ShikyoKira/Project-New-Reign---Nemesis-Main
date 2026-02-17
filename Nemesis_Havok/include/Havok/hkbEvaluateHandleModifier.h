#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbHandle.h"

namespace nemesis
{
    struct hkbEvaluateHandleModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x79757102,
                                                "hkbEvaluateHandleModifier",
                                                &nemesis::hkbModifier::Class,
                                                240,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEvaluateHandleModifier);

        using HandleChangeMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkRefPtr<nemesis::hkbHandle> m_handle;
        nemesis::hkVector4 m_handlePositionOut;
        nemesis::hkQuaternion m_handleRotationOut;
        bool m_isValidOut{};
        float m_extrapolationTimeStep{};
        float m_handleChangeSpeed{};
        nemesis::hkEnum<HandleChangeMode, char> m_handleChangeMode = {
            {"HANDLE_CHANGE_MODE_ABRUPT", 0},
            {"HANDLE_CHANGE_MODE_CONSTANT_VELOCITY", 1},
        };
        nemesis::hkbHandle m_oldHandle;
        nemesis::hkVector4 m_oldHandlePosition;
        nemesis::hkQuaternion m_oldHandleRotation;
        float m_timeSinceLastModify{};
        bool m_smoothlyChangingHandles{};

    public:
        hkbEvaluateHandleModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEvaluateHandleModifier::Class;
}

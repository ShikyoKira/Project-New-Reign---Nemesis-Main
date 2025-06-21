#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbCharacterControllerControlData.h"
#include "Havok/hkbCharacterControllerModifierControlData.h"

namespace nemesis
{
    struct hkbCharacterControllerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xf675d6fb,
                                                "hkbCharacterControllerModifier",
                                                &nemesis::hkbModifier::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterControllerModifier);

        using InitialVelocityCoordinates = nemesis::hkInternalClassEnumItem<char>;
        using MotionMode                 = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkbCharacterControllerControlData m_controlData_0;
        nemesis::hkbCharacterControllerModifierControlData m_controlData_1;
        nemesis::hkVector4 m_initialVelocity;
        nemesis::hkEnum<InitialVelocityCoordinates, char> m_initialVelocityCoordinates = {
            {"INITIAL_VELOCITY_IN_WORLD_COORDINATES", 0},
            {"INITIAL_VELOCITY_IN_MODEL_COORDINATES", 1},
        };
        nemesis::hkEnum<MotionMode, char> m_motionMode = {
            {"MOTION_MODE_FOLLOW_ANIMATION", 0},
            {"MOTION_MODE_DYNAMIC", 1},
        };
        bool m_forceDownwardMomentum;
        bool m_applyGravity;
        bool m_setInitialVelocity;
        bool m_isTouchingGround;
        nemesis::hkVector4 m_gravity;
        float m_timestep;
        bool m_isInitialVelocityAdded;

    public:
        hkbCharacterControllerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterControllerModifier::Class;
}

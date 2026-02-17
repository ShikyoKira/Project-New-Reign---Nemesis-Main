#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/BSLookAtModifierBoneData.h"
#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSLookAtModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xd756fc25,
                                                "BSLookAtModifier",
                                                &nemesis::hkbModifier::Class,
                                                224,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSLookAtModifier);

        bool m_lookAtTarget{};
        nemesis::hkArray<nemesis::BSLookAtModifierBoneData> m_bones;
        nemesis::hkArray<nemesis::BSLookAtModifierBoneData> m_eyeBones;
        float m_limitAngleDegrees{};
        float m_limitAngleThresholdDegrees{};
        bool m_continueLookOutsideOfLimit{};
        float m_onGain{};
        float m_offGain{};
        bool m_useBoneGains{};
        nemesis::hkVector4 m_targetLocation;
        bool m_targetOutsideLimits{};
        nemesis::hkbEventProperty m_targetOutOfLimitEvent;
        bool m_lookAtCamera{};
        float m_lookAtCameraX{};
        float m_lookAtCameraY{};
        float m_lookAtCameraZ{};
        float m_timeStep{};
        bool m_ballBonesValid{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_pSkeletonMemory;

    public:
        BSLookAtModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSLookAtModifier::Class;
}

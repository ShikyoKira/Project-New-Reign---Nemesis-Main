#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbProxyModifierProxyInfo.h"

namespace nemesis
{
    struct hkbProxyModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x8a41554f,
                                                "hkbProxyModifier",
                                                &nemesis::hkbModifier::Class,
                                                288,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbProxyModifier);

        using PhantomType = nemesis::hkInternalClassEnumItem<char>;
        using LinearVelocityMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkbProxyModifierProxyInfo m_proxyInfo;
        nemesis::hkVector4 m_linearVelocity;
        float m_horizontalGain{};
        float m_verticalGain{};
        float m_maxHorizontalSeparation{};
        float m_maxVerticalSeparation{};
        float m_verticalDisplacementError{};
        float m_verticalDisplacementErrorGain{};
        float m_maxVerticalDisplacement{};
        float m_minVerticalDisplacement{};
        float m_capsuleHeight{};
        float m_capsuleRadius{};
        float m_maxSlopeForRotation{};
        unsigned int m_collisionFilterInfo{};
        nemesis::hkEnum<PhantomType, char> m_phantomType = {
            {"PHANTOM_TYPE_SIMPLE", 0},
            {"PHANTOM_TYPE_CACHING", 1},
        };
        nemesis::hkEnum<LinearVelocityMode, char> m_linearVelocityMode = {
            {"LINEAR_VELOCITY_MODE_WORLD", 0},
            {"LINEAR_VELOCITY_MODE_MODEL", 1},
        };
        bool m_ignoreIncomingRotation{};
        bool m_ignoreCollisionDuringRotation{};
        bool m_ignoreIncomingTranslation{};
        bool m_includeDownwardMomentum{};
        bool m_followWorldFromModel{};
        bool m_isTouchingGround{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_characterProxy;
        nemesis::hkRefPtr<nemesis::HavokObject> m_phantom;
        nemesis::hkRefPtr<nemesis::HavokObject> m_phantomShape;
        nemesis::hkVector4 m_horizontalDisplacement;
        float m_verticalDisplacement{};
        float m_timestep{};
        bool m_previousFrameFollowWorldFromModel{};

    public:
        hkbProxyModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbProxyModifier::Class;
}

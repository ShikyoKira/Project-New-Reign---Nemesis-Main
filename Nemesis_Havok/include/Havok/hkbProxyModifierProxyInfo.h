#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbProxyModifierProxyInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x39de637e,
                                                "hkbProxyModifierProxyInfo",
                                                nullptr,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbProxyModifierProxyInfo);

        float m_dynamicFriction{};
        float m_staticFriction{};
        float m_keepContactTolerance{};
        nemesis::hkVector4 m_up;
        float m_keepDistance{};
        float m_contactAngleSensitivity{};
        unsigned int m_userPlanes{};
        float m_maxCharacterSpeedForSolver{};
        float m_characterStrength{};
        float m_characterMass{};
        float m_maxSlope{};
        float m_penetrationRecoverySpeed{};
        int m_maxCastIterations{};
        bool m_refreshManifoldInCheckSupport{};

    public:
        hkbProxyModifierProxyInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbProxyModifierProxyInfo::Class;
}

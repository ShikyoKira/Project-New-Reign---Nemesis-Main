#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbGetUpModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x61cb7ac0,
                                                "hkbGetUpModifier",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbGetUpModifier);

        nemesis::hkVector4 m_groundNormal;
        float m_duration;
        float m_alignWithGroundDuration;
        short m_rootBoneIndex;
        short m_otherBoneIndex;
        short m_anotherBoneIndex;
        float m_timeSinceBegin;
        float m_timeStep;
        bool m_initNextModify;

    public:
        hkbGetUpModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGetUpModifier::Class;
}

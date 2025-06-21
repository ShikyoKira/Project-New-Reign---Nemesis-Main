#pragma once

#include "Havok/hkbBlenderGenerator.h"

namespace nemesis
{
    struct hkbPoseMatchingGenerator : nemesis::hkbBlenderGenerator
    {
        static constexpr nemesis::hkClass Class{0x29e271b4,
                                                "hkbPoseMatchingGenerator",
                                                &nemesis::hkbBlenderGenerator::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbPoseMatchingGenerator);

        using Mode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkQuaternion m_worldFromModelRotation;
        float m_blendSpeed;
        float m_minSpeedToSwitch;
        float m_minSwitchTimeNoError;
        float m_minSwitchTimeFullError;
        int m_startPlayingEventId;
        int m_startMatchingEventId;
        short m_rootBoneIndex;
        short m_otherBoneIndex;
        short m_anotherBoneIndex;
        short m_pelvisIndex;
        nemesis::hkEnum<Mode, char> m_mode = {
            {"MODE_MATCH", 0},
            {"MODE_PLAY", 1},
        };
        int m_currentMatch;
        int m_bestMatch;
        float m_timeSinceBetterMatch;
        float m_error;
        bool m_resetCurrentMatchLocalTime;
        nemesis::hkRefPtr<nemesis::HavokObject> m_poseMatchingUtility;

    public:
        hkbPoseMatchingGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbPoseMatchingGenerator::Class;
}

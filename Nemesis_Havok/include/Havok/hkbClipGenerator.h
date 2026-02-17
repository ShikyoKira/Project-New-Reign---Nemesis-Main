#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbClipTriggerArray.h"

namespace nemesis
{
    struct hkbClipGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x333b85b9,
                                                "hkbClipGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                272,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbClipGenerator);

        using PlaybackMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkStringPtr m_animationBundleName;
        nemesis::hkStringPtr m_animationName;
        nemesis::hkRefPtr<nemesis::hkbClipTriggerArray> m_triggers;
        unsigned int m_userPartitionMask{};
        float m_cropStartAmountLocalTime{};
        float m_cropEndAmountLocalTime{};
        float m_startTime{};
        float m_playbackSpeed{};
        float m_enforcedDuration{};
        float m_userControlledTimeFraction{};
        short m_animationBindingIndex{};
        nemesis::hkEnum<PlaybackMode, char> m_mode = {
            {"MODE_SINGLE_PLAY", 0},
            {"MODE_LOOPING", 1},
            {"MODE_USER_CONTROLLED", 2},
            {"MODE_PING_PONG", 3},
            {"MODE_COUNT", 4},
        };
        char m_flags{};
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_animDatas;
        nemesis::hkRefPtr<nemesis::HavokObject> m_animationControl;
        nemesis::hkRefPtr<nemesis::HavokObject> m_originalTriggers;
        nemesis::hkRefPtr<nemesis::HavokObject> m_mapperData;
        nemesis::hkRefPtr<nemesis::HavokObject> m_binding;
        nemesis::hkRefPtr<nemesis::HavokObject> m_mirroredAnimation;
        nemesis::hkQsTransform m_extractedMotion;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_echos;
        float m_localTime{};
        float m_time{};
        float m_previousUserControlledTimeFraction{};
        int m_bufferSize{};
        int m_echoBufferSize{};
        bool m_atEnd{};
        bool m_ignoreStartTime{};
        bool m_pingPongBackward{};

    public:
        hkbClipGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbClipGenerator::Class;
}

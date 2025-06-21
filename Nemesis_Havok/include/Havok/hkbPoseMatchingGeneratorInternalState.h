#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbPoseMatchingGeneratorInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x552d9dd4,
                                                "hkbPoseMatchingGeneratorInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                1};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbPoseMatchingGeneratorInternalState);

        int m_currentMatch;
        int m_bestMatch;
        float m_timeSinceBetterMatch;
        float m_error;
        bool m_resetCurrentMatchLocalTime;

    public:
        hkbPoseMatchingGeneratorInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbPoseMatchingGeneratorInternalState::Class;
}

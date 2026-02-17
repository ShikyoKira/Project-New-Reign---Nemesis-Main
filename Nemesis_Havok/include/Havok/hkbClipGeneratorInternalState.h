#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbClipGeneratorEcho.h"

namespace nemesis
{
    struct hkbClipGeneratorInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x26ce5bf3,
                                                "hkbClipGeneratorInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbClipGeneratorInternalState);

        nemesis::hkQsTransform m_extractedMotion;
        nemesis::hkArray<nemesis::hkbClipGeneratorEcho> m_echos;
        float m_localTime{};
        float m_time{};
        float m_previousUserControlledTimeFraction{};
        int m_bufferSize{};
        int m_echoBufferSize{};
        bool m_atEnd{};
        bool m_ignoreStartTime{};
        bool m_pingPongBackward{};

    public:
        hkbClipGeneratorInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbClipGeneratorInternalState::Class;
}

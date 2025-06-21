#pragma once

#include "Havok/Base/hkaAnimatedReferenceFrame.h"

namespace nemesis
{
    struct hkaDefaultAnimatedReferenceFrame : nemesis::hkaAnimatedReferenceFrame
    {
        static constexpr nemesis::hkClass Class{0x6d85e445,
                                                "hkaDefaultAnimatedReferenceFrame",
                                                &nemesis::hkaAnimatedReferenceFrame::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkaDefaultAnimatedReferenceFrame);

        nemesis::hkVector4 m_up;
        nemesis::hkVector4 m_forward;
        float m_duration;
        nemesis::hkArray<nemesis::hkVector4> m_referenceFrameSamples;

    public:
        hkaDefaultAnimatedReferenceFrame() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaDefaultAnimatedReferenceFrame::Class;
}

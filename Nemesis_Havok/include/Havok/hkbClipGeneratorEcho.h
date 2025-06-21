#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbClipGeneratorEcho : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x750edf40,
                                                "hkbClipGeneratorEcho",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbClipGeneratorEcho);

        float m_offsetLocalTime; // align 16
        float m_weight;
        float m_dwdt;

    public:
        hkbClipGeneratorEcho() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbClipGeneratorEcho::Class;
}

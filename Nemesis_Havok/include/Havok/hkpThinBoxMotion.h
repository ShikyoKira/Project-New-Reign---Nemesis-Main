#pragma once

#include "Havok/hkpBoxMotion.h"

namespace nemesis
{
    struct hkpThinBoxMotion : nemesis::hkpBoxMotion
    {
        static constexpr nemesis::hkClass Class{0x64abf85c,
                                                "hkpThinBoxMotion",
                                                &nemesis::hkpBoxMotion::Class,
                                                320,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpThinBoxMotion);

    public:
        hkpThinBoxMotion() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpThinBoxMotion::Class;
}

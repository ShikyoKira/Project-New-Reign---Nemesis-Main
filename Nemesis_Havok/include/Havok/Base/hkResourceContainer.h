#pragma once

#include "Havok/Base/hkResourceBase.h"

namespace nemesis
{
    struct hkResourceContainer : nemesis::hkResourceBase
    {
        static constexpr nemesis::hkClass Class{0x4e94146,
                                                "hkResourceContainer",
                                                &nemesis::hkResourceBase::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    public:
        hkResourceContainer(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkResourceContainer::Class;
}

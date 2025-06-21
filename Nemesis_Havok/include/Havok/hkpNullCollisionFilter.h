#pragma once

#include "Havok/Base/hkpCollisionFilter.h"

namespace nemesis
{
    struct hkpNullCollisionFilter : nemesis::hkpCollisionFilter
    {
        static constexpr nemesis::hkClass Class{0xb120a34f,
                                                "hkpNullCollisionFilter",
                                                &nemesis::hkpCollisionFilter::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpNullCollisionFilter);

    public:
        hkpNullCollisionFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpNullCollisionFilter::Class;
}

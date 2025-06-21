#pragma once

#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpHeightFieldShape : nemesis::hkpShape
    {
        static constexpr nemesis::hkClass Class{0xe7eca7eb,
                                                "hkpHeightFieldShape",
                                                &nemesis::hkpShape::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    public:
        hkpHeightFieldShape(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpHeightFieldShape::Class;
}

#pragma once

#include "Havok/Base/hkcdShape.h"

namespace nemesis
{
    struct hkpShapeBase : nemesis::hkcdShape
    {
        static constexpr nemesis::hkClass Class{0,
                                                "hkpShapeBase",
                                                &nemesis::hkcdShape::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    public:
        hkpShapeBase(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpShapeBase::Class;
}

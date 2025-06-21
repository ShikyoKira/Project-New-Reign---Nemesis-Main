#pragma once

#include "Havok/Base/hkpConstraintData.h"

namespace nemesis
{
    struct hkpConstraintChainData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x5facc7ff,
                                                "hkpConstraintChainData",
                                                &nemesis::hkpConstraintData::Class,
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
        hkpConstraintChainData(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintChainData::Class;
}

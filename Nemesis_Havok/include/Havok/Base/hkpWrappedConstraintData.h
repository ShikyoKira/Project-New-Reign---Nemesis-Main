#pragma once

#include "Havok/Base/hkpConstraintData.h"

namespace nemesis
{
    struct hkpWrappedConstraintData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkpWrappedConstraintData",
                                                &nemesis::hkpConstraintData::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkRefPtr<nemesis::hkpConstraintData> m_constraintData;

    public:
        hkpWrappedConstraintData(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWrappedConstraintData::Class;
}

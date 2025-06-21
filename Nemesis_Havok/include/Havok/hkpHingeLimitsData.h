#pragma once

#include "Havok/Base/hkpConstraintData.h"

#include "Havok/hkpHingeLimitsDataAtoms.h"

namespace nemesis
{
    struct hkpHingeLimitsData : nemesis::hkpConstraintData
    {
        static constexpr nemesis::hkClass Class{0xbd46760a,
                                                "hkpHingeLimitsData",
                                                &nemesis::hkpConstraintData::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpHingeLimitsData);

        nemesis::hkpHingeLimitsDataAtoms m_atoms; // align 16

    public:
        hkpHingeLimitsData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpHingeLimitsData::Class;
}

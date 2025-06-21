#pragma once

#include "Havok/hkpConstraintListener.h"
#include "Havok/hkpPairCollisionFilter.h"

namespace nemesis
{
    struct hkpConstraintCollisionFilter : nemesis::hkpPairCollisionFilter, nemesis::hkpConstraintListener
    {
        static constexpr nemesis::hkClass Class{0xc3b577b1,
                                                "hkpConstraintCollisionFilter",
                                                &nemesis::hkpPairCollisionFilter::Class,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER_2(hkpConstraintCollisionFilter, hkReferencedObject);

    public:
        hkpConstraintCollisionFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintCollisionFilter::Class;
}

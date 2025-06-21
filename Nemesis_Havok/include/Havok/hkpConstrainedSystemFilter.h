#pragma once

#include "Havok/Base/hkpCollisionFilter.h"

#include "Havok/hkpConstraintListener.h"

namespace nemesis
{
    struct hkpConstrainedSystemFilter : nemesis::hkpCollisionFilter, nemesis::hkpConstraintListener
    {
        static constexpr nemesis::hkClass Class{0x20a447fe,
                                                "hkpConstrainedSystemFilter",
                                                &nemesis::hkpCollisionFilter::Class,
                                                88,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER_2(hkpConstrainedSystemFilter, hkReferencedObject);

        nemesis::hkRefPtr<nemesis::hkpCollisionFilter> m_otherFilter;

    public:
        hkpConstrainedSystemFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstrainedSystemFilter::Class;
}

#pragma once

#include "Havok/Base/hkpCollisionFilter.h"

#include "Havok/hkpPairCollisionFilterMapPairFilterKeyOverrideType.h"

namespace nemesis
{
    struct hkpPairCollisionFilter : nemesis::hkpCollisionFilter
    {
        static constexpr nemesis::hkClass Class{0x4abc140e,
                                                "hkpPairCollisionFilter",
                                                &nemesis::hkpCollisionFilter::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPairCollisionFilter);

        nemesis::hkpPairCollisionFilterMapPairFilterKeyOverrideType m_disabledPairs;
        nemesis::hkRefPtr<nemesis::hkpCollisionFilter> m_childFilter;

    public:
        hkpPairCollisionFilter() noexcept;
        hkpPairCollisionFilter(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPairCollisionFilter::Class;
}

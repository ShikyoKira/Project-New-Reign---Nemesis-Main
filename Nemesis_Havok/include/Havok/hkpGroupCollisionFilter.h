#pragma once

#include "Havok/Base/hkpCollisionFilter.h"

namespace nemesis
{
    struct hkpGroupCollisionFilter : nemesis::hkpCollisionFilter
    {
        static constexpr nemesis::hkClass Class{0x5cc01561,
                                                "hkpGroupCollisionFilter",
                                                &nemesis::hkpCollisionFilter::Class,
                                                208,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpGroupCollisionFilter);

        bool m_noGroupCollisionEnabled{};
        std::array<unsigned int, 32> m_collisionGroups{};

    public:
        hkpGroupCollisionFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpGroupCollisionFilter::Class;
}

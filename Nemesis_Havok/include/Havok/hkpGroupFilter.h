#pragma once

#include "Havok/Base/hkpCollisionFilter.h"

namespace nemesis
{
    struct hkpGroupFilter : nemesis::hkpCollisionFilter
    {
        static constexpr nemesis::hkClass Class{0x65ee88e4,
                                                "hkpGroupFilter",
                                                &nemesis::hkpCollisionFilter::Class,
                                                272,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpGroupFilter);

        int m_nextFreeSystemGroup{};
        std::array<unsigned int, 32> m_collisionLookupTable{};
        std::array<nemesis::hkVector4, 4> m_pad256;

    public:
        hkpGroupFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpGroupFilter::Class;
}

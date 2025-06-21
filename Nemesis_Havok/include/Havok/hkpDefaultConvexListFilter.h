#pragma once

#include "Havok/Base/hkpConvexListFilter.h"

namespace nemesis
{
    struct hkpDefaultConvexListFilter : nemesis::hkpConvexListFilter
    {
        static constexpr nemesis::hkClass Class{0xb69c1c02,
                                                "hkpDefaultConvexListFilter",
                                                &nemesis::hkpConvexListFilter::Class,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpDefaultConvexListFilter);

    public:
        hkpDefaultConvexListFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpDefaultConvexListFilter::Class;
}

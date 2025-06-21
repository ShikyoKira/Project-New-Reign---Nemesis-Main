#pragma once

#include "Havok/Base/hkReferencedObject.h"
#include "Havok/Base/hkpCollidableCollidableFilter.h"
#include "Havok/Base/hkpRayCollidableFilter.h"
#include "Havok/Base/hkpRayShapeCollectionFilter.h"
#include "Havok/Base/hkpShapeCollectionFilter.h"

namespace nemesis
{
    struct hkpCollisionFilter : nemesis::hkReferencedObject,
                                nemesis::hkpCollidableCollidableFilter,
                                nemesis::hkpShapeCollectionFilter,
                                nemesis::hkpRayShapeCollectionFilter,
                                nemesis::hkpRayCollidableFilter
    {
        static constexpr nemesis::hkClass Class{0x60960336,
                                                "hkpCollisionFilter",
                                                &nemesis::hkReferencedObject::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCollisionFilter);

        using hkpFilterType = nemesis::hkInternalClassEnumItem<unsigned int>;

        std::array<unsigned int, 2> m_prepad;
        nemesis::hkEnum<hkpFilterType, unsigned int> m_type = {
            {"HK_FILTER_UNKNOWN", 0},
            {"HK_FILTER_NULL", 1},
            {"HK_FILTER_GROUP", 2},
            {"HK_FILTER_LIST", 3},
            {"HK_FILTER_CUSTOM", 4},
            {"HK_FILTER_PAIR", 5},
            {"HK_FILTER_CONSTRAINT", 6},
        };
        std::array<unsigned int, 3> m_postpad;

    public:
        hkpCollisionFilter() noexcept;
        hkpCollisionFilter(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCollisionFilter::Class;
}

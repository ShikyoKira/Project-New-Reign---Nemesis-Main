#pragma once

#include "Havok/Base/hkpCollisionFilter.h"
#include "Havok/Base/hkpEntityListener.h"

#include "Havok/hkpEntity.h"

namespace nemesis
{
    struct hkpDisableEntityCollisionFilter : nemesis::hkpCollisionFilter, nemesis::hkpEntityListener
    {
        static constexpr nemesis::hkClass Class{0xfac3351c,
                                                "hkpDisableEntityCollisionFilter",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpDisableEntityCollisionFilter);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpEntity>> m_disabledEntities;

    public:
        hkpDisableEntityCollisionFilter() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpDisableEntityCollisionFilter::Class;
}

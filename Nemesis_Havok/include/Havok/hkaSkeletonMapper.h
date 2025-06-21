#pragma once

#include "Havok/hkaSkeletonMapperData.h"

namespace nemesis
{
    struct hkaSkeletonMapper : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x12df42a5,
                                                "hkaSkeletonMapper",
                                                &nemesis::hkReferencedObject::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonMapper);

        nemesis::hkaSkeletonMapperData m_mapping;

    public:
        hkaSkeletonMapper() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonMapper::Class;
}

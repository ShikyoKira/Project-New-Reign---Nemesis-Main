#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaSkeletonMapperDataPartitionMappingRange : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkaSkeletonMapperDataPartitionMappingRange",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonMapperDataPartitionMappingRange);

        int m_startMappingIndex;
        int m_numMappings;

    public:
        hkaSkeletonMapperDataPartitionMappingRange() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonMapperDataPartitionMappingRange::Class;
}

#pragma once

#include "Havok/hkaSkeleton.h"
#include "Havok/hkaSkeletonMapperDataChainMapping.h"
#include "Havok/hkaSkeletonMapperDataPartitionMappingRange.h"
#include "Havok/hkaSkeletonMapperDataSimpleMapping.h"

namespace nemesis
{
    struct hkaSkeletonMapperData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x95687ea0,
                                                "hkaSkeletonMapperData",
                                                nullptr,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonMapperData);

        using MappingType = nemesis::hkInternalClassEnumItem<int>;

        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_skeletonA;
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_skeletonB;
        nemesis::hkArray<short> m_partitionMap;
        nemesis::hkArray<nemesis::hkaSkeletonMapperDataPartitionMappingRange> m_simpleMappingPartitionRanges;
        nemesis::hkArray<nemesis::hkaSkeletonMapperDataPartitionMappingRange> m_chainMappingPartitionRanges;
        nemesis::hkArray<nemesis::hkaSkeletonMapperDataSimpleMapping> m_simpleMappings;
        nemesis::hkArray<nemesis::hkaSkeletonMapperDataChainMapping> m_chainMappings;
        nemesis::hkArray<short> m_unmappedBones;
        nemesis::hkQsTransform m_extractedMotionMapping;
        bool m_keepUnmappedLocal{};
        nemesis::hkEnum<MappingType, int> m_mappingType = {
            {"HK_RAGDOLL_MAPPING", 0},
            {"HK_RETARGETING_MAPPING", 1},
        };

    public:
        hkaSkeletonMapperData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonMapperData::Class;
}

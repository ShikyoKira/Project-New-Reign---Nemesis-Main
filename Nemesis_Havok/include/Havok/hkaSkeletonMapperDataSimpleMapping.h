#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaSkeletonMapperDataSimpleMapping : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x3405deca,
                                                "hkaSkeletonMapperDataSimpleMapping",
                                                nullptr,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonMapperDataSimpleMapping);

        short m_boneA{};
        short m_boneB{};
        nemesis::hkQsTransform m_aFromBTransform;

    public:
        hkaSkeletonMapperDataSimpleMapping() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonMapperDataSimpleMapping::Class;
}

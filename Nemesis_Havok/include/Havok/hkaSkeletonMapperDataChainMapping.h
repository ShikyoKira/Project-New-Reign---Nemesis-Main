#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaSkeletonMapperDataChainMapping : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa528f7cf,
                                                "hkaSkeletonMapperDataChainMapping",
                                                nullptr,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonMapperDataChainMapping);

        short m_startBoneA{};
        short m_endBoneA{};
        short m_startBoneB{};
        short m_endBoneB{};
        nemesis::hkQsTransform m_startAFromBTransform;
        nemesis::hkQsTransform m_endAFromBTransform;

    public:
        hkaSkeletonMapperDataChainMapping() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonMapperDataChainMapping::Class;
}

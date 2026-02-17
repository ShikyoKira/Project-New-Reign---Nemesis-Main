#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkaSkeletonPartition : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x00,
                                                "hkaSkeletonPartition",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeletonPartition);

        nemesis::hkStringPtr m_name;
        short m_startBoneIndex{};
        short m_numBones{};

    public:
        hkaSkeletonPartition() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeletonPartition::Class;
}
